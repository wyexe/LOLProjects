#ifndef __GAMEFUNCTION_H__
#define __GAMEFUNCTION_H__

#include "GameSend.h"
#include "Monster.h"
#include "Skill.h"
#include <memory>
#include "GameRes.h"
#include "Equment.h"
#include "Field.h"
#define EMPTY_PERSONHP (PERSON->GetHp() == NULL)

class CGameFunction
{
public:
	CGameFunction();
	~CGameFunction();
public:
	BOOL InitAccountGame();

	BOOL InitTimer();

	BOOL TerminateTestThread();

	///////Move///////////////////////////////////////////////////////////////////
	// 走路,一直走到终点坐标为止
	BOOL MoveToPoint(Point& EndPoint, float fDis);

	// 往后退
	BOOL BackOff();

	// 走到终点
	BOOL MoveEnd();

	BOOL MoveToEndPoint();

	BOOL MoveToEnemyTurrent();

	// 判断距离来使用技能
	BOOL UseSkill_By_Dis(em_Skill_Flag emSkillFlag, CMonster& Obj);

	/////////Other Function/////////////////////////////////////////////////////////////////
	// 是否游戏结束了
	BOOL IsGameOver();

	// 是不是该回城了
	BOOL IsBackHome();

	// 回家补给 or 补下状态
	BOOL BackHome();

	// 恢复生命值才去
	BOOL Rest();

	// 等待游戏开始
	BOOL WaitToStartGame();

	// 学习技能处理
	BOOL HandleSkillLevelUp();

	////////////AroundObject//////////////////////////////////////////////////////////////
	UINT GetMonsterList_By_Type(vector<CMonster>& vMonsterList, em_MonsterType emType, em_Camp emCamp, float fDis, Point* pFixPoint);

	// 寻找怪物
	BOOL FindMonster_For_Name(__in LPCWSTR pwszName, __in float fDis,__out CMonster& Monster);

	// 返回贫血的索引
	int GetAnemiaIndex(vector<CMonster>& vMonsterList);

	//////////Soilder////////////////////////////////////////////////////////////////
	// 获取附近的敌对小兵数量
	UINT GetAroundEnemySoliderCount(float fDis, Point* pFixPoint);

	// 获取附近的同盟小兵数量
	UINT GetAroundAllianceSoliderCount(float fDis, Point* pFixPoint);

	// 获取要攻击的小兵
	BOOL GetRecentlyEnemySolider(float fDis, Point* pFixPoint, CMonster& Monster);

	// 判断是否要清兵
	BOOL IsClearSolider();

	// 判断是否需要躲避小兵
	BOOL IsDodgeSolider();

	// 获取攻击自己的小兵数量
	UINT GetAttackSelfSoliderCount(float fDis, Point* pFixPoint);

	// 闪避敌方小兵的攻击
	BOOL DodgeEnemySolider();

	// 统治战场 获取附近除了自己的塔以外的塔
	UINT GetAroundEnemyGuardian(vector<CMonster>& vlst, float fDis, Point* pFixPoint);

	// 占领塔
	BOOL OccupyEnemyTurret();

	///////Hero///////////////////////////////////////////////////////////////////
	// 获取附近的敌对英雄的数量
	UINT GetAroundEnemyHeroCount(float fDis, Point* pFixPoint);

	// 获取附近的同盟英雄的数量
	UINT GetAroundAllianceHeroCount(float fDis, Point* pFixPoint);

	// 获取要攻击的英雄
	BOOL GetRecentlyEnemyHero(float fDis, Point* pFixPoint, CMonster& Monster);

	// 是否闪避敌方英雄的攻击
	BOOL IsDodgeHero();

	// 获取攻击自己的英雄数量
	UINT GetAttackSelfHeroCount(float fDis, Point* pFixPoint);

	// 是否主动攻击敌方英雄
	BOOL IsAttackHero();

	// 是否消耗敌方英雄的血量
	BOOL IsCoolConsumeSkill(DWORD dwLevel);
	BOOL IsConsumeEnemyHero();

	// 判断范围内,  有多少个队友达到x级以上
	UINT GetAroundAllianceHeroCount_By_Level(float fDis, DWORD dwLevel);

	///////////Turret///////////////////////////////////////////////////////////////
	// 获取最近塔的坐标
	Point GetRecentlyTurret(int nIndex, em_Path_Flag emPathFlag);

	// 移动到最近的塔下
	BOOL MoveToTurret(int nIndex, em_Path_Flag emPathFlag);

	// 获取附近敌对阵营的塔的数量
	BOOL GetAroundEnemyTurretCount(float fDis, Point* pFixPoint);

	// 获取要攻击的塔
	BOOL GetRecentlyEnemyTurret(float fDis, Point* pFixPoint, CMonster& Monster);

	// 获取自己最近友方的塔
	BOOL GetRecentlyAllianceTurret(float fDis, Point* pFixPoint, CMonster& Turret);

	// 是否清塔
	BOOL IsAttackTurret();

	// 是否存在中路水晶塔
	BOOL ExistMiddleCrystalTurret();

	// 匹配分路
	BOOL SetMatchingPath();

	//
	BOOL SetGamePath();

public:

	HANDLE hGuardPersonThread;
public:

};

#endif//__GAMEFUNCTION_H__