// MainDlg.cpp : 구현 파일
//
#include "stdafx.h"
#include "Main.h"
#include "MainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainDlg 대화 상자

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(UM_UDP_RECEIVE, &CMainDlg::OnUdpReceive)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMainDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMainDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_RESET_HOST, &CMainDlg::OnBnClickedBtnResetHost)
	ON_LBN_DBLCLK(IDC_LST_MSG, &CMainDlg::OnLbnDblclkLstMsg)
	ON_BN_CLICKED(IDC_BTN_SEND, &CMainDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_MSG_CLEAR, &CMainDlg::OnBnClickedBtnMsgClear)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CMainDlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()

// CMainDlg 메시지 처리기

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(_T("UDP"));

	SetDlgItemInt(IDC_EDT_LOCAL_PORT, 15001);
	SetDlgItemInt(IDC_EDT_HOST_PORT, 15000);
	SetDlgItemText(IDC_IPA_HOST_IP, _T("127.0.0.1"));

	GetDlgItem(IDC_BTN_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDT_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

	g_objVIsionManager.Initialize();
	Sleep(100);


	static int step = 0;

	switch(step)
	{
	case 0:
		g_objVIsionManager.Set_StatusUpdate(INSPECTOR_PC1, 0);
		Sleep(10);
		g_objVIsionManager.Set_ConnectEnd(INSPECTOR_PC1);
		Sleep(10);
		g_objVIsionManager.Set_ConnectRequest(INSPECTOR_PC1);
		Sleep(10);
		step = 100;
		break;
	case 100:
		step = 200;
		break;

	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN) {
			if (pMsg->hwnd == GetDlgItem(IDC_EDT_SEND)->m_hWnd) OnBnClickedBtnSend();
			return TRUE;
		} else if (pMsg->wParam == VK_ESCAPE) return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////
// UDP Socket Message

LRESULT CMainDlg::OnUdpReceive(WPARAM wLocalPort, LPARAM lParam)
{
	UINT nLocalPort = (UINT)wLocalPort;
	if (nLocalPort != m_nLPort) return 0;

	UINT nRecvPort = m_Udp.Get_RecvPort();
	CString strRecvIp = m_Udp.Get_RecvIp();
	if (m_nHPort != nRecvPort || m_strHIp != strRecvIp) {
		m_nHPort = nRecvPort;
		m_strHIp = strRecvIp;
		m_Udp.Reset_Host(m_nHPort, m_strHIp);
	}

	BYTE byRecv[1025] = { 0 };	// 마지막 0x00
	int nLen = m_Udp.Read_Socket(byRecv);

	char *pRecv = (char*)byRecv;
	CString strRecv = CString(CStringA(pRecv));

	CString strTemp;
	strTemp.Format(_T("[<-] %s"), strRecv);
	((CListBox *)GetDlgItem(IDC_LST_MSG))->AddString(strTemp);

	int nCount = ((CListBox *)GetDlgItem(IDC_LST_MSG))->GetCount();
	((CListBox *)GetDlgItem(IDC_LST_MSG))->SetCurSel(nCount - 1);

	return 0;
}

/////////////////////////////////////////////////////////////////////

void CMainDlg::OnBnClickedBtnOpen()
{
	m_nLPort = GetDlgItemInt(IDC_EDT_LOCAL_PORT);
	m_nHPort = GetDlgItemInt(IDC_EDT_HOST_PORT);
	GetDlgItemText(IDC_IPA_HOST_IP, m_strHIp);

	if (!m_Udp.Open_Socket(m_nLPort, m_nHPort, m_strHIp, this)) return;

	GetDlgItem(IDC_EDT_LOCAL_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_EXIT)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_CLOSE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDT_SEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
}

void CMainDlg::OnBnClickedBtnClose()
{
	m_Udp.Close_Socket();

	GetDlgItem(IDC_BTN_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDT_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDT_LOCAL_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_OPEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_EXIT)->EnableWindow(TRUE);
}

void CMainDlg::OnBnClickedBtnResetHost()
{
	m_nHPort = GetDlgItemInt(IDC_EDT_HOST_PORT);
	GetDlgItemText(IDC_IPA_HOST_IP, m_strHIp);

	m_Udp.Reset_Host(m_nHPort, m_strHIp);
}

void CMainDlg::OnLbnDblclkLstMsg()
{
	int nIndex = ((CListBox *)GetDlgItem(IDC_LST_MSG))->GetCurSel();

	CString strMsg;
	((CListBox *)GetDlgItem(IDC_LST_MSG))->GetText(nIndex, strMsg);

	AfxMessageBox(strMsg);
}

void CMainDlg::OnBnClickedBtnSend()
{
	CString strSend;
	GetDlgItemText(IDC_EDT_SEND, strSend);
	if (strSend.GetLength() == 0) return;

	char chSend[1024] = { 0 };
	int nLength = strSend.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)CStringA(strSend), nLength);

	if (!m_Udp.Write_Socket((BYTE*)chSend, nLength)) return;

	CString strTemp;
	strTemp.Format(_T("[->] %s"), strSend);
	((CListBox *)GetDlgItem(IDC_LST_MSG))->AddString(strTemp);

	int nListCount = ((CListBox *)GetDlgItem(IDC_LST_MSG))->GetCount();
	((CListBox *)GetDlgItem(IDC_LST_MSG))->SetCurSel(nListCount - 1);

	BOOL bClear = ((CButton*)GetDlgItem(IDC_CHK_SEND_CLEAR))->GetCheck();
	if (bClear) SetDlgItemText(IDC_EDT_SEND, _T(""));
}

void CMainDlg::OnBnClickedBtnMsgClear()
{
	((CListBox *)GetDlgItem(IDC_LST_MSG))->ResetContent();
}

void CMainDlg::OnBnClickedBtnExit()
{
	m_Udp.Close_Socket();
	DestroyWindow();
}

/////////////////////////////////////////////////////////////////////
