#ifndef __GAMEBASE_H__
#define __GAMEBASE_H__
#include "GameStruct.h"
#include "../../MyTools/MyTools/CLVector.h"
/************************************************************************/
/* 全局变量                                                                     */
/************************************************************************/
extern HANDLE						g_hFileMap;								//内存共享的句柄
extern HWND							g_hConsoleHwnd;							//控制台的句柄
extern PSHARED_INFO					g_pSharedInfo;							//映射到本进程内存的地址
extern PACCOUNT_INFO_GAME			g_pAccountGame;							//自身的帐号信息
extern DWORD						g_dwStartGameCmd;						//挂机的指令
extern vector<MYHOOK_CONTENT>		g_HookItemList;							//inline Hook列表
extern BOOL							g_bStartHook;							// 开始运行Hook运行自定义函数
extern DWORD						g_dwStudySkillIndex;					// 学习技能的索引
extern DWORD						g_dwBuyItemId;							// 购买物品的Id
extern BOOL							g_bMove;								// 是否走路
extern Point						g_MovePoint;							// 走路的坐标
extern DWORD						g_dwNormalAttackBase;					// 普通攻击的Obj
extern Point						g_AttackPoint;							// 攻击的坐标
extern _Hook_UseSkillInfo			g_Hook_UseSkillInfo;					// 使用技能参数
extern BOOL							g_bUseSkill;							// 使用技能
extern DWORD						g_dwHookSimKeyAddr;						// 钩挂按键
extern DWORD						g_dwHookSimKeyCALL;						// 同上
extern DWORD						g_dwMainThreadId;						// 主线程Id
extern BOOL							g_bStopAction;							// 是否停止动作
extern BOOL							g_bfnMove;								// 是否调用了走路
extern DWORD						g_dwForgeESP;							// 伪造的堆栈
extern DWORD						g_dwTimerId;							// 时钟的Id
extern DWORD						g_dwUseItemParm;						// 使用物品的ECX
extern float						g_fMaxDis_Attack;						// 最大攻击范围
extern DWORD						g_dwHookD3dAddr;						// HookD3d
extern DWORD						g_dwHookD3dValue;						// 原值
extern DWORD						g_dwFakeStackFrame[MAX_PATH];			// 伪造堆栈
extern BOOL							g_bGameStart;							// 是否游戏开始了
extern CLVector<GameTalkMsg>		g_vGameTalkMsg;							// 聊天消息
extern DWORD						g_dwOccupyObjBase;						// 统治占领塔的Base
extern MapType						g_MapType;								// 当前的地图类型
#endif//__GAMEBASE_H__