#pragma once

#include "AccountConfig.h"
// CAddAccountDlg dialog

class CAddAccountDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddAccountDlg)

public:
	CAddAccountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddAccountDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAdd();
	vector<ServerAccountInfo> UpdateAccountList;
};
