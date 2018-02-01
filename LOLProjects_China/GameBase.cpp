#include "stdafx.h"
#include "GameBase.h"

HANDLE g_hFileMap = NULL;
HWND g_hConsoleHwnd = NULL;
PSHARED_INFO g_pSharedInfo = NULL;
PACCOUNT_INFO_GAME g_pAccountGame = NULL;
DWORD g_dwStartGameCmd = 0x0;
vector<MYHOOK_CONTENT> g_HookItemList;
BOOL g_bStartHook = FALSE;
DWORD g_dwStudySkillIndex = -1;
DWORD g_dwBuyItemId = 0;
BOOL g_bMove = FALSE;
Point g_MovePoint;
DWORD g_dwNormalAttackBase = NULL;
Point g_AttackPoint;
_Hook_UseSkillInfo g_Hook_UseSkillInfo;
BOOL g_bUseSkill = FALSE;
DWORD g_dwHookSimKeyAddr = NULL;
DWORD g_dwHookSimKeyCALL = NULL;
DWORD g_dwMainThreadId = 0;
BOOL g_bStopAction = FALSE;
BOOL g_bfnMove = FALSE;
DWORD g_dwForgeESP = 0;
DWORD g_dwTimerId = NULL;
DWORD g_dwUseItemParm = NULL;
float g_fMaxDis_Attack = 10.0f;
DWORD g_dwHookD3dAddr = NULL;
DWORD g_dwHookD3dValue = NULL;
DWORD g_dwFakeStackFrame[MAX_PATH] = { 0 };
BOOL g_bGameStart = FALSE;
CLVector<GameTalkMsg> g_vGameTalkMsg;
DWORD g_dwOccupyObjBase = NULL;
MapType g_MapType = MapType_召唤师峡谷;