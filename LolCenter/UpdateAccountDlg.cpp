// UpdateAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LolCenter.h"
#include "UpdateAccountDlg.h"
#include "afxdialogex.h"
#include "AddAccountDlg.h"

#define _SELF L"UpdateAccountDlg.cpp"

// CUpdateAccountDlg dialog
vector<ServerAccountInfo> UpdateAccountList;
int g_nSelListIndex = -1;

IMPLEMENT_DYNAMIC(CUpdateAccountDlg, CDialogEx)

CUpdateAccountDlg::CUpdateAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUpdateAccountDlg::IDD, pParent)
{

}

CUpdateAccountDlg::~CUpdateAccountDlg()
{
}

void CUpdateAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdateAccountDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_UpdateAccount, &CUpdateAccountDlg::OnNMClickListUpdateaccount)
	ON_BN_CLICKED(IDC_RADIO_Custome, &CUpdateAccountDlg::OnBnClickedRadioCustome)
	ON_BN_CLICKED(IDC_RADIO_Default, &CUpdateAccountDlg::OnBnClickedRadioDefault)
	ON_BN_CLICKED(IDC_BTN_Delete, &CUpdateAccountDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_UpdateAccount, &CUpdateAccountDlg::OnBnClickedBtnUpdateaccount)
	ON_BN_CLICKED(IDC_BTN_AddAccount, &CUpdateAccountDlg::OnBnClickedBtnAddaccount)
END_MESSAGE_MAP()

DWORD WINAPI _ShowListThread(LPVOID lpParm)
{
	int nOrder = 0;
	CUpdateAccountDlg* dlg = (CUpdateAccountDlg*)lpParm;
	CListCtrl* pList = (CListCtrl*)dlg->GetDlgItem(IDC_LIST_UpdateAccount);
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
	pList->InsertColumn(pList->GetHeaderCtrl()->GetItemCount(), L"模式", LVCFMT_LEFT, 80);

	// 拷贝数据
	AccountListLock.Access([]{UpdateAccountList.assign(AccountList.begin(), AccountList.end()); });

	for (auto& __ServerAccountInfo : UpdateAccountList)
		dlg->AddListItem(__ServerAccountInfo);

	return 0;
}

VOID CUpdateAccountDlg::AddListItem(const ServerAccountInfo& __ServerAccountInfo)
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_UpdateAccount);

	int nCount = 0;
	int nRow = pList->InsertItem(pList->GetItemCount(), CAccountConfig::FormatNumber(pList->GetItemCount() + 1));

	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszComputerName);
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszQQ);
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszNiceName);
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszServer);
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.wszMapName);
	pList->SetItemText(nRow, ++nCount, CAccountConfig::FormatNumber(__ServerAccountInfo.dwMaxLevel));
	pList->SetItemText(nRow, ++nCount, CAccountConfig::FormatNumber(__ServerAccountInfo.dwMaxGold));
	pList->SetItemText(nRow, ++nCount, __ServerAccountInfo.PlayMode == AccountPlayMode_Default ? L"默认模式" : L"自定义模式");
}

// CUpdateAccountDlg message handlers
BOOL CUpdateAccountDlg::OnInitDialog()
{
	UpdateAccountList.clear();

	// 设置默认模式
	((CButton*)GetDlgItem(IDC_RADIO_Default))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_Custome))->SetCheck(FALSE);

	// 默认重复提示
	((CButton*)GetDlgItem(IDC_CHECK_Repeatedly))->SetCheck(TRUE);

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_Server);
	

	int nCount = 0;
	pComboBox->InsertString(nCount++, L"艾欧尼亚");
	pComboBox->InsertString(nCount++, L"比尔吉沃特");
	pComboBox->InsertString(nCount++, L"祖安");
	pComboBox->InsertString(nCount++, L"诺克萨斯");
	pComboBox->InsertString(nCount++, L"德玛西亚");
	pComboBox->InsertString(nCount++, L"班德尔城");
	pComboBox->InsertString(nCount++, L"皮尔特沃夫");
	pComboBox->InsertString(nCount++, L"战争学院");
	pComboBox->InsertString(nCount++, L"弗雷尔卓德");
	pComboBox->InsertString(nCount++, L"巨神峰");
	pComboBox->InsertString(nCount++, L"雷瑟守备");
	pComboBox->InsertString(nCount++, L"无畏先锋");
	pComboBox->InsertString(nCount++, L"裁决之地");
	pComboBox->InsertString(nCount++, L"黑色玫瑰");
	pComboBox->InsertString(nCount++, L"暗影岛");
	pComboBox->InsertString(nCount++, L"钢铁烈阳");
	pComboBox->InsertString(nCount++, L"恕瑞玛");
	pComboBox->InsertString(nCount++, L"均衡教派");
	pComboBox->InsertString(nCount++, L"水晶之痕");
	pComboBox->InsertString(nCount++, L"教育网专区");
	pComboBox->InsertString(nCount++, L"影流");
	pComboBox->InsertString(nCount++, L"守望之海");
	pComboBox->InsertString(nCount++, L"扭曲丛林");
	pComboBox->InsertString(nCount++, L"征服之海");
	pComboBox->InsertString(nCount++, L"卡拉曼达");
	pComboBox->InsertString(nCount++, L"皮城警备");
	pComboBox->InsertString(nCount++, L"巨龙之巢");

	CComboBox* pComboBoxMap = (CComboBox*)GetDlgItem(IDC_COMBO_Map);
	pComboBoxMap->InsertString(0, L"召唤师峡谷");
	pComboBoxMap->InsertString(1, L"水晶之痕");
	pComboBoxMap->InsertString(2, L"扭曲丛林");
	pComboBoxMap->InsertString(3, L"嚎哭深渊");
	pComboBoxMap->InsertString(4, L"内置");
	pComboBoxMap->EnableWindow(FALSE);

	chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)_ShowListThread, this, NULL, NULL);
	return TRUE;
}

void CUpdateAccountDlg::OnNMClickListUpdateaccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{
		g_nSelListIndex = pNMListView->iItem;

		CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_LIST_UpdateAccount);
		((CStatic*)GetDlgItem(IDC_STATIC_ComputerName))->SetWindowTextW(pList->GetItemText(pNMListView->iItem, 1));
		((CStatic*)GetDlgItem(IDC_STATIC_QQ))->SetWindowTextW(pList->GetItemText(pNMListView->iItem, 2));
		((CStatic*)GetDlgItem(IDC_STATIC_NiceName))->SetWindowTextW(pList->GetItemText(pNMListView->iItem, 3));

		////////大区//////////////////////////////////////////////////////////////////
		CComboBox* pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_Server);
		CString strServer = pList->GetItemText(pNMListView->iItem, 4);
		
		for (INT i = 0; i < pComboBox->GetCount(); ++i)
		{
			CString str;
			pComboBox->GetLBText(i, str);
			if (str == strServer)
			{
				pComboBox->SetCurSel(i);
				break;
			}
		}

		///////地图///////////////////////////////////////////////////////////////////
		CComboBox* pComboBoxMap = (CComboBox *)GetDlgItem(IDC_COMBO_Map);
		CString strMap = pList->GetItemText(pNMListView->iItem, 5);

		for (INT i = 0; i < pComboBoxMap->GetCount(); ++i)
		{
			CString str;
			pComboBoxMap->GetLBText(i, str);
			if (str == strMap)
			{
				pComboBoxMap->SetCurSel(i);
				break;
			}
		}

		CString strLevel = pList->GetItemText(pNMListView->iItem, 6);
		CString strGold = pList->GetItemText(pNMListView->iItem, 7);
		strLevel = strLevel.Mid(strLevel.Find(L"/") + 1);
		strGold = strGold.Mid(strGold.Find(L"/") + 1);

		((CStatic*)GetDlgItem(IDC_EDIT_Level))->SetWindowTextW(strLevel);
		((CStatic*)GetDlgItem(IDC_EDIT_Gold))->SetWindowTextW(strGold);

		if (CCharacter::wstrcmp_my(L"默认模式", pList->GetItemText(pNMListView->iItem, 8)))
		{
			((CButton*)GetDlgItem(IDC_RADIO_Default))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_RADIO_Custome))->SetCheck(FALSE);
			((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(FALSE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_Default))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_RADIO_Custome))->SetCheck(TRUE);
			((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(TRUE);
		}
	}

	*pResult = 0;
}


void CUpdateAccountDlg::OnBnClickedRadioCustome()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(TRUE);
}


void CUpdateAccountDlg::OnBnClickedRadioDefault()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(FALSE);
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->SetCurSel(4);
}


void CUpdateAccountDlg::OnBnClickedBtnDelete()
{
	try
	{
		CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_LIST_UpdateAccount);

		if (((CButton*)GetDlgItem(IDC_CHECK_Repeatedly))->GetCheck())
		{
			if (AfxMessageBox(L"是否确认再确认要删除?", MB_YESNO) == IDNO)
				return;
		}

		if (g_nSelListIndex == -1)
			return;

		// 清空选项
		ClearListItem();

		// 先把列表的内容给删除了
		pList->DeleteItem(g_nSelListIndex);

		ServerAccountInfo __ServerAccountInfo = UpdateAccountList.at(g_nSelListIndex);

		// 删除总表
		CAccountModel::RemoveAccount(__ServerAccountInfo);

		// 删除临时表
		UpdateAccountList.erase(UpdateAccountList.begin() + g_nSelListIndex);

		g_nSelListIndex = -1;
	}
	catch(...)
	{
		CPrintLog::MsgBoxLog_W(_SELF,__LINE__,L"OnBnClickedBtnDelete出现异常");
	}
}

VOID CUpdateAccountDlg::ClearListItem()
{
	((CStatic*)GetDlgItem(IDC_STATIC_ComputerName))->SetWindowTextW(L"………………");
	((CStatic*)GetDlgItem(IDC_STATIC_QQ))->SetWindowTextW(L"………………");
	((CStatic*)GetDlgItem(IDC_STATIC_NiceName))->SetWindowTextW(L"………………");
	((CComboBox*)GetDlgItem(IDC_COMBO_Server))->SetCurSel(-1);
	((CStatic*)GetDlgItem(IDC_EDIT_Level))->SetWindowTextW(L"");
	((CStatic*)GetDlgItem(IDC_EDIT_Gold))->SetWindowTextW(L"");
	((CButton*)GetDlgItem(IDC_RADIO_Default))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_Custome))->SetCheck(FALSE);
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->SetCurSel(-1);
	((CComboBox*)GetDlgItem(IDC_COMBO_Map))->EnableWindow(FALSE);
}

void CUpdateAccountDlg::OnBnClickedBtnUpdateaccount()
{
	CString strLevel;
	CString strGold;
	CString strServer;

	CListCtrl* pList = (CListCtrl *)GetDlgItem(IDC_LIST_UpdateAccount);

	// 修改大区
	CComboBox* pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_Server);
	if (pComboBox->GetCurSel() == -1 || g_nSelListIndex == -1)
	{
		AfxMessageBox(L"木有这个区!");
		return;
	}


	ServerAccountInfo __ServerAccountInfo = UpdateAccountList.at(g_nSelListIndex);

	// 修改大区
	pComboBox->GetLBText(pComboBox->GetCurSel(), strServer);
	CCharacter::wstrcpy_my(__ServerAccountInfo.wszServer, strServer);

	// 修改等级 和 金币
	((CEdit*)GetDlgItem(IDC_EDIT_Level))->GetWindowTextW(strLevel);
	((CEdit*)GetDlgItem(IDC_EDIT_Gold))->GetWindowTextW(strGold);

	__ServerAccountInfo.dwMaxLevel = _wtoi(strLevel.GetBuffer());
	__ServerAccountInfo.dwMaxGold = _wtoi(strGold.GetBuffer());

	// 判断模式
	if (((CButton*)GetDlgItem(IDC_RADIO_Custome))->GetCheck())
	{
		CComboBox* pComboBoxMap = (CComboBox *)GetDlgItem(IDC_COMBO_Map);
		if (pComboBoxMap->GetCurSel() == -1)
		{
			AfxMessageBox(L"自定义模式至少要选一个地图!");
			return;
		}
		CString strMap;
		pComboBoxMap->GetLBText(pComboBoxMap->GetCurSel(), strMap);
		CCharacter::wstrcpy_my(__ServerAccountInfo.wszMapName, strMap);

		__ServerAccountInfo.PlayMode = AccountPlayMode_Custome;
	}
	if (__ServerAccountInfo.PlayMode == AccountPlayMode_Custome && CCharacter::wstrcmp_my(__ServerAccountInfo.wszMapName, L"内置"))
	{
		AfxMessageBox(L"自定义模式下不可选内置!");
		return;
	}

	// 修改回临时表
	UpdateAccountList.at(g_nSelListIndex) = __ServerAccountInfo;

	// 修改显示列表
	pList->SetItemText(g_nSelListIndex, 6, strLevel.GetBuffer());
	pList->SetItemText(g_nSelListIndex, 7, strGold.GetBuffer());
	pList->SetItemText(g_nSelListIndex, 8, __ServerAccountInfo.PlayMode == AccountPlayMode_Default ? L"默认模式" : L"自定义模式");
	pList->SetItemText(g_nSelListIndex, 4, __ServerAccountInfo.wszServer);
	pList->SetItemText(g_nSelListIndex, 5, __ServerAccountInfo.wszMapName);

	// 修改总表
	CAccountModel::UpdateAccount(__ServerAccountInfo);
	ClearListItem();
	g_nSelListIndex = -1;
}


void CUpdateAccountDlg::OnBnClickedBtnAddaccount()
{
	CAddAccountDlg dlg;
	dlg.DoModal();

	auto fnExist = [](const ServerAccountInfo& __ServerAccountInfo)
	{
		for (auto& tmpServerAccountInfo : UpdateAccountList)
		{
			if (tmpServerAccountInfo.CompQqCrc(__ServerAccountInfo.dwQqCrc))
				return true;
		}
		return false;
	};

	for (auto& tmpServerAccountInfo : dlg.UpdateAccountList)
	{
		// 是否数据重复
		if (!fnExist(tmpServerAccountInfo))
		{
			// 修改临时表
			UpdateAccountList.push_back(tmpServerAccountInfo);

			// 修改界面
			AddListItem(tmpServerAccountInfo);

			// 修改总表
			CAccountModel::AddAccount(tmpServerAccountInfo);
		}
		else
		{
			CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"帐号:%s已经存在 在列表里,就不重复添加了!", tmpServerAccountInfo.wszQQ);
		}
	}
}
