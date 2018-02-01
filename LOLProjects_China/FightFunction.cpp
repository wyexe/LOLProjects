#include "stdafx.h"
#include "FightFunction.h"
#define _SELF L"FightFunction.cpp"
CFightFunction::CFightFunction()
{
	bSwitchPath = FALSE;
	bStopMoveToEnd = FALSE;
	dwKillDragonTick = NULL;
	dwKillBlueBuffTick = NULL;
	dwDragonCount = NULL;
	dwKillRedBuffTick = NULL;
}

CFightFunction::~CFightFunction()
{
}

BOOL CFightFunction::KillSolider_Skill(CMonster& Solider, BOOL bUseQ, BOOL bUseW, BOOL bUseE, BOOL bUseR)
{
	if (bUseQ && UseSkill_By_Dis(em_Skill_Flag_Q, Solider))
		return TRUE;

	if (bUseR && UseSkill_By_Dis(em_Skill_Flag_R, Solider))
		return TRUE;

	if (bUseW && UseSkill_By_Dis(em_Skill_Flag_W, Solider))
		return TRUE;

	if (bUseE && UseSkill_By_Dis(em_Skill_Flag_E, Solider))
		return TRUE;

	return Solider.NormalAttack();
}

BOOL CFightFunction::KillSolider(CMonster& Solider, BOOL bClearAround)
{
	// 按英雄分类
	UseSpecialSkill();
	switch (g_pAccountGame->MyAcount_Info.emHeroPro)
	{
	case em_Hero_Pro_Ryze: case em_Hero_Pro_Maokai:
		return KillSolider_Skill(Solider, TRUE, FALSE, TRUE, FALSE); // QE
	case em_Hero_Pro_Ashe:
		return KillSolider_Skill(Solider, FALSE, FALSE , FALSE, FALSE);
	case em_Hero_Pro_MasterYi: case em_Hero_Pro_Swain: case em_Hero_Pro_Graves:
	case em_Hero_Pro_Teemo: case em_Hero_Pro_MissFortune:
		return KillSolider_Skill(Solider, FALSE, FALSE, FALSE, FALSE); // Nop
	case em_Hero_Pro_Garen:  case em_Hero_Pro_Tristana: case em_Hero_Pro_Galio:
	case em_Hero_Pro_Aatrox: case em_Hero_Pro_Urgot:
		return KillSolider_Skill(Solider, FALSE, FALSE, TRUE, FALSE); // E
	case em_Hero_Pro_Malzahar:
		return KillSolider_Skill(Solider, FALSE, TRUE, TRUE, FALSE); // WE
	case em_Hero_Pro_Chogath:
		return KillSolider_Skill(Solider, Solider.GetPercentHp() > 50 ? FALSE : TRUE, FALSE, FALSE, TRUE);
	case em_Hero_Pro_Vayne:  case em_Hero_Pro_Lucian: case em_Hero_Pro_Trundle:
	case em_Hero_Pro_Ezreal: case em_Hero_Pro_Heimerdinger: case em_Hero_Pro_Nunu:
	case em_Hero_Pro_Syndra: case em_Hero_Pro_Rumble: case em_Hero_Pro_Diana:
	case em_Hero_Pro_Caitlyn: case em_Hero_Pro_Ahri:
		return KillSolider_Skill(Solider, TRUE, FALSE, FALSE, FALSE); // Q
	case em_Hero_Pro_Sejuani: case em_Hero_Pro_Taric:
		return KillSolider_Skill(Solider, FALSE, TRUE, FALSE, FALSE); // W
	case em_Hero_Pro_Veigar: case em_Hero_Pro_Annie:
		return KillSolider_Skill(Solider, Solider.GetPercentHp() <= 40 ? TRUE : FALSE, FALSE, FALSE, FALSE);
	default:
		return Solider.NormalAttack();
	}

	return FALSE;
}

BOOL CFightFunction::KillHero(CMonster& Hero, BOOL bConsume)
{
	// 按英雄分类
	UseSpecialSkill();
	switch (g_pAccountGame->MyAcount_Info.emHeroPro)
	{
	case em_Hero_Pro_Ryze:
		return KillHero_Ryze(Hero, bConsume);
	case em_Hero_Pro_Ashe:
		return KillHero_Ashe(Hero, bConsume);
	case em_Hero_Pro_MissFortune:
		return KillHero_MissFortune(Hero, bConsume);
	case em_Hero_Pro_MasterYi:
		return KillHero_MasterYi(Hero, bConsume);
	case em_Hero_Pro_Garen:
		return KillHero_Garen(Hero, bConsume);
	case em_Hero_Pro_Chogath:
		return KillHero_Chogath(Hero, bConsume);
	case em_Hero_Pro_Nunu:
		return KillHero_Nunu(Hero, bConsume);
	case em_Hero_Pro_Heimerdinger:
		return KillHero_Heimerdinger(Hero, bConsume);
	case em_Hero_Pro_Ezreal:
		return KillHero_Ezreal(Hero, bConsume);
	case em_Hero_Pro_Malzahar:
		return KillHero_Malzahar(Hero, bConsume);
	case em_Hero_Pro_Swain:
		return KillHero_Swain(Hero, bConsume);
	case em_Hero_Pro_Graves:
		return KillHero_Graves(Hero, bConsume);
	case em_Hero_Pro_Vayne:
		return KillHero_Vayne(Hero, bConsume);
	case em_Hero_Pro_Maokai:
		return KillHero_Maokai(Hero, bConsume);
	case em_Hero_Pro_Veigar:
		return KillHero_Veigar(Hero, bConsume);
	case em_Hero_Pro_Sejuani:
		return KillHero_Sejuani(Hero, bConsume);
	case em_Hero_Pro_Tristana:
		return KillHero_Tristana(Hero, bConsume);
	case em_Hero_Pro_Lucian:
		return KillHero_Lucian(Hero, bConsume);
	case em_Hero_Pro_Galio:
		return KillHero_Galio(Hero, bConsume);
	case em_Hero_Pro_Trundle:
		return KillHero_Tristana(Hero, bConsume);
	case em_Hero_Pro_Syndra:
		return KillHero_Syndra(Hero, bConsume);
	case em_Hero_Pro_Aatrox:
		return KillHero_Aatrox(Hero, bConsume);
	case em_Hero_Pro_Taric:
		return KillHero_Taric(Hero, bConsume);
	case em_Hero_Pro_Rumble:
		return KillHero_Rumble(Hero, bConsume);
	case em_Hero_Pro_Diana:
		return KillHero_Diana(Hero, bConsume);
	case em_Hero_Pro_Urgot:
		return KillHero_Urgot(Hero, bConsume);
	case em_Hero_Pro_Teemo:
		return KillHero_Teemo(Hero, bConsume);
	case em_Hero_Pro_Caitlyn:
		return KillHero_Caitlyn(Hero, bConsume);
	case em_Hero_Pro_Corki:
		return KillHero_Corki(Hero, bConsume);
	case em_Hero_Pro_Fiora:
		return KillHero_Fiora(Hero, bConsume);
	case em_Hero_Pro_Kennen:
		return KillHero_Kennen(Hero, bConsume);
	case em_Hero_Pro_Mordekaiser:
		return KillHero_Mordekaiser(Hero, bConsume);
	case em_Hero_Pro_Nautilus:
		return KillHero_Nautilus(Hero, bConsume);
	case em_Hero_Pro_RekSai:
		return KillHero_RekSai(Hero, bConsume);
	case em_Hero_Pro_Sivir:
		return KillHero_Sivir(Hero, bConsume);
	case em_Hero_Pro_Thresh:
		return KillHero_Thresh(Hero, bConsume);
	case em_Hero_Pro_TwistedFate:
		return KillHero_TwistedFate(Hero, bConsume);
	case em_Hero_Pro_KogMaw:
		return KillHero_KogMaw(Hero, bConsume);
	case em_Hero_Pro_Ahri:
		return KillHero_Ahri(Hero, bConsume);
	case em_Hero_Pro_Annie:
		return KillHero_Annie(Hero, bConsume);
	default:
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"暂时不支持这个英雄,em=%X", g_pAccountGame->MyAcount_Info.emHeroPro);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
		StopGame;
		return FALSE;
	}

	return FALSE;
}

BOOL CFightFunction::AttackSolider(BOOL bClearAround)
{
	CMonster Solider;
	if (!GetRecentlyEnemySolider(MaxDis_Attack, NULL, Solider))
		return TRUE;

	// 靠近小兵
	float fAttackDis = PERSON->GetAttackDis();
	if (PERSON->GetDis(Solider.GetPoint()) > fAttackDis)
		return CGameSend::Move(Solider.GetPoint());

	return HitSolider(Solider, bClearAround);
}

BOOL CFightFunction::HitSolider(CMonster& Solider, BOOL bClearAround)
{
	// 在贫血的情况下, 只用普通攻击, 就别浪费蓝用技能了
	if (Solider.IsAnemia())
		return Solider.NormalAttack();

	return KillSolider(Solider, bClearAround);
}

BOOL CFightFunction::AttackTurret()
{
	CMonster Turret;
	if (!GetRecentlyEnemyTurret(MaxDis_Attack, NULL, Turret))
		return TRUE;

	// 靠近塔
	float fAttackDis = PERSON->GetAttackDis();
	if (PERSON->GetDis(Turret.GetPoint()) > 7.0f)
		 CGameSend::Move(Turret.GetPoint());

	// 如果塔下攻击范围内有兵,先清兵.
	//if (GetAroundEnemySoliderCount(fAttackDis, NULL) > 0)
		//return AttackSolider(TRUE);

	return HitTurret(Turret);
}

BOOL CFightFunction::HitTurret(CMonster& Turret)
{
	return Turret.NormalAttack();
}

BOOL CFightFunction::DodgeTurret()
{
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"我要后退躲避塔的攻击");
	// 往后退
	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(MaxDis_Attack, NULL);
	BackOff();
	// 一直后退到塔的攻击范围20m开外, 想办法等兵线出塔,在塔外击杀英雄
	while (GetAroundEnemyTurretCount(MaxDis_Attack + 5.0f, NULL) > 0 && GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Sleep(500);
		BackOff();
	}

	// 停止动, 坐等塔下木有英雄
	if (uAroundEnemyHeroCount != 0)
	{
		//CPrintLog::PrintLog_W(_SELF,__LINE__,L"塔下有英雄, 禁止前进");
		//bStopMoveToEnd = TRUE;
	}

	if (CEqument::ExistItem(0xDB8))
		CSkill::UseNormalSkill_By_Name(L"ItemVoidGate", *PERSON, em_SkillType_UnDirectional);

	/*if (uAroundEnemyHeroCount != 0 && g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Ryze && bSwitchPath)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"后退躲塔的时候发现竟然有英雄在守塔, 不管了，去其他路Gank");
		SwitchPath();
		return TRUE;
	}*/
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"停止后退");
	// 停止后退
	return CGameSend::StopAction();
}

BOOL CFightFunction::HandleRegainMove()
{
	if (!bStopMoveToEnd)
		return FALSE;

	// 判断自己15m内是否有塔
	if (GetAroundEnemyTurretCount(20.0f, NULL) == 0)
	{
		//CPrintLog::PrintLog_W(_SELF,__LINE__,L"20m内都木有塔, 恢复前进");
		bStopMoveToEnd = FALSE;
		return TRUE;
	}

	// 获取最近的塔
	static CMonster NearestTurret;
	if (!GetRecentlyEnemyTurret(20.0f, NULL, NearestTurret))
	{
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"20m内都木有塔, 恢复前进");
		bStopMoveToEnd = FALSE;
		return TRUE;
	}

	// 获取15m内是否有英雄
	static CMonster NearMonster;
	if (!GetRecentlyEnemyHero(25.0f, NULL, NearMonster))
	{
		//CPrintLog::PrintLog_W(_SELF,__LINE__,L"25m内都木有英雄,恢复前进");
		bStopMoveToEnd = FALSE;
		return TRUE;
	}

	// 判断 最近的塔, 和最近的英雄的距离超过10m, 就恢复
	if (NearestTurret.GetDis(NearMonster.GetPoint()) > MaxDis_Attack)
	{
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"最近的敌方塔离最近的敌方英雄距离=%.2f,恢复前进", NearestTurret.GetDis(NearMonster.GetPoint()));
		bStopMoveToEnd = FALSE;
		return TRUE;
	}

	return TRUE;
}

BOOL CFightFunction::IsEvacuate()
{
	// 如果15m范围 对面的人数 > 己方人数, 就后退
	float fEvacuateDis = 10.0f;
	static CMonster NearestTurret;

	// 靠近塔3m之内就不用虚了! 毕竟塔的攻击范围就10m, 英雄的攻击距离一般都木有7m
	if (GetRecentlyAllianceTurret(5.0f, NULL, NearestTurret))
		return FALSE;

	if (PERSON->GetLevel() > 7)
		fEvacuateDis = 15.0f;

	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(fEvacuateDis, NULL);
	UINT uAroundAllianceHeroCount = GetAroundAllianceHeroCount(8.0f, NULL);

	/*if (g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Ryze)
	{
		if (uAroundEnemyHeroCount > uAroundAllianceHeroCount + 1)// 流浪只有4个人的时候才怂, 3个人随便压着打!
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"敌方有%d个人, 自己只有%d个人, 暂时后退一下,低调点~!", uAroundEnemyHeroCount, uAroundAllianceHeroCount);
			return TRUE;
		}
		return FALSE;
	}*/

	// 判断敌人是否在咱们塔下, 是的话就不退
	static CMonster SelfTurret;
	static CMonster NearMonster;
	if (GetRecentlyAllianceTurret(fEvacuateDis, NULL, SelfTurret) && GetRecentlyEnemyHero(fEvacuateDis, NULL, NearMonster))
	{
		// 敌人在咱们自己塔下, 怂毛?
		if (SelfTurret.GetDis(NearMonster.GetPoint()) <= 7.0f)
		{
			return FALSE;
		}
	}

	if (g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Ashe || g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Swain)
	{
		if (uAroundEnemyHeroCount <= uAroundAllianceHeroCount)
		{
			static CMonster EnemyHero;
			if (!GetRecentlyEnemyHero(fEvacuateDis, NULL, EnemyHero))
				return FALSE;

			// 如果敌方英雄在寒冰2m范围内, 就后退, 不要让他们近身!, 除非他们血量低于20%
			float fDis = PERSON->GetDis(EnemyHero.GetPoint());
			if (fDis <= 1.5f)
			{
				if (EnemyHero.GetPercentHp() <= 30)
					return FALSE;
				else
					return TRUE;
			}
			else if (fDis >= 3.0f)
				return FALSE;

			return TRUE;
		}
		return TRUE;
	}
	else
	{
		if (uAroundEnemyHeroCount > uAroundAllianceHeroCount)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"敌方有%d个人, 自己只有%d个人, 暂时后退一下,低调点~!", uAroundEnemyHeroCount, uAroundAllianceHeroCount);
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CFightFunction::IsEvacuate_Crystal()
{
	// 如果15m范围 对面的人数 > 己方人数, 就后退
	const float fEvacuateDis = 15.0f;
	static CMonster NearestTurret;

	// 靠近塔3m之内就不用虚了! 毕竟塔的攻击范围就7m, 英雄的攻击距离一般都木有7m
	if (GetRecentlyAllianceTurret(3.0f, NULL, NearestTurret))
		return FALSE;

	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(fEvacuateDis, NULL);
	UINT uAroundAllianceHeroCount = GetAroundAllianceHeroCount(8.0f, NULL);

	if (uAroundEnemyHeroCount > uAroundAllianceHeroCount)// 敌方人数占优势的情况下, 别刚正面
		return TRUE;

	return FALSE;
}

BOOL CFightFunction::IsEvacuate_Scuffle()
{
	// 如果15m范围 对面的人数 > 己方人数, 就后退
	const float fEvacuateDis = 15.0f;
	static CMonster NearestTurret;

	// 靠近塔3m之内就不用虚了! 毕竟塔的攻击范围就10m, 英雄的攻击距离一般都木有7m
	if (GetRecentlyAllianceTurret(3.0f, NULL, NearestTurret))
		return FALSE;

	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(fEvacuateDis, NULL);
	UINT uAroundAllianceHeroCount = GetAroundAllianceHeroCount(10.0f, NULL);

	if (uAroundEnemyHeroCount > uAroundAllianceHeroCount + 2)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"对面%d人, 自己这边%d个,后退!", uAroundEnemyHeroCount, uAroundAllianceHeroCount);
		return TRUE;
	}

	return FALSE;
}

BOOL CFightFunction::Evacuate()
{
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"Evacuate 5m!");
	Point NowPoint = PERSON->GetPoint();
	BackOff();

	while (GameStatus_Run && !EMPTY_PERSONHP && PERSON->GetDis(NowPoint) < 5.0f)
	{
		CGameSend::Sleep(WAITTIME);
		BackOff();
	}

	return TRUE;
	return CGameSend::StopAction();
}

BOOL CFightFunction::ConsumeEnemyHero()
{
	CMonster Hero;
	if (!GetRecentlyEnemyHero(MaxDis_Attack, NULL, Hero))
		return FALSE;

	KillHero(Hero, TRUE);
	if (Hero.GetTargetId() == PERSON->GetId())
		Evacuate();

	return TRUE;
}

BOOL CFightFunction::DodgeHero()
{
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"闪避英雄的攻击");
	// 往后退
	CGameSend::StopAction();
	BackOff();

	// 一直后退到英雄不攻击自己为止
	while (GetAttackSelfHeroCount(MaxDis_Attack, NULL) > 0 && GameStatus_Run && !EMPTY_PERSONHP)
	{
		BackOff();
		CGameSend::Sleep(WAITTIME);
	}
	// 停止后退
	return CGameSend::StopAction();
}

BOOL CFightFunction::IsDodgeTurret()
{
	// 获取敌对阵营的塔
	static vector<CMonster> vMonsterList;
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Turret, PERSON->GetEnemyCamp(), MaxDis_Attack + 2.0f, NULL);

	if (vMonsterList.size() == 0)
		return FALSE;

	// 水晶塔不躲
	if (vMonsterList.at(0).IsCrystalTurret())
		return FALSE;

	// 是否进泉水了,15m
	/*CMonster SpringTurr;
	if (FindMonster_For_Name(CGameRes::GetSpringName(PERSON->GetEnemyCamp()), 15.0f, SpringTurr))
		return TRUE;*/

	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(MaxDis_Attack, NULL);
	UINT uAroundAlliance = GetAroundAllianceHeroCount(MaxDis_Attack, NULL);

	// 看看炮塔的目标是不是自己
	for (CMonster& Turret : vMonsterList)
	{
		// 被炮塔攻击, 速速闪人
		if (Turret.GetTargetId() == PERSON->GetId())
			return TRUE;

		if (uAroundEnemyHeroCount == 0)
			continue;

		// 塔下被英雄攻击了, 闪!
		if (IsDodgeHero())
			return TRUE;

		if (uAroundEnemyHeroCount == 1)
		{
			// 找一下离我们最近的英雄
			CMonster Hero;
			if (!GetRecentlyEnemyHero(MaxDis_Attack, NULL, Hero))
				continue;

			// 如果最近的英雄离我们比较近, 那就不要去攻击塔了,走远一点吧
			if (Hero.GetDis(Turret.GetPoint()) < 7.0f)
			{
				return TRUE;
			}
		}
		else if (uAroundAlliance > uAroundEnemyHeroCount)
		{
			// 人数比他们英雄多的情况下, 不虚。 直接干塔
			continue;
		}
		else if (uAroundEnemyHeroCount > 1)
		{
			// 如果有2个人守塔, 拉到吧。 等待机会!
			return TRUE;
		}
	}

	// 10m之内少于2个自己的士兵,就先撤退
	if (GetAroundAllianceSoliderCount(PERSON->GetAttackDis() >= 7.0f ? MaxDis_Attack : MaxDis_Attack + 2.0f, &vMonsterList.at(0).GetPoint()) <= 2)
		return TRUE;

	return FALSE;
}

BOOL CFightFunction::IsDodgeCrystalTurret()
{
	static vector<CMonster> vGuardLst;
	// 获取敌对阵营的塔
	GetMonsterList_By_Type(vGuardLst, em_MonsterType_Soldier, PERSON->GetEnemyCamp(), 15.0f, NULL);
	if (vGuardLst.size() == 0)
		return FALSE;

	// 获取敌对阵营的英雄
	static vector<CMonster> vEnemyLst;
	GetMonsterList_By_Type(vEnemyLst, em_MonsterType_Hero, PERSON->GetEnemyCamp(), 15.0f, NULL);
	if (vEnemyLst.size() == 0)
		return FALSE;

	// 判断敌方英雄是否缩在塔下, 如果缩在塔下, 那么必须至少是敌方英雄人数+1才强攻!
	UINT uCount = 0;
	for (auto& Guard : vGuardLst)
	{
		for (auto& EnemyHero : vEnemyLst)
		{
			if (EnemyHero.GetDis(Guard.GetPoint()) <= 7.0f)
				uCount++;
		}
	}

	UINT uAllianceHeroCount = GetAroundAllianceHeroCount(10.0f, NULL);
	if (uAllianceHeroCount == 5)
		return FALSE;

	return uAllianceHeroCount >= uCount + 1;
}

BOOL CFightFunction::DodgeCrystalTurret()
{
	while (GameStatus_Run && !EMPTY_PERSONHP && IsDodgeCrystalTurret())
	{
		BackOff();
		Sleep(WAITTIME);
	}
	return TRUE;
}

BOOL CFightFunction::AttackHero()
{
	CMonster Hero;
	if (!GetRecentlyEnemyHero(g_fMaxDis_Attack, NULL, Hero))
		return TRUE;

	// 靠近小兵
	float fAttackDis = PERSON->GetAttackDis();
	if (PERSON->GetDis(Hero.GetPoint()) > fAttackDis)
		return CGameSend::Move(Hero.GetPoint());

	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"直接攻击英雄,HP=%d", Hero.GetPercentHp());
	return KillHero(Hero, FALSE);
}

BOOL CFightFunction::UseSpecialSkill()
{
	/*auto fn_UseSkill_By_Condition = [this](bool bConditionHp,em_Skill_Flag emSkillFlag)
	{
		static vector<CMonster> vMonsterList;
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetCamp(), 5.0f, NULL);
		for (auto& tmpHero : vMonsterList)
		{
			if (bConditionHp && tmpHero.GetPercentHp() <= 30 )
			{
				CSkill::UseNormalSkill(emSkillFlag, *PERSON);
				//CSkill::UseNormalSkill_By_Name(L"summonerheal", *PERSON, em_SkillType_Buff_Self);
				break;
			}
			else if (!bConditionHp && tmpHero.GetPercentMp() <= 50 )
			{
				CSkill::UseNormalSkill(emSkillFlag, *PERSON);
				//CSkill::UseNormalSkill_By_Name(L"summonermana", *PERSON, em_SkillType_Buff_Self);
				break;
			}
		}
	};*/

	/*if (CSkill::GetSkillCool(em_Skill_Flag_1)) // 180s CD  清晰术, 妈蛋TX取消了!
	{
		fn_UseSkill_By_Condition(false, em_Skill_Flag_1);
	}*/

	static DWORD dwHealTick = 0;
	if (::GetTickCount() - dwHealTick >= 240 * 1000)
	{
		if (PERSON->GetPercentHp() < 50)
		{
			CSkill::UseItemSkill(L"summonerheal");
			dwHealTick = ::GetTickCount();
			return TRUE;
		}
		static vector<CMonster> vMonsterList;
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetCamp(), 5.0f, NULL);
		for (auto& tmpHero : vMonsterList)
		{
			if (tmpHero.GetPercentHp() <= 50)
			{
				CSkill::UseItemSkill(L"summonerheal");
				break;
			}
		}
		
		dwHealTick = ::GetTickCount();
	}
	return TRUE;
}

BOOL CFightFunction::IsKillDragon()
{
	if (g_MapType != MapType_召唤师峡谷)
		return FALSE;

	static DWORD dwWaitTick = 0;

	// 当流浪法师去打过龙了, 往后都不去打龙了, 压住中路
	if (g_pAccountGame->MyAcount_Info.emHeroPro == em_Hero_Pro_Ryze)
		return FALSE;

	if (dwKillDragonTick != 0)
	{
		if (::GetTickCount() - dwKillDragonTick < 6 * 60 * 1000)
			return FALSE;

		CPrintLog::PrintLog_W(_SELF,__LINE__,L"6分钟的刷龙时间到了~!");
		return TRUE;
	}

	// 3s 遍历一次, 别太频繁
	if (::GetTickCount() - dwWaitTick >= 3 * 1000)
	{
		dwWaitTick = ::GetTickCount();
		// 查找队友的等级, 至少要有4个5级
		if (GetAroundAllianceHeroCount_By_Level(2000.0f, 5) >= 4)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"周围有足够5级的队友, 准备去杀小龙!");
			return TRUE;
		}
	}


	return FALSE;
}

BOOL CFightFunction::KillDragon()
{
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"准备杀龙,开始回城");
	BackHome();

	static CMonster Drogan;
	Point DragonPoint = CGameRes::GetDragonPoint();
	Point tmpPoint = DragonPoint;
	tmpPoint.X += 300.0f;
	tmpPoint.Y += 300.0f;
	static DWORD dwTickCount = 0;
	DWORD dwWaitTick = ::GetTickCount();

	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"往小龙的方向走……");
	

	// 等待队友集合, 如果有敌方英雄出来, 顺便把他给干了
	while (GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Move(DragonPoint);
		CGameSend::Sleep(500);

		// 判断附近是否有敌方英雄
		if (GetAroundEnemyHeroCount(MaxDis_Attack, NULL) > 0)
		{
			//CPrintLog::PrintLog_W(_SELF,__LINE__,L"攻击敌方英雄!");
			AttackHero();
			continue;
		}

		if (::GetTickCount() - dwWaitTick >= 1 * 60 * 1000)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"1分钟内都没人齐, 直接开打了!");
			break;
		}

		if (PERSON->GetDis(DragonPoint) < 2.0f)
		{
			// 5m范围内必须要有4个或以上的队友
			UINT uAroundAllianceHeroCoun = GetAroundAllianceHeroCount(MaxDis_Attack, NULL);
			if (dwKillDragonTick != NULL && uAroundAllianceHeroCoun >= 3)
				break;
			else if (uAroundAllianceHeroCoun >= 4)
				break;

			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"队友数量=%d", GetAroundAllianceHeroCount(MaxDis_Attack, NULL));
			if (!FindMonster_For_Name(DroganName, 10.0f, Drogan))
				goto lb_Exit;

			if (Drogan.GetPercentHp() != 100)
				break;
		}

		CGameSend::Move(tmpPoint);
	}
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"集合完毕!,准备开杀");

	// 中断处理
	if (!GameStatus_Run || EMPTY_PERSONHP)
		return FALSE;

	// 开始杀龙
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"开始搜索小龙的位置");
	if (!FindMonster_For_Name(DroganName, 10.0f, Drogan))
		goto lb_Exit;

	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"开打!");
	while (Drogan.GetHp() != NULL  && GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Sleep(100);
		KillHero(Drogan, FALSE);
	}

lb_Exit:;
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"打完小龙了!");
	BackHome();
	dwKillDragonTick = ::GetTickCount();
	SwitchPath();
	return TRUE;
}

BOOL CFightFunction::IsKillBlueBuff()
{
	if (g_MapType != MapType_召唤师峡谷)
		return FALSE;

	// 本来是6m刷新一次的,

	// 只有流浪法师 并且>=6 级才去刷蓝BUFF
	if (g_pAccountGame->MyAcount_Info.emHeroPro != em_Hero_Pro_Ryze)
		return FALSE;

	if (PERSON->GetLevel() < 6)
		return FALSE;

	// 第一次6级去刷, 时间肯定超过刷新蓝BUFF的时间了
	if ( dwKillBlueBuffTick == 0)
		return TRUE;

	if (::GetTickCount() - dwKillBlueBuffTick >= 6 * 60 * 1000)
		return TRUE;

	return FALSE;
}

BOOL CFightFunction::KillBlueBuff()
{
	// 获取蓝BUFF的坐标
	Point BluePoint = CGameRes::GetBlueBuffPoint(PERSON->GetCamp());

	// 等待队友集合, 如果有敌方英雄出来, 顺便把他给干了
	while (GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Move(BluePoint);
		CGameSend::Sleep(500);

		// 判断附近是否有敌方英雄
		while (GetAroundEnemyHeroCount(MaxDis_Attack, NULL) > 0 && GameStatus_Run && !EMPTY_PERSONHP)
		{
			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"攻击敌方英雄!");
			AttackHero();
			continue;
		}

		if (PERSON->GetDis(BluePoint) < 2.0f)
			break;
	}
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"集合完毕!,准备开杀");

	// 中断处理
	if (!GameStatus_Run || EMPTY_PERSONHP)
		return FALSE;

	// 开始杀龙
	static CMonster BlueBuff;
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"开始搜索蓝BUFF的位置");
	if (!FindMonster_For_Name(BlueBuffName_Top, 10.0f, BlueBuff) && !FindMonster_For_Name(BlueBuffName2_Buttom, 10.0f, BlueBuff))
		goto lb_Exit;

	MoveToPoint(BlueBuff.GetPoint(), 3.0f);
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"开打!");
	while (BlueBuff.GetHp() != NULL  && GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Sleep(100);
		KillHero(BlueBuff, FALSE);
	}

	// 清理附近的小野怪
	KillAroundSRU(5.0f);

lb_Exit:;
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"打完蓝BUFF了!");
	//BackHome();
	dwKillBlueBuffTick = ::GetTickCount();
	return TRUE;
}

BOOL CFightFunction::IsKillRedBuff()
{
	if (g_MapType != MapType_召唤师峡谷)
		return FALSE;

	// 判断身上有木有蓝BUFF
	if (CPersonBuff::ExistBuff(0x16))
		return FALSE;

	if (g_pAccountGame->MyAcount_Info.emHeroPro != em_Hero_Pro_Ryze)
		return FALSE;

	if (PERSON->GetLevel() < 6)
		return FALSE;

	// 第一次6级去刷, 时间肯定超过刷新蓝BUFF的时间了
	if (dwKillRedBuffTick == 0)
		return TRUE;

	if (::GetTickCount() - dwKillRedBuffTick >= 6 * 60 * 1000)
		return TRUE;

	return FALSE;
}

BOOL CFightFunction::KillRedBuff()
{
	// 获取蓝BUFF的坐标
	Point RedPoint = CGameRes::GetRedBuffPoint(PERSON->GetCamp());

	// 等待队友集合, 如果有敌方英雄出来, 顺便把他给干了
	while (GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Move(RedPoint);
		CGameSend::Sleep(500);

		// 判断附近是否有敌方英雄
		while (GetAroundEnemyHeroCount(MaxDis_Attack, NULL) > 0 && GameStatus_Run && !EMPTY_PERSONHP)
		{
			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"攻击敌方英雄!");
			AttackHero();
			continue;
		}

		if (PERSON->GetDis(RedPoint) < 2.0f)
			break;
	}
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"集合完毕!,准备开杀");

	// 中断处理
	if (!GameStatus_Run || EMPTY_PERSONHP)
		return FALSE;

	// 开始杀龙
	static CMonster RedBuff;
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"开始搜索红BUFF的位置");
	if (!FindMonster_For_Name(RedBuffName_Top, 10.0f, RedBuff) && !FindMonster_For_Name(RedBuffName2_Buttom, 10.0f, RedBuff))
		goto lb_Exit;

	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"开打!");
	MoveToPoint(RedBuff.GetPoint(), 3.0f);
	while (RedBuff.GetHp() != NULL && GameStatus_Run && !EMPTY_PERSONHP)
	{
		CGameSend::Sleep(100);
		KillHero(RedBuff, FALSE);
	}

	// 清理附近的小野怪
	KillAroundSRU(5.0f);
lb_Exit:;
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"打完红BUFF了!");
	//BackHome();
	dwKillRedBuffTick = ::GetTickCount();
	return TRUE;
}

BOOL CFightFunction::KillAroundSRU(float fDis)
{
	while (GameStatus_Run && !EMPTY_PERSONHP)
	{
		static vector<CMonster> vMonsterList;
		// 获取敌对阵营的野怪
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Soldier, em_Camp_Neutral, fDis, NULL);
		if (vMonsterList.size() == 0)
			break;

		for (auto& SRU : vMonsterList)
		{
			//CPrintLog::PrintLog_W(_SELF,__LINE__,L"清理附近的野怪!");
			while (SRU.GetHp() != NULL && GameStatus_Run && !EMPTY_PERSONHP)
			{
				CGameSend::Sleep(100);
				KillHero(SRU, FALSE);
			}
			break;
		}
	}
	return TRUE;
}

////////////分次处理逻辑//////////////////////////////////////////////////////////////
BOOL CFightFunction::HandleTurret()
{
	if (g_MapType == MapType_召唤师峡谷 || g_MapType == MapType_嚎哭深渊)
	{
		// 是否需要躲避塔的范围 || 攻击
		if (IsDodgeTurret())
			return DodgeTurret();

		// 是不是开始攻击塔
		if (IsAttackTurret())
			return AttackTurret();

		// 是否可以恢复前进
		//HandleRegainMove();
	}
	else if (g_MapType == MapType_水晶之痕)
	{
		//if (IsDodgeCrystalTurret())
			//return BackOff();

		return OccupyEnemyTurret();
	}
	
	return FALSE;
}

BOOL CFightFunction::HandleHero()
{
	// 是否需要后退
	if (g_MapType == MapType_召唤师峡谷)
	{
		if (IsEvacuate())
			return Evacuate();
	}
	else if (g_MapType == MapType_水晶之痕)
	{
		//if (IsEvacuate_Crystal())
			//return Evacuate();
	}
	else if (g_MapType == MapType_嚎哭深渊)
	{
		if (IsEvacuate_Scuffle())
			return Evacuate();
	}


	// 是否需要躲避英雄的攻击
	if (IsDodgeHero())
	{
		if (/*g_pAccountGame->MyAcount_Info.emHeroPro != em_Hero_Pro_Ryze && */g_MapType == MapType_召唤师峡谷 && PERSON->GetLevel() < 6)
			return DodgeHero();

		return AttackHero();
	}


	// 是否需要用技能消耗英雄的血量
	if (IsConsumeEnemyHero())
	{
		return ConsumeEnemyHero();
	}

	

	// 是不是开始攻击英雄
	if (IsAttackHero())
		return AttackHero();

	return FALSE;
}

BOOL CFightFunction::HandleSolider()
{
	if (g_MapType == MapType_水晶之痕)
		return FALSE;

	// 是否需要躲避小兵的攻击
	if (IsDodgeSolider())
		return DodgeEnemySolider();

	// 是不是开始清兵
	if (IsClearSolider())
		return AttackSolider(FALSE);

	return FALSE;
}

BOOL CFightFunction::SwitchPath()
{
	dwDragonCount = CPersonBuff::GetBuffCount(0x1C);// S5Tooltip_DragonSlayerBuffv1
	if (dwDragonCount == 3 || !ExistMiddleCrystalTurret())
	{
		// 分出2人去中路帮忙
		switch (g_pAccountGame->MyAcount_Info.emHeroPro)
		{
		 case em_Hero_Pro_Ashe: 
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Top;
			break;
		case em_Hero_Pro_Garen:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Buttom;
			break;
		default:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Middle;
			break;
		}
	}
	else if (dwDragonCount >= 5)
	{
		// 设置英雄的分路的路线
		switch (g_pAccountGame->MyAcount_Info.emHeroPro)
		{
		case em_Hero_Pro_Ryze: // 流浪法师中单
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Middle;
			break;
		case em_Hero_Pro_Swain: case em_Hero_Pro_Ashe: case em_Hero_Pro_Ezreal:  case em_Hero_Pro_Chogath:  case  em_Hero_Pro_Maokai:
		case em_Hero_Pro_MissFortune: case em_Hero_Pro_Sejuani: case em_Hero_Pro_Galio: case em_Hero_Pro_KogMaw: case em_Hero_Pro_Ahri:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Top;
			break;
		case em_Hero_Pro_Malzahar: case em_Hero_Pro_MasterYi: case em_Hero_Pro_Graves: case em_Hero_Pro_Vayne:
		case em_Hero_Pro_Heimerdinger: case em_Hero_Pro_Nunu: case em_Hero_Pro_Garen: case em_Hero_Pro_Veigar:
		case em_Hero_Pro_Tristana: case em_Hero_Pro_Lucian: case em_Hero_Pro_Corki: case em_Hero_Pro_Caitlyn:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Buttom;
			break;
		default:
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"少年,这个英雄:%s暂时还不支持喔~!", PERSON->GetHeroName());
			break;
		}
	}
	
	

	return TRUE;
	//////////////////////////////////////////////////////////////////////////
	if (PERSON->GetLevel() < 6)
		return FALSE;

	static bool bInit = false;
	static em_Path_Flag s_emPathFlag = em_Path_Flag_Top;
	
	if (!bInit)
	{
		s_emPathFlag = g_pAccountGame->MyAcount_Info.emPathFlag;
		bInit = true;
		return TRUE;
	}

	em_Camp emEnemyCamp = PERSON->GetEnemyCamp();
	auto pLeftCrystal = CGameRes::GetCrystalTurretArray(emEnemyCamp, em_Path_Flag_Top);
	auto pCenterCrystal = CGameRes::GetCrystalTurretArray(emEnemyCamp, em_Path_Flag_Middle);
	auto pRightCrystal = CGameRes::GetCrystalTurretArray(emEnemyCamp, em_Path_Flag_Buttom);
	if (pLeftCrystal == NULL || pCenterCrystal == NULL || pRightCrystal == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"不可能啊!!!!!!! 怎么会找不到水晶塔资源呢?");
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
		StopGame;
		return FALSE;
	}

	CMonster LeftCrystalTurret; // 上路水晶塔
	CMonster CenterCrystalTurret; // 中路水晶塔
	CMonster RightCrystalTurret; // 下路水晶塔

	// 如果三路都不在了. 去中路
	FindMonster_For_Name(pLeftCrystal->wszTurretName, 1000.0f, LeftCrystalTurret);
	FindMonster_For_Name(pCenterCrystal->wszTurretName, 1000.0f, CenterCrystalTurret);
	FindMonster_For_Name(pRightCrystal->wszTurretName, 1000.0f, RightCrystalTurret);

	// 如果中路水晶塔不在了, 就去干上路, 中路也木有就去下路
	if (CenterCrystalTurret.GetBase() == NULL)
	{
		if (LeftCrystalTurret.GetBase() != NULL)
		{
			s_emPathFlag = em_Path_Flag_Top;
		}
		else if (RightCrystalTurret.GetBase() != NULL)
		{
			s_emPathFlag = em_Path_Flag_Buttom;
		}
		else
		{
			s_emPathFlag = em_Path_Flag_Middle;
		}
	}
	else
	{
		s_emPathFlag = em_Path_Flag_Middle;
	}

	if (s_emPathFlag != g_pAccountGame->MyAcount_Info.emPathFlag)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"路径已经改变了!");
		BackHome();
		g_pAccountGame->MyAcount_Info.emPathFlag = s_emPathFlag;
	}

	return TRUE;
}

BOOL CFightFunction::HandleBackHome()
{
	// 极地大乱斗的情况下回城也不回血, 干脆别回了
	if (g_MapType == MapType_嚎哭深渊)
		return FALSE;

	if (IsBackHome())
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"HandleBackHome");
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"需要回城!");
		// 跑到塔下
		//MoveToTurret(0, g_pAccountGame->MyAcount_Info.emPathFlag);

		// 跑到塔下才发现血量回过来了不用回城了!
		//if (PERSON->GetLevel() < 10 && !IsBackHome())
			//return FALSE;

		BackHome();
		if (IsKillBlueBuff() && ExistMiddleCrystalTurret())
			return KillBlueBuff();
		else if (IsKillRedBuff() && ExistMiddleCrystalTurret())
			return KillRedBuff();
		return TRUE;
	}
	return FALSE;
}

BOOL CFightFunction::PartyStart()
{
	auto fn_WaitToLoad = []
	{
		while (PERSON->GetBase() == NULL && GameStatus_Run)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"过图中……");
			CGameSend::Sleep(5 * 1000);
		}

		while (GameStatus_Run && PERSON->GetMoney() == 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"木有钱!!……");
			CGameSend::Sleep(1 * 1000);
		}
	};

	auto fn_Test = []
	{
		Point NowPoint = PERSON->GetPoint();
		Point NewPoint = NowPoint;
		NewPoint.X += 300.0f;
		NewPoint.Y += 300.0f;

		while (GameStatus_Run)
		{
			CGameSend::Move(NewPoint);
			Sleep(5000);
			CGameSend::Move(NowPoint);
			Sleep(5000);
		}
	};

	g_dwMainThreadId = CLThread::GetMainThreadId();
	fn_WaitToLoad();
	::CreateMutexW(NULL, FALSE, Mutex_GameALive_Name);
	SetGamePath();
	InitAccountGame();
	Sleep(5 * 1000);

	//fn_Test();

	CSkill::LevelUpSkill();
	CEqument::SetEqumentIndex(g_pAccountGame->MyAcount_Info.emHeroPro);
	
	CEqument::BuyEqument();
	CSkill::InitNormalSkill();
	WaitToStartGame();
	CGameSend::LockView();
	SetMatchingPath();
	SetGamePath();
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"PartyStart");
	while (GameStatus_Run)
	{
		// 人物死亡处理
		//CGameSend::Sleep(50);
		if (!ExistMiddleCrystalTurret())
			SwitchPath();

		if (EMPTY_PERSONHP)
		{
			if (PERSON->GetLevel() >= 13 && CEqument::ExistItem(0xD11))
			{
				CPrintLog::PrintLog_W(_SELF,__LINE__,L"马上复活!");
				CSkill::UseItemSkill(L"OdinTrinketRevive");
			}
			while (GameStatus_Run && EMPTY_PERSONHP)
			{
				CGameSend::Sleep(1000);
			}

			CPrintLog::PrintLog_W(_SELF, __LINE__, L"人物复活了!");
			CEqument::BuyEqument();

			// 杀BUFF
			if (IsKillBlueBuff() && ExistMiddleCrystalTurret())
				KillBlueBuff();
			else if (IsKillRedBuff() && ExistMiddleCrystalTurret())
				KillRedBuff();

		}

		// 杀龙
		if (FALSE && ExistMiddleCrystalTurret() && IsKillDragon())
		{
			KillDragon();
			continue;
		}

		// 回城处理
		if (HandleBackHome())
			continue;

		// 塔的处理
		if (HandleTurret())
			continue;

		// 英雄的处理
		if (HandleHero())
			continue;

		// 小兵的处理
		if (HandleSolider())
			continue;

		// 走路的处理
		//if (!bStopMoveToEnd)
		MoveToEndPoint();

	}

	CPrintLog::PrintLog_W(_SELF,__LINE__,L"结束!");
	g_pAccountGame->MyAcount_Info.bClose = TRUE;

	return TRUE;
}

BOOL CFightFunction::TestAAA()
{
	g_dwMainThreadId = CLThread::GetMainThreadId();
	InitAccountGame();

	static vector<CMonster> vMonsterList;
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Soldier, em_Camp_Neutral, 20.0f, NULL);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"vMonsterList.size()=%d", vMonsterList.size());
	if (vMonsterList.size() == 0)
		return FALSE;

	CMonster Solider = vMonsterList.at(0);
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"Name:%s,dis=%.2f", Solider.GetName(), Solider.GetDis(PERSON->GetPoint()));
	Solider.NormalAttack();
	Sleep(1000);
	g_dwStartGameCmd = GameCmd_Stop;
	return TRUE;
}