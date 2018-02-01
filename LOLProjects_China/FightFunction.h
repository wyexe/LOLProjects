#ifndef __FIGHTFUNCTION_H__
#define __FIGHTFUNCTION_H__

#include "GameFunction.h"
#include "PersonBuff.h"

class CFightFunction : CGameFunction
{
public:
	CFightFunction();
	~CFightFunction();

	///////小兵///////////////////////////////////////////////////////////////////

	BOOL KillSolider_Skill(CMonster& Solider, BOOL bUseQ, BOOL bUseW, BOOL bUseE, BOOL bUseR);

	BOOL KillSolider(CMonster& Solider, BOOL bClearAround);

	///////英雄///////////////////////////////////////////////////////////////////

	// 流浪法师
	BOOL KillHero_Ryze(CMonster& Hero, BOOL bConsume/*是否只是消耗一下血量*/);

	// 寒冰射手
	BOOL KillHero_Ashe(CMonster& Hero, BOOL bConsume);

	// 无极剑圣
	BOOL KillHero_MasterYi(CMonster& Hero, BOOL bConsume);

	// 赏金猎人
	BOOL KillHero_MissFortune(CMonster& Hero, BOOL bConsume);

	// 德玛西亚
	BOOL KillHero_Garen(CMonster& Hero, BOOL bConsume);

	// 虚空先知
	BOOL KillHero_Malzahar(CMonster& Hero, BOOL bConsume);

	// 探险家
	BOOL KillHero_Ezreal(CMonster& Hero, BOOL bConsume);

	// 虚空恐惧
	BOOL KillHero_Chogath(CMonster& Hero, BOOL bConsume);

	// 雪人骑士
	BOOL KillHero_Nunu(CMonster& Hero, BOOL bConsume);

	// 大发明家
	BOOL KillHero_Heimerdinger(CMonster& Hero, BOOL bConsume);

	// 法外狂徒
	BOOL KillHero_Graves(CMonster& Hero, BOOL bConsume);

	// 策士统领
	BOOL KillHero_Swain(CMonster& Hero, BOOL bConsume);

	// 暗夜猎手
	BOOL KillHero_Vayne(CMonster& Hero, BOOL bConsume);

	// 扭曲树精
	BOOL KillHero_Maokai(CMonster& Hero, BOOL bConsum);

	// 邪恶小法师
	BOOL KillHero_Veigar(CMonster& Hero, BOOL bConsum);

	// 凛冬之怒
	BOOL KillHero_Sejuani(CMonster& Hero, BOOL bConsum);

	// 麦林炮手
	BOOL KillHero_Tristana(CMonster& Hero, BOOL bConsum);

	// 哨兵之殇
	BOOL KillHero_Galio(CMonster& Hero, BOOL bConsum);

	// 圣枪游侠
	BOOL KillHero_Lucian(CMonster& Hero, BOOL bConsum);

	// 巨魔之王
	BOOL KillHero_Trundle(CMonster& Hero, BOOL bConsum);

	// 暗黑元首
	BOOL KillHero_Syndra(CMonster& Hero, BOOL bConsum);

	// 暗裔剑魔
	BOOL KillHero_Aatrox(CMonster& Hero, BOOL bConsum);

	// 宝石骑士
	BOOL KillHero_Taric(CMonster& Hero, BOOL bConsum);

	// 机械公敌
	BOOL KillHero_Rumble(CMonster& Hero, BOOL bConsum);

	// 皎洁女神
	BOOL KillHero_Diana(CMonster& Hero, BOOL bConsum);

	// 首领之傲
	BOOL KillHero_Urgot(CMonster& Hero, BOOL bConsum);

	// 迅捷斥候
	BOOL KillHero_Teemo(CMonster& Hero, BOOL bConsum);

	// 皮城女警
	BOOL KillHero_Caitlyn(CMonster& Hero, BOOL bConsum);

	// 英勇投弹手
	BOOL KillHero_Corki(CMonster& Hero, BOOL bConsum);

	// 无双剑姬
	BOOL KillHero_Fiora(CMonster& Hero, BOOL bConsum);

	// 狂暴之心
	BOOL KillHero_Kennen(CMonster& Hero, BOOL bConsum);

	// 铁铠冥魂
	BOOL KillHero_Mordekaiser(CMonster& Hero, BOOL bConsum);

	// 深海泰坦
	BOOL KillHero_Nautilus(CMonster& Hero, BOOL bConsum);

	// 虚空遁地兽
	BOOL KillHero_RekSai(CMonster& Hero, BOOL bConsum);

	// 战争女神
	BOOL KillHero_Sivir(CMonster& Hero, BOOL bConsum);

	// 魂锁典狱长
	BOOL KillHero_Thresh(CMonster& Hero, BOOL bConsum);

	// 卡牌大师
	BOOL KillHero_TwistedFate(CMonster& Hero, BOOL bConsum);

	// 深渊巨口
	BOOL KillHero_KogMaw(CMonster& Hero, BOOL bConsum);

	// 九尾妖狐
	BOOL KillHero_Ahri(CMonster& Hero, BOOL bConsum);

	// 黑暗之女
	BOOL KillHero_Annie(CMonster& Hero, BOOL bConsum);

	BOOL KillHero(CMonster& Hero, BOOL bConsume);

	////////处理打怪逻辑//////////////////////////////////////////////////////////////////
	// 攻击小兵
	BOOL AttackSolider(BOOL bClearAround);
	BOOL HitSolider(CMonster& Solider, BOOL bClearAround);

	// 清塔
	BOOL AttackTurret();
	BOOL HitTurret(CMonster& Turret);
	BOOL IsDodgeTurret();
	BOOL IsDodgeCrystalTurret();
	BOOL DodgeCrystalTurret();
	BOOL DodgeTurret();
	BOOL HandleRegainMove();

	// 是不是对面人多 , 应该后退!
	BOOL IsEvacuate();
	BOOL IsEvacuate_Crystal();
	BOOL IsEvacuate_Scuffle();
	BOOL Evacuate();

	// 消耗敌方英雄的血量
	BOOL ConsumeEnemyHero();
	// 攻击敌方英雄
	BOOL AttackHero();
	// 使用恢复性的技能
	BOOL UseSpecialSkill();	
	// 闪避敌方英雄的攻击
	BOOL DodgeHero();

	// 是不是该去打龙了
	BOOL IsKillDragon();
	BOOL KillDragon();

	// 是不是该去收蓝BUFF了
	BOOL IsKillBlueBuff();
	BOOL KillBlueBuff();

	// 是不是该去收红BUFF了
	BOOL IsKillRedBuff();
	BOOL KillRedBuff();

	// 清附近的野怪
	BOOL KillAroundSRU(float fDis);

	BOOL HandleBackHome();

	////////处理分批逻辑//////////////////////////////////////////////////////////////////
	BOOL HandleTurret();
	BOOL HandleHero();
	BOOL HandleSolider();
	BOOL SwitchPath();

	// Party开始!
	BOOL PartyStart();

	BOOL TestAAA();
private:
	BOOL bStopMoveToEnd;
	DWORD dwKillDragonTick;
	DWORD dwKillBlueBuffTick;
	DWORD dwKillRedBuffTick;
	BOOL bSwitchPath;
	DWORD dwDragonCount;
};



#endif