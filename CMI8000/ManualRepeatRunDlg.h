#pragma once


// CManualRepeatRunDlg 대화 상자입니다.

class CManualRepeatRunDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualRepeatRunDlg)

public:
	CManualRepeatRunDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualRepeatRunDlg();

	CGroupCS	m_Group[1];
	CLabelCS	m_Label[3];
	CComboCS	m_cboPicker;
	CComboCS	m_cboPickNum;
	CEditCS		m_edtDelay;
	CCheckCS	m_chkRepeatRun;
	CLabelCS	m_lblCase;

	CEditCS		m_edtMsg;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_REPEAT_RUN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CString m_strLog;
	void Initial_Controls();

protected:
	CString m_strTemp;

	int m_nRepeatCase;

	int m_nPickerSelected;
	int m_nPickerNumSelected;

	int	 m_nPickerSelect;
	int	 m_nPickerNum;
	int	 m_nActionDelay;
	void Repeat_Action();

	BOOL		m_bThreadAction;
	CWinThread* m_pThreadAction;
	static UINT	Thread_ActionRun(LPVOID lpVoid);

public:
	void AddComboListPicker();
	BOOL CheckMotionPos();


public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedChkRepeatRun();
	afx_msg void OnCbnSelchangeCboPicker();
	afx_msg void OnCbnSelchangeCboPickNum();
	
};
