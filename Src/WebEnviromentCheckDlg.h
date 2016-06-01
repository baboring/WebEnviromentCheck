// WebEnviromentCheckDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "listCheck.h"


// CWebEnviromentCheckDlg ��ȭ ����
class CWebEnviromentCheckDlg : public CDialog,public CListCheck
{
// ����
public:
	CWebEnviromentCheckDlg(CWnd* pParent = NULL);	// ǥ�� ������

	BOOL	DownloadWebPage(int nNum,LPCTSTR szURL);

// ��ȭ ���� ������
	enum { IDD = IDD_WEBENVIROMENTCHECK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����


// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void	ProcessDomain();
	afx_msg void OnBnClickedBnStart();
	CListBox m_lstLog;
	afx_msg void OnLvnOdfinditemLstContents(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoLstContents(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLstContents(NMHDR *pNMHDR, LRESULT *pResult);
	CProgressCtrl m_ctrProgress;
};
