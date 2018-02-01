// AGEProjects_China.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "AGEProjects_China.h"
#include "Game.h"
#include "FightFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _SELF L"AGEProjects_Korea.cpp"

BEGIN_MESSAGE_MAP(CAGEProjects_ChinaApp, CWinApp)
END_MESSAGE_MAP()

// CAGEProjects_ChinaApp 构造

CAGEProjects_ChinaApp::CAGEProjects_ChinaApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAGEProjects_ChinaApp 对象

CAGEProjects_ChinaApp theApp;

DWORD WINAPI ShowThread(LPVOID lpParm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CGame CG;
	CG.DoModal();

	return 0;
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
	//CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"ClickPoint");
	return TRUE;
}

DWORD WINAPI KillThread(LPVOID lpParm)
{
	/*HWND hWndLuncher = ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net 客户端");
	if (hWndLuncher != NULL && ::IsWindowVisible(hWndLuncher))
	{
		::PostMessageW(hWndLuncher, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	}*/

	while (PERSON->GetBase() == NULL)
	{
		::Sleep(1000);
	}
	g_dwStartGameCmd = GameCmd_Start;

	srand((UINT)time(NULL));
	if (g_pAccountGame == NULL)
	{
		g_pAccountGame = new ACCOUNT_INFO_GAME;
		memset(g_pAccountGame, 0, sizeof(ACCOUNT_INFO_GAME));
		g_pAccountGame->MyAcount_Info.dwStartTick = ::GetTickCount();
	}

	while (g_pAccountGame->hWndGame == NULL)
	{
		g_pAccountGame->hWndGame = ::FindWindowW(L"RiotWindowClass", NULL);
		Sleep(1000);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"寻找窗口中……");
	}
	g_dwMainThreadId = CLThread::GetMainThreadId();

	CFightFunction CFF;
	CFF.PartyStart();
	Sleep(5 * 1000);
	::CreateMutexW(NULL, FALSE, Mutex_CloseGame_Name);

	RECT r;
	::GetWindowRect(g_pAccountGame->hWndGame, &r);
	for (int i = 0; i < 2; ++i)
	{
		ClickPoint(r.left + 645, r.top + 500);
		Sleep(500);
	}
	Sleep(5 * 1000);

	::TerminateProcess(::GetCurrentProcess(), 0);
	ExitProcess(0);
	CLPublic::ForceExit();

	

	return 0;
}

BOOL CAGEProjects_ChinaApp::InitInstance()
{
	CWinApp::InitInstance();
	HANDLE hMutex = OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOL_Console_Mutex");
	::CloseHandle(chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)ShowThread, NULL, NULL, NULL));
	/*if (hMutex != NULL)
	{
		::CloseHandle(chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)KillThread, this, NULL, NULL));
	}
	else
	{
		::CloseHandle(chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)ShowThread, NULL, NULL, NULL));
	}*/
	
	return TRUE;
}

__declspec(dllexport) int WINAPI BBB(DWORD dwID)
{
	return 0xF; 
}

int CAGEProjects_ChinaApp::ExitInstance()
{
	HANDLE curProcess = ::GetCurrentProcess();
	/*for (UINT i = 0; i < g_HookItemList.size(); ++i )
	{
		MYHOOK_CONTENT HC = g_HookItemList.at(i);
		::WriteProcessMemory(curProcess, (LPVOID)HC.dwHookAddr, &HC.dwOldCode1, 4, 0);
		::WriteProcessMemory(curProcess, (LPVOID)(HC.dwHookAddr + 4), &HC.dwOldCode2, 4, 0);
	}*/

	if (g_dwTimerId != NULL)
	{
		::KillTimer(g_pAccountGame->hWndGame, g_dwTimerId);
	}

	/*DWORD dwOldProtect = 0;
	if (::VirtualProtectEx(GetCurrentProcess(), (LPVOID)(g_dwHookD3dAddr + 0x44), 4, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		*(PDWORD)(g_dwHookD3dAddr + 0x44) = g_dwHookD3dValue;
		::VirtualProtectEx(GetCurrentProcess(), (LPVOID)(g_dwHookD3dAddr + 0x44), 4, dwOldProtect, &dwOldProtect);
	}*/

	//g_HookItemList.clear();
	CWinApp::ExitInstance();
	return 0;
}