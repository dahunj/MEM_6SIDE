// SetupMoveTab2Dlg.h : 헤더 파일
//
#pragma once

// CSetupMoveTab2Dlg 대화 상자입니다.

class CSetupMoveTab2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMoveTab2Dlg)

public:
	CSetupMoveTab2Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMoveTab2Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOVE_TAB2_DLG };
	CGroupCS	m_Group[15];
	CLabelCS	m_Label[55];
	CStaticCS	m_stcInspectStage1X[6];
	CStaticCS	m_stcInspectStage2X[6];
	CStaticCS	m_stcInspectStage3X[6];
	CStaticCS	m_stcModuleAlignY[3];
	CStaticCS	m_stcModuleAlignZ[2];
	CStaticCS	m_stcVisionTop1Z[1];
	CStaticCS	m_stcTop1MirrorZ[2];
	CStaticCS	m_stcTop2ShiftY[2];
	CStaticCS	m_stcVisionTop2Z[1];
	CStaticCS	m_stcBtm2PickerX[6];
	CStaticCS	m_stcBtm2PickerZ[4];
	CStaticCS	m_stcBtm2PickerP1[2];
	CStaticCS	m_stcBtm2PickerP2[2];
	CStaticCS	m_stcBufferStage1Y[6];
	CStaticCS	m_stcBufferStage2Y[6];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStcInspectStage1XClick(UINT nID);
	afx_msg void OnStcInspectStage2XClick(UINT nID);
	afx_msg void OnStcInspectStage3XClick(UINT nID);
	afx_msg void OnStcModuleAlignYClick(UINT nID);
	afx_msg void OnStcModuleAlignZClick(UINT nID);
	afx_msg void OnStcVisionTop1ZClick(UINT nID);
	afx_msg void OnStcTop1MirrorZClick(UINT nID);
	afx_msg void OnStcTop2ShiftYClick(UINT nID);
	afx_msg void OnStcVisionTop2ZClick(UINT nID);
	afx_msg void OnStcBtm2PickerXClick(UINT nID);
	afx_msg void OnStcBtm2PickerZClick(UINT nID);
	afx_msg void OnStcBtm2PickerP1Click(UINT nID);
	afx_msg void OnStcBtm2PickerP2Click(UINT nID);
	afx_msg void OnStcBufferStage1YClick(UINT nID);
	afx_msg void OnStcBufferStage2YClick(UINT nID);

private:
	void Initial_Controls();
	void Display_MoveData();

public:
	void Save_MoveData();
	void Save_MoveDataForAll();
	void Cancel_MoveData();
};

///////////////////////////////////////////////////////////////////////////////
