#include "VerClientSocket.h"
#include "ConsoleConfig.h"
#define _SELF L"VerClientSocket.cpp"

CRITICAL_SECTION CVerClient::CrSction;
stack<ByteBuffer> RecvLst;
DWORD dwLoseConTick = NULL;

CVerClientSocket::CVerClientSocket()
{
	hRecvThread = NULL;
	bStop = FALSE;
	pVerClient = NULL;
	pservice_pump = NULL;
}

CVerClientSocket::~CVerClientSocket()
{
	Stop();
}

BOOL CVerClientSocket::StartSocket(st_service_pump* pst_service_pump, CVerClient* pCVerClient)
{
	return TRUE;
}

BOOL CVerClientSocket::Start()
{
	if (hRecvThread == NULL)
	{
		hRecvThread = cbBEGINTHREADEX(NULL, NULL, RecvThread, this, NULL, NULL);
		return hRecvThread != NULL;
	}
	return TRUE;
}

BOOL CVerClientSocket::Stop()
{
	bStop = TRUE;

	pVerClient->stop_service();
	pservice_pump->stop_service();

	pVerClient = NULL;
	pservice_pump = NULL;

	if (hRecvThread != NULL)
	{
		if (WaitForSingleObject(hRecvThread, 3000) == WAIT_TIMEOUT)
		{
			::TerminateThread(hRecvThread, 0);
			::CloseHandle(hRecvThread);
			hRecvThread = NULL;
		}
	}
	return TRUE;
}

DWORD CVerClientSocket::RecvThread(LPVOID lpParm)
{
	CVerClientSocket* pVerClientSocket = (CVerClientSocket *)lpParm;
	if (pVerClientSocket == NULL)
		return 0;

	DWORD dwTick = ::GetTickCount();
	while (!pVerClientSocket->bStop)
	{
		if (pVerClientSocket->pVerClient->is_connected())
		{
			if (dwLoseConTick != NULL)
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"重新连接服务器成功!");
				dwLoseConTick = NULL;
			}
		}
		else
		{
			if (dwLoseConTick == NULL)
			{
				dwLoseConTick = ::GetTickCount();
			}
			pVerClientSocket->pVerClient->reset();
			Sleep(5 * 1000);
			continue;
		}

	
		if (::GetTickCount() - dwTick >= 5 * 1000)
		{
			ByteBuffer Bf;
			CreatePacket(SOCKET_MESSAGE_KEEPALIVE, Bf);
			pVerClientSocket->pVerClient->post_msg((char*)Bf.contents(), Bf.size(), false);
			dwTick = ::GetTickCount();
		}
		if (RecvLst.empty())
		{
			Sleep(100);
			continue;
		}

		CVerClient::Lock();
		auto Msg = RecvLst.top();
		RecvLst.pop();
		CVerClient::UnLock();

		ByteBuffer ServerBf = Msg;

		DWORD dwCmd = 0;
		ServerBf >> dwCmd;

		switch (dwCmd)
		{
		case SOCKET_MESSAGE_RECHARGELOGINUSER:
			pVerClientSocket->HandleLogin(ServerBf);
			break;
		case SOCKET_MESSAGE_KEEPALIVE: case SOCKET_MESSAGE_GROUP_MEMBER:
			pVerClientSocket->HandleKeepAlive(ServerBf);
			break;
		case SOCKET_MESSAGE_GETRES:
			
			break;
		case SOCKET_MESSAGE_GETGROUP: case SOCKET_MESSAGE_GAMETITLE: case SOCKET_MESSAGE_LEAVEGROUP: case SOCKET_MESSAGE_CLIENTMSG: case SOCKET_MESSAGE_RECORD:
			pVerClientSocket->HandleGroup(ServerBf);
			break;
		case SOCKET_MESSAGE_GETACCOUNT:
			pVerClientSocket->HandleAccount(ServerBf);
			break;
		case SOCKET_MESSAGE_CLOSECLIENT:
			CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
			CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
			CLProcess::TerminateProc_For_ProcName(L"WebLogin.exe");
			CLProcess::TerminateProc_For_ProcName(L"TGPLogin.exe");
			CLProcess::TerminateProc_For_ProcName(L"tgp_daemon.exe");
			CLProcess::TerminateProc_For_ProcName(L"LolConsole.exe");
			break;
		default:
			break;
		}
	}

	pVerClientSocket->hRecvThread = NULL;
	return 0;
}

BOOL CVerClientSocket::HandleGroup(ByteBuffer& Bf)
{
	static wstring wstr;
	// 初始化数据
	static DWORD dwHero = 0;
	static DWORD dwRetainHero = 0;

	try
	{
		DWORD dwNextCmd = 0;
		Bf >> dwNextCmd;
		switch (dwNextCmd)
		{
		case SOCKET_MESSAGE_GROUP_CAPTAIN:
			// 解包 转换
			Bf >> wstr;
			Bf >> dwHero;
			Bf >> dwRetainHero;
			// 写入全局变量
			emHeroPro = (em_Hero_Pro)dwHero;
			emRetainHero = (em_Hero_Pro)dwRetainHero;

			// 地图
			CCharacter::wstrcpy_my(g_ClientGroup.wszMapName, wstr.c_str());
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"服务器返回的地图是:%s", g_ClientGroup.wszMapName);
			CConsoleConfig::SetGameMap(g_ClientGroup.wszMapName);

			// 接收名字
			TeamMemNameberLst.clear();
			for (int i = 0; i < 4; ++i)
			{
				static TempSrc32 TS;
				Bf >> wstr;
				CCharacter::wstrcpy_my(TS.wszText, wstr.c_str());
				TeamMemNameberLst.push_back(TS);
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"接收的队友名称是:%s,Hero=%X,RetainHero=%X", TS.wszText, dwHero, dwRetainHero);
			}

			g_bCaptain = TRUE;
			g_bRecvGroupMsg = TRUE;
			break;
		case SOCKET_MESSAGE_WAIT:
			break;
		case SOCKET_MESSAGE_GROUP_MEMBER:
			// 解包 转换
			Bf >> wstr;
			Bf >> dwHero;
			Bf >> dwRetainHero;

			// 地图
			CCharacter::wstrcpy_my(g_ClientGroup.wszMapName, wstr.c_str());
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"服务器返回的地图是:%s", g_ClientGroup.wszMapName);
			CConsoleConfig::SetGameMap(g_ClientGroup.wszMapName);

			// 写入全局变量
			emHeroPro = (em_Hero_Pro)dwHero;
			emRetainHero = (em_Hero_Pro)dwRetainHero;
			cout << "emHeroPro:" << dwHero << "  emRetainHero:" << dwRetainHero << endl;
			g_bCaptain = FALSE;
			g_bRecvGroupMsg = TRUE;
			break;
		case SOCKET_MESSAGE_GETGROUP: // 回发服务端的正确数据
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"dwNextCmd=%s", L"SOCKET_MESSAGE_GETGROUP");
			break;
		case SOCKET_MESSAGE_LEAVEGROUP: // 离队
			g_dwRecvMsgType = dwNextCmd;
			g_bRecvLeaveMsg = TRUE;
			break;
		case SOCKET_MESSAGE_CLIENTMSG: case SOCKET_MESSAGE_RECORD:
			g_dwRecvMsgType = dwNextCmd;
			break;
		case SOCKET_MESSAGE_GAMETITLE: // 封号
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"封号的消息已经被服务器接收了!");
			g_dwRecvMsgType = dwNextCmd;
			g_bRecvIdSealedMsg = TRUE;
			break;
		case SOCKET_MESSAGE_RETMSG:
			Bf >> wstr;
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"组队:服务器返回信息:%s", wstr.c_str());
			break;
		default:
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"dwNextCmd=%X", dwNextCmd);
			break;
		}
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"HandleGroup出现异常");
	}

	return TRUE;
}

BOOL CVerClientSocket::HandleAccount(ByteBuffer& Bf)
{
	DWORD dwNextCmd = 0;
	Bf >> dwNextCmd;
	if (dwNextCmd == SOCKET_MESSAGE_GETACCOUNT)
	{
		wstring wstr;
		Bf >> wstr;
		// 这是正常的, 旧的帐号可以继续用!
		if (CCharacter::wstrcmp_my(wstr.c_str(), g_ClientGroup.SLD.wszGameUser))
			g_bSwitchAccount = FALSE;
		else
		{
			g_bSwitchAccount = TRUE;
			g_pSharedInfo->nRunTGPCount = 0;
		}


		g_dwRecvMsgType = SOCKET_MESSAGE_GETACCOUNT;
		// 否则, 帐号可能因为服务器被删除了, 或者封号了的原因, 重新下达一个新的帐号.
		// 需要重启TGP, 大厅, 网页查询
		// Qq >> Pass >> Server >> Map
		CCharacter::wstrcpy_my(g_ClientGroup.SLD.wszGameUser, wstr.c_str());

		Bf >> wstr;
		CCharacter::wstrcpy_my(g_ClientGroup.SLD.wszPass, wstr.c_str());

		Bf >> wstr;
		CCharacter::wstrcpy_my(g_ClientGroup.wszServerName, wstr.c_str());

		Bf >> wstr;
		CCharacter::wstrcpy_my(g_ClientGroup.wszMapName, wstr.c_str());

		// 是否打人机
		//Bf >> g_ClientGroup.bMatchMachine;

		//DWORD dwValue = 0;
		//Bf >> dwValue;
		//g_ClientGroup.Difficulty = (GameDifficulty)dwValue; // 难度

		// 是否单排
		//Bf >> g_ClientGroup.bMatching;

		//CConsoleConfig::SetGameMap(g_ClientGroup.wszMapName);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"返回的QQ:%s,Pass:%s", g_ClientGroup.SLD.wszGameUser, g_ClientGroup.SLD.wszPass);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"大区:%s", g_ClientGroup.wszServerName);
		return TRUE;
	}
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"HandleAccount::dwNextCmd=%X", dwNextCmd);
	return TRUE;
}

BOOL CVerClientSocket::HandleLogin(ByteBuffer& Bf)
{
	static wstring wstr;
	DWORD dwNextCmd = 0;
	Bf >> dwNextCmd;
	g_bRecvLoginMsg = TRUE;

	if (dwNextCmd == SOCKET_MESSAGE_RETMSG)
	{
		Bf >> wstr;
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"登录:服务器返回信息:%s", wstr.c_str());
		return FALSE;
	}
	else if (dwNextCmd == SOCKET_MESSAGE_RECHARGELOGINUSER)
	{
		DWORD dwTime = 0;
		Bf >> dwTime;
		cout << "剩余时间:" << dwTime << "小时" << endl;
		if (dwTime == 0)
			return FALSE;

		Bf >> wstr;
		if (!CCharacter::wstrstr_my(wstr.c_str(),L"正常"))
		{
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"登录返回消息:%s", wstr.c_str());
			return FALSE;
		}

		static WCHAR wszText[32];
		CCharacter::wstrcpy_my(wszText, wstr.c_str());
		CCharacter::GetRemoveRight(wszText, L"正常->", wszText);
		//g_ClientGroup.SLD.dwVerCode = wcstol(wszText, NULL, 16);
		return TRUE;
	}
	else
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"登录:消息类型错误");
	}

	return FALSE;
}

BOOL CVerClientSocket::HandleKeepAlive(ByteBuffer& Bf)
{
	try
	{
		static wstring wstr;
		DWORD dwNextCmd = 0;
		Bf >> dwNextCmd;

		if (dwNextCmd == SOCKET_MESSAGE_RETMSG)
		{
			Bf >> wstr;
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"心跳:服务器返回信息:%s", wstr.c_str());
			return FALSE;
		}
		else if (dwNextCmd == SOCKET_MESSAGE_KEEPALIVE || dwNextCmd == SOCKET_MESSAGE_GROUP_MEMBER)
		{
			// 正常的心跳
			return TRUE;
		}
		else if (dwNextCmd == SOCKET_MESSAGE_GROUP_CAPTAIN)
		{
			// 返回队友的名字
			TeamMemNameberLst.clear();

			for (int i = 0; i < 4; ++i)
			{
				Bf >> wstr;
				static TempSrc32 TS;
				CCharacter::wstrcpy_my(TS.wszText, wstr.c_str());
				TeamMemNameberLst.push_back(TS);
				wcout << L"接收到一个队友:" << wstr.c_str() << L" 现在队友数量:" << TeamMemNameberLst.size() << endl;
			}

			return TRUE;
		}
		else
		{
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"心跳:消息类型错误,Code=%X", dwNextCmd);
		}
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"HandleKeepAlive出现异常");
	}
	return TRUE;
}

BOOL CVerClientSocket::CreatePacket(__in DWORD dwCmd, __out ByteBuffer& RetBf)
{
	auto fn_Get_WebPersonInfo = []
	{
		g_pSharedInfo->MyGameInfo[0].Lock();
		if (g_pSharedInfo->MyGameInfo[0].webPerson.dwLevel != 0)
		{
			auto pWebPerson = &g_pSharedInfo->MyGameInfo[0].webPerson;
			CCharacter::wstrcpy_my(g_ClientGroup.SLD.wszNiceName, pWebPerson->wszNiceName);
			g_ClientGroup.SLD.nGold = pWebPerson->dwGold;
			g_ClientGroup.SLD.nLevel = pWebPerson->dwLevel;
			g_ClientGroup.SLD.nSumWarCount = pWebPerson->dwSumWarCount;
		}
		g_pSharedInfo->MyGameInfo[0].UnLock();
	};

	auto fn_Create_SockClientGroup = [](__out ByteBuffer& RetBf)
	{
		// ComputerName
		RetBf << wstring(g_ClientGroup.wszComputerName);

		// GameUser'QQ'
		RetBf << wstring(g_ClientGroup.SLD.wszGameUser);

		// NiceName
		RetBf << wstring(g_ClientGroup.SLD.wszNiceName);

		// ServerName
		RetBf << wstring(g_ClientGroup.wszServerName);

		// MapName
		RetBf << wstring(g_ClientGroup.wszMapName);

		// nGold
		RetBf << g_ClientGroup.SLD.nGold;

		// nLevel
		RetBf << g_ClientGroup.SLD.nLevel;

		// nSumWarCount
		RetBf << g_ClientGroup.SLD.nSumWarCount;
	};

	RetBf.clear();
	RetBf << dwCmd;
	//fn_Get_WebPersonInfo();
	fn_Create_SockClientGroup(RetBf);

	if (dwCmd == SOCKET_MESSAGE_CLIENTMSG)
	{
		// 再加上一个字符串消息
		RetBf << wstring(g_wszClientMsg);
	}

	return TRUE;
}

BOOL CVerClientSocket::_Send_JoinGroup()
{
	try
	{
		DWORD dwNowTick = ::GetTickCount();
		DWORD dwTick = 0;
		g_bRecvGroupMsg = FALSE;
		g_dwRecvMsgType = NULL;

		while (GameStatus_Run && !g_bRecvGroupMsg)
		{
			// 每10s循环发送加入团队的指令
			if (::GetTickCount() - dwTick >= 5 * 1000)
			{
				ByteBuffer Bf;
				CreatePacket(SOCKET_MESSAGE_GETGROUP, Bf);
				pVerClient->post_msg((char*)Bf.contents(), Bf.size(), false);
				dwTick = ::GetTickCount();
			}
			if (g_dwRecvMsgType == SOCKET_MESSAGE_GETACCOUNT)
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"接受到的消息是让咱们换号^那就换呗!");
				return FALSE;
			}
			/*if (::GetTickCount() - dwNowTick >= 5 * 60 * 1000)
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"5分钟都组不上队伍, 重启客户端!");
				return FALSE;
			}*/
			Sleep(500);
		}
	}
	catch(...)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"_Send_JoinGroup出现异常");
	}

	return g_bRecvGroupMsg;
}

BOOL CVerClientSocket::_Send_LeaveGroup()
{
	g_bRecvLeaveMsg = FALSE;

	DWORD dwTick = 0;
	while (GameStatus_Run && !g_bRecvLeaveMsg)
	{
		// 每10s循环发送加入团队的指令
		if (::GetTickCount() - dwTick >= 10 * 1000)
		{
			ByteBuffer Bf;
			CreatePacket(SOCKET_MESSAGE_LEAVEGROUP, Bf);
			pVerClient->post_msg((char*)Bf.contents(), Bf.size(), false);
			dwTick = ::GetTickCount();
		}
		Sleep(1000);
	}

	return g_bRecvLeaveMsg;
}

BOOL CVerClientSocket::_Send_IdSealed()
{
	g_bRecvIdSealedMsg = FALSE;
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"发送封号消息!");
	DWORD dwTick = 0;
	while (GameStatus_Run && !g_bRecvIdSealedMsg)
	{
		// 每10s循环发送封号的指令
		if (::GetTickCount() - dwTick >= 10 * 1000)
		{
			ByteBuffer Bf;
			CreatePacket(SOCKET_MESSAGE_GAMETITLE, Bf);
			pVerClient->post_msg((char*)Bf.contents(), Bf.size(), false);
			dwTick = ::GetTickCount();
		}
		Sleep(1000);
	}

	return g_bRecvIdSealedMsg;
}

BOOL CVerClientSocket::_Send_GetAccount()
{
	DWORD dwTick = 0;
	g_dwRecvMsgType = NULL;
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"Send GetAccount");
	while (GameStatus_Run && g_dwRecvMsgType != SOCKET_MESSAGE_GETACCOUNT)
	{
		// 每10s循环发送封号的指令
		if (::GetTickCount() - dwTick >= 10 * 1000)
		{
			ByteBuffer Bf;
			CreatePacket(SOCKET_MESSAGE_GETACCOUNT, Bf);
			pVerClient->post_msg((char*)Bf.contents(), Bf.size(), false);
			dwTick = ::GetTickCount();
		}
		Sleep(1000);
	}
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"Recv GetAccount!");
	return g_dwRecvMsgType == SOCKET_MESSAGE_GETACCOUNT ? TRUE : FALSE;
}

BOOL CVerClientSocket::_Wait_Login()
{
	ByteBuffer Bf;
	CreatePacket(SOCKET_MESSAGE_RECHARGELOGINUSER, Bf);
	pVerClient->post_msg((char*)Bf.contents(), Bf.size(), false);

	for (int i = 0;i < 10 * 2 && !g_bRecvLoginMsg; ++i)
	{
		Sleep(500);
	}

	if (!g_bRecvLoginMsg)
	{
		wcout << L"无法从服务器接收到登录信息!" << endl;
		return FALSE;
	}
	Sleep(500);
	//return g_ClientGroup.SLD.dwVerCode != NULL;
	return TRUE;
}

BOOL CVerClientSocket::_Send_ClientMsg(LPCWSTR pwszClientMsg)
{
	if (!pVerClient->is_connected())
		return TRUE;

	DWORD dwTick = 0;

	g_dwRecvMsgType = NULL;
	CCharacter::wstrcpy_my(g_wszClientMsg, pwszClientMsg, sizeof(g_wszClientMsg));

	while (GameStatus_Run && g_dwRecvMsgType != SOCKET_MESSAGE_CLIENTMSG)
	{
		// 每10s循环发送封号的指令
		if (::GetTickCount() - dwTick >= 10 * 1000)
		{
			ByteBuffer Bf;
			CreatePacket(SOCKET_MESSAGE_CLIENTMSG, Bf);
			pVerClient->post_msg((char*)Bf.contents(), Bf.size(), false);
			dwTick = ::GetTickCount();
		}
		Sleep(1000);
	}

	return g_dwRecvMsgType == SOCKET_MESSAGE_CLIENTMSG ? TRUE : FALSE;
}

BOOL CVerClientSocket::_Send_Record()
{
	DWORD dwTick = 0;

	g_dwRecvMsgType = NULL;

	while (GameStatus_Run && g_dwRecvMsgType != SOCKET_MESSAGE_RECORD)
	{
		// 每10s循环发送封号的指令
		if (::GetTickCount() - dwTick >= 10 * 1000)
		{
			ByteBuffer Bf;
			CreatePacket(SOCKET_MESSAGE_RECORD, Bf);
			pVerClient->post_msg((char*)Bf.contents(), Bf.size(), false);
			dwTick = ::GetTickCount();
		}
		Sleep(1000);
	}

	return g_dwRecvMsgType == SOCKET_MESSAGE_RECORD ? TRUE : FALSE;
}