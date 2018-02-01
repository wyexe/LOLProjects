#include "VerificationSystem.h"
#include <algorithm>
#define _SELF L"SvrVS.cpp"

vector<Server_GroupMember> CVerificationSystem::GroupLst;
vector<SockClientGroup> CVerificationSystem::WaitGroupLst;
vector<SockKeepConSrc> CVerificationSystem::SocketConectLst;

vector<ServerAccountInfo> AccountList;
CLLock AccountListLock;

CVerificationSystem::CVerificationSystem()
{

}
CVerificationSystem::~CVerificationSystem()
{

}

BOOL CVerificationSystem::InitCardInfo()
{
	vector<CardInfo> vlst;
	CDataBaseMdel::GetCardInfo(vlst);

	CPrintLog::PrintLog_W(_SELF, __LINE__, L"数据库有效卡号.size()=%d", vlst.size());
	CLoginManger::_InitCard(vlst);
	return TRUE;
}

BOOL CVerificationSystem::RechargeLoginUser(SockLoginData& SLD, LPCWSTR pwszIp, __out ByteBuffer& RetBf)
{
	/*if (wcslen(SLD.Card.szCard) > (sizeof(SLD.Card.szCard) / sizeof(WCHAR)))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"RechargeLoginUser.无法接收到正确的卡号!");
		RetBf << SOCKET_MESSAGE_RETMSG << L"无法接收到有效的卡号!";
		return TRUE;
	}

	DWORD dwCardHour = CLoginManger::_CheckCard(SLD.Card.szCard);
	if (dwCardHour == 0)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"RechargeLoginUser.卡号有误!!");
		RetBf << SOCKET_MESSAGE_RETMSG << L"无法接收到有效的卡号!卡号有误!";
		return TRUE;
	}

	// Init Card Lst
	WCHAR wszSQL[128] = { 0 };
	wsprintfW(wszSQL, L"exec [RechargeCard] '%s', '%s', %d, '%s'", SLD.Card.szCard, SLD.wszLoginUser, dwCardHour, pwszIp);
	WCHAR wszRetMsg[64] = { 0 };
	if (!CDataBaseMdel::ExcuteSQL(wszSQL, wszRetMsg))
	{
		RetBf << SOCKET_MESSAGE_RETMSG << L"无法连接到服务器的数据库~!联系本人~!错误:0x6";
		return TRUE;
	}

	if (CCharacter::wstrstr_my(wszRetMsg, L"成功"))
	{
		DWORD dwTime = CLoginManger::_CheckCard(SLD.Card.szCard);
		DWORD dwToken = CCharacter::GetRand_For_DWORD();
		CLoginManger::AddNewCard(SLD.Card, dwTime, dwToken, pwszIp);
		wsprintfW(wszRetMsg, L"正常->%X", dwToken);
		RetBf << SOCKET_MESSAGE_RECHARGELOGINUSER << dwTime << wszRetMsg;
		return TRUE;
	}

	LoginGameUser* pLgu = CLoginManger::InitLoginUser(SLD, pwszIp, wszRetMsg);
	if (pLgu == NULL)
	{
		RetBf << SOCKET_MESSAGE_RETMSG << wszRetMsg;
		return TRUE;
	}

	DWORD dwTime = (DWORD)(pLgu->dwSurplusHour - ((::GetTickCount() - pLgu->dwSurplusTick) / 1000 / 60 / 60));
	RetBf << SOCKET_MESSAGE_RECHARGELOGINUSER << dwTime << wszRetMsg;*/
	return TRUE;
}

BOOL CVerificationSystem::GetCardInfo_By_LoginUser(SockLoginData& SLD, LPCWSTR pwszIp, __out ByteBuffer& RetBf)
{
	return TRUE;
}

BOOL CVerificationSystem::SetSockKeepCon(__in const SockClientGroup& ClientGroup)
{
	DWORD dwComputerCrc = CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.wszComputerName, wcslen(ClientGroup.wszComputerName) * 2 + 1);

	BOOL bExist = FALSE;
	for (auto& __SockKeepConSrc : SocketConectLst)
	{
		if (__SockKeepConSrc.dwComputerCrc == dwComputerCrc)
		{
			__SockKeepConSrc.dwQqCrc = ClientGroup.dwGameUserCrc;
			__SockKeepConSrc.ulTick = ::GetTickCount64();
			bExist = TRUE;
		}
	}

	if (!bExist)
	{
		SockKeepConSrc SKC = { 0 };
		SKC.ulTick = ::GetTickCount64();
		SKC.dwQqCrc = ClientGroup.dwGameUserCrc;
		SKC.dwComputerCrc = dwComputerCrc;
		SocketConectLst.push_back(SKC);
	}

	return TRUE;
}

BOOL CVerificationSystem::KeepAlive(DWORD dwCmd, SockClientGroup& ClientGroup, LPCWSTR pwszIp, __out ByteBuffer& RetBf)
{
	/*if (CLoginManger::_GetCardTime(ClientGroup.SLD.Card.szCard, ClientGroup.SLD.dwVerCode, pwszIp) == 0)
		{
		RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"WTF? 少年,心跳的时候 貌似卡号不够时间了!")/ *此卡不存在或者被激活了* /;
		return TRUE;
		}*/

	// 初始化Crc
	_InitSockClientGroup(ClientGroup);

	// 心跳
	SetSockKeepCon(ClientGroup);

	// 获取对应的服务器列表
	auto pGroupServer = _ExistGroupServer(ClientGroup.wszServerName);
	if (pGroupServer == NULL)
	{
		RetBf << SOCKET_MESSAGE_KEEPALIVE;
		//RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"少年, 你这个是什么破服务器,我这里组队没这个选项!");
		return TRUE;
	}

	// 获取对应的地图列表
	auto pGroupMap = _ExistGroupMap(pGroupServer, ClientGroup.wszMapName);
	if (pGroupMap == NULL)
	{
		RetBf << SOCKET_MESSAGE_KEEPALIVE;
		//RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"少年, 你这张是什么破地图? 组队就4张地图,你在搞什么灰机?");
		return TRUE;
	}

	for (auto& Group : pGroupMap->vGrouplst)
	{
		// 填充属性(金币, 等级, 总场数)
		if (Group.Captain == ClientGroup)
		{
			Group.Captain.SLD = ClientGroup.SLD;
			Group.Captain.ulTick = ::GetTickCount64();
		}
		else
		{
			BOOL bExist = FALSE;
			for (auto& GroupMember : Group.MemberList)
			{
				if (GroupMember == ClientGroup)
				{
					bExist = TRUE;
					GroupMember.SLD = ClientGroup.SLD;
					GroupMember.ulTick = ::GetTickCount64();
					break;
				}
			}

			if (bExist)
				break;
		}
	}

	// 回发心跳头 给他表示正常
	RetBf << SOCKET_MESSAGE_KEEPALIVE;
	return TRUE;
}

////////组团//////////////////////////////////////////////////////////////////

BOOL CVerificationSystem::_InitSockClientGroup(SockClientGroup& ClientGroup)
{
	CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.SLD.wszGameUser, wcslen(ClientGroup.SLD.wszGameUser) * 2 + 1, ClientGroup.dwGameUserCrc);
	CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.SLD.wszLoginUser, wcslen(ClientGroup.SLD.wszLoginUser) * 2 + 1, ClientGroup.dwLoginUserCrc);
	CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.SLD.wszNiceName, wcslen(ClientGroup.SLD.wszNiceName) * 2 + 1, ClientGroup.dwNiceNameCrc);
	ClientGroup.ulTick = ::GetTickCount64();
	return TRUE;
}

BOOL CVerificationSystem::_InitGroup()
{
	auto fn_GetCrc_By_ServerName = [](LPCWSTR pwszServerName)
	{
		static Srt_ServerGroup SSG;
		ZeroMemory(&SSG, sizeof(SSG));

		CCharacter::wstrcpy_my(SSG.wszServerName, pwszServerName);
		CLPublic::GetCRC32_DWORD((LPSTR)pwszServerName, wcslen(pwszServerName) * 2 + 1, SSG.dwServerCrc);
		return &SSG;
	};

	/////////电信/////////////////////////////////////////////////////////////////
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"艾欧尼亚"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"祖安"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"诺克萨斯"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"班德尔城"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"皮尔特沃夫"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"战争学院"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"巨神峰"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"雷瑟守备"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"钢铁烈阳"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"裁决之地"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"黑色玫瑰"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"暗影岛"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"均衡教派"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"水晶之痕"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"影流"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"守望之海"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"征服之海"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"卡拉曼达"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"皮城警备"));

	////////网通//////////////////////////////////////////////////////////////////
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"比尔吉沃特"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"德玛西亚"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"弗雷尔卓德"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"无畏先锋"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"恕瑞玛"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"扭曲丛林"));
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"巨龙之巢"));

	/////////其他/////////////////////////////////////////////////////////////////
	_CreateGroupServer(fn_GetCrc_By_ServerName(L"教育网专区"));

	return TRUE;
}

BOOL CVerificationSystem::_CreateGroupServer(Srt_ServerGroup* pServerGroup)
{
	// 获取地图Crc
	auto fn_GetCrc_By_MapName = [](LPCWSTR pwszMapName)
	{
		static Srt_MapGroup SMG;
		ZeroMemory(&SMG, sizeof(SMG));

		CCharacter::wstrcpy_my(SMG.wszMapName, pwszMapName);
		CLPublic::GetCRC32_DWORD((LPSTR)pwszMapName, wcslen(pwszMapName) * 2 + 1, SMG.dwMapCrc);
		return &SMG;
	};

	// 初始化
	static Server_GroupMember SGM;
	vector<Map_GroupMember> vMaplst;
	ZeroMemory(&SGM, sizeof(SGM));

	// 给每一个服务器下都创建4个地图
	_CreateGroupMap(vMaplst, fn_GetCrc_By_MapName(L"召唤师峡谷"));
	_CreateGroupMap(vMaplst, fn_GetCrc_By_MapName(L"水晶之痕"));
	_CreateGroupMap(vMaplst, fn_GetCrc_By_MapName(L"扭曲丛林"));
	_CreateGroupMap(vMaplst, fn_GetCrc_By_MapName(L"嚎哭深渊"));

	// 添加到服务器列表
	SGM.Srt_Server = *pServerGroup;
	SGM.vMaplst = vMaplst;
	GroupLst.push_back(SGM);
	return TRUE;
}

BOOL CVerificationSystem::_CreateGroupMap(vector<Map_GroupMember>& vMaplst, Srt_MapGroup* pMapGroup)
{
	static Map_GroupMember MGM;
	ZeroMemory(&MGM, sizeof(MGM));
	MGM.Srt_Map = *pMapGroup;
	//MGM.vGrouplst
	vMaplst.push_back(MGM);
	return TRUE;
}

Server_GroupMember* CVerificationSystem::_ExistGroupServer(LPCWSTR pwszServerName)
{
	DWORD dwServerCrc = 0;
	CLPublic::GetCRC32_DWORD((LPSTR)pwszServerName, wcslen(pwszServerName) * 2 + 1, dwServerCrc);

	for (auto& GroupServer : GroupLst)
	{
		if (GroupServer.Srt_Server.dwServerCrc == dwServerCrc && CCharacter::wstrcmp_my(pwszServerName, GroupServer.Srt_Server.wszServerName))
			return &GroupServer;
	}

	return NULL;
}

Map_GroupMember* CVerificationSystem::_ExistGroupMap(Server_GroupMember* pGroupServer, LPCWSTR pwszMapName)
{
	if (pGroupServer == NULL || pwszMapName == NULL)
		return NULL;

	DWORD dwMapCrc = 0;
	CLPublic::GetCRC32_DWORD((LPSTR)pwszMapName, wcslen(pwszMapName) * 2 + 1, dwMapCrc);

	for (auto& GroupMap : pGroupServer->vMaplst)
	{
		if (GroupMap.Srt_Map.dwMapCrc == dwMapCrc && CCharacter::wstrcmp_my(pwszMapName, GroupMap.Srt_Map.wszMapName))
			return &GroupMap;
	}

	return NULL;
}

em_GroupPlayer CVerificationSystem::_ExistGroupUser(Map_GroupMember* pGroupMap, SockClientGroup& ClientGroup)
{
	try
	{
		// 搜索该地图下 所有的队伍, 是否存在这号人
		for (auto& Group : pGroupMap->vGrouplst)
		{
			// 从队长那里找找有木有这号人
			if (Group.Captain == ClientGroup && !Group.Captain.bLeave)
			{
				_InitSockClientGroup(ClientGroup);
				Group.Captain.ulTick = ::GetTickCount64();
				ClientGroup = Group.Captain;
				return em_GroupPlayer_Captain;
			}

			// 否则搜索4位成员
			for (auto& GroupMember : Group.MemberList)
			{
				if (GroupMember == ClientGroup && !GroupMember.bLeave)
				{
					GroupMember.ulTick = ::GetTickCount64();
					ClientGroup = GroupMember;
					return em_GroupPlayer_Member;
				}
			}
		}
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"_ExistGroupUser出现异常");
	}

	return em_GroupPlayer_None;
}

BOOL CVerificationSystem::_AddWaitGroupMember(__in Map_GroupMember* pMapGroup, __in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	//////暂时觉得不妥, 后面再完善////////////////////////////////////////////////////////////////////
	return FALSE;

	BOOL bExist = FALSE;
	for (UINT i = 0; i < WaitGroupLst.size(); ++i)
	{
		if (WaitGroupLst.at(i) == ClientGroup)
		{
			WaitGroupLst.erase(WaitGroupLst.begin() + i); // 删除等待列表的队长
			bExist = TRUE;
			break;
		}
	}

	// 不存在就直接返回不做任何操作
	if (!bExist)
		return FALSE;

	// 如果已经存在了, 那么自己肯定是队长!
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"随机队伍的队长是:%s,size()=%d", ClientGroup.SLD.wszNiceName, WaitGroupLst.size());
	random_shuffle(WaitGroupLst.begin(), WaitGroupLst.end()); // 先随机一番

	// 取前面的4个当队友
	if (WaitGroupLst.size() < 4)
	{
		WaitGroupLst.clear();
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"但是等候数量是:%d", WaitGroupLst.size());
		return FALSE;
	}

	// 判断WaitGroupLst里面同区同地图的人数, 再次筛选, 避免冲突
	vector<SockClientGroup> vlst;
	for (int i = 0; i < (INT)WaitGroupLst.size(); ++i)
	{
		auto& SCG = WaitGroupLst.at(i);
		if (CCharacter::wstrcmp_my(SCG.wszServerName, ClientGroup.wszServerName) && CCharacter::wstrcmp_my(SCG.wszMapName, ClientGroup.wszMapName))
		{
			vlst.push_back(SCG);
			WaitGroupLst.erase(WaitGroupLst.begin() + i--);
		}
	}

	if (vlst.size() < 4)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"但是等候数量是:%d -> %d", WaitGroupLst.size(), vlst.size());
		return FALSE;
	}

	// 队长创建新的队伍
	GroupList GL;
	ZeroMemory(&GL, sizeof(GL));

	GL.InitHero();
	_InitHero(GL.emList, ClientGroup);	// 队长是流浪
	GL.Captain = ClientGroup;
	GL.Captain.ulTick = ::GetTickCount64();
	GL.Captain.bLeave = FALSE;

	// 等待列表的前4个加入新队伍
	for (UINT i = 0; i < 4; ++i)
	{
		SockClientGroup SCG = vlst.at(i);
		_InitHero(GL.emList, SCG);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"队友%d是:%s, 英雄:%d,%d", i + 1, SCG.SLD.wszNiceName, (int)SCG.emHero, (int)SCG.emRetainHero);
		SCG.bAlive = FALSE;
		SCG.ulTick = ::GetTickCount64();
		SCG.bLeave = FALSE;
		GL.MemberList.push_back(SCG);
	}

	pMapGroup->vGrouplst.push_back(GL);
	// 将自己的名字回发, 表示自己就是团长!
	//RetBf << SOCKET_MESSAGE_GETGROUP << wstring(ClientGroup.SLD.wszNiceName) << (DWORD)ClientGroup.emHero << (DWORD)ClientGroup.emRetainHero; // 返回所属英雄
	RetBf << SOCKET_MESSAGE_WAIT;
	return TRUE;
}

BOOL CVerificationSystem::_HandleGroup(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	try
	{
		// 获取对应的服务器列表
		auto pGroupServer = _ExistGroupServer(ClientGroup.wszServerName);
		if (pGroupServer == NULL)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"少年, 你这个是什么破服务器:%s,我这里组队没这个选项!", ClientGroup.wszServerName);
			RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"少年, 你这个是什么破服务器,我这里组队没这个选项!");
			return TRUE;
		}

		// 判断模式
		ServerAccountInfo __ServerAccountInfo = { 0 };
		__ServerAccountInfo.dwQqCrc = ClientGroup.dwGameUserCrc;

		AccountListLock.Access([&__ServerAccountInfo]{
			for (auto& tmpServerAccountInfo : AccountList)
			{
				if (tmpServerAccountInfo.CompQqCrc(__ServerAccountInfo.dwQqCrc))
				{
					__ServerAccountInfo = tmpServerAccountInfo;
					break;
				}
			}
		});

		if (__ServerAccountInfo.dwComputerNameCrc == NULL)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"这个帐号被删除了!,让这位少年重新获取!");
			RetBf << SOCKET_MESSAGE_GETACCOUNT;
			return TRUE;
		}

		if (__ServerAccountInfo.PlayMode == AccountPlayMode_Default)
		{
			// 自定义模式下, 7级是刷人机召唤师峡谷,7级后刷统治战场
			if (ClientGroup.SLD.nSumWarCount >= 40)
				CCharacter::wstrcpy_my(ClientGroup.wszMapName, L"水晶之痕");
			else
				CCharacter::wstrcpy_my(ClientGroup.wszMapName, L"召唤师峡谷");
		}
		else if (__ServerAccountInfo.PlayMode == AccountPlayMode_Custome)
		{
			// 自定义模式下是 一直刷某个地图
			CCharacter::wstrcpy_my(ClientGroup.wszMapName, __ServerAccountInfo.wszMapName);
		}

		// 获取对应的地图列表
		auto pGroupMap = _ExistGroupMap(pGroupServer, ClientGroup.wszMapName);
		if (pGroupMap == NULL)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"少年, 你这张是什么破地图:%s? 组队就4张地图,你在搞什么灰机?", ClientGroup.wszMapName);
			RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"少年, 你这张是什么破地图? 组队就4张地图,你在搞什么灰机?");
			return TRUE;
		}

		// 判断是否等待列表中,是的话, 就把4个队友加进去!
		if (_AddWaitGroupMember(pGroupMap, ClientGroup, RetBf))
			return TRUE;

		// 判断自己是不是已经 在队伍中了, 不要重复加入2个队伍!
		em_GroupPlayer emPlayer = _ExistGroupUser(pGroupMap, ClientGroup);

		if (emPlayer == em_GroupPlayer_Captain)
		{
			// 二次访问了的队长
			_RefushGroupCaptain(pGroupMap, ClientGroup, RetBf);
		}
		else if (emPlayer == em_GroupPlayer_Member)
		{
			// 二次访问的队员
			_RefushGroupMember_To_Captain(pGroupMap, ClientGroup, RetBf);
		}
		else // None
		{
			// 一次访问
			_JoinGroup(pGroupMap, __ServerAccountInfo, ClientGroup);
			RetBf << SOCKET_MESSAGE_WAIT; // 一直等待!
		}
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"_HandleGroup出现异常");
	}

	return TRUE;
}

BOOL CVerificationSystem::_InitHero(__in vector<int>& vlst, __out SockClientGroup& ClientGroup)
{
	static em_Hero_Pro emHeroArray[] = {
		em_Hero_Pro_Ryze,
		em_Hero_Pro_Garen, em_Hero_Pro_Ashe,
		em_Hero_Pro_MissFortune, em_Hero_Pro_MasterYi
	};

	static em_Hero_Pro emRetainHeroArray[] = {
		em_Hero_Pro_Ryze,
		em_Hero_Pro_Garen, em_Hero_Pro_Ashe,
		em_Hero_Pro_Caitlyn, em_Hero_Pro_Ahri
	};

	if (vlst.size() == 0)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"握草, vlst.size=0");
		return FALSE;
	}
	random_shuffle(vlst.begin(), vlst.end());

	ClientGroup.nHeroIndex = vlst.at(0);
	if (ClientGroup.nHeroIndex >= 5)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"ClientGroup.nHeroIndex=%d", ClientGroup.nHeroIndex);
		ClientGroup.nHeroIndex %= 5;
	}

	ClientGroup.emHero = emHeroArray[ClientGroup.nHeroIndex];
	ClientGroup.emRetainHero = emRetainHeroArray[ClientGroup.nHeroIndex];
	vlst.erase(vlst.begin());
	return TRUE;
}

BOOL CVerificationSystem::_JoinGroup(Map_GroupMember* pMapGroup, __in const ServerAccountInfo& __ServerAccountInfo, SockClientGroup& ClientGroup)
{
	try
	{
		// 搜索有木有团队人没满
		for (auto& Group : pMapGroup->vGrouplst)
		{
			// 人家队伍已经满了……
			if (Group.MemberList.size() >= 4)
				continue;

			// 以防万一, 木有队长的也不考虑(可能是掉线了)
			if (Group.Captain.dwNiceNameCrc == NULL)
				continue;

			// 判断这个队长的模式, 是不是跟自己一个模式
			if (Group.PlayMode != __ServerAccountInfo.PlayMode)
				continue;

			// 好, 就是这个队伍有位置 也有队长, 加入这支队伍, +1是指, 队长永远都是流浪法师
			if (!_InitHero(Group.emList, ClientGroup))
				continue;

			ClientGroup.bAlive = FALSE;
			ClientGroup.bLeave = FALSE;
			Group.MemberList.push_back(ClientGroup);
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"加入队伍, 队长是:%s,自己是:%s,英雄:%d,%d", Group.Captain.SLD.wszNiceName, ClientGroup.SLD.wszNiceName, ClientGroup.emHero, ClientGroup.emRetainHero);
			return TRUE;
		}

		// 竟然木有满意的队伍, 那么自己创建一个
		static GroupList GL;
		ZeroMemory(&GL, sizeof(GL));
		GL.InitHero();

		_InitHero(GL.emList, ClientGroup);	// 队长是流浪
		GL.Captain = ClientGroup;
		GL.Captain.bLeave = FALSE;
		GL.PlayMode = __ServerAccountInfo.PlayMode;

		if (GL.PlayMode == AccountPlayMode_Default && ClientGroup.SLD.nSumWarCount % 5 == 0)
			CCharacter::wstrcpy_my(GL.Captain.wszMapName, L"水晶之痕");

		pMapGroup->vGrouplst.push_back(GL);

		// 将自己的名字回发, 表示自己就是团长!
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"创建队伍!,队长是:%s,英雄:%d,%d", ClientGroup.SLD.wszNiceName, ClientGroup.emHero, ClientGroup.emRetainHero);
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"_JoinGroup出现异常");
	}
	return TRUE;
}

BOOL CVerificationSystem::_RefushGroupMember_To_Captain(Map_GroupMember* pMapGroup, const SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	for (int i = 0; i < (int)pMapGroup->vGrouplst.size(); ++i)
	{
		for (auto& GroupMember : pMapGroup->vGrouplst.at(i).MemberList)
		{
			if (GroupMember == ClientGroup)
			{
				auto& Group = pMapGroup->vGrouplst.at(i);
				// 判断队长是否心跳超时!
				if (::GetTickCount64() - Group.Captain.ulTick >= 30 * 1000)
				{
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"握草! 队长%s心跳超时了, 解散队伍!", Group.Captain.SLD.wszNiceName);
					pMapGroup->vGrouplst.erase(pMapGroup->vGrouplst.begin() + i);
					RetBf << SOCKET_MESSAGE_WAIT;
					return TRUE;
				}
				if (Group.MemberList.size() < 4)
				{
					// 当队伍不足4人的时候, 就一直在等, 别冲动
					RetBf << SOCKET_MESSAGE_WAIT;
					return TRUE;
				}
				GroupMember.bAlive = TRUE; // 状态激活!

				// 判断这4人是否都是激活状态
				BOOL bAlive = TRUE;
				if (!Group.MemberList.at(0).bAlive || !Group.MemberList.at(1).bAlive || !Group.MemberList.at(2).bAlive || !Group.MemberList.at(3).bAlive)
					bAlive = FALSE;

				// 判断这4人是否有人离队了
				if (Group.MemberList.at(0).bLeave || Group.MemberList.at(1).bLeave || Group.MemberList.at(2).bLeave || Group.MemberList.at(3).bLeave)
					bAlive = FALSE;

				// 有人还是未激活状态,等!
				if (!bAlive)
				{
					RetBf << SOCKET_MESSAGE_WAIT;
					return TRUE;
				}

				/////处理地图名和状态/////////////////////////////////////////////////////////////////////
				_HandleServerStatus(ClientGroup, FALSE);

				// 然后回发 团长的名字
				RetBf << SOCKET_MESSAGE_GROUP_MEMBER << pMapGroup->vGrouplst.at(i).Captain.wszMapName << (DWORD)ClientGroup.emHero << (DWORD)ClientGroup.emRetainHero; // 返回所属英雄;
				return TRUE;
			}
		}
	}
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"握草!, 竟然找不到队长:%s", ClientGroup.SLD.wszNiceName);
	return TRUE;
}

BOOL CVerificationSystem::_HandleServerStatus(__in const SockClientGroup& ClientGroup, BOOL bLeave)
{
	ServerAccountInfo __ServerAccountInfo = { 0 };

	__ServerAccountInfo.dwQqCrc = ClientGroup.dwGameUserCrc;
	CCharacter::wstrcpy_my(__ServerAccountInfo.wszStatus, bLeave ? L"单身狗" : L"有队伍");
	CCharacter::wstrcpy_my(__ServerAccountInfo.wszMapName, ClientGroup.wszMapName);

	AccountListLock.Access([&__ServerAccountInfo]{
		for (auto& tmpServerAccountInfo : AccountList)
		{
			if (tmpServerAccountInfo.CompQqCrc(__ServerAccountInfo.dwQqCrc))
			{
				// 只更新一个状态即可!
				CCharacter::wstrcpy_my(tmpServerAccountInfo.wszStatus, __ServerAccountInfo.wszStatus);
				CCharacter::wstrcpy_my(tmpServerAccountInfo.wszMapName, __ServerAccountInfo.wszMapName);
				break;
			}
		}
	});

	return TRUE;
}

BOOL CVerificationSystem::_RefushGroupCaptain(Map_GroupMember* pMapGroup, const SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	UINT uGroupIndex = 0;
	for (auto& Group : pMapGroup->vGrouplst)
	{
		uGroupIndex++;
		// 判断组队
		if (Group.Captain == ClientGroup && !Group.Captain.bLeave)
		{
			// 判断是否有队友超时了!
			for (int i = 0; i < (int)Group.MemberList.size(); ++i)
			{
				if (::GetTickCount64() - Group.MemberList.at(i).ulTick >= 30 * 1000)
				{
					// 先把英雄归还到列表
					Group.emList.push_back(Group.MemberList.at(i).nHeroIndex);

					// 再删
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"队员%s 心跳超时! 解散队伍!", Group.MemberList.at(i).SLD.wszNiceName);
					pMapGroup->vGrouplst.erase(pMapGroup->vGrouplst.begin() + (uGroupIndex - 1));
					RetBf << SOCKET_MESSAGE_WAIT;
					return TRUE;
				}
			}

			// 要凑够4个人才往回发!
			if (Group.MemberList.size() != 4)
				break;

			// 判断这4人是否都是激活状态
			BOOL bAlive = TRUE;
			if (!Group.MemberList.at(0).bAlive || !Group.MemberList.at(1).bAlive || !Group.MemberList.at(2).bAlive || !Group.MemberList.at(3).bAlive)
				bAlive = FALSE;

			// 判断这4人是否有人离队了
			if (Group.MemberList.at(0).bLeave || Group.MemberList.at(1).bLeave || Group.MemberList.at(2).bLeave || Group.MemberList.at(3).bLeave)
				bAlive = FALSE;

			// 有人还是未激活状态,等!
			if (!bAlive)
				break;

			//////////////////////////////////////////////////////////////////////////
			/*if (CCharacter::wstrcmp_my(ClientGroup.wszMapName, L"水晶之痕")) // 表示5个号都是水晶之痕, 那么每5把可以来一把召唤师峡谷
			{
				if (ClientGroup.SLD.nSumWarCount % 5 == 0)
					CCharacter::wstrcpy_my(Group.Captain.wszMapName, L"召唤师峡谷");
			}*/

			/////处理地图名和状态/////////////////////////////////////////////////////////////////////
			_HandleServerStatus(ClientGroup, FALSE);

			// 然后回发 所属的英雄
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"回发队长客户端所属的4个队友! 队长是:%s", ClientGroup.SLD.wszNiceName);
			RetBf << SOCKET_MESSAGE_GROUP_CAPTAIN << Group.Captain.wszMapName << (DWORD)Group.Captain.emHero << (DWORD)Group.Captain.emRetainHero; // 返回所属英雄;
			for (auto GroupMember : Group.MemberList)
				RetBf << wstring(GroupMember.SLD.wszNiceName);

			return TRUE;
		}
	}

	RetBf << SOCKET_MESSAGE_WAIT;
	return TRUE;
}

BOOL CVerificationSystem::_LeaveGroup(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	/*if (CLoginManger::_GetCardTime(ClientGroup.SLD.Card.szCard, ClientGroup.SLD.dwVerCode, pwszIp) == 0)
	{
	RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"WTF? 少年,心跳的时候 貌似卡号不够时间了!")/ *此卡不存在或者被激活了* /;
	return TRUE;
	}*/

	// 初始化Crc
	_InitSockClientGroup(ClientGroup);

	// 获取对应的服务器列表
	auto pGroupServer = _ExistGroupServer(ClientGroup.wszServerName);
	if (pGroupServer == NULL)
	{
		RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"少年, 你这个是什么破服务器,我这里组队没这个选项!");
		return TRUE;
	}

	// 获取对应的地图列表
	auto pGroupMap = _ExistGroupMap(pGroupServer, ClientGroup.wszMapName);
	if (pGroupMap == NULL)
	{
		RetBf << SOCKET_MESSAGE_RETMSG << wstring(L"少年, 你这张是什么破地图? 组队就4张地图,你在搞什么灰机?");
		return TRUE;
	}

	for (INT i = 0; i < (INT)pGroupMap->vGrouplst.size(); ++i)
	{
		auto Group = pGroupMap->vGrouplst.at(i);
		// 如果发现队伍中的队长要离队
		if (Group.Captain == ClientGroup)
		{
			Group.Captain.bLeave = TRUE;
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"解散队伍,队长是:%s", Group.Captain.SLD.wszNiceName);
			pGroupMap->vGrouplst.erase(pGroupMap->vGrouplst.begin() + i--);
			break;
		}

		// 如果发现队伍中的队员要离队
		BOOL bExist = FALSE;
		for (auto& GroupMember : Group.MemberList)
		{
			if (GroupMember == ClientGroup)
			{
				GroupMember.bLeave = TRUE;
				bExist = TRUE;
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"解散队伍,队长是:%s", Group.Captain.SLD.wszNiceName);
				pGroupMap->vGrouplst.erase(pGroupMap->vGrouplst.begin() + i--);
				break;
			}
		}

		if (bExist)
			break;
	}

	// 加入到等待列表
	_JoinWaitList(ClientGroup);

	/////处理地图名和状态/////////////////////////////////////////////////////////////////////
	_HandleServerStatus(ClientGroup, TRUE);

	RetBf << SOCKET_MESSAGE_LEAVEGROUP;
	return TRUE;
}

BOOL CVerificationSystem::_JoinWaitList(__in SockClientGroup& SCG)
{
	////////等待列表暂时不启动//////////////////////////////////////////////////////////////////
	return FALSE;

	BOOL bExist = FALSE;
	for (auto& GroupMember : WaitGroupLst)
	{
		if (GroupMember == SCG)
		{
			bExist = TRUE;
			break;
		}
	}

	if (!bExist)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"%s加入等待列表", SCG.SLD.wszNiceName);
		WaitGroupLst.push_back(SCG);
	}

	return TRUE;
}

BOOL CVerificationSystem::_DeleteGroupList(__in SockClientGroup& ClientGroup)
{
	// 搜索特定地图下的所有队伍
	/*for (int i = 0; i < (int)pMapGroup->vGrouplst.size(); ++i)
	{
		// 离队数量
		UINT uLeaveCount = 0;
		auto pGL = &pMapGroup->vGrouplst.at(i);
		if (pGL->Captain.bLeave) // calc Captain
			uLeaveCount++;

		// calc Team Member Leave Count
		for (auto& GroupMember : pGL->MemberList)
		{
			if (GroupMember.bLeave)
				uLeaveCount++;
		}

		// All of it -> delete
		if (uLeaveCount >= 1)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"解散队伍,队长是:%s", pGL->Captain.SLD.wszNiceName);
			pMapGroup->vGrouplst.erase(pMapGroup->vGrouplst.begin() + i--);
			break;
		}
	}*/
	return TRUE;
}

BOOL CVerificationSystem::_IdSealed(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"帐号:%s 昵称:%s 被封号了", ClientGroup.SLD.wszGameUser, ClientGroup.SLD.wszNiceName);

	_InitSockClientGroup(ClientGroup);

	AccountListLock.Access([&ClientGroup]{
		for (auto& __ServerAccountInfo : AccountList)
		{
			if (__ServerAccountInfo.CompQqCrc(ClientGroup.dwGameUserCrc))
			{
				CCharacter::wstrcpy_my(__ServerAccountInfo.wszMsg, L"封号!");
				__ServerAccountInfo.bIdSealed = TRUE;
				break;
			}
		}
	});

	// 从GorupList删掉
	
	RetBf << SOCKET_MESSAGE_GAMETITLE;
	return TRUE;
}

BOOL CVerificationSystem::_GetAccount(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	ServerAccountInfo __ServerAccountInfo = { 0 };

	if (CCharacter::wstrcmp_my(ClientGroup.wszComputerName, L"test"))
	{
		RetBf << SOCKET_MESSAGE_RETMSG << L"你特么至少得改一个ID吧? 不然重复ID我怎么给你弄?";
		return TRUE;
	}

	_InitSockClientGroup(ClientGroup);
	__ServerAccountInfo.dwComputerNameCrc = CLPublic::GetCRC32_DWORD((LPSTR)ClientGroup.wszComputerName, wcslen(ClientGroup.wszComputerName) * 2 + 1);
	
	AccountListLock.Access([&__ServerAccountInfo]{
		for (auto& tmpServerAccountInfo : AccountList)
		{
			if (tmpServerAccountInfo == __ServerAccountInfo)
			{
				__ServerAccountInfo = tmpServerAccountInfo;
				break;
			}
		}
	});

	// 如果是NULL || 封号 || 出错 || 达成条件 的话, 就表示该帐号不可用
	if (__ServerAccountInfo.dwQqCrc == NULL || __ServerAccountInfo.bIdSealed || __ServerAccountInfo.bError || __ServerAccountInfo.IsFinish())
	{
		// 重新获取一个帐号
		//ZeroMemory(&__ServerAccountInfo, sizeof(__ServerAccountInfo));
		CCharacter::wstrcpy_my(__ServerAccountInfo.wszComputerName, ClientGroup.wszComputerName);

		AccountListLock.Access([&__ServerAccountInfo]{
			for (auto& tmpServerAccountInfo : AccountList)
			{
				// 必须木有编号被占用 && !封号 && !出错 && !达成条件
				if (tmpServerAccountInfo.dwComputerNameCrc == NULL && !tmpServerAccountInfo.bIdSealed && !tmpServerAccountInfo.bError && !tmpServerAccountInfo.IsFinish())
				{
					// 赋值编号, 就可以不让别的帐号重复获取
					tmpServerAccountInfo.dwComputerNameCrc = CLPublic::GetCRC32_DWORD((LPSTR)__ServerAccountInfo.wszComputerName, wcslen(__ServerAccountInfo.wszComputerName) * 2 + 1);
					CCharacter::wstrcpy_my(tmpServerAccountInfo.wszComputerName, __ServerAccountInfo.wszComputerName);
					CCharacter::wstrcpy_my(tmpServerAccountInfo.wszMsg, L"与客户端成功连接!");

					// 返回给此编号的电脑
					__ServerAccountInfo = tmpServerAccountInfo;
					break;
				}
			}
		});

		if (__ServerAccountInfo.dwComputerNameCrc == NULL)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"握草, 帐号不足!!!");
			return SOCKET_MESSAGE_WAIT;
		}
		// Qq >> Pass >> Server >> Map
		RetBf << SOCKET_MESSAGE_GETACCOUNT << __ServerAccountInfo.wszQQ << __ServerAccountInfo.wszPass << __ServerAccountInfo.wszServer << __ServerAccountInfo.wszMapName;
	}
	else
	{
		// 返回原来的帐号
		RetBf << SOCKET_MESSAGE_GETACCOUNT << __ServerAccountInfo.wszQQ << __ServerAccountInfo.wszPass << __ServerAccountInfo.wszServer << __ServerAccountInfo.wszMapName;
	}

	return TRUE;
}

BOOL CVerificationSystem::_ClientMsg(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	ServerAccountInfo __ServerAccountInfo = { 0 };

	_InitSockClientGroup(ClientGroup);
	__ServerAccountInfo.dwQqCrc = ClientGroup.dwGameUserCrc;

	AccountListLock.Access([&ClientGroup]{
		for (auto& tmpServerAccountInfo : AccountList)
		{
			if (tmpServerAccountInfo.CompQqCrc(ClientGroup.dwGameUserCrc))
			{
				if (CCharacter::wstrstr_my(ClientGroup.wszClientMsg, L"错误:"))
					tmpServerAccountInfo.bError = TRUE;

				// 消息
				CCharacter::wstrcpy_my(tmpServerAccountInfo.wszMsg, ClientGroup.wszClientMsg);

				// 顺带更新一下角色战绩到界面
				if (ClientGroup.SLD.nLevel != 0)
				{
					CCharacter::wstrcpy_my(tmpServerAccountInfo.wszNiceName, ClientGroup.SLD.wszNiceName);
					tmpServerAccountInfo.dwFightCount = ClientGroup.SLD.nSumWarCount;
					tmpServerAccountInfo.dwGold = ClientGroup.SLD.nGold;
					tmpServerAccountInfo.dwLevel = ClientGroup.SLD.nLevel;
				}
				break;
			}
		}
	});
	
	// 回发消息表示一切正常!
	RetBf << SOCKET_MESSAGE_CLIENTMSG;
	return TRUE;
}

BOOL CVerificationSystem::_GameRecord(__in SockClientGroup& ClientGroup, __out ByteBuffer& RetBf)
{
	RetBf << SOCKET_MESSAGE_RECORD;
	return TRUE;
}

BOOL CVerificationSystem::_RefushLoseCon()
{
	for (int i = 0; i < (int)SocketConectLst.size(); ++i)
	{
		auto pSockKeepConSrc = &SocketConectLst.at(i);
		if (::GetTickCount64() - pSockKeepConSrc->ulTick >= 30 * 1000)
		{
			// 有人掉线了!
			AccountListLock.Access([&pSockKeepConSrc]{
				for (auto& tmpServerAccountInfo : AccountList)
				{
					if (tmpServerAccountInfo.dwComputerNameCrc == pSockKeepConSrc->dwComputerCrc)
					{
						CCharacter::wstrcpy_my(tmpServerAccountInfo.wszMsg, L"错误: 与客户端断开连接!");
						tmpServerAccountInfo.bError = TRUE;
					}
				}
			});
			SocketConectLst.erase(SocketConectLst.begin() + i--);
		}
	}

	return TRUE;
}