#include "stdafx.h"
#include "FightFunction.h"
#define _SELF L"FightFunction_Hero.cpp"

BOOL CFightFunction::KillHero_Ryze(CMonster& Hero, BOOL bConsume)
{
	BOOL bRetCode = TRUE;
	while (bRetCode && GameStatus_Run && !EMPTY_PERSONHP)
	{
		//if (PERSON->GetDis(Hero.GetPoint()) > 7.0f)
			//break;

		// 有了天赋技能才用R
		if (!UseSkill_By_Dis(em_Skill_Flag_Q, Hero))
			bRetCode = FALSE;

		if (!UseSkill_By_Dis(em_Skill_Flag_E, Hero))
			bRetCode = FALSE;

		if (!UseSkill_By_Dis(em_Skill_Flag_W, Hero))
			bRetCode = FALSE;

		if (!UseSkill_By_Dis(em_Skill_Flag_R, Hero))
			bRetCode = FALSE;

		break;
	}
	
	DWORD dwLevel = PERSON->GetLevel();
	if (dwLevel >= 6)
	{
		Sleep(100);
		return TRUE;
		// 必须所有技能都木有冷却才A
		if (!CSkill::GetSkillCool(em_Skill_Flag_Q) && !CSkill::GetSkillCool(em_Skill_Flag_W) && !CSkill::GetSkillCool(em_Skill_Flag_E) && !CSkill::GetSkillCool(em_Skill_Flag_R))
		{
			return Hero.NormalAttack();
		}
	}

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Ashe(CMonster& Hero, BOOL bConsume)
{
	// 消耗模式下不用R

	if (PERSON->GetDis(Hero.GetPoint()) <= 7.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	// 打小龙的时候 不要用R
	if (Hero.GetPercentHp() < 50 && Hero.GetMonsterType() == em_MonsterType_Hero)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	if (CPersonBuff::ExistBuff(0xE))
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_MasterYi(CMonster& Hero, BOOL bConsume)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (!bConsume)
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (PERSON->GetPercentHp() <= 50 && CSkill::GetSkillCool(em_Skill_Flag_W))
	{
		DodgeHero();
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"+血");
		CSkill::UseNormalSkill(em_Skill_Flag_W, *PERSON);
		CGameSend::Sleep(4 * 1000);
		return TRUE;
	}

	if (Hero.GetPercentHp() < 50 && Hero.GetMonsterType() == em_MonsterType_Hero)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	if (bConsume)
		return TRUE;

	Hero.NormalAttack();
	CGameSend::Sleep(WAITTIME);
	return TRUE;
}

BOOL CFightFunction::KillHero_MissFortune(CMonster& Hero, BOOL bConsume)
{

	// 消耗模式开启下用E
	UseSkill_By_Dis(em_Skill_Flag_E, Hero);
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (!bConsume && !CPersonBuff::ExistBuff(0x19))
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (Hero.GetPercentHp() <= 40 && Hero.GetMonsterType() == em_MonsterType_Hero && UseSkill_By_Dis(em_Skill_Flag_R, Hero))
		CGameSend::Sleep(2 * 1000);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Garen(CMonster& Hero, BOOL bConsume)
{
	if (Hero.GetPercentHp() <= 25 && Hero.GetMonsterType() == em_MonsterType_Hero)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	static DWORD dwTickQ = 0;
	static DWORD dwTickE = 0;

	// Q的冷却是9s, 内置冷却判断
	if (::GetTickCount() - dwTickE > 9 * 1000 && UseSkill_By_Dis(em_Skill_Flag_E, Hero))
		dwTickE = ::GetTickCount();

	// 使用Q的时候, 至少要过了4s E的时效, 才能用Q
	if (::GetTickCount() - dwTickQ > 8 * 1000 && ::GetTickCount() - dwTickE > 4 * 1000 && UseSkill_By_Dis(em_Skill_Flag_Q, Hero))
		dwTickQ = ::GetTickCount();

	// 随时加盾, 反正又不用蓝
	UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	Hero.NormalAttack();
	Sleep(WAITTIME); // 没办法, 德玛西亚的起手比较长
	return TRUE;
}

BOOL CFightFunction::KillHero_Ezreal(CMonster& Hero, BOOL bConsume)
{
	if (!bConsume && Hero.GetPercentHp() < 15)
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	// Q随便用
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);
	// W随便用
	UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (Hero.GetPercentHp() < 50 && Hero.GetMonsterType() == em_MonsterType_Hero)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	if (bConsume)
		return TRUE;

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Malzahar(CMonster& Hero, BOOL bConsume)
{
	if (!bConsume)
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (Hero.GetPercentHp() < 50 && Hero.GetMonsterType() == em_MonsterType_Hero && UseSkill_By_Dis(em_Skill_Flag_R, Hero))
		CGameSend::Sleep(2500);

	if (!bConsume)
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (!bConsume)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (bConsume)
		return TRUE;

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Chogath(CMonster& Hero, BOOL bConsume)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (Hero.GetMonsterType() != em_MonsterType_Hero || (Hero.GetMonsterType() == em_MonsterType_Hero && Hero.GetPercentHp() < 30))
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);
	
	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Nunu(CMonster& Hero, BOOL bConsume)
{
	if (!bConsume && PERSON->GetDis(Hero.GetPoint()) <= PERSON->GetAttackDis() && Hero.GetPercentHp() < 40)
		UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetMonsterType() == em_MonsterType_Hero && Hero.GetPercentHp() < 60 && PERSON->GetDis(Hero.GetPoint()) <= PERSON->GetAttackDis() && UseSkill_By_Dis(em_Skill_Flag_R, Hero))
		CGameSend::Sleep(3 * 1000 + 500);

	return Hero.NormalAttack();

}

BOOL CFightFunction::KillHero_Heimerdinger(CMonster& Hero, BOOL bConsume)
{
	static DWORD dwTickQ = 0;
	if (::GetTickCount() - dwTickQ >= 25 * 1000 && UseSkill_By_Dis(em_Skill_Flag_Q, Hero))
	{
		dwTickQ = ::GetTickCount();
	}

	UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	static DWORD dwTickR = 0;
	if (!bConsume && ::GetTickCount() - dwTickR >= 15 * 1000 && Hero.GetMonsterType() == em_MonsterType_Hero && Hero.GetPercentHp() < 50 && UseSkill_By_Dis(em_Skill_Flag_R, Hero))
	{
		dwTickR = ::GetTickCount();
	}

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Graves(CMonster& Hero, BOOL bConsume)
{
	if (Hero.GetPercentHp() < 40)
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) < 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) < 6.0f)
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Swain(CMonster& Hero, BOOL bConsume)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);
	UseSkill_By_Dis(em_Skill_Flag_E, Hero);
	UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	// 判断敌人的距离, 超过50% or 超过5m就关闭
	static DWORD dwTickR = 0;
	static BOOL bUseR = FALSE;
	if (::GetTickCount() - dwTickR >= 10 * 1000 && Hero.GetMonsterType() == em_MonsterType_Hero &&  (Hero.GetPercentHp() < 50 || GetAroundEnemyHeroCount(7.0f, NULL) > 1) && PERSON->GetDis(Hero.GetPoint()) < 5.0f)
	{
		bUseR = TRUE;
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);
		dwTickR = ::GetTickCount();
	}
	if (bUseR && ::GetTickCount() - dwTickR >= 10 * 1000 && ::GetTickCount() - dwTickR <= 20 * 1000)
	{
		bUseR = FALSE;
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);
		dwTickR = ::GetTickCount();
	}


	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Vayne(CMonster& Hero, BOOL bConsume)
{	
	if (!CPersonBuff::ExistBuff(0xB))// VayneTumble
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Maokai(CMonster& Hero, BOOL bConsum)
{

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	static DWORD dwTickW = 0;
	if (UseSkill_By_Dis(em_Skill_Flag_W, Hero))
		dwTickW = ::GetTickCount();

	if (::GetTickCount() - dwTickW >= 1500)// 用了W后,有束缚状态,用Q没用
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (Hero.GetTargetId() == PERSON->GetId())
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Veigar(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) < 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) < 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() < 40 && Hero.GetMonsterType() == em_MonsterType_Hero)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Sejuani(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	static DWORD dwTickW = 0;
	if (::GetTickCount() - dwTickW >= 6 * 1000 && UseSkill_By_Dis(em_Skill_Flag_W, Hero))
		dwTickW = ::GetTickCount();

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() < 40 && Hero.GetMonsterType() == em_MonsterType_Hero)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Tristana(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) <= 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (!bConsum && Hero.GetPercentHp() < 30)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (Hero.GetPercentHp() < 60 && Hero.GetMonsterType() == em_MonsterType_Hero)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Galio(CMonster& Hero, BOOL bConsum)
{
	if (PERSON->GetDis(Hero.GetPoint()) <= 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetTargetId() == PERSON->GetId())
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (Hero.GetPercentHp() <= 40 && Hero.GetMonsterType() == em_MonsterType_Hero && UseSkill_By_Dis(em_Skill_Flag_R, *PERSON))
		Sleep(2000);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Lucian(CMonster& Hero, BOOL bConsum)
{
	if (PERSON->GetDis(Hero.GetPoint()) <= 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (!bConsum && PERSON->GetDis(Hero.GetPoint()) <= 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (!bConsum && Hero.GetPercentHp() <= 50)
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	static DWORD dwTickR = 0;
	if (Hero.GetPercentHp() < 40 && ::GetTickCount() - dwTickR >= 5 * 1000 && Hero.GetMonsterType() == em_MonsterType_Hero && UseSkill_By_Dis(em_Skill_Flag_R, *PERSON))
		dwTickR = ::GetTickCount();

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Trundle(CMonster& Hero, BOOL bConsum)
{
	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f && !CPersonBuff::ExistBuff(0x11))
		UseSkill_By_Dis(em_Skill_Flag_Q, *PERSON);

	if (PERSON->GetDis(Hero.GetPoint()) < 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Syndra(CMonster& Hero, BOOL bConsum)
{
	// 会产生一个Seed这种黑球
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) < 7.0f)
	{
		if (CPersonBuff::ExistBuff(0x7))
		{
			// 必然是二次施法
			UseSkill_By_Dis(em_Skill_Flag_W, Hero);
		}
		else
		{
			// 初次施法, 判断7m范围内是否有Seed这个玩意
			static CMonster cmSeed;
			if (FindMonster_For_Name(L"Seed", 7.0f, cmSeed))
				UseSkill_By_Dis(em_Skill_Flag_W, Hero);
		}
	}

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);
	
	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Aatrox(CMonster& Hero, BOOL bConsum)
{
	if (PERSON->GetDis(Hero.GetPoint()) < 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
	{
		UseSkill_By_Dis(em_Skill_Flag_R, *PERSON);
		Sleep(500);
	}
	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Taric(CMonster& Hero, BOOL bConsum)
{
	// Q是加血技能
	if (PERSON->GetPercentHp() > 60 && CSkill::GetSkillCool(em_Skill_Flag_Q))
	{
		static vector<CMonster> vMonsterList;
		// 获取同盟阵营的英雄
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetCamp(), 7.0f, NULL);
		
		for (auto& AllianceHero : vMonsterList)
		{
			if (AllianceHero.GetPercentHp() < 60)
			{
				UseSkill_By_Dis(em_Skill_Flag_Q, AllianceHero);
				break;
			}
		}
	}
	else
	{
		// HP < 60% 给自己加血
		UseSkill_By_Dis(em_Skill_Flag_Q, *PERSON);
	}

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
		UseSkill_By_Dis(em_Skill_Flag_R, *PERSON);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Rumble(CMonster& Hero, BOOL bConsum)
{
	// 过热的沉默状态!
	if (CPersonBuff::ExistBuff(0xE))
		return Hero.NormalAttack();

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
		UseSkill_By_Dis(em_Skill_Flag_Q, *PERSON);

	// 被人围攻的时候才用W
	if (CSkill::GetSkillCool(em_Skill_Flag_W))
	{
		static vector<CMonster> vMonsterList;
		// 获取同盟阵营的英雄
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), 10.0f, NULL);

		for (auto& EnemyHero : vMonsterList)
		{
			if (EnemyHero.GetTargetId() == PERSON->GetId())
			{
				UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);
				break;
			}
		}
	}

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Diana(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (!CPersonBuff::ExistBuff(0x9) && PERSON->GetDis(Hero.GetPoint()) < 3.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
	{
		UseSkill_By_Dis(em_Skill_Flag_E, *PERSON);
		Sleep(500);
	}

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Urgot(CMonster& Hero, BOOL bConsum)
{
	if (PERSON->GetDis(Hero.GetPoint()) < 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	// 被人围攻的时候才用W
	if (CSkill::GetSkillCool(em_Skill_Flag_W))
	{
		static vector<CMonster> vMonsterList;
		// 获取同盟阵营的英雄
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), 10.0f, NULL);

		for (auto& EnemyHero : vMonsterList)
		{
			if (EnemyHero.GetTargetId() == PERSON->GetId())
			{
				UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);
				break;
			}
		}
	}

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Teemo(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	// 追杀的时候才开W加速
	if (Hero.GetPercentHp() < 30)
		UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Caitlyn(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (Hero.GetPercentHp() < 30)
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() < 50)
	{
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);
		Sleep(2000);
	}

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Corki(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (!bConsum && Hero.GetMonsterType() == em_MonsterType_Hero)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (!bConsum)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Fiora(CMonster& Hero, BOOL bConsum)
{
	if (PERSON->GetDis(Hero.GetPoint()) <= 3.0f)
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	// 被人围攻的时候才用W
	if (CSkill::GetSkillCool(em_Skill_Flag_W))
	{
		static vector<CMonster> vMonsterList;
		// 获取同盟阵营的英雄
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), 10.0f, NULL);

		for (auto& EnemyHero : vMonsterList)
		{
			if (EnemyHero.GetTargetId() == PERSON->GetId())
			{
				UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);
				break;
			}
		}
	}

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Kennen(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);

	if (!CPersonBuff::ExistBuff(0x17))
		UseSkill_By_Dis(em_Skill_Flag_E, *PERSON);

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
		UseSkill_By_Dis(em_Skill_Flag_R, *PERSON);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_KogMaw(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (!bConsum && PERSON->GetDis(Hero.GetPoint()) < 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	static DWORD dwTickR = 0;
	if (::GetTickCount() - dwTickR >= 10 * 1000 && UseSkill_By_Dis(em_Skill_Flag_R, Hero))
		dwTickR = ::GetTickCount();

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Mordekaiser(CMonster& Hero, BOOL bConsum)
{
	if (!CPersonBuff::ExistBuff(0x17))
		UseSkill_By_Dis(em_Skill_Flag_Q, *PERSON);

	UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	if (PERSON->GetDis(Hero.GetPoint()))
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (Hero.GetPercentHp() <= 30 && PERSON->GetDis(Hero.GetPoint()) < 5.0f)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Nautilus(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	// 被人围攻的时候才用W
	if (CSkill::GetSkillCool(em_Skill_Flag_W))
	{
		static vector<CMonster> vMonsterList;
		// 获取敌对阵营的英雄
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), 10.0f, NULL);

		for (auto& EnemyHero : vMonsterList)
		{
			if (EnemyHero.GetTargetId() == PERSON->GetId())
			{
				UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);
				break;
			}
		}
	}

	UseSkill_By_Dis(em_Skill_Flag_E, *PERSON);

	if (Hero.GetPercentHp() < 50)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_RekSai(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	// 不用W, 麻烦

	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Sivir(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	// 被人围攻的时候才用W
	if (CSkill::GetSkillCool(em_Skill_Flag_E))
	{
		static vector<CMonster> vMonsterList;
		// 获取敌对阵营的英雄
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), 10.0f, NULL);

		for (auto& EnemyHero : vMonsterList)
		{
			if (EnemyHero.GetTargetId() == PERSON->GetId())
			{
				UseSkill_By_Dis(em_Skill_Flag_E, *PERSON);
				break;
			}
		}
	}

	if (Hero.GetPercentHp() < 30)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Thresh(CMonster& Hero, BOOL bConsum)
{
	if (UseSkill_By_Dis(em_Skill_Flag_Q, Hero))
		Sleep(500);

	if (PERSON->GetDis(Hero.GetPoint()) < 3.0f)
	{
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);
		UseSkill_By_Dis(em_Skill_Flag_E, Hero);
	}

	if (Hero.GetPercentHp() < 30 && PERSON->GetDis(Hero.GetPoint()))
		UseSkill_By_Dis(em_Skill_Flag_R, *PERSON);

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_TwistedFate(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	// 有颜色的牌的情况下, 就不要再洗牌了
	if (!CPersonBuff::ExistBuff(0x10) && !CPersonBuff::ExistBuff(0xF))
		UseSkill_By_Dis(em_Skill_Flag_W, *PERSON);

	// R感觉没鸟用

	return Hero.NormalAttack();
}

BOOL CFightFunction::KillHero_Ahri(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_E, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) <= 5.0f)
	{
		UseSkill_By_Dis(em_Skill_Flag_Q, Hero);
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

		if (Hero.GetMonsterType() == em_MonsterType_Hero && Hero.GetPercentHp() < 40)
			UseSkill_By_Dis(em_Skill_Flag_R, Hero);
	}

	return Hero.NormalAttack();
}



BOOL CFightFunction::KillHero_Annie(CMonster& Hero, BOOL bConsum)
{
	UseSkill_By_Dis(em_Skill_Flag_Q, Hero);

	if (PERSON->GetDis(Hero.GetPoint()) <= 2.0f)
		UseSkill_By_Dis(em_Skill_Flag_W, Hero);

	// 被人围攻的时候才用E
	if (CSkill::GetSkillCool(em_Skill_Flag_E))
	{
		static vector<CMonster> vMonsterList;
		// 获取敌对阵营的英雄
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), 10.0f, NULL);

		for (auto& EnemyHero : vMonsterList)
		{
			if (EnemyHero.GetTargetId() == PERSON->GetId())
			{
				UseSkill_By_Dis(em_Skill_Flag_E, *PERSON);
				break;
			}
		}
	}

	if (Hero.GetMonsterType() == em_MonsterType_Hero && Hero.GetPercentHp() < 40)
		UseSkill_By_Dis(em_Skill_Flag_R, Hero);

	return Hero.NormalAttack();
}
