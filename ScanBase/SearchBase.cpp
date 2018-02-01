#include "stdafx.h"
#include "SearchBase.h"
#include "../../MyTools/MyTools/CLHead.h"

#define _SELF L"SearchBase.cpp"

CSearchBase::CSearchBase()
{

}

CSearchBase::~CSearchBase()
{

}

BOOL CSearchBase::CompCode(const DWORD * pCode, const BYTE * pMem, UINT uLen)
{
	BOOL bComp = TRUE;
	for (UINT i = 0;i < uLen; ++i)
	{
		if ( pCode[i] != 0x100 && (BYTE)pCode[i] != (BYTE)pMem[i] )
		{
			bComp = FALSE;
			break;
		}
	}

	return bComp;
}

BOOL CSearchBase::SearchBase(LPCSTR szCode,DWORD * pArray,UINT& puLen, LPCWSTR lpszModule)
{
	SYSTEM_INFO		si;
	MEMORY_BASIC_INFORMATION		mbi;

	BOOL	bRetCode	= FALSE;

	//将字符串转换成BYTE数组
	UINT uCodeLen = strlen(szCode) / 2;
	if ( strlen(szCode) % 2 != 0 )
	{
		CPrintLog::PrintLog_A("",__LINE__,"szCode=%s,len=%d,CodeLen=%d",szCode,strlen(szCode),uCodeLen);
		AfxMessageBox(L"必须是2的倍数!");
		return FALSE;
	}

	int nCount = 0;
	DWORD * pCode = new DWORD[uCodeLen];
	memset(pCode,0,uCodeLen);

	for (UINT i = 0;i < uCodeLen; ++i)
	{
		if ( szCode[i * 2] != '?')
		{
			char szText[] = {szCode[i * 2],szCode[i * 2 + 1],'\0'};
			pCode[i] = (DWORD)strtol(szText,NULL,16);
		}
		else
		{
			pCode[i] = 0x100;
		}
	}

	//初始化
	::GetSystemInfo(&si);
	HANDLE hProcess = ::GetCurrentProcess();
	DWORD dwImageBase = (DWORD)::GetModuleHandleW(lpszModule);
	DWORD dwImageSize = GetImageSize((HMODULE)dwImageBase);
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"搜索的模块是:%s,模块起始地址是:%X,大小是:%X",lpszModule,dwImageBase,dwImageSize);

	for (DWORD dwAddr = dwImageBase; dwAddr < dwImageBase + dwImageSize; dwAddr += mbi.RegionSize)
	//for (BYTE * pCurPos = (LPBYTE)si.lpMinimumApplicationAddress; pCurPos < (LPBYTE)si.lpMaximumApplicationAddress - uCodeLen; pCurPos = (PBYTE)mbi.BaseAddress + mbi.RegionSize,nCount++ )
	{
		//查询当前内存页的属性
		::VirtualQueryEx(hProcess, (LPCVOID)dwAddr, &mbi, sizeof(mbi));
		if ( mbi.Protect == PAGE_READONLY )//扫描只读内存
		{
			DWORD dwOldProtect;
			::VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READ,&dwOldProtect);
		}
		//CPrintLog::PrintLog_W(_SELF,__LINE__,L"mbi.Protect=%X,mbi.BaseAddress=%X,mbi.RegionSize=%X,State=%X",mbi.Protect,mbi.BaseAddress,mbi.RegionSize,mbi.State);
		if ( mbi.State == MEM_COMMIT && (mbi.Protect == PAGE_EXECUTE_READ || mbi.Protect == PAGE_EXECUTE_READWRITE ) )
		{
			vector<int> vlst;
			CL_sunday(pCode, uCodeLen, (PBYTE)mbi.BaseAddress, mbi.RegionSize, vlst);
			//if (vlst.size() != 0)
				//CPrintLog::PrintLog_W(_SELF,__LINE__,L"vlst.size()=%d",vlst.size());
			for (UINT i = 0; i < vlst.size() && puLen < 10; ++i)
			{
				pArray[puLen] = (DWORD)mbi.BaseAddress + vlst.at(i);
				++puLen;
			}
			
		}
	}

	if ( puLen >= 10 )
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"puLen=%d", puLen);
		bRetCode = TRUE;
	}
	else if ( puLen < 10 && puLen > 0 )
	{
		bRetCode = TRUE;
	}
	delete [] pCode;
	//CPrintLog::PrintLog_W(_SELF,__LINE__,L"搜索到%d个结果",puLen);
	return bRetCode;
}

DWORD CSearchBase::GetImageSize(HMODULE hm)
{
	DWORD dwSize = 0x0;
	_asm
	{
		PUSHAD
		MOV EBX,DWORD PTR hm
		MOV EAX,DWORD PTR DS:[EBX + 0x3C]
		LEA EAX,DWORD PTR DS:[EBX + EAX + 0x50]
		MOV EAX,DWORD PTR DS:[EAX]
		MOV DWORD PTR DS:[dwSize],EAX
		POPAD
	}
	return dwSize;
}

DWORD CSearchBase::Read(DWORD dwAddr)
{
	if (dwAddr != 0x0 && !IsBadCodePtr(FARPROC(dwAddr)))//是否可读的
		return *(DWORD*)dwAddr;

	return 0;
}

DWORD CSearchBase::FindOffset(LPCSTR lpszCode, int nOffset, int nMov, int nOrderNum, LPCWSTR lpszModule, DWORD dwAddrLen /* = 0xFFFFFFFF */)
{
	DWORD	dwArray[10] = { 0 };
	UINT	uArrayLen = 0x0;
	DWORD	dwBase = 0x0;
	DWORD	dwAddr = 0x0;

	//开始搜索基址
	if (SearchBase(lpszCode, dwArray, uArrayLen, lpszModule))
	{
		if (uArrayLen == 1)//判断只有一个的时候
			dwAddr = dwArray[0];
		else
			dwAddr = dwArray[nOrderNum];
	}

	if (dwAddr != 0x0)
	{
		if (nOffset >= 0)
			dwAddr -= abs(nOffset);
		else
			dwAddr += abs(nOffset);

		dwAddr += nMov;
		dwBase = Read(dwAddr)&dwAddrLen;
	}

	return dwBase;
}

DWORD CSearchBase::FindBase(LPCSTR lpszCode,int nOffset,int nMov, int nOrderNum, LPCWSTR lpszModule, DWORD dwAddrLen)
{
	DWORD	dwArray[10] = {0};
	UINT	uArrayLen	= 0x0;
	DWORD	dwBase		= 0x0;
	DWORD	dwAddr		= 0x0;

	//开始搜索基址
	if ( SearchBase(lpszCode,dwArray,uArrayLen,lpszModule) )
	{
		if ( uArrayLen == 1)//判断只有一个的时候
			dwAddr = dwArray[0];
		else
			dwAddr = dwArray[nOrderNum];
	}

	if ( dwAddr != 0x0 )
	{
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"dwAddr=%X", dwAddr);
		if ( nOffset >= 0 )
			dwAddr -= abs(nOffset);
		else
			dwAddr += abs(nOffset);

		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"dwAddr=%X", dwAddr);
		dwAddr += nMov;
		//CPrintLog::PrintLog_W(_SELF, __LINE__, L"dwAddr=%X", dwAddr);
		dwBase = Read(dwAddr)&dwAddrLen;
	}

	return dwBase;
}

DWORD CSearchBase::FindCALL(LPCSTR lpszCode,int nOffset,DWORD dwModuleAddr, int nMov, int nOrderNum, LPCWSTR lpszModule)
{
	DWORD	dwArray[10] = {0};
	UINT	uArrayLen	= 0x0;
	DWORD	dwCALL		= 0x0;
	DWORD	dwAddr		= 0x0;

	//开始搜索基址
	if ( SearchBase(lpszCode,dwArray,uArrayLen,lpszModule) )
	{
		if ( uArrayLen == 1)//判断只有一个的时候
			dwAddr = dwArray[0];
		else
			dwAddr = dwArray[nOrderNum];
	}

	if ( dwAddr != 0x0 )
	{
		if ( nOffset >= 0 )
			dwAddr -= abs(nOffset);
		else
			dwAddr += abs(nOffset);

		//首先计算相对地址
		DWORD dwRelativeAddr = dwAddr - (dwModuleAddr + 0x1000) + 0x1000 + nMov;
		dwRelativeAddr += dwModuleAddr;
		DWORD dwReadAddr = Read(dwRelativeAddr);
		dwReadAddr += 4;
		dwReadAddr += dwRelativeAddr;
		dwCALL = dwReadAddr&0xFFFFFFFF;
	}

	return dwCALL;
}

DWORD CSearchBase::FindAddr(LPCSTR lpszCode, int nOffset, int nOrderNum, LPCWSTR lpszModule)
{
	DWORD	dwArray[10] = {0};
	UINT	uArrayLen	= 0x0;
	DWORD	dwAddr		= 0x0;

	//开始搜索基址
	if ( SearchBase(lpszCode,dwArray,uArrayLen,lpszModule) )
	{
		if ( uArrayLen == 1)//判断只有一个的时候
			dwAddr = dwArray[0];
		else
			dwAddr = dwArray[nOrderNum];
	}

	if ( dwAddr != 0x0 ){
		if ( nOffset >= 0 )
			dwAddr -= abs(nOffset);
		else
			dwAddr += abs(nOffset);
	}


	return dwAddr;
}

BOOL CSearchBase::FindBnsKoreaBase()
{
	DWORD dwBase = 0x0;
	DWORD dwCALL = 0x0;
	DWORD dwAddr = 0x0;
	DWORD dwClient = (DWORD)::GetModuleHandleW(L"League of Legends.exe");

	/************************************************************************/
	/* 第二个参数的位置的第一个是特征码的地址,第二个是对应Base或者CALL的地址                                                                     */
	/************************************************************************/

	DWORD dwStartTick = ::GetTickCount();
	dwBase = FindBase("0F84????????807F14000F84", 0xB6CB20 - 0x00B6CB69, 1, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"Role_base=%X", dwBase);

	dwBase = FindBase("85??74??8B??E8????????84C074??3C04", 0x00B656E7 - 0x00B656DC, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"Role_Skill_offset=%X", dwBase);

	dwBase = FindBase("83C41483FE3F77??8B84B7????00005F5E", 0xBF9970 - 0xBF9978, 3, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"Role_Skill_offset1=%X", dwBase);

	dwBase = FindBase("8B0D????????85C974098B01FF5020", 0x0100212C - 0x0100212C, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"Role_Skill_time=%X", dwBase);

	dwBase = FindBase("F30FC2C2010F28CB", 0x00B8DDE9 - 0x00B8DDC0, 4, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"yingxong_money=%X", dwBase);

	dwBase = FindBase("C1F902C1F8023BC1", 0x0056ADE1 - 0x0056ADD9, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"Minion_base=%X", dwBase);

	dwBase = FindCALL("74??33C0384510", 0x0DCE94F - 0x00DCE945,dwClient, 1, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_CALL_Func=%X", dwBase);

	dwBase = FindCALL("0F84????????807F14000F84", 0x00B6CB20 - 0x00B6CB75, dwClient,1, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_CALL_buy=%X", dwBase);

	dwBase = FindCALL("85??74??8B??E8????????84C074??3C04", 0x00B656E7 - 0x00B656E2, dwClient, 1, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_CALL_use_Skill=%X", dwBase);

	dwBase = FindCALL("6A0385??74??8b", 0x00596CD6 - 0x00596CEC, dwClient, 1, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_CALL_Study_skill=%X", dwBase);

	dwBase = FindBase("EB??83??0274??83??0374??83??0474", 0xE94D2A - 0xE94D03, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_D3D_BASE=%X", dwBase);

	dwBase = FindBase("8A??2084C00F84????????A1", 0x00DC01E7 - 0x00DC01D3, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_look_base=%X", dwBase);

	dwBase = FindBase("0F84????????8D47088D04", 0x0056D99A - 0x0056D986, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_Message_base=%X", dwBase);

	dwBase = FindCALL("C1E80CA8017411e8", 0x00ACD1FA - 0x00ACD201, dwClient, 1, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_CALL_GetName=%X", dwBase);

	dwBase = FindBase("84C00F84????????83??????0000008D", 0xC7CD39 - 0xC7CD1A, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_yeguai_base=%X", dwBase);

	dwBase = FindBase("6A00E8????????8B0D????????85??7405", 0xBA051E - 0xBA0525, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"BuffBase=%X", dwBase);

	dwBase = FindBase("83C1068B????83F90D", 0x00A83070 - 0x00A83068, 1, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"UseItemBase=%X", dwBase);

	dwBase = FindBase("83C1068B????83F90D", 0x00A83070 - 0x00A83073, 2, 0, Lol_China_Clinet,0xFF);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"UseItemOffset=%X", dwBase);

	dwBase = FindAddr("3c080f85????????8b450c85", 0x0A81475 - 0x00A81440, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"UseItemCALL=%X", dwBase);

	dwBase = FindBase("72090F2F05????????7305", 0x0A62782 - 0x00A62796, 2, 0, Lol_China_Clinet, 0xFF);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"怪物迷雾对象偏移=%X", dwBase);

	dwBase = FindBase("C1E80DF30F1145F8", 0x00933663 - 0x0933618, 2, 0, Lol_China_Clinet, 0xFF);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"怪物迷雾偏移1=%X", dwBase);

	dwBase = FindBase("C1E80DF30F1145F8", 0x00933663 - 0x009336AA, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"怪物迷雾偏移2=%X", dwBase);

	dwBase = FindBase("FFD084C074188B??8B", 0x0B430F8 - 0x00B4310C, 2, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"HeroLevel_Offset=%X", dwBase);

	dwBase = FindAddr("85DB74418BBD", 0x00D606D2 - 0x00D60732, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_CALL_retn=%X", dwBase);

	dwBase = FindAddr("85DB74418BBD", 0x00D606D2 - 0x00D6072A, 0, Lol_China_Clinet);
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"C_CALL_mid=%X", dwBase);


	DWORD dwEndTick = ::GetTickCount();
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"花费%dms", dwEndTick - dwStartTick);

	return TRUE;
}

int CSearchBase::GetWord_By_Char(BYTE dwWord, DWORD* pKey, UINT uKeyLen)
{
	int uLen = uKeyLen - 1;
	for (int i = uLen; i >= 0; --i)
	{
		if (pKey[i] == 0x100 || (BYTE)pKey[i] == dwWord)
		{
			return uKeyLen - i;
		}
	}
	return -1;
}

BOOL CSearchBase::CL_sunday(DWORD* pKey, UINT uKeyLen, BYTE* pCode, UINT uCodeLen, vector<int>& vlst)
{
	//807E1000740E
	UINT uNowPos = 0;
	while (uNowPos + uKeyLen < uCodeLen)
	{
		if (CompCode(pKey,pCode + uNowPos,uKeyLen))
		{
			vlst.push_back(uNowPos);
			uNowPos += uKeyLen + 1;
			continue;
		}

		BYTE bWord = pCode[uKeyLen + uNowPos];

		int nWordPos = GetWord_By_Char(bWord, pKey, uKeyLen);
		if (nWordPos == -1)
			uNowPos += uKeyLen + 1;
		else
			uNowPos += (UINT)nWordPos;
	}

	return TRUE;
}