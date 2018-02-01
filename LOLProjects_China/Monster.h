#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "GameSend.h"

#define PERSON CMonster::GetPerson()

class CMonster
{
public:
	CMonster() { Init(0); };
	CMonster(DWORD dwBase) { Init(dwBase); };
	~CMonster();

	BOOL Init(DWORD dwBase);
	DWORD GetBase();
	VOID SetPerson();

	// Person
	DWORD GetId();

	// HP
	DWORD GetHp();
	DWORD GetMaxHp();
	DWORD GetPercentHp();

	// MP
	DWORD GetMp();
	DWORD GetMaxMp();
	DWORD GetPercentMp();

	// Type
	em_MonsterType GetMonsterType();
	BOOL IsCrystalTurret();

	// Point
	Point GetPoint();
	float GetDis(Point& TarPoint);

	// Name
	LPWSTR GetName();

	// Camp
	em_Camp GetCamp();
	em_Camp GetEnemyCamp();

	// Soilder
	BOOL IsSuperSoilder();

	// Level
	DWORD GetLevel();

	// SkillPoint
	DWORD GetObj();
	DWORD GetSkillPoint();

	// Money
	DWORD GetMoney();

	// TarId
	DWORD GetTargetId();

	// Occ
	LPWSTR GetHeroName();

	DWORD GetMonsterParm();
	BOOL IsShow();

	// Attack Power
	static DWORD GetPersonAttackPower();

	// Attack this Target
	BOOL NormalAttack();

	BOOL Occupy();

	// Attack Dis
	float GetAttackDis();

	bool operator < (const CMonster& Obj);

	static UINT GetAroundObjectList(vector<CMonster>& vlst);

	static CMonster* GetPerson();

	// ÊÇ·ñÆ¶ÑªÁË
	BOOL IsAnemia();
	BOOL IsLackBlood();

	BOOL IsDead();
private:
	DWORD dwMonsterBase;
	WCHAR wszName[32];
	WCHAR wszHeroName[32];
	BOOL  bPerson;
	float fDis;
	DWORD dwMonsterParm;
};



#endif