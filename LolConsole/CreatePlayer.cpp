#include "CreatePlayer.h"

#define _SELF L"CreatePlayer.cpp"

CCreatePlayer::CCreatePlayer()
{
	// 获取窗口位置
	CSearchPic::GetGameRect(m_LauncherRect);

	// 初始化图色判断
	WCHAR wszPath[MAX_PATH];
	::GetCurrentDirectoryW(MAX_PATH, wszPath);
	lstrcatW(wszPath, L"\\Bmp");
	m_BmpFind.SetPath(wszPath);

	m_BmpFind.BindWindow(CSearchPic::GetGameHwnd());
}

CCreatePlayer::~CCreatePlayer()
{
}

BOOL CCreatePlayer::ClickPoint(int x, int y)
{
	for (int i = 0; i < 1; ++i)
	{
		// 设置鼠标
		::SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, NULL, NULL);
		mouse_event(MOUSEEVENTF_LEFTUP, x, y, NULL, NULL);
	}
	//CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"ClickPoint");
	return TRUE;
}

BOOL CCreatePlayer::WriteCreatePlayerConfig(BOOL bCreate)
{
	static WCHAR wszIniPath[MAX_PATH];

	::GetCurrentDirectoryW(MAX_PATH, wszIniPath);
	lstrcatW(wszIniPath, L"\\Login.ini");

	::WritePrivateProfileStringW(L"Config", L"PlayerEmpty", bCreate ? L"1" : L"0", wszIniPath);
	return TRUE;
}

BOOL CCreatePlayer::IsCreatePlayerSucc()
{
	int x, y;
	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"ChoosePlayerIco.bmp", x, y))
		return TRUE;

	return FALSE;
}

BOOL CCreatePlayer::IsLauncherMainPage()
{
	int x, y;
	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"4.bmp", x, y))
		return TRUE;

	return FALSE;
}

BOOL CCreatePlayer::CreatePlayer()
{
	int x = 0, y = 0;
	CSearchPic::GetGameRect(m_LauncherRect);

	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"HideMsg1.bmp", x, y) > 0)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"不要挡路!HideMsg1");
		ClickPoint(m_LauncherRect.left + x + 20, m_LauncherRect.top + y + 20);
		Sleep(5000);
	}

	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"CreatePlayer.bmp", x, y) > 0)
	{
	lb_Start:;
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"自动创建角色名称!");
		//52*250
		ClickPoint(m_LauncherRect.left + 620, m_LauncherRect.top + 494);
		for (int j = 0; j < 20; ++j)
			CLPublic::SendKey(8);

		Sleep(500);
		WCHAR wszName[32] = { 0 };
		GetRandName(wszName);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"wszName=%s", wszName);
		CLPublic::SendKeys(wszName);
		Sleep(1000);
		ClickPoint(m_LauncherRect.left + 649, m_LauncherRect.top + 573);
		Sleep(3000);

		// 是否创建角色成功
		if (!IsCreatePlayerSucc())
		{
			CPrintLog::PrintLog_W(_SELF, __LINE__, L"创建角色失败, 重新输入名字!");
			ClickPoint(m_LauncherRect.left + 647, m_LauncherRect.top + 438);
			Sleep(2000);
			goto lb_Start;
		}

		// 选择角色图标
		Sleep(1000);
		ClickPoint(m_LauncherRect.left + 385, m_LauncherRect.top + 324);
		Sleep(500);
		ClickPoint(m_LauncherRect.left + 385, m_LauncherRect.top + 324);

		// 选择角色图标后点确定
		Sleep(1000);
		ClickPoint(m_LauncherRect.left + 971, m_LauncherRect.top + 550);
		Sleep(3000);

		// 选择游戏的熟悉程度
		ClickPoint(m_LauncherRect.left + 865, m_LauncherRect.top + 365);
		Sleep(500);
		ClickPoint(m_LauncherRect.left + 900, m_LauncherRect.top + 545);
		Sleep(2000);

		// 是否在实战训练营提高自己
		ClickPoint(m_LauncherRect.left + 700, m_LauncherRect.top + 512);
		Sleep(5000);
	}

	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"12.bmp", x, y) > 0)
	{
		ClickPoint(m_LauncherRect.left + 865, m_LauncherRect.top + 365);
		Sleep(500);
		ClickPoint(m_LauncherRect.left + 900, m_LauncherRect.top + 545);
		Sleep(2000);

		ClickPoint(m_LauncherRect.left + 700, m_LauncherRect.top + 512);
		Sleep(5000);
	}

	if (TRUE || IsLauncherMainPage())
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"创建角色完毕!");
	}

	// 结束进程
	//CLProcess::TerminateProc_For_ProcName(LOL_CLIENTPROC_NAME);
	//Sleep(3000);

	// 窗口还存在的话, 试试发送关闭消息的命令
	//HWND m_hWndLuncher = ::FindWindowW(L"ApolloRuntimeContentWindow", L"PVP.net 客户端");
	//if (m_hWndLuncher != NULL)
		//::PostMessageW(m_hWndLuncher, WM_CLOSE, NULL, NULL);

//	Sleep(2000);
	return TRUE;
}

BOOL CCreatePlayer::IsCreate()
{
	static WCHAR wszValue[32];
	static WCHAR wszIniPath[MAX_PATH];

	::GetCurrentDirectoryW(MAX_PATH, wszIniPath);
	lstrcatW(wszIniPath, L"\\Login.ini");

	// 读取配置文件, 是否需要创建角色
	if (!GetPrivateProfileStringW(L"Config", L"PlayerEmpty", L"Error", wszValue, 32, wszIniPath) || CCharacter::wstrcmp_my(wszValue, L"Error"))
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"读取PlayerEmpty失败!");
		::TerminateProcess(::GetCurrentProcess(), 0);
		return FALSE;
	}

	// PlayerEmpty=0, 表示这个配置是新的, 估计需要创建角色
	if (CCharacter::wstrcmp_my(wszValue, L"0"))
		return TRUE;

	int x = 0, y = 0;
	m_BmpFind.BindWindow(CSearchPic::GetGameHwnd());
	if (m_BmpFind.FindPic(0, 0, 1280, 800, L"CreatePlayer.bmp", x, y) > 0) // 必须要创建角色
		return TRUE;

	return FALSE;
}

BOOL CCreatePlayer::GetRandName(LPWSTR pwszText)
{
	const WCHAR wszName1[26][2]
	{
		L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o", L"p", L"q", L"r",
			L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z"
	};

	const WCHAR wszName2[3][2]
	{
		L"灬", L"丶", L"彡"
	};

	const WCHAR wszName3[26][2]
	{
		L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R",
		L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z"
	};

	const int nMaxLen = 544;
	static WCHAR szName4[nMaxLen][5] = {
		L"邑", L"乾", L"绯", L"擎汉", L"豪英", L"剑愁", L"一笑",
		L"逊", L"蹇", L"涔", L"酬海", L"成败", L"冥幽", L"天问",
		L"紊", L"剑", L"臻", L"复天", L"忘幽", L"士萧", L"山河",
		L"凡", L"鞯", L"珩", L"严青", L"成危", L"中道", L"仇天",
		L"嵩", L"达", L"鹤", L"不斜", L"成风", L"道天", L"一斩",
		L"豪", L"烙", L"鞅", L"不可", L"成仁", L"道消", L"不二",
		L"裘", L"冥", L"霸", L"不正", L"无施", L"道罡", L"无剑",
		L"匪", L"惮", L"寇", L"无声", L"不斜", L"不凡", L"断天",
		L"戾", L"雍", L"博", L"百招", L"靖仇", L"不尤", L"远侵",
		L"瑛", L"鑫", L"匕", L"千万", L"若风", L"千筹", L"一刀",
		L"铭", L"靖", L"弼", L"随阴", L"若剑", L"断秋", L"碧空",
		L"霆", L"紊", L"碧", L"一手", L"若之", L"断缘", L"仇血",
		L"驳", L"钧", L"捕", L"三毒", L"亿先", L"道之", L"万声",
		L"疾", L"隶", L"败", L"三德", L"人达", L"剑心", L"万天",
		L"威", L"沅", L"悲", L"一德", L"人英", L"剑成", L"送终",
		L"祥", L"翎", L"羿", L"老九", L"人杰", L"剑封", L"绝山",
		L"誉", L"戎", L"储", L"老五", L"人雄", L"万言", L"绝义",
		L"枫", L"擎", L"靳", L"老四", L"元龙", L"绝悟", L"沧海",
		L"凛", L"荆", L"汲", L"十三", L"成威", L"绝施", L"万仇",
		L"涛", L"丑", L"邴", L"如豹", L"成协", L"乘风", L"万怨",
		L"卿", L"坤", L"糜", L"连虎", L"不评", L"乘云", L"万恶",
		L"豁", L"耷", L"松", L"似狮", L"老三", L"世开", L"恶天",
		L"焱", L"磬", L"井", L"大楚", L"广山", L"剑鬼", L"灭龙",
		L"虔", L"函", L"富", L"定帮", L"不言", L"远锋", L"剑身",
		L"契", L"翰", L"乌", L"伟帮", L"追命", L"远望", L"无敌",
		L"罡", L"遥", L"焦", L"大有", L"难敌", L"远山", L"半鬼",
		L"鸿", L"萧", L"奎", L"高烽", L"难破", L"广缘", L"行天",
		L"胜", L"嚣", L"稀", L"自中", L"难摧", L"天与", L"浩天",
		L"鹰", L"骁", L"班", L"天川", L"难胜", L"天抒", L"天寿",
		L"卿", L"鲂", L"仰", L"大开", L"德地", L"天奇", L"无血",
		L"城", L"剑", L"伊", L"傀儡", L"德天", L"半仙", L"聋五",
		L"康", L"筮", L"栾", L"羊青", L"世平", L"一鸣", L"如花",
		L"阁", L"勒", L"厉", L"牛青", L"世德", L"文龙", L"三问",
		L"穆", L"炳", L"戎", L"开山", L"世立", L"元正", L"非笑",
		L"扬", L"斩", L"天德", L"中恶", L"浩阑", L"蓝血",
		L"垣", L"颤", L"铁身", L"延恶", L"士晋", L"无招",
		L"师", L"孱", L"老黑", L"百川", L"铸海", L"无极",
		L"翼", L"续", L"半邪", L"世倌", L"人龙", L"鬼神",
		L"秋", L"岩", L"半山", L"连碧", L"伯云", L"满天",
		L"傥", L"奄", L"一江", L"岱周", L"老头", L"飞扬",
		L"乞", L"箴", L"十八", L"行恶", L"剑通",
		L"斌", L"莹", L"妍", L"鸣凤", L"不二", L"一一", L"大娘",
		L"茗", L"颖", L"嫣", L"无颜", L"千愁", L"摇伽", L"二娘",
		L"敏", L"盈", L"芫", L"莫言", L"稚晴", L"菲鹰", L"三娘",
		L"莆", L"樱", L"凤", L"无色", L"英姑", L"冰颜", L"四娘",
		L"姝", L"荧", L"珠", L"亚男", L"若颜", L"无心", L"涟妖",
		L"彤", L"萤", L"洙", L"素阴", L"飞飞", L"藏鸟", L"虔纹",
		L"桐", L"莺", L"竺", L"若男", L"妙芹", L"若冰", L"若魔",
		L"芝", L"瑛", L"萝", L"灭男", L"珊珊", L"嫣娆", L"绾绾",
		L"芷", L"璎", L"翠", L"高丽", L"如雪", L"唯雪", L"艳血",
		L"姿", L"芙", L"萃", L"妖丽", L"雪一", L"菲音", L"绝音",
		L"茈", L"芾", L"柔", L"三颜", L"茗茗", L"如冰", L"血茗",
		L"涫", L"湘", L"沁", L"代男", L"荧荧", L"如娆", L"凤妖",
		L"秀", L"襄", L"芹", L"踏歌", L"赛凤", L"如音", L"若血",
		L"蓉", L"香", L"衫", L"芙蓉", L"柏柳", L"一曲", L"雪冥",
		L"青", L"玲", L"珊", L"雁开", L"艳一", L"灵煌", L"冰姬",
		L"卿", L"灵", L"琳", L"东蒽", L"宝川", L"宫苴", L"绝音",
		L"清", L"绫", L"莛", L"沉鱼", L"宝莹", L"涔雨", L"邪欢",
		L"惠", L"翎", L"葶", L"落雁", L"不弱", L"太君", L"老姆",
		L"荟", L"丝", L"芮", L"凡英", L"不惜", L"莫茗", L"黎云",
		L"筝", L"姒", L"赛君", L"可仁", L"莫英", L"凤凰",
		L"真", L"苠", L"闭月", L"可兰", L"朝雪", L"一寡",
		L"贞", L"绮", L"羞花", L"可燕", L"老太", L"颜演",
		L"珍", L"淇", L"不愁", L"可愁", L"茹嫣", L"幻然",
		L"宛", L"荠", L"盛男", L"可冥", L"丹妗", L"幻悲",
		L"娩", L"琦", L"青荷", L"梨愁", L"缘郡", L"幻雪",
		L"莞", L"衣", L"清涟", L"不悔", L"冥茗", L"幻嫣",
		L"菀", L"沂", L"一兰", L"不乐", L"溪灵", L"葵阴",
		L"素", L"苡", L"如花", L"起眸", L"惋清", L"灭绝",
		L"愫", L"怡", L"聋五", L"惋庭", L"太英", L"夜阑",
		L"涑", L"悒", L"绣连", L"香芦", L"太清", L"藏今",
		L"兰", L"芳", L"金连", L"飞凤", L"善斓", L"幻姬",
		L"澜", L"芸", L"大凄", L"无春", L"善愁", L"茹妖",
		L"蓝", L"笙", L"傻姑", L"紫烟", L"善若", L"香魔",
		L"斓", L"忻", L"蛟凤", L"双双", L"嫣然", L"魂幽",
		L"纹", L"芯", L"莹芝", L"汝燕", L"阑悦", L"藏花",
		L"雯", L"紫", L"中蓝", L"玉兰", L"阑香", L"妖妖",
		L"若", L"茈", L"太兰", L"婷冉", L"岂愈", L"一凤",
		L"弱", L"芷", L"寡妇", L"秋尽", L"紫伊", L"芸遥",
		L"萍", L"颜", L"外绣", L"凤灵", L"清炎", L"行云",
		L"颦", L"艳", L"不平", L"媚颜", L"清涟", L"舞仙",
		L"婴", L"雁", L"毒娘", L"问晴", L"苑睐", L"小小"
	};

	srand(::GetTickCount());
	int nR1 = 0 + rand() % (26 - 1 - 0 + 1);
	int nR2 = 0 + rand() % (3 - 1 - 0 + 1);
	int nR3 = 0 + rand() % (26 - 1 - 0 + 1);
	int nR4 = 0 + rand() % (544 - 1 - 0 + 1);

	int nR5 = 0 + rand() % (4 - 1 - 0 + 1);
	switch (nR5)
	{
	case 0:
		lstrcatW(pwszText, szName4[nR4]);
		lstrcatW(pwszText, wszName2[nR2]);
		lstrcatW(pwszText, wszName3[nR3]);
		lstrcatW(pwszText, wszName1[nR1]);
		break;
	case 1:
		lstrcatW(pwszText, wszName2[nR2]);
		lstrcatW(pwszText, wszName3[nR3]);
		lstrcatW(pwszText, wszName1[nR1]);
		lstrcatW(pwszText, szName4[nR4]);
		break;
	case 2:
		lstrcatW(pwszText, wszName1[nR3]);
		lstrcatW(pwszText, wszName3[nR1]);
		lstrcatW(pwszText, wszName2[nR2]);
		lstrcatW(pwszText, szName4[nR4]);
		break;
	default:
		lstrcatW(pwszText, szName4[nR4]);
		lstrcatW(pwszText, wszName2[nR2]);
		lstrcatW(pwszText, wszName1[nR3]);
		lstrcatW(pwszText, wszName3[nR1]);
		break;
	}
	return TRUE;
	//////////////////////////////////////////////////////////////////////////
	/*CHAR szText[32] = { 0 };
	while (strlen(szText) < 10)
	{
		int nR4 = 0 + rand() % (nMaxLen - 1 - 0 + 1);
		lstrcatA(szText, szNameArray[nR4]);
		Sleep(500);
	}

	WCHAR wszTemp[32] = { 0 };
	ASCIIToUnicode(szText, wszTemp);
	lstrcatW(pwszText, wszTemp);*/

	return TRUE;
}