#ifndef __GAMERES_H__
#define __GAMERES_H__

#include "GameSend.h"

class CGameRes
{
public:
	CGameRes();
	~CGameRes();

	static Point GetBlueBuffPoint(em_Camp emCamp);
	static Point GetRedBuffPoint(em_Camp emCamp);
	static Point GetDragonPoint();
	static Point GetSpringPoint(em_Camp emCamp);
	static LPCWSTR GetSpringName(em_Camp emCamp);
	static em_Skill_Flag FindSkillPoint_For_Hero(em_Hero_Pro emHero, DWORD dwLevel);
	static ResItem* GetEuqmentArray(em_Hero_Pro emHero,int& nMaxCount);
	static ResItem* GetNextEqument(em_Hero_Pro emHero, DWORD& dwIndex);
	static DWORD GetIndex_By_Equment(em_Hero_Pro emHero, vector<DWORD>& vlst);
	static TurretSrt* GetTurretArray();
	static TurretSrt* GetCrystalTurretArray(em_Camp emCamp, em_Path_Flag emPathFlag);
	static SkillConfig* FindSkillConfig(LPCWSTR pwszName);
	static ResPoint* GetResPointArray(em_Path_Flag emPathFlag, __out int& nRetMaxLen);
	static ResPoint* FindRecentlyResPoint(Point& NowPoint, em_Path_Flag emPathFlag, em_Camp emCamp, BOOL bNextPoint = FALSE);
	static ResPoint* GetResPoint_By_Index(DWORD dwIndex, em_Camp emCamp);
	static ResPoint* FindResPoint_For_Turret(em_Camp emCamp, em_Path_Flag emPathFlag, PointType PitType);
private:

};



#endif