// ManualLoadDlg.h : 헤더 파일
//
#pragma once

// CManualLoadDlg 대화 상자입니다.

class CManualLoadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualLoadDlg)

public:
	CManualLoadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualLoadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_LOAD_DLG };
	CGroupCS	m_Group[19];
	CLabelCS	m_Label[12];
	CStaticCS	m_stcAxisPos[12];
	CButtonCS	m_btnLoadPortIo[8];
	CLedCS		m_ledLoadPortIo[23];
	CButtonCS	m_btnLoadStageX[3];
	CButtonCS	m_btnLoadStageZ[4];
	CButtonCS	m_btnLoadStageIo[4];
	CLedCS		m_ledLoadStageIo[5];
	CButtonCS	m_btnLoadPickerY[2];
	CButtonCS	m_btnLoadPickerIo[6];
	CLedCS		m_ledLoadPickerIo[7];
	CButtonCS	m_btnAngleUnitY[4];
	CButtonCS	m_btnAngleUnitX[4];
	CButtonCS	m_btnAngleUnitR[2];
	CButtonCS	m_btnVisionAngleZ[1];
	CButtonCS	m_btnVisionAlignZ[2];
	CButtonCS	m_btnAnglePortIo[6];
	CLedCS		m_ledAnglePortIo[9];
	CButtonCS	m_btnAngleStage1Y[7];
	CButtonCS	m_btnAngleStage1Z[5];
	CButtonCS	m_btnAngleStage1Io[4];
	CLedCS		m_ledAngleStage1Io[5];
	CButtonCS	m_btnAngleStage2Y[7];
	CButtonCS	m_btnAngleStage2Z[5];
	CButtonCS	m_btnAngleStage2Io[4];
	CLedCS		m_ledAngleStage2Io[5];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnLoadPortIoClick(UINT nID);
	afx_msg void OnBtnLoadStageXClick(UINT nID);
	afx_msg void OnBtnLoadStageZClick(UINT nID);
	afx_msg void OnBtnLoadStageIoClick(UINT nID);
	afx_msg void OnBtnLoadPickerYClick(UINT nID);
	afx_msg void OnBtnLoadPickerIoClick(UINT nID);
	afx_msg void OnBtnAngleUnitYClick(UINT nID);
	afx_msg void OnBtnAngleUnitXClick(UINT nID);
	afx_msg void OnBtnAngleUnitRClick(UINT nID);
	afx_msg void OnBtnVisionAngleZClick(UINT nID);
	afx_msg void OnBtnVisionAlignZClick(UINT nID);
	afx_msg void OnBtnAnglePortIoClick(UINT nID);
	afx_msg void OnBtnAngleStage1YClick(UINT nID);
	afx_msg void OnBtnAngleStage1ZClick(UINT nID);
	afx_msg void OnChkAngleStage1IoClick(UINT nID);
	afx_msg void OnBtnAngleStage1IoClick(UINT nID);
	afx_msg void OnBtnAngleStage2YClick(UINT nID);
	afx_msg void OnBtnAngleStage2ZClick(UINT nID);
	afx_msg void OnChkAngleStage2IoClick(UINT nID);
	afx_msg void OnBtnAngleStage2IoClick(UINT nID);

private:
	CString m_strLog;
	void Initial_Controls();

public:
	void Display_Status();
};
