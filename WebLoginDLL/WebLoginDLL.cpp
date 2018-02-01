// WebLoginDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "WebLoginDLL.h"
#include "../LOLProjects_China/GameStruct.h"

#define _SELF L"WebLoginDLL.cpp"

// This is the constructor of a class that has been exported.
// see WebLoginDLL.h for the class definition
CWebLoginDLL::CWebLoginDLL()
{
	return;
}

///////var///////////////////////////////////////////////////////////////////
HANDLE g_hFileMap = NULL;
SHARED_INFO* g_pSharedInfo = NULL;

// Init MapView
BOOL InitMapView()
{
	//检查文件映射
	g_hFileMap = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SZFILE_NAME_SHAREDINFO);
	if (g_hFileMap == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"无法打开文件映射!");
		return FALSE;
	}

	g_pSharedInfo = (PSHARED_INFO)MapViewOfFile(g_hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(SHARED_INFO));
	if (!g_pSharedInfo)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"映射内存失败!");
		return FALSE;
	}

	return TRUE;
}

WEBLOGINDLL_API int __cdecl SetInfo(int nLevel, int nSumFightCount, int nGold, LPCWSTR pwszNiceName)
{
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"nLevel=%d", nLevel);
	if (g_pSharedInfo == NULL)
		return 0;

	__try
	{
		g_pSharedInfo->MyGameInfo[0].Lock();
		auto pWebPerson = &g_pSharedInfo->MyGameInfo[0].webPerson;
		pWebPerson->dwLevel = (DWORD)nLevel;
		pWebPerson->dwGold = (DWORD)nGold;
		pWebPerson->dwSumWarCount = (DWORD)nSumFightCount;
		CCharacter::wstrcpy_my(pWebPerson->wszNiceName, pwszNiceName);

		g_pSharedInfo->MyGameInfo[0].UnLock();
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"SetInfo发生了异常");
	}
	return 1;
}

WEBLOGINDLL_API int __cdecl GetInfo(int& nAreId, LPWSTR pwszQQUser, LPWSTR pwszQQPass, LPWSTR pwszdamaUser, LPWSTR pwszdamaPass)
{
	// 获取文件映射
	if (!InitMapView())
		return 0;

	g_pSharedInfo->MyGameInfo[0].Lock();
	auto pWebInfo = &g_pSharedInfo->MyGameInfo[0].webInfo;

	CCharacter::wstrcpy_my(pwszQQUser, pWebInfo->wszGameUser);
	CCharacter::wstrcpy_my(pwszQQPass, pWebInfo->wszGamePass);
	CCharacter::wstrcpy_my(pwszdamaUser, pWebInfo->wszdamaUser);
	CCharacter::wstrcpy_my(pwszdamaPass, pWebInfo->wszdamaPass);
	nAreId = (int)pWebInfo->dwAreId;
	g_pSharedInfo->MyGameInfo[0].UnLock();
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"网页查询的QQ号是:%s,Pass:%s", pwszQQUser, pwszQQPass);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"AreId:%d", nAreId);
	return 1;
}