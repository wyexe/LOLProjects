#include "SearchPic.h"

#define _SELF L"SearchPic.cpp"

CBmpFind CSearchPic::CBF;

CSearchPic::CSearchPic()
{
}

CSearchPic::~CSearchPic()
{
}

BOOL CSearchPic::ExistBmp(LPCWSTR pwszBmpName, BmpPoint& RetPT)
{
	static bool bInit = false;
	if (!bInit)
	{
		WCHAR wszPath[MAX_PATH];
		::GetCurrentDirectoryW(MAX_PATH, wszPath);
		lstrcatW(wszPath, L"\\Bmp");
		CBF.SetPath(wszPath);
	}

	CBF.BindWindow(GetGameHwnd());
	Sleep(300);
	BmpPoint Bp;
	if (CBF.FindPic(0, 0, 0, 0, pwszBmpName, 0, 0, 0.8, Bp.x, Bp.y))
	{
		RetPT = Bp;
		return TRUE;
	}
	RetPT.x = RetPT.y = 0;
	return FALSE;
}


BOOL CSearchPic::GetGameRect(RECT& r)
{
	hWndLuncher = ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net 客户端");
	::GetWindowRect(hWndLuncher, &r);
	return TRUE;
}

HWND CSearchPic::GetGameHwnd()
{
	hWndLuncher = ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net 客户端");
	if (hWndLuncher == NULL || ::IsWindowVisible(hWndLuncher) != TRUE)
	{
		return ::GetDesktopWindow();
	}
	return hWndLuncher;
}
