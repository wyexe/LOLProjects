#include "LoginManger.h"

#define _SELF L"LoginManger.cpp"

/////////////////全局变量区/////////////////////////////////////////////////////////
vector<LoginGameUser> CLoginManger::LgUserLst;


////////////////////函数实现//////////////////////////////////////////////////////
CLoginManger::CLoginManger()
{
}

CLoginManger::~CLoginManger()
{
}

LoginGameUser* CLoginManger::_ExistCard(CardStruct & Card)
{
	// 获取卡号的Crc, 用Crc去搜索, 比起用字符串去搜索较省时
	CLPublic::GetCRC32_DWORD((LPSTR)Card.szCard, wcslen(Card.szCard) + 1, Card.dwCardCode);

	for (auto& lgUser : LgUserLst)
	{
		if (lgUser.Card.dwCardCode == Card.dwCardCode && CCharacter::wstrcmp_my(lgUser.Card.szCard, Card.szCard))
			return &lgUser;
	}

	return NULL;
}

BOOL CLoginManger::_EnoughTime(LoginGameUser* pLgGameUser)
{
	if (::GetTickCount() - pLgGameUser->dwSurplusTick > pLgGameUser->dwSurplusHour * 60 * 60 * 1000)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"%s的卡号已经访问超时了,Card:%s", pLgGameUser->wszLoginName, pLgGameUser->Card.szCard);
		return FALSE;
	}
	return TRUE;
}

BOOL CLoginManger::_EnoughLoginTime(LoginGameUser* pLgGameUser)
{
	if (::GetTickCount() - pLgGameUser->dwLoginTick < 1 * 60 * 1000)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"%s请求登录在%ds之内,Card:%s", pLgGameUser->wszLoginName, (::GetTickCount() - pLgGameUser->dwLoginTick) / 1000, pLgGameUser->Card.szCard);
		return FALSE;
	}
	pLgGameUser->dwLoginTick = ::GetTickCount();
	return TRUE;
}

BOOL CLoginManger::_SameIp(LoginGameUser* LgGameUser, LPCWSTR pwszIp)
{
	return CCharacter::wstrcmp_my(LgGameUser->wszIp, pwszIp) ? TRUE : FALSE;
}

BOOL CLoginManger::_TimeOut_ByCount(LoginGameUser* LgGameUser)
{
	// 判断达到20次后,时间是否超过1分钟
	if (++LgGameUser->dwUseCount >= 20 * 15)
	{
		if (::GetTickCount() - LgGameUser->dwTickCount > 60 * 1000)
		{
			// 正常, 重新置0
			LgGameUser->dwUseCount = 0;
			LgGameUser->dwTickCount = ::GetTickCount();
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

BOOL CLoginManger::VerCard(SockLoginData& SLD, LPCWSTR pwszIp, LPWSTR pwszMsg)
{
	try
	{
		/*LoginGameUser* pLgUser = _ExistCard(SLD.Card);
		if (pLgUser != NULL)
		{
			// 判断卡号是否过期了
			if (!_EnoughTime(pLgUser))
			{
				CCharacter::wstrcpy_my(pwszMsg, L"该卡号已经过期啦!时间不足!");
				goto lb_Exit;
			}

			// 判断IP是否跟初始化登录的一致
			if (!_SameIp(pLgUser, pwszIp))
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"CurIp:%s,LocalIp:%s", pwszIp, pLgUser->wszIp);
				wsprintfW(pwszMsg, L"该卡号已经在别的机器上使用了……抢号的IP是:%s,现在IP:%s", pLgUser->wszIp, pwszIp);
				goto lb_Exit;
			}

			// 判断令牌
			if (pLgUser->dwVerCode != SLD.dwVerCode)
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"令牌不一致,卡号:%s,User:%s", pLgUser->Card.szCard, pLgUser->wszLoginName);
				CCharacter::wstrcpy_my(pwszMsg, L"不可能啊? 你直接跳过登录器了?");
				goto lb_Exit;
			}


			//  次数使用校验
			if (!_TimeOut_ByCount(pLgUser))
			{
				CCharacter::wstrcpy_my(pwszMsg, L"1分钟内获取次数过多!");
				goto lb_Exit;
			}

			// 帐号校验(可以不要,百无聊赖)
			/ *if (!CCharacter::wstrcmp_my(pLgUser->wszGameUserName, GameUser.wchGameUserName))
			{
			CCharacter::wstrcpy_my(pwszMsg, L"帐号不符合!");
			goto lb_Exit;
			}* /

			return TRUE;
		}
		else
		{
			wsprintfW(pwszMsg, L"此卡号:%s,不存在", SLD.Card.szCard);
		}*/
	}
	catch (...)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"VerGameUser发生了异常");
		CCharacter::wstrcpy_my(pwszMsg, L"服务端VerGameUser发生异常!");
	}
//lb_Exit:;
	return FALSE;
}

LoginGameUser* CLoginManger::InitLoginUser(SockLoginData& SLD, LPCWSTR pwszIp, LPWSTR pwszMsg)
{
	/*try
	{
		LoginGameUser* pLgUser = _ExistCard(SLD.Card);
		if (pLgUser != NULL)
		{
			// 判断卡号是否过期了
			if (!_EnoughTime(pLgUser))
			{
				CCharacter::wstrcpy_my(pwszMsg, L"卡号已经过期啦!");
				goto lb_Exit;
			}

			/ *if (!_EnoughLoginTime(pLgUser))
			{
				CCharacter::wstrcpy_my(pwszMsg, L"登录请求需要在1分钟内不可重复请求");
				goto lb_Exit;
			}* /

			// 重置IP
			CCharacter::wstrcpy_my(pLgUser->wszIp, pwszIp);

			// 重置令牌
			pLgUser->dwVerCode = CCharacter::GetRand_For_DWORD();
			pLgUser->dwVerTick = ::GetTickCount();

			// 重置请求数量
			pLgUser->dwUseCount = 0;
			pLgUser->dwTickCount = ::GetTickCount();

			// 记录帐号
			CCharacter::wstrcpy_my(pLgUser->wszLoginName, SLD.wszLoginUser);

			wsprintfW(pwszMsg, L"正常->%X", pLgUser->dwVerCode);

			return pLgUser;
		}
		else
		{
			wsprintfW(pwszMsg, L"此卡号:%s,不存在", SLD.Card.szCard);
		}
	}
	catch (...)
	{
		CCharacter::wstrcpy_my(pwszMsg, L"服务端CreateNewLoginGameUser发生异常!");
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CreateNewLoginGameUser发生了异常");
	}
lb_Exit:;*/
	return NULL;
}

DWORD CLoginManger::_CheckCard(LPCWSTR pwszCard)
{
	if (wcslen(pwszCard) > 36)
		return 0;

	if (wcslen(pwszCard) < 20)
		return 0;

	WCHAR wszHour[5] = { 0 };
	for (int i = 0; i < 4; ++i)
		wszHour[i] = pwszCard[i];

	wszHour[4] = '\0';
	return _wtoi(wszHour);
}

BOOL CLoginManger::AddNewCard(CardStruct& Card, int nTime, DWORD dwToken, LPCWSTR pwszIp)
{
	try
	{
		// 判断卡号是否已经存在了……虽然是多此一举
		BOOL bExist = FALSE;
		for (UINT i = 0; i < LgUserLst.size(); ++i)
		{
			if (CCharacter::wstrcmp_my(LgUserLst.at(i).Card.szCard, Card.szCard))
			{
				bExist = TRUE;
				break;
			}
		}

		if (bExist)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"……竟然充值成功的卡,会已经出现过……不太可能啊!");
			return TRUE;
		}

		// 添加新的
		LoginGameUser LgGameUser;
		ZeroMemory(&LgGameUser, sizeof(LgGameUser));

		// 以CRC的值作为索引去搜索
		CLPublic::GetCRC32_DWORD((LPSTR)Card.szCard, wcslen(Card.szCard) + 1, Card.dwCardCode);

		memcpy(&LgGameUser.Card, &Card, sizeof(Card));

		// 写入剩余时间
		LgGameUser.dwSurplusHour = nTime;
		LgGameUser.dwSurplusTick = ::GetTickCount();

		// 重置IP
		CCharacter::wstrcpy_my(LgGameUser.wszIp, pwszIp);

		// 写入令牌
		LgGameUser.dwVerCode = dwToken;
		LgGameUser.dwVerTick = ::GetTickCount();

		// 写入登录时间
		LgGameUser.dwLoginTick = 0;

		LgUserLst.push_back(LgGameUser);

		return TRUE;
	}
	catch (...)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"AddNewCard发生了异常");
	}
	return FALSE;
}

BOOL CLoginManger::_InitCard(vector<CardInfo>& vlst)
{
	try
	{
		for (UINT i = 0; i < vlst.size(); ++i)
		{
			AddNewCard(vlst.at(i).Card, _wtoi(vlst.at(i).wszSurplusHour), NULL, L"127.0.0.1");
		}

		return TRUE;
	}
	catch (...)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"_InitCard发生了异常");
	}
	return FALSE;
}

DWORD CLoginManger::_GetCardTime(LPCWSTR pwszCardNo, DWORD dwToken, LPCWSTR pwszIp)
{
	DWORD dwTime = 0;
	try
	{
		CardStruct Card = { 0 };
		CCharacter::wstrcpy_my(Card.szCard, pwszCardNo);
		LoginGameUser* pLgu = _ExistCard(Card);
		if (pLgu != NULL)
		{
			if (dwToken != NULL && pLgu->dwVerCode != dwToken)
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"_GetCardTime Token UnComp!");
				return 0;
			}
			
			int nTime = (int)(pLgu->dwSurplusHour - ((::GetTickCount() - pLgu->dwSurplusTick) / 1000 / 60 / 60));
			if (nTime > 0)
			{
				dwTime = nTime;
				pLgu->dwVerTick = ::GetTickCount();
				CCharacter::wstrcpy_my(pLgu->wszIp, pwszIp);
			}
		}
	}
	catch (...)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"_GetCardTime发生了异常");
	}
	return dwTime;
}