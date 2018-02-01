#include "DataBaseModel.h"

#define _SELF L"DataBaseModel.cpp"

#define		SELECT_ALL				0
#define		SELECT_CITY				Still_Unsigned_Yet>_<|||
#define		SELECT_LAT_EXACT		1
#define		SELECT_LAT_SCOPE		2

SQLHENV CDataBaseMdel::ms_henv = SQL_NULL_HENV;
SQLHDBC	CDataBaseMdel::ms_hdbc = SQL_NULL_HDBC;
SQLHSTMT CDataBaseMdel::ms_hstmt = SQL_NULL_HSTMT;
CRITICAL_SECTION CDataBaseMdel::ms_DbSection;

CDataBaseMdel::CDataBaseMdel()
{
}

CDataBaseMdel::~CDataBaseMdel()
{
}

HANDLE CDataBaseMdel::GetMutex()
{
	static BOOL bInit = FALSE;
	if (!bInit)
	{
		//InitializeCriticalSection(&ms_DbSection);
		bInit = TRUE;
	}
	return NULL;
}

VOID CDataBaseMdel::Lock(HANDLE hMutex)
{
	//EnterCriticalSection(&ms_DbSection);
}

VOID CDataBaseMdel::UnLock(HANDLE hMutex)
{
	//LeaveCriticalSection(&ms_DbSection);
}

BOOL CDataBaseMdel::OpenSQLCon()
{
	if (ms_hdbc != SQL_NULL_HDBC)
		return TRUE;

	try
	{
		// 配置ODBC必须是用户Dns
		static WCHAR wszDns[32] = { L"LolChina" };
		static WCHAR wszUser[32] = { L"GameUser" };
		static WCHAR wszPass[32] = { L"DONGQI!@#^%$" };

		static RETCODE RetCode;

		// 分配环境
		RetCode = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &ms_henv);
		if (RetCode != SQL_SUCCESS)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"分配环境失败:SQLAllocHandle(SQL_HANDLE_ENV, NULL, &ms_henv)");
			SQLFreeHandle(SQL_HANDLE_ENV, ms_henv);
			ms_hdbc = SQL_NULL_HDBC;
			return FALSE;
		}

		// 设置环境属性
		RetCode = SQLSetEnvAttr(ms_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
		if (RetCode != SQL_SUCCESS)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"设置环境属性失败:SQLSetEnvAttr(ms_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER)");
			SQLFreeHandle(SQL_HANDLE_ENV, ms_henv);
			ms_hdbc = SQL_NULL_HDBC;
			return FALSE;
		}

		// 分配连接
		RetCode = SQLAllocHandle(SQL_HANDLE_DBC, ms_henv, &ms_hdbc);
		if (RetCode != SQL_SUCCESS)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"分配连接失败:SQLAllocHandle(SQL_HANDLE_DBC, ms_henv, &ms_hdbc)");
			SQLFreeHandle(SQL_HANDLE_DBC, ms_hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, ms_henv);
			ms_hdbc = SQL_NULL_HDBC;
			return FALSE;
		}

		// 创建数据库连接
		RetCode = SQLConnectW(ms_hdbc, wszDns, wcslen(wszDns) + 1, wszUser, wcslen(wszUser) + 1, wszPass, wcslen(wszPass) + 1);
		if (RetCode != SQL_SUCCESS && RetCode != SQL_SUCCESS_WITH_INFO)
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"创建数据库连接失败:SQLConnectW");
			SQLDisconnect(ms_hdbc);
			SQLFreeHandle(SQL_HANDLE_DBC, ms_hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, ms_henv);
			ms_hdbc = SQL_NULL_HDBC;
			return FALSE;
		}

		return TRUE;
	}
	catch (...)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"OpenSQLCon出现异常");
	}
	return FALSE;
}

VOID CDataBaseMdel::Clear()
{
	if (ms_hstmt != SQL_NULL_HSTMT)
	{
		SQLCloseCursor(ms_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, ms_hstmt);
		ms_hstmt = SQL_NULL_HSTMT;
	}

	if (ms_hdbc != SQL_NULL_HDBC)
	{
		SQLDisconnect(ms_hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, ms_hdbc);
		ms_hdbc = NULL;
	}

	if (ms_henv != SQL_NULL_HENV)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, ms_henv);
		ms_henv = NULL;
	}
}

BOOL CDataBaseMdel::GetCardInfo(__out vector<CardInfo>& vlst)
{
	Lock(GetMutex());
	try
	{
		if (!OpenSQLCon())
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"OpenSQLCon Faild!");
			Clear();
			UnLock(GetMutex());
			return FALSE;
		}

		static WCHAR wszSQL[1024];
		wsprintfW(wszSQL, L"exec GetAllCard");

		RETCODE RetCode = SQLAllocHandle(SQL_HANDLE_STMT, ms_hdbc, &ms_hstmt);

		static CardInfo Info;
		SQLExecDirectW(ms_hstmt, wszSQL, wcslen(wszSQL));

		ZeroMemory(&Info, sizeof(Info));
		SQLBindCol(ms_hstmt, 1, SQL_C_WCHAR, Info.Card.szCard, sizeof(Info.Card.szCard), NULL);
		SQLBindCol(ms_hstmt, 2, SQL_C_WCHAR, Info.wszSurplusHour, sizeof(Info.wszSurplusHour), NULL);
		SQLBindCol(ms_hstmt, 3, SQL_C_WCHAR, Info.wszCreateDate, sizeof(Info.wszCreateDate), NULL);
		SQLBindCol(ms_hstmt, 4, SQL_C_WCHAR, Info.wszActiveDate, sizeof(Info.wszActiveDate), NULL);
		SQLBindCol(ms_hstmt, 5, SQL_C_WCHAR, Info.wszLoginUser, sizeof(Info.wszLoginUser), NULL);

		int nCount = 0;

		// 特么的必须设置存储过程的SET NOCOUNT ON,否则结果不能返回~!
		while (true)
		{
			RetCode = SQLFetch(ms_hstmt);
			if (RetCode == SQL_ERROR)
			{
				CPrintLog::PrintLog_W(_SELF, __LINE__, L"RetCode=SQL_ERROR");
				CCharacter::wstrcpy_my(Info.Card.szCard, L"执行结果错误~!");
				Clear();
				UnLock(GetMutex());
				return FALSE;
			}
			if (RetCode != SQL_SUCCESS)
				break;

			vlst.push_back(Info);
		}

		SQLCloseCursor(ms_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, ms_hstmt);
		ms_hstmt = SQL_NULL_HSTMT;
		UnLock(GetMutex());
		return TRUE;
	}
	catch (...)
	{
		Clear();
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"GetCardInfo_For_LoginUser出现异常!");
	}
	UnLock(GetMutex());
	return FALSE;
}

BOOL CDataBaseMdel::ExcuteSQL(LPCWSTR pwszSQL, __out LPWSTR pwszMsg)
{
	Lock(GetMutex());
	try
	{
		if (!OpenSQLCon())
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"OpenSQLCon失败!");
			Clear();
			UnLock(GetMutex());
			return FALSE;
		}

		RETCODE RetCode = SQLAllocHandle(SQL_HANDLE_STMT, ms_hdbc, &ms_hstmt);

		SQLExecDirectW(ms_hstmt, (WCHAR*)pwszSQL, wcslen(pwszSQL) + 1);

		SQLBindCol(ms_hstmt, 1, SQL_C_WCHAR, pwszMsg, 32 * 2, NULL);

		// 特么的必须设置存储过程的SET NOCOUNT ON,否则结果不能返回~!
		RetCode = SQLFetch(ms_hstmt);
		if (RetCode == SQL_ERROR)
		{
			CCharacter::wstrcpy_my(pwszMsg, L"执行结果错误~!");
			Clear();
		}

		SQLCloseCursor(ms_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, ms_hstmt);
		ms_hstmt = SQL_NULL_HSTMT;

		UnLock(GetMutex());
		return TRUE;
	}
	catch (...)
	{
		Clear();
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"ExcuteSQL出现异常");
	}
	UnLock(GetMutex());
	return FALSE;
}