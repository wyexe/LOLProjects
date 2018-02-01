#ifndef __LOL_EQUMENT_H__
#define __LOL_EQUMENT_H__

#include "GameSend.h"
#include "GameRes.h"
#include "Monster.h"

class CEqument
{
public:
	CEqument(DWORD dwBase);
	~CEqument();

	DWORD GetBase();
	DWORD GetCount();
	DWORD GetParm();
	DWORD GetId();

	static BOOL ExistItem(DWORD dwItemId);
	static UINT GetEuqmentList(vector<CEqument>& vlst);
	static BOOL BuyEqument();
	static BOOL SetEqumentIndex(em_Hero_Pro emHero);
private:
	DWORD m_dwBase;
};



#endif