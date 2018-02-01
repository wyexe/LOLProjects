#ifndef __LOL_CENTER_ACCOUNTCONFIG_H__
#define __LOL_CENTER_ACCOUNTCONFIG_H__

#include "AccountModel.h"

typedef struct _AccountConfigInfo
{
	WCHAR wszQQ[16];			// QQ
	WCHAR wszPass[32];			// QQ密码
	WCHAR wszServer[8];			// 哪个区
	WCHAR wszMapName[8];		// 地图
	DWORD dwConditionLevel;		// 达成条件的等级
	DWORD dwConditionGold;		// 达成条件的金币
	AccountPlayMode PlayMode;
	WCHAR wszComputerName[16];	// ID
}AccountConfigInfo;

class CAccountConfig
{
public:
	CAccountConfig();
	~CAccountConfig();

	// 读取文件
	static BOOL ReadAccountConfig(__out vector<AccountConfigInfo>& vlst);

	// 保存已经刷完的帐号,(调用前记得使用CAccountModel::RemoveAccount)
	static BOOL SaveFinishAccount(__in const ServerAccountInfo& __ServerAccountInfo);

	// 保存出错的帐号,(调用前记得使用CAccountModel::RemoveAccount)
	static BOOL SaveErrorAccount(__in const ServerAccountInfo& __ServerAccountInfo);

	// 保存封号的帐号,(调用前记得使用CAccountModel::RemoveAccount)
	static BOOL SaveIdSealedAccount(__in const ServerAccountInfo& __ServerAccountInfo);

	// 保存备份数据
	static BOOL SaveAccountConfig();
	
	// 格式化1个数字
	static LPCWSTR FormatNumber(DWORD dwNumber);

	// 格式化2个数字
	static LPCWSTR FormatNumber(DWORD dwNumber1, DWORD dwNumber2);

	// 返回当前时间
	static LPCWSTR FormatNowTime();

	// 返回AccountInfo
	static BOOL GetAccountInfo_By_Text(__in LPCWSTR pwszText, __out AccountConfigInfo& ConfigInfo);


private:
	static BOOL ReadAccountConfigFile(__out string& strOutText);
	
	static BOOL WriteFile(const string& strText, LPCSTR pszFileName, LPCSTR pszMode);
	
};



#endif