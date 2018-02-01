#include "stdafx.h"
#include "HookCode.h"
#include "GameSend.h"

#define _SELF L"HookCode.cpp"
static DWORD gs_dwThreadId;

__declspec(naked) VOID WINAPI HookKeyDownPrint()
{
	__asm PUSHAD

	gs_dwThreadId = ::GetCurrentThreadId();
	if (gs_dwThreadId != g_dwMainThreadId)
		goto lb_Exit;


lb_Exit:;
	__asm POPAD
	_asm
	{
		MOV EAX, g_dwHookSimKeyCALL
			CALL EAX
			PUSH g_dwHookSimKeyAddr
			RET
	}
}