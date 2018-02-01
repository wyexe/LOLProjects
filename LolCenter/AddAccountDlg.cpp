// AddAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LolCenter.h"
#include "AddAccountDlg.h"
#include "afxdialogex.h"

#define _SELF L"AddAccountDlg.cpp"
// CAddAccountDlg dialog

IMPLEMENT_DYNAMIC(CAddAccountDlg, CDialogEx)

CAddAccountDlg::CAddAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddAccountDlg::IDD, pParent)
{

}

CAddAccountDlg::~CAddAccountDlg()
{
}

void CAddAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddAccountDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD, &CAddAccountDlg::OnBnClickedBtnAdd)
END_MESSAGE_MAP()


// CAddAccountDlg message handlers
BOOL CAddAccountDlg::OnInitDialog()
{
	return TRUE;
}

DWORD WINAPI _AddAccountThread(LPVOID lpParm)
{
	CAddAccountDlg* dlg = (CAddAccountDlg *)lpParm;
	CEdit* pEdit = (CEdit *)dlg->GetDlgItem(IDC_EDIT_AccountText);

	CString strAccountText;
	CString strTemp;
	
	vector<AccountConfigInfo> vlst;

	pEdit->GetWindowTextW(strAccountText);

	try
	{
		// 将字符串转换成结构
		while (true)
		{
			static AccountConfigInfo __AccountConfigInfo;

			int nIndex = strAccountText.Find(L"\r\n");
			if (nIndex == -1)
			{
				strTemp = strAccountText;
				ZeroMemory(&__AccountConfigInfo, sizeof(__AccountConfigInfo));
				if (!CAccountConfig::GetAccountInfo_By_Text(strTemp.GetBuffer(), __AccountConfigInfo))
					return FALSE;

				vlst.push_back(__AccountConfigInfo);
				break;
			}

			strTemp = strAccountText.Mid(0, nIndex);
			strAccountText = strAccountText.Mid(nIndex + 2);

			if (!CAccountConfig::GetAccountInfo_By_Text(strTemp.GetBuffer(), __AccountConfigInfo))
				return FALSE;

			vlst.push_back(__AccountConfigInfo);
		}
	}
	catch(...)
	{
		CPrintLog::MsgBoxLog_W(_SELF,__LINE__,L"将字符串转换成结构 出现异常");
		return 0;
	}
	
	try
	{
		vector<ServerAccountInfo>* pAccountList = &dlg->UpdateAccountList;
		auto fnExist = [&pAccountList](DWORD dwQqCrc)
		{
			for (auto& __ServerAccountInfo : *pAccountList)
			{
				if (__ServerAccountInfo.CompQqCrc(dwQqCrc))
					return true;
			}
			return false;
		};

		for (auto& tmpAccountConfigInfo : vlst)
		{
			if (!fnExist(CLPublic::GetCRC32_DWORD((LPSTR)tmpAccountConfigInfo.wszQQ, wcslen(tmpAccountConfigInfo.wszQQ) * 2 + 1)))
			{
				// 转换临时表
				static ServerAccountInfo __ServerAccountInfo;
				ZeroMemory(&__ServerAccountInfo, sizeof(__ServerAccountInfo));

				CCharacter::wstrcpy_my(__ServerAccountInfo.wszQQ, tmpAccountConfigInfo.wszQQ);
				CCharacter::wstrcpy_my(__ServerAccountInfo.wszPass, tmpAccountConfigInfo.wszPass);
				CCharacter::wstrcpy_my(__ServerAccountInfo.wszServer, tmpAccountConfigInfo.wszServer);
				__ServerAccountInfo.dwMaxLevel = tmpAccountConfigInfo.dwConditionLevel;
				__ServerAccountInfo.dwMaxGold = tmpAccountConfigInfo.dwConditionGold;
				__ServerAccountInfo.dwQqCrc = CLPublic::GetCRC32_DWORD((LPSTR)__ServerAccountInfo.wszQQ, wcslen(__ServerAccountInfo.wszQQ) * 2 + 1);
				__ServerAccountInfo.PlayMode = AccountPlayMode_Default;
				CCharacter::wstrcpy_my(__ServerAccountInfo.wszStatus, L"尚未开始!");
				pAccountList->push_back(__ServerAccountInfo);
			}
			else
			{
				CPrintLog::MsgBoxLog_W(_SELF, __LINE__, L"帐号:%s已经存在了", tmpAccountConfigInfo.wszQQ);
			}
		}

		dlg->PostMessageW(WM_CLOSE);
	}
	catch(...)
	{
		CPrintLog::MsgBoxLog_W(_SELF,__LINE__,L"转换临时表 出现异常");
		return 0;
	}

	return 0;
}

void CAddAccountDlg::OnBnClickedBtnAdd()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_BTN_ADD);
	pButton->EnableWindow(FALSE);
	pButton->SetWindowTextW(L"添加中…………");
	chBEGINTHREADEX(NULL, NULL, (LPTHREAD_START_ROUTINE)_AddAccountThread, this, NULL, NULL);
}
