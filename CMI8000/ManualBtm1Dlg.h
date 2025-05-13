// ManualBtm1Dlg.h : 헤더 파일
//
#pragma once

// CManualBtm1Dlg 대화 상자입니다.

class CManualBtm1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualBtm1Dlg)

public:
	CManualBtm1Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualBtm1Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_BTM1_DLG };
	CGroupCS	m_Group[20];
	CLabelCS	m_Label[14];
	CStaticCS	m_stcAxisPos[14];
	CButtonCS	m_btnBtm1PickerX[12];
	CButtonCS	m_btnBtm1PickerZ[6];
	CButtonCS	m_btnBtm1PickerP1[3];
	CButtonCS	m_btnBtm1PickerP2[3];
	CButtonCS	m_btnBtm1ShiftY[2];
	CButtonCS	m_btnBtm1PickerIo[54];
	CLedCS		m_ledBtm1PickerIo[40];
	CButtonCS	m_btnInspectStage1X[10];
	CCheckCS	m_chkInspectStage1Io[8];
	CButtonCS	m_btnInspectStage1Io[4];
	CLedCS		m_ledInspectStage1Io[10];
	CButtonCS	m_btnInspectStage2X[10];
	CCheckCS	m_chkInspectStage2Io[8];
	CButtonCS	m_btnInspectStage2Io[6];
	CLedCS		m_ledInspectStage2Io[12];
	CButtonCS	m_btnInspectStage3X[10];
	CCheckCS	m_chkInspectStage3Io[8];
	CButtonCS	m_btnInspectStage3Io[6];
	CLedCS		m_ledInspectStage3Io[12];
	CButtonCS	m_btnModuleAlignY[3];
	CButtonCS	m_btnModuleAlignZ[2];
	CButtonCS	m_btnModuleAlignIo[4];
	CLedCS		m_ledModuleAlignIo[4];
	CButtonCS	m_btnVisionTop1Z[1];
	CButtonCS	m_btnTop1MirrorZ[2];
	CButtonCS	m_btnTop1MirrorIo[2];
	CLedCS		m_ledTop1MirrorIo[4];
	CButtonCS	m_btnVisionTop2Z[1];

	CRadioCS		m_rdo3DOn;
	CRadioCS		m_rdo3DOff;

	CRadioCS		m_rdoBlowOn;
	CRadioCS		m_rdoBlowOff;
	CRadioCS		m_rdoSucOn;
	CRadioCS		m_rdoSucOff;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnBtm1PickerXClick(UINT nID);
	afx_msg void OnBtnBtm1PickerZClick(UINT nID);
	afx_msg void OnBtnBtm1PickerP1Click(UINT nID);
	afx_msg void OnBtnBtm1PickerP2Click(UINT nID);
	afx_msg void OnBtnBtm1ShiftYClick(UINT nID);

	afx_msg void OnBtnBtm1PickerIoClick(UINT nID);
	afx_msg void OnBtnInspectStage1XClick(UINT nID);
	afx_msg void OnChkInspectStage1IoClick(UINT nID);
	afx_msg void OnBtnInspectStage1IoClick(UINT nID);
	afx_msg void OnBtnInspectStage2XClick(UINT nID);
	afx_msg void OnChkInspectStage2IoClick(UINT nID);
	afx_msg void OnBtnInspectStage2IoClick(UINT nID);
	afx_msg void OnBtnInspectStage3XClick(UINT nID);
	afx_msg void OnChkInspectStage3IoClick(UINT nID);
	afx_msg void OnBtnInspectStage3IoClick(UINT nID);
	afx_msg void OnBtnModuleAlignYClick(UINT nID);
	afx_msg void OnBtnModuleAlignZClick(UINT nID);
	afx_msg void OnBtnModuleAlignIoClick(UINT nID);
	afx_msg void OnBtnVisionTop1ZClick(UINT nID);
	afx_msg void OnBtnTop1MirrorZClick(UINT nID);
	afx_msg void OnBtnTop1MirrorIoClick(UINT nID);
	afx_msg void OnBtnTop2ShiftYClick(UINT nID);
	afx_msg void OnBtnVisionTop2ZClick(UINT nID);

public:
	int iBtmStep;
private:
	CString m_strLog;
	void Initial_Controls();

public:
	void Display_Status();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnBtm1PickerX12();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtn3don();
	afx_msg void OnBnClickedBtn3doff();


	afx_msg void OnBnClickedRdo3don();
	afx_msg void OnBnClickedRdo3don2();
	afx_msg void OnBnClickedRdo3doff();
	afx_msg void OnBnClickedRdoBlowOn();
	afx_msg void OnBnClickedRdoBlowOff();
	afx_msg void OnBnClickedRdoSucOn();
	afx_msg void OnBnClickedRdoSucOff();
};
