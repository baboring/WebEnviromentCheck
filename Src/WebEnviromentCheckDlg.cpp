// WebEnviromentCheckDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "WebEnviromentCheck.h"
#include "WebEnviromentCheckDlg.h"
// -------------------------------------------------
#include <afxsock.h>		// MFC socket extensions
#include <afxinet.h>	//ftp 를 위해 추가해야됨
#include <afxmt.h>
#include ".\webenviromentcheckdlg.h"
// -------------------------------------------------
#include "TextLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

// 구현
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


// CWebEnviromentCheckDlg 대화 상자


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

	// 알파벳으로 시작하는 이름일 경우에는 domain name으로 간주합니다.
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
	else if( (domain[0] > '0')&&(domain[0] < '9') )  // 첫글자가 숫자일 경우에는 decimal notation으로 표현된 ip 주소라고 가정
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


// CWebEnviromentCheckDlg 메시지 처리기

BOOL CWebEnviromentCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	g_hMainFrame = m_hWnd;

	CListCheck::Initial();

	CWebInfo info;
/*
	info.m_Title = "A 웹";
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
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CWebEnviromentCheckDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CWebEnviromentCheckDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*********************************************************************************************************/
BOOL CWebEnviromentCheckDlg::DownloadWebPage(int nNum,LPCTSTR szURL)    //1번
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
				int nLimit = 10;	// 임시로 파일 전체 다운로드를 제안하자..
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CListCheck::OnOdfinditemList(pNMHDR,pResult);
	*pResult = 0;
}

void CWebEnviromentCheckDlg::OnLvnGetdispinfoLstContents(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CListCheck::OnGetdispinfoList(pNMHDR,pResult);
	*pResult = 0;
}

void CWebEnviromentCheckDlg::OnNMClickLstContents(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CListCheck::OnClickList(pNMHDR,pResult);
	*pResult = 0;
}
