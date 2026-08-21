// SetupEquipDlg.h : 헤더 파일
//
#pragma once

#include "SetupMotionDlg.h"
#include "SetupMoveDlg.h"
// CSetupEquipDlg 대화 상자입니다.

class CSetupEquipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupEquipDlg)

public:
	CSetupEquipDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupEquipDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_EQUIP_DLG };
	CGroupCS	m_Group[8];
	CLabelCS	m_Label[40];
	CStaticCS	m_stcEquipName;
	CRadioCS	m_rdoModel[2];	// 0:R63B, 1:R64B
	CComboCS	m_cboLotBarcodePort;
	CStaticCS	m_stcScreenOff;
	CStaticCS	m_stcNoWorkTime;
	CStaticCS	m_stcLogDate;
	CCheckCS	m_chkManualTaktTest;
	CStaticCS	m_stcMotionCheck;
	CLabelCS	m_lblDoorLock;
	CLabelCS	m_lblDoorLockTime;
	CStaticCS	m_stcDoorLockTime;
	CRadioCS	m_rdoDoorLock[2];
	CCheckCS	m_chkUseVisionAlign;
	CCheckCS	m_chkUseInspectAngle;
	CCheckCS	m_chkUseInspectBtm1_SP;
	CCheckCS	m_chkUseInspectBtm1_AG;
	CCheckCS	m_chkUseInspectBtm1_3D;
	CCheckCS	m_chkUseInspectTop1;
	CCheckCS	m_chkUseInspectTop2;
	CCheckCS	m_chkUseInspectBtm2;
	CCheckCS	m_chkUseDispatcher;
	CCheckCS	m_chkUseInline;
	CCheckCS	m_chkUseBtm2;
	CCheckCS	m_chkUseMesApd;
	CCheckCS	m_chkUseSNgMix;
	CCheckCS	m_chkUseRosSkip;
	CCheckCS	m_chkUseInspectSkip;
	CCheckCS	m_chkUsePMTrigger;
	CCheckCS    m_chkUse2ndAVI;

	CCheckCS	m_chkUseDryRun;

	CStaticCS	m_stcTrayData[4];
	CStaticCS	m_stcInspectData[8];
	CStaticCS	m_stcNgBufferData[1];
	CStaticCS	m_stcVacOffDelay[5];
	CStaticCS	m_stcVacOffRepeat[5];
	CStaticCS	m_stcDelayAdd[5];
	CGroupCS	m_grpRos;
	CLabelCS	m_lblRos[3];
	CIPAddressCtrl	m_ipaDispatcherIp;
	CIPAddressCtrl	m_ipaHandlerIp;
	CButtonCS	m_btnRosConnect;
	CButtonCS	m_btnRosDisconnect;
	CStaticCS	m_stcJudgeTimeOver;
	CCheckCS	m_chkJudgeSpecialNg;
	CGroupCS	m_grpCap;
	CLabelCS	m_lblCap;
	CIPAddressCtrl	m_ipaCapAttachIp;
	CButtonCS	m_btnCapConnect;
	CButtonCS	m_btnCapDisconnect;
	CGroupCS	m_grpTower;
	CLabelCS	m_lblTower[10];
	CCheckCS	m_chkTower[6][4];
	CGroupCS	m_grpBuzzer;
	CLabelCS	m_lblBuzzer[8];
	CCheckCS	m_chkBuzzer[2][6];
	CGroupCS	m_grpHidden;
	CLabelCS	m_lblPasswordMt;
	CStaticCS	m_stcPasswordMt;
	CLabelCS	m_lblPasswordSi;
	CEditCS		m_edtPasswordSi;
	CGroupCS	m_grpResultTest;
	CCheckCS	m_chkResultTestUse;
	CLabelCS	m_lblResultTest[4];
	CEditCS		m_edtResultTest[4];
	CStaticCS	m_stcVisProgVer;
	CStaticCS	m_stcVisParaVer;
	CCheckCS	m_chkUseFocusLog;
	CStaticCS	m_stcTmrFocusLog;
	CStaticCS	m_stcJigNgRate;
	CStaticCS	m_stcJigNgCnt;
	CStaticCS   m_stcTriggerData[4];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStnClickedStcEquipName();
	afx_msg void OnStnClickedStcScreenOff();
	afx_msg void OnStcTrayDataClick(UINT nID);
	afx_msg void OnStcInspectDataClick(UINT nID);
	afx_msg void OnStcNgBufferDataClick(UINT nID);
	afx_msg void OnStcVacOffDelayClick(UINT nID);
	afx_msg void OnStcVacOffRepeatClick(UINT nID);
	afx_msg void OnStcDelayAddClick(UINT nID);
	afx_msg void OnStcTriggerDataClick(UINT nID);
	afx_msg void OnBnClickedBtnRosConnect();
	afx_msg void OnBnClickedBtnRosDisconnect();
	afx_msg void OnStnClickedStcJudgeTimeOver();
	afx_msg void OnBnClickedBtnCapConnect();
	afx_msg void OnBnClickedBtnCapDisconnect();
	afx_msg void OnStnClickedStcShowHidden();
	afx_msg void OnStnClickedStcPasswordMt();
	afx_msg void OnStnClickedStcNoworkTime();
	afx_msg void OnStnClickedStcLogDate();
	afx_msg void OnStnClickedStcMotionCheck();
	afx_msg void OnStnClickedStcVisProgVer();
	afx_msg void OnStnClickedStcVisParaVer();
	afx_msg void OnStnClickedStcTmrFocusLog();
	afx_msg void OnStnClickedStcJigNgRate();
	afx_msg void OnStnClickedStcJigNgCnt();

private:
	CString m_strLog;

	void Initial_Controls();
	void Display_EquipData();
	void Disable_RosCtrl(BOOL bDisable);

public:
	void Set_Enable(BOOL bEnable);
		
	void Save_EquipData();
	void Save_EquipDataForAllParam();
	void Cancel_EquipData();
	void Update_Ip(CString sIp) { m_ipaDispatcherIp.SetWindowText(sIp); }

	void SaveLog_DoorInterlock(CString resultCode, CString doorLockUse);
	
	afx_msg void OnStnClickedStcDoorlockTime();
	afx_msg void OnBnClickedChkUseVisionAlign();
	afx_msg void OnBnClickedChkUseInspectAngle();
	afx_msg void OnBnClickedChkUseInspectBtm1Sp();
	afx_msg void OnBnClickedChkUseInspectBtm1Ag();
	afx_msg void OnBnClickedChkUseInspectBtm13d();
	afx_msg void OnBnClickedChkUseInspectTop1();
	afx_msg void OnBnClickedChkUseInspectTop2();
	afx_msg void OnBnClickedChkUseInspectBtm2();
	afx_msg void OnBnClickedChkUseInline();
	afx_msg void OnBnClickedChkUseBtm2();
	afx_msg void OnBnClickedRdoDoorLock0();
	afx_msg void OnBnClickedRdoDoorLock1();

	afx_msg void OnBnClickedChkUseDryRun();
};

///////////////////////////////////////////////////////////////////////////////
