#include "stdafx.h"
#include "AccountModel.h"

CAccountModel::CAccountModel()
{
}

CAccountModel::~CAccountModel()
{
}

BOOL CAccountModel::RemoveAccount(__in const ServerAccountInfo& __ServerAccountInfo)
{
	// 把完成的帐号 从 List消除
	AccountListLock.Access([&__ServerAccountInfo]()
	{
		for (auto itr = AccountList.begin(); itr != AccountList.end(); ++itr)
		{
			if (itr->CompQqCrc(__ServerAccountInfo.dwQqCrc))
			{
				AccountList.erase(itr);
				break;
			}
		}
	});
	return TRUE;
}

BOOL CAccountModel::UpdateAccount(__in const ServerAccountInfo& __ServerAccountInfo)
{
	AccountListLock.Access([&__ServerAccountInfo]{
		for (auto& tmpServerAccountInfo : AccountList)
		{
			if (tmpServerAccountInfo.CompQqCrc(__ServerAccountInfo.dwQqCrc))
			{
				tmpServerAccountInfo = __ServerAccountInfo;
				break;
			}
		}
	});
	return TRUE;
}

BOOL CAccountModel::AddAccount(__in const ServerAccountInfo& __ServerAccountInfo)
{
	AccountListLock.Access([&__ServerAccountInfo]{
		BOOL bExist = FALSE;
		for (auto& tmpServerAccountInfo : AccountList)
		{
			if (tmpServerAccountInfo.CompQqCrc(__ServerAccountInfo.dwQqCrc))
			{
				bExist = TRUE;
				break;
			}
		}

		if (!bExist)
		{
			AccountList.push_back(__ServerAccountInfo);
		}
	});
	return TRUE;
}