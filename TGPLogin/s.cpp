#include "../../MyTools/MyTools/CLHead.h"
#include "../LolConsole/BmpFind.h"
#include "CrackCaptchaAPI.h"
#include <iostream>
using namespace std;
#define _SELF L"TGPLogin.cpp"

static BOOL CALLBACK EnumWindowsProc(HWND h, LPARAM lParam)
{
	if (IsWindow(h) && IsWindowVisible(h) && (GetWindowLong(h, GWL_EXSTYLE)&WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW && (GetWindowLong(h, GWL_HWNDPARENT) == 0))
	{
		WCHAR wszText[MAX_PATH];
		::RealGetWindowClass(h, wszText, MAX_PATH);
		WCHAR wszTitle[MAX_PATH];
		::GetWindowText(h, wszTitle, MAX_PATH);
		if (!CCharacter::wstrcmp_my(wszText, L"") && !CCharacter::wstrcmp_my(wszTitle, L""))
		{
			if (!CCharacter::wstrcmp_my(wszText, L"TWINCONTROL"))
			{
				::ShowWindow(h, SW_SHOWMINIMIZED);
			}
		}
	}


	return TRUE;
}

VOID CreateEndMutex()
{
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"TGP登录结束!");
	::CreateMutexW(NULL, FALSE, L"TGPLogin_End_Mutex");
	Sleep(-1);
}

VOID ShowTGP()
{
	HWND hWndLuncher = ::FindWindowW(L"TWINCONTROL", L"腾讯游戏平台");
	if (hWndLuncher == NULL)
	{
		hWndLuncher = ::FindWindowW(L"TWINCONTROL", NULL);
		if (hWndLuncher == NULL)
		{
			CCharacter::wstrcpy_my(g_pSharedInfo->wszTGPMsg, L"错误:握草, TGP都还没启动呢");
			CreateEndMutex();
			return;
		}
	}

	ShowWindow(hWndLuncher, SW_RESTORE);
	::SetWindowPos(hWndLuncher, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SwitchToThisWindow(hWndLuncher, TRUE);
}

BOOL GetTGPPath(LPWSTR pwszTgpPath)
{
	BOOL bRetCode = FALSE;
	HKEY hKey;
	DWORD dwRet;
	DWORD dwLen = MAX_PATH;

	dwRet = RegOpenKey(HKEY_CURRENT_USER, L"Software\\Tencent\\bugReport\\TGP\\", &hKey); //打开注册表左侧树
	if (dwRet == ERROR_SUCCESS)
	{
		dwRet = RegQueryValueExW(hKey, L"InstallDir", NULL, NULL, (BYTE *)pwszTgpPath, &dwLen);  //读取右侧的字符串值
		if (dwRet == ERROR_SUCCESS)
		{
			//到这里tcPath里面就是要读取的值
			bRetCode = TRUE;
		}
	}
	RegCloseKey(hKey);
	return FALSE;
}

BOOL ClickPoint(int x, int y)
{
	for (int i = 0; i < 1; ++i)
	{
		// 设置鼠标
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, NULL, NULL);
		mouse_event(MOUSEEVENTF_LEFTUP, x, y, NULL, NULL);
	}
	return TRUE;
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
		return FALSE;
	}

	g_pSharedInfo = (PSHARED_INFO)MapViewOfFile(g_hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(SHARED_INFO));
	if (!g_pSharedInfo)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL RunTGP(LPCWSTR pwszTGPPath)
{
	if (!CLProcess::Is_Exist_Process_For_ProcName(L"tgp_daemon.exe"))
	{
		WCHAR wszTempPath[MAX_PATH] = { 0 };
		CCharacter::wstrcpy_my(wszTempPath, pwszTGPPath);

		lstrcatW(wszTempPath, L"\\tpf_ui\\res\\login\\pic_check.png");
		if (CLPublic::FileExit(wszTempPath)) // 删除验证码图片
			::DeleteFileW(wszTempPath);

		CPrintLog::PrintLog_W(_SELF, __LINE__, L"创建进程:%s", pwszTGPPath);
		STARTUPINFOW si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		WCHAR wszText[MAX_PATH] = { 0 };
		CCharacter::wstrcpy_my(wszText, pwszTGPPath);
		lstrcatW(wszText, L"\\tgp_daemon.exe");
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		BOOL bRet = ::CreateProcessW(
			NULL,
			wszText,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&si,
			&pi);
		Sleep(15 * 1000);
	}

	return CLProcess::Is_Exist_Process_For_ProcName(L"tgp_daemon.exe");
}

typedef enum _GameServerType
{
	GameServerType_Telecom,	// 电信
	GameServerType_Netcom,	// 网通
	GameServerType_Other	// 其他
}GameServerType;

typedef struct _ServerRect
{
	GameServerType ServerType;
	int x;
	int y;
}ServerRect;
BOOL GetServerInfo_By_Name(__in LPCWSTR pwszServerName, __out ServerRect& SR)
{
	typedef struct _GameServerInfo
	{
		WCHAR wszServerName[32];	// 服务器名
		GameServerType ServerType;	// 服务器分类
	}GameServerInfo;

	const int nMaxLen = 27;
	static GameServerInfo ServerInfoArray[nMaxLen] = {
		L"艾欧尼亚", GameServerType_Telecom,
		L"祖安", GameServerType_Telecom,
		L"诺克萨斯", GameServerType_Telecom,
		L"班德尔城", GameServerType_Telecom,
		L"皮尔特沃夫", GameServerType_Telecom,
		L"战争学院", GameServerType_Telecom,
		L"巨神峰", GameServerType_Telecom,
		L"雷瑟守备", GameServerType_Telecom,
		L"裁决之地", GameServerType_Telecom,
		L"黑色玫瑰", GameServerType_Telecom,
		L"暗影岛", GameServerType_Telecom,
		L"钢铁烈阳", GameServerType_Telecom,
		L"水晶之痕", GameServerType_Telecom,
		L"均衡教派", GameServerType_Telecom,
		L"影流", GameServerType_Telecom,
		L"守望之海", GameServerType_Telecom,
		L"征服之海", GameServerType_Telecom,
		L"卡拉曼达", GameServerType_Telecom,
		L"皮城警备", GameServerType_Telecom,
		L"比尔吉沃特", GameServerType_Netcom,
		L"德玛西亚", GameServerType_Netcom,
		L"弗雷尔卓德", GameServerType_Netcom,
		L"无畏先锋", GameServerType_Netcom,
		L"恕瑞玛", GameServerType_Netcom,
		L"扭曲丛林", GameServerType_Netcom,
		L"巨龙之巢", GameServerType_Netcom,
		L"教育网专区", GameServerType_Other
	};

	auto GetRowColum_ByIndex = [](__in int nIndex, __out int& nRow, __out int& nColum)
	{
		const int nMaxRow = 4;
		const int nMaxColum = 4;

		if (nIndex % nMaxRow != 0)
			nRow = nIndex / nMaxRow + 1;
		else
			nRow = nIndex / nMaxRow;

		if (nIndex % nMaxColum == 0)
			nColum = nMaxColum;
		else
			nColum = nIndex % nMaxColum;
	};

	for (int i = 0; i < nMaxLen; ++i)
	{
		if (CCharacter::wstrcmp_my(pwszServerName, ServerInfoArray[i].wszServerName))
		{
			int nIndex = 0;
			if (ServerInfoArray[i].ServerType == GameServerType_Netcom)
				nIndex = i - 19;
			else if (ServerInfoArray[i].ServerType == GameServerType_Other)
				nIndex = 0;
			else
				nIndex = i;

			int nRow, nColum;
			GetRowColum_ByIndex(nIndex + 1, nRow, nColum);
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"i=%d,nRow=%d,nColum=%d", nIndex + 1, nRow, nColum);
			SR.ServerType = ServerInfoArray[i].ServerType;
			SR.x = 100 + (nColum - 1) * 170;
			SR.y = 500 + (nRow - 1) * 35;
			return TRUE;
		}
	}
	return FALSE;
}

int main(int argc, CHAR* argv[])
{
	if (argc <= 1)
		return 0;
	if (!CCharacter::strcmp_my(argv[1], "-start"))
		return 0;


	if (!InitMapView())
	{
		CCharacter::wstrcpy_my(g_pSharedInfo->wszTGPMsg, L"错误:内存映射失败");
		CreateEndMutex();
	}

	if (!RunTGP(g_pSharedInfo->wszTGPPath))
	{
		CCharacter::wstrcpy_my(g_pSharedInfo->wszTGPMsg, L"错误:启动TGP失败");
		CreateEndMutex();
	}

	HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"TGPLogin_End_Mutex");
	if (hMutex != NULL)
	{
		::ReleaseMutex(hMutex);
		::CloseHandle(hMutex);
		hMutex = NULL;
	}

	// 正常显示一下TGP
	ShowTGP();

	// 除了TGP以外, 其他所有窗口都最小化
	EnumWindows(EnumWindowsProc, NULL);

	// 初始化找图的参数
	CBmpFind dm;
	int x, y;
	static RECT LauncherRect;
	WCHAR wszPath[MAX_PATH] = { 0 };
	HWND hTGP = NULL;
	CHAR szCheckPicPath[MAX_PATH] = { 0 };

	CCharacter::UnicodeToASCII(g_pSharedInfo->wszTGPPath, szCheckPicPath);
	lstrcatA(szCheckPicPath, "\\tpf_ui\\res\\login\\pic_check.png");

	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Bmp");
	dm.SetPath(wszPath);
	dm.BindWindow(::GetDesktopWindow());

	// 开始找图
	for (int i = 0; i < 60; ++i)
	{
		Sleep(1000);

		hTGP = ::FindWindowW(L"TWINCONTROL", L"腾讯游戏平台");
		::GetWindowRect(hTGP, &LauncherRect);

		HWND hWndLuncher = ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net 客户端");
		if (hWndLuncher != NULL)
		{
			CCharacter::wstrcpy_my(g_pSharedInfo->wszTGPMsg, L"成功启动大厅!");
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"成功启动大厅!");
			CreateEndMutex();
			break;
		}

		if (g_pSharedInfo->nRunTGPCount > 1)
		{
			// 只搜索开始游戏!
			if (dm.FindPic(0, 0, 1920, 1080, L"TGPMainPage.bmp", x, y))
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"打开游戏助手!x=%d,y=%d,l=%d,r=%d", x, y, LauncherRect.left, LauncherRect.top);
				ClickPoint(x + 200, y + 117);
				Sleep(10 * 1000);
			}
			continue;
		}

		static bool bMoveWindow = false;
		if (!bMoveWindow && LauncherRect.right - LauncherRect.left < 300)
		{
			auto fnFixTGP = [&hTGP]
			{
				hTGP = ::FindWindowW(L"TWINCONTROL", L"腾讯游戏平台");
				RECT r;
				::GetWindowRect(hTGP, &r);
				int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
				int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

				nScreenWidth -= (r.right - r.left)/*计算TGP的宽度*/ + 50/*给右边留那么一丢丢距离*/;
				nScreenHeight = 50;
				MoveWindow(hTGP, nScreenWidth, nScreenHeight, r.bottom - r.top, r.right - r.left, TRUE);
			};
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"固定窗口到右上角");
			// 固定窗口到右上角, 只固定一次!
			bMoveWindow = true;
			fnFixTGP();
		}

		if (dm.FindPic(0, 0, 1920, 1080, L"TGPLogin.bmp", x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"没错, 正在TGP登录页面,User:%s,Pass:%s", g_pSharedInfo->MyGameInfo[0].webInfo.wszGameUser, g_pSharedInfo->MyGameInfo[0].webInfo.wszGamePass);
			ClickPoint(LauncherRect.left + 430, LauncherRect.top + 75);
			Sleep(1000);
			for (int i = 0; i < 20; ++i)
				CLPublic::SendKey(8);

			CLPublic::SendKeys(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameUser);
			Sleep(1000);

			ClickPoint(LauncherRect.left + 430, LauncherRect.top + 121);
			for (int i = 0; i < 20; ++i)
				CLPublic::SendKey(8);

			Sleep(1000);
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"Pass:%s", g_pSharedInfo->MyGameInfo[0].webInfo.wszGamePass);
			CLPublic::SendKeys(g_pSharedInfo->MyGameInfo[0].webInfo.wszGamePass);
			Sleep(1000);
			ClickPoint(LauncherRect.left + 377, LauncherRect.top + 205);
			Sleep(2 * 1000);
			continue;
		}

		if (CLPublic::FileExistA(szCheckPicPath))
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"出现验证码了!");
			CHAR szdamaUser[32];
			CHAR szdamaPass[32];
			CHAR szCode[32];
			CCharacter::UnicodeToASCII(g_pSharedInfo->MyGameInfo[0].webInfo.wszdamaUser, szdamaUser);
			CCharacter::UnicodeToASCII(g_pSharedInfo->MyGameInfo[0].webInfo.wszdamaPass, szdamaPass);
			typedef int(CALLBACK *pD2File)(const char *pszSoftwareId, const char *pszUserName, const char *pszUserPassword, \
				const char *pszFilePath, unsigned short usTimeout, unsigned long ulVCodeTypeID, char *pszVCodeText);

			HMODULE hm = ::LoadLibraryW(L"CrackCaptchaAPI.dll");
			if (hm == NULL)
			{
				MessageBoxW(NULL, L"缺少CrackCaptchaAPI.dll", L"", NULL);
				return TRUE;
			}

			pD2File fnD2File = (pD2File)::GetProcAddress(hm, "D2File");
			CPrintLog::PrintLog_A("Hall.cpp", __LINE__, "答题帐号:%s,%s", szdamaUser, szdamaPass);
			int nRet = fnD2File("f2e0d5bfad94c9e325e1a7b707f3fcc9", szdamaUser, szdamaPass, szCheckPicPath, 30, 101, szCode);
			if (nRet < 0)
			{
				CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"无法答题!可能是帐号没钱了吧?nRet=%d", nRet);
				CLPublic::ForceExit();
			}

			// 删除验证码图片
			::DeleteFileA(szCheckPicPath);
			CPrintLog::PrintLog_A("Hall.cpp", __LINE__, "验证码:%s", szCode);
			ClickPoint(LauncherRect.left + 180, LauncherRect.top + 100);
			Sleep(1000);
			for (int i = 0; i < 20; ++i)
				CLPublic::SendKey(8);

			Sleep(1000);
			CLPublic::SendKeys(szCode);
			ClickPoint(LauncherRect.left + 201, LauncherRect.top + 222);
			Sleep(5 * 1000);
			continue;
		}

		if (dm.FindPic(0, 0, 1920, 1080, L"TGPLoginFaild.bmp", x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"TGP登录失败,密码错或者冻结了");
			CCharacter::wstrcpy_my(g_pSharedInfo->wszTGPMsg, L"错误:TGP登录失败,密码错或者冻结了");
			CreateEndMutex();
			continue;
		}

		if (dm.FindPic(0, 0, 1920, 1080, L"TGP_MsgClose.bmp", x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"TGP X掉消息框");
			ClickPoint(x + 10, y + 10);
			Sleep(8 * 1000);
			continue;
		}

		if (dm.FindPic(0, 0, 1920, 1080, L"TGPNextStep.bmp", x, y))
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"TGP下一步设置");
			ClickPoint(x + 27, y - 30);
			Sleep(5 * 1000);
			continue;
		}

		if (dm.FindPic(0, 0, 1920, 1080, L"TGPOpenPic.bmp", x, y))
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"打开TGP助手!");
			ClickPoint(x, y);
			Sleep(5 * 1000);
			continue;
		}

		// 打开游戏助手
		if (dm.FindPic(0, 0, 1920, 1080, L"TGPMainPage.bmp", x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"打开游戏助手!x=%d,y=%d,l=%d,r=%d", x, y, LauncherRect.left, LauncherRect.top);
			ClickPoint(x, y);
			Sleep(7 * 1000);
			//continue;
		}

		// 查找更多设置
		static bool bMoreSetting = true;
		if (!bMoreSetting && dm.FindPic(0, 0, 1920, 1080, L"TGPMoreSetting.bmp", x, y))
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"查找更多设置");
			bMoreSetting = true;
			ClickPoint(x + 44, y + 12);
			Sleep(5 * 1000);

			if (dm.FindPic(0, 0, 1920, 1080, L"TGPPureModeSetting.bmp", x, y))
			{
				// 勾上纯净模式
				ClickPoint(x + 5, y + 5);
				Sleep(1000);

				// 提示点确定
				ClickPoint(x + 258, y - 152);
				Sleep(1000);

				CPrintLog::PrintLog_W(_SELF, __LINE__, L"关闭'更多设置'");
				ClickPoint(x + 350, y + 10);
				Sleep(1000);
			}
		}

		// 大区
		if (bMoreSetting && dm.FindPic(0, 0, 1920, 1080, L"TGPOtherServer.bmp", x, y))
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"选大区!");
			ClickPoint(x - 50, y);
			Sleep(5 * 1000);
			ServerRect SR;
			if (GetServerInfo_By_Name(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameServer, SR))
			{
				::GetWindowRect(hTGP, &LauncherRect);
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"SR.ServerType=%d,x=%d,y=%d", SR.ServerType, SR.x, SR.y);
				if (SR.ServerType == GameServerType_Telecom) // 电信
					ClickPoint(LauncherRect.left - 743 + 75, LauncherRect.top + 457);
				else if (SR.ServerType == GameServerType_Netcom)// 网通
					ClickPoint(LauncherRect.left - 743 + 180, LauncherRect.top + 457);
				else// 其他
					ClickPoint(LauncherRect.left - 743 + 270, LauncherRect.top + 457);

				Sleep(2 * 1000);
				ClickPoint(LauncherRect.left - 743 + SR.x, LauncherRect.top + SR.y);
				ClickPoint(LauncherRect.left - 743 + SR.x, LauncherRect.top + SR.y);
				Sleep(2 * 1000);
				continue;
			}
		}
	}

	CPrintLog::PrintLog_W(_SELF, __LINE__, L"TGP登录1分钟超时");
	CCharacter::wstrcpy_my(g_pSharedInfo->wszTGPMsg, L"错误:TGP登录1分钟超时");
	CreateEndMutex();

	return 0;
}