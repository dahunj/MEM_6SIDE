// ManualUnloadDlg.h : 헤더 파일
//
#pragma once

// CManualNgDlg 대화 상자입니다.

class CManualUnloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualUnloadDlg)

public:
	CManualUnloadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualUnloadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_UNLOAD_DLG };
	CGroupCS	m_Group[17];
	CLabelCS	m_Label[8];
	CStaticCS	m_stcAxisPos[8];
	CButtonCS	m_btnGoodStage1Y[6];
	CButtonCS	m_btnGoodStage1Z[5];
	CButtonCS	m_btnGoodStage1Io[4];
	CLedCS		m_ledGoodStage1Io[5];
	CButtonCS	m_btnGoodStage2Y[6];
	CButtonCS	m_btnGoodStage2Z[5];
	CButtonCS	m_btnGoodStage2Io[4];
	CLedCS		m_ledGoodStage2Io[5];
	CButtonCS	m_btnGoodPortIo[2];
	CLedCS		m_ledGoodPortIo[8];
	CButtonCS	m_btnNgStageY[5];
	CLedCS		m_ledNgStageIo[6];
	CButtonCS	m_btnNgPortIo[2];
	CLedCS		m_ledNgPortIo[5];
	CButtonCS	m_btnEmptyTrans1X[2];
	CButtonCS	m_btnEmptyTrans1Io[6];
	CLedCS		m_ledEmptyTrans1Io[7];
	CButtonCS	m_btnEmptyPortZ[8];
	CLedCS		m_ledEmptyPortIo[5];
	CButtonCS	m_btnEmptyTrans2Y[2];
	CButtonCS	m_btnEmptyTrans2Io[6];
	CLedCS		m_ledEmptyTrans2Io[7];
	CButtonCS	m_btnGoodTrayBufferIo[2];
	CLedCS		m_ledGoodTrayBufferIo[5];
	CButtonCS	m_btnNgBufferIo[24];
	CLedCS		m_ledNgBufferIo[10];

	CRadioCS		m_rdoSkipOn;
	CRadioCS		m_rdoSkipOff;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnGoodStage1YClick(UINT nID);
	afx_msg void OnBtnGoodStage1ZClick(UINT nID);
	afx_msg void OnBtnGoodStage1IoClick(UINT nID);
	afx_msg void OnBtnGoodStage2YClick(UINT nID);
	afx_msg void OnBtnGoodStage2ZClick(UINT nID);
	afx_msg void OnBtnGoodStage2IoClick(UINT nID);
	afx_msg void OnBtnGoodPortIoClick(UINT nID);
	afx_msg void OnBtnNgStageYClick(UINT nID);
	afx_msg void OnBtnNgPortIoClick(UINT nID);
	afx_msg void OnBtnEmptyTrans1XClick(UINT nID);
	afx_msg void OnBtnEmptyTrans1IoClick(UINT nID);
	afx_msg void OnBtnEmptyPortZClick(UINT nID);
	afx_msg void OnBtnEmptyTrans2YClick(UINT nID);
	afx_msg void OnBtnEmptyTrans2IoClick(UINT nID);
	afx_msg void OnBtnGoodBufferIoClick(UINT nID);
	afx_msg void OnBtnNgBufferIoClick(UINT nID);

private:
	CString m_strLog;
	void Initial_Controls();

public:
	void Display_Status();
	
	
	afx_msg void OnBnClickedRdoSkipon();
	afx_msg void OnBnClickedRdoSkipoff();
};
