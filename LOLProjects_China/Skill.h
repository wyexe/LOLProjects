#ifndef __SKILL_H__
#define __SKILL_H__

#include "GameSend.h"
#include "Monster.h"
#include "GameRes.h"

class CSkill
{
public:
	CSkill();
	~CSkill();
	CSkill(DWORD dwIndex, DWORD dwBase);

	VOID Init(DWORD dwBase);
	DWORD GetBase(){ return dwSkillBase; };

	// Level
	DWORD GetSkillLevel();

	// Name
	LPWSTR GetName();

	// Cool?
	BOOL IsCool();

	DWORD GetUseMp();

	BOOL GetUseStatus();

	// 使用技能
	BOOL UseSkill(CMonster& Monster, em_SkillType emSkillType);

	// 初始化技能
	static BOOL InitNormalSkill();

	// 判断技能是否冷却完毕
	static BOOL GetSkillCool(em_Skill_Flag emSkillFlag);

	// 使用技能
	static BOOL UseNormalSkill(em_Skill_Flag emSkillFlag, CMonster& Monster);

	// 获取技能的施法距离
	static float GetSkillDis(em_Skill_Flag emSkillFlag);

	// 学习技能
	static BOOL StudySkill(em_Skill_Flag emSkillFlag);

	// 遍历技能
	static UINT GetSkillList(vector<CSkill>& vSkillList);

	// 技能升级
	static BOOL LevelUpSkill();

	// 使用物品技能ByName
	static BOOL UseItemSkill(LPCWSTR pwszName);

	/// 使用普通技能ByName
	static BOOL UseNormalSkill_By_Name(LPCWSTR pwszName, CMonster& Monster, em_SkillType emSkillType);
private:
	DWORD dwSkillBase;
	WCHAR wszName[32];
	DWORD dwKeyIndex;
	float fSkillDis;
public:
	em_SkillType SkillType;
	static vector<CSkill> vNormalSkill;
};



#endif