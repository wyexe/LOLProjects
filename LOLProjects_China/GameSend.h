#ifndef __GAMESEND_H__
#define __GAMESEND_H__

#include "GameBase.h"

#define StopGameFun_Retn_True	if ( g_dwStartGameCmd == GameCmd_Stop ) return TRUE;
#define StopGameFun_Retn_False	if ( g_dwStartGameCmd == GameCmd_Stop ) return FALSE;
#define StopGameFun_Retn_Null	if ( g_dwStartGameCmd == GameCmd_Stop ) return ;
#define GameStatus_Run			(g_dwStartGameCmd == GameCmd_Start)
#define StopGame				g_dwStartGameCmd = GameCmd_Stop
#define WAITTIME				300

class CGameSend
{
public:
	CGameSend();
	~CGameSend();
public:
	static BOOL  GameTalk(LPCWSTR pwszText);
	static float Get3DDistance(const Point * LParm, const Point * RParm);
	static float Get2DDistance(const Point * LParm, const Point * RParm);
	static VOID Sleep(DWORD dwTime);
	static DWORD GetStartTime();
	static BOOL BuyItem(DWORD dwItemId);
	static BOOL Move(Point& EndPoint);
	static BOOL StopAction();
	static BOOL LockView();
	static DWORD GetSelfHp();
	static DWORD GetEnemyHp();
	static DWORD ReadD(DWORD dwAddr);
	static BYTE ReadB(DWORD dwAddr);
	static float ReadF(DWORD dwAddr);
private:
	//私有函数
public:
	//共有函数
	
private:
	//私有属性
};

VOID WINAPI CALL_StudySkill(DWORD(&dwFakeStackFrame)[260]);
VOID WINAPI CALL_BuyItem(DWORD(&dwFakeStackFrame)[260]);
VOID WINAPI CALL_Move(DWORD(&dwFakeStackFrame)[260]);
VOID WINAPI CALL_Attack(DWORD(&dwFakeStackFrame)[260]);
VOID WINAPI CALL_Stop(DWORD(&dwFakeStackFrame)[260]);
VOID WINAPI CALL_UseSkill(DWORD(&dwFakeStackFrame)[260]);
static DWORD CALL_GetMonsterName(DWORD dwBase);
VOID WINAPI CALL_UseItem(DWORD(&dwFakeStackFrame)[260]);
VOID WINAPI CALL_Occupy(DWORD(&dwFakeStackFrame)[260]);

#endif//__GAMESEND_H__