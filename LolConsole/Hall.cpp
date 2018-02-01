#include "Hall.h"
#include <tchar.h>
//#pragma comment(lib,"CrackCaptchaAPI.lib")

#define _SELF L"Hall.cpp"

#define SendClientMsg(x) if (m_pCVerClientSocket != NULL)		\
						 {										\
							m_pCVerClientSocket->_Send_ClientMsg(x);	\
						 }

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
			if (!CCharacter::wstrcmp_my(wszText, L"RiotWindowClass"))
			{
				::ShowWindow(h, SW_SHOWMINIMIZED);
			}
		}
	}


	return TRUE;
}

CHall::CHall(CVerClientSocket* pCVerClientSocket)
{
	bGameStart = FALSE;
	dwStartGameTick = 0;
	m_pCVerClientSocket = pCVerClientSocket;
}

CHall::~CHall()
{
}

BOOL CHall::Init()
{
	hWndLuncher = ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net 客户端");
	if (hWndLuncher == NULL)
	{
		//CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"游戏还没启动啊,兄台!");
		return FALSE;
	}

	ShowWindow(hWndLuncher, SW_RESTORE);
	::SetWindowPos(hWndLuncher, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SwitchToThisWindow(hWndLuncher, TRUE);

	RECT LauncherRect;
	CSearchPic::GetGameRect(LauncherRect);
	ClickPoint(LauncherRect.left + 100, LauncherRect.top + 10);
	return TRUE;
}

BOOL CHall::ClickPoint(int x, int y)
{
	for (int i = 0; i < 1;++i)
	{
		// 设置鼠标
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, NULL, NULL);
		mouse_event(MOUSEEVENTF_LEFTUP, x, y, NULL, NULL);
	}
	//CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"ClickPoint");
	return TRUE;
}

BOOL CHall::ClearTaskMenu()
{
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
	rc.bottom += 15;
	POINT pt;
	::GetCursorPos(&pt);
	for (int i = rc.right - 500; i < rc.right; i += 10)
	{
		::SetCursorPos(i, rc.bottom);
		Sleep(20);
	}
	::SetCursorPos(pt.x, pt.y);
	return TRUE;
}

BOOL CHall::GetHeroBmpName(__in em_Hero_Pro emHero, __out LPWSTR szHeroBmpName)
{
	switch (emHero)
	{
	case em_Hero_Pro_Ryze:
		CCharacter::wstrcpy_my(szHeroBmpName, L"fashi.bmp");
		break;
	case em_Hero_Pro_Garen:
		CCharacter::wstrcpy_my(szHeroBmpName, L"dema.bmp");
		break;
	case em_Hero_Pro_Ashe:
		CCharacter::wstrcpy_my(szHeroBmpName, L"hanbing.bmp");
		break;
	case em_Hero_Pro_MissFortune:
		CCharacter::wstrcpy_my(szHeroBmpName, L"shangjin.bmp");
		break;
	case em_Hero_Pro_MasterYi:
		CCharacter::wstrcpy_my(szHeroBmpName, L"jiansheng.bmp");
		break;
	case em_Hero_Pro_Malzahar:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Malzahar.bmp");
		break;
	case em_Hero_Pro_Ezreal:
		CCharacter::wstrcpy_my(szHeroBmpName, L"tanxianjia.bmp");
		break;
	case em_Hero_Pro_Chogath:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Chogath.bmp");
		break;
	case em_Hero_Pro_Nunu:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Nunu.bmp");
		break;
	case em_Hero_Pro_Heimerdinger:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Heimerdinger.bmp");
		break;
	case em_Hero_Pro_Graves:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Graves.bmp");
		break;
	case em_Hero_Pro_Swain:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Swain.bmp");
		break;
	case em_Hero_Pro_Vayne:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Vayne.bmp");
		break;
	case em_Hero_Pro_Maokai:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Maokai.bmp");
		break;
	case em_Hero_Pro_Veigar:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Veigar.bmp");
		break;
	case em_Hero_Pro_Sejuani:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Sejuani.bmp");
		break;
	case em_Hero_Pro_Tristana:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Tristana.bmp");
		break;
	case em_Hero_Pro_Galio:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Galio.bmp");
		break;
	case em_Hero_Pro_Lucian:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Lucian.bmp");
		break;
	case em_Hero_Pro_KogMaw:
		CCharacter::wstrcpy_my(szHeroBmpName, L"KogMaw.bmp");
		break;
	case em_Hero_Pro_Corki:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Corki.bmp");
		break;
	case em_Hero_Pro_Caitlyn:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Caitlyn.bmp");
		break;
	case em_Hero_Pro_Ahri:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Ahri.bmp");
		break;
	case em_Hero_Pro_Annie:
		CCharacter::wstrcpy_my(szHeroBmpName, L"Annie.bmp");
		break;
	default:
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"无法找到这个英雄,你确定你没填错? emHeroPro=%X", emHero);
		return FALSE;
	}
	return TRUE;
}

BOOL CHall::PraiseTeamMember(RECT& LauncherRect)
{
	Sleep(2000);
	// 先加好友
	for (int i = 0; i < 4; ++i)
	{
		ClickPoint(LauncherRect.left + 1176, LauncherRect.top + 100 + i * 25);
		Sleep(500);
	}
	// 互赞
	for (int i = 0; i < 4; ++i)
	{
		//ClickPoint(LauncherRect.left + 1197, LauncherRect.top + 100 + i * 25);
		//Sleep(2000);
		//ClickPoint(LauncherRect.left + 1045, LauncherRect.top + 150 + i * 60);
		//Sleep(2000);
	}
	return TRUE;
}

BOOL CHall::HandleCloseGameMutex(CBmpFind& dm, RECT& LauncherRect)
{
	HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, Mutex_CloseGame_Name);
	if (hMutex == NULL)
		return FALSE;


	CConsoleConfig::WriteFightCount(++g_ClientGroup.SLD.nSumWarCount);
	SendClientMsg(L"游戏结束了!");
	// 写入战斗场数

	Sleep(30 * 1000);
	HandleTV();
	ClearTaskMenu();
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"关闭游戏进程");
	if (CLProcess::Is_Exist_Process_For_ProcName(LOL_GAMEPROC_NAME))
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"游戏进程还存在!");
		Init();
		Sleep(5000);
		CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
		Sleep(2000);
		Init();
	}
	/*if (CLProcess::Is_Exist_Process_For_ProcName(LOL_GAMEPROC_NAME))
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"握草, 结束游戏进程失败, 直接X控制台了!");
		goto lb_Exit;
	}*/
	Sleep(5 * 1000);
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"正常显示!");
	Init();
	for (int i = 0; i < 10; ++i)
	{
		Sleep(2000);
		int x = 0, y = 0;
		if (dm.FindPic(786, 681, 1030, 770, _T("21.bmp"), 0, 0, 0.9, x, y))//回到大厅
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"游戏分辨率木有变化!");
			::ReleaseMutex(hMutex);
			::CloseHandle(hMutex);
			hMutex = NULL;
			bGameStart = FALSE;
			::GetWindowRect(hWndLuncher, &LauncherRect);
			PraiseTeamMember(LauncherRect);
			return TRUE;
		}
	}
	
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"游戏分辨率变了,关闭客户端进程!");
//lb_Exit:;
	Init();
	if (CLProcess::Is_Exist_Process_For_ProcName(LOL_CLIENTPROC_NAME))
	{
		CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
		CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
	}
	::ReleaseMutex(hMutex);
	::CloseHandle(hMutex);
	hMutex = NULL;
	bGameStart = FALSE;
	return TRUE;
}

BOOL CHall::HandleGameDisappear()
{
	if (dwStartGameTick != NULL && ::GetTickCount() - dwStartGameTick >= 15 * 1000)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"发现游戏进程已经创建了!等待15s");
		Sleep(15 * 1000);
		dwStartGameTick = NULL;

		HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOL_Game_Load_Done");
		if (hMutex == NULL)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"游戏应该是崩溃了!");
			return FALSE;
		}

		// 除了游戏以外, 其他所有窗口都最小化
		EnumWindows(EnumWindowsProc, NULL);
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"游戏应该是正常状态!");
		::CloseHandle(hMutex);
		SendClientMsg(L"游戏正常运行");
	}
	return TRUE;
}

BOOL CHall::HandleBugReport()
{
	if (CLProcess::Is_Exist_Process_For_ProcName(L"BsSndRpt.exe"))
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"异常:出现错误报告, 直接怒X游戏和大厅,重新开始!");
		CLProcess::TerminateProc_For_ProcName(L"BsSndRpt.exe");
		Sleep(3000);
		CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
		Sleep(3000);
		CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
		Sleep(3000);
	}
	return TRUE;
}

BOOL CHall::HandleTV()
{
	const int nMaxArrayLen = 7;
	static WCHAR wszProcArray[nMaxArrayLen][32] = {
		L"iexplore.exe", L"broadcasting.exe", L"TQMCenter.exe", L"QQMicroGameBoxTray.exe", L"CrossProxy.exe", L"TPHelper.exe", L"Tencentdl.exe" };

	for (int i = 0; i < nMaxArrayLen; ++i)
	{
		if (CLProcess::Is_Exist_Process_For_ProcName(wszProcArray[i]))
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"TerminateProc:%s", wszProcArray[i]);
			CLProcess::TerminateProc_For_ProcName(wszProcArray[i]);
		}
	}
	
	static WCHAR szClassName[MAX_PATH];
	HWND hParentWnd = ::GetDesktopWindow();
	HWND hWndPrevious = ::GetWindow(hParentWnd, GW_CHILD);
	while (::IsWindow(hWndPrevious))
	{
		::GetClassNameW(hWndPrevious, szClassName, MAX_PATH);
		if (CCharacter::wstrcmp_my(szClassName, L"TXGuiFoundation"))
		{
			static WCHAR wszCaption[MAX_PATH];
			::GetWindowTextW(hWndPrevious, wszCaption, MAX_PATH);
			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"wszCaption=%s", wszCaption);
			if (!CCharacter::wstrcmp_my(wszCaption, L"QQ"))
				::PostMessage(hWndPrevious, WM_CLOSE, NULL, NULL);
		}
		if (CCharacter::wstrcmp_my(szClassName,L"TWINCONTROL"))
		{
			static WCHAR wszCaption[MAX_PATH];
			::GetWindowTextW(hWndPrevious, wszCaption, MAX_PATH);
			if (!CCharacter::wstrcmp_my(wszCaption,L"腾讯游戏平台"))
				::PostMessage(hWndPrevious, WM_CLOSE, NULL, NULL);
		}
		hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT);
	}

	return TRUE;
}

BOOL CHall::HandleQT()
{
	return TRUE;
}

BOOL CHall::IsQueuing()
{
	CBmpFind dm;
	WCHAR wszPath[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Bmp");
	dm.SetPath(wszPath);
	int x = 0, y = 0;

	Init();
	Sleep(2000);
	dm.BindWindow(CSearchPic::GetGameHwnd());
	
	
	HandleTV();
	if (dm.FindPic(0, 0, 1280, 800, _T("Queuing.bmp"), 0, 0, 0.9, x, y) > 0)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"排队中……");
		Sleep(10 * 1000);
		return TRUE;
	}
	return FALSE;
}

BOOL CHall::CaptainHanle()
{
	static BmpPoint BP;
	static RECT LauncherRect;
	CBmpFind dm;

	// init
	DWORD dwRunTick = 0;
	WCHAR wszPath[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Bmp");
	dm.SetPath(wszPath);
	int x = 0, y = 0;
	BOOL bExistProc = FALSE;
	CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
	int nInviateCount = 0;
	dwStartGameTick = 0;

	while (!HandleCloseGameMutex(dm, LauncherRect))
	{
		Sleep(1000);

		HANDLE hGameLoseConMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, Mutex_GameLoseCon_Name);
		if (hGameLoseConMutex != NULL)
		{
			::ReleaseMutex(hGameLoseConMutex);
			::CloseHandle(hGameLoseConMutex);
			hGameLoseConMutex = NULL;
			goto lb_Terminate;
		}

		if (!HandleGameDisappear())
			goto lb_Terminate;

		CSearchPic::GetGameRect(LauncherRect);
		if (CLProcess::Is_Exist_Process_For_ProcName(LOL_GAMEPROC_NAME))
		{
			if (dwRunTick == 0)
				dwRunTick = ::GetTickCount();

			if (::GetTickCount() - dwRunTick >= 60 * 60 * 1000)
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"一把游戏打了60min, 累不累啊?");
				bExistProc = TRUE;
				dwRunTick = ::GetTickCount();
				goto lb_Terminate;
			}

			bExistProc = TRUE;
			Sleep(4 * 1000);
			HandleBugReport();
			if (WaitToGameStart() == em_GameStatus_Faild)
				goto lb_Terminate;
			continue;
		}

		if (bExistProc)
		{
		lb_Terminate:;
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"游戏进程消失了……掉线了!X掉游戏,X掉大厅");
			CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
			Sleep(1 * 1000);
			CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
			Sleep(1 * 1000);
			RunLolClient();
			bExistProc = FALSE;
			dwStartGameTick = NULL;
		}

		dm.BindWindow(CSearchPic::GetGameHwnd());
		Init();

		// 烦人的直播框
		HandleTV();

		// 是否掉线重连
		if (dm.FindPic(440, 279, 845, 500, _T("22.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"掉线重连!");
			ClickPoint(LauncherRect.left + x + 50, LauncherRect.top + y + 10);
			ClickPoint(LauncherRect.left + x + 50, LauncherRect.top + y + 10);
			Sleep(5 * 1000);
			continue;
		}

		if (dm.FindPic(0, 0, 1280, 800, _T("MsgClose1.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"MsgClose1.bmp");
			ClickPoint(LauncherRect.left + x + 5, LauncherRect.top + y + 5);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("MsgClose2.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"MsgClose2.bmp");
			ClickPoint(LauncherRect.left + x + 5, LauncherRect.top + y + 5);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("NewTalent_No.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"NewTalent_No.bmp");
			ClickPoint(LauncherRect.left + x + 120, LauncherRect.top + y + 17);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("MsgClose3.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"荣誉花光了,点确定");
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("CloseMsg_TakeGiftNow.bmp"), x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"新手礼包挡住了!");
			ClickPoint(LauncherRect.left + x + 5, LauncherRect.top + y + 5);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("CloseMsg_Record.bmp"), x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"游戏录制资格挡住了!");
			ClickPoint(LauncherRect.left + x + 5, LauncherRect.top + y + 5);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("HideMsg1.bmp"), x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"不要挡路!HideMsg1");
			ClickPoint(LauncherRect.left + x + 20, LauncherRect.top + y + 20);
			continue;
		}

		if (dm.FindPic(0, 0, 1280, 800, _T("WaitMatchTeam.bmp"), x, y))//1005  430
		{
			if (!dm.FindPic(0, 0, 1280, 800, _T("TeamRoomExit.bmp"), x, y))//1005  430
			{
				static DWORD dwTmpCount = 0;
				if (++dwTmpCount % 5 == 0)
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"正在匹配玩家……");

				continue;
			}
		}

		// 领取礼包
		/*if (dm.FindPic(0, 0, 1280, 800, _T("GoTakeGiftNow.bmp"), x, y) || dm.FindPic(0, 0, 1280, 800, _T("GoTakeGift.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"领取礼包1!");
			ClickPoint(LauncherRect.left + x + 50, LauncherRect.top + y + 18);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("TakeGiftNow.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"领取礼包3!");
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			continue;
		}*/

		if (dm.FindPic(0, 0, 1280, 800, _T("10.bmp"), 0, 0, 0.9, x, y) || dm.FindPic(0, 0, 1280, 800, _T("11.bmp"), 0, 0, 0.9, x, y) ||
			dm.FindPic(0, 0, 1280, 800, _T("19.bmp"), 0, 0, 0.9, x, y) || dm.FindPic(0, 0, 1280, 800, _T("1.bmp"), 0, 0, 0.9, x, y) ||
			dm.FindPic(0, 0, 1280, 800, _T("Sure.bmp"), 0, 0, 0.9, x, y) || dm.FindPic(0, 0, 1280, 800, _T("LoseCon_Sure.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"确定.bmp");
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			Sleep(2000);
			continue;
		}

		// 离开房间
		if (dm.FindPic(1000, 425, 1044, 530, _T("27.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"有人离开房间!那就解散好了!");
			g_bCloseGame = TRUE;
			ClickPoint(LauncherRect.left + 947 - 136, LauncherRect.top + 545);
			break;
		}

		// 判断是否在最初始的大厅中
		if (dm.FindPic(524, 11, 743, 82, L"4.bmp", 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"在最初始的大厅里, 点Play");
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
			continue;
		}

		if (dm.FindPic(0, 0, 1280, 800, L"HideAcceptInvaite.bmp", 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"因为是队长,所以需要隐藏接受邀请的栏");
			ClickPoint(LauncherRect.left + x + 15, LauncherRect.top + y + 15);
			Sleep(2000);
			continue;
		}

		if (dm.FindPic(96, 65, 211, 154, _T("8.bmp"), 0, 0, 0.9, x, y))//
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"邀请输入名字界面");
			for (UINT i = 0; i < TeamMemNameberLst.size(); ++i)
			{
				ClickPoint(LauncherRect.left + 860, LauncherRect.top + 198);
				for (int j = 0; j < 20; ++j)
					CLPublic::SendKey(8);

				CLPublic::SendKeys(TeamMemNameberLst.at(i).wszText);
				ClickPoint(LauncherRect.left + 1041, LauncherRect.top + 199);
				Sleep(1000);
			}

			ClickPoint(LauncherRect.left + 910, LauncherRect.top + 716);
			Sleep(8 * 1000);

			if (dm.FindPic(96, 65, 211, 154, _T("8.bmp"), 0, 0, 0.9, x, y))
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"握草,卡住在邀请这里了!点X");
				ClickPoint(LauncherRect.left + 1165, LauncherRect.top + 79);
				Sleep(2 * 1000);
				g_bCloseGame = TRUE;
			}

			if (nInviateCount++ >= 5)
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"5次都木有集合完毕……妈蛋离开房间,解散队伍, 等待服务器重新安排!");
				nInviateCount = 0;
				ClickPoint(LauncherRect.left + 947 - 136, LauncherRect.top + 545);
				Sleep(1000);
				g_bCloseGame = TRUE;
				break;
			}
			continue;
		}

		// 是否在组队界面
		if (dm.FindPic(878, 520, 1064, 591, _T("6.bmp"), 0, 0, 0.9, x, y))
		{
			int nTmpX = x;
			int nTmpY = y;
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"组队界面");
			if (dm.FindPic(1000, 425, 1044, 530, _T("28.bmp"), 0, 0, 0.95, x, y))//1005  430
			{
				ClickPoint(LauncherRect.left + nTmpX, LauncherRect.top + nTmpY);
			}
			else
			{
				ClickPoint(LauncherRect.left + nTmpX - 148, LauncherRect.top + nTmpY - 160);
			}

			Sleep(2000);
			continue;
		}

		// 是否在创建团队界面
		if (dm.FindPic(677, 617, 1071, 756, _T("5.bmp"), 0, 0, 0.95, x, y))//279  723
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"准备创建团队,地图是:%s", g_ClientGroup.wszMapName);
			if (g_MapType == MapType_召唤师峡谷)
			{
				ClickPoint(LauncherRect.left + x + 46, LauncherRect.top + y - 549);
				Sleep(500);
				ClickPoint(LauncherRect.left + x + 195 + 20, LauncherRect.top + y - 575);
				Sleep(500);
				ClickPoint(LauncherRect.left + x + 387 + 20, LauncherRect.top + y - 568);
				Sleep(1000);
				switch (g_ClientGroup.Difficulty)
				{
				case GameDifficulty_Entry:
					break;
				case GameDifficulty_Easy:
					ClickPoint(LauncherRect.left + x + 592 + 20, LauncherRect.top + y - 545); // 简单难度
					break;
				case GameDifficulty_Normal:
					ClickPoint(LauncherRect.left + x + 592 + 20, LauncherRect.top + y - 516); // 一般难度
					break;
				default:
					break;
				}

				Sleep(1000);
				ClickPoint(LauncherRect.left + x + 37 + 20, LauncherRect.top + y - 547);
				Sleep(1000);
			}
			else if (g_MapType == MapType_水晶之痕)
			{
				ClickPoint(LauncherRect.left + x + 46, LauncherRect.top + y - 600);
				Sleep(500);
				ClickPoint(LauncherRect.left + x + 200, LauncherRect.top + y - 540);
				Sleep(500);
				ClickPoint(LauncherRect.left + x + 400, LauncherRect.top + y - 570);
				Sleep(1000);
				ClickPoint(LauncherRect.left + x + 600, LauncherRect.top + y - 570);
				Sleep(1000);
				ClickPoint(LauncherRect.left + x + 46, LauncherRect.top + y - 600);
				Sleep(1000);
			}
			else if (g_MapType == MapType_嚎哭深渊)
			{
				ClickPoint(LauncherRect.left + x + 46, LauncherRect.top + y - 600);
				Sleep(500);
				ClickPoint(LauncherRect.left + x + 200, LauncherRect.top + y - 515);
				Sleep(500);
				ClickPoint(LauncherRect.left + x + 400, LauncherRect.top + y - 570);
				Sleep(1000);
				ClickPoint(LauncherRect.left + x + 600, LauncherRect.top + y - 570);
				Sleep(1000);
				ClickPoint(LauncherRect.left + x + 46, LauncherRect.top + y - 600);
				Sleep(1000);
			}
			
			if (g_ClientGroup.bMatching)
				ClickPoint(LauncherRect.left + x + 475, LauncherRect.top + y); // 单人
			else
				ClickPoint(LauncherRect.left + x + 643, LauncherRect.top + y); // 团队
			
			Sleep(5 * 1000);
			continue;
		}

		if (dm.FindPic(0, 0, 829, 470, _T("12.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"12.bmp");
			ClickPoint(LauncherRect.left + x + 861, LauncherRect.top + y + 385);
			Sleep(1000);
			ClickPoint(LauncherRect.left + x + 889, LauncherRect.top + y + 545);
			Sleep(1000);
			continue;
		}

		if (dm.FindPic(647, 495, 738, 524, _T("13.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"13.bmp");
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			Sleep(2000);
			continue;
		}

		if (dm.FindPic(0, 0, 1000, 600, _T("17.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"现在开始.bmp");
			ClickPoint(LauncherRect.left + x + 10, LauncherRect.top + y + 10);
			Sleep(2000);
			continue;
		}

		if (dm.FindPic(549, 500, 1000, 600, _T("20.bmp"), x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"被封号拉！！！");
			g_bGameTitle = TRUE;
			break;
		}

		if (dm.FindPic(786, 681, 1030, 770, _T("21.bmp"), 0, 0, 0.9, x, y))//回到大厅
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"回到大厅");
			ClickPoint(LauncherRect.left + 856 + 20, LauncherRect.top + 742);
			Sleep(2000);
			continue;
		}

		if (dm.FindPic(549, 500, 1000, 600, _T("Sure.bmp"), 0, 0, 0.9, x, y))//869 508 确定开局
		{
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"Sure.bmp");
			ClickPoint(LauncherRect.left + x + 60, LauncherRect.top + y + 20);
			continue;
		}

		if (dm.FindPic(0, 0, 1000, 600, _T("TakeGiftNow.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"TakeGiftNow.bmp");
			ClickPoint(LauncherRect.left + x + 50, LauncherRect.top + y + 10);
			Sleep(2000);
			continue;
		}

		if (dm.FindPic(0, 0, 1000, 600, _T("24.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"24.bmp");
			ClickPoint(LauncherRect.left + x + 781, LauncherRect.top + y - 121);
			Sleep(2000);
			continue;
		}

		if (g_MapType == MapType_嚎哭深渊)
		{
			if (dm.FindPic(0, 0, 1280, 800, L"WaitChooseHero.bmp", x, y))
				goto lb_WaitSartGame;
		}

		// 是否在选英雄界面
		if (dm.FindPic(0, 0, 1280, 800, _T("18.bmp"), 0, 0, 0.9, x, y))//92 35 确定开局
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"正在搜索英雄……");
			WCHAR szHeroBmpName[32] = { 0 };
			// 搜索8个英雄
			static em_Hero_Pro emHeroArray[] = {
				em_Hero_Pro_Ryze, em_Hero_Pro_Garen, em_Hero_Pro_Ashe, em_Hero_Pro_MissFortune, em_Hero_Pro_MasterYi,
				em_Hero_Pro_Ezreal, em_Hero_Pro_Nunu, em_Hero_Pro_Annie
			};

			//random_shuffle(emHeroArray, emHeroArray + 8);
			srand(::GetTickCount());
			BOOL bExist = FALSE;
			while (true)
			{
				int i = rand() % 8;
				if (!GetHeroBmpName(emHeroArray[i], szHeroBmpName))
				{
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"找不到该英雄，emHeroPro=%d, i = %d", emHeroPro, i);
					continue;
				}
				if (dm.FindPic(0, 0, 1280, 800, szHeroBmpName, 0, 0, 0.8, x, y))
				{
					bExist = TRUE;
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"选择英雄!%s", szHeroBmpName);
					break;
				}
				Sleep(100);
			}

			if (!bExist)
			{
				// 返回循环
				continue;
			}

			CPrintLog::PrintLog_W(_SELF, __LINE__, L"点击英雄图标:%s", szHeroBmpName);
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			Sleep(500);
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"点确定");
			ClickPoint(LauncherRect.left + 870, LauncherRect.top + 505);
			Sleep(1000);
		lb_WaitSartGame:;
			// 判断天赋技能1是否存在
			/*if (!dm.FindPic(531,471,700,600,L"Talent1.bmp",x,y))
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"选择第一个天赋技能");
				ClickPoint(LauncherRect.left + 597, LauncherRect.top + 525);
				Sleep(1000);
				ClickPoint(LauncherRect.left + 543, LauncherRect.top + 255);
				Sleep(1000);
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"选择第二个天赋技能");
				ClickPoint(LauncherRect.left + 649, LauncherRect.top + 529);
				Sleep(1000);
				ClickPoint(LauncherRect.left + 469, LauncherRect.top + 257);
				Sleep(1000);
			}
			// 判断天赋技能2是否存在
			if (!dm.FindPic(531, 471, 700, 600, L"Talent2.bmp", x, y))
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"选择第一个天赋技能");
				ClickPoint(LauncherRect.left + 597, LauncherRect.top + 525);
				Sleep(1000);
				ClickPoint(LauncherRect.left + 543, LauncherRect.top + 255);
				Sleep(1000);
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"选择第二个天赋技能");
				ClickPoint(LauncherRect.left + 649, LauncherRect.top + 529);
				Sleep(1000);
				ClickPoint(LauncherRect.left + 469, LauncherRect.top + 257);
				Sleep(1000);
			}*/

			Sleep(2000);
			if (!dm.FindPic(0, 0, 1280, 800, L"WaitChooseHero.bmp", x, y))
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"……貌似选择英雄失败!");
				continue;
			}

			SendClientMsg(L"大厅选择英雄")

			for (int i = 0; i < 90; ++i)
			{
				Sleep(1000);

				if (dm.FindPic(0, 0, 1280, 800, L"WaitChooseHero.bmp", x, y))
					continue;

				break;
			}
			Sleep(10 * 1000);
			dwStartGameTick = ::GetTickCount();
			continue;
		}
	}
	return TRUE;
}

BOOL CHall::MemberHandle()
{
	static BmpPoint BP;
	static RECT LauncherRect;
	CBmpFind dm;

	// init
	WCHAR wszPath[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Bmp");
	dm.SetPath(wszPath);
	int x = 0, y = 0;
	CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
	BOOL bExistProc = FALSE;
	DWORD dwTick = ::GetTickCount();
	dwStartGameTick = 0;

	while (!HandleCloseGameMutex(dm, LauncherRect))
	{
		Sleep(1000);

		HANDLE hGameLoseConMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, Mutex_GameLoseCon_Name);
		if (hGameLoseConMutex != NULL)
		{
			::ReleaseMutex(hGameLoseConMutex);
			::CloseHandle(hGameLoseConMutex);
			hGameLoseConMutex = NULL;
			goto lb_Terminate;
		}

		if (!HandleGameDisappear())
			goto lb_Terminate;

		CSearchPic::GetGameRect(LauncherRect);
		if (CLProcess::Is_Exist_Process_For_ProcName(LOL_GAMEPROC_NAME))
		{
			bExistProc = TRUE;
			Sleep(4 * 1000);
			HandleBugReport();
			if (WaitToGameStart() == em_GameStatus_Faild)
				goto lb_Terminate;
			continue;
		}

		if (bExistProc)
		{
		lb_Terminate:;
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"游戏进程消失了……掉线了!X掉游戏,X掉大厅");
			CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
			Sleep(1 * 1000);
			CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
			Sleep(1 * 1000);
			RunLolClient();
			bExistProc = FALSE;
			dwTick = ::GetTickCount();
			dwStartGameTick = NULL;
		}

		dm.BindWindow(CSearchPic::GetGameHwnd());
		Init();

		// 烦人的直播框
		HandleTV();

		// 是否掉线重连
		if (dm.FindPic(440, 279, 845, 500, _T("22.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"掉线重连!");
			ClickPoint(LauncherRect.left + x + 50, LauncherRect.top + y + 10);
			ClickPoint(LauncherRect.left + x + 50, LauncherRect.top + y + 10);
			continue;
		}

		if (dwStartGameTick == NULL && ::GetTickCount() - dwTick >= 4 * 60 * 1000)// 必须是在没开始游戏之前的状态!
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"4分钟内还没开始游戏! 肯定有问题,重新安排队伍!");
			g_bCloseGame = TRUE;
			break;
		}

		if (dm.FindPic(0, 0, 1280, 800, _T("MsgClose1.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"MsgClose1.bmp");
			ClickPoint(LauncherRect.left + x + 10, LauncherRect.top + y + 10);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("NewTalent_No.bmp"), x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"NewTalent_No.bmp");
			ClickPoint(LauncherRect.left + x + 110, LauncherRect.top + y + 15);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("MsgClose3.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"荣誉花光了,点确定");
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("CloseMsg_TakeGiftNow.bmp"), x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"新手礼包挡住了!");
			ClickPoint(LauncherRect.left + x + 5, LauncherRect.top + y + 5);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("CloseMsg_Record.bmp"), x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"游戏录制资格挡住了!");
			ClickPoint(LauncherRect.left + x + 5, LauncherRect.top + y + 5);
			continue;
		}
		if (dm.FindPic(0, 0, 1280, 800, _T("HideMsg1.bmp"), x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"不要挡路!HideMsg1");
			ClickPoint(LauncherRect.left + x + 20, LauncherRect.top + y + 20);
			continue;
		}

		if (dm.FindPic(0, 0, 1280, 800, _T("WaitMatchTeam.bmp"), x, y))//1005  430
		{
			if (!dm.FindPic(0, 0, 1280, 800, _T("TeamRoomExit.bmp"), x, y))//1005  430
			{
				static DWORD dwTmpCount = 0;
				if (++dwTmpCount % 5 == 0)
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"正在匹配玩家……");
				
				dwTick = ::GetTickCount();
				continue;
			}
		}

		// 领取礼包
		/*if (dm.FindPic(0, 0, 1280, 800, _T("GoTakeGiftNow.bmp"), x, y) || dm.FindPic(0, 0, 1280, 800, _T("GoTakeGift.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"领取礼包1!");
			ClickPoint(LauncherRect.left + x + 50, LauncherRect.top + y + 15);
			continue;
		}*/
		/*if (dm.FindPic(0, 0, 1280, 800, _T("TakeGiftNow.bmp"), 0, 0, 0.9, x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"领取礼包3!");
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			continue;
		}*/

		if (dm.FindPic(0, 0, 1280, 800, _T("10.bmp"), 0, 0, 0.9, x, y) || dm.FindPic(0, 0, 1280, 800, _T("11.bmp"), 0, 0, 0.9, x, y) ||
			dm.FindPic(0, 0, 1280, 800, _T("19.bmp"), 0, 0, 0.9, x, y) || dm.FindPic(0, 0, 1280, 800, _T("1.bmp"), 0, 0, 0.9, x, y) ||
			dm.FindPic(0, 0, 1280, 800, _T("Sure.bmp"), x, y) || dm.FindPic(0, 0, 1280, 800, _T("LoseCon_Sure.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"确定.bmp");
			ClickPoint(LauncherRect.left + x + 10, LauncherRect.top + y + 5);
			Sleep(2000);
			continue;
		}

		if (dm.FindPic(0, 0, 1280, 800, _T("LeaveTakeGift.bmp"), x, y) > 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"离开领取奖励!");
			ClickPoint(LauncherRect.left + x + 290, LauncherRect.top + y + 7);
			continue;
		}

		// 离开房间
		if (dm.FindPic(1000, 425, 1044, 530, _T("27.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"离开房间!重新安排队伍!");
			ClickPoint(LauncherRect.left + 947 - 136, LauncherRect.top + 545);
			g_bCloseGame = TRUE;
			break;
		}

		// 判断是否在最初始的大厅中
		if (dm.FindPic(524, 11, 743, 82, L"4.bmp", 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"在最初始的大厅里, 点Play");
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
			continue;
		}

		if (dm.FindPic(0, 0, 829, 470, _T("12.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"12.bmp");
			ClickPoint(LauncherRect.left + x + 861, LauncherRect.top + y + 385);
			Sleep(1000);
			ClickPoint(LauncherRect.left + x + 889, LauncherRect.top + y + 545);
			Sleep(1000);
			continue;
		}

		if (dm.FindPic(647, 495, 738, 524, _T("13.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"13.bmp");
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			Sleep(2000);
			continue;
		}

		if (dm.FindPic(0, 0, 1000, 600, _T("17.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"17.bmp");
			ClickPoint(LauncherRect.left + x + 10, LauncherRect.top + y + 10);
			Sleep(2000);
			continue;
		}

		if (dm.FindPic(549, 500, 1000, 600, _T("20.bmp"), x, y))
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"被封号拉！！！");
			g_bGameTitle = TRUE;
			break;
		}

		if (dm.FindPic(786, 681, 1030, 770, _T("21.bmp"), 0, 0, 0.9, x, y))//回到大厅
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"回到大厅");
			ClickPoint(LauncherRect.left + 856 + 20, LauncherRect.top + 742);
			Sleep(2000);
			continue;
		}

		if (dm.FindPic(0, 0, 1000, 600, _T("24.bmp"), 0, 0, 0.9, x, y))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"24.bmp");
			ClickPoint(LauncherRect.left + x + 781, LauncherRect.top + y - 121);
			Sleep(2000);
			continue;
		}


		if (dm.FindPic(677, 617, 1071, 756, _T("5.bmp"), 0, 0, 0.95, x, y))//279  723
		{
			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"5.bmp");
			if (dm.FindPic(455, 350, 829, 470, _T("14.bmp"), 0, 0, 0.9, x, y))//1123  121
			{
				for (int i = 0; i < 5; i++)
				{
					ClickPoint(LauncherRect.left + 1256, LauncherRect.top + 194 + i);
				}

			}
			else
			{
				ClickPoint(LauncherRect.left + 1236, LauncherRect.top + 780);
			}
		}

		if (g_MapType == MapType_嚎哭深渊)
		{
			if (dm.FindPic(0, 0, 1280, 800, L"WaitChooseHero.bmp", x, y))
				goto lb_WaitSartGame;
		}

		// 是否在选英雄界面
		if (dm.FindPic(549, 500, 1000, 600, _T("18.bmp"), 0, 0, 0.9, x, y))//92 35 确定开局
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"正在搜索英雄……");
			// 首先先选英雄
			WCHAR szHeroBmpName[32] = { 0 };
			if (!GetHeroBmpName(emHeroPro, szHeroBmpName))
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"找不到该英雄，emHeroPro=%d", emHeroPro);
				continue;
			}

			if (!dm.FindPic(0, 0, 1280, 800, szHeroBmpName, 0, 0, 0.8, x, y))
			{
				// 查找后备英雄
				if (!GetHeroBmpName(emRetainHero, szHeroBmpName))
				{
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"找不到该英雄，emRetainHero=%d", emRetainHero);
					continue;
				}

				if (!dm.FindPic(0, 0, 1280, 800, szHeroBmpName, 0, 0, 0.8, x, y))
				{
					continue;
				}
			}

			CPrintLog::PrintLog_W(_SELF, __LINE__, L"点击英雄图标:%s", szHeroBmpName);
			ClickPoint(LauncherRect.left + x, LauncherRect.top + y);
			Sleep(1500);
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"点确定");
			ClickPoint(LauncherRect.left + 870, LauncherRect.top + 505);
			Sleep(2000);
		lb_WaitSartGame:;
			// 判断天赋技能1是否存在
			if (!dm.FindPic(531, 471, 700, 600, L"Talent1.bmp", x, y))
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"选择第一个天赋技能");
				if (g_MapType == MapType_嚎哭深渊)
				{
					ClickPoint(LauncherRect.left + 597, LauncherRect.top + 525);
					Sleep(1000);
					ClickPoint(LauncherRect.left + 469, LauncherRect.top + 258);
					Sleep(1000);
				}
				else
				{
					ClickPoint(LauncherRect.left + 597, LauncherRect.top + 525);
					Sleep(1000);
					ClickPoint(LauncherRect.left + 544, LauncherRect.top + 254);
					Sleep(1000);
				}
				
			}
			// 判断天赋技能2是否存在
			if (!dm.FindPic(531, 471, 700, 600, L"Talent2.bmp", x, y))
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"选择第二个天赋技能");
				if (g_MapType == MapType_嚎哭深渊)
				{
					ClickPoint(LauncherRect.left + 649, LauncherRect.top + 529);
					Sleep(1000);
					ClickPoint(LauncherRect.left + 547, LauncherRect.top + 266);
					Sleep(1000);
				}
				else
				{
					ClickPoint(LauncherRect.left + 649, LauncherRect.top + 529);
					Sleep(1000);
					ClickPoint(LauncherRect.left + 469, LauncherRect.top + 257);
					Sleep(1000);
				}
				
			}

			for (int i = 0; i < 90; ++i)
			{
				Sleep(1000);

				if (dm.FindPic(0, 0, 1280, 800, L"WaitChooseHero.bmp", x, y))
					continue;

				break;
			}
			Sleep(10 * 1000);
			dwStartGameTick = ::GetTickCount();
			continue;
		}
	}

	return TRUE;
}

BOOL CHall::RunTGP()
{
	SendClientMsg(L"正在启动TGP");
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"启动TGP登录器!");

	WCHAR wszLoginPath[MAX_PATH] = { 0 };
	::GetCurrentDirectoryW(MAX_PATH, wszLoginPath);
	lstrcatW(wszLoginPath, L"\\TGPLogin.exe -start");

	// 启动次数+1, 这是为了二次启动的时候就不要重复选区和修改模式
	g_pSharedInfo->nRunTGPCount += 1;

	// 设置好帐号密码
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameUser, g_ClientGroup.SLD.wszGameUser);
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszGamePass, g_ClientGroup.SLD.wszPass);
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszdamaUser, wszdamaUser);
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszdamaPass, wszdamaPass);
	CCharacter::wstrcpy_my(g_pSharedInfo->MyGameInfo[0].webInfo.wszGameServer, g_ClientGroup.wszServerName);

	STARTUPINFOW si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	BOOL bRet = ::CreateProcessW(
		NULL,
		wszLoginPath,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi);
	Sleep(10 * 1000);
	return TGPLogin();
}

BOOL CHall::TGPLogin()
{
	auto fnCloseTGPLogin = [](HANDLE& hMutex)
	{
		CLProcess::TerminateProc_For_ProcName(L"TGPLogin.exe");
		::ReleaseMutex(hMutex);
		::CloseHandle(hMutex);
		hMutex = NULL;
	};

	for (int i = 0; i < 5 * 60; ++i)
	{
		Sleep(1000);
		HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"TGPLogin_End_Mutex");
		if (hMutex != NULL)
		{
			if (!CCharacter::wstrcmp_my(g_pSharedInfo->wszTGPMsg,L"成功启动大厅!"))
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"wszTGPMsg=%s", g_pSharedInfo->wszTGPMsg);
				SendClientMsg(g_pSharedInfo->wszTGPMsg);
				Sleep(-1);
			}

			// Close && Release Res
			fnCloseTGPLogin(hMutex);
			return ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net 客户端") != NULL ? TRUE : FALSE;
		}
	}

	SendClientMsg(L"TGP校验5分钟超时!");
	Sleep(-1);
	return FALSE;
}

BOOL CHall::RunLolClient()
{
	// 置顶LOL大厅
	do 
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"启动TGP");
		RunTGP();
		Init();
	} while (hWndLuncher == NULL);	

	if (CLProcess::Is_Exist_Process_For_ProcName(LOL_CLIENTPROC_NAME))
	{
		auto fn_CopyInjectorDLL = []
		{
			// 复制文件到游戏目录下
			static WCHAR wszDLLPath[MAX_PATH];
			static WCHAR wszFullGamePath[MAX_PATH];

			// 游戏完整路径
			CCharacter::wstrcpy_my(wszFullGamePath, wszGamePath);
			lstrcatW(wszFullGamePath, L"\\Game\\d3d8th.dll");

			::GetCurrentDirectoryW(MAX_PATH, wszDLLPath);
			lstrcatW(wszDLLPath, L"\\d3d8th.dll");

			if (CLPublic::FileExit(wszDLLPath))
			{
				::CopyFileW(wszDLLPath, wszFullGamePath, FALSE);
				::DeleteFileW(wszDLLPath);
			}


			CCharacter::wstrcpy_my(wszFullGamePath, wszGamePath);
			lstrcatW(wszFullGamePath, L"\\Game\\d3d8thk.dll");

			::GetCurrentDirectoryW(MAX_PATH, wszDLLPath);
			lstrcatW(wszDLLPath, L"\\d3d8thk.dll");

			if (CLPublic::FileExit(wszDLLPath))
			{
				::CopyFileW(wszDLLPath, wszFullGamePath, FALSE);
				::DeleteFileW(wszDLLPath);
			}
		};

		auto fn_CopyMsvcr = [=](LPCWSTR pwszName)
		{
			static WCHAR wszSysPath[MAX_PATH] = { 0 };
			static WCHAR wszConsolePath[MAX_PATH] = { 0 };

			CCharacter::wstrcpy_my(wszSysPath, L"C:\\Windows\\SysWOW64\\");
			lstrcatW(wszSysPath, pwszName);

			
			::GetCurrentDirectoryW(MAX_PATH, wszConsolePath);
			lstrcatW(wszConsolePath, L"\\");
			lstrcatW(wszConsolePath, pwszName);

			if (CLPublic::FileExit(wszConsolePath))
			{
				::CopyFileW(wszConsolePath, wszSysPath, FALSE);
				::DeleteFileW(wszConsolePath);
			}
		};

		auto fn_ChangeGameConfig = []
		{
			static WCHAR wszPath[MAX_PATH] = { 0 };
			CCharacter::wstrcpy_my(wszPath, wszGamePath);

			lstrcatW(wszPath, L"\\Game\\Config\\game.cfg");
			::WritePrivateProfileStringW(L"General", L"WindowMode", L"1", wszPath);
			::WritePrivateProfileStringW(L"General", L"Height", L"768", wszPath);
			::WritePrivateProfileStringW(L"General", L"Width", L"1024", wszPath);
		};

		if (!CLProcess::Is_Exist_Process_For_ProcName(LOL_GAMEPROC_NAME) && CConsoleConfig::GetGamePath_By_Process(wszGamePath))
		{
			// 拷贝d3d8thk和d3d8th到游戏目录下
			fn_CopyInjectorDLL();

			// 剪切msvc库到SysWow64下
			fn_CopyMsvcr(L"mfc120ud.dll");
			fn_CopyMsvcr(L"mfc120u.dll");
			fn_CopyMsvcr(L"msvcp120.dll");
			fn_CopyMsvcr(L"msvcp120d.dll");
			fn_CopyMsvcr(L"msvcr120.dll");
			fn_CopyMsvcr(L"msvcr120d.dll");

			// 修改分辨率
			fn_ChangeGameConfig();
		}
		else
		{
			SendClientMsg(L"获取游戏路径失败! 理论上不可能会发生的");
			//Sleep(-1);
		}
	}

	HWND hTGP = ::FindWindowW(L"TWINCONTROL", L"腾讯游戏平台");
	if (hTGP != NULL)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"TGP取消窗口置顶并且窗口最小化!");
		::SetWindowPos(hTGP, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		::SendMessage(hTGP, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
		Sleep(1000);
	}

	ClearTaskMenu();
	SendClientMsg(L"LOL大厅启动完毕!");
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"RunLolClient done!");
	return TRUE;
}

em_GameStatus CHall::WaitToGameStart()
{
	static DWORD dwTick = 0;
	if (bGameStart)
		return em_GameStatus_Start;
	if (dwTick == NULL)
		dwTick = ::GetTickCount();

	HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, Mutex_GameALive_Name);

	// 读图超时3分钟
	if (CLProcess::Is_Exist_Process_For_ProcName(L"WerFault.exe"))
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"出现游戏停止工作!");
		CLProcess::TerminateProc_For_ProcName(L"WerFault.exe");
		Sleep(2000);
		bGameStart = FALSE;
		dwTick = NULL;
		return em_GameStatus_Faild;
	}
	else if (::GetTickCount() - dwTick >= 6 * 60 * 1000 || hMutex != NULL)
	{
		// 判断是否成功进入游戏的互斥
		dwTick = 0;
		if (hMutex != NULL)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"成功进入游戏!");
			::CloseHandle(hMutex);
			hMutex = NULL;
			bGameStart = TRUE;
			return em_GameStatus_Start;
		}

		// 如果没成功的话, 就把游戏给X掉!
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"6分钟内都无法进入游戏……X掉游戏重开!");
		CLProcess::TerminateProc_For_ProcName(LOL_GAMEPROC_NAME);
		bGameStart = FALSE;
		dwTick = NULL;
		return em_GameStatus_Faild;
	}
	bGameStart = FALSE;
	return em_GameStatus_Load;
}

BOOL CHall::IsSealUser()
{
	CBmpFind dm;
	WCHAR wszPath[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Bmp");
	dm.SetPath(wszPath);
	int x = 0, y = 0;

	dm.BindWindow(CSearchPic::GetGameHwnd());
	HandleTV();

	if (dm.FindPic(0, 0, 1280, 800, _T("20.bmp"), x, y) > 0)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"被封号!……");
		g_bCloseGame = TRUE;
		g_bGameTitle = TRUE;
		return TRUE;
	}

	return FALSE;
}

BOOL CHall::IsLoseCon()
{
	static RECT LauncherRect;
	CBmpFind dm;

	// init
	WCHAR wszPath[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Bmp");
	dm.SetPath(wszPath);
	int x = 0, y = 0;

	if (dm.FindPic(0, 0, 1280, 800, _T("HideMsg1.bmp"), x, y) > 0)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"不要挡路!HideMsg1");
		ClickPoint(LauncherRect.left + x + 20, LauncherRect.top + y + 20);
		Sleep(5 * 1000);
	}

	// 是否掉线重连
	Init();
	dm.BindWindow(CSearchPic::GetGameHwnd());
	Sleep(3000);
	if (dm.FindPic(0, 0, 1280, 800, _T("22.bmp"), x, y) > 0)
	{
		return TRUE;
	}
	return FALSE;
}