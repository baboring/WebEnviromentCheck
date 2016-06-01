// WebEnviromentCheckDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "listCheck.h"


// CWebEnviromentCheckDlg 대화 상자
class CWebEnviromentCheckDlg : public CDialog,public CListCheck
{
// 생성
public:
	CWebEnviromentCheckDlg(CWnd* pParent = NULL);	// 표준 생성자

	BOOL	DownloadWebPage(int nNum,LPCTSTR szURL);

// 대화 상자 데이터
	enum { IDD = IDD_WEBENVIROMENTCHECK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
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
