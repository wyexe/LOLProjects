#include <iostream>
#include "VerClientSocket.h"
#include "Hall.h"
#include "CreatePlayer.h"
#include "ConsoleConfig.h"

//#include "BindStatusCallback.h"
using namespace std;
#define _SELF L"main.cpp"

static DWORD GetAreaId_ByName(LPCWSTR pwszName)
{
	typedef struct _AreaSrc
	{
		WCHAR wszAreaName[32];
		DWORD dwAreaId;
	}AreaSrc;

	const int nMaxArrayLen = 27;
	static AreaSrc AreaSrcArray[nMaxArrayLen] = {
		L"艾欧尼亚", 1,
		L"比尔吉沃特", 2,
		L"祖安", 3,
		L"诺克萨斯", 4,
		L"德玛西亚", 5,
		L"班德尔城", 6,
		L"皮尔特沃夫", 7,
		L"战争学院", 8,
		L"弗雷尔卓德", 9,
		L"巨神峰", 10,
		L"雷瑟守备", 11,
		L"无畏先锋", 12,
		L"裁决之地", 13,
		L"黑色玫瑰", 14,
		L"暗影岛", 15,
		L"钢铁烈阳", 17,
		L"恕瑞玛", 16,
		L"均衡教派", 19,
		L"水晶之痕", 18,
		L"教育网专区", 21,
		L"影流", 22,
		L"守望之海", 23,
		L"扭曲丛林", 20,
		L"征服之海", 24,
		L"卡拉曼达", 25,
		L"皮城警备", 27,
		L"巨龙之巢", 26
	};

	for (int i = 0; i < nMaxArrayLen; ++i)
	{
		if (CCharacter::wstrcmp_my(pwszName, AreaSrcArray[i].wszAreaName))
			return AreaSrcArray[i].dwAreaId;
	}

	return 0;
}

static BOOL GetWebInfo()
{
	// 写入网页登录
	g_pSharedInfo->MyGameInfo[0].Lock();
	auto pWebLoginInfo = &g_pSharedInfo->MyGameInfo[0].webInfo;
	CCharacter::wstrcpy_my(pWebLoginInfo->wszGameUser, g_ClientGroup.SLD.wszGameUser);
	CCharacter::wstrcpy_my(pWebLoginInfo->wszGamePass, g_ClientGroup.SLD.wszPass);
	CCharacter::wstrcpy_my(pWebLoginInfo->wszdamaUser, wszdamaUser);
	CCharacter::wstrcpy_my(pWebLoginInfo->wszdamaPass, wszdamaPass);
	pWebLoginInfo->dwAreId = GetAreaId_ByName(g_ClientGroup.wszServerName);
	g_pSharedInfo->MyGameInfo[0].UnLock();
	if (pWebLoginInfo->dwAreId == 0)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"获取服务器失败, 不存在服务器:%s", g_ClientGroup.wszServerName);
		return FALSE;
	}
	

	wcout << L"等待网页获取帐号昵称…………" << endl;
	if (!CLProcess::Is_Exist_Process_For_ProcName(L"WebLogin.exe"))
	{
		WinExec("WebLogin.exe", SW_SHOW);
		Sleep(1000);
	}

	if (g_pSharedInfo->MyGameInfo[0].webPerson.dwLevel == -1)
		g_pSharedInfo->MyGameInfo[0].webPerson.dwLevel = 0;

	while (true)
	{
		g_pSharedInfo->MyGameInfo[0].Lock();
		if (g_pSharedInfo->MyGameInfo[0].webPerson.dwLevel == (DWORD)-1)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"查询失败……X掉");
			CLProcess::TerminateProc_For_ProcName(L"WebLogin.exe");
			return FALSE;
		}
		if (g_pSharedInfo->MyGameInfo[0].webPerson.dwLevel > 0)
		{
			auto pWebPerson = &g_pSharedInfo->MyGameInfo[0].webPerson;
			CCharacter::wstrcpy_my(g_ClientGroup.SLD.wszNiceName, pWebPerson->wszNiceName);
			g_ClientGroup.SLD.nGold = pWebPerson->dwGold;
			g_ClientGroup.SLD.nLevel = pWebPerson->dwLevel;
			g_ClientGroup.SLD.nSumWarCount = pWebPerson->dwSumWarCount;
			break;
		}
		g_pSharedInfo->MyGameInfo[0].UnLock();
		Sleep(1000);
	}
	

	return TRUE;
}

CHAR szServerIp[32] = { 0 };
BOOL ReadConfig()
{
	WCHAR wszIniPath[MAX_PATH];

	::GetCurrentDirectoryW(MAX_PATH, wszIniPath);
	lstrcatW(wszIniPath, L"\\Config.ini");

	if (!CConsoleConfig::GetTGPPath_By_Reg(g_pSharedInfo->wszTGPPath))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"读取TGP路径失败!");
		return FALSE;
	}

	WCHAR wszTmpPath[MAX_PATH];
	if (!CLPublic::FileExit(L"C:\\admin.CL"))
	{
		// 判断是否存在d3d8thk.dll和d3d8th.dll
		auto fn_DeleteFile = [&wszTmpPath](LPCWSTR pwszDllName)
		{
			CCharacter::wstrcpy_my(wszTmpPath, wszGamePath);
			lstrcatW(wszTmpPath, L"\\Game\\");
			lstrcatW(wszTmpPath, pwszDllName);
			if (CLPublic::FileExit(wszTmpPath))
				::DeleteFileW(wszTmpPath);
		};
		
		//fn_DeleteFile(L"d3d8th.dll");
		//fn_DeleteFile(L"d3d8thk.dll");
	}

	CCharacter::wstrcpy_my(wszTmpPath, g_pSharedInfo->wszTGPPath);
	lstrcatW(wszTmpPath, L"\\tgp_daemon.exe");
	if (!CLPublic::FileExit(wszTmpPath))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"TGP路径错误=%s", wszTmpPath);
		return FALSE;
	}

	ZeroMemory(&g_ClientGroup, sizeof(g_ClientGroup));

	if (!GetPrivateProfileStringW(L"Login", L"damaUser", L"Error", wszdamaUser, MAX_PATH, wszIniPath) || CCharacter::wstrcmp_my(wszdamaUser, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"读取打码兔帐号失败!");
		return FALSE;
	}

	if (!GetPrivateProfileStringW(L"Login", L"damaPass", L"Error", wszdamaPass, MAX_PATH, wszIniPath) || CCharacter::wstrcmp_my(wszdamaPass, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"读取打码兔密码失败!");
		return FALSE;
	}

	if (!GetPrivateProfileStringW(L"Login", L"ID", L"Error", g_ClientGroup.wszComputerName, 32, wszIniPath) || CCharacter::wstrcmp_my(g_ClientGroup.wszComputerName, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"读取编号失败!");
		return FALSE;
	}

	WCHAR wszServerIp[32] = { 0 };
	if (!GetPrivateProfileStringW(L"Login", L"ServerIp", L"Error", wszServerIp, 32, wszIniPath) || CCharacter::wstrcmp_my(wszServerIp, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"读取服务器Ip失败!");
		return FALSE;
	}
	CCharacter::UnicodeToASCII(wszServerIp, szServerIp);

	return TRUE;
}

static bool CreateMapView()
{
	//检查文件映射
	HANDLE hFileMap = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, SZFILE_NAME_SHAREDINFO);
	if (hFileMap == NULL)
	{
		hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024 * 1024, SZFILE_NAME_SHAREDINFO);
		if (hFileMap == NULL)
		{
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"打开文件映射失败");
			return false;
		}
	}

	BYTE * pCheckBuf = (BYTE *)MapViewOfFile(hFileMap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
	if (pCheckBuf == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"映射文件失败");
		return false;
	}

	wchar_t szFilePath[MAX_PATH] = { 0 };
	wsprintf(szFilePath, L"%s.SharedMem", SZFILE_NAME_SHAREDINFO);
	HANDLE hFileSharedInfo = CreateFile(szFilePath, GENERIC_READ | GENERIC_WRITE,          // open for reading
		FILE_SHARE_READ | FILE_SHARE_WRITE,       // share for reading
		NULL,                  // default security
		CREATE_ALWAYS,         // existing file only
		FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, // normal file
		NULL);
	if (hFileSharedInfo == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"映射文件不存在");
		return FALSE;
	}

	hFileMap = CreateFileMapping(hFileSharedInfo, NULL, PAGE_READWRITE, 0, sizeof(SHARED_INFO), SZFILE_NAME_SHAREDINFO);
	if (hFileMap == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"重新映射失败!");
		return FALSE;
	}

	//关闭文件句柄
	::CloseHandle(hFileSharedInfo);

	g_pSharedInfo = (PSHARED_INFO)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, sizeof(SHARED_INFO));
	if (!g_pSharedInfo)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"不知道说什么好了……");
		return false;
	}

	//初始化共享内存空间
	memset(g_pSharedInfo, 0, sizeof(SHARED_INFO));
	g_pSharedInfo->nRunTGPCount = 0;
	g_pSharedInfo->nAccountCount = 1;

	//设置控制台的路径给游戏DLL
	/*CCharacter::wstrcpy_my(g_pSharedInfo->wszTGPPath, L"F:\\Program Files\\Tencent\\TGP");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameUser, L"1743051060");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszGamePass, L"liu1155");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszdamaUser, L"chchch25");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszdamaPass, L"123456");
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameServer, L"裁决之地");
	g_pSharedInfo->MyGameInfo[0].webInfo.dwAreId = GetAreaId_ByName(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameServer);*/

	return true;
}

DWORD WINAPI WorkThread(LPVOID lpParm)
{
	static st_service_pump service_pump;
	static CVerClient client(service_pump);
	static CVerClientSocket ClientSocket;
	static CCreatePlayer CCP;
	static CHall Hall(&ClientSocket);

	//////////启动socket////////////////////////////////////////////////////////////////
	cout << "ServerIP:" << szServerIp << endl;
	ClientSocket.pVerClient = &client;
	ClientSocket.pservice_pump = &service_pump;
	client.set_server_addr(12365, szServerIp);
	service_pump.start_service(1);
	if (!service_pump.is_running())
	{
		service_pump.stop_service();
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"无法连接服务器!");
		goto Function_Exit;
	}
	ClientSocket.Start();
	Sleep(5 * 1000);

	while (!client.is_connected())
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"等待服务器响应……");
		Sleep(10 * 1000);
	}

	////////从服务端获取帐号//////////////////////////////////////////////////////////////////
	ClientSocket._Send_GetAccount();

lb_Loop:;
	//////////启动TGP和LOL大厅////////////////////////////////////////////////////////////////
	hWndLuncher = ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net 客户端");
	if (hWndLuncher == NULL)
	{
		Hall.RunLolClient();
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"好了, 已经启动LOL!");
	}

	////////////创建角色//////////////////////////////////////////////////////////////
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"判断是否需要创建角色");
	if (!GetWebInfo())
	{
		// 开始创建角色
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"需要创建角色");
		CCP.CreatePlayer();

		if (!GetWebInfo())
		{
			static int nCount = 0;
			if (++nCount >= 3)
			{
				CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"反复创建角色3次都失败!");
			}
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"获取角色名字失败!");
			goto lb_Loop;
		}
	}

	/////////排队/////////////////////////////////////////////////////////////////
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"判断排队!");
	while (Hall.IsQueuing())
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"排队中……");
		Sleep(5 * 1000);
		continue;
	}

	// 是否被封号
	if (Hall.IsSealUser())
	{
		// 发送封号消息 
		ClientSocket._Send_IdSealed();

		// 关闭socket
		ClientSocket.pVerClient->disconnect(false);
		ClientSocket.pVerClient->force_close(false);

		// 关闭WebLogin.exe
		CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);

		Sleep(5000);
		exit(0);
		goto lb_Loop;
	}

	////////战斗循环//////////////////////////////////////////////////////////////////
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"开始循环!");
	while (TRUE)
	{
		// 掉线重连
		if (Hall.IsLoseCon())
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"掉线重连…就不要经过组队了!…");
			ClientSocket._Send_ClientMsg(L"掉线重连进入游戏!");
			goto lb_CreateTeam;
		}

		
		/*if (g_ClientGroup.bMatching)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"单排情况下不组队……");
			goto lb_CreateTeam;
		}*/

		// 等待服务器组队
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"等待服务器组队中……");
		//ClientSocket._Send_ClientMsg(L"开始等待服务器组队!");
		//if (!ClientSocket._Send_JoinGroup())
		//{
		//	g_bCloseGame = TRUE;
		//	goto lb_LeaveGroup;
		//}

		g_ClientGroup.bMatching = TRUE;
		g_ClientGroup.bMatchMachine = TRUE;
		g_ClientGroup.Difficulty = GameDifficulty_Normal;
		if (g_ClientGroup.SLD.nSumWarCount >= 30)
		{
			if (rand() % 5 == 2)
				CCharacter::wstrcpy_my(g_ClientGroup.wszMapName, L"召唤师峡谷");
			else
				CCharacter::wstrcpy_my(g_ClientGroup.wszMapName, L"水晶之痕");

		}
		else
		{
			CCharacter::wstrcpy_my(g_ClientGroup.wszMapName, L"召唤师峡谷");
		}
		CConsoleConfig::SetGameMap(g_ClientGroup.wszMapName);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"昵称:%s,地图:%s,帐号:%s,nSumWarCount=%d", g_ClientGroup.SLD.wszNiceName, g_ClientGroup.wszMapName, g_ClientGroup.SLD.wszGameUser, g_ClientGroup.SLD.nSumWarCount);

	lb_CreateTeam:;
		HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOL_Game_Load_Wait");
		if (hMutex != NULL)
		{
			::ReleaseMutex(hMutex);
			::CloseHandle(hMutex);
			hMutex = NULL;
		}

		Hall.Init();
		if (g_bCaptain || g_ClientGroup.bMatching)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"我是队长,准备做队长的操作!");
			Hall.CaptainHanle();
		}
		else
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"我只是队员罢了, 坐等邀请");
			Hall.MemberHandle();
		}

//	lb_LeaveGroup:;
		// 战斗结束后离队!
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"战斗结束后离队");
		//ClientSocket._Send_LeaveGroup();

		// 是否封号
		/*if (g_bGameTitle)
		{
			// 发送封号消息 
			ClientSocket._Send_IdSealed();

			// 关闭socket
			ClientSocket.pVerClient->disconnect(false);
			ClientSocket.pVerClient->force_close(false);

			// 关闭WebLogin.exe
			CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);

			Sleep(5000);
			exit(0);
		}*/

		// 重新获取帐号
		//ClientSocket._Send_GetAccount();
		if (g_bSwitchAccount && FALSE)
		{
			// 需要更换帐号, 有可能是等级刷够了, 有可能是号被冻结了, 有可能是此号在服务器已经不存在了
			// 关闭TGP
			CLProcess::TerminateProc_For_ProcName(L"tgp_render.exe");
			CLProcess::TerminateProc_For_ProcName(L"tgp_daemon.exe");
			CLProcess::TerminateProc_For_ProcName(L"tcls_core.exe");
			CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
			CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
			CLProcess::TerminateProc_For_ProcName(L"WebLogin.exe");
			CLProcess::TerminateProc_For_ProcName(L"TGPLogin.exe");
			Sleep(3 * 1000);
			goto lb_Loop;
		}

		if (g_bCloseGame)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"需要X掉游戏,X掉控制台~!");
			g_bCloseGame = FALSE;
			CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
			CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
		}

		
		Hall.RunLolClient();
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"等待0分钟后, 服务器重新随机组队!");
		//Sleep(2 * 60 * 1000);
	}

	CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"游戏被封号了……杯具了~!");

Function_Exit:;
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"线程操作已经停止了~!");
	hWorkThread = NULL;
	return 0;
}

DWORD WINAPI RemoteInjectorThread(LPVOID lpParm)
{
	BOOL bExistProc = FALSE;
	while (true)
	{
		Sleep(1000);
		if (!bExistProc)
		{
			// 等待进程出现, 然后注入
			if (CLProcess::Is_Exist_Process_For_ProcName(LOL_GAMEPROC_NAME))
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"准备注入DLL");
				Sleep(20 * 1000);

				// 窗口前置
				auto fn_SwitchWindow = [](HWND hWnd)
				{
					if (hWnd != NULL)
					{
						ShowWindow(hWnd, SW_RESTORE);
						::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
						SwitchToThisWindow(hWnd, TRUE);
					}
				};
				fn_SwitchWindow(::FindWindowW(L"RiotWindowClass", NULL));

				static WCHAR wszDLLPath[MAX_PATH];
				::GetCurrentDirectoryW(MAX_PATH, wszDLLPath);
				lstrcatW(wszDLLPath, L"\\d3d8th.dll");
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"开始注入!");
				if (CLProcess::LoadRemoteDLL(LOL_GAMEPROC_NAME, wszDLLPath))
					bExistProc = TRUE;
				else
					CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"注入失败!");
			}
		}
		else
		{
			// 判断进程是否消失掉了
			if (!CLProcess::Is_Exist_Process_For_ProcName(LOL_GAMEPROC_NAME))
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"进程消失了……接着开始搜索!");
				bExistProc = FALSE;
			}
		}

	}
	return 0;
}

static BOOL RunDebugLog()
{
	CHAR wszPath[MAX_PATH];
	::GetCurrentDirectoryA(MAX_PATH, wszPath);
	lstrcatA(wszPath, "\\Log\\DebugLog.exe");

	if (!CLProcess::Is_Exist_Process_For_ProcName(L"DebugLog.exe"))
	{
		cout << "启动DebugLog.exe" << endl;
		WinExec(wszPath, SW_SHOW);
		Sleep(5 * 1000);
	}
	return TRUE;
}

static VOID ClearFile()
{
	auto fnDeleteFile = [](LPCWSTR pwszFileName)
	{
		static WCHAR wszCurrentPath[MAX_PATH];

		::GetCurrentDirectoryW(MAX_PATH, wszCurrentPath);
		lstrcatW(wszCurrentPath, L"\\");
		lstrcatW(wszCurrentPath, pwszFileName);

		if (CLPublic::FileExit(wszCurrentPath))
			::DeleteFileW(wszCurrentPath);
	};

	fnDeleteFile(L"CrackCaptcha.log");
	fnDeleteFile(L"LOLServer.dll");
	fnDeleteFile(L"Login.ini");
	fnDeleteFile(L"更新日志.rtf");
}

int main(int argc, CHAR* argv[])
{
	RunDebugLog();
	HANDLE hConsoleMutex = ::CreateMutexW(NULL, FALSE, L"Lol_More_Console_Mutex");
	if (hConsoleMutex != NULL && ::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"只能开1个LolConsole.exe进程!");
		return 0;
	}

	if (argc <= 1)
		return 0;

	if (CCharacter::strcmp_my(argv[1], "-test"))
	{
		CBmpFind dm;
		int x, y;
		WCHAR wszPath[MAX_PATH];
		::GetCurrentDirectoryW(MAX_PATH, wszPath);
		lstrcatW(wszPath, L"\\Bmp");
		dm.SetPath(wszPath);
		dm.BindWindow(::GetDesktopWindow());
		if (dm.FindPic(0, 0, 1920, 1080, L"test.bmp", x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"1111111111");
		}
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"ret");
		return 0;
	}
	else if (CCharacter::strcmp_my(argv[1], "-map"))
	{
		if (CCharacter::strcmp_my(argv[2], "-召唤师峡谷"))
		{
			CConsoleConfig::SetGameMap(L"召唤师峡谷");
		}
		else if (CCharacter::strcmp_my(argv[2], "-水晶之痕"))
		{
			CConsoleConfig::SetGameMap(L"水晶之痕");
		}
		else if (CCharacter::strcmp_my(argv[2], "-嚎哭深渊"))
		{
			CConsoleConfig::SetGameMap(L"嚎哭深渊");
		}
		Sleep(-1);
	}
	else if (CCharacter::strcmp_my(argv[1], "-start"))
	{
		if (!CreateMapView())
			return 0;

		// 创建互斥
		::CreateMutexW(NULL, FALSE, L"CL_LOL_Console_Mutex");

		// 设置UniCode输出
		SetConsole_Language_CHINA;

		while (true)
		{
			cout << "Give Me Order!" << endl;
			char szText[32] = { 0 };
			//cin >> szText;

			if (CCharacter::strcmp_my(szText, "-start") || TRUE)
			{
				if (hWorkThread != NULL)
				{
					cout << "已经在运行中了……少年, 别重复运行,知道吗?" << endl;
					continue;
				}
				if (!CLProcess::Is_Exist_Process_For_ProcName(L"QQ.exe"))
				{
					cout << "必须运行QQ才可以!" << endl;
					//continue;
				}

				// 关闭游戏!
				CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
				CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
				CLProcess::TerminateProc_For_ProcName(L"tgp_daemon.exe");
				if (!ReadConfig())
				{
					cout << "读取配置文件失败" << endl;
					Sleep(-1);
				}

				// 修改hosts
				if (!CConsoleConfig::UpdateHosts())
				{
					cout << "修改hosts文件失败, 是不是没管理员权限?" << endl;
					Sleep(-1);
				}

				// 清理上古遗留下来的文件
				ClearFile();

				cout << "已经启动匹配线程! 剩下的信息将全部可以从日志中观察~!" << endl;
				g_dwCmd = GameCmd_Start;
				hWorkThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkThread, NULL, NULL, NULL);
				///::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RemoteInjectorThread, NULL, NULL, NULL);
			}
			else if (CCharacter::strcmp_my(szText, "-stop"))
			{
				StopGame;
				if (WaitForSingleObject(hWorkThread, 3000) == WAIT_TIMEOUT)
				{
					::TerminateThread(hWorkThread, 0);
					::CloseHandle(hWorkThread);
				}
				hWorkThread = NULL;
				cout << "成功结束掉线程了!" << endl;
			}
			Sleep(-1);
		}
	}
	

	
	
	return 0;
}
