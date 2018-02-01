#ifndef __SEARCHBASE_H__
#define __SEARCHBASE_H__ TRUE

#include <vector>
using namespace std;
union Base  
{
	DWORD   address;
	BYTE    data[4];
};

typedef enum _SearchType
{
	SearchType_Base = 0x1,
	SearchType_CALL	= 0x2
}SearchType;

#define Lol_China_Clinet				L"League of Legends.exe"

class CSearchBase
{
public:
	CSearchBase();
	~CSearchBase();

	BOOL	CompCode(const DWORD * pCode, const BYTE * pMem, UINT uLen);
	BOOL	SearchBase(LPCSTR szCode, DWORD * pArray, UINT& puLen, LPCWSTR lpszModule);
	BOOL	SearchBase(LPCSTR szCode, DWORD * pArray, UINT * puLen, LPCWSTR lpszModule, BOOL bOther);
	VOID	UnicodeToASCII(LPCWSTR lpszWText, LPSTR lpszText);
	DWORD	FindCALL(LPCSTR lpszCode, int nOffset, DWORD dwModuleAddr, int nMov, int nOrderNum, LPCWSTR lpszModule);
	DWORD	FindBase(LPCSTR lpszCode, int nOffset, int nMov, int nOrderNum, LPCWSTR lpszModule, DWORD dwAddrLen = 0xFFFFFFFF);
	DWORD	FindOffset(LPCSTR lpszCode, int nOffset, int nMov, int nOrderNum, LPCWSTR lpszModule, DWORD dwAddrLen = 0xFFFFFFFF);
	DWORD	FindAddr(LPCSTR lpszCode, int nOffset, int nOrderNum, LPCWSTR lpszModule);
	BOOL	FindBnsKoreaBase();
	DWORD	Read(DWORD dwAddr);
	DWORD	GetImageSize(HMODULE hm);

	int		GetWord_By_Char(BYTE dwWord, DWORD* pKey, UINT uKeyLen);
	BOOL	CL_sunday(DWORD* pKey, UINT uKeyLen, BYTE* pCode, UINT uCodeLen, vector<int>& vlst);
};

#endif//__SEARCHBASE_H__