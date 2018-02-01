#ifndef __LOL_CONSOLE_CONSOLECONFIG_H__
#define __LOL_CONSOLE_CONSOLECONFIG_H__

#include "VarBase.h"

class CConsoleConfig
{
public:
	CConsoleConfig();
	~CConsoleConfig();

	// 战斗场数
	static BOOL WriteFightCount(DWORD dwCount);

	// 战斗场数
	static DWORD GetFightCount();

	// 输赢日志
	static BOOL WriteGameResult();

	// 保存地图
	static BOOL SaveGameMap(LPCWSTR pwszMapName);

	// 设置战斗地图
	static BOOL SetGameMap(LPCWSTR wszMapName);

	// 根据名称返回地图Type
	static BOOL GetGameMap_By_Name(LPCWSTR pwszMapName, MapType& Mt);

	// 修改Hosts文件
	static BOOL UpdateHosts();

	// 获取游戏路径
	static BOOL GetTGPPath_By_Reg(LPWSTR pwszTGPPath); 

	// 获取TGP路径
	static BOOL GetGamePath_By_Process(LPWSTR pwszGamePath);
private:

};



#endif