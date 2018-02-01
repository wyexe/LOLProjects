#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__

//#define _BANNEDLOG 1

#include "../../MyTools/MyTools/CLHead.h"
#include <math.h>
#include <vector>
#include <queue>
#include <process.h>
using namespace std;

#define PRIVILEGE_CLOSE 0
#define PRIVILEGE_OPEN  1
#define PRIVILEGEDMODE	PRIVILEGE_OPEN

#pragma comment(linker,"/STACK:102400000,1024000")
/************************************************************************/
/* 人物相关                                                                     */
/************************************************************************/


#define Role_base             0x158D638
#define Role_Skill_offset      0x2418//0x2428
#define Role_Skill_offset1      0x518
#define Role_Skill_time         0x32252DC//游戏的开场时间
#define Role_Skill_time_offset  0x2C
#define Role_skill_mp            0x4EC
//#define Role_Lev                0x343c//可直接修改等级  技能点对应增加
//#define Role_skill_dian         0x3460//可学习的技能点
#define Role_Equip_offset       0x11A8//装备偏移
#define Role_Equip_ID           0x70

#define yingxiong_base     0x313736C
#define yingxiong_name  0x20
#define yingxong_Addr    0x5C
#define yingxong_sudu    0xB3C
#define yingxong_money    0x1174
#define yingxiong_hp       0x154
#define yingxiong_mp      0x20C
#define yingxiong_Title   0x8D0//职业
#define yingxiong_ID       0xFC
#define Occupation_State   0x2800  //统战正在占领塔的状态 

#define C_Acctck_ID      0x3564//攻击目标的ID  ?

#define Minion_base    0x25BD1FC

//#define Attr_base   0x157B310

#define C_CALL_Func  0xE6A5C0 //跟走路是一个


#define C_Timer_Acctck   5600
#define C_Timer_Optimi   5800//优化

#define C_CALL_buy        0xCA3DC0

#define C_CALL_use_Skill   0xD9BDB0

#define C_CALL_Study_skill  0xBE68E0             // 有检测

#define C_D3D_BASE          0x158FE34

#define C_look_base         0x1593FE0    //视野锁定基址

#define C_Goods_base        0x18A7738 //所有装备遍历基址

#define C_Message_base      0x158BCB0   //提示断开基址


#define C_CALL_GetName     0xC74910

#define C_CALL_retn       0x412638//堆栈返回地址
#define C_CALL_mid        0x00412630//伪造的调用地址

#define C_yeguai_base     0x1590444

#define C_TZ_hp           0x14BACF0
#define C_Skill_is_use    0x00C82880

#define BuffBase												0x1593A3C
#define UseItemBase												0x1593DE8
#define UseItemOffset											0x28
#define UseItem_Of_SkillOffset									0xDC
#define UseItemCALL												0x685C50

#define 统治_SelfHpBase											0x158DD70
#define 统治_EnemyHpBase											统治_SelfHpBase + 0x4
#define 统治占领CALL												0xD7BD00
#define 统治占领偏移												0x2EB0

#define 死亡偏移													0x208
#define CRC校验地址												0x7A7D70B
#define TenSLXAddr												0x79B0000


#define 周围环境对象基址											Minion_base
#define 人物基址													Role_base

#define 学习技能CALL												C_CALL_Study_skill

#define 人物装备偏移												Role_Equip_offset
#define 购买物品CALL												C_CALL_buy

#define 走路CALL													C_CALL_Func
#define 普通攻击CALL												走路CALL

#define 人物技能偏移												Role_Skill_offset
#define 人物技能偏移2											Role_Skill_offset1
#define 使用技能CALL												C_CALL_use_Skill

#define 人物金钱偏移												yingxong_money

#define 人物目标ID												C_Acctck_ID

#define 怪物名字CALL												C_CALL_GetName

#define 游戏开场时间基址											Role_Skill_time

#define HeroLevel_Offset										0x2B0



#define 人物属性基址												0x0

#define 怪物血偏移												yingxiong_hp
#define 怪物最大血偏移											怪物血偏移 + 0x10

#define 怪物蓝偏移												yingxiong_mp
#define 怪物最大蓝偏移											怪物蓝偏移 + 0x10

#define 怪物X坐标偏移											yingxong_Addr
#define 怪物Y坐标偏移											怪物X坐标偏移 + 0x4
#define 怪物Z坐标偏移											怪物X坐标偏移 + 0x8

#define 怪物名称偏移												0x20
#define 怪物迷雾偏移1											0x70
#define 怪物迷雾偏移2											0x2DC
#define 怪物迷雾对象偏移											0x210


#define MonsterType_营地炮塔										0x20005
#define MonsterType_炮塔											0x2401
#define MonsterType_Hero										0x1401
#define MonsterType_Solider										0xC01

#define Camp_Blue												0x64
#define Camp_Red												0xC8
#define Camp_Neutral											0x12C

#define MaxDis_Attack											10.0f
#define SkillName_BackHome										L"recall"
#define DroganName												L"SRU_Dragon6.1.1"
#define BlueBuffName_Top										L"SRU_Blue7.1.1"
#define BlueBuffName2_Buttom									L"SRU_Blue1.1.1"
#define RedBuffName2_Buttom										L"SRU_Red4.1.1"
#define RedBuffName_Top											L"SRU_Red4.1.1"

//////////////////////////////////////////////////////////////////////////

#define SOCKET_MESSAGE_GETACCOUNT					0x1
#define SOCKET_MESSAGE_RECHARGELOGINUSER			0x6
#define SOCKET_MESSAGE_GETALLCARDINFO				0xA
#define SOCKET_MESSAGE_SENDJPG						0xD
#define SOCKET_MESSAGE_RETMSG						0x11
#define SOCKET_MESSAGE_KEEPALIVE					0x14
#define SOCKET_MESSAGE_GETRES						0x15
#define SOCKET_MESSAGE_CLIENTMSG					0x2
#define SOCKET_MESSAGE_RECORD						0x3

#define SOCKET_MESSAGE_GETGROUP						0x16		// 获取团队
#define SOCKET_MESSAGE_GROUP_CAPTAIN				0x17
#define SOCKET_MESSAGE_GROUP_MEMBER					0x18
#define SOCKET_MESSAGE_LEAVEGROUP					0x19
#define SOCKET_MESSAGE_GAMETITLE					0x20
#define SOCKET_MESSAGE_CLOSECLIENT					0x21


#define SOCKET_MESSAGE_WAIT							0x1A

#define SIZE_NPC_Name								64
#define SIZE_NAME_Len								sizeof(wchar_t) * SIZE_NPC_Name
#define TeamMember_Dis								150
#define SIZE_MAX_ServerCount						10000

#define SIZE_SEND_LEN								1024
#define TAKEITEM_DISTANCE							200							//拾取物品的距离
#define SZFILE_NAME_SHAREDINFO						L"LOL_CN_Share_Info"
#define SZFILE_PERSONNAME_SHAREINFO					L"LOL_CN_PERSN_SHARE_INFO"
#define SZSERVER_NAME_SHAREDINFO					L"LOL_CN_Share_Info_Server"
#define MAX_GAME_COUNT								1

#define Mutex_CloseGame_Name						L"LOL_CN_CloseGame_Mutex"
#define Mutex_GameALive_Name						L"LOL_CN_GameALive_Mutex"
#define LOL_GAMEPROC_NAME							L"League of Legends.exe"
#define LOL_CLIENTPROC_NAME							L"LolClient.exe"
#define Mutex_GameLoseCon_Name						L"LOL_CN_GameLoseCon_Mutex"

#define Mutex_GameResult_Win						L"LOL_CN_GAMERESULT_WIN"
#define Mutex_GameResult_Lose						L"LOL_CN_GAMERESULT_LOSE"

#define GameCmd_None								0x0							//正常状态下,没开始挂机
#define GameCmd_Start								0x1							//已经开始挂机了
#define GameCmd_Stop								0x2							//停止挂机

#define ReadDWORD(x)								CCharacter::ReadDWORD(x)
#define ReadBYTE(x)									CCharacter::ReadBYTE(x)
#define ReadFloat(x)								CCharacter::ReadFloat(x)
//#define UnicodeToASCII(wszText,szText)				CCharacter::UnicodeToASCII(wszText,szText)
//#define ASCIIToUnicode(szText,wszText)				CCharacter::ASCIIToUnicode(szText,wszText)
//#define UTF8ToUnicode(wszText,pUTF8)				CCharacter::UTF8ToUnicode(wszText,pUTF8)
//#define WriteFloat(x,y)								*(float*)(x) = y

typedef unsigned (WINAPI *PTHREAD_START) (void*);
#define chBEGINTHREADEX(psa, cbStackSize, pfnStartAddr,		\
	pvParam, dwCreateFlags, pdwThreadID)					\
	((HANDLE)_beginthreadex(\
	(void *)(psa), \
	(unsigned)(cbStackSize), \
	(PTHREAD_START)(pfnStartAddr), \
	(void *)(pvParam), \
	(unsigned)(dwCreateFlags), \
	(unsigned *)(pdwThreadID)))





typedef struct _Point
{
	float X, Z, Y;
	_Point()
	{
		X = Y = Z = 0;
	}
	_Point(float pX, float pY, float pZ)
	{
		X = pX;
		Y = pY;
		Z = pZ;
	}
	bool operator == (const _Point & p)const
	{
		if (p.X == this->X && p.Y == this->Y)
		{
			return true;
		}
		return false;
	}
	bool operator != (const _Point & p)const
	{
		if (p.X == this->X && p.Y == this->Y)
		{
			return false;
		}
		return true;
	}
	bool IsZero()
	{
		if (this->X == 0 && this->Y == 0 && this->Z == 0)
		{
			return true;
		}
		return false;
	}
	void ConvertToInt()
	{
		this->X = (float)(int)this->X;
		this->Y = (float)(int)this->Y;
		this->Z = (float)(int)this->Z;
	}
	int GetX()
	{
		return (int)X;
	}
	int GetY()
	{
		return (int)Y;
	}
}Point;

typedef enum _MapType
{
	MapType_召唤师峡谷,
	MapType_水晶之痕,
	MapType_扭曲丛林,
	MapType_嚎哭深渊
}MapType;

typedef struct _GameTalkMsg
{
	WCHAR wszNiceName[32];
	WCHAR wszMsg[64];
}GameTalkMsg;

typedef struct _Hook_UseSkillInfo
{
	Point NowPoint;
	Point TarPoint;
	DWORD dwIndex;
	DWORD dwSkillBase;
	DWORD dwId;
}_Hook_UseSkillInfo;

typedef enum _em_SkillType
{
	em_SkillType_Directional,	// 指向性技能
	em_SkillType_UnDirectional, // 非指向性技能
	em_SkillType_Buff_Self,		// Buff技能
	em_SkillType_Self_UnDirectional // 非指向性的技能, 不过是以自己坐标
}em_SkillType;

typedef enum _em_Hero_Pro
{
	em_Hero_Pro_Ryze,			// 流浪法师
	em_Hero_Pro_Garen,			// 德玛西亚
	em_Hero_Pro_Ashe,			// 寒冰射手
	em_Hero_Pro_MissFortune,	// 赏金猎人
	em_Hero_Pro_Ezreal,			// 探险家
	em_Hero_Pro_MasterYi,		// 无极剑圣
	em_Hero_Pro_Malzahar,		// 虚空先知
	em_Hero_Pro_Chogath,		// 虚空恐惧
	em_Hero_Pro_Nunu,			// 雪人骑士
	em_Hero_Pro_Heimerdinger,	// 大发明家
	em_Hero_Pro_Graves,			// 法外狂徒
	em_Hero_Pro_Swain,			// 策士统领
	em_Hero_Pro_Annie,			// 黑暗之女
	em_Hero_Pro_Vayne,			// 暗夜猎手
	em_Hero_Pro_Maokai,			// 扭曲树精
	em_Hero_Pro_Veigar,			// 邪恶小法师
	em_Hero_Pro_Sejuani,		// 凛冬之怒
	em_Hero_Pro_Tristana,		// 麦林炮手
	em_Hero_Pro_Galio,			// 哨兵之殇
	em_Hero_Pro_Lucian,			// 圣枪游侠
	em_Hero_Pro_Trundle,		// 巨魔之王
	em_Hero_Pro_Syndra,			// 暗黑元首
	em_Hero_Pro_Aatrox,			// 暗裔剑魔
	em_Hero_Pro_Taric,			// 宝石骑士
	em_Hero_Pro_Rumble,			// 机械公敌
	em_Hero_Pro_Diana,			// 皎洁女神
	em_Hero_Pro_Urgot,			// 首领之傲
	em_Hero_Pro_Teemo,			// 迅捷斥候
	em_Hero_Pro_Caitlyn,		// 皮城女警
	em_Hero_Pro_Corki,			// 英勇投弹手
	em_Hero_Pro_Fiora,			// 无双剑姬
	em_Hero_Pro_Kennen,			// 狂暴之心
	em_Hero_Pro_Mordekaiser,	// 铁铠冥魂
	em_Hero_Pro_Nautilus,		// 深海泰坦
	em_Hero_Pro_RekSai,			// 虚空遁地兽
	em_Hero_Pro_Sivir,			// 战争女神
	em_Hero_Pro_Thresh,			// 魂锁典狱长
	em_Hero_Pro_TwistedFate,	// 卡牌大师
	em_Hero_Pro_KogMaw,			// 深渊巨口
	em_Hero_Pro_Ahri			// 九尾妖狐
}em_Hero_Pro;

typedef struct _SkillConfig
{
	int nHeroPro;
	WCHAR wszOccName[32];
	em_SkillType emType_Q;
	float fSkillDis_Q;
	em_SkillType emType_W;
	float fSkillDis_W;
	em_SkillType emType_E;
	float fSkillDis_E;
	em_SkillType emType_R;
	float fSkillDis_R;
	em_SkillType emType_1;
	float fSkillDis_1;
	em_SkillType emType_2;
	float fSkillDis_2;
}SkillConfig;

typedef enum _em_Skill_Flag
{
	em_Skill_Flag_Q,
	em_Skill_Flag_W,
	em_Skill_Flag_E,
	em_Skill_Flag_R,
	em_Skill_Flag_1,
	em_Skill_Flag_2
}em_Skill_Flag;

typedef struct _ResItem
{
	DWORD dwItemId;
	DWORD dwPrice;
}ResItem;

typedef enum _PointType
{
	PointType_HighTurret,
	PointType_2Turret,
	PointType_1Turret,
	PointType_Normal
}PointType;

typedef enum _em_Path_Flag
{
	em_Path_Flag_Top,
	em_Path_Flag_Middle,
	em_Path_Flag_Buttom
}em_Path_Flag;

typedef enum _em_MonsterType
{
	em_MonsterType_Turret,		// 塔
	em_MonsterType_Hero,		// 英雄
	em_MonsterType_Soldier,		// 士兵
	em_MonsterType_UnKnow		// 未知
}em_MonsterType;

typedef enum _em_Camp
{
	em_Camp_Blue,
	em_Camp_Red,
	em_Camp_Neutral,
	em_Camp_UnKnow
}em_Camp;

typedef struct _TurretSrt
{
	WCHAR wszTurretName[32];
	em_Camp emCamp;
	em_Path_Flag emPathFlag;
}TurretSrt;

typedef struct _ResPoint
{
	int nIndex;
	Point Pit;
	PointType PitType;
	em_Camp emCamp;
}ResPoint;

typedef struct _CardStruct
{
	WCHAR szCard[35];
	DWORD dwCardCode;
}CardStruct;

typedef struct _CardInfo
{
	CardStruct	Card;
	WCHAR wszSurplusHour[32];
	WCHAR wszCreateDate[32];
	WCHAR wszActiveDate[32];
	WCHAR wszLoginUser[32];
}CardInfo;

typedef struct _SockLoginData
{
	WCHAR wszLoginUser[32];
	WCHAR wszGameUser[16];
	WCHAR wszPass[32];
	WCHAR wszNiceName[16];
	int nLevel;
	int nGold;
	int nSumWarCount;
}SockLoginData;

typedef enum _em_SockRes
{
	em_SockRes_EqumentPrice_By_Id,	// 根据装备Id获取价格
	em_SockRes_DragonPoint,			// 小龙坐标
	em_SockRes_SkillConfig,			// 技能配置
	em_SockRes_Path_MovePoint,		// 分路的坐标
}em_SockRes;

typedef struct _SockRes
{
	em_SockRes emSockRes;			// 枚举Id
	SockLoginData SLD;				// 通讯包
}SockRes;

typedef struct _SockKeepConSrc
{
	DWORD dwQqCrc;
	ULONG ulTick;
	DWORD dwComputerCrc;
}SockKeepConSrc;

typedef enum _GameDifficulty
{
	GameDifficulty_Entry,			// 入门
	GameDifficulty_Easy,			// 简单
	GameDifficulty_Normal			// 一般
}GameDifficulty;

typedef struct _SockClientGroup
{
	SockLoginData SLD;				// 帐号信息

	DWORD dwNiceNameCrc;			// 昵称Crc
	DWORD dwLoginUserCrc;			// 登录帐号Crc
	DWORD dwGameUserCrc;			// QQ号Crc

	WCHAR wszServerName[32];		// 服务器CRC
	WCHAR wszMapName[32];			// 地图Crc

	WCHAR wszComputerName[32];		// 编号
	WCHAR wszClientMsg[64];			// 客户端消息

	ULONGLONG ulTick;				// 激活时间
	BOOL bAlive;					// 是否可返回此英雄到客户端(二次请求)
	BOOL bLeave;					// 是否离队了

	BOOL bMatchMachine;				// 是否打人机

	GameDifficulty Difficulty;		// 难度

	BOOL bMatching;					// 是否单排

	int nHeroIndex;					// 
	em_Hero_Pro emHero;				// 正式的英雄
	em_Hero_Pro emRetainHero;		// 备用的英雄
	bool operator == (const _SockClientGroup& s1)
	{
		return s1.dwNiceNameCrc == this->dwNiceNameCrc && s1.dwGameUserCrc == this->dwGameUserCrc && s1.dwLoginUserCrc == this->dwLoginUserCrc;
	}
}SockClientGroup;

typedef struct _Srt_ServerGroup
{
	DWORD dwServerCrc;
	WCHAR wszServerName[32];
	bool operator == (const _Srt_ServerGroup& s)
	{
		return s.dwServerCrc == this->dwServerCrc && CCharacter::wstrcmp_my(this->wszServerName, s.wszServerName);
	}
}Srt_ServerGroup;

typedef struct _Srt_MapGroup
{
	DWORD dwMapCrc;
	WCHAR wszMapName[32];
	bool operator == (const _Srt_MapGroup& s)
	{
		return s.dwMapCrc == this->dwMapCrc && CCharacter::wstrcmp_my(this->wszMapName, s.wszMapName);
	}
}Srt_MapGroup;

typedef enum _AccountPlayMode
{
	AccountPlayMode_Default,
	AccountPlayMode_Custome
}AccountPlayMode;

typedef struct _GroupList
{
	SockClientGroup Captain;		// 队长
	vector<SockClientGroup> MemberList;// 队员
	vector<int> emList;
	AccountPlayMode PlayMode;

	void InitHero()
	{
		emList.clear();
		for (int i = 0; i < 5; ++i)
			emList.push_back(i);
	}
}GroupList;

typedef struct _Map_GroupMember
{
	vector<GroupList> vGrouplst;	// 该地图下的团队列表
	Srt_MapGroup	 Srt_Map;
}Map_GroupMember;


typedef struct _Server_GroupMember
{
	vector<Map_GroupMember> vMaplst;// 服务器下的地图列表
	Srt_ServerGroup			Srt_Server;
}Server_GroupMember;

typedef struct _WebLoginInfo
{
	WCHAR wszGameUser[16];
	WCHAR wszGamePass[32];
	WCHAR wszGameServer[8];
	DWORD dwAreId;
	WCHAR wszdamaUser[16];
	WCHAR wszdamaPass[16];
}WebLoginInfo;

typedef struct _WebPersonInfo
{
	DWORD dwLevel;
	DWORD dwSumWarCount;
	DWORD dwGold;
	WCHAR wszNiceName[32];
}WebPersonInfo;

typedef struct _ServerAccountInfo
{
	WCHAR wszQQ[16];				// QQ
	WCHAR wszPass[32];				// 密码
	WCHAR wszComputerName[16];		// 编号
	WCHAR wszServer[8];				// 大区
	WCHAR wszNiceName[16];			// 昵称
	WCHAR wszMapName[8];			// 地图
	WCHAR wszStatus[32];			// 状态
	WCHAR wszCaptain[16];			// 队长
	WCHAR wszMsg[32];				// 消息
	DWORD dwLevel;					// 等级
	DWORD dwMaxLevel;				// 达成条件的等级
	DWORD dwGold;					// 金币
	DWORD dwMaxGold;				// 达成条件的金币
	DWORD dwFightCount;				// 局数
	DWORD dwComputerNameCrc;		// 编号Crc
	DWORD dwQqCrc;					// QqCrc
	BOOL bIdSealed;					// 是否封号了
	BOOL bError;					// 是否出错了
	AccountPlayMode PlayMode;		// 模式

	bool operator == (const _ServerAccountInfo& __ServerAccountInfo)
	{
		return __ServerAccountInfo.dwComputerNameCrc == this->dwComputerNameCrc;
	}

	bool CompQqCrc(DWORD dwQqCrc)
	{
		return dwQqCrc == this->dwQqCrc;
	}
	bool IsFinish()
	{
		return dwGold >= dwMaxGold && dwLevel >= dwMaxLevel;
	}
}ServerAccountInfo;

typedef struct _Account_Info
{
	WCHAR szUserName[64];			//	帐号
	WCHAR szPassWord[32];			//	密码
	WCHAR szPlayerName[32];			//	玩家名称
	DWORD dwLevel;					//	等级
	DWORD dwGold;					//	金币
	WCHAR szServer[32];				//	区组
	WCHAR wszArea[32];				//	服务器名称
	WCHAR szCardNo[64];				//	卡号
	WCHAR szCRC[32];				//	CRC32
	DWORD dwLoginStatus;			//	登录状态
	DWORD dwEqumentIndex;			// 购买装备的索引
	em_Path_Flag emPathFlag;		// 人物要走哪条路
	em_Hero_Pro emHeroPro;			// 职业
	DWORD dwStartTick;				// 开始时间
	BOOL bClose;					// 是否要关闭了
	BOOL bWin;						// 是否赢了
}ACCOUNT_INFO,*PACCOUNT_INFO;

#define ACCOUNT_INFO_GAME_FLAG_NONE		0x1
#define ACCOUNT_INFO_GAME_FLAG_DELETE	0x2
#define ACCOUNT_INFO_GAME_FLAG_LOGIN	0x3
#define ACCOUNT_INFO_GAME_FLAG_RUN		0x4
#define ACCOUNT_INFO_GAME_FLAG_RELOGIN	0x5

#define ACCOUNT_INFO_GAME_GAMERUN_NONE			0x0
#define ACCOUNT_INFO_GAME_GAMERUN_RUN			0x1
#define ACCOUNT_INFO_GAME_GAMERUN_EXIST_RUN		0x2
#define ACCOUNT_INFO_GAME_GAMERUN_SUCC_RUN		0x3
#define ACCOUNT_INFO_GAME_GAMERUN_STOP			0x4
#define ACCOUNT_INFO_GAME_GAMERUN__EXIST_STOP	0x5
#define ACCOUNT_INFO_GAME_GAMERUN_SUCC_STOP		0x6

typedef struct _Account_Info_GAME
{
	ACCOUNT_INFO MyAcount_Info;		// 帐号信息
	DWORD dwPid;					// 进程ID
	HWND hWndGame;					// 窗口句柄
	HWND hMainFormWnd;				// 挂机窗口句柄
	HANDLE hProcess;				// 游戏句柄
	BOOL bClose;					// 是否需要关闭游戏
	BOOL bStop;						// 是否暂停挂机
	int  nIndex;					// 索引
	DWORD dwFlag;					// 标志
	WebLoginInfo webInfo;			// 网页登录所需的信息
	WebPersonInfo webPerson;		// 网页人物的信息
	BOOL bLock;
	
	void Lock()
	{
		for (int i = 0; i < 3 * 10 && bLock; ++i)
			Sleep(100);

		bLock = TRUE;
	}

	void UnLock()
	{
		bLock = FALSE;
	}
}ACCOUNT_INFO_GAME,*PACCOUNT_INFO_GAME;

typedef struct _Shared_Info
{
	WCHAR wszTGPMsg[64];			//登录错误的日志
	WCHAR wszTGPPath[MAX_PATH];		//游戏的路径
	int nAccountCount;				//帐号数量
	int nRunTGPCount;				// 启动TGP次数
	ACCOUNT_INFO_GAME MyGameInfo[MAX_GAME_COUNT];//帐号信息

	bool GetExistUser(LPCWSTR lpszUser)
	{
		for (int i = 0; i < MAX_GAME_COUNT; ++i)
		{
			if (CCharacter::wstrcmp_my(lpszUser,this->MyGameInfo[i].MyAcount_Info.szUserName))
			{
				return true;
			}
		}
		return false;
	}
	PACCOUNT_INFO_GAME GetAccount_For_User(LPCWSTR lpszUser)
	{
		for (int i = 0; i < MAX_GAME_COUNT; ++i)
		{
			if (CCharacter::wstrcmp_my(lpszUser,this->MyGameInfo[i].MyAcount_Info.szUserName))
			{
				return &this->MyGameInfo[i];
			}
		}
		return NULL;
	}
	PACCOUNT_INFO_GAME GetAccount_For_GameName(LPCWSTR lpszUser)
	{
		for (int i = 0; i < MAX_GAME_COUNT; ++i)
		{
			if (CCharacter::wstrcmp_my(lpszUser,this->MyGameInfo[i].MyAcount_Info.szPlayerName))
			{
				return &this->MyGameInfo[i];
			}
		}
		return NULL;
	}
	PACCOUNT_INFO_GAME GetAccount_For_Empty()
	{
		for (int i = 0; i < MAX_GAME_COUNT; ++i)
		{
			if (CCharacter::wstrcmp_my(this->MyGameInfo[i].MyAcount_Info.szUserName, L""))
			{
				return &this->MyGameInfo[i];
			}
		}
		return NULL;
	}
}SHARED_INFO,*PSHARED_INFO;

#endif