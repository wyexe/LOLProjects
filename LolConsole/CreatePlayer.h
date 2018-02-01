#ifndef __LOL_CONSOLE_CREATEPLAYER_H__
#define __LOL_CONSOLE_CREATEPLAYER_H__
#include "SearchPic.h"

class CCreatePlayer
{
public:
	CCreatePlayer();
	~CCreatePlayer();

	static BOOL GetRandName(LPWSTR pwszText);

	BOOL ClickPoint(int x, int y);

	BOOL CreatePlayer();

	// 创建角色完毕, 写入配置
	BOOL WriteCreatePlayerConfig(BOOL bCreate = TRUE);

	// 是否需要创建角色
	BOOL IsCreate();

	/////图色判断/////////////////////////////////////////////////////////////////////

	// 判断是否创建角色名称完毕
	BOOL IsCreatePlayerSucc();

	// 判断是否进入登录器主页
	BOOL IsLauncherMainPage();

private:
	CBmpFind m_BmpFind;
	RECT m_LauncherRect;
};



#endif// __LOL_CONSOLE_CREATEPLAYER_H__