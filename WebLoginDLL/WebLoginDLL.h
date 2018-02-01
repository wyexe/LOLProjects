// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WEBLOGINDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WEBLOGINDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WEBLOGINDLL_EXPORTS
#define WEBLOGINDLL_API __declspec(dllexport)
#else
#define WEBLOGINDLL_API __declspec(dllimport)
#endif

// This class is exported from the WebLoginDLL.dll
class CWebLoginDLL {
public:
	CWebLoginDLL(void);
	// TODO: add your methods here.
};
extern "C"
{
	WEBLOGINDLL_API int __cdecl SetInfo(int nLevel, int nSumFightCount, int nGold, LPCWSTR pwszNiceName);
	WEBLOGINDLL_API int __cdecl GetInfo(int& nAreId, LPWSTR pwszQQUser, LPWSTR pwszQQPass, LPWSTR pwszdamaUser, LPWSTR pwszdamaPass);
}
