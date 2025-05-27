// ManualRepeatRunDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMI8000.h"
#include "ManualRepeatRunDlg.h"
#include "afxdialogex.h"

#include "Common.h"
#include "AJinDefine.h"
#include "AJinAXL.h"
#include "LogFile.h"

// CManualRepeatRunDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualRepeatRunDlg, CDialogEx)

CManualRepeatRunDlg::CManualRepeatRunDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualRepeatRunDlg::IDD, pParent)
{

}

CManualRepeatRunDlg::~CManualRepeatRunDlg()
{
}

void CManualRepeatRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_0, m_Group[0]);
	for(int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	DDX_Control(pDX, IDC_CBO_PICKER, m_cboPicker);
	DDX_Control(pDX, IDC_CBO_PICK_NUM, m_cboPickNum);
	DDX_Control(pDX, IDC_EDT_DELAY, m_edtDelay);
	DDX_Control(pDX, IDC_CHK_REPEAT_RUN, m_chkRepeatRun);
}


BEGIN_MESSAGE_MAP(CManualRepeatRunDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_REPEAT_RUN, &CManualRepeatRunDlg::OnBnClickedChkRepeatRun)
	ON_CBN_SELCHANGE(IDC_CBO_PICKER, &CManualRepeatRunDlg::OnCbnSelchangeCboPicker)
	ON_CBN_SELCHANGE(IDC_CBO_PICK_NUM, &CManualRepeatRunDlg::OnCbnSelchangeCboPickNum)
END_MESSAGE_MAP()


// CManualRepeatRunDlg 메시지 처리기입니다.


BOOL CManualRepeatRunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_cboPicker.AddString("Btm 1 Picker");
	m_cboPicker.AddString("Btm 2 Picker");
	m_cboPicker.AddString("Sort 1 Picker");
	m_cboPicker.AddString("Sort 2 Picker");

	m_cboPicker.SetCurSel(0);
	m_cboPickNum.SetCurSel(0);

	m_nPickerSelected = 0;
	m_nPickerNumSelected = 0;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CManualRepeatRunDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CManualRepeatRunDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CManualRepeatRunDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CManualRepeatRunDlg::Initial_Controls()
{
	m_Group[0].Init_Ctrl("Arial", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_cboPicker.Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_cboPickNum.Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_edtDelay.Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_chkRepeatRun.Init_Ctrl("Arial", 10, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CManualRepeatRunDlg::OnBnClickedChkRepeatRun()
{
	m_cboPicker.EnableWindow(!m_chkRepeatRun.GetCheck());
	m_cboPickNum.EnableWindow(!m_chkRepeatRun.GetCheck());
	m_edtDelay.EnableWindow(!m_chkRepeatRun.GetCheck());

	CString strText, strTemp;
	if (m_chkRepeatRun.GetCheck()) {

		int nMotionNo = g_objCommon.Check_MotionPos();
		if (nMotionNo < 99) {
			double dCurrentPos = g_objAJinAXL.Get_Position(nMotionNo);
			CString strName = g_objAJinAXL.Get_AxisName(nMotionNo);
			strTemp.Format("Motion(%s) 위치를 Check 하세요.\n이전위치(%0.3lf) != 현재위치(%0.3lf)", strName, gAlm.dMotionPos[nMotionNo], dCurrentPos);
			g_objLogFile.Save_HandlerLog(strTemp);

			g_objCommon.Show_MsgBox(1, strTemp);			
			return;
		}

		m_nPickerSelect = m_cboPicker.GetCurSel();
		m_nPickerNum = m_cboPickNum.GetCurSel();
		m_edtDelay.GetWindowText(strText);
		m_nActionDelay = atoi(strText);

		m_bThreadAction = TRUE;
		m_pThreadAction = AfxBeginThread(Thread_ActionRun, this);
	} else {
		if (!m_pThreadAction) return;
		m_bThreadAction = FALSE;
		WaitForSingleObject(m_pThreadAction->m_hThread, INFINITE);
	}
}


UINT CManualRepeatRunDlg::Thread_ActionRun(LPVOID lpVoid)
{
	CManualRepeatRunDlg* pOwner = (CManualRepeatRunDlg*)lpVoid;

	while (pOwner->m_bThreadAction) {
		pOwner->Repeat_Action();
	} 
	pOwner->m_bThreadAction = FALSE;
	pOwner->m_pThreadAction = NULL;

	return 0;
}


void CManualRepeatRunDlg::Repeat_Action()
{

	switch(m_nRepeatCase)
	{
	case 0:
		{
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 1);
			m_nRepeatCase = 100;
		}
		break;
	//
	case 100:
		if(g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 1))
		{
			theApp.uSleep(300);
			g_objCommon.Set_Btm1PickerOpen(m_nPickerNumSelected);
			m_nRepeatCase = 110;
		}
		break;
	case 110:
		break;



	}
	/*DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();

	if (m_nActionItem == 0) { pDY00->oCleanUnitIn = TRUE; pDY00->oCleanUnitOut = FALSE; g_objAJinAXL.Write_Output(0); }
	if (m_nActionItem == 1) { pDY00->oCleanUnitUp = FALSE; pDY00->oCleanUnitDown = TRUE; g_objAJinAXL.Write_Output(0); }
	if (m_nActionItem == 2) g_objCommon.Set_BtmFlipperClose(0);
	Sleep(m_nActionDelay);

	if (m_nActionItem == 0) { pDY00->oCleanUnitIn = FALSE; pDY00->oCleanUnitOut = TRUE; g_objAJinAXL.Write_Output(0); }
	if (m_nActionItem == 1) { pDY00->oCleanUnitUp = TRUE; pDY00->oCleanUnitDown = FALSE; g_objAJinAXL.Write_Output(0); }
	if (m_nActionItem == 2) g_objCommon.Set_BtmFlipperOpen(0);
	Sleep(m_nActionDelay);*/
}

void CManualRepeatRunDlg::OnCbnSelchangeCboPicker()
{
	m_cboPickNum.ResetContent();

	if(m_cboPicker.GetCurSel() == 0)
	{
		for(int i = 1; i < gData.nBtmPickQt+1 ; i++)
		{	
			m_strLog.Format("Btm1 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}
		for(int i = gData.nBtmPickQt+1; i < gData.nBtmPickQt*2+1 ; i++)
		{	
			m_strLog.Format("Btm1 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}
	}
	else if(m_cboPicker.GetCurSel() == 1)
	{
		for(int i = 1; i < gData.nBtmPickQt+1 ; i++)
		{	
			m_strLog.Format("Btm2 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}
		for(int i = gData.nBtmPickQt+1; i < gData.nBtmPickQt*2+1 ; i++)
		{	
			m_strLog.Format("Btm2 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}
	}
	else if(m_cboPicker.GetCurSel() == 2)
	{
		for(int i = 1; i < gData.nSortPickQt+1 ; i++)
		{	
			m_strLog.Format("Sort 1 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}
		
	}
	else if(m_cboPicker.GetCurSel() == 3)
	{
		for(int i = 1; i < gData.nSortPickQt+1 ; i++)
		{	
			m_strLog.Format("Sort 2 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}
		
	}
}


void CManualRepeatRunDlg::OnCbnSelchangeCboPickNum()
{
	m_nPickerNumSelected = m_cboPickNum.GetCurSel();
}
