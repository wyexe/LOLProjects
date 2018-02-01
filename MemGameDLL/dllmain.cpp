// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "../../MyTools/MyTools/CLNoModuleDLL.h"
#include "../LOLProjects_China/FightFunction.h"
#include <time.h>

#define NO_MODULE_MARK 0x20151026

#define _SELF L"dllmain.cpp"

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

static BOOL SetMapType()
{
	HANDLE hMapTypeMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOL_Game_MapType_0");
	if (hMapTypeMutex != NULL)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"当前选择的地图是:峡谷");
		g_MapType = MapType_召唤师峡谷;
		goto lb_Exit;
	}

	hMapTypeMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOL_Game_MapType_1");
	if (hMapTypeMutex != NULL)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"当前选择的地图是:统治");
		g_MapType = MapType_水晶之痕;
		goto lb_Exit;
	}

	hMapTypeMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOL_Game_MapType_2");
	if (hMapTypeMutex != NULL)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"当前选择的地图是:扭曲丛林");
		g_MapType = MapType_扭曲丛林;
		goto lb_Exit;
	}

	hMapTypeMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOL_Game_MapType_3");
	if (hMapTypeMutex != NULL)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"当前选择的地图是:嚎哭深渊");
		g_MapType = MapType_嚎哭深渊;
		goto lb_Exit;
	}

	// Default set
	g_MapType = MapType_召唤师峡谷;
	
lb_Exit:;
	if (hMapTypeMutex != NULL)
	{
		::ReleaseMutex(hMapTypeMutex);
		::CloseHandle(hMapTypeMutex);
		hMapTypeMutex = NULL;
	}
	return TRUE;
}

void WINAPIV WorkThread(LPVOID lpParm)
{
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"WorkThread");
	CreateMutexW(NULL, FALSE, L"CL_LOL_Game_Load_Wait");
	HANDLE hMutex = OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"CL_LOL_Console_Mutex");
	if (hMutex == NULL)
	{
		//CPrintLog::PrintLog_W(_SELF,__LINE__,L"非正常启动……不给予挂机功能!");
		//return;
	}

	SetMapType();

	static DWORD dwTick = ::GetTickCount();
	static BOOL  bGameLoad = FALSE;

	CPrintLog::PrintLog_W(_SELF,__LINE__,L"Loading……");
	while (PERSON->GetBase() == NULL)
	{
		::Sleep(1000);
		if (!bGameLoad && ::GetTickCount() - dwTick >= 10 * 1000)
		{
			CreateMutexW(NULL, FALSE, L"CL_LOL_Game_Load_Done");
			bGameLoad = TRUE;
		}
	}
	g_dwStartGameCmd = GameCmd_Start;
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"SetGameCmd");
	srand((UINT)time(NULL));
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"srand");
	if (g_pAccountGame == NULL)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"Alloc Start");
		g_pAccountGame = new ACCOUNT_INFO_GAME;
		memset(g_pAccountGame, 0, sizeof(ACCOUNT_INFO_GAME));
		g_pAccountGame->MyAcount_Info.dwStartTick = ::GetTickCount();
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"Alloc End");
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

	::CreateMutexW(NULL, FALSE, Mutex_CloseGame_Name);
	Sleep(5 * 1000);
	
	RECT r;
	::GetWindowRect(g_pAccountGame->hWndGame, &r);
	for (int i = 0; i < 2; ++i)
	{
		ClickPoint(r.left + 645, r.top + 500);
		Sleep(500);
	}
	for (int i = 0; i < 2; ++i)
	{
		ClickPoint(r.left + 520, r.top + 480);
		Sleep(500);
	}
	for (int i = 0; i < 2; ++i)
	{
		ClickPoint(r.left + 525, r.top + 505);
		Sleep(500);
	}
	Sleep(20 * 1000);

	_asm XOR ESP,ESP
	CLPublic::ForceExit();

	return;
}

__declspec(naked) VOID WINAPI HookMessageBox()
{
	/*CPrintLog::PrintLog_W(_SELF,__LINE__,L"游戏网络挂了……");
	CreateMutexW(NULL, FALSE, Mutex_GameLoseCon_Name);
	Sleep(10 * 1000);
	::TerminateProcess(::GetCurrentProcess(), 0);
	CLPublic::ForceExit();
	Sleep(-1);*/
}

BOOL InitNoModule(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	auto fnHook = [](DWORD dwHookAddr)
	{
		MYHOOK_CONTENT HC;
		HC.dwFunAddr = (DWORD)HookMessageBox;
		HC.dwHookAddr = dwHookAddr;
		HC.dwOldCode1 = ReadDWORD(HC.dwFunAddr + 0x0);
		HC.dwOldCode2 = ReadDWORD(HC.dwFunAddr + 0x4);
		HC.uNopCount = 0x0;
		CLHook::Hook_Fun_Jmp_MyAddr(&HC);
	};
	
	static CHAR szModuleFile[MAX_PATH];
	::GetModuleFileNameA(hModule, szModuleFile, MAX_PATH);
	
	WCHAR wszExeName[MAX_PATH];
	::GetModuleFileNameW(NULL, wszExeName, MAX_PATH);
	if (CCharacter::wstrstr_my(wszExeName, L"League of Legends"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"开始加载……");

		HMODULE hmUser32 = ::GetModuleHandleW(L"USER32.dll");
		if (hmUser32 != NULL)
		{
			fnHook((DWORD)MessageBoxA);
			fnHook((DWORD)MessageBoxW);
		}

		//CLNoModuleDLL NoModuleDLL;
		//SetErrorMode(SEM_NOGPFAULTERRORBOX); // 系统不显示Windows错误报告对话框
		//NoModuleDLL.LaunchDll("d3d8th.dll", NO_MODULE_MARK);
		//NoModuleDLL.HideDllPeb(L"d3d8th.dll");
		//NoModuleDLL.HideDllHashLink(L"d3d8th.dll");

	}
	

	return FALSE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		//InitNoModule(hModule, ul_reason_for_call, lpReserved);
		_beginthread(WorkThread, NULL, NULL);
		return TRUE;
		//return InitNoModule(hModule, ul_reason_for_call, lpReserved);
	}
	else if (ul_reason_for_call == NO_MODULE_MARK)
	{
		_beginthread(WorkThread, NULL, NULL);
		return TRUE;
	}

	return FALSE;
}

