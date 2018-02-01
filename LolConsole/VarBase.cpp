#include "VarBase.h"

BOOL g_bRecvGroupMsg = FALSE;
DWORD g_dwCmd = GameCmd_Stop;
em_Hero_Pro emHeroPro = em_Hero_Pro_Ryze;
em_Hero_Pro emRetainHero = em_Hero_Pro_Ryze;
BOOL g_bCaptain = FALSE;
HWND hWndLuncher = NULL;
HWND hWndGame = NULL;
vector<TempSrc32> TeamMemNameberLst;
SockClientGroup g_ClientGroup;
BOOL g_bRecvLoginMsg = FALSE;
PSHARED_INFO g_pSharedInfo = NULL;
BOOL g_bRecvLeaveMsg = FALSE;
BOOL g_bGameTitle = FALSE;
BOOL g_bCloseGame = FALSE;
WCHAR wszdamaUser[32] = { 0 };
WCHAR wszdamaPass[32] = { 0 };
HANDLE hWorkThread = NULL;
MapType g_MapType = MapType_ÕÙ»½Ê¦Ï¿¹È;
BOOL g_bRecvIdSealedMsg = FALSE;
DWORD g_dwRecvMsgType = 0;
BOOL g_bSwitchAccount = FALSE;
WCHAR g_wszClientMsg[64] = { 0 };
WCHAR wszGamePath[MAX_PATH] = { 0 };
BOOL g_bMatching = FALSE;