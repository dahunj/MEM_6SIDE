// SetupMoveTab1Dlg.h : 헤더 파일
//
#pragma once

// CSetupMoveTab1Dlg 대화 상자입니다.

class CSetupMoveTab1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMoveTab1Dlg)

public:
	CSetupMoveTab1Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMoveTab1Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOVE_TAB1_DLG };
	CGroupCS	m_Group[17];
	CLabelCS	m_Label[61];
	CStaticCS	m_stcLoadStageX[3];
	CStaticCS	m_stcLoadStageZ[4];
	CStaticCS	m_stcLoadPickerY[2];
	CStaticCS	m_stcAngleUnitY[2];
	CStaticCS	m_stcAngleUnitX[2];
	CStaticCS	m_stcAngleUnitR[2];
	CStaticCS	m_stcVisionAngleZ[1];
	CStaticCS	m_stcVisionAlignZ[2];
	CStaticCS	m_stcAngleStage1Y[5];
	CStaticCS	m_stcAngleStage1Z[6];
	CStaticCS	m_stcAngleStage2Y[5];
	CStaticCS	m_stcAngleStage2Z[6];
	CStaticCS	m_stcBtm1PickerX[9];
	CStaticCS	m_stcBtm1PickerZ[6];
	CStaticCS	m_stcBtm1PickerP1[2];
	CStaticCS	m_stcBtm1PickerP2[2];
	CStaticCS	m_stcBtm1ShiftY[2];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStcLoadStageXClick(UINT nID);
	afx_msg void OnStcLoadStageZClick(UINT nID);
	afx_msg void OnStcLoadPickerYClick(UINT nID);
	afx_msg void OnStcAngleUnitYClick(UINT nID);
	afx_msg void OnStcAngleUnitXClick(UINT nID);
	afx_msg void OnStcAngleUnitRClick(UINT nID);
	afx_msg void OnStcVisionAngleZClick(UINT nID);
	afx_msg void OnStcVisionAlignZClick(UINT nID);
	afx_msg void OnStcAngleStage1YClick(UINT nID);
	afx_msg void OnStcAngleStage1ZClick(UINT nID);
	afx_msg void OnStcAngleStage2YClick(UINT nID);
	afx_msg void OnStcAngleStage2ZClick(UINT nID);
	afx_msg void OnStcBtm1PickerXClick(UINT nID);
	afx_msg void OnStcBtm1PickerZClick(UINT nID);
	afx_msg void OnStcBtm1PickerP1Click(UINT nID);
	afx_msg void OnStcBtm1PickerP2Click(UINT nID);
	afx_msg void OnStcBtm1ShiftYClick(UINT nID);

private:
	void Initial_Controls();
	void Display_MoveData();

public:
	void Save_MoveData();
	void Save_MoveDataForAll();
	void Cancel_MoveData();
};

///////////////////////////////////////////////////////////////////////////////
