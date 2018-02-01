#ifndef __LOL_VARBASE_H__
#define __LOL_VARBASE_H__

#include "../LOLProjects_China/GameStruct.h"

///////define ///////////////////////////////////////////////////////////////////
#define GameStatus_Run  (g_dwCmd == 1)
#define StopGame		g_dwCmd = GameCmd_Start


/////////struct/////////////////////////////////////////////////////////////////
typedef struct _TempSrc32
{
	WCHAR wszText[32];
}TempSrc32;

typedef struct _MatchSrc
{
	int nLeft;
	int nRight;
	int nTop;
	int nButtom;
	float fSim;
	WCHAR wszBmpName[32];
	int nX;
	int nY;
}MatchSrc;

/////////Var/////////////////////////////////////////////////////////////////
extern BOOL g_bRecvGroupMsg;
extern DWORD g_dwCmd;
extern em_Hero_Pro emHeroPro;
extern em_Hero_Pro emRetainHero;
extern BOOL g_bCaptain;
extern HWND hWndLuncher;
extern HWND hWndGame;
extern vector<TempSrc32> TeamMemNameberLst;
extern SockClientGroup g_ClientGroup;
extern BOOL g_bRecvLoginMsg;
extern PSHARED_INFO g_pSharedInfo;
extern BOOL g_bRecvLeaveMsg;
extern BOOL g_bGameTitle;
extern BOOL g_bCloseGame;
extern WCHAR wszdamaUser[32];
extern WCHAR wszdamaPass[32];
extern HANDLE hWorkThread;
extern MapType g_MapType;
extern BOOL g_bRecvIdSealedMsg;
extern DWORD g_dwRecvMsgType;
extern BOOL g_bSwitchAccount;
extern WCHAR g_wszClientMsg[64];
extern WCHAR wszGamePath[MAX_PATH];
extern BOOL g_bMatching;
#endif// __LOL_VARBASE_H__