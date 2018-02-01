#ifndef __LOL_DATABASEMODEL_H__
#define __LOL_DATABASEMODEL_H__
#include "../../MyTools/MyTools/CLHead.h"
#include "../LOLProjects_China/GameStruct.h"
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>

class CDataBaseMdel
{
public:
	CDataBaseMdel();
	~CDataBaseMdel();

	static HANDLE	GetMutex();
	static BOOL		OpenSQLCon();

	static VOID	Lock(HANDLE hMutex);
	static VOID	UnLock(HANDLE hMutex);
	static VOID Clear();

	static BOOL GetCardInfo(__out vector<CardInfo>& vlst);
	static BOOL ExcuteSQL(LPCWSTR pwszSQL, __out LPWSTR pwszMsg);
public:
	static SQLHENV	ms_henv;
	static SQLHDBC	ms_hdbc;
	static SQLHSTMT ms_hstmt;
	static CRITICAL_SECTION ms_DbSection;
};



#endif// __LOL_DATABASEMODEL_H__