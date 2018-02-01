#include "stdafx.h"
#include "Field.h"


CField::CField(DWORD dwBase)
{
	m_dwBase = dwBase;
	ZeroMemory(wszName, sizeof(wszName));
}

CField::~CField()
{

}

DWORD CField::GetId()
{
	return ReadDWORD(m_dwBase + 0x8);
}

LPWSTR CField::GetName()
{
	if (!CCharacter::wstrcmp_my(wszName, L""))
		return wszName;

	if (ReadDWORD(m_dwBase + 0x20 + 0x10) > 0xF)
		CCharacter::UTF8ToUnicode(wszName, (CHAR*)ReadDWORD(m_dwBase + 0x20));
	else
		CCharacter::UTF8ToUnicode(wszName, (CHAR*)(m_dwBase + 0x20));

	return wszName;
}

BOOL CField::IsRefush()
{
	return ReadBYTE(m_dwBase + 0x165) == 0 ? FALSE : TRUE;
}

BOOL CField::TraverseField(vector<CField>& vlst, DWORD dwNode)
{
	if (dwNode != NULL && ReadBYTE(dwNode + 0xD) == 0)
	{
		CField CF(ReadDWORD(dwNode + 0x14));
		vlst.push_back(CF);

		TraverseField(vlst, ReadDWORD(dwNode + 0x0));
		TraverseField(vlst, ReadDWORD(dwNode + 0x8));
	}
	return TRUE;
}

BOOL CField::GetFieldList(vector<CField>& vlst)
{
	DWORD dwRoot = ReadDWORD(ReadDWORD(ReadDWORD(C_yeguai_base) + 0x4) + 0x4);
	return TraverseField(vlst, dwRoot);
}

BOOL CField::RefushField(LPCWSTR pwszName)
{
	static vector<CField> vlst;
	GetFieldList(vlst);

	for (auto& FD : vlst)
	{
		if (CCharacter::wstrcmp_my(pwszName, FD.GetName()) && FD.IsRefush())
			return TRUE;
	}

	return FALSE;
}