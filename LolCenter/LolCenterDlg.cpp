
// LolCenterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LolCenter.h"
#include "LolCenterDlg.h"
#include "afxdialogex.h"
#include "UpdateAccountDlg.h"

#define _SELF L"LOlCenterDlg.cpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLolCenterDlg dialog

typedef int(__cdecl * fnStartSocket)(void);
typedef int(__cdecl * PauseSocket)(void);
typedef int(__cdecl * RecoverySocket)(void);
typedef int(__cdecl * SetAccountList)(const vector<ServerAccountInfo>&);
typedef int(__cdecl * GetAccountList)(vector<ServerAccountInfo>&);
typedef void(__cdecl * fnCloseALLClient)(void);

fnStartSocket pfnStartSocket = NULL;
PauseSocket pPauseSocket = NULL;
RecoverySocket pRecoverySocket = NULL;
SetAccountList pSetAccountList = NULL;
GetAccountList pGetAccountList = NULL;
fnCloseALLClient pfnCloseALLClient = NULL;


CLolCenterDlg::CLolCenterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLolCenterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLolCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLolCenterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_IMPORTACCOUNT, &CLolCenterDlg::OnBnClickedBtnImportaccount)
ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &CLolCenterDlg::OnLvnColumnclickList1)
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BTN_SaveData, &CLolCenterDlg::OnBnClickedBtnSavedata)
ON_BN_CLICKED(IDC_BTN_Start, &CLolCenterDlg::OnBnClickedBtnStart)
ON_BN_CLICKED(IDC_BTN_UpdateAccount, &CLolCenterDlg::OnBnClickedBtnUpdateaccount)
ON_BN_CLICKED(IDC_BTN_Pause, &CLolCenterDlg::OnBnClickedBtnPause)
ON_BN_CLICKED(IDC_BTN_Recovery, &CLolCenterDlg::OnBnClickedBtnRecovery)
ON_BN_CLICKED(IDC_BTN_CLOSECLINET, &CLolCenterDlg::OnBnClickedBtnCloseclinet)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CLolCenterDlg::OnNMCustomdrawList1)
END_MESSAGE_MAP()

BOOL CloseSelf_By_Time(CLolCenterDlg* dlg)
{
	return TRUE;
	SYSTEMTIME stLocal;
	::GetLocalTime(&stLocal);

	if (stLocal.wDayOfWeek == 5 && stLocal.wHour >= 9 && stLocal.wHour < 10)
	{
		// 星期五的早上 9点-10点 这段时间不打!
		dlg->PostMessageW(WM_CLOSE);
	}

	return TRUE;
}

BOOL g_bRefush = FALSE;
HANDLE hRefushListThread = NULL;
DWORD WINAPI _RefushListThread(LPVOID lpParm)
{
	CLolCenterDlg* dlg = (CLolCenterDlg *)lpParm;

	DWORD dwTick = ::GetTickCount();
	while (true)
	{

		if (::GetTickCount() - dwTick >= 30 * 1000 || g_bRefush)
		{
			CloseSelf_By_Time(dlg);

			// 刷新界面
			auto fnConvert = [](const ServerAccountInfo& __ServerAccountInfo)
			{
				static AccountConfigInfo __AccountConfigInfo;
				ZeroMemory(&__AccountConfigInfo, sizeof(__AccountConfigInfo));

				CCharacter::wstrcpy_my(__AccountConfigInfo.wszQQ, __ServerAccountInfo.wszQQ);
				CCharacter::wstrcpy_my(__AccountConfigInfo.wszPass, __ServerAccountInfo.wszPass);
				CCharacter::wstrcpy_my(__AccountConfigInfo.wszServer, __ServerAccountInfo.wszServer);
				CCharacter::wstrcpy_my(__AccountConfigInfo.wszMapName, __ServerAccountInfo.wszMapName);
				CCharacter::wstrcpy_my(__AccountConfigInfo.wszComputerName, __ServerAccountInfo.wszComputerName);
				__AccountConfigInfo.PlayMode = __ServerAccountInfo.PlayMode;
				__AccountConfigInfo.dwConditionGold = __ServerAccountInfo.dwMaxGold;
				__AccountConfigInfo.dwConditionLevel = __ServerAccountInfo.dwMaxLevel;
				return &__AccountConfigInfo;
			};

			vector<ServerAccountInfo> vlst;
			if (::GetTickCount() - dwTick >= 30 * 1000 && pGetAccountList != NULL)
			{
				// 从Server获取数据
				pGetAccountList(vlst);
				AccountListLock.Access([&vlst]{
					AccountList.clear();
					AccountList.assign(vlst.begin(), vlst.end());
				});
			}
			else
			{
				// 总表拷贝数据
				AccountListLock.Access([&vlst]{
					vlst.assign(AccountList.begin(), AccountList.end());
				});
			}
			

			// 删除多余的数据
			dlg->DeleteListItem(vlst);

			// 修改已有的数据
			dlg->UpdateListItem(vlst);

			// 添加新增的数据
			for (auto& __ServerAccountInfo : vlst)
			{
				int nIndex = dlg->ExistList(__ServerAccountInfo.wszQQ);
				if (nIndex == -1)
				{
					dlg->AddListItem(*fnConvert(__ServerAccountInfo));
				}
			}

			dwTick = ::GetTickCount();
			g_bRefush = FALSE;
		}
		Sleep(1 * 1000);
	}
	return 0;
}

DWORD WINAPI _ReadConfigThread(LPVOID lpParm)
{
	CLolCenterDlg* dlg = (CLolCenterDlg*)lpParm;
	CListCtrl* pList = (CListCtrl*)dlg->GetDlgItem(IDC_LIST1);
	// Set FULLRowSelect
	pList->SetExtendedStyle(pList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	pList->SetTextBkColor(RGB(240, 247, 233));

	// 还原List
	pList->DeleteAllItems();
	int nColumnCount = pList->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < nColumnCount; i++)
		pList->DeleteColumn(0);

	pList->InsertColumn(0, L"序号", LVCFMT_LEFT, 50);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"编号", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"QQ", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"昵称", LVCFMT_LEFT, 100);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"大区", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"地图", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"等级条件", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"金币条件", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"局数", LVCFMT_LEFT, 50);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"模式", LVCFMT_LEFT, 80);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"状态", LVCFMT_LEFT, 100);
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"消息", LVCFMT_LEFT, 300);

	vector<AccountConfigInfo> vlst;

	////////初始化, 第一次!//////////////////////////////////////////////////////////////////
	if (!CAccountConfig::ReadAccountConfig(vlst))
	{
		((CButton*)dlg->GetDlgItem(IDC_BTN_IMPORTACCOUNT))->EnableWindow(TRUE);
		((CEdit*)dlg->GetDlgItem(IDC_EDIT_Log))->SetWindowTextW(L"读取帐号失败!");
		return 0;
	}

	try
	{
		auto fnExistItem = [&vlst](LPCWSTR pwszQq)
		{
			for (auto& __ServerAccountInfo : AccountList)
			{
				if (CCharacter::wstrcmp_my(pwszQq, __ServerAccountInfo.wszQQ))
					return true;
			}
			return false;
		};


		srand(::GetTickCount());
		for (auto& __AccountConfigInfo : vlst)
		{
			if (fnExistItem(__AccountConfigInfo.wszQQ))
			{
				CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"帐号:%s已经存在了,就不重复添加了!", __AccountConfigInfo.wszQQ);
				continue;
			}

			dlg->AddListItem(__AccountConfigInfo);

			// 转换到全局表
			static ServerAccountInfo __ServerAccountInfo;
			ZeroMemory(&__ServerAccountInfo, sizeof(__ServerAccountInfo));

			CCharacter::wstrcpy_my(__ServerAccountInfo.wszQQ, __AccountConfigInfo.wszQQ);
			CCharacter::wstrcpy_my(__ServerAccountInfo.wszPass, __AccountConfigInfo.wszPass);
			CCharacter::wstrcpy_my(__ServerAccountInfo.wszServer, __AccountConfigInfo.wszServer);
			CCharacter::wstrcpy_my(__ServerAccountInfo.wszMapName, __AccountConfigInfo.wszMapName);
			CCharacter::wstrcpy_my(__ServerAccountInfo.wszComputerName, __AccountConfigInfo.wszComputerName);
			__ServerAccountInfo.dwMaxLevel = __AccountConfigInfo.dwConditionLevel;
			__ServerAccountInfo.dwMaxGold = __AccountConfigInfo.dwConditionGold;
			__ServerAccountInfo.dwQqCrc = CLPublic::GetCRC32_DWORD((LPSTR)__ServerAccountInfo.wszQQ, wcslen(__ServerAccountInfo.wszQQ) * 2 + 1);
			__ServerAccountInfo.PlayMode = __AccountConfigInfo.PlayMode;

			if (wcslen(__ServerAccountInfo.wszComputerName) == 0) // 计算Crc
				__ServerAccountInfo.dwComputerNameCrc = NULL;
			else
				__ServerAccountInfo.dwComputerNameCrc = CLPublic::GetCRC32_DWORD((LPSTR)__ServerAccountInfo.wszComputerName, wcslen(__ServerAccountInfo.wszComputerName) * 2 + 1);

			CCharacter::wstrcpy_my(__ServerAccountInfo.wszStatus, L"尚未开始!");
			AccountList.push_back(__ServerAccountInfo);
		}
	}
	catch (...)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"_ReadConfigThread出现异常");
	}


	hRefushListThread = chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)_RefushListThread, dlg, NULL, NULL);
	((CButton*)dlg->GetDlgItem(IDC_BTN_Start))->ShowWindow(SW_SHOW);
	((CButton*)dlg->GetDlgItem(IDC_BTN_IMPORTACCOUNT))->ShowWindow(SW_HIDE);
	((CButton*)dlg->GetDlgItem(IDC_BTN_UpdateAccount))->ShowWindow(SW_SHOW);
	((CEdit*)dlg->GetDlgItem(IDC_EDIT_Log))->SetWindowTextW(L"读取完毕!");

	return 0;
}

BOOL CLolCenterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLolCenterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLolCenterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLolCenterDlg::AddListItem(const AccountConfigInfo& __AccountConfigInfo)
{
	int nCount = 0;
	static WCHAR wszTemp[64];

	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	int nRow = pList->InsertItem(pList->GetItemCount(), CAccountConfig::FormatNumber(pList->GetItemCount() + 1));	// 序号
	pList->SetItemText(nRow, ++nCount, __AccountConfigInfo.wszComputerName);					// 编号
	pList->SetItemText(nRow, ++nCount, __AccountConfigInfo.wszQQ);	// QQ
	pList->SetItemText(nRow, ++nCount, L"NULL");					// 昵称

	pList->SetItemText(nRow, ++nCount, __AccountConfigInfo.wszServer);	// 大区

	pList->SetItemText(nRow, ++nCount, __AccountConfigInfo.wszMapName);	// 地图

	wsprintfW(wszTemp, L"0/%d", __AccountConfigInfo.dwConditionLevel); // 当前等级/条件等级
	pList->SetItemText(nRow, ++nCount, wszTemp);

	wsprintfW(wszTemp, L"0/%d", __AccountConfigInfo.dwConditionGold); // 当前金币/条件金币
	pList->SetItemText(nRow, ++nCount, wszTemp);

	pList->SetItemText(nRow, ++nCount, L"0");							// 局数

	pList->SetItemText(nRow, ++nCount, __AccountConfigInfo.PlayMode == AccountPlayMode_Default ? L"默认模式" : L"自定义模式");
	pList->SetItemText(nRow, ++nCount, L"NULL");
	pList->SetItemText(nRow, ++nCount, L"NULL");
}

int CLolCenterDlg::ExistList(LPCWSTR pwszQq)
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	for (int i = 0; i < pList->GetItemCount(); ++i)
	{
		CString strText = pList->GetItemText(i, 2);
		if (CCharacter::wstrcmp_my(pwszQq, strText.GetBuffer()))
		{
			return i;
		}
	}
	return -1;
}

void CLolCenterDlg::DeleteListItem(const vector<ServerAccountInfo>& vlst)
{
	auto fnExistItem = [&vlst](LPCWSTR pwszQq)
	{
		for (auto& __ServerAccountInfo : vlst)
		{
			if (CCharacter::wstrcmp_my(pwszQq, __ServerAccountInfo.wszQQ))
				return true;
		}
		return false;
	};

	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	for (int i = 0; i < pList->GetItemCount(); ++i)
	{
		CString strText = pList->GetItemText(i, 2);
		if (!fnExistItem(strText.GetBuffer()))
		{
			pList->DeleteItem(i--);
		}
	}
}

VOID CLolCenterDlg::UpdateListItem(const vector<ServerAccountInfo>& vlst)
{
	auto fnGetItem = [&vlst](LPCWSTR pwszQq)
	{
		for (auto& __ServerAccountInfo : vlst)
		{
			if (CCharacter::wstrcmp_my(pwszQq, __ServerAccountInfo.wszQQ))
				return &__ServerAccountInfo;
		}
		return (const ServerAccountInfo*)NULL;
	};


	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	for (int i = 0; i < pList->GetItemCount(); ++i)
	{
		CString strQqText = pList->GetItemText(i, 2);
		const ServerAccountInfo* pServerAccountInfo = fnGetItem(strQqText.GetBuffer());
		if (pServerAccountInfo != NULL)
		{
			// 大区 等级， 金币, 模式
			pList->SetItemText(i, 1, pServerAccountInfo->wszComputerName); // 编号

			pList->SetItemText(i, 3, pServerAccountInfo->wszNiceName);
			pList->SetItemText(i, 4, pServerAccountInfo->wszServer); // 大区
			pList->SetItemText(i, 9, pServerAccountInfo->PlayMode == AccountPlayMode_Default ? L"默认模式" : L"自定义模式"); // 模式
			pList->SetItemText(i, 5, pServerAccountInfo->wszMapName); // 地图
			pList->SetItemText(i, 6, CAccountConfig::FormatNumber(pServerAccountInfo->dwLevel, pServerAccountInfo->dwMaxLevel)); // 等级
			pList->SetItemText(i, 7, CAccountConfig::FormatNumber(pServerAccountInfo->dwGold, pServerAccountInfo->dwMaxGold)); // 金币
			pList->SetItemText(i, 8, CAccountConfig::FormatNumber(pServerAccountInfo->dwFightCount)); // 局数
																		// 9是模式
			pList->SetItemText(i, 10, pServerAccountInfo->wszStatus); // Socket消息
			pList->SetItemText(i, 11, pServerAccountInfo->wszMsg); // 客户端返回的消息
		}
	}

}

void CLolCenterDlg::OnBnClickedBtnImportaccount()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_BTN_IMPORTACCOUNT);
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	if (pList == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"pList = NULL");
		return;
	}
	if (pButton == NULL)
	{
		CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"pButton = NULL");
		return;
	}

	chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)_ReadConfigThread, this, NULL, NULL);
	pButton->EnableWindow(FALSE);
}

typedef struct _SortData_List
{
	int nColumnIndex;
	CListCtrl* pList;
	BOOL bMethod;
}SortData_List;

static int CALLBACK CompProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SortData_List* pSortData_List = (SortData_List *)lParamSort;
	int nRowIndex1 = (int)lParam1;
	int nRowIndex2 = (int)lParam2;

	if (pSortData_List->nColumnIndex == 0 || pSortData_List->nColumnIndex == 7) // 按照序号排序 按照局数排序
	{
		CString strRow1 = pSortData_List->pList->GetItemText(nRowIndex1, pSortData_List->nColumnIndex);
		CString strRow2 = pSortData_List->pList->GetItemText(nRowIndex2, pSortData_List->nColumnIndex);

		if (strRow1 == L"NULL" || strRow2 == L"NULL")
			return 0;

		if (pSortData_List->bMethod)
			return _wtoi(strRow1.GetBuffer()) - _wtoi(strRow2.GetBuffer());
		else
			return _wtoi(strRow2.GetBuffer()) - _wtoi(strRow1.GetBuffer());
	}
	else if (pSortData_List->nColumnIndex == 5 || pSortData_List->nColumnIndex == 6) // 按照等级排序 按照金币排序
	{
		CString strRow1 = pSortData_List->pList->GetItemText(nRowIndex1, pSortData_List->nColumnIndex);
		CString strRow2 = pSortData_List->pList->GetItemText(nRowIndex2, pSortData_List->nColumnIndex);

		strRow1 = strRow1.Mid(0, strRow1.Find(L"/"));
		strRow2 = strRow2.Mid(0, strRow2.Find(L"/"));

		if (strRow1 == L"NULL" || strRow2 == L"NULL")
			return 0;
		
		if (pSortData_List->bMethod)
			return _wtoi(strRow1.GetBuffer()) - _wtoi(strRow2.GetBuffer());
		else
			return _wtoi(strRow2.GetBuffer()) - _wtoi(strRow1.GetBuffer());
	}
	return 0;
}

void CLolCenterDlg::OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	static BOOL bOrderMethod = TRUE;
	static BOOL bLevelMethod = TRUE;
	static BOOL bGoldMethod = TRUE;
	static BOOL bFightCountMethod = TRUE;

	// 获取选中的列表
	SortData_List __SortData_List = { 0 };
	__SortData_List.nColumnIndex = pNMLV->iSubItem;
	__SortData_List.pList = (CListCtrl*)GetDlgItem(IDC_LIST1);

	for (int i = 0; i < __SortData_List.pList->GetItemCount(); ++i)
	{
		__SortData_List.pList->SetItemData(i, i); // 每行的比较关键字，此处为列序号（点击的列号），可以设置为其他 比较函数的第一二个参数
	}

	switch (pNMLV->iSubItem)
	{
	case 0: // 按照序号排序
		__SortData_List.bMethod = bOrderMethod;
		__SortData_List.pList->SortItems(CompProc, (DWORD_PTR)&__SortData_List);
		bOrderMethod = !bOrderMethod;
		break;
	case 5: // 按照等级排序
		__SortData_List.bMethod = bLevelMethod;
		__SortData_List.pList->SortItems(CompProc, (DWORD_PTR)&__SortData_List);
		bLevelMethod = !bLevelMethod;
		break;
	case 6: // 按照金币排序
		__SortData_List.bMethod = bGoldMethod;
		__SortData_List.pList->SortItems(CompProc, (DWORD_PTR)&__SortData_List);
		bGoldMethod = !bGoldMethod;
		break;
	case 7: // 按照局数排序
		__SortData_List.bMethod = bFightCountMethod;
		__SortData_List.pList->SortItems(CompProc, (DWORD_PTR)&__SortData_List);
		bFightCountMethod = !bFightCountMethod;
		break;
	default:
		// 其他列表不排序
		break;
	}

	*pResult = 0;
}

void CLolCenterDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (hRefushListThread != NULL)
	{
		::TerminateThread(hRefushListThread, 0);
		::CloseHandle(hRefushListThread);
		hRefushListThread = NULL;
	}
	//OnBnClickedBtnSavedata();
	CDialogEx::OnClose();
}

void CLolCenterDlg::OnBnClickedBtnSavedata()
{
	CAccountConfig::SaveAccountConfig();
	::TerminateProcess(::GetCurrentProcess(), 0);
}

DWORD WINAPI _StartSocketThread(LPVOID lpParm)
{
	CLolCenterDlg* dlg = (CLolCenterDlg *)lpParm;


	HMODULE hm = ::LoadLibraryW(L"LOLServer.dll");
	pfnStartSocket = (fnStartSocket)::GetProcAddress(hm, "StartSocket");
	pPauseSocket = (PauseSocket)::GetProcAddress(hm, "PauseSocket");
	pRecoverySocket = (RecoverySocket)::GetProcAddress(hm, "RecoverySocket");
	pSetAccountList = (SetAccountList)::GetProcAddress(hm, "SetAccountList");
	pGetAccountList = (GetAccountList)::GetProcAddress(hm, "GetAccountList");
	pfnCloseALLClient = (fnCloseALLClient)::GetProcAddress(hm, "CloseALLClient");

	if (hm == NULL || pfnStartSocket == NULL || pPauseSocket == NULL || pRecoverySocket == NULL || pSetAccountList == NULL || pGetAccountList == NULL)
	{
		AfxMessageBox(L"LOLServer.dll 导出函数不对!");
		((CButton*)dlg->GetDlgItem(IDC_BTN_Start))->EnableWindow(TRUE);
		return 0;
	}

	if (pfnStartSocket() == 0)
	{
		((CEdit*)dlg->GetDlgItem(IDC_EDIT_Log))->SetWindowTextW(L"开启通讯失败!");
		((CButton*)dlg->GetDlgItem(IDC_BTN_Start))->EnableWindow(TRUE);
		return 0;
	}

	// 拷贝内存
	vector<ServerAccountInfo> vlst;
	AccountListLock.Access([&vlst]{
		vlst.assign(AccountList.begin(), AccountList.end());
	});

	// 设置帐号
	pSetAccountList(vlst);

	((CButton*)dlg->GetDlgItem(IDC_BTN_Pause))->ShowWindow(SW_SHOW);
	((CButton*)dlg->GetDlgItem(IDC_BTN_Start))->ShowWindow(SW_HIDE);
	((CEdit*)dlg->GetDlgItem(IDC_EDIT_Log))->SetWindowTextW(L"开启通讯成功!");
	return 0;
}

void CLolCenterDlg::OnBnClickedBtnStart()
{
	((CButton*)GetDlgItem(IDC_BTN_Start))->EnableWindow(FALSE);
	chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)_StartSocketThread, this, NULL, NULL);
}

void CLolCenterDlg::OnBnClickedBtnUpdateaccount()
{
	static CUpdateAccountDlg UpdateAccountDlg;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UpdateAccountDlg.DoModal();
	g_bRefush = TRUE;

	// 拷贝内存
	vector<ServerAccountInfo> vlst;
	AccountListLock.Access([&vlst]{
		vlst.assign(AccountList.begin(), AccountList.end());
	});

	// 设置帐号
	if (pSetAccountList != NULL)
		pSetAccountList(vlst);
}


void CLolCenterDlg::OnBnClickedBtnPause()
{
	pPauseSocket();
	((CButton*)GetDlgItem(IDC_BTN_Pause))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_BTN_Recovery))->ShowWindow(SW_SHOW);
}


void CLolCenterDlg::OnBnClickedBtnRecovery()
{
	pRecoverySocket();
	((CButton*)GetDlgItem(IDC_BTN_Pause))->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_BTN_Recovery))->ShowWindow(SW_HIDE);
}


void CLolCenterDlg::OnBnClickedBtnCloseclinet()
{
	if (pfnCloseALLClient == NULL)
	{
		AfxMessageBox(L"尚未开始挂机");
	}
	else
	{
		pfnCloseALLClient();
		AfxMessageBox(L"Done!");
	}
}


void CLolCenterDlg::OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	//*pResult = 0;

	/*NMLVCUSTOMDRAW* pNMCD = (NMLVCUSTOMDRAW*)(pNMHDR);

	if (CDDS_PREPAINT == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pNMCD->nmcd.dwDrawStage)
	{
		COLORREF  clrNewTextColor, clrNewBkColor;
		int nItem = static_cast<int>(pNMCD->nmcd.dwItemSpec);

		CListCtrl* m_List = (CListCtrl *)GetDlgItem(IDC_LIST1);
		CString str = m_List->GetItemText(nItem, 11);
		if (nItem % 2 == 0)
		{
			clrNewTextColor = RGB(0, 0, 0);
			clrNewBkColor = RGB(240, 247, 233);
		}
		else
		{
			clrNewTextColor = RGB(0, 0, 0);
			clrNewBkColor = RGB(255, 255, 255);
		}

		pNMCD->clrText = clrNewTextColor;
		pNMCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_DODEFAULT;
	}*/
}
