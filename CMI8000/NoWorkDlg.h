#pragma once


// CNoWorkDlg 대화 상자입니다.

class CNoWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNoWorkDlg)

public:
	CNoWorkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNoWorkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NO_WORK_DLG };
	CGroupCS	m_Group[3];
	CLabelCS	m_Label[5];
	CStaticCS	m_stcNoWorkOpId;
	CStaticCS	m_stcNoWorkTime[3];
	CButtonCS	m_btnStopReason[24];
	CStaticCS	m_stcNoWorkExit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedStcNoWorkOpId();
	afx_msg void OnBtnStopReasonClick(UINT nID);
	afx_msg void OnStnClickedStcNoWorkExit();

private:
	CString		m_strOpId;
	CString		m_strCode;
	CString		m_strText;
	CString		m_strData[24][3];

	DWORD		m_dwNoWorkStart;
	CString		m_strNoWorkStart;

private:
	void Initial_Controls();
	BOOL Read_StopLossList();
	void Set_NoWorkReport(BOOL bSet);

public:
	void Start_NoWork(BOOL bNow);
};

extern CNoWorkDlg g_dlgNoWork;