#include "stdafx.h"
#include "GameSend.h"
#pragma warning(disable:4731)
#define _SELF L"GameSend.cpp"
CGameSend::CGameSend()
{
	
}

CGameSend::~CGameSend()
{

}

BOOL CGameSend::GameTalk(LPCWSTR pwszText)
{
	static GameTalkMsg GTM;
	ZeroMemory(&GTM, sizeof(GTM));

	CCharacter::wstrcpy_my(GTM.wszMsg, pwszText);
	//g_vGameTalkMsg.push_back(GTM);
	return TRUE;
}

float CGameSend::Get3DDistance(const Point * LParm, const Point * RParm)
{
	__try
	{
		float Width = (float)sqrt((float)(LParm->X - RParm->X)*(LParm->X - RParm->X) + (float)((LParm->Y - RParm->Y)*(LParm->Y - RParm->Y)));
		float ZZ = (float)(LParm->Z - RParm->Z);
		return (float)sqrt(ZZ*ZZ + Width*Width) / 100;
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"Get3DDistance出现异常");
	}
	return 0;
}

float CGameSend::Get2DDistance(const Point * LParm, const Point * RParm)
{
	return  (float)sqrt((float)(LParm->X - RParm->X)*(LParm->X - RParm->X) + (float)((LParm->Y - RParm->Y)*(LParm->Y - RParm->Y))) / 100;
}

VOID CGameSend::Sleep(DWORD dwSleepTime)
{
	if (dwSleepTime <= 100)
	{
		::Sleep(dwSleepTime);
	}

	for (DWORD i = 0; i <= dwSleepTime && GameStatus_Run; i += 100)
	{
		::Sleep(100);
	}
}

BOOL CGameSend::Move(Point& EndPoint)
{
	g_bfnMove = TRUE;
	g_MovePoint = EndPoint;
	g_bMove = TRUE;

	for (int i = 0; i < 10 && GameStatus_Run && g_bMove; ++i)
		CGameSend::Sleep(100);

	return TRUE;
}

BOOL CGameSend::StopAction()
{
	if (!g_bfnMove)
		return FALSE;

	g_bfnMove = FALSE;
	g_bStopAction = TRUE;
	for (int i = 0; i < 10 && GameStatus_Run && g_bStopAction; ++i)
		CGameSend::Sleep(100);

	return TRUE;
}

DWORD CGameSend::GetStartTime()
{
	return (DWORD)ReadFloat(ReadDWORD(游戏开场时间基址) + 0x2C);
}

BOOL CGameSend::BuyItem(DWORD dwItemId)
{
	g_dwBuyItemId = dwItemId;
	for (int i = 0; i < 10 && GameStatus_Run && g_dwBuyItemId != NULL; ++i)
		CGameSend::Sleep(100);

	g_dwBuyItemId = NULL;
	return TRUE; 
}

BOOL CGameSend::LockView()
{
	DWORD dwAddr = ReadDWORD(C_look_base) + 0x20;
	*(BYTE*)dwAddr = 0x1;
	return TRUE;
}

DWORD CGameSend::GetSelfHp()
{
	return ReadDWORD(统治_SelfHpBase);
}

DWORD CGameSend::GetEnemyHp()
{
	return ReadDWORD(统治_EnemyHpBase);
}

DWORD CGameSend::ReadD(DWORD dwAddr)
{
	return *(DWORD*)dwAddr;
}

BYTE CGameSend::ReadB(DWORD dwAddr)
{
	return *(BYTE*)dwAddr;
}

float CGameSend::ReadF(DWORD dwAddr)
{
	return *(float*)dwAddr;
}

VOID WINAPI CALL_StudySkill(DWORD(&dwFakeStackFrame)[260])
{
	__try
	{
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"StudySkill:%d", g_dwStudySkillIndex);
		_asm
		{
			/*nop
				nop
				nop
				push ebp
				mov ebp, esp
				pushad
				jmp __pushfuncret
			__thunk :*/

				push g_dwStudySkillIndex
				mov ecx, Role_base
				mov ecx, [ecx]
				mov eax, C_CALL_Study_skill
				add eax, 0x6
				call eax
				/*push C_CALL_retn //必须要的 游戏内的retn地址
				mov ebp, [ebp + 8] //开始伪造

				jmp eax
				jmp __funcret
			__pushfuncret :
			call __thunk
			__funcret :
			//如果要平衡堆栈在这里
			popad
				mov esp, ebp
				pop ebp*/
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CALL_StudySkill发生了异常");
	}
}

VOID WINAPI CALL_BuyItem(DWORD(&dwFakeStackFrame)[260])
{
	__try
	{
		_asm
		{
			/*NOP
				NOP
				NOP
				PUSH EBP
				MOV EBP, ESP
				PUSHAD
				JMP __pushfuncret
			__thunk :*/

			PUSH g_dwBuyItemId
				MOV EAX, 人物基址
				MOV EAX, DWORD PTR DS : [EAX]
				LEA ECX, DWORD PTR DS : [EAX + 人物装备偏移]

				MOV EAX, 购买物品CALL
				CALL EAX

				/*PUSH C_CALL_retn//必须要的 游戏内的retn地址
				MOV EBP, DWORD PTR DS : [EBP + 8]	//开始伪造
				JMP EAX
				JMP __funcret
			__pushfuncret :
			CALL __thunk
			__funcret :
			//如果要平衡堆栈在这里
			POPAD
				MOV ESP, EBP
				POP EBP*/
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CALL_BuyItem发生了异常");
	}
}

VOID WINAPI CALL_Move(DWORD(&dwFakeStackFrame)[260])
{
	__try
	{
		static float fArray[6];
		fArray[0] = g_MovePoint.X;
		fArray[1] = g_MovePoint.Z;
		fArray[2] = g_MovePoint.Y;
		fArray[3] = g_MovePoint.X;
		fArray[4] = g_MovePoint.Z;
		fArray[5] = g_MovePoint.Y;

		_asm
		{
			/*NOP
				NOP
				NOP
				PUSH EBP
				MOV EBP, ESP
				PUSHAD
				JMP __pushfuncret
			__thunk :*/

			PUSH 1
				PUSH 0
				PUSH 0
				PUSH 0
				LEA EAX, fArray
				PUSH EAX
				PUSH 2
				MOV ECX, 人物基址
				MOV ECX, DWORD PTR DS : [ECX]
				MOV EAX, 走路CALL
				CALL EAX
				/*PUSH C_CALL_retn//必须要的 游戏内的retn地址
				MOV EBP, DWORD PTR DS : [EBP + 8]	//开始伪造
				JMP EAX
				JMP __funcret
			__pushfuncret :
			CALL __thunk
			__funcret :
			//如果要平衡堆栈在这里
			POPAD
				MOV ESP, EBP
				POP EBP*/
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CALL_Move发生了异常");
	}
}

VOID WINAPI CALL_Attack(DWORD(&dwFakeStackFrame)[260])
{
	__try
	{
		static float fArray[3];
		fArray[0] = g_AttackPoint.X;
		fArray[1] = g_AttackPoint.Z;
		fArray[2] = g_AttackPoint.Y;

		_asm
		{
			/*NOP
				NOP
				NOP
				PUSH EBP
				MOV EBP, ESP
				PUSHAD
				JMP __pushfuncret
			__thunk :*/

			PUSH 1
				PUSH 0
				PUSH 0
				PUSH g_dwNormalAttackBase
				LEA EAX, fArray
				PUSH EAX
				PUSH 3
				MOV ECX, 人物基址
				MOV ECX, DWORD PTR DS : [ECX]
				MOV EAX, 普通攻击CALL
				CALL EAX
				/*PUSH C_CALL_retn//必须要的 游戏内的retn地址
				MOV EBP, DWORD PTR DS : [EBP + 8]	//开始伪造
				JMP EAX
				JMP __funcret
			__pushfuncret :
			CALL __thunk
			__funcret :
			//如果要平衡堆栈在这里
			POPAD
				MOV ESP, EBP
				POP EBP*/
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CALL_Attack发生了异常");
	}
}

VOID WINAPI CALL_Stop(DWORD(&dwFakeStackFrame)[260])
{
	__try
	{
		_asm
		{
			/*NOP
				NOP
				NOP
				PUSH EBP
				MOV EBP, ESP
				PUSHAD
				JMP __pushfuncret
			__thunk :*/

			PUSH 1
				PUSH 0
				PUSH 0
				PUSH 0
				MOV ECX, 人物基址
				MOV ECX, DWORD PTR DS : [ECX]
				LEA EAX, DWORD PTR DS : [ECX + 怪物X坐标偏移]
				PUSH EAX
				PUSH 0xA
				MOV EAX, 普通攻击CALL
				CALL EAX
				/*PUSH C_CALL_retn//必须要的 游戏内的retn地址
				MOV EBP, DWORD PTR DS : [EBP + 8]	//开始伪造
				JMP EAX
				JMP __funcret
			__pushfuncret :
			CALL __thunk
			__funcret :
			//如果要平衡堆栈在这里
			POPAD
				MOV ESP, EBP
				POP EBP*/
			
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CALL_Stop发生了异常");
	}
}

VOID WINAPI CALL_UseSkill(DWORD(&dwFakeStackFrame)[260])
{
	__try
	{
		//g_Hook_UseSkillInfo
		static float fNowArray[3];
		static float fTarArray[3];
		static DWORD dwId;
		static DWORD dwIndex;
		static DWORD dwSkillBase;

		fNowArray[0] = g_Hook_UseSkillInfo.NowPoint.X;
		fNowArray[1] = g_Hook_UseSkillInfo.NowPoint.Z;
		fNowArray[2] = g_Hook_UseSkillInfo.NowPoint.Y;

		fTarArray[0] = g_Hook_UseSkillInfo.TarPoint.X;
		fTarArray[1] = g_Hook_UseSkillInfo.TarPoint.Z;
		fTarArray[2] = g_Hook_UseSkillInfo.TarPoint.Y;

		dwId = g_Hook_UseSkillInfo.dwId;
		dwIndex = g_Hook_UseSkillInfo.dwIndex;
		dwSkillBase = g_Hook_UseSkillInfo.dwSkillBase;

		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"dwId=%X,index=%d,Base=%d", g_Hook_UseSkillInfo.dwId, g_Hook_UseSkillInfo.dwIndex, g_Hook_UseSkillInfo.dwSkillBase);
		_asm
		{
			/*NOP
				NOP
				NOP
				PUSH EBP
				MOV EBP, ESP
				PUSHAD
				JMP __pushfuncret
			__thunk :*/

			MOV EAX, 人物基址
				MOV EAX, DWORD PTR DS : [EAX]
				LEA ECX, DWORD PTR DS : [EAX + 人物技能偏移]

				PUSH dwId

				LEA EAX, fNowArray
				PUSH EAX

				LEA EAX, fTarArray
				PUSH EAX

				PUSH dwIndex
				PUSH dwSkillBase

				MOV EAX, 使用技能CALL
				CALL EAX
				/*PUSH C_CALL_retn//必须要的 游戏内的retn地址
				MOV EBP, DWORD PTR DS : [EBP + 8]	//开始伪造
				JMP EAX
				JMP __funcret
			__pushfuncret :
			CALL __thunk
			__funcret :
			//如果要平衡堆栈在这里
			POPAD
				MOV ESP, EBP
				POP EBP*/
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CALL_UseSkill发生了异常");
	}
}

DWORD CALL_GetMonsterName(DWORD dwBase)
{
	__try
	{
		DWORD dwResul = 0;
		_asm
		{
			MOV ECX, dwBase
				MOV EAX, 怪物名字CALL
				CALL EAX
				MOV dwResul, EAX
		}
		return dwResul;
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CALL_GetMonsterName发生了异常");
	}
	return 0;
}

VOID WINAPI CALL_UseItem(DWORD(&dwFakeStackFrame)[260])
{
	__try
	{
		_asm
		{
			/*NOP
				NOP
				NOP
				PUSH EBP
				MOV EBP, ESP
				PUSHAD
				JMP __pushfuncret
			__thunk :*/

			MOV ECX, UseItemBase
				MOV ECX, DWORD PTR DS : [ECX]
				MOV ECX, DWORD PTR DS : [ECX + UseItemOffset]
				PUSH 0
				PUSH g_dwUseItemParm
				MOV EAX, UseItemCALL
				CALL EAX
				/*PUSH C_CALL_retn//必须要的 游戏内的retn地址
				MOV EBP, DWORD PTR DS : [EBP + 8]	//开始伪造
				JMP EAX
				JMP __funcret
			__pushfuncret :
			CALL __thunk
			__funcret :
			//如果要平衡堆栈在这里
			POPAD
				MOV ESP, EBP
				POP EBP*/
			
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF,__LINE__,L"CALL_UseItem发生了异常");
	}
}

VOID WINAPI CALL_Occupy(DWORD(&dwFakeStackFrame)[260])
{
	__try
	{
		_asm
		{
			/*NOP
				NOP
				NOP
				PUSH EBP
				MOV EBP, ESP
				PUSHAD
				JMP __pushfuncret
			__thunk :*/

			MOV EAX, 人物基址
				MOV EAX, DWORD PTR DS : [EAX]
				LEA ECX, DWORD PTR DS : [EAX + 统治占领偏移]
				PUSH g_dwOccupyObjBase
				MOV EAX, 统治占领CALL
				CALL EAX
				/*PUSH C_CALL_retn//必须要的 游戏内的retn地址
				MOV EBP, DWORD PTR DS : [EBP + 8]	//开始伪造
				JMP EAX
				JMP __funcret
			__pushfuncret :
			CALL __thunk
			__funcret :
			//如果要平衡堆栈在这里
			POPAD
				MOV ESP, EBP
				POP EBP*/

		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"CALL_Occupy发生了异常");
	}
}