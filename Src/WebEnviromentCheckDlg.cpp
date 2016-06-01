// WebEnviromentCheckDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "WebEnviromentCheck.h"
#include "WebEnviromentCheckDlg.h"
// -------------------------------------------------
#include <afxsock.h>		// MFC socket extensions
#include <afxinet.h>	//ftp �� ���� �߰��ؾߵ�
#include <afxmt.h>
#include ".\webenviromentcheckdlg.h"
// -------------------------------------------------
#include "TextLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CWebEnviromentCheckDlg ��ȭ ����


#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib");
#elif UNIX
#include <netdb.h>   
#include <sys/socket.h>   
#include <arpa/inet.h>   
#include <netinet/in.h>
#endif

int getHostIP(char *domain,char* szResult)
{
	int ret = 0;
	struct hostent *pHost;
	struct in_addr *pAddr;

#ifdef WIN32
	WSADATA wsaData;
	ret = WSAStartup(WINSOCK_VERSION, &wsaData);
	if(ret != 0)
	{
		printf("WSAStartup() failed, error code = %d\n", ret);
		return 300006;
	}
#endif

	// ���ĺ����� �����ϴ� �̸��� ��쿡�� domain name���� �����մϴ�.
	if( ((domain[0] > 'A')&&(domain[0] < 'Z')) || ((domain[0] > 'a')&&(domain[0] < 'z')) ) 
	{ 
		pHost = gethostbyname(domain);
		if(pHost == NULL)
		{
#ifdef WIN32
			ret = WSAGetLastError();
#endif
			printf("Error in gethostbyname(), error code = %d\n", ret);
			goto HANDLE_ERROR;
		}
		else
		{
			//printf("Official name of the host is: '%s'\n", pHost->h_name);   
			while(*pHost->h_addr_list != NULL)
			{
				pAddr = reinterpret_cast<in_addr *>(*(pHost->h_addr_list));
				sprintf(szResult,"%s", inet_ntoa(*pAddr));
				pHost->h_addr_list++;
			}
		}
#ifdef WIN32 
		ret = WSACleanup();
		if(ret != 0)
		{
			ret = WSAGetLastError();
			printf("WSACleanup() failed, error code = %d\n", ret);
			goto HANDLE_ERROR;
		}
#endif
		return 0;
	}
	else if( (domain[0] > '0')&&(domain[0] < '9') )  // ù���ڰ� ������ ��쿡�� decimal notation���� ǥ���� ip �ּҶ�� ����
	{ 
		sprintf(szResult,"%s", domain);
		return 0;
	} else goto HANDLE_ERROR;
HANDLE_ERROR:
#ifdef WIN32
	WSACleanup();
#endif
	return ret;
}


CWebEnviromentCheckDlg::CWebEnviromentCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebEnviromentCheckDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebEnviromentCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_CONTENTS, m_ctrList);
	DDX_Control(pDX, IDC_LST_LOG, m_lstLog);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrProgress);
}



BEGIN_MESSAGE_MAP(CWebEnviromentCheckDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BN_START, OnBnClickedBnStart)
	ON_NOTIFY(LVN_ODFINDITEM, IDC_LST_CONTENTS, OnLvnOdfinditemLstContents)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LST_CONTENTS, OnLvnGetdispinfoLstContents)
	ON_NOTIFY(NM_CLICK, IDC_LST_CONTENTS, OnNMClickLstContents)
END_MESSAGE_MAP()


// CWebEnviromentCheckDlg �޽��� ó����

BOOL CWebEnviromentCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	g_hMainFrame = m_hWnd;

	CListCheck::Initial();

	CWebInfo info;
/*
	info.m_Title = "A ��";
	info.m_URL = "72.34.243.115";
	CListCheck::Add(info);
*/	
	if(TEXTLOADER.Open("config.ini"))
	{
		for(int i=0;i<TEXTLOADER.GetCount();i++)
		{
			if(TEXTLOADER.GetText(i,info.m_Title,info.m_URL))
			{
				CListCheck::Add(info);
			}
		}
	}


	CListCheck::Redraw();
	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

void CWebEnviromentCheckDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CWebEnviromentCheckDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CWebEnviromentCheckDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*********************************************************************************************************/
BOOL CWebEnviromentCheckDlg::DownloadWebPage(int nNum,LPCTSTR szURL)    //1��
/*********************************************************************************************************/
{	
	BOOL bResult = TRUE;
	DWORD dwServiceType;
	INTERNET_PORT nPort;
	CString strServer, strObject, strData;
	if(!AfxParseURL(szURL, dwServiceType, strServer, strObject, nPort))
	{
		OutputDebugString("ERROR# URL Parsing\n");
		return FALSE;
	}
	DWORD dwRet;
	CString strHeader = "Content-Type: application/x-www-form-urlencoded";
	CInternetSession session("My Session");
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	try
	{
		pServer = session.GetHttpConnection(strServer, nPort);
		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, 0, 1, 0, 0, INTERNET_FLAG_RELOAD);
		pFile->AddRequestHeaders(strHeader);
		pFile->SendRequest();
		pFile->QueryInfoStatusCode(dwRet);

		if(dwRet == HTTP_STATUS_OK)
		{
			char strTemp[512];
			CStdioFile stdFile;
			CString szFileName;
			szFileName.Format("down\\%s.txt",szURL);
			szFileName.Replace('/','_');
			szFileName.Replace(':','_');
			if(stdFile.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
			{
				int nLimit = 10;	// �ӽ÷� ���� ��ü �ٿ�ε带 ��������..
				while(pFile->ReadString(strTemp,512) && nLimit)
				{
					nLimit--;
					stdFile.WriteString(strTemp);
				}
				stdFile.Close();
			}
		}
		else
		{
			OutputDebugString("ERROR# HTTP_STATUS\n");
			bResult = FALSE;
		}

		delete pFile;
		delete pServer;
	}
	catch(CInternetException* pEx)
	{
		TCHAR szCause[255];
		CString strError;

		pEx->GetErrorMessage(szCause, 255);
		strError.Format("ERROR# Connect Failed[%d] :", pEx->m_dwError);
		strError += szCause;
		strError += _T("\n");
		OutputDebugString(strError);

		session.Close();
		return FALSE;
	}

	session.Close();

	return bResult;
}
// ===========================================================
//
// ===========================================================
DWORD WINAPI ThreadProcess(LPVOID arg)
{
	//	DWORD tid1;
	CWebEnviromentCheckDlg* lpThis = (CWebEnviromentCheckDlg*)arg;
	lpThis->ProcessDomain();
	return 0;
}

// ===============================================================================
void CWebEnviromentCheckDlg::OnBnClickedBnStart()
{
	CWnd* lpWnd = GetDlgItem(IDC_BN_START);
	if(lpWnd)
	{
		if(lpWnd->IsWindowEnabled())
		{
			DWORD tid;
			CreateThread(0,0,&ThreadProcess,this,0,&tid);	
		}
	}
}
// ===============================================================================
void CWebEnviromentCheckDlg::ProcessDomain()
{
	CWnd* lpWnd = GetDlgItem(IDC_BN_START);
	if(lpWnd)
	{
		lpWnd->EnableWindow(FALSE);
		CString info;
		CString temp;
		char szTemp[128] = "";
		char szDomain[128] = "";

		m_ctrProgress.SetRange(0,CListCheck::GetCount());
		m_ctrProgress.SetPos(0);

		for(int i=0;i<CListCheck::GetCount();i++)
		{
			temp = CListCheck::m_database[i].m_URL;
			temp.Trim();

			if(temp.GetLength() > 4)
			{
				info = "http://";
				info += temp;
				
				CListCheck::SelectRow(i);

				if(DownloadWebPage(i,info))
				{
					CListCheck::m_database[i].m_Status = "success";
					CListCheck::m_database[i].m_image = 0;
				}
				else
				{
					CListCheck::m_database[i].m_Status = "fails";
					CListCheck::m_database[i].m_image = 1;
				}

				ParseStr(CListCheck::m_database[i].m_URL.GetBuffer(),szTemp,':',0);
				ParseStr(szTemp,szDomain,'/',0);

				memset(szTemp,0,sizeof(szTemp));
				getHostIP(szDomain,szTemp);

				CListCheck::m_database[i].m_IP = szTemp;
				CListCheck::m_database[i].m_Comment = szDomain;
			}
			m_ctrProgress.SetPos(i+1);
			CListCheck::Redraw();
		}

		lpWnd->EnableWindow(TRUE);
	}
}
// ===============================================================================
void CWebEnviromentCheckDlg::OnLvnOdfinditemLstContents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVFINDITEM pFindInfo = reinterpret_cast<LPNMLVFINDITEM>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CListCheck::OnOdfinditemList(pNMHDR,pResult);
	*pResult = 0;
}

void CWebEnviromentCheckDlg::OnLvnGetdispinfoLstContents(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CListCheck::OnGetdispinfoList(pNMHDR,pResult);
	*pResult = 0;
}

void CWebEnviromentCheckDlg::OnNMClickLstContents(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CListCheck::OnClickList(pNMHDR,pResult);
	*pResult = 0;
}
