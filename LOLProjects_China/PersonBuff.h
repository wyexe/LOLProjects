#ifndef __LOL_PERSONBUFF_H__
#define __LOL_PERSONBUFF_H__

#include "GameSend.h"

class CPersonBuff
{
public:
	CPersonBuff();
	~CPersonBuff();
	CPersonBuff(DWORD dwBase);
	DWORD GetBase() { return m_dwBase; };
	LPWSTR GetName();
	DWORD GetId();
	DWORD GetCount();

	static UINT GetPersonBuffLst(vector<CPersonBuff>& vlst);
	static BOOL ExistBuff(DWORD dwBuffId);
	static DWORD GetBuffCount(DWORD dwBuffId);
private:
	DWORD m_dwBase;
	WCHAR wszName[32];
};



#endif