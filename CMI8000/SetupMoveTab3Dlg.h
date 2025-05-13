// SetupMoveTab3Dlg.h : 헤더 파일
//
#pragma once

// CSetupMoveTab3Dlg 대화 상자입니다.

class CSetupMoveTab3Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMoveTab3Dlg)

public:
	CSetupMoveTab3Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMoveTab3Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOVE_TAB3_DLG };
	CGroupCS	m_Group[14];
	CLabelCS	m_Label[63];
	CStaticCS	m_stcSortPicker1X[7];
	CStaticCS	m_stcSortPicker1Z[6];
	CStaticCS	m_stcSortPicker1P[2];
	CStaticCS	m_stcSortPicker2X[7];
	CStaticCS	m_stcSortPicker2Z[6];
	CStaticCS	m_stcSortPicker2P[2];
	CStaticCS	m_stcGoodStage1Y[4];
	CStaticCS	m_stcGoodStage1Z[5];
	CStaticCS	m_stcGoodStage2Y[4];
	CStaticCS	m_stcGoodStage2Z[5];
	CStaticCS	m_stcNgStageY[5];
	CStaticCS	m_stcEmptyTrans1X[4];
	CStaticCS	m_stcEmptyPortZ[3];
	CStaticCS	m_stcEmptyTrans2Y[3];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStcSortPicker1XClick(UINT nID);
	afx_msg void OnStcSortPicker1ZClick(UINT nID);
	afx_msg void OnStcSortPicker1PClick(UINT nID);
	afx_msg void OnStcSortPicker2XClick(UINT nID);
	afx_msg void OnStcSortPicker2ZClick(UINT nID);
	afx_msg void OnStcSortPicker2PClick(UINT nID);
	afx_msg void OnStcGoodStage1YClick(UINT nID);
	afx_msg void OnStcGoodStage1ZClick(UINT nID);
	afx_msg void OnStcGoodStage2YClick(UINT nID);
	afx_msg void OnStcGoodStage2ZClick(UINT nID);
	afx_msg void OnStcNgStageYClick(UINT nID);
	afx_msg void OnStcEmptyTrans1XClick(UINT nID);
	afx_msg void OnStcEmptyPortZClick(UINT nID);
	afx_msg void OnStcEmptyTrans2YClick(UINT nID);

private:
	void Initial_Controls();
	void Display_MoveData();

public:
	void Save_MoveData();
	void Save_MoveDataForAll();
	void Cancel_MoveData();
};

///////////////////////////////////////////////////////////////////////////////
