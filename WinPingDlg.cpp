
// WinPingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinPing.h"
#include "WinPingDlg.h"

#include "ProtoInfo.h"
#include "NetworkAdapter.h"
#include "public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const UINT IdTestStatusTimer = 501;
const UINT uTestStatusTimeout = 250;

//CNEWBMP g_status_bmp;
CNetworkAdaper g_networkAdaper;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CWinPingDlg dialog

CString CWinPingDlg::m_strResult = _T("");
bool CWinPingDlg::m_bPingThread_Idle = TRUE;

#define UM_PINGMSG	WM_USER+10011
#define UM_PINGFIN  WM_USER+10012
//#define UM_AUTOTEST  WM_USER+10013

#define DEFAULT_DATA_SIZE      32
#define DEFAULT_SEND_COUNT     4
#define DEFAULT_RECV_TIMEOUT   6000

#define MAX_BUFFER_LEN		   1024


CWinPingDlg::CWinPingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinPingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinPingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ADDRESS, m_combo);
	DDX_Control(pDX, IDC_EDIT_RESULT, m_edit);
}

BEGIN_MESSAGE_MAP(CWinPingDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CWinPingDlg::OnBnClickedOk)
	ON_MESSAGE(UM_PINGMSG, &CWinPingDlg::OnPingMsg)
	ON_MESSAGE(UM_PINGFIN, &CWinPingDlg::OnPingFin)
	//ON_MESSAGE(UM_AUTOTEST, &CWinPingDlg::OnPingFin)
	//ON_WM_DEVICECHANGE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWinPingDlg message handlers

BOOL CWinPingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//HWND hWnd = AfxGetMainWnd()->m_hWnd;
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	//g_status_bmp.SetWindHand((HWND)GetDlgItem(IDC_STATIC_STATUS_PIC));

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	//SetTimer(IdTestStatusTimer, uTestStatusTimeout, NULL);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_combo.AddString(_T("192.168.3.8"));
	m_combo.SetCurSel(0);
	m_bPingThread_Idle = TRUE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinPingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinPingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinPingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWinPingDlg::SetBitmap(UINT uBmpResource, UINT uCtrlResource)
{
    HBITMAP hBitmap;
    CStatic *pStatic = (CStatic *)GetDlgItem(uCtrlResource);
    hBitmap = (HBITMAP)LoadImage(
        AfxGetInstanceHandle(),
        MAKEINTRESOURCE(uBmpResource),
        IMAGE_BITMAP,
        0,
        0,
        LR_LOADMAP3DCOLORS);
    pStatic->ModifyStyle(0xF, SS_BITMAP);
    pStatic->SetBitmap(hBitmap);
	// pStatic->ReleaseDC(pStatic->GetDC());
	//pStatic->GetDC
}

void CWinPingDlg::ClearBmp(UINT uCtrlResource)
{
	HDC   pDC;
	CDC*   pcDC;
	RECT rect;
	HWND hMap;
	CStatic *pStatic = (CStatic *)GetDlgItem(uCtrlResource);
	//hMap = GetDlgItem(uCtrlResource);
	pcDC = pStatic->GetDC();
	pDC=pcDC->GetSafeHdc();//这个用法不错,学习一下.
	rect.left =0;               //我一般不会这么用的.
	rect.top=0;
	rect.right=90;
	rect.bottom=60;
	FillRect(pDC,&rect,(HBRUSH)COLOR_BTNSHADOW);
}



void CWinPingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	//m_edit.SetWindowText(_T(""));  // this statement use to clean editbox.
	char mac[100] = {0};
	char strIp[40] = {0};
	TCHAR strMac[100] = {0};
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	AddStringToComboBox();
	ClearBmp(IDC_STATIC_STATUS_PIC);
	SetDlgItemText(IDC_STATIC_MAC_ADDR, _T(""));
	SetTimer(IdTestStatusTimer, uTestStatusTimeout, NULL);
	
	
	CWinThread* pThread = AfxBeginThread(PingThreadProc, (LPVOID)GetSafeHwnd());
	GetDlgItemTextA(GetSafeHwnd(), IDC_COMBO_ADDRESS, strIp, MAX_BUFFER_LEN);
	if((g_networkAdaper.getLocalMac(strIp, mac))>0)
	{
		c2w(strMac, strlen(mac), mac);
		SetDlgItemText(IDC_STATIC_MAC_ADDR, strMac);
		
	}

	//SetBitmap(IDB_BITMAP_NG, IDC_STATIC_STATUS_PIC);
	//CloseHandle(pThread->m_hThread);
}

void CWinPingDlg::AddStringToComboBox()
{
	CString strText(_T(""));
	m_combo.GetWindowText(strText);
	if(CB_ERR == m_combo.FindString(-1, strText))
	{
		m_combo.AddString(strText);
	}
}

UINT CWinPingDlg::PingThreadProc(LPVOID lParam)
{
	HWND hWnd = (HWND)lParam;
	ASSERT(hWnd);
	
	WSAOVERLAPPED ol;
	SOCKET sRaw = INVALID_SOCKET;
	char* icmpbuf = NULL;
	struct addrinfo* dest = NULL;
	struct addrinfo* local = NULL;

	SOCKADDR_STORAGE from;
	DWORD bytes = 0;
	DWORD flags = 0;
	int packlen = 0;
	int fromlen = sizeof(from);
	int time = 0;
	int rc = 0;
	int status = 0;
	m_bPingThread_Idle = FALSE;
	
	CString strTmp (_T(""));
	CString strIpDest(_T(""));
	
	ol.hEvent = WSA_INVALID_EVENT;

	int uiError = 0;

	char szDestination[MAX_BUFFER_LEN] = {0};
	char recvBuf[MAX_BUFFER_LEN] = {0};

	//add to record device ping status.
	g_stPingStatus = 0;
	
	GetDlgItemTextA(hWnd, IDC_COMBO_ADDRESS, szDestination, MAX_BUFFER_LEN);

	dest = CProtoInfo::ResolveAddress(szDestination, "0", AF_INET, 0, 0);
	if(NULL == dest)
	{
		//AfxMessageBox(_T("ResolveAddress 1 failed."));
		// return 0;
		m_strResult.Format(_T("ResolveAddress 1 failed."));
		::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
		m_strResult.ReleaseBuffer();
		goto THREAD_END;
	}
	
	
	local = CProtoInfo::ResolveAddress(NULL, "0", AF_INET, 0, 0);
	if(NULL == local)
	{
		//AfxMessageBox(_T("ResolveAddress 2 failed."));
		m_strResult.Format(_T("ResolveAddress 2 failed."));
		::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
		m_strResult.ReleaseBuffer();
		// return 0;
		goto THREAD_END;
	}
	//WSACleanup();
	//uiError = GetLastError();
	sRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(INVALID_SOCKET == sRaw)
	{
		//uiError = GetLastError();
		//AfxMessageBox(_T("socket failed."));
		m_strResult.Format(_T("socket failed."));
		::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
		m_strResult.ReleaseBuffer();
		// return 0;
		goto THREAD_END;
	}

	CProtoInfo::SetTTL(sRaw);

	packlen += sizeof(ICMP_HDR);
	int nDataSize = DEFAULT_DATA_SIZE;
	packlen += nDataSize;

	icmpbuf = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, packlen);
	if(NULL == icmpbuf)
	{
		//AfxMessageBox(_T("HeapAlloc failed."));
		m_strResult.Format(_T("HeapAlloc failed."));
		::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
		m_strResult.ReleaseBuffer();
		closesocket(sRaw);
		// return 0;
		goto THREAD_END;
	}

	CProtoInfo::InitIcmpHeader(icmpbuf, nDataSize);

	if(SOCKET_ERROR == bind(sRaw, local->ai_addr, (int)local->ai_addrlen))
	{
		//AfxMessageBox(_T("bind failed."));
		m_strResult.Format(_T("bind failed."));
		::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
		m_strResult.ReleaseBuffer();
		closesocket(sRaw);
		// return 0;
		goto THREAD_END;
	}

	memset(&ol, 0, sizeof(ol));
	ol.hEvent = WSACreateEvent();
	if(WSA_INVALID_EVENT == ol.hEvent)
	{
		//AfxMessageBox(_T("WSACreateEvent failed."));
		m_strResult.Format(_T("WSACreateEvent failed."));
		::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
		m_strResult.ReleaseBuffer();
		closesocket(sRaw);
		// return 0;
		goto THREAD_END;
	}

	WSABUF wbuf;
	wbuf.buf = recvBuf;
	wbuf.len = 1024;

	if(SOCKET_ERROR == WSARecvFrom(sRaw, &wbuf, 1, &bytes, &flags, (LPSOCKADDR)&from, &fromlen, &ol, NULL))
	{
		if(WSA_IO_PENDING != WSAGetLastError())
		{
			//AfxMessageBox(_T("WSARecvFrom failed."));
			m_strResult.Format(_T("WSARecvFrom failed."));
			::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
			m_strResult.ReleaseBuffer();
			closesocket(sRaw);
			// return 0;
			goto THREAD_END;
		}
	}

	BOOL bAddr = FALSE;
	if(0 == atoi(szDestination))
	{
		bAddr = TRUE;
	}

	
	strTmp = CProtoInfo::PrintAddress(dest->ai_addr, (int)dest->ai_addrlen);
	strIpDest = strTmp;
	if(!bAddr)
	{
		m_strResult.Format(_T("Ping %s with %d bytes of data:\r\n"), strTmp, nDataSize);
	}
	else
	{
		m_strResult.Format(_T("Ping %s [%s] with %d bytes of data:\r\n"), CString(szDestination), strTmp, nDataSize);
	}
	
	::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
	m_strResult.ReleaseBuffer();

	int nRecive = DEFAULT_SEND_COUNT;
	int nSend = DEFAULT_SEND_COUNT;
	int nMin = DEFAULT_RECV_TIMEOUT;
	int nMax = 0;
	int nTime[DEFAULT_SEND_COUNT] = {0};

	for(int i=0; i<DEFAULT_SEND_COUNT; i++)
	{
		CProtoInfo::SetIcmpSequence(icmpbuf);
		CProtoInfo::ComputeIcmpChecksum(icmpbuf, packlen);
		time = GetTickCount();
		if(SOCKET_ERROR == sendto(sRaw, icmpbuf, packlen, 0, dest->ai_addr, (int)dest->ai_addrlen))
		{
			--nSend;
			continue;
		}
		rc = WaitForSingleObject((HANDLE)ol.hEvent, DEFAULT_RECV_TIMEOUT);
		if(WAIT_FAILED == rc)
		{
			continue;
		}
		else if(WAIT_TIMEOUT == rc)
		{
			m_strResult.Format(_T("Request timeout."));
			::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
			m_strResult.ReleaseBuffer();
			--nRecive;
		}
		else
		{
			WSAGetOverlappedResult(sRaw, &ol, &bytes, FALSE, &flags);
			time = GetTickCount() - time;
			nTime[i] = time;
			nMin = min(nMin, time);
			nMax = max(nMax, time);
			WSAResetEvent(ol.hEvent);
			strTmp = CProtoInfo::PrintAddress((SOCKADDR*)&from, fromlen);
			if(strIpDest != strTmp)
			{
				m_strResult.Format(_T("Reply from %s: destination host unreachable"), strTmp);
			}
			else
			{
				g_stPingStatus = 1;
				if(0 == time)
				{
					m_strResult.Format(_T("Reply from %s: bytes=%d time<1ms TTL=%d"), strTmp, nDataSize, DEFAULT_TTL);
				}
				else
				{
					m_strResult.Format(_T("Reply from %s: bytes=%d time=%dms TTL=%d"), strTmp, nDataSize, time, DEFAULT_TTL);
				}
			}
			::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
			m_strResult.ReleaseBuffer();

			// add to record ping device status. 2015年2月12日 17:15:52.
			// device detect success. 
			//SetBitmap(IDB_BITMAP_NG, IDC_STATIC_STATUS_PIC);

			if(i < DEFAULT_SEND_COUNT - 1)
			{
				fromlen = sizeof(from);
				if(SOCKET_ERROR == WSARecvFrom(sRaw, &wbuf, 1, &bytes, &flags, (SOCKADDR*)&from, &fromlen, &ol, NULL))
				{
					if(WSA_IO_PENDING != WSAGetLastError())
					{
						--nRecive;
						continue;
					}
				}
			}
		}
		Sleep(500);
	}

	strTmp = CProtoInfo::PrintAddress((SOCKADDR*)dest->ai_addr, (int)dest->ai_addrlen);
	int nLost = nSend - nRecive;
	int nAver = 0;
	int nSum = 0;
	for(int i=0; i<DEFAULT_SEND_COUNT; i++)
	{
		nSum += nTime[i];
	}
	nAver = nSum/DEFAULT_SEND_COUNT;

	m_strResult.Format(_T("\r\nPing statistics for %s:\r\n\tPackets: Send = %d, Received = %d, Lost = %d <%d%% lost>,\r\n")
		_T("Approximate round trip times in milli-seconds:\r\n\tMinimum = %dms, Maximum = %dms, Average = %dms"), strTmp, nSend, nRecive, nLost, nLost*100/nRecive, nMin, nMax, nAver);
	
	::SendMessage(hWnd, UM_PINGMSG, (WPARAM)0, (LPARAM)m_strResult.GetBuffer(m_strResult.GetLength()));
	m_strResult.ReleaseBuffer();

	if(dest)
	{
		freeaddrinfo(dest);
		dest = NULL;
	}
	if(local)
	{
		freeaddrinfo(local);
		local = NULL;
	}

	if(WSA_INVALID_EVENT != ol.hEvent)
	{
		WSACloseEvent(ol.hEvent);
		ol.hEvent = WSA_INVALID_EVENT;
	}

	if(icmpbuf)
	{
		HeapFree(GetProcessHeap(), 0, icmpbuf);
		icmpbuf = NULL;
	}
	if(INVALID_SOCKET != sRaw)
	{
		closesocket(sRaw);
		sRaw = INVALID_SOCKET;
	}

	THREAD_END:
	m_bPingThread_Idle = TRUE;
	::PostMessage(hWnd, UM_PINGFIN, 0, 0);
	
	return 0;
}

LRESULT CWinPingDlg::OnPingMsg(WPARAM wParam, LPARAM lParam)
{
	LPTSTR lpszText = (LPTSTR)lParam;
	int iLen = 0;

	if(NULL != lpszText)
	{
		CString strText = lpszText;
		GetDlgItemText(IDC_EDIT_RESULT, strText);
		strText += m_strResult + _T("\r\n");
		SetDlgItemText(IDC_EDIT_RESULT, _T(""));
		((CEdit*)GetDlgItem(IDC_EDIT_RESULT))->ReplaceSel(strText);
	}
	
	return 0;
}

void CWinPingDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == IdTestStatusTimer)
	{
		if((g_networkAdaper.m_bGetMacStatus_Idle == TRUE) && (m_bPingThread_Idle == TRUE))
		{
			if(g_stPingStatus == 1)
			{
				SetBitmap(IDB_BITMAP_PASS, IDC_STATIC_STATUS_PIC);
				WinExec("cmd.exe /c \"devcon disable *PID_0179*\"",SW_SHOW);
			}
			else if(g_stPingStatus == 0)
			{
				SetBitmap(IDB_BITMAP_NG, IDC_STATIC_STATUS_PIC);
			}
			KillTimer(IdTestStatusTimer);
		}
	}
}

LRESULT CWinPingDlg::OnPingFin(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	return 0;
}
