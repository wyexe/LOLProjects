#ifndef __LOL_CONSOLE_GAMEPICHANDLE_H__
#define __LOL_CONSOLE_GAMEPICHANDLE_H__

#include "SearchPic.h"

typedef enum _GamePicType
{
	GamePicType_TGP,
	GamePicType_LOLConsole
}GamePicType;

class CGamePicHandle
{
public:
	CGamePicHandle();
	~CGamePicHandle();

	// Mouse Click
	static VOID ClickPoint(int x, int y);

	// Set ClassName && TextName
	VOID SetWinInfo(LPCWSTR pwszClassName, LPCWSTR pwszTextName, GamePicType PicType);

	// init m_BmpFind
	VOID InitBmpFind();

private:
	// BindWindow Parm
	HWND GetWnd();

	// TopMost for GameWindows
	VOID ShowGameWindow();

private:
	WCHAR wszClassName[MAX_PATH];
	WCHAR wszTextName[MAX_PATH];
	CBmpFind m_BmdFind;
	GamePicType m_GamePicType;
};


#endif