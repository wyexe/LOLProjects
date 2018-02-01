#include "ConsoleConfig.h"

#define _SELF L"ConsoleConfig.cpp"

CConsoleConfig::CConsoleConfig()
{
}

CConsoleConfig::~CConsoleConfig()
{
}

BOOL CConsoleConfig::WriteFightCount(DWORD dwCount)
{
	WCHAR wszValue[32] = { 0 };
	WCHAR wszLoginPath[MAX_PATH] = { 0 };


	::GetCurrentDirectoryW(MAX_PATH, wszLoginPath);
	lstrcatW(wszLoginPath, L"\\Login.ini");

	// format
	wsprintfW(wszValue, L"%d", dwCount);

	return WritePrivateProfileStringW(L"Config", L"FightCount", wszValue, wszLoginPath);
}

DWORD CConsoleConfig::GetFightCount()
{
	WCHAR wszValue[32] = { 0 };
	WCHAR wszLoginPath[MAX_PATH] = { 0 };


	::GetCurrentDirectoryW(MAX_PATH, wszLoginPath);
	lstrcatW(wszLoginPath, L"\\Login.ini");

	if (!GetPrivateProfileStringW(L"Config", L"FightCount", L"Error", wszValue, 32, wszLoginPath) || CCharacter::wstrcmp_my(wszValue, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"读取Login.ini -> %s失败!", L"FightCount");
		return 0;
	}

	return (DWORD)_wtoi(wszValue);
}

BOOL CConsoleConfig::WriteGameResult()
{
	auto fnAppendFile = [](LPCSTR pszText)
	{
		FILE* pFile = NULL;
		fopen_s(&pFile, "GameResult.txt", "a+");
		if (pFile == NULL)
			return false;

		fseek(pFile, 0, SEEK_END);
		fwrite(pszText, sizeof(char), strlen(pszText) + 1, pFile);
		fclose(pFile);
		return true;
	};

	SYSTEMTIME st;
	static CHAR szText[64] = { 0 };

	::GetLocalTime(&st);
	wsprintfA(szText, "%4d-%2d-%2d %2d:%2d:%2d 战斗结果:", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	HANDLE hMutexGameResult_Win = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, Mutex_GameResult_Win);
	if (hMutexGameResult_Win != NULL)
	{
		lstrcatA(szText, "胜利\r\n");
		fnAppendFile(szText);
		::ReleaseMutex(hMutexGameResult_Win);
		::CloseHandle(hMutexGameResult_Win);
		hMutexGameResult_Win = NULL;
		return TRUE;
	}

	HANDLE hMutexGameResult_Lose = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, Mutex_GameResult_Lose);
	if (hMutexGameResult_Lose != NULL)
	{
		lstrcatA(szText, "失败\r\n");
		fnAppendFile(szText);
		::ReleaseMutex(hMutexGameResult_Lose);
		::CloseHandle(hMutexGameResult_Lose);
		hMutexGameResult_Lose = NULL;
		return TRUE;
	}

	lstrcatA(szText, "未知\r\n");
	fnAppendFile(szText);
	return FALSE;
}

BOOL CConsoleConfig::SaveGameMap(LPCWSTR pwszMapName)
{
	WCHAR wszValue[32] = { 0 };
	WCHAR wszConfigPath[MAX_PATH] = { 0 };

	::GetCurrentDirectoryW(MAX_PATH, wszConfigPath);
	lstrcatW(wszConfigPath, L"\\Config.ini");

	return WritePrivateProfileStringW(L"Login", L"Map", pwszMapName, wszConfigPath);
}

BOOL CConsoleConfig::SetGameMap(LPCWSTR wszMapName)
{
	if (!GetGameMap_By_Name(wszMapName, g_MapType))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"获取地图类型失败,wszMapName=%s", wszMapName);
		return FALSE;
	}
	else
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"SetGameMap:%s", wszMapName);
	}
	
	auto CloseGameMapMutex = [](LPCWSTR pwszMutexName)
	{
		HANDLE hGameMapMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, pwszMutexName);
		if (hGameMapMutex != NULL)
		{
			::ReleaseMutex(hGameMapMutex);
			::CloseHandle(hGameMapMutex);
			hGameMapMutex = NULL;
		}
	};

	auto CreateGameMapMutex = [](const MapType& Mt)
	{
		if (Mt == MapType_召唤师峡谷)
			::CreateMutexW(NULL, FALSE, L"CL_LOL_Game_MapType_0");
		else if (Mt == MapType_水晶之痕)
			::CreateMutexW(NULL, FALSE, L"CL_LOL_Game_MapType_1");
		else if (Mt == MapType_扭曲丛林)
			::CreateMutexW(NULL, FALSE, L"CL_LOL_Game_MapType_2");
		else if (Mt == MapType_嚎哭深渊)
			::CreateMutexW(NULL, FALSE, L"CL_LOL_Game_MapType_3");
	};

	CloseGameMapMutex(L"CL_LOL_Game_MapType_0");
	CloseGameMapMutex(L"CL_LOL_Game_MapType_1");
	CloseGameMapMutex(L"CL_LOL_Game_MapType_2");
	CloseGameMapMutex(L"CL_LOL_Game_MapType_3");
	CreateGameMapMutex(g_MapType);
	return TRUE;
}

BOOL CConsoleConfig::GetGameMap_By_Name(LPCWSTR pwszMapName, MapType& Mt)
{
	BOOL bRetCode = FALSE;
	if (CCharacter::wstrcmp_my(pwszMapName, L"召唤师峡谷"))
	{
		Mt = MapType_召唤师峡谷;
		bRetCode = TRUE;
	}
	else if (CCharacter::wstrcmp_my(pwszMapName, L"水晶之痕"))
	{
		Mt = MapType_水晶之痕;
		bRetCode = TRUE;
	}
	else if (CCharacter::wstrcmp_my(pwszMapName, L"扭曲丛林"))
	{
		Mt = MapType_扭曲丛林;
		bRetCode = TRUE;
	}
	else if (CCharacter::wstrcmp_my(pwszMapName, L"嚎哭深渊"))
	{
		Mt = MapType_嚎哭深渊;
		bRetCode = TRUE;
	}
	return bRetCode;
}

BOOL CConsoleConfig::UpdateHosts()
{
	FILE* pFile = NULL;
	fopen_s(&pFile, "C:\\Windows\\System32\\drivers\\etc\\hosts", "w");
	if (pFile == NULL)
		return FALSE;

	//char szText[] = { "127.0.0.1 lol.qq.com" };
	char szText[] = { "" };
	fwrite(szText, 1, strlen(szText) + 1, pFile);
	fclose(pFile);
	return TRUE;
}

BOOL CConsoleConfig::GetTGPPath_By_Reg(LPWSTR pwszTGPPath)
{
	BOOL bRetCode = FALSE;
	HKEY hKey;
	DWORD dwRet;
	DWORD dwLen = MAX_PATH;

	dwRet = RegOpenKey(HKEY_CURRENT_USER, L"Software\\Tencent\\bugReport\\TGP\\", &hKey); //打开注册表左侧树
	if (dwRet == ERROR_SUCCESS)
	{
		dwRet = RegQueryValueExW(hKey, L"InstallDir", NULL, NULL, (BYTE *)pwszTGPPath, &dwLen);  //读取右侧的字符串值
		if (dwRet == ERROR_SUCCESS)
		{
			//到这里pwszTGPPath里面就是要读取的值
			bRetCode = TRUE;
		}
	}
	RegCloseKey(hKey);
	return bRetCode;
}

BOOL CConsoleConfig::GetGamePath_By_Process(LPWSTR pwszGamePath)
{
	if (CLProcess::Is_Exist_Process_For_ProcName(LOL_CLIENTPROC_NAME))
	{
		MODULEENTRY32 ME32 = { 0 };
		CLProcess::GetProcessModule_For_Name(LOL_CLIENTPROC_NAME, LOL_CLIENTPROC_NAME, ME32);

		CCharacter::GetRemoveLeft(ME32.szExePath, L"\\Air", pwszGamePath);
		return TRUE;
	}
	return FALSE;
}