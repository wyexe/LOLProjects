#pragma once

#include "AccountConfig.h"
// CUpdateAccountDlg dialog

extern vector<ServerAccountInfo> UpdateAccountList;

class CUpdateAccountDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdateAccountDlg)

public:
	CUpdateAccountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUpdateAccountDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickListUpdateaccount(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioCustome();
	afx_msg void OnBnClickedRadioDefault();
	afx_msg void OnBnClickedBtnDelete();
	VOID ClearListItem();
	VOID AddListItem(const ServerAccountInfo& __ServerAccountInfo);
	afx_msg void OnBnClickedBtnUpdateaccount();
	afx_msg void OnBnClickedBtnAddaccount();
};
