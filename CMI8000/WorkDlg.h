// WorkDlg.h : 헤더 파일
//
#pragma once

#include "WorkInfoDlg.h"

#define UM_UPDATE_MODEL			WM_USER+1
#define UM_UPDATE_TRAY_INFO		WM_USER+2
#define UM_UPDATE_BARCODE		WM_USER+3
#define UM_RESET_CYCLE_STOP		WM_USER+4
#define UM_UPDATE_UPH			WM_USER+5
#define UM_VISION_RESULT		WM_USER+6
#define UM_LOT_START_END		WM_USER+7
#define UM_LOT_INFO_CLEAR		WM_USER+8
#define UM_MODEL_CHANGE			WM_USER+9
#define UM_SHOW_MSG				WM_USER+10
#define UM_UPDATE_LASER			WM_USER+11

// CWorkDlg 대화 상자입니다.

class CWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkDlg)

public:
	CWorkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORK_DLG };
	CGroupCS		m_Group[6];
	CLabelCS		m_Label[7];
	CLabelCS		m_lblLot[6];
	CGroupCS		m_grpLot[2];
	CStaticCS		m_stcLotId[2];
	CStaticCS		m_stcTrayCount[2];
	CStaticCS		m_stcCmCount[2];
	CPictureCS		m_imgEquipment;
	CPictureCS		m_picUphBack;
	CLedCS			m_ledEquipOption[10];
	CComboCS		m_cboStartIndexJig;
	CStaticCS		m_stcB1No[10];
	CStaticCS		m_stcB2No[10];
	CStaticCS		m_stcInsp1No[10];
	CStaticCS		m_stcInsp2No[10];
	CStaticCS		m_stcInsp3No[10];
	CStaticCS		m_stcS1No[5];
	CStaticCS		m_stcS2No[5];
	CStaticCS		m_stcCycleTime;
	CRadioCS		m_rdoWorkStart;
	CRadioCS		m_rdoWorkStop;
	CLedCS			m_ledInitComplete;
	CLedCS			m_ledLotLoadEnable;
	CCheckCS		m_chkCycleStop;
	CCheckCS		m_chkMesUse;
	CCheckCS		m_chkLotIDInsert;
	CPictureCS		m_picTrayBack[6];
	CStaticCS		m_stcAngleTrayCount[2];
	CStaticCS		m_stcBufferTrayCount[2];
	CStaticCS		m_stcNgTrayCount;
	CStaticCS		m_stcGoodTrayCount;
	CGridCS			m_grdAngleTray[2];
	CGridCS			m_grdNgTray[4];
	CGridCS			m_grdBufferTray[2];
	CGridCS			m_grdGoodTray;
	CStaticCS		m_stcWorkCase[20];
	CStaticCS		m_stcTakt[4];
	CStaticCS		m_stcUph[4];
	CStaticCS		m_stcDay[2];
	CLedCS			m_ledVisionStatus[7];
	CStaticCS		m_stcVisionResult[16];
	CButtonCS		m_btnLotID[2];
	CButtonCS		m_btnNgClear[4];
	CButtonCS		m_btnBuffChange;
	CButtonCS		m_btnBuffComplete;
	CStaticCS		m_stcLaserResult[2];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStcLotIdClick(UINT nID);
	afx_msg void OnStcCmCountClick(UINT nID);
	afx_msg void OnStcBtm1InfoClick(UINT nID);
	afx_msg void OnBnClickedRdoWorkStart();
	afx_msg void OnBnClickedRdoWorkStop();
	afx_msg void OnBnClickedChkCycleStop();
	afx_msg void OnBnClickedChkMesUse();
	afx_msg void OnBnClickedBtnMesAbort();
	afx_msg void OnBnClickedBtnMesManual();
	afx_msg void OnBnClickedLotID(UINT nID);
	afx_msg void OnBnClickedNgClear(UINT nID);
	afx_msg LRESULT OnUpdateModel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateTrayInfo(WPARAM nTray, LPARAM lParam);
	afx_msg LRESULT OnUpdateBarcode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateLaser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResetCycleStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateUph(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnVisionResult(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLotStartEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLotInfoClear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnModelChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnBufferChange();
	afx_msg void OnBnClickedBtnBufferComplete();
	

private:
	CWorkInfoDlg *m_pWorkInfoDlg;

	CBitmap	m_bmpEquipment;
	BOOL	m_bAutoRunning;
	int		m_nPortIdx;

	int		m_nBuffChange;

	BOOL	m_bMesManual;

private:
	BOOL Work_Start();
	void Initial_Controls();
	void Initial_Grid(CGridCS *pGrid, int nRows, int nCols, int nType=1);
	void Check_Lamp();
	void Display_Status();
	void Reset_AlarmLog();

public:
	void Set_AutoRun(BOOL bAutoRun);
	void Set_AutoRunStatus(BOOL bAutoRun) { m_bAutoRunning = bAutoRun; }
	void Set_State(int nState);
	void Enable_UserInput(int nNo, BOOL bEnable);
	void Get_LotInfo(int nNo);
	void MachineStopLog(CString sType);

	void WriteCMCount(int nPortNo);
	void FakeLoadPortSensor();

	afx_msg void OnBnClickedBtnIdleReport();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	
};

extern CWorkDlg g_dlgWork;

///////////////////////////////////////////////////////////////////////////////
