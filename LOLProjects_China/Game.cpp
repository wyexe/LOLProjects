// Game.cpp : 实现文件
//

#include "stdafx.h"
#include "AGEProjects_China.h"
#include "Game.h"
#include "afxdialogex.h"
#include "FightFunction.h"
#include "HookCode.h"
#include "PersonBuff.h"
#define _SELF L"Game.cpp"
// CGame 对话框

IMPLEMENT_DYNAMIC(CGame, CDialogEx)

CGame::CGame(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGame::IDD, pParent)
{

}

CGame::~CGame()
{
}

void CGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CGame, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_Excute, &CGame::OnBnClickedBtnExcute)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CGame::OnLvnColumnclickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CGame::OnNMClickList1)
	ON_BN_CLICKED(IDC_BTN_Telport, &CGame::OnBnClickedBtnTelport)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CGame::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGame::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CGame::OnBnClickedButton3)
	ON_MESSAGE(WM_HOTKEY, &CGame::OnHotKey)
	ON_BN_CLICKED(IDC_BUTTON4, &CGame::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CGame::OnBnClickedButton5)
END_MESSAGE_MAP()

VOID InitCCB(CGame * dlg)
{
	CComboBox * CCB = (CComboBox *)dlg->GetDlgItem(IDC_CCB_CMD);
	CCB->InsertString(0, L"遍历怪物");
	CCB->InsertString(1, L"野怪");
	CCB->InsertString(2, L"遍历主动技能");
	CCB->InsertString(3, L"遍历背包");
	CCB->InsertString(4, L"Buff");
	CCB->InsertString(5, L"Test");
}

HANDLE hTestThread = NULL;
HANDLE hMonsterThread = NULL;
HANDLE hTaskThread = NULL;
HANDLE hFloatingThread = NULL;

typedef struct _PacketSrc
{
	DWORD dwParm1;
	DWORD dwParm2;
	DWORD dwType;
}PacketSrc;

static PacketSrc gs_PacketSrc;
vector<PacketSrc> g_vlst;
CLLock g_vlst_Lock;
CRITICAL_SECTION g_cr;
void AddPacket(DWORD m_dwESP)
{
	EnterCriticalSection(&g_cr);
	gs_PacketSrc.dwParm1 = ReadDWORD(m_dwESP + 0x18);
	gs_PacketSrc.dwParm2 = ReadDWORD(m_dwESP + 0x14);
	gs_PacketSrc.dwType = ReadDWORD(ReadDWORD(m_dwESP + 0x10) + 0x4);

	DWORD dwAddr = ReadDWORD(m_dwESP + 0x10);;
	switch (gs_PacketSrc.dwType)
	{
	case 0xC9: case 0x6A: case 0x97: case 0x6:
		
		break;
	default:
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"dwType:%X", gs_PacketSrc.dwType);
		break;
	}

	BOOL bExist = FALSE;
	for (auto& i : g_vlst)
	{
		if (i.dwType == gs_PacketSrc.dwType)
		{
			bExist = TRUE;
			break;
		}
	}

	if (!bExist)
	{
		g_vlst.push_back(gs_PacketSrc);
	}
	LeaveCriticalSection(&g_cr);
}

static DWORD dwESP = 0;
__declspec(naked) void HookSend()
{
	__asm MOV dwESP,ESP
	__asm PUSHAD

	// [ESP+0x10]==Packet  +14,+18
	// [[ESP+0x10]+0x4] == Type

	//CPrintLog::PrintLog_W(_SELF, __LINE__, L"Send Parm1:%X,Parm2:%X,Type:%X", ReadDWORD(dwESP + 0x18), ReadDWORD(dwESP + 0x14), ReadDWORD(ReadDWORD(dwESP + 0x10) + 0x4));
	AddPacket(dwESP);
	

	__asm POPAD
	__asm SUB ESP,0xC
	__asm PUSH EBX
	__asm PUSH ESI
	__asm PUSH 0x7EF143
	__asm RETN
}


DWORD WINAPI TestThread(LPVOID lpParm)
{
	g_MapType = MapType_召唤师峡谷;
	CFightFunction CFF;
	CFF.PartyStart();
	//CGameFunction::HookSend();
	//InitializeCriticalSection(&g_cr);

	/*MYHOOK_CONTENT HC;
	HC.dwFunAddr = (DWORD)HookSend;
	HC.dwHookAddr = 0x007EF13E;
	HC.dwOldCode1 = ReadDWORD(HC.dwFunAddr + 0x0);
	HC.dwOldCode2 = ReadDWORD(HC.dwFunAddr + 0x4);
	HC.uNopCount = 0;
	CLHook::Hook_Fun_Jmp_MyAddr(&HC);*/

	/*while (true)
	{
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"g_vlst.size()=%d", g_vlst.size());
		Sleep(5 * 1000);
	}*/
	

	/*auto fnFillStydyStruct = [](LPVOID pAddr)
	{
		_asm
		{
			MOV ECX, pAddr
			MOV EAX, 0xEB6890
			CALL EAX
		}
	};

	auto fnEncryptIndex = [](DWORD dwIndex)
	{
		BYTE bResult = 0;
		_asm
		{
			MOV ECX,dwIndex
			MOV EAX, 0xF379B0
			CALL EAX
			MOV bResult, AL
		}
		return bResult;
	};

	//dd [[[[150C674]+2428+14]+4]+90]
	DWORD dwFlag = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(Role_base) + Role_Skill_offset + 0x14) + 0x4) + 0x90));

	DWORD dwSkillIndex = 1;
	static BYTE dwArray[64] = { 0 };
	ZeroMemory(dwArray, sizeof(dwArray));
	__asm MOV EAX,EAX
	__asm MOV EAX, EAX
	// 填充加密结构
	fnFillStydyStruct(dwArray);

	// 填入加密的技能索引至发包
	dwArray[13] = fnEncryptIndex(dwSkillIndex);

	// 填入Flag
	memcpy(dwArray + 0x6, &dwFlag, 4);

	_asm
	{
		LEA EAX,dwArray
		
		PUSH 0
		PUSH 1
		PUSH EAX
		MOV ECX, 0x319CE00
		MOV ECX, [ECX]
		MOV EAX, [ECX]
		MOV EAX, [EAX + 0x34]
		CALL EAX
	}*/

	/*auto fnFillBuyItemStruct = [](LPVOID pAddr)
	{
		_asm
		{
			MOV ECX, pAddr
				MOV EAX, 0xC34BD0
				CALL EAX
		}
	};

	auto fnEncryptIndex = [](DWORD dwIndex)
	{
		BYTE bResult = 0;
		_asm
		{
			MOV ECX, dwIndex
				MOV EAX, 0xF379B0
				CALL EAX
				MOV bResult, AL
		}
		return bResult;
	};

	//dd [[[[150C674]+2428+14]+4]+90]
	DWORD dwFlag = ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(ReadDWORD(Role_base) + Role_Skill_offset + 0x14) + 0x4) + 0x90));

	DWORD dwItemId = 0x7D3;
	static BYTE dwArray[64] = { 0 };
	ZeroMemory(dwArray, sizeof(dwArray));
	__asm MOV EAX, EAX
	__asm MOV EAX, EAX
	// 填充加密结构
	fnFillBuyItemStruct(dwArray);

	// 填入Flag
	memcpy(dwArray + 0x6, &dwFlag, 4);

	// 填入购买ID
	memcpy(dwArray + 0xA, &dwItemId, 4);

	// 将0x7D3 分成BYTE, 0xD3, 0x07, 0x0, 0x0 去加密
	// 将购买Id加密
	for (int i = 0; i < 4; ++i)
	{
		BYTE bResult = fnEncryptIndex(dwItemId >> (i * 8) & 0xFF);
		memcpy(dwArray + 0xA + i, &bResult, 1);
	}

	_asm
	{
		LEA EAX, dwArray

			PUSH 0
			PUSH 1
			PUSH EAX
			MOV ECX, 0x319CE00
			MOV ECX, [ECX]
			MOV EAX, [ECX]
			MOV EAX, [EAX + 0x34]
			CALL EAX
	}*/

	hTestThread = NULL;
	return 0;
}

// CGame 消息处理程序
BOOL bRunF4 = FALSE;
BOOL CGame::OnInitDialog()
{
	srand((UINT)time(NULL));
	InitCCB(this);
	CListCtrl * m_List = ((CListCtrl *)GetDlgItem(IDC_LIST1));
	m_List->SetExtendedStyle(m_List->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	if (g_pAccountGame == NULL)
	{
		g_pAccountGame = new ACCOUNT_INFO_GAME;
		memset(g_pAccountGame, 0, sizeof(ACCOUNT_INFO_GAME));
	}


	while (g_pAccountGame->hWndGame == NULL)
	{
		g_pAccountGame->hWndGame = ::FindWindowW(L"RiotWindowClass", NULL);
		Sleep(1000);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"寻找窗口中……");
	}

	g_dwMainThreadId = CLThread::GetMainThreadId();
	CPrintLog::PrintLog_W(_SELF, __LINE__, L"MainThreadId=%X", g_dwMainThreadId);
	//g_dwStartGameCmd = GameCmd_Start;
	//hTestThread = chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)TestThread, this, NULL, NULL);
	return TRUE;
}

 
DWORD WINAPI MonsterThread(LPVOID lpParm)
{
	CStringA str;
	for (auto& i : g_vlst)
	{
		CStringA strTemp;
		strTemp.Format("SendPacket(%X,%X,%X)\r\n", i.dwParm1, i.dwParm2, i.dwType);
		str.Append(strTemp);
	}

	// 保存
	CFile CF;
	if (CF.Open(L"C:\\1.txt", CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate))
	{
		CF.SeekToEnd();
		CF.Write(str.GetBuffer(), str.GetLength());
		CF.Close();
	}

	return 0;
}


void CGame::OnBnClickedBtnExcute()
{
	CComboBox * CCB = (CComboBox*)GetDlgItem(IDC_CCB_CMD);
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_Copy);
	CListCtrl * m_List = (CListCtrl *)GetDlgItem(IDC_LIST1);
	CString str;
	m_List->DeleteAllItems();
	int nIndex = CCB->GetCurSel();
	int nColumnCount = m_List->GetHeaderCtrl()->GetItemCount();
	for (int i = 0;i < nColumnCount; i++)
	{
		m_List->DeleteColumn(0);
	}

	if ( nIndex == 0 )
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"HP/MAXHP/P", LVCFMT_LEFT, 80);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"MP/MAXMP/P", LVCFMT_LEFT, 80);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Camp", LVCFMT_LEFT, 50);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"MonsterType", LVCFMT_LEFT, 50);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Level", LVCFMT_LEFT, 30);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Name", LVCFMT_LEFT, 80);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Point", LVCFMT_LEFT, 200);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Dis", LVCFMT_LEFT, 50);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Temp", LVCFMT_LEFT, 50);

		CGameFunction GF;
		vector<CMonster> vlst;
		CMonster::GetAroundObjectList(vlst);

		for (UINT i = 0; i < vlst.size(); ++i)
		{
			CMonster Monster = vlst.at(i);

			str.Format(L"%X",Monster.GetBase());
			int nRow = m_List->InsertItem(m_List->GetItemCount(), str);

			str.Format(L"%d/%d/%d",Monster.GetHp(), Monster.GetMaxHp(), Monster.GetPercentHp());
			m_List->SetItemText(nRow, 1, str);

			str.Format(L"%d/%d/%d", Monster.GetMp(), Monster.GetMaxMp(), Monster.GetPercentHp());
			m_List->SetItemText(nRow, 2, str);

			str.Format(L"%X", Monster.GetCamp());
			m_List->SetItemText(nRow, 3, str);

			str.Format(L"%X", ReadDWORD(Monster.GetBase() + 0x18) );
			m_List->SetItemText(nRow, 4, str);

			str.Format(L"%X", Monster.GetLevel());
			m_List->SetItemText(nRow, 5, str);

			m_List->SetItemText(nRow, 6, Monster.GetName());

			Point pt = Monster.GetPoint();
			str.Format(L"%.2f,%.2f,%.2f", pt.X, pt.Y, pt.Z);
			m_List->SetItemText(nRow, 7, str);

			str.Format(L"%.2f", Monster.GetDis(PERSON->GetPoint()));
			m_List->SetItemText(nRow, 8, str);

			str.Format(L"%X", Monster.IsShow());
			m_List->SetItemText(nRow, 9, str);
			//CPrintLog::PrintLog_W(_SELF, __LINE__, L"Base=%X,Name=%s,dwResult=%X,+354+24=%X", dwBase, Monster.GetName(), dwResult, ReadBYTE(dwResult + 0x354));
		}
		
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"SkillPoint:%d,OccName=%s", PERSON->GetSkillPoint(), PERSON->GetHeroName());
	}
	else if (nIndex == 1)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Id", LVCFMT_LEFT, 30);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Name", LVCFMT_LEFT, 150);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"IsRefush", LVCFMT_LEFT, 30);
		
		CGameFunction GF;
		vector<CField> vlst;
		CField::GetFieldList(vlst);

		for (UINT i = 0; i < vlst.size(); ++i)
		{
			CField FD = vlst.at(i);
			str.Format(L"%X", FD.GetBase());
			int nRow = m_List->InsertItem(m_List->GetItemCount(), str);

			str.Format(L"%X", FD.GetId());
			m_List->SetItemText(nRow, 1, str);

			m_List->SetItemText(nRow, 2, FD.GetName());

			str.Format(L"%s", FD.IsRefush() ? L"Yes" : L"No");
			m_List->SetItemText(nRow, 3, str);
		}
	}
	else if (nIndex == 2)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Level", LVCFMT_LEFT, 30);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Name", LVCFMT_LEFT, 150);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Mp", LVCFMT_LEFT, 30);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"isCool", LVCFMT_LEFT, 30);

		CGameFunction GF;
		vector<CSkill> vlst;
		CSkill::GetSkillList(vlst);

		for (UINT i = 0; i < vlst.size(); ++i)
		{
			CSkill Skill = vlst.at(i);
			str.Format(L"%X",Skill.GetBase());
			int nRow = m_List->InsertItem(m_List->GetItemCount(), str);

			str.Format(L"%d", Skill.GetSkillLevel());
			m_List->SetItemText(nRow, 1, str);

			m_List->SetItemText(nRow, 2, Skill.GetName());

			str.Format(L"%d", Skill.GetUseMp());
			m_List->SetItemText(nRow, 3, str);

			m_List->SetItemText(nRow, 4, Skill.IsCool() ? L"Yes" : L"No");
		}
	}
	else if (nIndex == 3)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Count", LVCFMT_LEFT, 50);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Parm", LVCFMT_LEFT, 80);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Id", LVCFMT_LEFT, 50);

		CGameFunction GF;
		vector<CEqument> vlst;
		CEqument::GetEuqmentList(vlst);

		for (UINT i = 0; i < vlst.size(); ++i)
		{
			auto EI = vlst.at(i);
			str.Format(L"%X", EI.GetBase());
			int nRow = m_List->InsertItem(m_List->GetItemCount(), str);

			str.Format(L"%d", EI.GetCount());
			m_List->SetItemText(nRow, 1, str);

			str.Format(L"%X", EI.GetParm());
			m_List->SetItemText(nRow, 2, str);

			str.Format(L"%X", EI.GetId());
			m_List->SetItemText(nRow, 3, str);
		}
	}
	else if (nIndex == 4)
	{
		m_List->InsertColumn(0, L"Base", LVCFMT_LEFT, 80);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Name", LVCFMT_LEFT, 100);
		m_List->InsertColumn(m_List->GetHeaderCtrl()->GetItemCount(), L"Id", LVCFMT_LEFT, 50);

		vector<CPersonBuff> vlst;
		CPersonBuff::GetPersonBuffLst(vlst);

		for (auto& PersonBuff : vlst)
		{
			str.Format(L"%X", PersonBuff.GetBase());
			int nRow = m_List->InsertItem(m_List->GetItemCount(), str);

			m_List->SetItemText(nRow, 1, PersonBuff.GetName());

			str.Format(L"%X", PersonBuff.GetId());
			m_List->SetItemText(nRow, 2, str);

		}
	}
	else if (nIndex == 5)
	{
		
	}
}

int sort_column;
bool method;
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int row1 = (int) lParam1;
	int row2 = (int) lParam2;
	WCHAR * p = NULL;
	CListCtrl* lc = (CListCtrl*)lParamSort;
	CString lp1 = lc->GetItemText(row1,sort_column);
	CString lp2 = lc->GetItemText(row2,sort_column);
	//PrintDebug_W(_SELF,__LINE__,L"lp1=%s,lp2=%s",lp1,lp2);


	if ( method )
		return wcstol(lp1,&p,16) - wcstol(lp2,&p,16);
	else
		return wcstol(lp2,&p,16) - wcstol(lp1,&p,16);

	return 0;
}

void CGame::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	sort_column = pNMLV->iSubItem;//点击的列

	CListCtrl * m_List = (CListCtrl *)GetDlgItem(IDC_LIST1);

	int count = m_List->GetItemCount();

	for (int i = 0; i < count; ++i)
	{
		m_List->SetItemData(i,i); // 每行的比较关键字，此处为列序号（点击的列号），可以设置为其他 比较函数的第一二个参数
	}
	m_List->SortItems(MyCompareProc,(DWORD_PTR)m_List);//排序 第二个参数是比较函数的第三个参数
	method = !method;

	*pResult = 0;
}


void CGame::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{
		CListCtrl * m_list = (CListCtrl *)GetDlgItem(IDC_LIST1);
		CString str = m_list->GetItemText(pNMListView->iItem,pNMListView->iSubItem);
		((CEdit*)GetDlgItem(IDC_EDIT_Copy))->SetWindowTextW(str);
	}
	*pResult = 0;
}


void CGame::OnBnClickedBtnTelport()
{
	
}


void CGame::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (g_pAccountGame != NULL)
		delete g_pAccountGame;
	//delete g_pSharedInfo;
	if ( hTestThread != NULL )
	{
		TerminateThread(hTestThread, 0);
		WaitForSingleObject(hTestThread, 3000);
		hTestThread = NULL;
	}
	if (hMonsterThread != NULL)
	{
		TerminateThread(hMonsterThread, 0x0);
		CloseHandle(hMonsterThread);
		hMonsterThread = NULL;
	}

	if (hTaskThread != NULL)
	{
		TerminateThread(hTaskThread, 0);
		CloseHandle(hTaskThread);
		hTaskThread = NULL;
	}

	if (hFloatingThread != NULL)
	{
		TerminateThread(hFloatingThread, 0);
		CloseHandle(hFloatingThread);
		hFloatingThread = NULL;
	}

	HANDLE curProcess = ::GetCurrentProcess();
	for (UINT i = 0; i < g_HookItemList.size(); ++i)
	{
		MYHOOK_CONTENT HC = g_HookItemList.at(i);

		if (HC.dwFlag == 0x2)
		{
			CCharacter::WriteDWORD(HC.dwHookAddr, HC.dwOldCode1);
			continue;
		}
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"HC.dwAddr1=%X,dwCode1=%X", HC.dwHookAddr, HC.dwOldCode1);
		CPrintLog::PrintLog_W(_SELF, __LINE__, L"HC.dwAddr2=%X,dwCode2=%X", HC.dwHookAddr + 4, HC.dwOldCode2);
		::WriteProcessMemory(curProcess, (LPVOID)HC.dwHookAddr, &HC.dwOldCode1, 4, 0);
		::WriteProcessMemory(curProcess, (LPVOID)(HC.dwHookAddr + 4), &HC.dwOldCode2, 4, 0);
	}

	g_HookItemList.clear();

	CDialogEx::OnClose();
}


void CGame::OnBnClickedButton1()
{
	if (hTestThread == NULL)
	{
		g_dwStartGameCmd = GameCmd_Start;
		hTestThread = chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)TestThread, this, NULL, NULL);
	}
	else
	{
		g_dwStartGameCmd = GameCmd_Stop;
		if (WaitForSingleObject(hTestThread, 10 * 1000) == WAIT_TIMEOUT)
		{
			TerminateThread(hTestThread, 0);
		}
		CloseHandle(hTestThread);
		hTestThread = NULL;
	}
}


void CGame::OnBnClickedButton2()
{
	chBEGINTHREADEX(NULL, NULL, MonsterThread, this, NULL, NULL);
}


void CGame::OnBnClickedButton3()
{

}

LRESULT CGame::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	
	return FALSE;
}

void CGame::OnBnClickedButton4()
{

}


void CGame::OnBnClickedButton5()
{
	
}
