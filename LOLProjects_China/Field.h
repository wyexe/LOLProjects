#ifndef __LOL_FIELD_H__
#define __LOL_FIELD_H__

#include "GameSend.h"

class CField
{
public:
	CField(DWORD dwBase);
	~CField();

	DWORD GetBase() { return m_dwBase; };

	DWORD GetId();
	LPWSTR GetName();
	BOOL IsRefush();

	static BOOL TraverseField(vector<CField>& vlst, DWORD dwNode);
	static BOOL GetFieldList(vector<CField>& vlst);
	static BOOL RefushField(LPCWSTR pwszName);
private:
	DWORD m_dwBase;
	WCHAR wszName[32];
};


#endif// __LOL_FIELD_H__