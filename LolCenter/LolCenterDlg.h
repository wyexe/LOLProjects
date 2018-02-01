
// LolCenterDlg.h : header file
//

#pragma once
#include "AccountConfig.h"

// CLolCenterDlg dialog
class CLolCenterDlg : public CDialogEx
{
// Construction
public:
	CLolCenterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LOLCENTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnImportaccount();
	void AddListItem(const AccountConfigInfo& __AccountConfigInfo);
	int ExistList(LPCWSTR pwszQq);
	void DeleteListItem(const vector<ServerAccountInfo>& vlst);
	void UpdateListItem(const vector<ServerAccountInfo>& vlst);
//	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSortFinish();
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSavedata();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnUpdateaccount();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnRecovery();
	afx_msg void OnBnClickedBtnCloseclinet();
	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
};
