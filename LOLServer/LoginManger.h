#ifndef __LOL_LOGINMANGER_H__
#define __LOL_LOGINMANGER_H__

#include "DataBaseModel.h"

typedef struct _LoginGameUser
{
	WCHAR wszIp[16];			// 所在的IP
	WCHAR wszLoginName[32];		// 帐号

	DWORD dwUseCount;			// 使用数量,每20次判断一下 NowTick - dwTick 必须< 1分钟
	ULONGLONG dwTickCount;		// 当使用数量置0时,这个也必须重新清0

	DWORD dwVerCode;			// 令牌Key
	ULONGLONG dwVerTick;		// 校验时间,当令牌修改时,重新置0

	ULONGLONG dwSurplusHour;	// 卡号剩余时间Hour
	ULONGLONG dwSurplusTick;	// 卡号剩余时间Tick

	DWORD dwLoginTick;			// 校验的登录时间

	CardStruct Card;			// 卡号
}LoginGameUser;

class CLoginManger
{
public:
	CLoginManger();
	~CLoginManger();

	// 判断卡号是否已经被占用了,判断卡号与令牌是否一致
	static LoginGameUser* _ExistCard(CardStruct & Card);

	// 判断是否还有剩余的卡号时间
	static BOOL _EnoughTime(LoginGameUser* LgGameUser);

	// 判断是否在登录时间范围内
	static BOOL _EnoughLoginTime(LoginGameUser* LgGameUser);

	// 判断校验时间是否过期了
	static BOOL _OverdueVer(LoginGameUser* LgGameUser);

	// 判断IP是否一致
	static BOOL _SameIp(LoginGameUser* LgGameUser, LPCWSTR pwszIp);

	// 判断次数是否超时
	static BOOL _TimeOut_ByCount(LoginGameUser* LgGameUser);

	// 校验卡号入口
	static BOOL VerCard(SockLoginData& SLD, LPCWSTR pwszIp, LPWSTR pwszMsg);

	// 重新生成结构
	static LoginGameUser* InitLoginUser(SockLoginData& SLD, LPCWSTR pwszIp, LPWSTR pwszMsg);

	// 新增卡号到内存
	static BOOL AddNewCard(CardStruct& Card, int nTime, DWORD dwToken, LPCWSTR pwszIp);

	static DWORD _CheckCard(LPCWSTR pwszCard);

	static BOOL _InitCard(vector<CardInfo>& vlst);

	static DWORD _GetCardTime(LPCWSTR pwszCardNo, DWORD dwToken, LPCWSTR pwszIp);
public:
	static vector<LoginGameUser> LgUserLst;
};

#endif// __LOL_LOGINMANGER_H__