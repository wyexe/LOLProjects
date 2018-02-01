#include "BindStatusCallback.h"
#include <shlwapi.h>                    // for StrFormatByteSize()
#include "VarBase.h"
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCallback::CCallback()
{
}

CCallback::~CCallback()
{
}

HRESULT CCallback::OnProgress(ULONG ulProgress, ULONG ulProgressMax,
	ULONG ulStatusCode, LPCWSTR wszStatusText)
{

	static TCHAR   szCustomStatusMsg[256];
	static TCHAR   szAmtDownloaded[256], szTotalSize[256];

	UNREFERENCED_PARAMETER(ulStatusCode);

	// Make our own progress message - we'll show the amount downloaded and
	// the total file size (if known).

	StrFormatByteSize(ulProgress, szAmtDownloaded, 256);
	StrFormatByteSize(ulProgressMax, szTotalSize, 256);

	if (0 != ulProgressMax)
		wsprintf(szCustomStatusMsg, L"Downloaded %s of %s", szAmtDownloaded, szTotalSize);
	else
		wsprintf(szCustomStatusMsg, L"Downloaded %s (total size unknown)", szAmtDownloaded);

	// Report the progress back to the main window.

	if (0 != ulProgressMax)
		printf("%.2lf%%\r", int(100.0 * ulProgress / ulProgressMax));
	else
		printf("%s", szCustomStatusMsg);

	return S_OK;
}

#define _SELF L"DownLoadFile.cpp"

BOOL DownLoadFile(LPCWSTR pwszUrl, LPCWSTR pwszSaveAsFileName)
{
	if (pwszUrl == NULL)
		return FALSE;

	static WCHAR wszSaveAsFilePath[MAX_PATH];
	static WCHAR wszDownLoadUrl[MAX_PATH];

	::GetCurrentDirectoryW(MAX_PATH, wszSaveAsFilePath);
	lstrcatW(wszSaveAsFilePath, L"\\");
	lstrcatW(wszSaveAsFilePath, pwszSaveAsFileName);

	CCharacter::wstrcpy_my(wszDownLoadUrl, pwszUrl);


	CPrintLog::PrintLog_W(_SELF,__LINE__,L"ÏÂÔØURL=%s", pwszUrl);
	CCallback callBack;
	//HRESULT hr = ::URLDownloadToFileW(NULL, pwszUrl, pwszSaveAsFileName, NULL, &callBack);

	return TRUE;
}