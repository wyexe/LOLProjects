#ifndef __LOL_CENTER_ACCOUNTMODEL_H__
#define __LOL_CENTER_ACCOUNTMODEL_H__

#include "VarBase.h"

class CAccountModel
{
public:
	CAccountModel();
	~CAccountModel();

	static BOOL RemoveAccount(__in const ServerAccountInfo& __ServerAccountInfo);

	static BOOL UpdateAccount(__in const ServerAccountInfo& __ServerAccountInfo);

	static BOOL AddAccount(__in const ServerAccountInfo& __ServerAccountInfo);
private:

};



#endif