#include "stdafx.h"
#include "PersonBuff.h"

CPersonBuff::CPersonBuff()
{
	m_dwBase = NULL;
	ZeroMemory(wszName, sizeof(wszName));
}

CPersonBuff::~CPersonBuff()
{
}

CPersonBuff::CPersonBuff(DWORD dwBase)
{
	m_dwBase = dwBase;
	ZeroMemory(wszName, sizeof(wszName));
}

UINT CPersonBuff::GetPersonBuffLst(vector<CPersonBuff>& vlst)
{
	//dd[[154E03C] + 10]
	vlst.clear();
	DWORD dwArrayAddr = ReadDWORD(ReadDWORD(BuffBase) + 0x10);

	for (int i = 0; i < 0x40; ++i)
	{
		DWORD dwBuffAddr = dwArrayAddr + i * 0x34;
		if (ReadDWORD(dwBuffAddr + 0x2C) != 0xFFFFFFFF)
		{
			vlst.push_back(CPersonBuff(dwBuffAddr));
		}
	}

	return vlst.size();
}

BOOL CPersonBuff::ExistBuff(DWORD dwBuffId)
{
	static vector<CPersonBuff> vlst;
	GetPersonBuffLst(vlst);

	for (auto& Pb : vlst)
	{
		if (Pb.GetId() == dwBuffId)
			return TRUE;
	}

	return FALSE;
}

DWORD CPersonBuff::GetBuffCount(DWORD dwBuffId)
{
	static vector<CPersonBuff> vlst;
	GetPersonBuffLst(vlst);

	for (auto& Pb : vlst)
	{
		if (Pb.GetId() == dwBuffId)
			return Pb.GetCount();
	}
	return 0;
}

LPWSTR CPersonBuff::GetName()
{
	if (!CCharacter::wstrcmp_my(wszName, L""))
		return wszName;

	DWORD dwAddr = ReadDWORD(GetBase() + 0xC);
	if (dwAddr != NULL)
	{
		if (dwAddr > 0xD)
			CCharacter::UTF8ToUnicode(wszName, (LPCSTR)(dwAddr));
		else
			CCharacter::UTF8ToUnicode(wszName, (LPCSTR)(GetBase() + 0xC));
	}

	return wszName;
}

DWORD CPersonBuff::GetId()
{
	return ReadDWORD(GetBase() + 0x1C);
}

DWORD CPersonBuff::GetCount()
{
	DWORD dwCount = ReadDWORD(GetBase() + 0x28);
	return dwCount == 0 ? 1 : dwCount;
}