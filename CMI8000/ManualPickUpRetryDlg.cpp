// ManualPickUpRetryDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMI8000.h"
#include "ManualPickUpRetryDlg.h"
#include "afxdialogex.h"

#include "Common.h"
#include "AJinDefine.h"
#include "AJinAXL.h"
#include "LogFile.h"

#include "CMI8000Dlg.h"
#include "ManualDlg.h"

// CManualPickUpRetryDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualPickUpRetryDlg, CDialogEx)

CManualPickUpRetryDlg::CManualPickUpRetryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualPickUpRetryDlg::IDD, pParent)
{

}

CManualPickUpRetryDlg::~CManualPickUpRetryDlg()
{
}

void CManualPickUpRetryDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(CManualPickUpRetryDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHK_REPEAT_RUN, &CManualPickUpRetryDlg::OnBnClickedChkRepeatRun)
	ON_CBN_SELCHANGE(IDC_CBO_PICKER, &CManualPickUpRetryDlg::OnCbnSelchangeCboPicker)
	ON_CBN_SELCHANGE(IDC_CBO_PICK_NUM, &CManualPickUpRetryDlg::OnCbnSelchangeCboPickNum)
END_MESSAGE_MAP()


// CManualPickUpRetryDlg 메시지 처리기입니다.

BOOL CManualPickUpRetryDlg::OnInitDialog()
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


BOOL CManualPickUpRetryDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualPickUpRetryDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
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


void CManualPickUpRetryDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CManualPickUpRetryDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CManualPickUpRetryDlg::Initial_Controls()
{
	m_Group[0].Init_Ctrl("Arial", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_cboPicker.Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_cboPickNum.Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_edtDelay.Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_edtMsg.Init_Ctrl("Arial", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_chkRepeatRun.Init_Ctrl("Arial", 10, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CManualPickUpRetryDlg::OnBnClickedChkRepeatRun()
{
	CString strText, strTemp;

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
			
	if (m_chkRepeatRun.GetCheck())
	{		
		//if(!CheckMotionPos()) return;

		m_nPickerSelect = m_cboPicker.GetCurSel();
		m_nPickerNum = m_cboPickNum.GetCurSel();
		m_edtDelay.GetWindowText(strText);
		m_nActionDelay = atoi(strText);

		m_bThreadAction = TRUE;
		m_pThreadAction = AfxBeginThread(Thread_ActionRun, this);
	} 
	else
	{
		m_bThreadStop = TRUE;
		m_pThreadStop = AfxBeginThread(Thread_ActionStop, this);

		while(m_bThreadStop) 
		{	
			g_objCommon.DoEvents();
			if(!m_bThreadStop) break;
		}

		m_cboPicker.ResetContent();
		AddComboListPicker();

		m_nRepeatCase = 0;
		m_strTemp.Format("%d", m_nRepeatCase);
		m_lblCase.SetWindowText(m_strTemp);		
		
	/*	if (!m_pThreadAction) return;
		m_bThreadAction = FALSE;
		WaitForSingleObject(m_pThreadAction->m_hThread, INFINITE);*/
	}

	m_cboPicker.EnableWindow(!m_chkRepeatRun.GetCheck());
	m_cboPickNum.EnableWindow(!m_chkRepeatRun.GetCheck());
	m_edtDelay.EnableWindow(!m_chkRepeatRun.GetCheck());

	pMainDlg->Enable_ModeButton(!m_chkRepeatRun.GetCheck());
	pMainDlg->m_btnMainOperator.EnableWindow(!m_chkRepeatRun.GetCheck());

	g_dlgManual.m_rdoManualBtm1.EnableWindow(!m_chkRepeatRun.GetCheck());
	g_dlgManual.m_rdoManualBtm2.EnableWindow(!m_chkRepeatRun.GetCheck());
	g_dlgManual.m_rdoManualLoad.EnableWindow(!m_chkRepeatRun.GetCheck());
	g_dlgManual.m_rdoManualUnload.EnableWindow(!m_chkRepeatRun.GetCheck());
}

void CManualPickUpRetryDlg::AddComboListPicker()
{
	/*m_cboPicker.AddString("Btm1 Picker (Angle Tray)");
	m_cboPicker.AddString("Btm2 Picker (Buffer)");
	m_cboPicker.AddString("Sort1 Picker (Buffer 1)");
	m_cboPicker.AddString("Sort1 Picker (Buffer 2)");
	m_cboPicker.AddString("Sort2 Picker (Buffer 1)");
	m_cboPicker.AddString("Sort2 Picker (Buffer 2)");*/

	m_cboPicker.AddString("Btm1 Picker(Angle Stage 1 or 2)");	
	m_cboPicker.AddString("Btm2 Picker(Inspection Stage)");	
	m_cboPicker.AddString("Sort1 Picker(Buffer 1)");
	m_cboPicker.AddString("Sort1 Picker(Buffer 2)");
	m_cboPicker.AddString("Sort2 Picker(Buffer 1)");
	m_cboPicker.AddString("Sort2 Picker(Buffer 2)");	
	
}

BOOL CManualPickUpRetryDlg::CheckMotionPos()
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

UINT CManualPickUpRetryDlg::Thread_ActionRun(LPVOID lpVoid)
{
	CManualPickUpRetryDlg* pOwner = (CManualPickUpRetryDlg*)lpVoid;

	while (pOwner->m_bThreadAction) {
		pOwner->Repeat_Action();
	} 
	pOwner->m_bThreadAction = FALSE;
	pOwner->m_pThreadAction = NULL;

	return 0;
}




UINT CManualPickUpRetryDlg::Thread_ActionStop(LPVOID lpVoid)
{
	CManualPickUpRetryDlg* pOwner = (CManualPickUpRetryDlg*)lpVoid;

	while (pOwner->m_bThreadStop) {
		pOwner->Repeat_Stop();
	} 
	pOwner->m_bThreadStop = FALSE;
	pOwner->m_pThreadStop = NULL;

	return 0;
}

void CManualPickUpRetryDlg::Repeat_Stop()
{
	if(m_nRepeatCase == 0 ||m_nRepeatCase == 100 || m_nRepeatCase == 200 || m_nRepeatCase == 300 || m_nRepeatCase == 400 || m_nRepeatCase == 500 || m_nRepeatCase == 600)
	{
		m_nRepeatCase = 0;
		m_bThreadAction = FALSE;
		m_pThreadAction = NULL;

		m_bThreadStop = FALSE;
		m_pThreadStop = NULL;	

		
	}		
}


void CManualPickUpRetryDlg::Repeat_Action()
{

	m_strTemp.Format("%d", m_nRepeatCase);
	m_lblCase.SetWindowText(m_strTemp);

	if(m_nRepeatCase == 100 && !g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0))
	{
		m_bThreadAction = FALSE;
		m_pThreadAction = NULL;
		AfxMessageBox("Btm1 Picker Z Ready Up 아닙니다.");

		m_strLog.Format("[Retry - interlock] Btm1 Picker Z Ready Up 아닙니다.");
		g_objLogFile.Save_HandlerLog(m_strLog);
		return;
	}
	if(m_nRepeatCase == 200 && !g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) 
	{
		m_bThreadAction = FALSE;
		m_pThreadAction = NULL;
		AfxMessageBox("Btm2 Picker Z Ready Up 아닙니다.");

		m_strLog.Format("[Retry - interlock] Btm2 Picker Z Ready Up 아닙니다.");
		g_objLogFile.Save_HandlerLog(m_strLog);
		return;
	}
	if((m_nRepeatCase == 300 && !g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0))
		||(m_nRepeatCase == 400 && !g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)))
	{
		m_bThreadAction = FALSE;
		m_pThreadAction = NULL;
		AfxMessageBox("Sort 1 Picker Z Ready Up 아닙니다.");

		m_strLog.Format("[Retry - interlock] Sort 1 Picker Z Ready Up 아닙니다.");
		g_objLogFile.Save_HandlerLog(m_strLog);

		return;
	}
	if((m_nRepeatCase == 500 && !g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0))
		|| (m_nRepeatCase == 600 && !g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)))
	{
		m_bThreadAction = FALSE;
		m_pThreadAction = NULL;
		AfxMessageBox("Sort 2 Picker Z Ready Up 아닙니다.");

		m_strLog.Format("[Retry - interlock] Sort 2 Picker Z Ready Up 아닙니다.");
		g_objLogFile.Save_HandlerLog(m_strLog);
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
			m_strLog.Format("[Retry - interlock] Btm1 Picker (Tray) Position 아닙니다.");
			g_objLogFile.Save_HandlerLog(m_strLog);
			return;
		}
		if(g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_Btm1PickerOpen(m_nPickerNumSelected); //open
			m_nRepeatCase = 110;

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
		}		
		break;
	case 110:		
		if(g_objCommon.Get_Btm1PickerOpen(m_nPickerNumSelected) && g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 1); // Tray down
			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 120;
		}		
		break;
	case 120:
		if(g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_Btm1PickerClose(m_nPickerNumSelected); // close
			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 130;
		}
		break;	
	case 130:
		if(g_objCommon.Get_Btm1PickerClose(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0); // z up 
			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);

			
			m_nRepeatCase = 0;m_chkRepeatRun.SetCheck(FALSE);
			OnBnClickedChkRepeatRun();
			return;
		}
		break;
		/////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////
	case 200:		
		if(!g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0))
		{
			m_bThreadAction = FALSE;
			m_pThreadAction = NULL;
			AfxMessageBox("Btm2 Picker (Inspection Stage) Position 아닙니다.");
			m_strLog.Format("[Retry - interlock] Btm2 Picker (Inspection Stage) Position 아닙니다.");
			g_objLogFile.Save_HandlerLog(m_strLog);
			return;
		}
		if(g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_Btm2PickerOpen(m_nPickerNumSelected); //open

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 210;
		}
		break;
	case 210:
		if(g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) && g_objCommon.Get_Btm2PickerOpen(m_nPickerNumSelected))
		{
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 1); //Inspection down 

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 220;
		}
		break;
	case 220:
		if(g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_Btm2PickerClose(m_nPickerNumSelected);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 230;
		}
		break;
	case 230:
		if(g_objCommon.Get_Btm2PickerClose(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);

			m_nRepeatCase = 0;
			m_chkRepeatRun.SetCheck(FALSE);
			OnBnClickedChkRepeatRun();
			return;
		}
		break;

		/////////////////////////////////////////////////////////////////////////////
		// Sort 1 - buffer 1
	case 300:
		if(!g_objCommon.Check_Position(AX_SORT_PICKER1_X, 0))
		{
			m_bThreadAction = FALSE;
			m_pThreadAction = NULL;
			AfxMessageBox("Sort1 Picker (Buffer 1) Position 아닙니다.");
			m_strLog.Format("[Retry - interlock] Sort1 Picker (Buffer 1) Position 아닙니다.");
			g_objLogFile.Save_HandlerLog(m_strLog);
			return;
		}
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker1Open(m_nPickerNumSelected); //open

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 310;
		}
		break;
	case 310:
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Get_SortPicker1Open(m_nPickerNumSelected))
		{
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 1); // buffer down

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 320;
		}
		break;
	case 320:
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker1Close(m_nPickerNumSelected);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 330;
		}
		break;
	case 330:
		if(g_objCommon.Get_SortPicker1Close(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);

			m_nRepeatCase = 0;
			m_chkRepeatRun.SetCheck(FALSE);
			OnBnClickedChkRepeatRun();
			return;
		}
		break;


		// Sort 1 - buffer 2
	case 400:
		if(!g_objCommon.Check_Position(AX_SORT_PICKER1_X, 1))
		{
			m_bThreadAction = FALSE;
			m_pThreadAction = NULL;
			AfxMessageBox("Sort1 Picker (Buffer 2) Position 아닙니다.");
			m_strLog.Format("[Retry - interlock] Sort1 Picker (Buffer 2) Position 아닙니다.");
			g_objLogFile.Save_HandlerLog(m_strLog);
			return;
		}		
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker1Open(m_nPickerNumSelected); //open

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 410;
		}
		break;
	case 410:
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Get_SortPicker1Open(m_nPickerNumSelected))
		{
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 5); // buffer down

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 420;
		}
		break;
	case 420:
		if(g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 5))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker1Close(m_nPickerNumSelected);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 430;
		}
		break;
	case 430:
		if(g_objCommon.Get_SortPicker1Close(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);

			m_nRepeatCase = 0;
			m_chkRepeatRun.SetCheck(FALSE);
			OnBnClickedChkRepeatRun();
			return;
		}
		break;
		/////////////////////////////////////////////////////////////////////////////
		// Sort 2 - buffer 1
	case 500:
		if(!g_objCommon.Check_Position(AX_SORT_PICKER2_X, 0)) // X buffer 1 
		{
			m_bThreadAction = FALSE;
			m_pThreadAction = NULL;
			AfxMessageBox("Sort 2 Picker (Buffer 1) Position 아닙니다.");
			m_strLog.Format("[Retry - interlock] Sort2 Picker (Buffer 1) Position 아닙니다.");
			g_objLogFile.Save_HandlerLog(m_strLog);
			return;
		}
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker2Open(m_nPickerNumSelected); //open

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 510;
		}
		break;
	case 510:
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Get_SortPicker2Open(m_nPickerNumSelected))
		{
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 1);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 520;
		}
		break;
	case 520:
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 1))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker2Close(m_nPickerNumSelected);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 530;
		}
		break;
	case 530:
		if(g_objCommon.Get_SortPicker2Close(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);

			m_nRepeatCase = 0;
			m_chkRepeatRun.SetCheck(FALSE);
			OnBnClickedChkRepeatRun();
			return;
		}
		break;
		// Sort 1 - buffer 2
	case 600:
		if(!g_objCommon.Check_Position(AX_SORT_PICKER1_X, 1))
		{
			m_bThreadAction = FALSE;
			m_pThreadAction = NULL;
			AfxMessageBox("Sort2 Picker (Buffer 2) Position 아닙니다.");
			return;
		}
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker2Open(m_nPickerNumSelected); //open

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 610;
		}
		break;
	case 610:
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Get_SortPicker2Open(m_nPickerNumSelected))
		{
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 5);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 620;
		}
		break;
	case 620:
		if(g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 5))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Set_SortPicker2Close(m_nPickerNumSelected);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);
			m_nRepeatCase = 630;
		}
		break;
	case 630:
		if(g_objCommon.Get_SortPicker2Close(m_nPickerNumSelected))
		{
			theApp.uSleep(m_nActionDelay);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);

			m_strLog.Format("[Retry] Case : %d", m_nRepeatCase);
			g_objLogFile.Save_HandlerLog(m_strLog);

			m_nRepeatCase = 0;
			m_chkRepeatRun.SetCheck(FALSE);
			OnBnClickedChkRepeatRun();
			return;
		}
		break;	
	}
	
}

void CManualPickUpRetryDlg::OnCbnSelchangeCboPicker()
{
	m_cboPickNum.ResetContent();

	if(m_cboPicker.GetCurSel() == 0) // btm1 picker angle stage 
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
	else if(m_cboPicker.GetCurSel() == 1) // Btm2 picker inspection stage  
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
	else if(m_cboPicker.GetCurSel() == 2) //sort 1 picker buffer 1
	{
		m_nRepeatCase = 300;
		for(int i = 1; i < gData.nSortPickQt+1 ; i++)
		{	
			m_strLog.Format("Sort1 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}		
	}
	else if(m_cboPicker.GetCurSel() == 3) //sort 1 picker buffer 2
	{
		m_nRepeatCase = 400;
		for(int i = 1; i < gData.nSortPickQt+1 ; i++)
		{	
			m_strLog.Format("Sort1 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}		
	}
	else if(m_cboPicker.GetCurSel() == 4) //sort 2 picker buffer 1
	{
		m_nRepeatCase = 500;
		for(int i = 1; i < gData.nSortPickQt+1 ; i++)
		{	
			m_strLog.Format("Sort2 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}	
	}
	else if(m_cboPicker.GetCurSel() == 5) //sort 2 picker buffer 2
	{
		m_nRepeatCase = 600;
		for(int i = 1; i < gData.nSortPickQt+1 ; i++)
		{	
			m_strLog.Format("Sort2 CM No.: %d", i);
			m_cboPickNum.AddString(m_strLog);
		}	
	}
	
}


void CManualPickUpRetryDlg::OnCbnSelchangeCboPickNum()
{
	m_nPickerNumSelected = m_cboPickNum.GetCurSel() + 1;
}


