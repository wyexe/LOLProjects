#pragma once


// CGame 对话框

class CGame : public CDialogEx
{
	DECLARE_DYNAMIC(CGame)

public:
	CGame(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGame();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
