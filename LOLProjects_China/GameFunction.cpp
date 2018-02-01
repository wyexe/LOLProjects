#include "stdafx.h"
#include "GameFunction.h"
#include "HookCode.h"
#include "../../MyTools/MyTools/CLSearchBase.h"

#define _SELF L"GameFunction.cpp"

CGameFunction::CGameFunction()
{
	hGuardPersonThread = NULL;
}

CGameFunction::~CGameFunction()
{
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"~CGameFunction");
	//StopGame;
	if (hGuardPersonThread != NULL && WaitForSingleObject(hGuardPersonThread, 2000) == WAIT_TIMEOUT)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"结束辅助线程失败……强制关闭线程!");
		::TerminateThread(hGuardPersonThread, 0);
		::CloseHandle(hGuardPersonThread);
		hGuardPersonThread = NULL;
	}
}

static DWORD WINAPI GuardPersonThread(LPVOID lpParm)
{
	static DWORD dwHpTick = 0;
	static DWORD dwMpTick = 0;
	DWORD dwMoneyTick = ::GetTickCount();
	BOOL bDisCon = FALSE;
	CGameFunction* pGF = (CGameFunction *)lpParm;
	DWORD dwCount = 0;
	DWORD dwStartTick = ::GetTickCount();
	BOOL bShow = TRUE;

	//if (CLPublic::FileExit(L"C:\\admin.CL"))
		bShow = FALSE;

	auto fnShowWindow = [&bShow]()
	{
		if (bShow && ::GetForegroundWindow() != g_pAccountGame->hWndGame)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"Game窗口置顶");
			ShowWindow(g_pAccountGame->hWndGame, SW_RESTORE);
			::SetWindowPos(g_pAccountGame->hWndGame, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			SwitchToThisWindow(g_pAccountGame->hWndGame, TRUE);
		}
	};
	
	fnShowWindow();
	//CGameSend::Sleep(20 * 1000);
	DWORD dwMoney = -1;
	while (GameStatus_Run)
	{
		CGameSend::Sleep(1000);
		// 游戏结束了
		if (pGF->IsGameOver())
		{
			CGameSend::Sleep(5 * 1000);
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
			StopGame;
			pGF->hGuardPersonThread = NULL;
			break;
		}

		// 是否需要学习技能
		pGF->HandleSkillLevelUp();

		if (!g_bGameStart)
		{
			if (::GetTickCount() - dwStartTick > 3 * 60 * 1000) // 开始3min后开始计算掉线!
				g_bGameStart = TRUE;
		}
		else
		{
			bDisCon = (dwMoney == PERSON->GetMoney()) ? TRUE : FALSE;

			if (bDisCon)
			{
				if (::GetTickCount() - dwMoneyTick >= 10 * 1000)
				{
					CPrintLog::PrintLog_W(_SELF, __LINE__, L"10s内金钱没变化,应该是掉线了!dwMoney=%d,Now=%d", dwMoney, PERSON->GetMoney());
					::TerminateProcess(::GetCurrentProcess(), 0);
					ExitProcess(0);
					CLPublic::ForceExit();
				}
			}
			else
			{
				dwMoneyTick = ::GetTickCount();
			}

			/*if (++dwCount % 4 == 0 && g_vGameTalkMsg.size() > 0)
			{
				fnShowWindow();

				GameTalkMsg GTM = g_vGameTalkMsg.at(0);
				g_vGameTalkMsg.clear();

				static WCHAR wszRandText[32];
				CLPublic::SendKey(VK_RETURN);
				CGameSend::Sleep(500);
				CLPublic::SendKeys(GTM.wszMsg);
				CGameSend::Sleep(500);
				CLPublic::SendKey(VK_RETURN);
			}*/
		}

		if (EMPTY_PERSONHP)
			continue;

		if (::GetTickCount() - dwHpTick >= 8 * 1000 && PERSON->GetPercentHp() <= 70)
		{
			if (CEqument::ExistItem(0x7D3) && CSkill::UseItemSkill(L"RegenerationPotion"))
				dwHpTick = ::GetTickCount();
		}
		if (::GetTickCount() - dwMpTick >= 8 * 1000 && PERSON->GetPercentMp() <= 70)
		{
			if (CEqument::ExistItem(0x7D4) && CSkill::UseItemSkill(L"FlaskOfCrystalWater"))
				dwMpTick = ::GetTickCount();
		}
	}
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"GuardPersonThread End");
	return 0;
}

BOOL CGameFunction::InitAccountGame()
{
	auto pSkillConfig = CGameRes::FindSkillConfig(PERSON->GetHeroName());
	MsgBoxDebug_W(pSkillConfig != NULL, _SELF, __LINE__, L"少年,这个英雄:%s 还没做呢!", PERSON->GetHeroName());

	// 设置英雄职业 枚举类型
	g_pAccountGame->MyAcount_Info.emHeroPro = (em_Hero_Pro)pSkillConfig->nHeroPro;
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"nHeroPro=%X", pSkillConfig->nHeroPro);

	// 设置英雄的分路的路线
	if (g_MapType == MapType_召唤师峡谷)
	{
		/*switch (g_pAccountGame->MyAcount_Info.emHeroPro)
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
			MsgBoxDebug_W(FALSE, _SELF, __LINE__, L"少年,这个英雄:%s暂时还不支持喔~!", PERSON->GetHeroName());
			break;
		}*/
	}
	else if (g_MapType == MapType_水晶之痕)
	{
		switch (g_pAccountGame->MyAcount_Info.emHeroPro)
		{
		case em_Hero_Pro_Ryze:  case em_Hero_Pro_Garen:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Top;
			break;
		default:
			g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Buttom;
		}
	}
	else if (g_MapType == MapType_嚎哭深渊)
	{
		g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Middle;
	}
	

	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"PERSON->GetLevel()=%d", PERSON->GetLevel());
	if (PERSON->GetLevel() >= 8 || g_MapType == MapType_嚎哭深渊)
		g_fMaxDis_Attack = 15.0f;

	return InitTimer();
Function_Exit:;
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
	StopGame;
	return FALSE;
}

__inline VOID FillStackFrame(DWORD(&dwFakeStackFrame)[MAX_PATH], DWORD dwFakeRetAddr)
{
	for (int i = 0; i < 100; i += 2)
	{
		dwFakeStackFrame[i] = (DWORD)&dwFakeStackFrame[i + 2];
		dwFakeStackFrame[i + 1] = dwFakeRetAddr;
	}
}

static DWORD g_dwHookRetAddr = 0;
VOID WINAPI HandleModule()
{
	static DWORD dwFakeStackFrame[MAX_PATH] = { 0 };

	if (g_dwStudySkillIndex != -1)
	{
		FillStackFrame(dwFakeStackFrame, C_CALL_mid);
		CALL_StudySkill(dwFakeStackFrame);
		g_dwStudySkillIndex = -1;
	}

	if (g_dwBuyItemId != NULL)
	{
		DWORD dwFakeStackFrame[MAX_PATH] = { 0 };
		FillStackFrame(dwFakeStackFrame, C_CALL_mid);
		CALL_BuyItem(dwFakeStackFrame);
		g_dwBuyItemId = NULL;
	}

	if (g_bMove)
	{
		FillStackFrame(dwFakeStackFrame, C_CALL_mid);
		CALL_Move(dwFakeStackFrame);
		g_bMove = FALSE;
	}

	if (g_dwNormalAttackBase != NULL)
	{
		FillStackFrame(dwFakeStackFrame, C_CALL_mid);
		CALL_Attack(dwFakeStackFrame);
		g_dwNormalAttackBase = NULL;
	}

	if (g_bUseSkill)
	{
		FillStackFrame(dwFakeStackFrame, C_CALL_mid);
		CALL_UseSkill(dwFakeStackFrame);
		g_bUseSkill = FALSE;
	}

	if (g_bStopAction)
	{
		FillStackFrame(dwFakeStackFrame, C_CALL_mid);
		CALL_Stop(dwFakeStackFrame);
		g_bStopAction = FALSE;
	}

	if (g_dwUseItemParm != NULL)
	{
		FillStackFrame(dwFakeStackFrame, C_CALL_mid);
		CALL_UseItem(dwFakeStackFrame);
		g_dwUseItemParm = NULL;
	}

	if (g_dwOccupyObjBase != NULL)
	{
		FillStackFrame(dwFakeStackFrame, C_CALL_mid);
		CALL_Occupy(dwFakeStackFrame);
		g_dwOccupyObjBase = NULL;
	}

	//__asm POPAD
	//__asm MOV EAX, g_dwHookRetAddr
	//__asm JMP EAX
}

__declspec(naked) VOID WINAPI HookModule()
{
	static DWORD gs_dwThreadId;
	gs_dwThreadId = ::GetCurrentThreadId();
	if (gs_dwThreadId != g_dwMainThreadId)
		goto lb_Exit;

	__asm PUSHAD
	//HandleModule();
	
	static DWORD dwTick = NULL;
	if (dwTick == NULL)
		dwTick = ::GetTickCount();

	CPrintLog::PrintLog_W(_SELF,__LINE__,L"%dms", ::GetTickCount() - dwTick);
	dwTick = ::GetTickCount();

	__asm POPAD
	lb_Exit:
	__asm MOV EAX, g_dwHookRetAddr
	__asm JMP EAX
}

static void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	if (nTimerid != 0x1000)
		return;

	HandleModule();
	
}
static DWORD dwHookD3dJmpAddr = 0;
static DWORD gs_dwSleepTime = 30;
__declspec(naked) VOID WINAPI HookD3D()
{
	__asm PUSHAD

	static DWORD gs_dwThreadId;
	gs_dwThreadId = ::GetCurrentThreadId();
	if (gs_dwThreadId == g_dwMainThreadId)
	{
		HandleModule();
		//::Sleep(100);
	}

	__asm POPAD
	__asm MOV EAX, dwHookD3dJmpAddr
	__asm JMP EAX
}

BOOL CGameFunction::InitTimer()
{
	static BOOL bHook = FALSE;
	if (bHook)
		return TRUE;

	g_dwTimerId = ::SetTimer(g_pAccountGame->hWndGame, 0x1000, 100, TimerProc);
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"g_dwTimerId=%X", g_dwTimerId);
	hGuardPersonThread = chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)GuardPersonThread, this, NULL, NULL);
	

	g_dwHookD3dAddr = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(C_D3D_BASE) + 0x14) + 0x14) + 0x0);
	g_dwHookD3dValue = ReadDWORD(g_dwHookD3dAddr);
	dwHookD3dJmpAddr = ReadDWORD(g_dwHookD3dAddr + 0x44);
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"g_dwHookD3dAddr=%X,g_dwHookD3dValue=%X,dwHookD3dJmpAddr=%X");

	if (CLPublic::FileExit(L"C:\\admin.CL"))
		gs_dwSleepTime = 0;
	else
		gs_dwSleepTime = 100;

	/*DWORD dwOldProtect = 0;
	if (::VirtualProtectEx(GetCurrentProcess(), (LPVOID)(g_dwHookD3dAddr + 0x44), 4, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		*(PDWORD)(g_dwHookD3dAddr + 0x44) = (DWORD)HookD3D;
		::VirtualProtectEx(GetCurrentProcess(), (LPVOID)(g_dwHookD3dAddr + 0x44), 4, dwOldProtect, &dwOldProtect);
	}*/

	//TerminateTestThread();
	bHook = TRUE;

	/*g_dwHookSimKeyAddr = CLSearchBase::FindAddr("741a395d247415", 0x77276CF7 - 0x77276D35, 0, L"USER32.dll") + 0x1;

	DWORD dwReaderAddr = ReadDWORD(g_dwHookSimKeyAddr);
	dwReaderAddr += 4;
	dwReaderAddr += g_dwHookSimKeyAddr;
	g_dwHookSimKeyCALL = dwReaderAddr & 0xFFFFFFFF;

	g_dwHookSimKeyAddr = g_dwHookSimKeyAddr - 0x1 + 0x5;
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"g_dwHookSimKeyAddr=%X,g_dwHookSimKeyCALL=%X", g_dwHookSimKeyAddr, g_dwHookSimKeyCALL);
	//钩挂获取
	MYHOOK_CONTENT HC;
	HC.dwFunAddr = (DWORD)HookKeyDownPrint;
	HC.dwHookAddr = g_dwHookSimKeyAddr - 0x5;
	HC.dwOldCode1 = ReadDWORD(HC.dwFunAddr + 0x0);
	HC.dwOldCode2 = ReadDWORD(HC.dwFunAddr + 0x4);
	HC.uNopCount = 0x0;
	CLHook::Hook_Fun_Jmp_MyAddr(&HC);

	CGameFunction::AddHookList(HC);*/

	return TRUE;
}

BOOL CGameFunction::TerminateTestThread()
{
	return 0;
	vector<CL_PROCESS_THREADINFO> vlst;
	if (!CLThread::QueryThreadInfo_By_Pid(::GetCurrentProcessId(), vlst))
		return FALSE;

	for (auto& i : vlst)
	{
		if (CCharacter::wstrstr_my(i.wszModuleName, L"TenSLX.dll") || CCharacter::wstrstr_my(i.wszModuleName, L"TenRpcs.dll"))
		{
			

			HANDLE hThread = ::OpenThread(THREAD_ALL_ACCESS, FALSE, i.dwTid);
			if (hThread == NULL)
				continue;

			
			::SuspendThread(hThread);
			::TerminateThread(hThread, 0);
			::CloseHandle(hThread);
			hThread = NULL;
		}
	}
	return TRUE;
}

BOOL CGameFunction::MoveToPoint(Point& EndPoint, float fDis)
{
	if (PERSON->GetDis(EndPoint) <= fDis)
		return TRUE;

	DWORD dwCount = 0;
	CGameSend::Move(EndPoint);
	while (GameStatus_Run && !EMPTY_PERSONHP && PERSON->GetDis(EndPoint) > fDis)
	{
		Point NowPoint = PERSON->GetPoint();
		CGameSend::Sleep(500);
		if (NowPoint == PERSON->GetPoint())
		{
			if (++dwCount >= 3)
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"多次卡点不动……break;");
				break;
			}
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"ReMove,Stop,fdis=%.2f", PERSON->GetDis(EndPoint));
			CGameSend::Move(EndPoint);
		}
	}

	return CGameSend::StopAction();
}

BOOL CGameFunction::BackOff()
{
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"BackOff……");
	if (g_MapType == MapType_召唤师峡谷)
	{
		auto pResPoint = CGameRes::FindRecentlyResPoint(PERSON->GetPoint(), g_pAccountGame->MyAcount_Info.emPathFlag, PERSON->GetCamp(), FALSE);
		MsgBoxDebug_W(pResPoint != NULL, _SELF, __LINE__, L"怎么可能,找不到路径坐标!!!");

		return CGameSend::Move(pResPoint->Pit);
	}
	else if (g_MapType == MapType_水晶之痕 || g_MapType == MapType_嚎哭深渊)
	{
		static Point BackOffPoint(0, 0, 0);
		if (BackOffPoint.GetX() == 0) // 只初始化一次!
			BackOffPoint = CGameRes::GetSpringPoint(PERSON->GetCamp());

		return CGameSend::Move(BackOffPoint);
	}
	
Function_Exit:;
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"StopGame");
	StopGame;
	return FALSE;
}

BOOL CGameFunction::MoveEnd()
{
	// 分段走
	auto fnSubsectionMove = [=](Point& EndPoint)
	{
		/*const float fMoveDis = 7.0f;			// 分段的每次移动的最长的移动距离
		Point NowPoint = PERSON->GetPoint();
		float fDis = PERSON->GetDis(EndPoint);

		if (fDis > fMoveDis)
		{
			float fScale = fMoveDis / fDis;	// 相似三角形比例
			//CPrintLog::PrintLog_W(_SELF,__LINE__,L"PT:%d,%d",EndPoint.GetX(), EndPoint.GetY());
			Point TmpPoint = EndPoint;
			TmpPoint.X = (EndPoint.X - NowPoint.X) * fScale + NowPoint.X;
			TmpPoint.Y = (EndPoint.Y - NowPoint.Y) * fScale + NowPoint.Y;
			TmpPoint.Z = EndPoint.Z;
			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"TmpPoint:%d,%d", TmpPoint.GetX(), TmpPoint.GetY());
			return CGameSend::Move(TmpPoint);
		}*/
		return CGameSend::Move(EndPoint);
	};

	auto pResPoint = CGameRes::FindRecentlyResPoint(PERSON->GetPoint(), g_pAccountGame->MyAcount_Info.emPathFlag, PERSON->GetCamp(), TRUE);
	if (pResPoint == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"怎么可能,找不到路径坐标!!!");
		StopGame;
		return TRUE;
	}
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"pt:%.2f,%.2f,%.2f", pResPoint->Pit.X, pResPoint->Pit.Y, pResPoint->Pit.Z);
	fnSubsectionMove(pResPoint->Pit);
	return TRUE;
}

BOOL CGameFunction::MoveToEndPoint()
{
	if (g_MapType == MapType_召唤师峡谷 || g_MapType == MapType_嚎哭深渊)
		return MoveEnd();
	else if (g_MapType == MapType_水晶之痕)
		return MoveToEnemyTurrent();

	return TRUE;
}

BOOL CGameFunction::MoveToEnemyTurrent()
{
	static Point MovePoint;
	vector<CMonster> vlst;
	auto fnSoldier_by_Camp = [=, &vlst](em_Camp emCamp)
	{
		static vector<CMonster> vGuardLst;
		// 获取敌对阵营的塔
		GetMonsterList_By_Type(vGuardLst, em_MonsterType_Soldier, emCamp, 1000.0f, NULL);

		for_each(vGuardLst.begin(), vGuardLst.end(), [&vlst](CMonster& cmGuardian)
		{
			if (CCharacter::wstrcmp_my(cmGuardian.GetName(), L"OdinNeutralGuardian"))
			{
				vlst.push_back(cmGuardian);
			}
		});
	};

	auto fnGetTurrent_By_Point = [=, &vlst](Point TarPoint)
	{
		int nIndex = -1;
		float fMaxDis = 100000.0f;
		for (UINT i = 0; i < vlst.size(); ++i)
		{
			float fDis = CGameSend::Get2DDistance(&vlst[i].GetPoint(), &TarPoint);
			if (fDis < fMaxDis)
			{
				fMaxDis = fDis;
				nIndex = i;
			}
		}

		return &vlst.at(nIndex);
	};

	// 分段走
	auto fnSubsectionMove = [=](Point& EndPoint)
	{
		const float fMoveDis = 7.0f;			// 分段的每次移动的最长的移动距离
		Point NowPoint = PERSON->GetPoint();
		float fDis = PERSON->GetDis(EndPoint);

		if (fDis > fMoveDis)
		{
			float fScale = fMoveDis / fDis;	// 相似三角形比例
			//CPrintLog::PrintLog_W(_SELF,__LINE__,L"PT:%d,%d",EndPoint.GetX(), EndPoint.GetY());
			Point TmpPoint = EndPoint;
			TmpPoint.X = (EndPoint.X - NowPoint.X) * fScale + NowPoint.X;
			TmpPoint.Y = (EndPoint.Y - NowPoint.Y) * fScale + NowPoint.Y;
			TmpPoint.Z = EndPoint.Z;
			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"TmpPoint:%d,%d", TmpPoint.GetX(), TmpPoint.GetY());
			return CGameSend::Move(TmpPoint);
		}
		return CGameSend::Move(EndPoint);
	};

	// 判断是否有+血的, 不管血量, 总比对面的人吃了要好吧?
	/*CMonster cmOdinShieldRelic;
	static DWORD dwShieldTick = 0;
	if (::GetTickCount() - dwShieldTick >= 30 * 1000 && FindMonster_For_Name(L"OdinShieldRelic", 10.0f, cmOdinShieldRelic))
	{
		if (PERSON->GetDis(cmOdinShieldRelic.GetPoint()) <= 1.0f)
			dwShieldTick = ::GetTickCount();

		MovePoint = cmOdinShieldRelic.GetPoint();
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"Move:拿血包!");
		return CGameSend::Move(MovePoint);
	}*/

	// 获取所有的 敌方塔 和 未占领的塔 还有自己的塔
	fnSoldier_by_Camp(em_Camp_Neutral); // 未占领
	fnSoldier_by_Camp(PERSON->GetEnemyCamp()); // 敌方
	fnSoldier_by_Camp(PERSON->GetCamp()); // 自己的塔

	if (g_pAccountGame->MyAcount_Info.emPathFlag == em_Path_Flag_Buttom)
	{
		// 左边: 左下， 左上， 上     
		// 右边: 右下， 右上， 上
		for (DWORD i = 0; i < 3; ++i)
		{
			auto pResPoint = CGameRes::GetResPoint_By_Index(i, PERSON->GetCamp());
			if (pResPoint != NULL)
			{
				CMonster* pTurrent = fnGetTurrent_By_Point(pResPoint->Pit);
				// 判断该顺序下的塔是否是自己的, 不是自己的, 就跑去抢!
				if (pTurrent->GetCamp() != PERSON->GetCamp())
					return fnSubsectionMove(pResPoint->Pit);
			}
		}
	}
	else
	{
		vector<int> IndexLst;

		// 上路只负责2个塔, 左下角和右下角的塔
		if (PERSON->GetCamp() == em_Camp_Blue)
		{
			IndexLst.push_back(3); // 己方左下边索引
			IndexLst.push_back(0); // 敌方右下角索引
		}
		else
		{
			IndexLst.push_back(0); // 敌方右下角索引
			IndexLst.push_back(3); // 己方左下边索引
		}

		for (UINT i = 0; i < IndexLst.size(); ++i)
		{
			auto pResPoint = CGameRes::GetResPoint_By_Index((DWORD)IndexLst.at(i), PERSON->GetCamp());
			if (pResPoint != NULL)
			{
				CMonster* pTurrent = fnGetTurrent_By_Point(pResPoint->Pit);
				// 判断该顺序下的塔是否是自己的, 不是自己的, 就跑去抢!
				if (pTurrent->GetCamp() != PERSON->GetCamp())
					return fnSubsectionMove(pResPoint->Pit);
			}
		}

		// 如果到这步了, 表示已经抢占了敌方的下方塔了, 敌人必定要来抢的, 所以这2个人就务必要守住!
		auto pResPoint = CGameRes::GetResPoint_By_Index((DWORD)IndexLst.at(0), PERSON->GetCamp());
		if (pResPoint != NULL)
		{
			CMonster* pTurrent = fnGetTurrent_By_Point(pResPoint->Pit);
			if (pTurrent != NULL)
			{
				static DWORD dwtmpCount = 0;
				if (++dwtmpCount % 4 == 0)
					return fnSubsectionMove(pResPoint->Pit);
				else
					return fnSubsectionMove(pTurrent->GetPoint());
			}
		}
	}
	

	// 如果到这步了, 就表示那3个塔都是我们的, 现在应该是去四塔和五塔
	for (DWORD i = 3; i < 5; ++i)
	{
		auto pResPoint = CGameRes::GetResPoint_By_Index(i, PERSON->GetCamp());
		if (pResPoint != NULL)
		{
			CMonster* pTurrent = fnGetTurrent_By_Point(pResPoint->Pit);
			// 判断该顺序下的塔是否是自己的, 不是自己的, 就跑去抢!
			if (pTurrent->GetCamp() != PERSON->GetCamp())
				return fnSubsectionMove(pResPoint->Pit);
		}
	}

	// 到这里了, 就表示5个塔都是自己的, 随便吧
	static Point TopTurrentPoint(6431.00f, 10273.00f, -188.57f);
	static Point TopNextTurrentPoint(6431.00f + 500.0f, 10273.00f + 500.0f, -188.57f);

	static int nTmpCount = 0;
	if (++nTmpCount % 2 == 0)
		return fnSubsectionMove(TopTurrentPoint);

	return fnSubsectionMove(TopNextTurrentPoint);
}

BOOL CGameFunction::UseSkill_By_Dis(em_Skill_Flag emSkillFlag, CMonster& Obj)
{
	// 判断技能是否冷却完毕!
	if (!CSkill::GetSkillCool(emSkillFlag))
		return FALSE;

	// 获取技能的施法距离
	float fSkillDis = CSkill::GetSkillDis(emSkillFlag);
	if (fSkillDis == 0.0f)// 给自己加状态的技能
		return CSkill::UseNormalSkill(emSkillFlag, Obj);

	if (PERSON->GetDis(Obj.GetPoint()) > fSkillDis)
	{
		// 判断是否在塔的范围内
		CMonster Turret;
		if (GetRecentlyEnemyTurret(MaxDis_Attack + 1.0f, NULL, Turret))
		{
			// 人物在塔下就不要跑过去放技能了,而且塔不能是水晶塔
			if (!Turret.IsCrystalTurret() && Obj.GetDis(Turret.GetPoint()) < MaxDis_Attack)
				return FALSE;

		}

		// 技能超出了施法范围, 先走近一点
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"这个对象超出了施法的范围,范围是:%.2f,现在Dis=%.2f", fSkillDis, PERSON->GetDis(Obj.GetPoint()));
		CGameSend::Move(Obj.GetPoint());
		//return FALSE; // 不算是成功施法
	}

	return CSkill::UseNormalSkill(emSkillFlag, Obj);
}

/////////Other Function/////////////////////////////////////////////////////////////////
BOOL CGameFunction::IsGameOver()
{
	auto fnCreateWinMutex = []()
	{
		::CreateMutex(NULL, FALSE, Mutex_GameResult_Win);
	};

	auto fnCreateLoseMutex = []()
	{
		::CreateMutex(NULL, FALSE, Mutex_GameResult_Lose);
	};

	if (g_MapType == MapType_召唤师峡谷 || g_MapType == MapType_嚎哭深渊)
	{
		static CMonster BlueHome;
		static CMonster RedHome;
		if (!FindMonster_For_Name(L"HQ_T1", 1000.0f, BlueHome))
		{
			if (PERSON->GetCamp() == em_Camp_Blue)
			{
				fnCreateLoseMutex();
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"握草,蓝方输了");
				g_pAccountGame->MyAcount_Info.bWin = FALSE;
			}
			else
			{
				fnCreateWinMutex();
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"Yeah！红方赢了");
				g_pAccountGame->MyAcount_Info.bWin = TRUE;
			}
			//g_pAccountGame->MyAcount_Info.bClose = TRUE;
			return TRUE;
		}
		else if (!FindMonster_For_Name(L"HQ_T2", 1000.0f, RedHome))
		{
			if (PERSON->GetCamp() == em_Camp_Blue)
			{
				fnCreateWinMutex();
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"Yeah！蓝方赢了");
				g_pAccountGame->MyAcount_Info.bWin = TRUE;
			}
			else
			{
				fnCreateLoseMutex();
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"握草,红方输了");
				g_pAccountGame->MyAcount_Info.bWin = FALSE;
			}
			//g_pAccountGame->MyAcount_Info.bClose = TRUE;
			return TRUE;
		}
	}
	else if (g_MapType == MapType_水晶之痕)
	{
		DWORD dwSelfHp = CGameSend::GetSelfHp();
		DWORD dwEnemyHp = CGameSend::GetEnemyHp();

		auto fnGameOver = [](DWORD dwHp)
		{
			return dwHp < 3 || dwHp > 500;
		};
		
		if (fnGameOver(CGameSend::GetSelfHp()))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"卧槽……输了");
			Sleep(10 * 1000);
			fnCreateLoseMutex();
			return TRUE;
		}
		if (fnGameOver(CGameSend::GetEnemyHp()))
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"Yes!,赢了!");
			Sleep(10 * 1000);
			fnCreateWinMutex();
			return TRUE;
		}
		
	}
	return FALSE;
}

BOOL CGameFunction::IsBackHome()
{
	// MP 低于 5% 就回家, 但是盲僧他没蓝!
	if (PERSON->GetPercentMp() < 5)
		return TRUE;

	if (PERSON->GetPercentHp() <= 25)
		return TRUE;

	if (g_MapType == MapType_水晶之痕 && PERSON->GetMoney() >= 3000)
		return TRUE;

	return FALSE;
}

BOOL CGameFunction::BackHome()
{
lb_ReStart:;
	if (!GameStatus_Run || EMPTY_PERSONHP)
		return FALSE;

	// 跑到塔下
	MoveToTurret(0, g_pAccountGame->MyAcount_Info.emPathFlag);

	// 判断是否回城完毕了
	auto fn_IsBackHome = [=]
	{
		if (g_MapType == MapType_召唤师峡谷)
		{
			static CMonster OrderTurretShrine;
			static CMonster ChaosTurretShrine;
			if (((PERSON->GetCamp() == em_Camp_Blue && FindMonster_For_Name(L"Turret_OrderTurretShrine_A", 15.0f, OrderTurretShrine)) || \
				(PERSON->GetCamp() == em_Camp_Red && FindMonster_For_Name(L"Turret_ChaosTurretShrine_A", 15.f, ChaosTurretShrine))) && \
				GameStatus_Run && !EMPTY_PERSONHP)
			{
				return TRUE;
			}
		}
		else if (g_MapType == MapType_水晶之痕)
		{
			if (PERSON->GetDis(CGameRes::GetSpringPoint(PERSON->GetCamp())) < 3.0f)
				return TRUE;
		}
		return FALSE;
	};

	if (fn_IsBackHome())
	{
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"已经用11路回泉水完毕!");
		return Rest();
	}

	// 按B回城
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"按B");
	auto fn_ReCall = [this]
	{
		static vector<CSkill> vSkillList;
		CSkill::GetSkillList(vSkillList);

		for (auto Skill : vSkillList)
		{
			if (CCharacter::wstrcmp_my(Skill.GetName(), SkillName_BackHome) || CCharacter::wstrcmp_my(Skill.GetName(),L"OdinRecall"))
			{
				CGameSend::StopAction();
				CGameSend::Sleep(500);
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"使用 'recall'");
				Skill.UseSkill(*PERSON, em_SkillType_Buff_Self);
				CGameSend::Sleep(500);
				break;
			}
		}

	};
	
	fn_ReCall();
	for (int i = 0; i < 10 && GameStatus_Run; ++i)
	{
		if ((GetAroundEnemyHeroCount(10.0f, NULL) != 0 || GetAroundEnemySoliderCount(10.0f, NULL) != 0))
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"周围有敌人……走远点再回城");
			goto lb_ReStart;
		}
		CGameSend::Sleep(1000);
	}

	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"使用完毕!");
	// 判断是否回城失败了,是的话,直接走路回去吧,别传送了
	if (fn_IsBackHome())
	{
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"已经用回泉水完毕!");
		return Rest();
	}
	
	// 回城失败就递归传送
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"回城失败,重新执行");
	return BackHome();
}

BOOL CGameFunction::Rest()
{
	RECT r;
	::GetWindowRect(g_pAccountGame->hWndGame, &r);

	while (GameStatus_Run && (PERSON->GetPercentHp() < 70 || PERSON->GetPercentMp() < 70))
	{
		// 狂点击鼠标
		//CLPublic::SimulationRightMouse(r.left + 530, r.top + 350);
		Sleep(500);
	}
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"休息够了,看下是不是需要买点装备!");
	return CEqument::BuyEqument();
}

BOOL CGameFunction::WaitToStartGame()
{
	if (g_MapType == MapType_召唤师峡谷 || g_MapType == MapType_嚎哭深渊)
	{
		// 等待出兵就算游戏开始
		while (GameStatus_Run)
		{
			static vector<CMonster> vlst;
			GetMonsterList_By_Type(vlst, em_MonsterType_Soldier, PERSON->GetCamp(), 1000.0f, NULL);
			if (vlst.size() > 5)
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"出发!,size=%d", vlst.size());
				break;
			}
			CGameSend::Sleep(1000);
		}
	}
	else if (g_MapType == MapType_水晶之痕)
	{
		// 默认就算游戏开始了
		return TRUE;
	}
	
	return TRUE;
}

BOOL CGameFunction::HandleSkillLevelUp()
{
	// 是否需要学习技能
	if (PERSON->GetSkillPoint() != 0)
	{
		CSkill::LevelUpSkill();
		DWORD dwLevel = PERSON->GetLevel();
		if (dwLevel == 8)
			g_fMaxDis_Attack = 15.0f;
	}
	return FALSE;
}

////////////AroundObject//////////////////////////////////////////////////////////////
UINT CGameFunction::GetMonsterList_By_Type(vector<CMonster>& vMonsterList, em_MonsterType emType, em_Camp emCamp, float fDis, Point* pFixPoint)
{
	Point FixPoint;
	if (pFixPoint == NULL)
		FixPoint = PERSON->GetPoint();
	else
		FixPoint = *pFixPoint;

	em_Camp emPersonCamp = PERSON->GetCamp();
	CMonster::GetAroundObjectList(vMonsterList);
	for (int i = 0; i < (int)vMonsterList.size(); ++i)
	{
		auto pMonster = &vMonsterList.at(i);

		// 迷雾外 || 怪物类型 || 怪物阵营
		if ((pMonster->GetCamp() != emPersonCamp && pMonster->IsShow() == FALSE) || 
			pMonster->GetMonsterType() != emType || pMonster->GetCamp() != emCamp || 
			vMonsterList.at(i).GetDis(FixPoint) > fDis || CCharacter::wstrcmp_my(pMonster->GetName(),L"Test"))
		{
			vMonsterList.erase(vMonsterList.begin() + i--);
		}
	}

	sort(vMonsterList.begin(), vMonsterList.end());
	return vMonsterList.size();
}

int CGameFunction::GetAnemiaIndex(vector<CMonster>& vMonsterList)
{
	for (int i = 0; i < (int)vMonsterList.size(); ++i)
	{
		if (vMonsterList.at(i).IsAnemia())
		{
			return i;
		}
	}
	return -1;
}

BOOL CGameFunction::FindMonster_For_Name(__in LPCWSTR pwszName, __in float fDis,__out CMonster& Monster)
{
	vector<CMonster> vMonsterList;
	CMonster::GetAroundObjectList(vMonsterList);
	
	Point FixPoint = PERSON->GetPoint();
	for (UINT i = 0; i < vMonsterList.size(); ++i)
	{
		auto pMonster = &vMonsterList.at(i);
		if (pMonster->GetDis(FixPoint) < fDis && CCharacter::wstrcmp_my(pwszName, pMonster->GetName()))
		{
			Monster = *pMonster;
			return TRUE;
		}
	}

	return FALSE;
}

//////////////Soilder////////////////////////////////////////////////////////////
UINT CGameFunction::GetAroundEnemySoliderCount(float fDis, Point* pFixPoint)
{
	Point FixPoint;
	if (pFixPoint == NULL)
		FixPoint = PERSON->GetPoint();
	else
		FixPoint = *pFixPoint;

	vector<CMonster> vMonsterList;

	// 获取敌对阵营的小兵
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Soldier, PERSON->GetEnemyCamp(), fDis, pFixPoint);

	// 特殊过滤
	for (int i = 0;i < (int)vMonsterList.size(); ++i)
	{
		if (CCharacter::wstrcmp_my(vMonsterList.at(i).GetName(),L"OdinCenterRelic"))
		{
			vMonsterList.erase(vMonsterList.begin() + i--);
		}
	}

	return vMonsterList.size();
}

UINT CGameFunction::GetAroundAllianceSoliderCount(float fDis, Point* pFixPoint)
{
	Point FixPoint;
	if (pFixPoint == NULL)
		FixPoint = PERSON->GetPoint();
	else
		FixPoint = *pFixPoint;

	vector<CMonster> vMonsterList;

	// 获取同盟阵营的小兵
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Soldier, PERSON->GetCamp(), fDis, pFixPoint);

	return vMonsterList.size();
}

BOOL CGameFunction::GetRecentlyEnemySolider(float fDis, Point* pFixPoint, CMonster& Monster)
{
	static vector<CMonster> vMonsterList;

	// 获取敌对阵营的小兵
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Soldier, PERSON->GetEnemyCamp(), fDis, pFixPoint);
	if (vMonsterList.size() == 0)
		return FALSE;

	int nAnemiaIndex = GetAnemiaIndex(vMonsterList);
	if (nAnemiaIndex == -1)
	{
		// 发现范围内都木有贫血的小兵, 就返回最近的那个
		Monster = vMonsterList.at(0);
		return TRUE;
	}

	Monster = vMonsterList.at(nAnemiaIndex);
	return TRUE;
}

////////////Hero//////////////////////////////////////////////////////////////
UINT CGameFunction::GetAroundEnemyHeroCount(float fDis, Point* pFixPoint)
{
	static vector<CMonster> vMonsterList;
	// 获取敌对阵营的英雄
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), fDis, pFixPoint);
	return vMonsterList.size();
}

UINT CGameFunction::GetAroundAllianceHeroCount(float fDis, Point* pFixPoint)
{
	static vector<CMonster> vMonsterList;
	// 获取同盟阵营的英雄
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetCamp(), fDis, pFixPoint);
	// 因为遍历的时候去除自身, 所以……你懂的
	return vMonsterList.size();
}

BOOL CGameFunction::GetRecentlyEnemyHero(float fDis, Point* pFixPoint, CMonster& Monster)
{
	vector<CMonster> vMonsterList;

	// 获取敌对阵营的英雄
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), fDis, pFixPoint);
	if (vMonsterList.size() == 0)
		return FALSE;

	Monster = vMonsterList.at(0);
	return TRUE;

	///////不要用残血去判断, 否则经常站前面的不打///////////////////////////////////////////////////////////////////
	int nAnemiaIndex = GetAnemiaIndex(vMonsterList);
	if (nAnemiaIndex == -1)
	{
		// 发现范围内都木有贫血的英雄, 就返回最近的那个
		Monster = vMonsterList.at(0);
		return TRUE;
	}

	Monster = vMonsterList.at(nAnemiaIndex);
	return TRUE;
}

BOOL CGameFunction::IsDodgeHero()
{
	static vector<CMonster> vMonsterList;

	// 获取敌对阵营的英雄
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), MaxDis_Attack, NULL);
	if (vMonsterList.size() == 0)
		return FALSE;

	for (CMonster& Hero : vMonsterList)
	{
		// 血量高于40%,并且在追杀自己,那还是得退
		if (Hero.GetTargetId() == PERSON->GetId())
			return TRUE;

		/*if (Hero.GetPercentHp() > 50 && PERSON->GetDis(Hero.GetPoint()) <= 8)
		{
			CPrintLog::PrintLog_W(_SELF,__LINE__,L"HP高于50, 并且距离很近, 先退一下");
			return TRUE;
		}*/
	}

	return FALSE;
}

UINT CGameFunction::GetAttackSelfHeroCount(float fDis, Point* pFixPoint)
{
	vector<CMonster> vMonsterList;

	// 获取敌对阵营的英雄
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetEnemyCamp(), fDis, pFixPoint);

	UINT uAttackSlefHeroCount = 0;
	for (CMonster& Hero : vMonsterList)
	{
		if (Hero.GetTargetId() == PERSON->GetId())
			uAttackSlefHeroCount++;
	}

	return uAttackSlefHeroCount;
}

BOOL CGameFunction::IsAttackHero()
{
	// 附近根本木有英雄, 不攻击
	UINT uEnemyHeroCount = GetAroundEnemyHeroCount(g_fMaxDis_Attack, NULL);
	if (uEnemyHeroCount == 0)
		return FALSE;

	// 低于6级, 并非中路的情况下, 不攻击!
	//if (PERSON->GetLevel() < 6 && g_pAccountGame->MyAcount_Info.emHeroPro != em_Hero_Pro_Ryze)
		//return FALSE;

	// 他们人多比我们多…… 不要硬拼, 低调点
	/*UINT uAllianceHeroCount = GetAroundAllianceHeroCount(g_fMaxDis_Attack, NULL);
	if (uEnemyHeroCount > uAllianceHeroCount)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"他们人多比我们多…… 不要硬拼, 低调点,%d > %d", uEnemyHeroCount, uAllianceHeroCount);
		return FALSE;
	}*/

	// 把离我们最近的那个英雄找来
	CMonster EnemyHero;
	if (!GetRecentlyEnemyHero(g_fMaxDis_Attack, NULL, EnemyHero))
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"竟然找不到最近的英雄^");
		return FALSE;
	}

	// 血量还多着呢, 先不急, 先耗一下
	//if (EnemyHero.GetPercentHp() >= 50)
	//	return FALSE;


	// 如果有自己人, 而且人多比他们多，就不管一套技能冷却,直接冲上去就是干
	//if (uAllianceHeroCount > uEnemyHeroCount)
		//return TRUE;

	if (g_MapType == MapType_召唤师峡谷)
	{
		// 周围必须少于2个兵打自己(6级前 扛不起) (8级后最大可以抗5个兵)
		DWORD dwLevel = PERSON->GetLevel();
		UINT uEnemySolider = GetAttackSelfSoliderCount(MaxDis_Attack, NULL);

		if (dwLevel < 6 && uEnemySolider >= 3)
			return FALSE;
		else if (dwLevel >= 8 && uEnemySolider >= 5)
			return FALSE;
	}

	

	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"怒攻击英雄");
	return TRUE;
}

BOOL CGameFunction::IsCoolConsumeSkill(DWORD dwLevel)
{
	switch (g_pAccountGame->MyAcount_Info.emHeroPro)
	{
	case em_Hero_Pro_Ryze:
		return CSkill::GetSkillCool(em_Skill_Flag_W) || CSkill::GetSkillCool(em_Skill_Flag_E);
	case em_Hero_Pro_Ashe:
		return CSkill::GetSkillCool(em_Skill_Flag_W);
	case em_Hero_Pro_MissFortune: case em_Hero_Pro_Garen: case em_Hero_Pro_Swain: case em_Hero_Pro_Vayne: case em_Hero_Pro_Maokai: case em_Hero_Pro_Galio:
	case em_Hero_Pro_KogMaw:
		return CSkill::GetSkillCool(em_Skill_Flag_E) || CSkill::GetSkillCool(em_Skill_Flag_Q); 
	case em_Hero_Pro_MasterYi: case em_Hero_Pro_Heimerdinger:  case em_Hero_Pro_Corki: case em_Hero_Pro_Annie:
		return CSkill::GetSkillCool(em_Skill_Flag_Q);
	case em_Hero_Pro_Chogath: case em_Hero_Pro_Ezreal: case em_Hero_Pro_Graves: case em_Hero_Pro_Veigar: case em_Hero_Pro_Sejuani: case em_Hero_Pro_Lucian:
		return CSkill::GetSkillCool(em_Skill_Flag_Q) || CSkill::GetSkillCool(em_Skill_Flag_W);
	case em_Hero_Pro_Malzahar: case em_Hero_Pro_Nunu: case em_Hero_Pro_Tristana:
		return CSkill::GetSkillCool(em_Skill_Flag_E);
	default:
		break;
	}
	return FALSE;
}

BOOL CGameFunction::IsConsumeEnemyHero()
{
	// 6级以上 or 中路就不耗血
	if (g_MapType != MapType_召唤师峡谷)
		return FALSE;
	if (PERSON->GetLevel() >= 6)
		return FALSE;
	
	UINT uEnemyHeroCount = GetAroundEnemyHeroCount(MaxDis_Attack, NULL); 
	if (uEnemyHeroCount == 0)
		return FALSE;

	// 判断10m内是否有塔,有塔的情况下就不攻击 (特么不好判断技能是不是能否到达, 万一不小心走进去又挨塔揍了)
	if (GetAroundEnemyTurretCount(MaxDis_Attack, NULL) > 0)
		return FALSE;

	// 周围必须少于2个兵打自己(6级前 扛不起) (6级后最大可以抗5个兵)
	if (GetAttackSelfSoliderCount(MaxDis_Attack, NULL) >= 2)
		return FALSE;
	
	// 判断是否有一套技能可用
	//if (!IsCoolConsumeSkill(0))
		//return FALSE;

	if (PERSON->GetAttackDis() < 7.0f)
	{
		// 近战英雄不要随便上去, 判断附近的兵
		static CMonster NearestHero;
		if (GetRecentlyEnemyHero(MaxDis_Attack, NULL, NearestHero))
		{
			if (GetAroundEnemySoliderCount(7.0f, &NearestHero.GetPoint()) >= 5)
			{
				// 对方最近那个英雄附近有超过5个以上的兵, 先别去
				return FALSE;
			}
		}
	}

	// 如果英雄数量一样的, 就不怕，上去削点血量
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"耗一下敌方英雄的血");
	return TRUE;
}

UINT CGameFunction::GetAroundAllianceHeroCount_By_Level(float fDis, DWORD dwLevel)
{
	static vector<CMonster> vlst;
	GetMonsterList_By_Type(vlst, em_MonsterType_Hero, PERSON->GetCamp(), fDis, NULL);

	UINT uCount = 0;
	for (auto& Hero : vlst)
	{
		if (Hero.GetLevel() >= dwLevel)
			uCount++;
	}

	return uCount;
}

///////Solider///////////////////////////////////////////////////////////////////
BOOL CGameFunction::IsClearSolider()
{
	if (GetAroundEnemySoliderCount(MaxDis_Attack, NULL) > 0)
		return TRUE;

	return FALSE;
}

BOOL CGameFunction::IsDodgeSolider()
{
	static CMonster NearestTurret;
	static CMonster NearestSolider;
	// // 获取离自己最近的小兵 和 友方塔
	if (PERSON->GetLevel() >= 6 && GetRecentlyAllianceTurret(MaxDis_Attack, NULL, NearestTurret) && GetRecentlyEnemySolider(MaxDis_Attack, NULL, NearestSolider))
	{
		// 判断是否在塔下, 如果小兵在塔下, 那就不用管, 直接上去硬抗
		if (NearestTurret.GetDis(NearestSolider.GetPoint()) <= MaxDis_Attack)
			return FALSE;
	}

	// 判断等级, 8级以上能抗5个兵, 以下只能抗2个兵
	DWORD dwLevel = PERSON->GetLevel();
	UINT uAttackSelfSoldier = GetAttackSelfSoliderCount(MaxDis_Attack, NULL);

	if (dwLevel >= 8 && uAttackSelfSoldier >= 4)
		return TRUE;
	else if (dwLevel < 8 && uAttackSelfSoldier >= 2)
		return TRUE;

	return FALSE;
}

UINT CGameFunction::GetAttackSelfSoliderCount(float fDis, Point* pFixPoint)
{
	vector<CMonster> vMonsterList;

	// 获取敌对阵营的小兵
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Soldier, PERSON->GetEnemyCamp(), fDis, pFixPoint);
	
	UINT uAttackSlefSoliderCount = 0;
	for (auto Monster : vMonsterList)
	{
		if (Monster.GetTargetId() == PERSON->GetId())
			uAttackSlefSoliderCount++;
	}

	return uAttackSlefSoliderCount;
}

BOOL CGameFunction::DodgeEnemySolider()
{
	// 一直后退到小兵的攻击数量 < 2
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"躲避小兵的攻击");
	while (IsDodgeSolider() && GameStatus_Run && !EMPTY_PERSONHP)
	{
		BackOff();
		CGameSend::Sleep(WAITTIME);
		
	}

	// 停止后退
	return CGameSend::StopAction();
}

UINT CGameFunction::GetAroundEnemyGuardian(vector<CMonster>& vlst, float fDis, Point* pFixPoint)
{
	auto fnSoldier_by_Camp = [=,&vlst](em_Camp emCamp)
	{
		static vector<CMonster> vGuardLst;
		// 获取敌对阵营的塔
		GetMonsterList_By_Type(vGuardLst, em_MonsterType_Soldier, emCamp, fDis, pFixPoint);

		for_each(vGuardLst.begin(), vGuardLst.end(), [&vlst](CMonster& cmGuardian)
		{
			if (cmGuardian.GetMaxHp() == 50000 && CCharacter::wstrcmp_my(cmGuardian.GetName(), L"OdinNeutralGuardian"))
			{
				vlst.push_back(cmGuardian);
			}
		});
	};

	auto fnFindCenterRelic = [=, &vlst](em_Camp emCamp)
	{
		static vector<CMonster> vGuardLst;
		// 获取敌对阵营的塔
		GetMonsterList_By_Type(vGuardLst, em_MonsterType_Soldier, emCamp, fDis, pFixPoint);
		for_each(vGuardLst.begin(), vGuardLst.end(), [&vlst](CMonster& cmGrardian)
		{
			if (CCharacter::wstrcmp_my(cmGrardian.GetName(), L"OdinCenterRelic"))
				vlst.push_back(cmGrardian);
		});
	};
	
	vlst.clear();

	fnFindCenterRelic(PERSON->GetEnemyCamp());
	fnSoldier_by_Camp(PERSON->GetEnemyCamp());
	fnSoldier_by_Camp(em_Camp_Neutral);
	return vlst.size();
}

BOOL CGameFunction::OccupyEnemyTurret()
{
	while (GameStatus_Run && !EMPTY_PERSONHP && !IsBackHome())
	{
		static vector<CMonster> vGuardLst;
		if (GetAroundEnemyGuardian(vGuardLst, 10.0f, NULL) == NULL)
			break;

		// 3m之内有敌方小兵就放弃!
		//if (GetAroundEnemySoliderCount(3.0f, NULL) > 0)
		//{
			//CPrintLog::PrintLog_W(_SELF,__LINE__,L"3m之内有敌方小兵的存在!");
			//return FALSE;
		//}

		// 15m之内有敌方英雄
		if (GetAroundEnemyHeroCount(10.0f, NULL) > 0)
		{
			//CPrintLog::PrintLog_W(_SELF,__LINE__,L"10m之内有敌方英雄存在!");
			return FALSE;
		}

		CGameSend::StopAction();
		CMonster cmGuardian = vGuardLst.at(0);
		cmGuardian.Occupy();
		CGameSend::Sleep(1000);
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"正在占领……");
	}
	return FALSE;
}

/////////////Turret/////////////////////////////////////////////////////////////
Point CGameFunction::GetRecentlyTurret(int nIndex, em_Path_Flag emPathFlag)
{
	// 判断1塔-> 2塔 -> 高地塔 -> 基地
	em_Camp emCamp = PERSON->GetCamp();
	if (nIndex >= 3)
		return CGameRes::GetSpringPoint(emCamp);

	TurretSrt* pTurretArray = CGameRes::GetTurretArray();
	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"Array Index=%d", nIndex + (int)emPathFlag * 3 + (emCamp == em_Camp_Red ? 9 : 0));
	TurretSrt Turret = pTurretArray[nIndex + (int)emPathFlag * 3 + (emCamp == em_Camp_Red ? 9 : 0)];

	CMonster TmpTurret;
	if (!FindMonster_For_Name(Turret.wszTurretName, 1000.0f, TmpTurret) && GameStatus_Run && !EMPTY_PERSONHP)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"%s找不到,应该是这个塔被弄了,找下一个塔", Turret.wszTurretName);
		return GetRecentlyTurret(nIndex + 1, emPathFlag);
	}

	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"移动到%s塔下", Turret.wszTurretName);

	auto fn_GetPointType_For_Index = [](int nIndex)
	{
		switch (nIndex)
		{
		case 0:
			return PointType_1Turret;
			break;
		case 1:
			return PointType_2Turret;
			break;
		case 2:
			return PointType_HighTurret;
			break;
		default:
			break;
		}
		return PointType_HighTurret;
	};

	auto pResPoint = CGameRes::FindResPoint_For_Turret(emCamp, emPathFlag, fn_GetPointType_For_Index(nIndex));
	MsgBoxDebug_W(pResPoint != NULL, _SELF, __LINE__, L"无法在资源表找到塔后的坐标,nIndex=%d", nIndex);

	return pResPoint->Pit;
Function_Exit:;
	CPrintLog::PrintLog_W(_SELF,__LINE__,L"StopGame");
	StopGame;
	return Point();
}

BOOL CGameFunction::MoveToTurret(int nIndex, em_Path_Flag emPathFlag)
{
	// 一直往后退, 退到木有英雄的地方
	BackOff();
	static DWORD dwRunFastTick = 0;
	static DWORD dwAddHpTick = 0;
	while (GameStatus_Run && !EMPTY_PERSONHP && (GetAroundEnemyHeroCount(15.0f, NULL) != 0 || GetAroundEnemySoliderCount(10.0f, NULL) != 0) )
	{
		if (GetAroundEnemyHeroCount(15.0f, NULL) != 0)
		{
			if (::GetTickCount() - dwRunFastTick >= 210 * 1000 && PERSON->GetPercentHp() < 30)
			{
				// 使用幽灵疾步
				//RECT r;
				//::GetWindowRect(g_pAccountGame->hWndGame, &r);
				//CLPublic::SimulationMouse(r.left + 567, r.top + 722);

				CSkill::UseItemSkill(L"summonerhaste");
				dwRunFastTick = ::GetTickCount();
			}
			if (::GetTickCount() - dwAddHpTick >= 240 * 1000 && PERSON->GetPercentHp() < 30)
			{
				// 使用治疗术
				//RECT r;
				//::GetWindowRect(g_pAccountGame->hWndGame, &r);
				//CLPublic::SimulationMouse(r.left + 602, r.top + 722);

				CSkill::UseItemSkill(L"summonerheal");
				dwAddHpTick = ::GetTickCount();
			}
			
		}
		BackOff();
		CGameSend::Sleep(500);
	}
	CGameSend::Sleep(1000);
	return CGameSend::StopAction();
	//return MoveToPoint(GetRecentlyTurret(nIndex, emPathFlag), 2.0f);
}

BOOL CGameFunction::GetAroundEnemyTurretCount(float fDis, Point* pFixPoint)
{
	vector<CMonster> vMonsterList;

	// 获取敌对阵营的塔
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Turret, PERSON->GetEnemyCamp(), fDis, pFixPoint);

	return vMonsterList.size();
}

BOOL CGameFunction::GetRecentlyEnemyTurret(float fDis, Point* pFixPoint, CMonster& Monster)
{
	static vector<CMonster> vMonsterList;

	// 获取敌对阵营的塔
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Turret, PERSON->GetEnemyCamp(), fDis, pFixPoint);
	if (vMonsterList.size() == 0)
		return FALSE;

	Monster = vMonsterList.at(0);
	return TRUE;
}

BOOL CGameFunction::GetRecentlyAllianceTurret(float fDis, Point* pFixPoint, CMonster& Turret)
{
	static vector<CMonster> vMonsterList;
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Turret, PERSON->GetCamp(), fDis, pFixPoint);
	if (vMonsterList.size() == 0)
		return FALSE;

	Turret = vMonsterList.at(0);
	return TRUE;
}

BOOL CGameFunction::IsAttackTurret()
{
	// 获取敌对阵营的塔
	static vector<CMonster> vMonsterList;
	GetMonsterList_By_Type(vMonsterList, em_MonsterType_Turret, PERSON->GetEnemyCamp(), MaxDis_Attack, NULL);
	if (vMonsterList.size() == 0)
		return FALSE;

	UINT uAroundEnemyHeroCount = GetAroundEnemyHeroCount(MaxDis_Attack, NULL);

	for (CMonster& Turrent : vMonsterList)
	{
		// 如果是水晶塔, 并且周围有英雄 ,那么就不攻击塔
		if (Turrent.IsCrystalTurret() && uAroundEnemyHeroCount > 0)
			return FALSE;

		// 必须塔的附近有超过2个自己人的小兵
		if(GetAroundAllianceSoliderCount(MaxDis_Attack, &Turrent.GetPoint()) > 2)
			return TRUE;
	}
	return FALSE;
}

BOOL CGameFunction::ExistMiddleCrystalTurret()
{
	if (g_MapType != MapType_召唤师峡谷)
		return TRUE;

	static DWORD dwTick = 0;

	if (::GetTickCount() - dwTick >= 3 * 1000)
	{
		dwTick = ::GetTickCount();

		TurretSrt* pTurretSrt = CGameRes::GetCrystalTurretArray(PERSON->GetCamp(), em_Path_Flag_Middle);
		if (pTurretSrt != NULL)
		{
			CMonster MiddleCrystal;
			if (FindMonster_For_Name(pTurretSrt->wszTurretName, 1000.0f, MiddleCrystal))
				return TRUE;

			/*if (g_pAccountGame->MyAcount_Info.emPathFlag != em_Path_Flag_Middle)
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"中路水晶塔没了……所有人集合中路");
				g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Middle;
				BackHome();
			}*/
			return FALSE;
		}

	}

	return TRUE;
}

BOOL CGameFunction::SetMatchingPath()
{
	if (g_MapType != MapType_召唤师峡谷)
		return FALSE;

	auto fn_GetHeroCount_By_Turret = [=](em_Path_Flag emPathFlag)
	{
		// 获取自己阵营的2塔的坐标 60m
		Point TopTurret = GetRecentlyTurret(0, emPathFlag);

		// 获取所有已方阵营的英雄
		static vector<CMonster> vMonsterList;
		GetMonsterList_By_Type(vMonsterList, em_MonsterType_Hero, PERSON->GetCamp(), 1000.0f, NULL);

		int nHeroCount = 0;
		for_each(vMonsterList.begin(), vMonsterList.end(), [&nHeroCount, &TopTurret](CMonster& Hero){
			if (Hero.GetDis(TopTurret) < 30.0f) // 当已方英雄与外塔的距离<50m的时候, 就视为在某路上
				nHeroCount++;
		});

		return nHeroCount;
	};

	auto fn_SetEmptyPath = [](int nCount, em_Path_Flag emPathFlag)
	{
		if (nCount == 0)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"%X路木有人", emPathFlag);
			g_pAccountGame->MyAcount_Info.emPathFlag = emPathFlag;
			return TRUE;
		}
		return FALSE;
	};

	int nHeroCount_TopPath = fn_GetHeroCount_By_Turret(em_Path_Flag_Top);
	int nHeroCount_MiddlePath = fn_GetHeroCount_By_Turret(em_Path_Flag_Middle);
	int nHeroCount_ButtomPath = fn_GetHeroCount_By_Turret(em_Path_Flag_Buttom);

	// 判断哪条路没人, 就去哪条路
	if (fn_SetEmptyPath(nHeroCount_MiddlePath, em_Path_Flag_Middle) || fn_SetEmptyPath(nHeroCount_TopPath, em_Path_Flag_Top) || fn_SetEmptyPath(nHeroCount_ButtomPath, em_Path_Flag_Buttom))
		return TRUE;

	// 上路少于2人的时候, 就去上路凑数
	if (nHeroCount_TopPath < 2)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"上路少于2人的时候, 就去下路凑数");
		g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Top;
		return TRUE;
	}

	// 下路少于2人的时候, 就去下路凑数
	if (nHeroCount_MiddlePath < 2)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"下路少于2人的时候, 就去下路凑数");
		g_pAccountGame->MyAcount_Info.emPathFlag = em_Path_Flag_Buttom;
		return TRUE;
	}

	return TRUE;
}

BOOL CGameFunction::SetGamePath()
{
	//g_MapType
	CMonster Monster;
	if (!FindMonster_For_Name(L"HQ_T1", 1000.0f, Monster))
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"水晶之痕");
		g_MapType = MapType_水晶之痕;
	}
	else
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"召唤师峡谷");
		g_MapType = MapType_召唤师峡谷;
	}
	return TRUE;
}