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

#include "CMI8000Dlg.h"
#include "ManualDlg.h"


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
	DDX_Control(pDX, IDC_EDT_MSG, m_edtMsg);
	DDX_Control(pDX, IDC_LBL_CASE, m_lblCase);
}


BEGIN_MESSAGE_MAP(CManualRepeatRunDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
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
	
	AddComboListPicker();
	
	m_nPickerSelected = 0;
	m_nPickerNumSelected = 0;

	m_edtDelay.SetWindowText("1000");

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CManualRepeatRunDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualRepeatRunDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	CString strMsg;
	strMsg = "1. 각 Picker Z Ready Up 시작할것 (아닐시 정지)\r\n";
	strMsg += "2. 정지시 (Repeat Stop) 후 해당 부분 센서 점검필요 \r\n";
	m_edtMsg.SetWindowText(strMsg);

	//Display_Status();

	m_strLog.Format("[Manual Repeat] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
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
	m_edtMsg.Init_Ctrl("Arial", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_chkRepeatRun.Init_Ctrl("Arial", 10, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CManualRepeatRunDlg::OnBnClickedChkRepeatRun()
{
	CString strText, strTemp;

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();

	m_cboPicker.EnableWindow(!m_chkRepeatRun.GetCheck());
	m_cboPickNum.EnableWindow(!m_chkRepeatRun.GetCheck());
	m_edtDelay.EnableWindow(!m_chkRepeatRun.GetCheck());

	pMainDlg->Enable_ModeButton(!m_chkRepeatRun.GetCheck());
	pMainDlg->m_btnMainOperator.EnableWindow(!m_chkRepeatRun.GetCheck());

	g_dlgManual.m_rdoManualBtm1.EnableWindow(!m_chkRepeatRun.GetCheck());
	g_dlgManual.m_rdoManualBtm2.EnableWindow(!m_chkRepeatRun.GetCheck());
	g_dlgManual.m_rdoManualLoad.EnableWindow(!m_chkRepeatRun.GetCheck());
	g_dlgManual.m_rdoManualUnload.EnableWindow(!m_chkRepeatRun.GetCheck());
			
	if (m_chkRepeatRun.GetCheck())
	{		
		if(!CheckMotionPos()) return;

		m_nPickerSelect = m_cboPicker.GetCurSel();
		m_nPickerNum = m_cboPickNum.GetCurSel();
		m_edtDelay.GetWindowText(strText);
		m_nActionDelay = atoi(strText);

		m_bThreadAction = TRUE;
		m_pThreadAction = AfxBeginThread(Thread_ActionRun, this);
	} 
	else
	{
		m_cboPicker.ResetContent();
		AddComboListPicker();

		m_nRepeatCase = 0;
		m_strTemp.Format("%d", m_nRepeatCase);
		m_lblCase.SetWindowText(m_strTemp);

		if (!m_pThreadAction) return;
		m_bThreadAction = FALSE;
		WaitForSingleObject(m_pThreadAction->m_hThread, INFINITE);
	}
}

void CManualRepeatRunDlg::AddComboListPicker()
{
	m_cboPicker.AddString("Btm 1 Picker (Angle Tray)");
	m_cboPicker.AddString("Btm 2 Picker (Buffer)");
	m_cboPicker.AddString("Sort 1 Picker (Buffer)");
	m_cboPicker.AddString("Sort 2 Picker (Buffer)");
}

BOOL CManualRepeatRunDlg::CheckMotionPos()
{
	CString strTemp;

	int nMotionNo = g_objCommon.Check_MotionPos();
	if (nMotionNo < 99) {
		double dCurrentPos = g_objAJinAXL.Get_Position(nMotionNo);
		CString strName = g_objAJinAXL.Get_AxisName(nMotionNo);
		strTemp.Format("Motion(%s) 위치를 Check 하세요.\n이전위치(%0.3lf) != 현재위치(%0.3lf)", strName, gAlm.dMotionPos[nMotionNo], dCurrentPos);
		g_objLogFile.Save_HandlerLog(strTemp);

		g_objCommon.Show_MsgBox(1, strTemp);		
		return FALSE;
	}
	return TRUE;
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

	m_strTemp.Format("%d", m_nRepeatCase);
	m_lblCase.SetWindowText(m_strTemp);

	if(m_nRepeatCase == 100 && !g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0))
	{
		m_bThreadAction = FALSE;
		m_pThreadAction = NULL;
		AfxMessageBox("Btm1 Picker Z Ready Up 아닙니다.");
		return;
	}
	if(m_nRepeatCase == 200 && !g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0))
	{
		m_bThreadAction = FALSE;
		m_pThreadAction = NULL;
		AfxMessageBox("Btm2 Picker Z Ready Up 아닙니다.");
		return;
	}
	if(m_nRepeatCase == 300 && !g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0))
	{
		m_bThreadAction = FALSE;
		m_pThreadAction = NULL;
		AfxMessageBox("Sort 1 Picker Z Ready Up 아닙니다.");
		return;
	}
	if(m_nRepeatCase == 400 && !g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0))
	{
		m_bThreadAction = FALSE;
		m_pThreadAction = NULL;
		AfxMessageBox("Sort 2 Picker Z Ready Up 아닙니다.");
		return;
	}


	switch(m_nRepeatCase)
	{
	case 0:
		break;
	
	//Btm1 Picker :100
	case 100:
		//Btm1 X Tray position 아니면 Stop 
		if(!g_objCommon.Check_Position(AX_BTM1_PICKER_X, 0) && !g_objCommon.Check_Position(AX_BTM1_PICKER_X, 1)
			&& !g_objCommon.Check_Position(AX_BTM1_PICKER_X, 2) && !g_objCommon.Check_Position(AX_BTM1_PICKER_X, 3))
		{
			m_bThreadAction = FALSE;
			m_pThreadAction = NULL;
			AfxMessageBox("Btm1 Picker (Tray) Position 아닙니다.");
			return;
		}

		if(g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0))
		{
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 1); // z down
			m_nRepeatCase = 110;
		}		
		break;
	case 110:
		if(g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_Btm1PickerClose(m_nPickerNumSelected); // close
			m_nRepeatCase = 120;
		}
		break;
	case 120:
		if(g_objCommon.Get_Btm1PickerClose(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0); // z up 
			m_nRepeatCase = 130;
		}
		break;
	case 130:
		if(g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0) && g_objCommon.Get_Btm1PickerClose(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 1); // z down
			m_nRepeatCase = 140;
		}
		break;
	case 140:
		if(g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_Btm1PickerOpen(m_nPickerNumSelected); //open
			m_nRepeatCase = 150;
		}
		break;
	case 150:
		if(g_objCommon.Get_Btm1PickerOpen(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0);// Z up 
			m_nRepeatCase = 100;
		}
		break;
//////////
	case 200:

		//Btm2 X Buffer position 아니면 Stop 
		if(!g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2) && !g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)
			&& !g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4) && !g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5))
		{
			m_bThreadAction = FALSE;
			m_pThreadAction = NULL;
			AfxMessageBox("Btm2 Picker (Buffer) Position 아닙니다.");
			return;
		}
		if(g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0))
		{
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 1); //z down 
			m_nRepeatCase = 210;
		}
		break;
	case 210:
		if(g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_Btm2PickerClose(m_nPickerNumSelected);
			m_nRepeatCase = 220;
		}
		break;
	case 220:
		if(g_objCommon.Get_Btm2PickerClose(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);
			m_nRepeatCase = 230;
		}
		break;
	case 230:
		if(g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) && g_objCommon.Get_Btm2PickerClose(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 1); // z down
			m_nRepeatCase = 240;
		}
		break;
	case 240:
		if(g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_Btm2PickerOpen(m_nPickerNumSelected); //open
			m_nRepeatCase = 250;
		}
		break;
	case 250:
		if(g_objCommon.Get_Btm2PickerOpen(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);// Z up 
			m_nRepeatCase = 200;
		}
		break;
	// Sort 1
	case 300:
		if(!g_objCommon.Check_Position(AX_SORT_PICKER1_X, 0) && !g_objCommon.Check_Position(AX_SORT_PICKER1_X, 1))
		{
			m_bThreadAction = FALSE;
			m_pThreadAction = NULL;
			AfxMessageBox("Sort 1 Picker (Buffer) Position 아닙니다.");
			return;
		}
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0))
		{
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 1);
			m_nRepeatCase = 310;
		}
		break;
	case 310:
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker1Close(m_nPickerNumSelected);
			m_nRepeatCase = 320;
		}
		break;
	case 320:
		if(g_objCommon.Get_SortPicker1Close(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);
			m_nRepeatCase = 330;
		}
		break;
	case 330:
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Get_SortPicker1Close(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 1); // z down
			m_nRepeatCase = 340;
		}
		break;
	case 340:
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker1Open(m_nPickerNumSelected); //open
			m_nRepeatCase = 350;
		}
		break;
	case 350:
		if(g_objCommon.Get_SortPicker1Open(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);// Z up 
			m_nRepeatCase = 300;
		}
		break;
	// Sort 2
	case 400:
		if(!g_objCommon.Check_Position(AX_SORT_PICKER2_X, 0) && !g_objCommon.Check_Position(AX_SORT_PICKER2_X, 1))
		{
			m_bThreadAction = FALSE;
			m_pThreadAction = NULL;
			AfxMessageBox("Sort 2 Picker (Buffer) Position 아닙니다.");
			return;
		}
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0))
		{
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 1);
			m_nRepeatCase = 410;
		}
		break;
	case 410:
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker2Close(m_nPickerNumSelected);
			m_nRepeatCase = 420;
		}
		break;
	case 420:
		if(g_objCommon.Get_SortPicker2Close(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);
			m_nRepeatCase = 430;
		}
		break;
	case 430:
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Get_SortPicker2Close(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 1); // z down
			m_nRepeatCase = 440;
		}
		break;
	case 440:
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker2Open(m_nPickerNumSelected); //open
			m_nRepeatCase = 450;
		}
		break;
	case 450:
		if(g_objCommon.Get_SortPicker2Open(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);// Z up 
			m_nRepeatCase = 400;
		}
		break;



	}
	
}

void CManualRepeatRunDlg::OnCbnSelchangeCboPicker()
{
	m_cboPickNum.ResetContent();

	if(m_cboPicker.GetCurSel() == 0) // btm1 picker 
	{
		m_nRepeatCase = 100;
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
	else if(m_cboPicker.GetCurSel() == 1) // Btm2 picker
	{
		m_nRepeatCase = 200;
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
	else if(m_cboPicker.GetCurSel() == 2) //sort 1 picker
	{
		m_nRepeatCase = 300;
		for(int i = 1; i < gData.nSortPickQt+1 ; i++)
		{	
			m_strLog.Format("Sort 1 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}
		
	}
	else if(m_cboPicker.GetCurSel() == 3) //sort 2 picker 
	{
		m_nRepeatCase = 400;
		for(int i = 1; i < gData.nSortPickQt+1 ; i++)
		{	
			m_strLog.Format("Sort 2 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}
		
	}
}


void CManualRepeatRunDlg::OnCbnSelchangeCboPickNum()
{
	m_nPickerNumSelected = m_cboPickNum.GetCurSel() + 1;
}


