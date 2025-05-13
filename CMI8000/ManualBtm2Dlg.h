// ManualBtm2Dlg.h : 헤더 파일
//
#pragma once

// CManualBtm2Dlg 대화 상자입니다.

class CManualBtm2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualBtm2Dlg)

public:
	CManualBtm2Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualBtm2Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_BTM2_DLG };
	CGroupCS	m_Group[17];
	CLabelCS	m_Label[12];
	CStaticCS	m_stcAxisPos[12];
	CButtonCS	m_btnBtm2PickerX[8];
	CButtonCS	m_btnBtm2PickerZ[4];
	CButtonCS	m_btnBtm2PickerP1[3];
	CButtonCS	m_btnBtm2PickerP2[3];
	CButtonCS	m_btnBtm2PickerIo[54];
	CLedCS		m_ledBtm2PickerIo[40];
	CButtonCS	m_btnBufferStage1Y[6];
	CCheckCS	m_chkBufferStage1Io[1];
	CButtonCS	m_btnBufferStage1Io[4];
	CLedCS		m_ledBufferStage1Io[5];
	CButtonCS	m_btnBufferStage2Y[6];
	CCheckCS	m_chkBufferStage2Io[1];
	CButtonCS	m_btnBufferStage2Io[4];
	CLedCS		m_ledBufferStage2Io[5];
	CButtonCS	m_btnSortPicker1X[9];
	CButtonCS	m_btnSortPicker1Z[6];
	CButtonCS	m_btnSortPicker1P[2];
	CButtonCS	m_btnSortPicker1Io[24];
	CLedCS		m_ledSortPicker1Io[20];
	CButtonCS	m_btnSortPicker2X[9];
	CButtonCS	m_btnSortPicker2Z[6];
	CButtonCS	m_btnSortPicker2P[2];
	CButtonCS	m_btnSortPicker2Io[24];
	CLedCS		m_ledSortPicker2Io[20];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnBtm2PickerXClick(UINT nID);
	afx_msg void OnBtnBtm2PickerZClick(UINT nID);
	afx_msg void OnBtnBtm2PickerP1Click(UINT nID);
	afx_msg void OnBtnBtm2PickerP2Click(UINT nID);
	afx_msg void OnBtnBtm2PickerIoClick(UINT nID);
	afx_msg void OnBtnBufferStage1YClick(UINT nID);
	afx_msg void OnChkBufferStage1IoClick(UINT nID);
	afx_msg void OnBtnBufferStage1IoClick(UINT nID);
	afx_msg void OnBtnBufferStage2YClick(UINT nID);
	afx_msg void OnChkBufferStage2IoClick(UINT nID);
	afx_msg void OnBtnBufferStage2IoClick(UINT nID);
	afx_msg void OnBtnSortPicker1XClick(UINT nID);
	afx_msg void OnBtnSortPicker1ZClick(UINT nID);
	afx_msg void OnBtnSortPicker1PClick(UINT nID);
	afx_msg void OnBtnSortPicker1IoClick(UINT nID);
	afx_msg void OnBtnSortPicker2XClick(UINT nID);
	afx_msg void OnBtnSortPicker2ZClick(UINT nID);
	afx_msg void OnBtnSortPicker2PClick(UINT nID);
	afx_msg void OnBtnSortPicker2IoClick(UINT nID);

private:
	CString m_strLog;
	void Initial_Controls();

public:
	void Display_Status();
};
