#include "GamePicHandle.h"

CGamePicHandle::CGamePicHandle()
{
	ZeroMemory(wszClassName, sizeof(wszClassName));
	ZeroMemory(wszTextName, sizeof(wszTextName));
}

CGamePicHandle::~CGamePicHandle()
{
}

VOID CGamePicHandle::ClickPoint(int x, int y)
{
	for (int i = 0; i < 1; ++i)
	{
		// 设置鼠标
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, NULL, NULL);
		mouse_event(MOUSEEVENTF_LEFTUP, x, y, NULL, NULL);
	}
}

VOID CGamePicHandle::SetWinInfo(LPCWSTR pwszClassName, LPCWSTR pwszTextName, GamePicType PicType)
{
	CCharacter::wstrcpy_my(wszClassName, pwszClassName);
	CCharacter::wstrcpy_my(wszTextName, pwszTextName);
	m_GamePicType = PicType;
}

HWND CGamePicHandle::GetWnd()
{
	HWND hConsoleWnd = ::FindWindowW(wszClassName, wszTextName);
	if (hConsoleWnd != NULL)
		return hConsoleWnd;
	return ::GetDesktopWindow();
}

VOID CGamePicHandle::InitBmpFind()
{
	WCHAR wszPath[MAX_PATH];

	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Bmp");
	m_BmdFind.SetPath(wszPath); // Bind Path
}

VOID CGamePicHandle::ShowGameWindow()
{
	ShowWindow(GetWnd(), SW_RESTORE);
	::SetWindowPos(GetWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SwitchToThisWindow(GetWnd(), TRUE);

	if (m_GamePicType == GamePicType_LOLConsole)
	{
		// 按照国际惯例, 每次都点一下窗口左上角以示尊敬!
		RECT r;
		::GetWindowRect(GetWnd(), &r);
		ClickPoint(r.left + 100, r.top + 10);
	}
}