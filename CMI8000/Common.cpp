// Common.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "Common.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"

#include "ErrorDlg.h"
#include "AlarmDlg.h"
#include "MsgBoxDlg.h"
#include "NumPadDlg.h"
#include "KeyPadDlg.h"
#include "PasswordDlg.h"

#include "SequenceMain.h"

#include <iostream>
#include <string>
#include <fstream>
#include "Wininet.h"

#include <wbemidl.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")

// CCommon
CCommon g_objCommon;

IMPLEMENT_DYNAMIC(CCommon, CWnd)

CCriticalSection g_csShowError;
CCriticalSection g_csShowAlarm;

CCommon::CCommon()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	m_nFreq = freq.QuadPart;

	g_initDone = 0;
	g_lock = 0;
}

CCommon::~CCommon()
{
}

BEGIN_MESSAGE_MAP(CCommon, CWnd)
END_MESSAGE_MAP()

// CCommon 메시지 처리기입니다.

void CCommon::DoEvents()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void CCommon::uSleep(int msec)
{
	LARGE_INTEGER tStart, tNow;
	LONGLONG lTerm = 0;
	QueryPerformanceCounter(&tStart);

	while (TRUE) {
		QueryPerformanceCounter(&tNow);
		lTerm = (tNow.QuadPart - tStart.QuadPart) * 1000 / m_nFreq;
		if (lTerm > msec) break;
		DoEvents();
	}
}

void CCommon::MakeFolder(CString sPath)	//"D:\\Run\\Log\\LotData\\YYYY\\MM\\DD\\";
{
	CString sTemp;
	int nIdx = 0;

	while(true) {
		nIdx = sPath.Find("\\");
		sTemp += sPath.Mid(0, nIdx) + "\\";
		CreateDirectory(sTemp,FALSE);
		sPath = sPath.Mid(nIdx+1, sPath.GetLength());
		if(nIdx<0) break;
	}
}

void CCommon::Save_MotionPos()
{
	int nCount = 0;
	for(int i=0; i<AXIS_COUNT; i++) {
		if(g_objAJinAXL.Is_Done(i)) { gAlm.dMotionPos[i] = g_objAJinAXL.Get_Position(i); }
		else						{ gAlm.dMotionPos[i] = -100.0; nCount++; }
	}

	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	gAlm.bBufferUpStatus[0] = pDX09->iBufferStage1Up;
	gAlm.bBufferUpStatus[1] = pDX09->iBufferStage2Up;
	gAlm.bBufferDownStatus[0] = pDX09->iBufferStage1Down;
	gAlm.bBufferDownStatus[1] = pDX09->iBufferStage2Down;
	
	if (nCount == 0) return;

	uSleep(1000);
	nCount = 0;
	for(int i=0; i<AXIS_COUNT; i++) {
		if (gAlm.dMotionPos[i] < -10.0) {
			if(g_objAJinAXL.Is_Done(i)) gAlm.dMotionPos[i] = g_objAJinAXL.Get_Position(i);
			else nCount++;
		}
	}
	if (nCount == 0) return;

	uSleep(3000);
	for(int i=0; i<AXIS_COUNT; i++) {
		if (gAlm.dMotionPos[i] < -10.0) {
			if(g_objAJinAXL.Is_Done(i)) gAlm.dMotionPos[i] = g_objAJinAXL.Get_Position(i);
		}
	}
}

int CCommon::Check_MotionPos()
{
	int		nMotionNo = 99;
	double	dCurrentPos, dCheckPos;
	double	dRange = gAlm.dMotionChkPos;
	if (dRange < 0.05) return nMotionNo;

	

	for(int i=0; i<AXIS_COUNT; i++) {
		if (i == 44) continue;	// Empty Port Z Axis
		if (gAlm.dMotionPos[i] < -10.0) continue;
		if (!g_objAJinAXL.Is_Done(i)) continue;

		dCheckPos   = gAlm.dMotionPos[i];
		dCurrentPos = g_objAJinAXL.Get_Position(i);
		if (fabs(dCurrentPos - dCheckPos) > dRange) return i;
	}
	return nMotionNo;
}

ULONGLONG CCommon::GetTickCount64Compat()
{
	static DWORD      s_last32 = 0;
	static ULONGLONG  s_high64 = 0;
	static LONG       s_inited = 0;

	DWORD cur32 = ::GetTickCount();

	if (InterlockedCompareExchange(&s_inited, 1, 0) == 0)
	{
		s_last32 = cur32;
		return (s_high64 + (ULONGLONG)cur32);
	}

	Lock();

	if (cur32 < s_last32)
		s_high64 += (1ULL << 32);

	s_last32 = cur32;
	ULONGLONG result = s_high64 + (ULONGLONG)cur32;

	Unlock();
	return result;
}

BOOL CCommon::Check_Position(int nAxis, int nMoveIdx, double dRange)
{
	double dCurrentPos = g_objAJinAXL.Get_Position(nAxis);
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	double dCheckPos = -1.0;

	if (g_objAJinAXL.Is_Done(nAxis)) {
		if (nAxis == AX_LOAD_STAGE_X)		dCheckPos = pMoveData->dLoadStageX[nMoveIdx];		// AXIS 00
		if (nAxis == AX_LOAD_STAGE_Z)		dCheckPos = pMoveData->dLoadStageZ[nMoveIdx];		// AXIS 01
		if (nAxis == AX_LOAD_PICKER_Y)		dCheckPos = pMoveData->dLoadPickerY[nMoveIdx];		// AXIS 02
		if (nAxis == AX_ANGLE_UNIT_Y)		dCheckPos = pMoveData->dAngleUnitY[nMoveIdx];		// AXIS 03
		if (nAxis == AX_ANGLE_UNIT_X)		dCheckPos = pMoveData->dAngleUnitX[nMoveIdx];		// AXIS 04
		if (nAxis == AX_ANGLE_UNIT_R)		dCheckPos = pMoveData->dAngleUnitR[nMoveIdx];		// AXIS 05
		if (nAxis == AX_VISION_ANGLE_Z)		dCheckPos = pMoveData->dVisionAngleZ[nMoveIdx];		// AXIS 06
		if (nAxis == AX_VISION_ALIGN_Z)		dCheckPos = pMoveData->dVisonAlignZ[nMoveIdx];		// AXIS 07
		if (nAxis == AX_ANGLE_STAGE1_Y)		dCheckPos = pMoveData->dAngleStage1Y[nMoveIdx];		// AXIS 08
		if (nAxis == AX_ANGLE_STAGE1_Z)		dCheckPos = pMoveData->dAngleStage1Z[nMoveIdx];		// AXIS 09
		if (nAxis == AX_ANGLE_STAGE2_Y)		dCheckPos = pMoveData->dAngleStage2Y[nMoveIdx];		// AXIS 10
		if (nAxis == AX_ANGLE_STAGE2_Z)		dCheckPos = pMoveData->dAngleStage2Z[nMoveIdx];		// AXIS 11
		if (nAxis == AX_BTM1_PICKER_X)		dCheckPos = pMoveData->dBtm1PickerX[nMoveIdx];		// AXIS 12
		if (nAxis == AX_BTM1_PICKER_Z)		dCheckPos = pMoveData->dBtm1PickerZ[nMoveIdx];		// AXIS 13
		if (nAxis == AX_BTM1_PICKER_P1)		dCheckPos = pMoveData->dBtm1PickerP1[nMoveIdx];		// AXIS 14
		if (nAxis == AX_BTM1_PICKER_P2)		dCheckPos = pMoveData->dBtm1PickerP2[nMoveIdx];		// AXIS 15
		if (nAxis == AX_BTM1_SHIFT_Y)		dCheckPos = pMoveData->dBtm1ShiftY[nMoveIdx];		// AXIS 16

		if (nAxis == AX_INSPECT_STAGE1_X)	dCheckPos = pMoveData->dInspectStage1X[nMoveIdx];	// AXIS 17
		if (nAxis == AX_INSPECT_STAGE2_X)	dCheckPos = pMoveData->dInspectStage2X[nMoveIdx];	// AXIS 18
		if (nAxis == AX_INSPECT_STAGE3_X)	dCheckPos = pMoveData->dInspectStage3X[nMoveIdx];	// AXIS 19
		if (nAxis == AX_MODULE_ALIGN_Y)		dCheckPos = pMoveData->dModuleAlignY[nMoveIdx];		// AXIS 20
		if (nAxis == AX_MODULE_ALIGN_Z)		dCheckPos = pMoveData->dModuleAlignZ[nMoveIdx];		// AXIS 21
		if (nAxis == AX_VISION_TOP1_Z)		dCheckPos = pMoveData->dVisionTop1Z[nMoveIdx];		// AXIS 22
		if (nAxis == AX_TOP1_MIRROR_Z)		dCheckPos = pMoveData->dTop1MirrorZ[nMoveIdx];		// AXIS 23
		if (nAxis == AX_TOP2_SHIFT_Y)		dCheckPos = pMoveData->dTop2ShiftY[nMoveIdx];		// AXIS 24
		if (nAxis == AX_VISION_TOP2_Z)		dCheckPos = pMoveData->dVisionTop2Z[nMoveIdx];		// AXIS 25
		if (nAxis == AX_BTM2_PICKER_X)		dCheckPos = pMoveData->dBtm2PickerX[nMoveIdx];		// AXIS 26
		if (nAxis == AX_BTM2_PICKER_Z)		dCheckPos = pMoveData->dBtm2PickerZ[nMoveIdx];		// AXIS 27
		if (nAxis == AX_BTM2_PICKER_P1)		dCheckPos = pMoveData->dBtm2PickerP1[nMoveIdx];		// AXIS 28
		if (nAxis == AX_BTM2_PICKER_P2)		dCheckPos = pMoveData->dBtm2PickerP2[nMoveIdx];		// AXIS 29
		if (nAxis == AX_BUFFER_STAGE1_Y)	dCheckPos = pMoveData->dBufferStage1Y[nMoveIdx];	// AXIS 30
		if (nAxis == AX_BUFFER_STAGE2_Y)	dCheckPos = pMoveData->dBufferStage2Y[nMoveIdx];	// AXIS 31
		if (nAxis == AX_SORT_PICKER1_X)		dCheckPos = pMoveData->dSortPicker1X[nMoveIdx];		// AXIS 32
		if (nAxis == AX_SORT_PICKER1_Z)		dCheckPos = pMoveData->dSortPicker1Z[nMoveIdx];		// AXIS 33
		if (nAxis == AX_SORT_PICKER1_P)		dCheckPos = pMoveData->dSortPicker1P[nMoveIdx];		// AXIS 34
		if (nAxis == AX_SORT_PICKER2_X)		dCheckPos = pMoveData->dSortPicker2X[nMoveIdx];		// AXIS 35
		if (nAxis == AX_SORT_PICKER2_Z)		dCheckPos = pMoveData->dSortPicker2Z[nMoveIdx];		// AXIS 36
		if (nAxis == AX_SORT_PICKER2_P)		dCheckPos = pMoveData->dSortPicker2P[nMoveIdx];		// AXIS 37
		if (nAxis == AX_GOOD_STAGE1_Y)		dCheckPos = pMoveData->dGoodStage1Y[nMoveIdx];		// AXIS 38
		if (nAxis == AX_GOOD_STAGE1_Z)		dCheckPos = pMoveData->dGoodStage1Z[nMoveIdx];		// AXIS 39
		if (nAxis == AX_GOOD_STAGE2_Y)		dCheckPos = pMoveData->dGoodStage2Y[nMoveIdx];		// AXIS 40
		if (nAxis == AX_GOOD_STAGE2_Z)		dCheckPos = pMoveData->dGoodStage2Z[nMoveIdx];		// AXIS 41
		if (nAxis == AX_NG_STAGE_Y)			dCheckPos = pMoveData->dNgStageY[nMoveIdx];			// AXIS 42
		if (nAxis == AX_EMPTY_TRANS1_X)		dCheckPos = pMoveData->dEmptyTrans1X[nMoveIdx];		// AXIS 43
		if (nAxis == AX_EMPTY_PORT_Z)		dCheckPos = pMoveData->dEmptyPortZ[nMoveIdx];		// AXIS 44
		if (nAxis == AX_EMPTY_TRANS2_Y)		dCheckPos = pMoveData->dEmptyTrans2Y[nMoveIdx];		// AXIS 45

		if (fabs(dCurrentPos - dCheckPos) < dRange) return TRUE;

	} else return FALSE;

	return FALSE;
}

void CCommon::Move_Position(int nAxis, int nMoveIdx)
{
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	double dMovePos = -999.0;

	if (nAxis == AX_LOAD_STAGE_X)		dMovePos = pMoveData->dLoadStageX[nMoveIdx];		// AXIS 00
	if (nAxis == AX_LOAD_STAGE_Z)		dMovePos = pMoveData->dLoadStageZ[nMoveIdx];		// AXIS 01
	if (nAxis == AX_LOAD_PICKER_Y)		dMovePos = pMoveData->dLoadPickerY[nMoveIdx];		// AXIS 02
	if (nAxis == AX_ANGLE_UNIT_Y)		dMovePos = pMoveData->dAngleUnitY[nMoveIdx];		// AXIS 03
	if (nAxis == AX_ANGLE_UNIT_X)		dMovePos = pMoveData->dAngleUnitX[nMoveIdx];		// AXIS 04
	if (nAxis == AX_ANGLE_UNIT_R)		dMovePos = pMoveData->dAngleUnitR[nMoveIdx];		// AXIS 05
	if (nAxis == AX_VISION_ANGLE_Z)		dMovePos = pMoveData->dVisionAngleZ[nMoveIdx];		// AXIS 06
	if (nAxis == AX_VISION_ALIGN_Z)		dMovePos = pMoveData->dVisonAlignZ[nMoveIdx];		// AXIS 07
	if (nAxis == AX_ANGLE_STAGE1_Y)		dMovePos = pMoveData->dAngleStage1Y[nMoveIdx];		// AXIS 08
	if (nAxis == AX_ANGLE_STAGE1_Z)		dMovePos = pMoveData->dAngleStage1Z[nMoveIdx];		// AXIS 09
	if (nAxis == AX_ANGLE_STAGE2_Y)		dMovePos = pMoveData->dAngleStage2Y[nMoveIdx];		// AXIS 10
	if (nAxis == AX_ANGLE_STAGE2_Z)		dMovePos = pMoveData->dAngleStage2Z[nMoveIdx];		// AXIS 11
	if (nAxis == AX_BTM1_PICKER_X)		dMovePos = pMoveData->dBtm1PickerX[nMoveIdx];		// AXIS 12
	if (nAxis == AX_BTM1_PICKER_Z)		dMovePos = pMoveData->dBtm1PickerZ[nMoveIdx];		// AXIS 13
	if (nAxis == AX_BTM1_PICKER_P1)		dMovePos = pMoveData->dBtm1PickerP1[nMoveIdx];		// AXIS 14
	if (nAxis == AX_BTM1_PICKER_P2)		dMovePos = pMoveData->dBtm1PickerP2[nMoveIdx];		// AXIS 15
	if (nAxis == AX_BTM1_SHIFT_Y)		dMovePos = pMoveData->dBtm1ShiftY[nMoveIdx];		// AXIS 16
	if (nAxis == AX_INSPECT_STAGE1_X)	dMovePos = pMoveData->dInspectStage1X[nMoveIdx];	// AXIS 17
	if (nAxis == AX_INSPECT_STAGE2_X)	dMovePos = pMoveData->dInspectStage2X[nMoveIdx];	// AXIS 18
	if (nAxis == AX_INSPECT_STAGE3_X)	dMovePos = pMoveData->dInspectStage3X[nMoveIdx];	// AXIS 19
	if (nAxis == AX_MODULE_ALIGN_Y)		dMovePos = pMoveData->dModuleAlignY[nMoveIdx];		// AXIS 20
	if (nAxis == AX_MODULE_ALIGN_Z)		dMovePos = pMoveData->dModuleAlignZ[nMoveIdx];		// AXIS 21
	if (nAxis == AX_VISION_TOP1_Z)		dMovePos = pMoveData->dVisionTop1Z[nMoveIdx];		// AXIS 22
	if (nAxis == AX_TOP1_MIRROR_Z)		dMovePos = pMoveData->dTop1MirrorZ[nMoveIdx];		// AXIS 23
	if (nAxis == AX_TOP2_SHIFT_Y)		dMovePos = pMoveData->dTop2ShiftY[nMoveIdx];		// AXIS 24
	if (nAxis == AX_VISION_TOP2_Z)		dMovePos = pMoveData->dVisionTop2Z[nMoveIdx];		// AXIS 25
	if (nAxis == AX_BTM2_PICKER_X)		dMovePos = pMoveData->dBtm2PickerX[nMoveIdx];		// AXIS 26
	if (nAxis == AX_BTM2_PICKER_Z)		dMovePos = pMoveData->dBtm2PickerZ[nMoveIdx];		// AXIS 27
	if (nAxis == AX_BTM2_PICKER_P1)		dMovePos = pMoveData->dBtm2PickerP1[nMoveIdx];		// AXIS 28
	if (nAxis == AX_BTM2_PICKER_P2)		dMovePos = pMoveData->dBtm2PickerP2[nMoveIdx];		// AXIS 29
	if (nAxis == AX_BUFFER_STAGE1_Y)	dMovePos = pMoveData->dBufferStage1Y[nMoveIdx];		// AXIS 30
	if (nAxis == AX_BUFFER_STAGE2_Y)	dMovePos = pMoveData->dBufferStage2Y[nMoveIdx];		// AXIS 31
	if (nAxis == AX_SORT_PICKER1_X)		dMovePos = pMoveData->dSortPicker1X[nMoveIdx];		// AXIS 32
	if (nAxis == AX_SORT_PICKER1_Z)		dMovePos = pMoveData->dSortPicker1Z[nMoveIdx];		// AXIS 33
	if (nAxis == AX_SORT_PICKER1_P)		dMovePos = pMoveData->dSortPicker1P[nMoveIdx];		// AXIS 34
	if (nAxis == AX_SORT_PICKER2_X)		dMovePos = pMoveData->dSortPicker2X[nMoveIdx];		// AXIS 35
	if (nAxis == AX_SORT_PICKER2_Z)		dMovePos = pMoveData->dSortPicker2Z[nMoveIdx];		// AXIS 36
	if (nAxis == AX_SORT_PICKER2_P)		dMovePos = pMoveData->dSortPicker2P[nMoveIdx];		// AXIS 37
	if (nAxis == AX_GOOD_STAGE1_Y)		dMovePos = pMoveData->dGoodStage1Y[nMoveIdx];		// AXIS 38
	if (nAxis == AX_GOOD_STAGE1_Z)		dMovePos = pMoveData->dGoodStage1Z[nMoveIdx];		// AXIS 39
	if (nAxis == AX_GOOD_STAGE2_Y)		dMovePos = pMoveData->dGoodStage2Y[nMoveIdx];		// AXIS 40
	if (nAxis == AX_GOOD_STAGE2_Z)		dMovePos = pMoveData->dGoodStage2Z[nMoveIdx];		// AXIS 41
	if (nAxis == AX_NG_STAGE_Y)			dMovePos = pMoveData->dNgStageY[nMoveIdx];			// AXIS 42
	if (nAxis == AX_EMPTY_TRANS1_X)		dMovePos = pMoveData->dEmptyTrans1X[nMoveIdx];		// AXIS 43
	if (nAxis == AX_EMPTY_PORT_Z)		dMovePos = pMoveData->dEmptyPortZ[nMoveIdx];		// AXIS 44
	if (nAxis == AX_EMPTY_TRANS2_Y)		dMovePos = pMoveData->dEmptyTrans2Y[nMoveIdx];		// AXIS 45

	if (dMovePos == -999) return;
	g_objAJinAXL.Move_Absolute(nAxis, dMovePos);
}

void CCommon::Show_Error(int nErrNo)
{
	g_csShowError.Lock();
	if (!g_dlgError.IsWindowVisible() && !g_dlgAlarm.IsWindowVisible()) {
		g_dlgError.Set_ErrorNumber(nErrNo);
		g_dlgError.ShowWindow(SW_SHOW);
	}
	g_csShowError.Unlock();
}

void CCommon::Set_ErrorSubMessage(CString sErrSubMsg)
{
	g_dlgError.Set_ErrorSubMessage(sErrSubMsg);
}

void CCommon::Show_Alarm(CString sMsg, BOOL bOn)
{
	g_csShowAlarm.Lock();
	if (bOn) {
		if (!g_dlgError.IsWindowVisible() && !g_dlgAlarm.IsWindowVisible()) {
			g_dlgAlarm.Set_AlarmMessage(sMsg);
			g_dlgAlarm.ShowWindow(SW_SHOW);
		}
	} else {
		if (!g_dlgError.IsWindowVisible() && g_dlgAlarm.IsWindowVisible()) {
			g_dlgAlarm.ShowWindow(SW_HIDE);
		}
	}
	g_csShowAlarm.Unlock();
}

int CCommon::Show_MsgBox(int nBtnCnt, CString sMsg)
{
	CMsgBoxDlg dlgMsgBox;
	dlgMsgBox.Set_Message(nBtnCnt, sMsg);
	return dlgMsgBox.DoModal();
}

int CCommon::Show_NumPad(CString sOld, CString &sNew, int nAxis)
{
	CNumPadDlg dlgNumPad;
	dlgNumPad.Set_OldValue(sOld, nAxis);
	if (dlgNumPad.DoModal() != IDOK) return IDCANCEL;

	sNew = g_strReturnNumPad;
	return IDOK;
}

int CCommon::Show_KeyPad(CString &sKey)
{
	CKeyPadDlg dlgKeyPad;
	if (dlgKeyPad.DoModal() != IDOK) return IDCANCEL;

	sKey = g_strReturnKeyPad;
	return IDOK;
}

int CCommon::Show_Password(int &nUser)
{
	CPasswordDlg dlgPassword;
	if (dlgPassword.DoModal() != IDOK) return IDCANCEL;

	nUser = g_nLoginUser;
	return IDOK;
}

BOOL CCommon::Check_MainEmgAir()
{
#ifdef AJIN_BOARD_USE
	DX_DATA_14 *pDX14 = g_objAJinAXL.Get_pDX14();
	if (pDX14->iEmgSw1)		{ Show_Error(1); return FALSE; }
	if (pDX14->iEmgSw2)		{ Show_Error(2); return FALSE; }
	if (pDX14->iEmgSw3)		{ Show_Error(3); return FALSE; }
	if (pDX14->iEmgSw4)		{ Show_Error(4); return FALSE; }
	if (pDX14->iEmgSw5)		{ Show_Error(5); return FALSE; }
	if (pDX14->iEmgSw6)		{ Show_Error(6); return FALSE; }
// 	if (!pDX14->iMainAir1)	{ Show_Error(7); return FALSE; }
// 	if (!pDX14->iMainAir2)	{ Show_Error(8); return FALSE; }
// 	if (!pDX14->iMainAir3)	{ Show_Error(9); return FALSE; }
#endif
	return TRUE;
}

BOOL CCommon::Check_MainDoor(BOOL bAuto)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseDoorLock) return TRUE;

#ifdef AJIN_BOARD_USE
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
	BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);


// 	if ((!bAuto || !bLoad) && pDX15->iDoor04Unlock) { Show_Alarm("Main 04번 Door(X1019) Unlocked."); return FALSE; }

	if (pDX15->iDoor01Unlock) { /*Show_Alarm("Main 01번 Door(X1500) Unlocked.");*/ Show_Error(11); return FALSE; }
	if (pDX15->iDoor02Unlock && !bLoad1) { /*Show_Alarm("Main 02번 Door(X1501) Unlocked.");*/ Show_Error(12); return FALSE; }
	if (pDX15->iDoor03Unlock && !bLoad2) { /*Show_Alarm("Main 03번 Door(X1502) Unlocked.");*/ Show_Error(13); return FALSE; }
	if (pDX15->iDoor04Unlock && !gData.bNGTrayWait && !gData.bNgTrayEnd) { /*Show_Alarm("Main 04번 Door(X1503) Unlocked.");*/ Show_Error(14); return FALSE; }
	if (pDX15->iDoor05Unlock && !gData.bGoodTrayWait) { /*Show_Alarm("Main 05번 Door(X1504) Unlocked.");*/ Show_Error(15); return FALSE; }
	if (pDX15->iDoor06Unlock) { /*Show_Alarm("Main 06번 Door(X1505) Unlocked.");*/ Show_Error(16); return FALSE; }
	if (pDX15->iDoor07Unlock) { /*Show_Alarm("Main 07번 Door(X1506) Unlocked.");*/ Show_Error(17); return FALSE; }
	if (pDX15->iDoor08Unlock) { /*Show_Alarm("Main 08번 Door(X1507) Unlocked.");*/ Show_Error(18); return FALSE; }
	if (pDX15->iDoor09Unlock) { /*Show_Alarm("Main 09번 Door(X1508) Unlocked.");*/ Show_Error(19); return FALSE; }
	if (pDX15->iDoor10Unlock && !gData.bEmptyZWait) { /*Show_Alarm("Main 10번 Door(X1509) Unlocked.");*/ Show_Error(20); return FALSE; }
	if (pDX15->iDoor11Unlock) { /*Show_Alarm("Main 11번 Door(X1510) Unlocked.");*/ Show_Error(21); return FALSE; }
	if (pDX15->iDoor12Unlock) { /*Show_Alarm("Main 12번 Door(X1511) Unlocked.");*/ Show_Error(22); return FALSE; }
	if (pDX15->iDoor13Unlock) { /*Show_Alarm("Main 13번 Door(X1512) Unlocked.");*/ Show_Error(23); return FALSE; }
	if (pDX15->iDoor14Unlock) { /*Show_Alarm("Main 14번 Door(X1513) Unlocked.");*/ Show_Error(24); return FALSE; }
	if (pDX15->iDoor15Unlock) { /*Show_Alarm("Main 15번 Door(X1514) Unlocked.");*/ Show_Error(25); return FALSE; }
	if (pDX15->iDoor16Unlock) { /*Show_Alarm("Main 16번 Door(X1515) Unlocked.");*/ Show_Error(26); return FALSE; }
	if (pDX15->iDoor17Unlock) { /*Show_Alarm("Main 17번 Door(X1516) Unlocked.");*/ Show_Error(27); return FALSE; }
	if (pDX15->iDoor18Unlock) { /*Show_Alarm("Main 18번 Door(X1517) Unlocked.");*/ Show_Error(28); return FALSE; }
	if (pDX15->iDoor19Unlock) { /*Show_Alarm("Main 19번 Door(X1518) Unlocked.");*/ Show_Error(29); return FALSE; }
	if (pDX15->iDoor20Unlock) { /*Show_Alarm("Main 19번 Door(X1518) Unlocked.");*/ Show_Error(30); return FALSE; }
#endif
	return TRUE;
}

CString CCommon::Get_StateOfDoors()
{
#ifdef DRY_RUN_TEST
	return "";
#endif
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	
	CString strStates;
	CString strDoors[20];


	if (0) { strDoors[0]="Open"; } else { strDoors[0]="Close";}
	if (1){ strDoors[1]="Open"; } else { strDoors[1]="Close";}
	if (1) { strDoors[2]="Open"; } else { strDoors[2]="Close";}
	if (1) { strDoors[3]="Open"; } else { strDoors[3]="Close";}
	if (0){ strDoors[4]="Open"; } else { strDoors[4]="Close";}
	if (1){ strDoors[5]="Open"; } else { strDoors[5]="Close";}
	if (1) { strDoors[6]="Open"; } else { strDoors[6]="Close";}
	if (1) { strDoors[7]="Open"; } else { strDoors[7]="Close";}
	if (0) { strDoors[8]="Open"; } else { strDoors[8]="Close";}
	if (0) { strDoors[9]="Open"; } else { strDoors[9]="Close";}
	if (0) { strDoors[10]="Open"; } else { strDoors[10]="Close";}
	if (0) { strDoors[11]="Open"; } else { strDoors[11]="Close";}
	if (0) { strDoors[12]="Open"; } else { strDoors[12]="Close";}
	if (0) { strDoors[13]="Open"; } else { strDoors[13]="Close";}
	if (0){ strDoors[14]="Open"; } else { strDoors[14]="Close";}
	if (0){ strDoors[15]="Open"; } else { strDoors[15]="Close";}
	if (0) { strDoors[16]="Open"; } else { strDoors[16]="Close";}
	if (0){ strDoors[17]="Open"; } else { strDoors[17]="Close";}
	if (0) { strDoors[18]="Open"; } else { strDoors[18]="Close";}
	if (0) { strDoors[19]="Open"; } else { strDoors[19]="Close";}


#ifdef AJIN_BOARD_USE
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
		
	if (pDX15->iDoor01Unlock) { strDoors[0]="Open"; } else { strDoors[0]="Close";}
	if (pDX15->iDoor02Unlock ){ strDoors[1]="Open"; } else { strDoors[1]="Close";}
	if (pDX15->iDoor03Unlock ) { strDoors[2]="Open"; } else { strDoors[2]="Close";}
	if (pDX15->iDoor04Unlock ) { strDoors[3]="Open"; } else { strDoors[3]="Close";}
	if (pDX15->iDoor05Unlock ){ strDoors[4]="Open"; } else { strDoors[4]="Close";}
	if (pDX15->iDoor06Unlock){ strDoors[5]="Open"; } else { strDoors[5]="Close";}
	if (pDX15->iDoor07Unlock) { strDoors[6]="Open"; } else { strDoors[6]="Close";}
	if (pDX15->iDoor08Unlock) { strDoors[7]="Open"; } else { strDoors[7]="Close";}
	if (pDX15->iDoor09Unlock) { strDoors[8]="Open"; } else { strDoors[8]="Close";}
	if (pDX15->iDoor10Unlock) { strDoors[9]="Open"; } else { strDoors[9]="Close";}
	if (pDX15->iDoor11Unlock) { strDoors[10]="Open"; } else { strDoors[10]="Close";}
	if (pDX15->iDoor12Unlock) { strDoors[11]="Open"; } else { strDoors[11]="Close";}
	if (pDX15->iDoor13Unlock) { strDoors[12]="Open"; } else { strDoors[12]="Close";}
	if (pDX15->iDoor14Unlock) { strDoors[13]="Open"; } else { strDoors[13]="Close";}
	if (pDX15->iDoor15Unlock){ strDoors[14]="Open"; } else { strDoors[14]="Close";}
	if (pDX15->iDoor16Unlock){ strDoors[15]="Open"; } else { strDoors[15]="Close";}
	if (pDX15->iDoor17Unlock) { strDoors[16]="Open"; } else { strDoors[16]="Close";}
	if (pDX15->iDoor18Unlock){ strDoors[17]="Open"; } else { strDoors[17]="Close";}
	if (pDX15->iDoor19Unlock) { strDoors[18]="Open"; } else { strDoors[18]="Close";}
	if (pDX15->iDoor20Unlock) { strDoors[19]="Open"; } else { strDoors[19]="Close";}

	
#endif
	strStates.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", 
		strDoors[0],strDoors[1],strDoors[2],strDoors[3],strDoors[4],strDoors[5],strDoors[6],strDoors[7],strDoors[8],strDoors[9],
		strDoors[10],strDoors[11],strDoors[12],strDoors[13],strDoors[14],strDoors[15],strDoors[16],strDoors[17],strDoors[19],strDoors[19]);

	return strStates;
}


BOOL CCommon::Check_PortArea(BOOL bAuto)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseDoorLock) return TRUE;
	if (!bAuto)	return TRUE;

#ifdef AJIN_BOARD_USE
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	DX_DATA_09 *pDX09 = g_objAJinAXL.Get_pDX09();
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();
	BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
	BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);

	// Load Port1
	if (!pDX00->iLoadPortAreaCheck && (pDX00->iLoadPort1SlideOpen || pDX00->iLoadPort1SlideClose)) {
		//Show_Alarm("Load Port Area(X0006) 감지 되었습니다."); return FALSE;
	}

	// Load Port2
	if (!pDX00->iLoadPortAreaCheck && (pDX00->iLoadPort2SlideOpen || pDX00->iLoadPort2SlideClose)) {
		//Show_Alarm("Load Port Area(X0006) 감지 되었습니다."); return FALSE;
	}

	// Empty Port
#ifdef EDITION_2ND
	if (!pDX01->iEmptyPortAreaCheck	&& (pDX01->iEmptyPortSlideOpen || pDX01->iEmptyPortSlideClose)) {
		//Show_Alarm("Empty Port Area(X0728) 감지 되었습니다."); return FALSE;
	}
#else
	if (!pDX07->iEmptyPortAreaCheck	&& (pDX07->iEmptyPortSlideOpen || pDX07->iEmptyPortSlideClose)) {
		Show_Alarm("Empty Port Area(X0728) 감지 되었습니다."); return FALSE;
	}
#endif

	// Ng Port
	if (!pDX09->iNgPortAreaCheck && (pDX09->iNgPortSlideOpen || pDX09->iNgPortSlideClose)) 
	{
		Show_MsgBox(1, "Ng Port Area(X0930) 감지 되었습니다.");
		//Show_Alarm("Ng Port Area(X0930) 감지 되었습니다."); return FALSE;
	}

	//Good Port
	if (!pDX12->iGoodPortAreaCheck && (pDX12->iGoodPortSlideOpen || pDX12->iGoodPortSlideClose)) {
		//Show_MsgBox(1, "Good Port Area(X1206) 감지 되었습니다.");
		//Show_Alarm("Good Port Area(X1206) 감지 되었습니다."); return FALSE;
	}
#endif
	return TRUE;
}

BOOL CCommon::Check_PortFull()
{
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (pDX12->iGoodPortUpper && !pEquipData->bUseInlineMode) {	Show_Alarm("Good Port Upper Sensor(X1200) 감지 되었습니다."); return FALSE;}

	return TRUE;
}

BOOL CCommon::Check_TrayFull()
{
#ifdef AJIN_BOARD_USE
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();

// 	if (!pDX00->iLoadPort2FullChk)	{ Show_Alarm("Load Port2 Full Sensor Checked."); return FALSE; }
// 	if (!pDX08->iGoodPortFullChk)	{ Show_Alarm("Good Port Full Sensor Checked."); return FALSE; }
// 	if (!pDX08->iNNgPortFullChk)	{ Show_Alarm("Normal NG Port Full Sensor Checked."); return FALSE; }
// 	if (!pDX08->iSNgPortFullChk)	{ Show_Alarm("Special NG Port Full Sensor Checked."); return FALSE; }
#endif
	return TRUE;
}

// Servo On Error : Error (100 - 145)
BOOL CCommon::Check_ServoOn()
{
#ifdef AJIN_BOARD_USE
	for (int i = 0; i < AXIS_COUNT; i++) {
		if(i==16)continue;
		if (!g_objAJinAXL.Get_ServoOn(i)) {  // 16번축 btm shift 제거됨
			Show_Error(i + 100); return FALSE;
		}
	}
#endif
	return TRUE;
}

// Drive Alarm : Error(200 - 245)
BOOL CCommon::Check_DirveAlarm()
{
#ifdef AJIN_BOARD_USE
	for (int i = 0; i < AXIS_COUNT; i++) {
		if(i==16)continue;
		if (g_objAJinAXL.Get_AmpAlarm(i)) { //16 axis remove
			Show_Error(i + 200); return FALSE; 
		}
	}
#endif
	return TRUE;
}

// Limit Check : Error ELN(300 - 345), ELP(400 - 445)
BOOL CCommon::Check_EndLimit()
{
#ifdef AJIN_BOARD_USE
	for (int i = 0; i < AXIS_COUNT; i++) {
		if(i==16)continue;
		if (g_objAJinAXL.Use_OrgAxis(i)) continue;
		if (g_objAJinAXL.Get_EndLimitPos(i)) { Show_Error(i + 300); return FALSE; }
		if (g_objAJinAXL.Get_EndLimitNeg(i)) { Show_Error(i + 400); return FALSE; }
	}
#endif
	return TRUE;
}

// Home Check : Error (500 - 545)
BOOL CCommon::Check_HomeDone()
{
#ifdef AJIN_BOARD_USE
	for (int i = 0; i < AXIS_COUNT; i++) {
		if(i==16)continue;
		if (!g_objAJinAXL.Get_HomeDone(i)) { Show_Error(i + 500); return FALSE; }
	}
#endif
	return TRUE;
}

void CCommon::Btm1BlowSunctionOnOff(BOOL bOn)
{
	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();

	pDY09->oBtm1BlowPower = TRUE;

	if(bOn)
	{
		pDY09->oBtm1BlowOnOff = TRUE;

		pDY09->oBtm1SuctionOn = TRUE;
		pDY09->oBtm1SuctionRelease = FALSE;
	}
	else{
		pDY09->oBtm1BlowOnOff = FALSE;

		pDY09->oBtm1SuctionOn = FALSE;
		pDY09->oBtm1SuctionRelease = TRUE;
	}

	g_objAJinAXL.Write_Output(9);
	
}


void CCommon::TopBlowSunctionOnOff(BOOL bOn)
{
	DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();

	pDY09->oTopBlowPower = TRUE;

	if(bOn)
	{
		pDY09->oTopBlowOnOff = TRUE;

		pDY09->oTopSuctionOn = TRUE;
		pDY09->oTopSuctionRelease = FALSE;
	}
	else{
		pDY09->oTopBlowOnOff = FALSE;

		pDY09->oTopSuctionOn = FALSE;
		pDY09->oTopSuctionRelease = TRUE;
	}

	g_objAJinAXL.Write_Output(9);
	
}

void CCommon::Locking_MainDoor(BOOL bLock, BOOL bAuto)
{
	DY_DATA_15 *pDY15 = g_objAJinAXL.Get_pDY15();

	// 0000 0000 0000 1111 1111 1111 1111 1111
	// 0    0    0    F    F    F    F    F
	if (bLock) {
		BOOL bLoad1 = g_objSequenceMain.Get_LotLoadEnable(0);
		BOOL bLoad2 = g_objSequenceMain.Get_LotLoadEnable(1);
		pDY15->nValue &= ~0xFFFFF;	// All Lock
		if (bAuto && bLoad1)				pDY15->nValue |= 0x2;	// Except 2
		if (bAuto && bLoad2)				pDY15->nValue |= 0x4;	// Except 3
		if (bAuto && gData.bNGTrayWait)		pDY15->nValue |= 0x8;	// Except 4
		if (bAuto && gData.bGoodTrayWait)	pDY15->nValue |= 0x10;	// Except 5
		if (bAuto && gData.bEmptyZWait)		pDY15->nValue |= 0x200;	// Except 10
		
	} 
	else
	{
		DWORD dwStart = GetTickCount();
		while (TRUE) {
			BOOL bMove = FALSE;
			for (int i = 0; i < AXIS_COUNT; i++) {
				if (!g_objAJinAXL.Is_Done(i)) { bMove = TRUE; break; }
			}
			if (!bMove) break;
			if (GetTickCount() - dwStart > 5000) break;	// 5초
			theApp.DoEvents();
		}
		pDY15->nValue |= 0xFFFFF;
	}
	g_objAJinAXL.Write_Output(15);
}

void CCommon::Locking_PortSlide(BOOL bLock, int nPart)
{
	if (nPart == 0 || nPart == 1) {
		DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
		pDY00->oLoadPort1SlideLock = bLock; pDY00->oLoadPort1SlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(0);
	}
	if (nPart == 0 || nPart == 2) {
		DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
		pDY00->oLoadPort2SlideLock = bLock; pDY00->oLoadPort2SlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(0);
	}
	if (nPart == 0 || nPart == 3) {
		DY_DATA_09 *pDY09 = g_objAJinAXL.Get_pDY09();
		pDY09->oNgPortSlideLock = bLock; pDY09->oNgPortSlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(9);
	}
	if (nPart == 0 || nPart == 4) {
		DY_DATA_12 *pDY12 = g_objAJinAXL.Get_pDY12();
		pDY12->oGoodPortSlideLock = bLock; pDY12->oGoodPortSlideUnlock = !bLock;
		g_objAJinAXL.Write_Output(12);
	}

}

/////////////////////////////////////////////////////////////////////////////

int CCommon::Get_Random(int nStart, int nEnd)
{
	static BOOL bSeed = FALSE;
	if (nStart >= nEnd) return 0;
	if (!bSeed) { srand((unsigned)time(NULL)); bSeed = TRUE; }
	return (rand() % (nEnd - nStart + 1) + nStart);
}

void CCommon::Backup_File(CString strPath, CString strName)
{
	g_objLogFile.Create_Folder(strPath + "\\Backup");

	CString strOriginFile, strTargetFile, strDateTime;

	SYSTEMTIME time;
	GetLocalTime(&time);
	strDateTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	strOriginFile.Format("%s\\%s.ini", strPath, strName);
	strTargetFile.Format("%s\\Backup\\%s_%s.ini", strPath, strName, strDateTime);

	if (!CopyFile(strOriginFile, strTargetFile, FALSE)) return;
}


///////////////////////////////////////////////////////////////////////////////
// 1. Load Port I/O 함수
void CCommon::Set_LoadPortSupportIn(int nNo)
{
	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	if (nNo == 1) pDY00->oLoadPort1SupportIn = TRUE; pDY00->oLoadPort1SupportOut = FALSE;
	if (nNo == 2) pDY00->oLoadPort2SupportIn = TRUE; pDY00->oLoadPort2SupportOut = FALSE;
	g_objAJinAXL.Write_Output(0);
}

BOOL CCommon::Get_LoadPortSupportIn(int nNo)
{
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	if (nNo == 1) {
		if (!pDX00->iLoadPort1Support1In || pDX00->iLoadPort1Support1Out) return FALSE;
		if (!pDX00->iLoadPort1Support2In || pDX00->iLoadPort1Support2Out) return FALSE;
	} else if (nNo == 2) {
		if (!pDX00->iLoadPort2Support1In || pDX00->iLoadPort2Support1Out) return FALSE;
		if (!pDX00->iLoadPort2Support2In || pDX00->iLoadPort2Support2Out) return FALSE;
	} 
	return TRUE;
}

void CCommon::Set_LoadPortSupportOut(int nNo)
{
	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	if (nNo == 1) pDY00->oLoadPort1SupportIn = FALSE; pDY00->oLoadPort1SupportOut = TRUE;
	if (nNo == 2) pDY00->oLoadPort2SupportIn = FALSE; pDY00->oLoadPort2SupportOut = TRUE;
	g_objAJinAXL.Write_Output(0);
}

BOOL CCommon::Get_LoadPortSupportOut(int nNo)
{
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	if (nNo == 1) {
		if (pDX00->iLoadPort1Support1In || !pDX00->iLoadPort1Support1Out) return FALSE;
		if (pDX00->iLoadPort1Support2In || !pDX00->iLoadPort1Support2Out) return FALSE;
	} else if (nNo == 2) {
		if (pDX00->iLoadPort2Support1In || !pDX00->iLoadPort2Support1Out) return FALSE;
		if (pDX00->iLoadPort2Support2In || !pDX00->iLoadPort2Support2Out) return FALSE;
	} 
	return TRUE;
}

// 2. Load Tray Picker I/O 함수
void CCommon::Set_LoadPickerUp()
{
	DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();
	pDY01->oLoadPickerUp = TRUE; pDY01->oLoadPickerDown = FALSE;
	g_objAJinAXL.Write_Output(1);
}

BOOL CCommon::Get_LoadPickerUp()
{
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	if (!pDX01->iLoadPickerUp || pDX01->iLoadPickerDown) return FALSE;
	return TRUE;
}

void CCommon::Set_LoadPickerDown()
{
	DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();
	pDY01->oLoadPickerUp = FALSE; pDY01->oLoadPickerDown = TRUE;
	g_objAJinAXL.Write_Output(1);
}

BOOL CCommon::Get_LoadPickerDown()
{
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	if (pDX01->iLoadPickerUp || !pDX01->iLoadPickerDown) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_LoadPickerMasterSlaveIn()
{
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	if (!pDX01->iLoadPickerMasterIn || pDX01->iLoadPickerMasterOut) return FALSE;
	if (!pDX01->iLoadPickerSlaveIn  || pDX01->iLoadPickerSlaveOut) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_LoadPickerMasterSlaveOut()
{
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	if (pDX01->iLoadPickerMasterIn || !pDX01->iLoadPickerMasterOut) return FALSE;
	if (pDX01->iLoadPickerSlaveIn  || !pDX01->iLoadPickerSlaveOut) return FALSE;
	return TRUE;
}

// 3. Angle Port I/O 함수
void CCommon::Set_AnglePortSupportIn()
{
	DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();
	pDY02->oAnglePortSupportIn = TRUE; pDY02->oAnglePortSupportOut = FALSE;
	g_objAJinAXL.Write_Output(2);
}

BOOL CCommon::Get_AnglePortSupportIn()
{
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	if (!pDX02->iAnglePortSupport1In || pDX02->iAnglePortSupport1Out) return FALSE;
	if (!pDX02->iAnglePortSupport2In || pDX02->iAnglePortSupport2Out) return FALSE;
	return TRUE;
}

void CCommon::Set_AnglePortSupportOut()
{
	DY_DATA_02 *pDY02 = g_objAJinAXL.Get_pDY02();
	pDY02->oAnglePortSupportIn = FALSE; pDY02->oAnglePortSupportOut = TRUE;
	g_objAJinAXL.Write_Output(2);
}

BOOL CCommon::Get_AnglePortSupportOut()
{
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	if (pDX02->iAnglePortSupport1In || !pDX02->iAnglePortSupport1Out) return FALSE;
	if (pDX02->iAnglePortSupport2In || !pDX02->iAnglePortSupport2Out) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_AnglePortMasterSlaveIn()
{
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	if (!pDX02->iAnglePortMasterIn || pDX02->iAnglePortMasterOut) return FALSE;
	if (!pDX02->iAnglePortSlaveIn  || pDX02->iAnglePortSlaveOut) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_AnglePortMasterSlaveOut()
{
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	if (pDX02->iAnglePortMasterIn || !pDX02->iAnglePortMasterOut) return FALSE;
	if (pDX02->iAnglePortSlaveIn  || !pDX02->iAnglePortSlaveOut) return FALSE;
	return TRUE;
}

// 4. Btm1 Picker I/O 함수
void CCommon::Set_Btm1Picker2RowUp()
{
#ifdef EDITION_2ND
	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();
	pDY04->oBtm1PickerDown05 = FALSE;
	pDY04->oBtm1PickerDown06 = FALSE;
	pDY04->oBtm1PickerDown07 = FALSE;
	pDY04->oBtm1PickerDown08 = FALSE;
	g_objAJinAXL.Write_Output(4);
#else
	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	pDY03->oBtm1PickerDown06 = FALSE;
	pDY03->oBtm1PickerDown07 = FALSE;
	g_objAJinAXL.Write_Output(3);
#endif
}

BOOL CCommon::Get_Btm1Picker2RowUp()
{
#ifdef EDITION_2ND
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();

	// 1Row Down, 2Row Up
	if (!pDX03->iBtm1PickerUp01 && pDX03->iBtm1PickerDown01 &&
		!pDX03->iBtm1PickerUp02 && pDX03->iBtm1PickerDown02 &&
		!pDX03->iBtm1PickerUp03 && pDX03->iBtm1PickerDown03 &&
		!pDX03->iBtm1PickerUp04 && pDX03->iBtm1PickerDown04 &&
		pDX04->iBtm1PickerUp05 && !pDX04->iBtm1PickerDown05 &&
		pDX04->iBtm1PickerUp06 && !pDX04->iBtm1PickerDown06 &&
		pDX04->iBtm1PickerUp07 && !pDX04->iBtm1PickerDown07 &&
		pDX04->iBtm1PickerUp08 && !pDX04->iBtm1PickerDown08) return TRUE;
#else
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	
	// 1Row Down, 2Row Up
	if (!pDX03->iBtm1PickerUp02 && pDX03->iBtm1PickerDown02 &&
		!pDX03->iBtm1PickerUp04 && pDX03->iBtm1PickerDown04 &&
		pDX03->iBtm1PickerUp07 && !pDX03->iBtm1PickerDown07 &&
		pDX03->iBtm1PickerUp09 && !pDX03->iBtm1PickerDown09) return TRUE;

#endif
	return FALSE;
}

void CCommon::Set_Btm1PickerDown()
{
 	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();

#ifdef EDITION_2ND
	pDY03->oBtm1PickerDown01 = TRUE;
	pDY03->oBtm1PickerDown02 = TRUE;
	pDY03->oBtm1PickerDown03 = TRUE;
	pDY03->oBtm1PickerDown04 = TRUE;

	pDY04->oBtm1PickerDown05 = TRUE;
	pDY04->oBtm1PickerDown06 = TRUE;
	pDY04->oBtm1PickerDown07 = TRUE;
	pDY04->oBtm1PickerDown08 = TRUE;
#else
	pDY03->oBtm1PickerDown01 = TRUE; pDY03->oBtm1PickerDown02 = TRUE;
	pDY03->oBtm1PickerDown06 = TRUE; pDY03->oBtm1PickerDown07 = TRUE;
#endif
	g_objAJinAXL.Write_Output(3);
	g_objAJinAXL.Write_Output(4);
}


void CCommon::Set_Btm1PickerRow2Down()
{
 	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();

#ifdef EDITION_2ND
	pDY03->oBtm1PickerDown01 = FALSE;
	pDY03->oBtm1PickerDown02 = FALSE;
	pDY03->oBtm1PickerDown03 = FALSE;
	pDY03->oBtm1PickerDown04 = FALSE;

	pDY04->oBtm1PickerDown05 = TRUE;
	pDY04->oBtm1PickerDown06 = TRUE;
	pDY04->oBtm1PickerDown07 = TRUE;
	pDY04->oBtm1PickerDown08 = TRUE;
#else
	pDY03->oBtm1PickerDown01 = TRUE; pDY03->oBtm1PickerDown02 = TRUE;
	pDY03->oBtm1PickerDown06 = TRUE; pDY03->oBtm1PickerDown07 = TRUE;
#endif
	g_objAJinAXL.Write_Output(3);
	g_objAJinAXL.Write_Output(4);
}

BOOL CCommon::Get_Btm1PickerDown(int nSub)
{
#ifdef EDITION_2ND
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	if (nSub == 0) {
		if (!pDX03->iBtm1PickerUp01 && pDX03->iBtm1PickerDown01 &&
			!pDX03->iBtm1PickerUp02 && pDX03->iBtm1PickerDown02 &&
			!pDX03->iBtm1PickerUp03 && pDX03->iBtm1PickerDown03 &&
			!pDX03->iBtm1PickerUp04 && pDX03->iBtm1PickerDown04 &&
			!pDX04->iBtm1PickerUp05 && pDX04->iBtm1PickerDown05 &&
			!pDX04->iBtm1PickerUp06 && pDX04->iBtm1PickerDown06 &&
			!pDX04->iBtm1PickerUp07 && pDX04->iBtm1PickerDown07 &&
			!pDX04->iBtm1PickerUp08 && pDX04->iBtm1PickerDown08) return TRUE;
	}
	else if (nSub ==  1 && !pDX03->iBtm1PickerUp01 && pDX03->iBtm1PickerDown01) return TRUE;
	else if (nSub ==  2 && !pDX03->iBtm1PickerUp02 && pDX03->iBtm1PickerDown02) return TRUE;
	else if (nSub ==  3 && !pDX03->iBtm1PickerUp03 && pDX03->iBtm1PickerDown03) return TRUE;
	else if (nSub ==  4 && !pDX03->iBtm1PickerUp04 && pDX03->iBtm1PickerDown04) return TRUE;
	else if (nSub ==  5 && !pDX04->iBtm1PickerUp05 && pDX04->iBtm1PickerDown05) return TRUE;
	else if (nSub ==  6 && !pDX04->iBtm1PickerUp06 && pDX04->iBtm1PickerDown06) return TRUE;
	else if (nSub ==  7 && !pDX04->iBtm1PickerUp07 && pDX04->iBtm1PickerDown07) return TRUE;
	else if (nSub ==  8 && !pDX04->iBtm1PickerUp08 && pDX04->iBtm1PickerDown08) return TRUE;
	return FALSE;
#else
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	if (nSub == 0) {
		if (!pDX03->iBtm1PickerUp02 && pDX03->iBtm1PickerDown02 &&
			!pDX03->iBtm1PickerUp04 && pDX03->iBtm1PickerDown04 &&
			!pDX03->iBtm1PickerUp07 && pDX03->iBtm1PickerDown07 &&
			!pDX03->iBtm1PickerUp09 && pDX03->iBtm1PickerDown09) return TRUE;
	}
	else if (nSub ==  2 && !pDX03->iBtm1PickerUp02 && pDX03->iBtm1PickerDown02) return TRUE;
	else if (nSub ==  4 && !pDX03->iBtm1PickerUp04 && pDX03->iBtm1PickerDown04) return TRUE;
	else if (nSub ==  7 && !pDX03->iBtm1PickerUp07 && pDX03->iBtm1PickerDown07) return TRUE;
	else if (nSub ==  9 && !pDX03->iBtm1PickerUp09 && pDX03->iBtm1PickerDown09) return TRUE;
	return FALSE;
#endif

}

void CCommon::Set_Btm1PickerVacOff(int nSub)
{
	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();
	if (nSub == 0 || nSub ==  1) { pDY04->oBtm1PickerVac01 = FALSE; pDY04->oBtm1PickerAir01 = TRUE; }
	if (nSub == 0 || nSub ==  2) { pDY04->oBtm1PickerVac02 = FALSE; pDY04->oBtm1PickerAir02 = TRUE; }
	if (nSub == 0 || nSub ==  3) { pDY04->oBtm1PickerVac03 = FALSE; pDY04->oBtm1PickerAir03 = TRUE; }
	if (nSub == 0 || nSub ==  4) { pDY04->oBtm1PickerVac04 = FALSE; pDY04->oBtm1PickerAir04 = TRUE; }
	if (nSub == 0 || nSub ==  5) { pDY04->oBtm1PickerVac05 = FALSE; pDY04->oBtm1PickerAir05 = TRUE; }
	if (nSub == 0 || nSub ==  6) { pDY04->oBtm1PickerVac06 = FALSE; pDY04->oBtm1PickerAir06 = TRUE; }
	if (nSub == 0 || nSub ==  7) { pDY04->oBtm1PickerVac07 = FALSE; pDY04->oBtm1PickerAir07 = TRUE; }
	if (nSub == 0 || nSub ==  8) { pDY04->oBtm1PickerVac08 = FALSE; pDY04->oBtm1PickerAir08 = TRUE; }
	g_objAJinAXL.Write_Output(4);
}

BOOL CCommon::Get_Btm1PickerVacOff(int nSub)
{
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	if (nSub == 0) {
		if (!pDX04->iBtm1PickerVac01 && !pDX04->iBtm1PickerVac02 &&
			!pDX04->iBtm1PickerVac03 && !pDX04->iBtm1PickerVac04 &&
			!pDX04->iBtm1PickerVac05 && !pDX04->iBtm1PickerVac06 &&
			!pDX04->iBtm1PickerVac07 && !pDX04->iBtm1PickerVac08) return TRUE;
	}
	else if (nSub ==  1 && !pDX04->iBtm1PickerVac01) return TRUE;
	else if (nSub ==  2 && !pDX04->iBtm1PickerVac02) return TRUE;
	else if (nSub ==  3 && !pDX04->iBtm1PickerVac03) return TRUE;
	else if (nSub ==  4 && !pDX04->iBtm1PickerVac04) return TRUE;
	else if (nSub ==  5 && !pDX04->iBtm1PickerVac05) return TRUE;
	else if (nSub ==  6 && !pDX04->iBtm1PickerVac06) return TRUE;
	else if (nSub ==  7 && !pDX04->iBtm1PickerVac07) return TRUE;
	else if (nSub ==  8 && !pDX04->iBtm1PickerVac08) return TRUE;
	return FALSE;
}

void CCommon::Set_Btm1PickerAirOff(int nSub)
{
	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();
	if (nSub == 0 || nSub ==  1) pDY04->oBtm1PickerAir01 = FALSE;
	if (nSub == 0 || nSub ==  2) pDY04->oBtm1PickerAir02 = FALSE;
	if (nSub == 0 || nSub ==  3) pDY04->oBtm1PickerAir03 = FALSE;
	if (nSub == 0 || nSub ==  4) pDY04->oBtm1PickerAir04 = FALSE;
	if (nSub == 0 || nSub ==  5) pDY04->oBtm1PickerAir05 = FALSE;
	if (nSub == 0 || nSub ==  6) pDY04->oBtm1PickerAir06 = FALSE;
	if (nSub == 0 || nSub ==  7) pDY04->oBtm1PickerAir07 = FALSE;
	if (nSub == 0 || nSub ==  8) pDY04->oBtm1PickerAir08 = FALSE;
	g_objAJinAXL.Write_Output(4);
}

void CCommon::Set_Btm1PickerOpen(int nSub)
{
	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	if (nSub == 0 || nSub ==  1) { pDY03->oBtm1PickerOpen01 = TRUE; pDY03->oBtm1PickerClose01 = FALSE; }
	if (nSub == 0 || nSub ==  2) { pDY03->oBtm1PickerOpen02 = TRUE; pDY03->oBtm1PickerClose02 = FALSE; }
	if (nSub == 0 || nSub ==  3) { pDY03->oBtm1PickerOpen03 = TRUE; pDY03->oBtm1PickerClose03 = FALSE; }
	if (nSub == 0 || nSub ==  4) { pDY03->oBtm1PickerOpen04 = TRUE; pDY03->oBtm1PickerClose04 = FALSE; }
	if (nSub == 0 || nSub ==  5) { pDY03->oBtm1PickerOpen05 = TRUE; pDY03->oBtm1PickerClose05 = FALSE; }
	if (nSub == 0 || nSub ==  6) { pDY03->oBtm1PickerOpen06 = TRUE; pDY03->oBtm1PickerClose06 = FALSE; }
	if (nSub == 0 || nSub ==  7) { pDY03->oBtm1PickerOpen07 = TRUE; pDY03->oBtm1PickerClose07 = FALSE; }
	if (nSub == 0 || nSub ==  8) { pDY03->oBtm1PickerOpen08 = TRUE; pDY03->oBtm1PickerClose08 = FALSE; }
	g_objAJinAXL.Write_Output(3);
}

BOOL CCommon::Get_Btm1PickerOpen(int nSub)
{
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	if (nSub == 0) {
		if (pDX03->iBtm1PickerOpen01 && pDX03->iBtm1PickerOpen02 &&
			pDX03->iBtm1PickerOpen03 && pDX03->iBtm1PickerOpen04 &&
			pDX03->iBtm1PickerOpen05 && pDX03->iBtm1PickerOpen06 &&
			pDX03->iBtm1PickerOpen07 && pDX03->iBtm1PickerOpen08) return TRUE;
	}
	else if (nSub ==  1 && pDX03->iBtm1PickerOpen01) return TRUE;
	else if (nSub ==  2 && pDX03->iBtm1PickerOpen02) return TRUE;
	else if (nSub ==  3 && pDX03->iBtm1PickerOpen03) return TRUE;
	else if (nSub ==  4 && pDX03->iBtm1PickerOpen04) return TRUE;
	else if (nSub ==  5 && pDX03->iBtm1PickerOpen05) return TRUE;
	else if (nSub ==  6 && pDX03->iBtm1PickerOpen06) return TRUE;
	else if (nSub ==  7 && pDX03->iBtm1PickerOpen07) return TRUE;
	else if (nSub ==  8 && pDX03->iBtm1PickerOpen08) return TRUE;
	return FALSE;
}


void CCommon::Set_Btm1PickerClose(int nSub)
{
	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	if (nSub == 0 || nSub ==  1) { pDY03->oBtm1PickerOpen01 = FALSE; pDY03->oBtm1PickerClose01 = TRUE; }
	if (nSub == 0 || nSub ==  2) { pDY03->oBtm1PickerOpen02 = FALSE; pDY03->oBtm1PickerClose02 = TRUE; }
	if (nSub == 0 || nSub ==  3) { pDY03->oBtm1PickerOpen03 = FALSE; pDY03->oBtm1PickerClose03 = TRUE; }
	if (nSub == 0 || nSub ==  4) { pDY03->oBtm1PickerOpen04 = FALSE; pDY03->oBtm1PickerClose04 = TRUE; }
	if (nSub == 0 || nSub ==  5) { pDY03->oBtm1PickerOpen05 = FALSE; pDY03->oBtm1PickerClose05 = TRUE; }
	if (nSub == 0 || nSub ==  6) { pDY03->oBtm1PickerOpen06 = FALSE; pDY03->oBtm1PickerClose06 = TRUE; }
	if (nSub == 0 || nSub ==  7) { pDY03->oBtm1PickerOpen07 = FALSE; pDY03->oBtm1PickerClose07 = TRUE; }
	if (nSub == 0 || nSub ==  8) { pDY03->oBtm1PickerOpen08 = FALSE; pDY03->oBtm1PickerClose08 = TRUE; }
	g_objAJinAXL.Write_Output(3);
}


BOOL CCommon::Get_Btm1PickerClose(int nSub)
{
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	if (nSub == 0) {
		if (pDX03->iBtm1PickerExist01 && pDX03->iBtm1PickerExist02 &&
			pDX03->iBtm1PickerExist03 && pDX03->iBtm1PickerExist04 &&
			pDX03->iBtm1PickerExist05 && pDX03->iBtm1PickerExist06 &&
			pDX03->iBtm1PickerExist07 && pDX03->iBtm1PickerExist08) return TRUE;
	}
	else if (nSub ==  1 && pDX03->iBtm1PickerExist01) return TRUE;
	else if (nSub ==  2 && pDX03->iBtm1PickerExist02) return TRUE;
	else if (nSub ==  3 && pDX03->iBtm1PickerExist03) return TRUE;
	else if (nSub ==  4 && pDX03->iBtm1PickerExist04) return TRUE;
	else if (nSub ==  5 && pDX03->iBtm1PickerExist05) return TRUE;
	else if (nSub ==  6 && pDX03->iBtm1PickerExist06) return TRUE;
	else if (nSub ==  7 && pDX03->iBtm1PickerExist07) return TRUE;
	else if (nSub ==  8 && pDX03->iBtm1PickerExist08) return TRUE;
	return FALSE;
}




BOOL CCommon::Get_Btm1PickerAllNotChk()
{
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	if (!pDX03->iBtm1PickerExist01 && !pDX03->iBtm1PickerExist02 && !pDX03->iBtm1PickerExist03 && !pDX03->iBtm1PickerExist04 &&
		!pDX03->iBtm1PickerExist05 && !pDX03->iBtm1PickerExist06 && !pDX03->iBtm1PickerExist07 && !pDX03->iBtm1PickerExist08) return TRUE;
	return FALSE;
}

// 5. Inspection I/O 함수
void CCommon::Set_InspectStage1VacOff(int nSub)
{
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	if (nSub == 0 || nSub ==  1) pDY05->oInspectStage1Vac01 = FALSE;
	if (nSub == 0 || nSub ==  2) pDY05->oInspectStage1Vac02 = FALSE;
	if (nSub == 0 || nSub ==  3) pDY05->oInspectStage1Vac03 = FALSE;
	if (nSub == 0 || nSub ==  4) pDY05->oInspectStage1Vac04 = FALSE;
	if (nSub == 0 || nSub ==  5) pDY05->oInspectStage1Vac05 = FALSE;
	if (nSub == 0 || nSub ==  6) pDY05->oInspectStage1Vac06 = FALSE;
	if (nSub == 0 || nSub ==  7) pDY05->oInspectStage1Vac07 = FALSE;
	if (nSub == 0 || nSub ==  8) pDY05->oInspectStage1Vac08 = FALSE;
	g_objAJinAXL.Write_Output(5);
}

BOOL CCommon::Get_InspectStage1VacOff(int nSub)
{
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	if (nSub == 0) {
		if (!pDX05->iInspectStage1Vac01 && !pDX05->iInspectStage1Vac02 &&
			!pDX05->iInspectStage1Vac03 && !pDX05->iInspectStage1Vac04 &&
			!pDX05->iInspectStage1Vac05 && !pDX05->iInspectStage1Vac06 &&
			!pDX05->iInspectStage1Vac07 && !pDX05->iInspectStage1Vac08) return TRUE;
	}
	else if (nSub ==  1 && !pDX05->iInspectStage1Vac01) return TRUE;
	else if (nSub ==  2 && !pDX05->iInspectStage1Vac02) return TRUE;
	else if (nSub ==  3 && !pDX05->iInspectStage1Vac03) return TRUE;
	else if (nSub ==  4 && !pDX05->iInspectStage1Vac04) return TRUE;
	else if (nSub ==  5 && !pDX05->iInspectStage1Vac05) return TRUE;
	else if (nSub ==  6 && !pDX05->iInspectStage1Vac06) return TRUE;
	else if (nSub ==  7 && !pDX05->iInspectStage1Vac07) return TRUE;
	else if (nSub ==  8 && !pDX05->iInspectStage1Vac08) return TRUE;
	return FALSE;
}

void CCommon::Set_InspectStage2VacOff(int nSub)
{
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	if (nSub == 0 || nSub ==  1) pDY06->oInspectStage2Vac01 = FALSE;
	if (nSub == 0 || nSub ==  2) pDY06->oInspectStage2Vac02 = FALSE;
	if (nSub == 0 || nSub ==  3) pDY06->oInspectStage2Vac03 = FALSE;
	if (nSub == 0 || nSub ==  4) pDY06->oInspectStage2Vac04 = FALSE;
	if (nSub == 0 || nSub ==  5) pDY06->oInspectStage2Vac05 = FALSE;
	if (nSub == 0 || nSub ==  6) pDY06->oInspectStage2Vac06 = FALSE;
	if (nSub == 0 || nSub ==  7) pDY06->oInspectStage2Vac07 = FALSE;
	if (nSub == 0 || nSub ==  8) pDY06->oInspectStage2Vac08 = FALSE;
	g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_InspectStage2VacOff(int nSub)
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	if (nSub == 0) {
		if (!pDX06->iInspectStage2Vac01 && !pDX06->iInspectStage2Vac02 &&
			!pDX06->iInspectStage2Vac03 && !pDX06->iInspectStage2Vac04 &&
			!pDX06->iInspectStage2Vac05 && !pDX06->iInspectStage2Vac06 &&
			!pDX06->iInspectStage2Vac07 && !pDX06->iInspectStage2Vac08) return TRUE;
	}
	else if (nSub ==  1 && !pDX06->iInspectStage2Vac01) return TRUE;
	else if (nSub ==  2 && !pDX06->iInspectStage2Vac02) return TRUE;
	else if (nSub ==  3 && !pDX06->iInspectStage2Vac03) return TRUE;
	else if (nSub ==  4 && !pDX06->iInspectStage2Vac04) return TRUE;
	else if (nSub ==  5 && !pDX06->iInspectStage2Vac05) return TRUE;
	else if (nSub ==  6 && !pDX06->iInspectStage2Vac06) return TRUE;
	else if (nSub ==  7 && !pDX06->iInspectStage2Vac07) return TRUE;
	else if (nSub ==  8 && !pDX06->iInspectStage2Vac08) return TRUE;
	return FALSE;
}

void CCommon::Set_InspectStage3VacOff(int nSub)
{
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	if (nSub == 0 || nSub ==  1) pDY06->oInspectStage3Vac01 = FALSE;
	if (nSub == 0 || nSub ==  2) pDY06->oInspectStage3Vac02 = FALSE;
	if (nSub == 0 || nSub ==  3) pDY06->oInspectStage3Vac03 = FALSE;
	if (nSub == 0 || nSub ==  4) pDY06->oInspectStage3Vac04 = FALSE;
	if (nSub == 0 || nSub ==  5) pDY06->oInspectStage3Vac05 = FALSE;
	if (nSub == 0 || nSub ==  6) pDY06->oInspectStage3Vac06 = FALSE;
	if (nSub == 0 || nSub ==  7) pDY06->oInspectStage3Vac07 = FALSE;
	if (nSub == 0 || nSub ==  8) pDY06->oInspectStage3Vac08 = FALSE;
	g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_InspectStage3VacOff(int nSub)
{
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	if (nSub == 0) {
		if (!pDX06->iInspectStage3Vac01 && !pDX06->iInspectStage3Vac02 &&
			!pDX06->iInspectStage3Vac03 && !pDX06->iInspectStage3Vac04 &&
			!pDX06->iInspectStage3Vac05 && !pDX06->iInspectStage3Vac06 &&
			!pDX06->iInspectStage3Vac07 && !pDX06->iInspectStage3Vac08) return TRUE;
	}
	else if (nSub ==  1 && !pDX06->iInspectStage3Vac01) return TRUE;
	else if (nSub ==  2 && !pDX06->iInspectStage3Vac02) return TRUE;
	else if (nSub ==  3 && !pDX06->iInspectStage3Vac03) return TRUE;
	else if (nSub ==  4 && !pDX06->iInspectStage3Vac04) return TRUE;
	else if (nSub ==  5 && !pDX06->iInspectStage3Vac05) return TRUE;
	else if (nSub ==  6 && !pDX06->iInspectStage3Vac06) return TRUE;
	else if (nSub ==  7 && !pDX06->iInspectStage3Vac07) return TRUE;
	else if (nSub ==  8 && !pDX06->iInspectStage3Vac08) return TRUE;
	return FALSE;
}
void CCommon::Set_InspectVacOn(int nNo, int nSub)
{
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();

	if (nNo == 1) {
		if (nSub == 0 || nSub ==  1) pDY05->oInspectStage1Vac01 = TRUE;
		if (nSub == 0 || nSub ==  2) pDY05->oInspectStage1Vac02 = TRUE;
		if (nSub == 0 || nSub ==  3) pDY05->oInspectStage1Vac03 = TRUE;
		if (nSub == 0 || nSub ==  4) pDY05->oInspectStage1Vac04 = TRUE;
		if (nSub == 0 || nSub ==  5) pDY05->oInspectStage1Vac05 = TRUE;
		if (nSub == 0 || nSub ==  6) pDY05->oInspectStage1Vac06 = TRUE;
		if (nSub == 0 || nSub ==  7) pDY05->oInspectStage1Vac07 = TRUE;
		if (nSub == 0 || nSub ==  8) pDY05->oInspectStage1Vac08 = TRUE;

	} else if (nNo == 2) {
		if (nSub == 0 || nSub ==  1) pDY06->oInspectStage2Vac01 = TRUE;
		if (nSub == 0 || nSub ==  2) pDY06->oInspectStage2Vac02 = TRUE;
		if (nSub == 0 || nSub ==  3) pDY06->oInspectStage2Vac03 = TRUE;
		if (nSub == 0 || nSub ==  4) pDY06->oInspectStage2Vac04 = TRUE;
		if (nSub == 0 || nSub ==  5) pDY06->oInspectStage2Vac05 = TRUE;
		if (nSub == 0 || nSub ==  6) pDY06->oInspectStage2Vac06 = TRUE;
		if (nSub == 0 || nSub ==  7) pDY06->oInspectStage2Vac07 = TRUE;
		if (nSub == 0 || nSub ==  8) pDY06->oInspectStage2Vac08 = TRUE;

	} else if (nNo == 3) {
		if (nSub == 0 || nSub ==  1) pDY06->oInspectStage3Vac01 = TRUE;
		if (nSub == 0 || nSub ==  2) pDY06->oInspectStage3Vac02 = TRUE;
		if (nSub == 0 || nSub ==  3) pDY06->oInspectStage3Vac03 = TRUE;
		if (nSub == 0 || nSub ==  4) pDY06->oInspectStage3Vac04 = TRUE;
		if (nSub == 0 || nSub ==  5) pDY06->oInspectStage3Vac05 = TRUE;
		if (nSub == 0 || nSub ==  6) pDY06->oInspectStage3Vac06 = TRUE;
		if (nSub == 0 || nSub ==  7) pDY06->oInspectStage3Vac07 = TRUE;
		if (nSub == 0 || nSub ==  8) pDY06->oInspectStage3Vac08 = TRUE;
	}

	if (nNo == 1) g_objAJinAXL.Write_Output(5);
	else		  g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_InspectVacOn(int nNo, int nSub)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();

	if (nNo == 1) {	// Inspect Stage 1
		if (nSub == 0) {
			if (pDX05->iInspectStage1Vac01 && pDX05->iInspectStage1Vac02 &&
				pDX05->iInspectStage1Vac03 && pDX05->iInspectStage1Vac04 &&
				pDX05->iInspectStage1Vac05 && pDX05->iInspectStage1Vac06 &&
				pDX05->iInspectStage1Vac07 && pDX05->iInspectStage1Vac08) return TRUE;
		}
		else if (nSub ==  1 && pDX05->iInspectStage1Vac01) return TRUE;
		else if (nSub ==  2 && pDX05->iInspectStage1Vac02) return TRUE;
		else if (nSub ==  3 && pDX05->iInspectStage1Vac03) return TRUE;
		else if (nSub ==  4 && pDX05->iInspectStage1Vac04) return TRUE;
		else if (nSub ==  5 && pDX05->iInspectStage1Vac05) return TRUE;
		else if (nSub ==  6 && pDX05->iInspectStage1Vac06) return TRUE;
		else if (nSub ==  7 && pDX05->iInspectStage1Vac07) return TRUE;
		else if (nSub ==  8 && pDX05->iInspectStage1Vac08) return TRUE;
		
	} else if (nNo == 2) {	// Inspect Stage 2
		if (nSub == 0) {
			if (pDX06->iInspectStage2Vac01 && pDX06->iInspectStage2Vac02 &&
				pDX06->iInspectStage2Vac03 && pDX06->iInspectStage2Vac04 &&
				pDX06->iInspectStage2Vac05 && pDX06->iInspectStage2Vac06 &&
				pDX06->iInspectStage2Vac07 && pDX06->iInspectStage2Vac08) return TRUE;
		}
		else if (nSub ==  1 && pDX06->iInspectStage2Vac01) return TRUE;
		else if (nSub ==  2 && pDX06->iInspectStage2Vac02) return TRUE;
		else if (nSub ==  3 && pDX06->iInspectStage2Vac03) return TRUE;
		else if (nSub ==  4 && pDX06->iInspectStage2Vac04) return TRUE;
		else if (nSub ==  5 && pDX06->iInspectStage2Vac05) return TRUE;
		else if (nSub ==  6 && pDX06->iInspectStage2Vac06) return TRUE;
		else if (nSub ==  7 && pDX06->iInspectStage2Vac07) return TRUE;
		else if (nSub ==  8 && pDX06->iInspectStage2Vac08) return TRUE;

	} else if (nNo == 3) {	// Inspect Stage 3
		if (nSub == 0) {
			if (pDX06->iInspectStage3Vac01 && pDX06->iInspectStage3Vac02 &&
				pDX06->iInspectStage3Vac03 && pDX06->iInspectStage3Vac04 &&
				pDX06->iInspectStage3Vac05 && pDX06->iInspectStage3Vac06 &&
				pDX06->iInspectStage3Vac07 && pDX06->iInspectStage3Vac08) return TRUE;
		}
		else if (nSub ==  1 && pDX06->iInspectStage3Vac01) return TRUE;
		else if (nSub ==  2 && pDX06->iInspectStage3Vac02) return TRUE;
		else if (nSub ==  3 && pDX06->iInspectStage3Vac03) return TRUE;
		else if (nSub ==  4 && pDX06->iInspectStage3Vac04) return TRUE;
		else if (nSub ==  5 && pDX06->iInspectStage3Vac05) return TRUE;
		else if (nSub ==  6 && pDX06->iInspectStage3Vac06) return TRUE;
		else if (nSub ==  7 && pDX06->iInspectStage3Vac07) return TRUE;
		else if (nSub ==  8 && pDX06->iInspectStage3Vac08) return TRUE;
	}
	return FALSE;
}

void CCommon::Set_InspectVacOff(int nNo, int nSub)
{
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();

	if (nNo == 1) {
		if (nSub == 0 || nSub ==  1) pDY05->oInspectStage1Vac01 = FALSE;
		if (nSub == 0 || nSub ==  2) pDY05->oInspectStage1Vac02 = FALSE;
		if (nSub == 0 || nSub ==  3) pDY05->oInspectStage1Vac03 = FALSE;
		if (nSub == 0 || nSub ==  4) pDY05->oInspectStage1Vac04 = FALSE;
		if (nSub == 0 || nSub ==  5) pDY05->oInspectStage1Vac05 = FALSE;
		if (nSub == 0 || nSub ==  6) pDY05->oInspectStage1Vac06 = FALSE;
		if (nSub == 0 || nSub ==  7) pDY05->oInspectStage1Vac07 = FALSE;
		if (nSub == 0 || nSub ==  8) pDY05->oInspectStage1Vac08 = FALSE;

	} else if (nNo == 2) {
		if (nSub == 0 || nSub ==  1) pDY06->oInspectStage2Vac01 = FALSE;
		if (nSub == 0 || nSub ==  2) pDY06->oInspectStage2Vac02 = FALSE;
		if (nSub == 0 || nSub ==  3) pDY06->oInspectStage2Vac03 = FALSE;
		if (nSub == 0 || nSub ==  4) pDY06->oInspectStage2Vac04 = FALSE;
		if (nSub == 0 || nSub ==  5) pDY06->oInspectStage2Vac05 = FALSE;
		if (nSub == 0 || nSub ==  6) pDY06->oInspectStage2Vac06 = FALSE;
		if (nSub == 0 || nSub ==  7) pDY06->oInspectStage2Vac07 = FALSE;
		if (nSub == 0 || nSub ==  8) pDY06->oInspectStage2Vac08 = FALSE;

	} else if (nNo == 3) {
		if (nSub == 0 || nSub ==  1) pDY06->oInspectStage3Vac01 = FALSE;
		if (nSub == 0 || nSub ==  2) pDY06->oInspectStage3Vac02 = FALSE;
		if (nSub == 0 || nSub ==  3) pDY06->oInspectStage3Vac03 = FALSE;
		if (nSub == 0 || nSub ==  4) pDY06->oInspectStage3Vac04 = FALSE;
		if (nSub == 0 || nSub ==  5) pDY06->oInspectStage3Vac05 = FALSE;
		if (nSub == 0 || nSub ==  6) pDY06->oInspectStage3Vac06 = FALSE;
		if (nSub == 0 || nSub ==  7) pDY06->oInspectStage3Vac07 = FALSE;
		if (nSub == 0 || nSub ==  8) pDY06->oInspectStage3Vac08 = FALSE;
	}

	if (nNo == 1) g_objAJinAXL.Write_Output(5);
	else		  g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_InspectVacOff(int nNo, int nSub)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();

	if (nNo == 1) {	// Inspect Stage 1
		if (nSub == 0) {
			if (!pDX05->iInspectStage1Vac01 && !pDX05->iInspectStage1Vac02 &&
				!pDX05->iInspectStage1Vac03 && !pDX05->iInspectStage1Vac04 &&
				!pDX05->iInspectStage1Vac05 && !pDX05->iInspectStage1Vac06 &&
				!pDX05->iInspectStage1Vac07 && !pDX05->iInspectStage1Vac08) return TRUE;
		}
		else if (nSub ==  1 && !pDX05->iInspectStage1Vac01) return TRUE;
		else if (nSub ==  2 && !pDX05->iInspectStage1Vac02) return TRUE;
		else if (nSub ==  3 && !pDX05->iInspectStage1Vac03) return TRUE;
		else if (nSub ==  4 && !pDX05->iInspectStage1Vac04) return TRUE;
		else if (nSub ==  5 && !pDX05->iInspectStage1Vac05) return TRUE;
		else if (nSub ==  6 && !pDX05->iInspectStage1Vac06) return TRUE;
		else if (nSub ==  7 && !pDX05->iInspectStage1Vac07) return TRUE;
		else if (nSub ==  8 && !pDX05->iInspectStage1Vac08) return TRUE;

	} else if (nNo == 2) {	// Inspect Stage 2
		if (nSub == 0) {
			if (!pDX06->iInspectStage2Vac01 && !pDX06->iInspectStage2Vac02 &&
				!pDX06->iInspectStage2Vac03 && !pDX06->iInspectStage2Vac04 &&
				!pDX06->iInspectStage2Vac05 && !pDX06->iInspectStage2Vac06 &&
				!pDX06->iInspectStage2Vac07 && !pDX06->iInspectStage2Vac08) return TRUE;
		}
		else if (nSub ==  1 && !pDX06->iInspectStage2Vac01) return TRUE;
		else if (nSub ==  2 && !pDX06->iInspectStage2Vac02) return TRUE;
		else if (nSub ==  3 && !pDX06->iInspectStage2Vac03) return TRUE;
		else if (nSub ==  4 && !pDX06->iInspectStage2Vac04) return TRUE;
		else if (nSub ==  5 && !pDX06->iInspectStage2Vac05) return TRUE;
		else if (nSub ==  6 && !pDX06->iInspectStage2Vac06) return TRUE;
		else if (nSub ==  7 && !pDX06->iInspectStage2Vac07) return TRUE;
		else if (nSub ==  8 && !pDX06->iInspectStage2Vac08) return TRUE;

	} else if (nNo == 3) {	// Inspect Stage 3
		if (nSub == 0) {
			if (!pDX06->iInspectStage3Vac01 && !pDX06->iInspectStage3Vac02 &&
				!pDX06->iInspectStage3Vac03 && !pDX06->iInspectStage3Vac04 &&
				!pDX06->iInspectStage3Vac05 && !pDX06->iInspectStage3Vac06 &&
				!pDX06->iInspectStage3Vac07 && !pDX06->iInspectStage3Vac08) return TRUE;
		}
		else if (nSub ==  1 && !pDX06->iInspectStage3Vac01) return TRUE;
		else if (nSub ==  2 && !pDX06->iInspectStage3Vac02) return TRUE;
		else if (nSub ==  3 && !pDX06->iInspectStage3Vac03) return TRUE;
		else if (nSub ==  4 && !pDX06->iInspectStage3Vac04) return TRUE;
		else if (nSub ==  5 && !pDX06->iInspectStage3Vac05) return TRUE;
		else if (nSub ==  6 && !pDX06->iInspectStage3Vac06) return TRUE;
		else if (nSub ==  7 && !pDX06->iInspectStage3Vac07) return TRUE;
		else if (nSub ==  8 && !pDX06->iInspectStage3Vac08) return TRUE;
	}
	return FALSE;
}

// 6. Btm2 Picker I/O 함수
void CCommon::Set_Btm2PickerUp(int nSub)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
#ifdef EDITION_2ND
	if (nSub == 0 || nSub ==  5) pDY08->oBtm2PickerDown05 = FALSE;
	if (nSub == 0 || nSub ==  6) pDY08->oBtm2PickerDown06 = FALSE;
	if (nSub == 0 || nSub ==  7) pDY08->oBtm2PickerDown07 = FALSE;
	if (nSub == 0 || nSub ==  8) pDY08->oBtm2PickerDown08 = FALSE;
	g_objAJinAXL.Write_Output(8);
#else
	pDY07->oBtm2PickerDown06 = FALSE;
	pDY07->oBtm2PickerDown07 = FALSE;
	g_objAJinAXL.Write_Output(7);
#endif
}

BOOL CCommon::Get_Btm2PickerUp(int nSub)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
#ifdef EDITION_2ND
	if (nSub == 0) {
		if (pDX08->iBtm2PickerUp05 && !pDX08->iBtm2PickerDown05 &&
			pDX08->iBtm2PickerUp06 && !pDX08->iBtm2PickerDown06 &&
			pDX08->iBtm2PickerUp07 && !pDX08->iBtm2PickerDown07 &&
			pDX08->iBtm2PickerUp08 && !pDX08->iBtm2PickerDown08) return TRUE;
	}
 	else if (nSub ==  5 && pDX08->iBtm2PickerUp05 && !pDX08->iBtm2PickerDown05) return TRUE;
	else if (nSub ==  6 && pDX08->iBtm2PickerUp06 && !pDX08->iBtm2PickerDown06) return TRUE;
	else if (nSub ==  7 && pDX08->iBtm2PickerUp07 && !pDX08->iBtm2PickerDown07) return TRUE;
	else if (nSub ==  8 && pDX08->iBtm2PickerUp08 && !pDX08->iBtm2PickerDown08) return TRUE;
#else
	if (nSub == 0) {
		if (pDX08->iBtm2PickerUp07 && !pDX08->iBtm2PickerDown07 &&
			pDX08->iBtm2PickerUp09 && !pDX08->iBtm2PickerDown09) return TRUE;
	}
	else if (nSub ==  7 && pDX08->iBtm2PickerUp07 && !pDX08->iBtm2PickerDown07) return TRUE;
	else if (nSub ==  9 && pDX08->iBtm2PickerUp09 && !pDX08->iBtm2PickerDown09) return TRUE;
#endif
	return FALSE;
}

void CCommon::Set_Btm2PickerDown()
{
 	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();

#ifdef EDITION_2ND
	pDY07->oBtm2PickerDown01 = pDY08->oBtm2PickerDown05 = TRUE;
	pDY07->oBtm2PickerDown02 = pDY08->oBtm2PickerDown06 = TRUE;
	pDY07->oBtm2PickerDown03 = pDY08->oBtm2PickerDown07 = TRUE;
	pDY07->oBtm2PickerDown04 = pDY08->oBtm2PickerDown08 = TRUE;
#else
	pDY07->oBtm2PickerDown01 = TRUE; pDY07->oBtm2PickerDown02 = TRUE;
	pDY07->oBtm2PickerDown06 = TRUE; pDY07->oBtm2PickerDown07 = TRUE;
#endif
	g_objAJinAXL.Write_Output(7);
	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_Btm2PickerDown(int nSub)
{
#ifdef EDITION_2ND
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();

	if (nSub == 0) {
		if (!pDX07->iBtm2PickerUp01 && pDX07->iBtm2PickerDown01 &&
			!pDX07->iBtm2PickerUp02 && pDX07->iBtm2PickerDown02 &&
			!pDX07->iBtm2PickerUp03 && pDX07->iBtm2PickerDown03 &&
			!pDX07->iBtm2PickerUp04 && pDX07->iBtm2PickerDown04 &&
			!pDX08->iBtm2PickerUp05 && pDX08->iBtm2PickerDown05 &&
			!pDX08->iBtm2PickerUp06 && pDX08->iBtm2PickerDown06 &&
			!pDX08->iBtm2PickerUp07 && pDX08->iBtm2PickerDown07 &&
			!pDX08->iBtm2PickerUp08 && pDX08->iBtm2PickerDown08) return TRUE;
	}
	else if (nSub ==  1 && !pDX07->iBtm2PickerUp01 && pDX07->iBtm2PickerDown01) return TRUE;
	else if (nSub ==  2 && !pDX07->iBtm2PickerUp02 && pDX07->iBtm2PickerDown02) return TRUE;
	else if (nSub ==  3 && !pDX07->iBtm2PickerUp03 && pDX07->iBtm2PickerDown03) return TRUE;
	else if (nSub ==  4 && !pDX07->iBtm2PickerUp04 && pDX07->iBtm2PickerDown04) return TRUE;
	else if (nSub ==  5 && !pDX08->iBtm2PickerUp05 && pDX08->iBtm2PickerDown05) return TRUE;
	else if (nSub ==  6 && !pDX08->iBtm2PickerUp06 && pDX08->iBtm2PickerDown06) return TRUE;
	else if (nSub ==  7 && !pDX08->iBtm2PickerUp07 && pDX08->iBtm2PickerDown07) return TRUE;
	else if (nSub ==  8 && !pDX08->iBtm2PickerUp08 && pDX08->iBtm2PickerDown08) return TRUE;
	return FALSE;
#else
	return TRUE;
#endif
}

void CCommon::Set_Btm2PickerVacOff(int nSub)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	if (nSub == 0 || nSub ==  1) { pDY08->oBtm2PickerVac01 = FALSE; pDY08->oBtm2PickerAir01 = TRUE; }
	if (nSub == 0 || nSub ==  2) { pDY08->oBtm2PickerVac02 = FALSE; pDY08->oBtm2PickerAir02 = TRUE; }
	if (nSub == 0 || nSub ==  3) { pDY08->oBtm2PickerVac03 = FALSE; pDY08->oBtm2PickerAir03 = TRUE; }
	if (nSub == 0 || nSub ==  4) { pDY08->oBtm2PickerVac04 = FALSE; pDY08->oBtm2PickerAir04 = TRUE; }
	if (nSub == 0 || nSub ==  5) { pDY08->oBtm2PickerVac05 = FALSE; pDY08->oBtm2PickerAir05 = TRUE; }
	if (nSub == 0 || nSub ==  6) { pDY08->oBtm2PickerVac06 = FALSE; pDY08->oBtm2PickerAir06 = TRUE; }
	if (nSub == 0 || nSub ==  7) { pDY08->oBtm2PickerVac07 = FALSE; pDY08->oBtm2PickerAir07 = TRUE; }
	if (nSub == 0 || nSub ==  8) { pDY08->oBtm2PickerVac08 = FALSE; pDY08->oBtm2PickerAir08 = TRUE; }
	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_Btm2PickerVacOff(int nSub)
{
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	if (nSub == 0) {
		if (!pDX08->iBtm2PickerVac01 && !pDX08->iBtm2PickerVac02 &&
			!pDX08->iBtm2PickerVac03 && !pDX08->iBtm2PickerVac04 &&
			!pDX08->iBtm2PickerVac05 && !pDX08->iBtm2PickerVac06 &&
			!pDX08->iBtm2PickerVac07 && !pDX08->iBtm2PickerVac08) return TRUE;
	}
	else if (nSub ==  1 && !pDX08->iBtm2PickerVac01) return TRUE;
	else if (nSub ==  2 && !pDX08->iBtm2PickerVac02) return TRUE;
	else if (nSub ==  3 && !pDX08->iBtm2PickerVac03) return TRUE;
	else if (nSub ==  4 && !pDX08->iBtm2PickerVac04) return TRUE;
	else if (nSub ==  5 && !pDX08->iBtm2PickerVac05) return TRUE;
	else if (nSub ==  6 && !pDX08->iBtm2PickerVac06) return TRUE;
	else if (nSub ==  7 && !pDX08->iBtm2PickerVac07) return TRUE;
	else if (nSub ==  8 && !pDX08->iBtm2PickerVac08) return TRUE;
	return FALSE;
}

void CCommon::Set_Btm2PickerAirOff(int nSub)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	if (nSub == 0 || nSub ==  1) pDY08->oBtm2PickerAir01 = FALSE;
	if (nSub == 0 || nSub ==  2) pDY08->oBtm2PickerAir02 = FALSE;
	if (nSub == 0 || nSub ==  3) pDY08->oBtm2PickerAir03 = FALSE;
	if (nSub == 0 || nSub ==  4) pDY08->oBtm2PickerAir04 = FALSE;
	if (nSub == 0 || nSub ==  5) pDY08->oBtm2PickerAir05 = FALSE;
	if (nSub == 0 || nSub ==  6) pDY08->oBtm2PickerAir06 = FALSE;
	if (nSub == 0 || nSub ==  7) pDY08->oBtm2PickerAir07 = FALSE;
	if (nSub == 0 || nSub ==  8) pDY08->oBtm2PickerAir08 = FALSE;
	g_objAJinAXL.Write_Output(8);
}

void CCommon::Set_Btm2PickerOpen(int nSub, int nRow)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	if (nSub == 0 || nSub ==  1 || nRow == 0) { pDY07->oBtm2PickerOpen01 = TRUE; pDY07->oBtm2PickerClose01 = FALSE; }
	if (nSub == 0 || nSub ==  2 || nRow == 0) { pDY07->oBtm2PickerOpen02 = TRUE; pDY07->oBtm2PickerClose02 = FALSE; }
	if (nSub == 0 || nSub ==  3 || nRow == 0) { pDY07->oBtm2PickerOpen03 = TRUE; pDY07->oBtm2PickerClose03 = FALSE; }
	if (nSub == 0 || nSub ==  4 || nRow == 0) { pDY07->oBtm2PickerOpen04 = TRUE; pDY07->oBtm2PickerClose04 = FALSE; }
	if (nSub == 0 || nSub ==  5 || nRow == 1) { pDY07->oBtm2PickerOpen05 = TRUE; pDY07->oBtm2PickerClose05 = FALSE; }
	if (nSub == 0 || nSub ==  6 || nRow == 1) { pDY07->oBtm2PickerOpen06 = TRUE; pDY07->oBtm2PickerClose06 = FALSE; }
	if (nSub == 0 || nSub ==  7 || nRow == 1) { pDY07->oBtm2PickerOpen07 = TRUE; pDY07->oBtm2PickerClose07 = FALSE; }
	if (nSub == 0 || nSub ==  8 || nRow == 1) { pDY07->oBtm2PickerOpen08 = TRUE; pDY07->oBtm2PickerClose08 = FALSE; }
	g_objAJinAXL.Write_Output(7);
}

void CCommon::Set_Btm2PickerOpen(int nSub)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	if (nSub == 0 || nSub ==  1 ) { pDY07->oBtm2PickerOpen01 = TRUE; pDY07->oBtm2PickerClose01 = FALSE; }
	if (nSub == 0 || nSub ==  2 ) { pDY07->oBtm2PickerOpen02 = TRUE; pDY07->oBtm2PickerClose02 = FALSE; }
	if (nSub == 0 || nSub ==  3 ) { pDY07->oBtm2PickerOpen03 = TRUE; pDY07->oBtm2PickerClose03 = FALSE; }
	if (nSub == 0 || nSub ==  4 ) { pDY07->oBtm2PickerOpen04 = TRUE; pDY07->oBtm2PickerClose04 = FALSE; }
	if (nSub == 0 || nSub ==  5 ) { pDY07->oBtm2PickerOpen05 = TRUE; pDY07->oBtm2PickerClose05 = FALSE; }
	if (nSub == 0 || nSub ==  6 ) { pDY07->oBtm2PickerOpen06 = TRUE; pDY07->oBtm2PickerClose06 = FALSE; }
	if (nSub == 0 || nSub ==  7 ) { pDY07->oBtm2PickerOpen07 = TRUE; pDY07->oBtm2PickerClose07 = FALSE; }
	if (nSub == 0 || nSub ==  8 ) { pDY07->oBtm2PickerOpen08 = TRUE; pDY07->oBtm2PickerClose08 = FALSE; }
	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_Btm2PickerOpen(int nSub, int nRow)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	if (nSub == 0) {
		if (pDX07->iBtm2PickerOpen01 && pDX07->iBtm2PickerOpen02 &&
			pDX07->iBtm2PickerOpen03 && pDX07->iBtm2PickerOpen04 &&
			pDX07->iBtm2PickerOpen05 && pDX07->iBtm2PickerOpen06 &&
			pDX07->iBtm2PickerOpen07 && pDX07->iBtm2PickerOpen08) return TRUE;
	}
	BOOL bRet = TRUE;
	if ((nSub ==  1 || nRow == 0) && !pDX07->iBtm2PickerOpen01) bRet = FALSE;
	if ((nSub ==  2 || nRow == 0) && !pDX07->iBtm2PickerOpen02) bRet = FALSE;
	if ((nSub ==  3 || nRow == 0) && !pDX07->iBtm2PickerOpen03) bRet = FALSE;
	if ((nSub ==  4 || nRow == 0) && !pDX07->iBtm2PickerOpen04) bRet = FALSE;
	if ((nSub ==  5 || nRow == 1) && !pDX07->iBtm2PickerOpen05) bRet = FALSE;
	if ((nSub ==  6 || nRow == 1) && !pDX07->iBtm2PickerOpen06) bRet = FALSE;
	if ((nSub ==  7 || nRow == 1) && !pDX07->iBtm2PickerOpen07) bRet = FALSE;
	if ((nSub ==  8 || nRow == 1) && !pDX07->iBtm2PickerOpen08) bRet = FALSE;

	return bRet;
}

BOOL CCommon::Get_Btm2PickerOpen(int nSub)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	if (nSub == 0) {
		if (pDX07->iBtm2PickerOpen01 && pDX07->iBtm2PickerOpen02 &&
			pDX07->iBtm2PickerOpen03 && pDX07->iBtm2PickerOpen04 &&
			pDX07->iBtm2PickerOpen05 && pDX07->iBtm2PickerOpen06 &&
			pDX07->iBtm2PickerOpen07 && pDX07->iBtm2PickerOpen08) return TRUE;
	}
	BOOL bRet = TRUE;
	if ((nSub ==  1 ) && !pDX07->iBtm2PickerOpen01) bRet = FALSE;
	if ((nSub ==  2 ) && !pDX07->iBtm2PickerOpen02) bRet = FALSE;
	if ((nSub ==  3 ) && !pDX07->iBtm2PickerOpen03) bRet = FALSE;
	if ((nSub ==  4 ) && !pDX07->iBtm2PickerOpen04) bRet = FALSE;
	if ((nSub ==  5 ) && !pDX07->iBtm2PickerOpen05) bRet = FALSE;
	if ((nSub ==  6 ) && !pDX07->iBtm2PickerOpen06) bRet = FALSE;
	if ((nSub ==  7 ) && !pDX07->iBtm2PickerOpen07) bRet = FALSE;
	if ((nSub ==  8 ) && !pDX07->iBtm2PickerOpen08) bRet = FALSE;

	return bRet;
}



void CCommon::Set_Btm2PickerClose(int nSub)
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	if (nSub == 0 || nSub ==  1 ) { pDY07->oBtm2PickerOpen01 = FALSE; pDY07->oBtm2PickerClose01 = TRUE; }
	if (nSub == 0 || nSub ==  2 ) { pDY07->oBtm2PickerOpen02 = FALSE; pDY07->oBtm2PickerClose02 = TRUE; }
	if (nSub == 0 || nSub ==  3 ) { pDY07->oBtm2PickerOpen03 = FALSE; pDY07->oBtm2PickerClose03 = TRUE; }
	if (nSub == 0 || nSub ==  4 ) { pDY07->oBtm2PickerOpen04 = FALSE; pDY07->oBtm2PickerClose04 = TRUE; }
	if (nSub == 0 || nSub ==  5 ) { pDY07->oBtm2PickerOpen05 = FALSE; pDY07->oBtm2PickerClose05 = TRUE; }
	if (nSub == 0 || nSub ==  6 ) { pDY07->oBtm2PickerOpen06 = FALSE; pDY07->oBtm2PickerClose06 = TRUE; }
	if (nSub == 0 || nSub ==  7 ) { pDY07->oBtm2PickerOpen07 = FALSE; pDY07->oBtm2PickerClose07 = TRUE; }
	if (nSub == 0 || nSub ==  8 ) { pDY07->oBtm2PickerOpen08 = FALSE; pDY07->oBtm2PickerClose08 = TRUE; }
	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_Btm2PickerClose(int nSub)
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	if (nSub == 0) {
		if (pDX07->iBtm2PickerExist01 && pDX07->iBtm2PickerExist02 &&
			pDX07->iBtm2PickerExist03 && pDX07->iBtm2PickerExist04 &&
			pDX07->iBtm2PickerExist05 && pDX07->iBtm2PickerExist06 &&
			pDX07->iBtm2PickerExist07 && pDX07->iBtm2PickerExist08) return TRUE;
	}
	BOOL bRet = TRUE;
	if ((nSub ==  1 ) && !pDX07->iBtm2PickerExist01) bRet = FALSE;
	if ((nSub ==  2 ) && !pDX07->iBtm2PickerExist02) bRet = FALSE;
	if ((nSub ==  3 ) && !pDX07->iBtm2PickerExist03) bRet = FALSE;
	if ((nSub ==  4 ) && !pDX07->iBtm2PickerExist04) bRet = FALSE;
	if ((nSub ==  5 ) && !pDX07->iBtm2PickerExist05) bRet = FALSE;
	if ((nSub ==  6 ) && !pDX07->iBtm2PickerExist06) bRet = FALSE;
	if ((nSub ==  7 ) && !pDX07->iBtm2PickerExist07) bRet = FALSE;
	if ((nSub ==  8 ) && !pDX07->iBtm2PickerExist08) bRet = FALSE;

	return bRet;
}



BOOL CCommon::Get_Btm2PickerAllNotChk()
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	if (!pDX07->iBtm2PickerExist01 && !pDX07->iBtm2PickerExist02 && !pDX07->iBtm2PickerExist03 && !pDX07->iBtm2PickerExist04 &&
		!pDX07->iBtm2PickerExist05 && !pDX07->iBtm2PickerExist06 && !pDX07->iBtm2PickerExist07 && !pDX07->iBtm2PickerExist08) return TRUE;
	return FALSE;
}

void CCommon::Set_Btm2PickerVacOffMulti(int nSub, int nCnt, int nRow)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();

	if (nRow == 0) {
		if (nSub == 1) {
			if (nCnt > 0) { pDY08->oBtm2PickerVac01 = FALSE; pDY08->oBtm2PickerAir01 = TRUE; }
			if (nCnt > 1) { pDY08->oBtm2PickerVac02 = FALSE; pDY08->oBtm2PickerAir02 = TRUE; }
			if (nCnt > 2) { pDY08->oBtm2PickerVac03 = FALSE; pDY08->oBtm2PickerAir03 = TRUE; }
			if (nCnt > 3) { pDY08->oBtm2PickerVac04 = FALSE; pDY08->oBtm2PickerAir04 = TRUE; }
		}
		else if (nSub == 2) {
			if (nCnt > 0) { pDY08->oBtm2PickerVac02 = FALSE; pDY08->oBtm2PickerAir02 = TRUE; }
			if (nCnt > 1) { pDY08->oBtm2PickerVac03 = FALSE; pDY08->oBtm2PickerAir03 = TRUE; }
			if (nCnt > 2) { pDY08->oBtm2PickerVac04 = FALSE; pDY08->oBtm2PickerAir04 = TRUE; }
		}
		else if (nSub == 3) {
			if (nCnt > 0) { pDY08->oBtm2PickerVac03 = FALSE; pDY08->oBtm2PickerAir03 = TRUE; }
			if (nCnt > 1) { pDY08->oBtm2PickerVac04 = FALSE; pDY08->oBtm2PickerAir04 = TRUE; }
		}
		else if (nSub == 4) {
			if (nCnt > 0) { pDY08->oBtm2PickerVac04 = FALSE; pDY08->oBtm2PickerAir04 = TRUE; }
		}
	} 
	else if (nRow == 1) {
		if (nSub == 1) {
			if (nCnt > 0) { pDY08->oBtm2PickerVac05 = FALSE; pDY08->oBtm2PickerAir05 = TRUE; }
			if (nCnt > 1) { pDY08->oBtm2PickerVac06 = FALSE; pDY08->oBtm2PickerAir06 = TRUE; }
			if (nCnt > 2) { pDY08->oBtm2PickerVac07 = FALSE; pDY08->oBtm2PickerAir07 = TRUE; }
			if (nCnt > 3) { pDY08->oBtm2PickerVac08 = FALSE; pDY08->oBtm2PickerAir08 = TRUE; }
		}													 
		else if (nSub == 2) {								 
			if (nCnt > 0) { pDY08->oBtm2PickerVac06 = FALSE; pDY08->oBtm2PickerAir06 = TRUE; }
			if (nCnt > 1) { pDY08->oBtm2PickerVac07 = FALSE; pDY08->oBtm2PickerAir07 = TRUE; }
			if (nCnt > 2) { pDY08->oBtm2PickerVac08 = FALSE; pDY08->oBtm2PickerAir08 = TRUE; }
		}													 
		else if (nSub == 3) {								 
			if (nCnt > 0) { pDY08->oBtm2PickerVac07 = FALSE; pDY08->oBtm2PickerAir07 = TRUE; }
			if (nCnt > 1) { pDY08->oBtm2PickerVac08 = FALSE; pDY08->oBtm2PickerAir08 = TRUE; }
		}													 
		else if (nSub == 4) {								 
			if (nCnt > 0) { pDY08->oBtm2PickerVac08 = FALSE; pDY08->oBtm2PickerAir08 = TRUE; }
		}													 
	}
	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_Btm2PickerVacOffMulti(int nSub, int nCnt, int nRow)
{
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();

	if (nRow == 0) {
		if (nSub == 1) {
			if (nCnt > 0 && pDX08->iBtm2PickerVac01) return FALSE;
			if (nCnt > 1 && pDX08->iBtm2PickerVac02) return FALSE;
			if (nCnt > 2 && pDX08->iBtm2PickerVac03) return FALSE;
			if (nCnt > 3 && pDX08->iBtm2PickerVac04) return FALSE;
		}
		else if (nSub == 2) {
			if (nCnt > 0 && pDX08->iBtm2PickerVac02) return FALSE;
			if (nCnt > 1 && pDX08->iBtm2PickerVac03) return FALSE;
			if (nCnt > 2 && pDX08->iBtm2PickerVac04) return FALSE;
		}
		else if (nSub == 3) {
			if (nCnt > 0 && pDX08->iBtm2PickerVac03) return FALSE;
			if (nCnt > 1 && pDX08->iBtm2PickerVac04) return FALSE;
		}
		else if (nSub == 4) {
			if (nCnt > 0 && pDX08->iBtm2PickerVac04) return FALSE;
		}
	}
	else if (nRow == 1){
		if (nSub == 1) {
			if (nCnt > 0 && pDX08->iBtm2PickerVac05) return FALSE;
			if (nCnt > 1 && pDX08->iBtm2PickerVac06) return FALSE;
			if (nCnt > 2 && pDX08->iBtm2PickerVac07) return FALSE;
			if (nCnt > 3 && pDX08->iBtm2PickerVac08) return FALSE;
		}
		else if (nSub == 2) {
			if (nCnt > 0 && pDX08->iBtm2PickerVac06) return FALSE;
			if (nCnt > 1 && pDX08->iBtm2PickerVac07) return FALSE;
			if (nCnt > 2 && pDX08->iBtm2PickerVac08) return FALSE;
		}
		else if (nSub == 3) {
			if (nCnt > 0 && pDX08->iBtm2PickerVac07) return FALSE;
			if (nCnt > 1 && pDX08->iBtm2PickerVac08) return FALSE;
		}
		else if (nSub == 4) {
			if (nCnt > 0 && pDX08->iBtm2PickerVac08) return FALSE;
		}
	}
	return TRUE;
}

void CCommon::Set_Btm2PickerAirOffMulti(int nSub, int nCnt, int nRow)
{
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();

	if (nRow == 0) {
		if (nSub == 1) {
			if (nCnt > 0) pDY08->oBtm2PickerAir01 = FALSE;
			if (nCnt > 1) pDY08->oBtm2PickerAir02 = FALSE;
			if (nCnt > 2) pDY08->oBtm2PickerAir03 = FALSE;
			if (nCnt > 3) pDY08->oBtm2PickerAir04 = FALSE;
		}
		else if (nSub == 2) {
			if (nCnt > 0) pDY08->oBtm2PickerAir02 = FALSE;
			if (nCnt > 1) pDY08->oBtm2PickerAir03 = FALSE;
			if (nCnt > 2) pDY08->oBtm2PickerAir04 = FALSE;
		}
		else if (nSub == 3) {
			if (nCnt > 0) pDY08->oBtm2PickerAir03 = FALSE;
			if (nCnt > 1) pDY08->oBtm2PickerAir04 = FALSE;
		}
		else if (nSub == 4) {
			if (nCnt > 0) pDY08->oBtm2PickerAir04 = FALSE;
		}
	} 
	else if (nRow == 1) {
		if (nSub == 1) {
			if (nCnt > 0) pDY08->oBtm2PickerAir05 = FALSE;
			if (nCnt > 1) pDY08->oBtm2PickerAir06 = FALSE;
			if (nCnt > 2) pDY08->oBtm2PickerAir07 = FALSE;
			if (nCnt > 3) pDY08->oBtm2PickerAir08 = FALSE;
		}
		else if (nSub == 2) {
			if (nCnt > 0) pDY08->oBtm2PickerAir06 = FALSE;
			if (nCnt > 1) pDY08->oBtm2PickerAir07 = FALSE;
			if (nCnt > 2) pDY08->oBtm2PickerAir08 = FALSE;
		}
		else if (nSub == 3) {
			if (nCnt > 0) pDY08->oBtm2PickerAir07 = FALSE;
			if (nCnt > 1) pDY08->oBtm2PickerAir08 = FALSE;
		}
		else if (nSub == 4) {
			if (nCnt > 0) pDY08->oBtm2PickerAir08 = FALSE;
		}
	}
	g_objAJinAXL.Write_Output(8);
}

// 7. Sort Picker1 I/O 함수
void CCommon::Set_SortPicker1Up(int nSub)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	if (nSub == 0 || nSub == 1) pDY10->oSortPicker1Down1 = FALSE;
	if (nSub == 0 || nSub == 2) pDY10->oSortPicker1Down2 = FALSE;
	if (nSub == 0 || nSub == 3) pDY10->oSortPicker1Down3 = FALSE;
	if (nSub == 0 || nSub == 4) pDY10->oSortPicker1Down4 = FALSE;
	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_SortPicker1Up(int nSub)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	if (nSub == 0) {
		if (pDX10->iSortPicker1Up1 && !pDX10->iSortPicker1Down1 &&
			pDX10->iSortPicker1Up2 && !pDX10->iSortPicker1Down2 &&
			pDX10->iSortPicker1Up3 && !pDX10->iSortPicker1Down3 &&
			pDX10->iSortPicker1Up4 && !pDX10->iSortPicker1Down4) return TRUE;
	}
	else if (nSub == 1 && pDX10->iSortPicker1Up1 && !pDX10->iSortPicker1Down1) return TRUE;
	else if (nSub == 2 && pDX10->iSortPicker1Up2 && !pDX10->iSortPicker1Down2) return TRUE;
	else if (nSub == 3 && pDX10->iSortPicker1Up3 && !pDX10->iSortPicker1Down3) return TRUE;
	else if (nSub == 4 && pDX10->iSortPicker1Up4 && !pDX10->iSortPicker1Down4) return TRUE;
	return FALSE;
}

void CCommon::Set_SortPicker1Down(int nSub)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();

	if (nSub == 0 || nSub == 1) pDY10->oSortPicker1Down1 = TRUE;
	if (nSub == 0 || nSub == 2) pDY10->oSortPicker1Down2 = TRUE;
	if (nSub == 0 || nSub == 3) pDY10->oSortPicker1Down3 = TRUE;
	if (nSub == 0 || nSub == 4) pDY10->oSortPicker1Down4 = TRUE;
	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_SortPicker1Down(int nSub)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();

	if (nSub == 0) {
		if (!pDX10->iSortPicker1Up1 && pDX10->iSortPicker1Down1 &&
			!pDX10->iSortPicker1Up2 && pDX10->iSortPicker1Down2 &&
			!pDX10->iSortPicker1Up3 && pDX10->iSortPicker1Down3 &&
			!pDX10->iSortPicker1Up4 && pDX10->iSortPicker1Down4) return TRUE;
	}
	else if (nSub == 1 && !pDX10->iSortPicker1Up1 && pDX10->iSortPicker1Down1) return TRUE;
	else if (nSub == 2 && !pDX10->iSortPicker1Up2 && pDX10->iSortPicker1Down2) return TRUE;
	else if (nSub == 3 && !pDX10->iSortPicker1Up3 && pDX10->iSortPicker1Down3) return TRUE;
	else if (nSub == 4 && !pDX10->iSortPicker1Up4 && pDX10->iSortPicker1Down4) return TRUE;
	
	return FALSE;
}

void CCommon::Set_SortPicker1Open(int nSub)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	if (nSub == 0 || nSub ==  1) { pDY10->oSortPicker1Open1 = TRUE; pDY10->oSortPicker1Close1 = FALSE; }
	if (nSub == 0 || nSub ==  2) { pDY10->oSortPicker1Open2 = TRUE; pDY10->oSortPicker1Close2 = FALSE; }
	if (nSub == 0 || nSub ==  3) { pDY10->oSortPicker1Open3 = TRUE; pDY10->oSortPicker1Close3 = FALSE; }
	if (nSub == 0 || nSub ==  4) { pDY10->oSortPicker1Open4 = TRUE; pDY10->oSortPicker1Close4 = FALSE; }
	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_SortPicker1Open(int nSub)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	if (nSub == 0) {
		if (pDX10->iSortPicker1Open1 && pDX10->iSortPicker1Open2 &&
			pDX10->iSortPicker1Open3 && pDX10->iSortPicker1Open4) return TRUE;
	}
	else if (nSub ==  1 && pDX10->iSortPicker1Open1) return TRUE;
	else if (nSub ==  2 && pDX10->iSortPicker1Open2) return TRUE;
	else if (nSub ==  3 && pDX10->iSortPicker1Open3) return TRUE;
	else if (nSub ==  4 && pDX10->iSortPicker1Open4) return TRUE;
	return FALSE;
}


void CCommon::Set_SortPicker1Close(int nSub)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	if (nSub == 0 || nSub ==  1) { pDY10->oSortPicker1Open1 = FALSE; pDY10->oSortPicker1Close1 = TRUE; }
	if (nSub == 0 || nSub ==  2) { pDY10->oSortPicker1Open2 = FALSE; pDY10->oSortPicker1Close2 = TRUE; }
	if (nSub == 0 || nSub ==  3) { pDY10->oSortPicker1Open3 = FALSE; pDY10->oSortPicker1Close3 = TRUE; }
	if (nSub == 0 || nSub ==  4) { pDY10->oSortPicker1Open4 = FALSE; pDY10->oSortPicker1Close4 = TRUE; }
	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_SortPicker1Close(int nSub)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	if (nSub == 0) {
		if (pDX10->iSortPicker1Exist1 && pDX10->iSortPicker1Exist2 &&
			pDX10->iSortPicker1Exist3 && pDX10->iSortPicker1Exist4) return TRUE;
	}
	else if (nSub ==  1 && pDX10->iSortPicker1Exist1) return TRUE;
	else if (nSub ==  2 && pDX10->iSortPicker1Exist2) return TRUE;
	else if (nSub ==  3 && pDX10->iSortPicker1Exist3) return TRUE;
	else if (nSub ==  4 && pDX10->iSortPicker1Exist4) return TRUE;
	return FALSE;
}

BOOL CCommon::Get_SortPicker1AllNotChk()
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	if (!pDX10->iSortPicker1Exist1 && !pDX10->iSortPicker1Exist2 && !pDX10->iSortPicker1Exist3 && !pDX10->iSortPicker1Exist4) return TRUE;
	return FALSE;
}

void CCommon::Set_SortPicker1DownMulti(int nSub, int nCnt)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();

	if (nSub == 1) {
		if (nCnt > 0) pDY10->oSortPicker1Down1 = TRUE;
		if (nCnt > 1) pDY10->oSortPicker1Down2 = TRUE;
		if (nCnt > 2) pDY10->oSortPicker1Down3 = TRUE;
		if (nCnt > 3) pDY10->oSortPicker1Down4 = TRUE;
	}
	else if (nSub == 2) {
		if (nCnt > 0) pDY10->oSortPicker1Down2 = TRUE;
		if (nCnt > 1) pDY10->oSortPicker1Down3 = TRUE;
		if (nCnt > 2) pDY10->oSortPicker1Down4 = TRUE;
	}
	else if (nSub == 3) {
		if (nCnt > 0) pDY10->oSortPicker1Down3 = TRUE;
		if (nCnt > 1) pDY10->oSortPicker1Down4 = TRUE;
	}
	else if (nSub == 4) {
		if (nCnt > 0) pDY10->oSortPicker1Down4 = TRUE;
	}
	
	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_SortPicker1DownMulti(int nSub, int nCnt)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();

	if (nSub == 1) {
		if (nCnt > 0 && (pDX10->iSortPicker1Up1 || !pDX10->iSortPicker1Down1)) return FALSE;
		if (nCnt > 1 && (pDX10->iSortPicker1Up2 || !pDX10->iSortPicker1Down2)) return FALSE;
		if (nCnt > 2 && (pDX10->iSortPicker1Up3 || !pDX10->iSortPicker1Down3)) return FALSE;
		if (nCnt > 3 && (pDX10->iSortPicker1Up4 || !pDX10->iSortPicker1Down4)) return FALSE;
	}
	else if (nSub == 2) {
		if (nCnt > 0 && (pDX10->iSortPicker1Up2 || !pDX10->iSortPicker1Down2)) return FALSE;
		if (nCnt > 1 && (pDX10->iSortPicker1Up3 || !pDX10->iSortPicker1Down3)) return FALSE;
		if (nCnt > 2 && (pDX10->iSortPicker1Up4 || !pDX10->iSortPicker1Down4)) return FALSE;
	}
	else if (nSub == 3) {
		if (nCnt > 0 && (pDX10->iSortPicker1Up3 || !pDX10->iSortPicker1Down3)) return FALSE;
		if (nCnt > 1 && (pDX10->iSortPicker1Up4 || !pDX10->iSortPicker1Down4)) return FALSE;
	}
	else if (nSub == 4) {
		if (nCnt > 0 && (pDX10->iSortPicker1Up4 || !pDX10->iSortPicker1Down4)) return FALSE;
	}
	
	return TRUE;
}

void CCommon::Set_SortPicker1OpenMulti(int nSub, int nCnt)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();

	if (nSub == 1) {
		if (nCnt > 0) { pDY10->oSortPicker1Open1 = TRUE; pDY10->oSortPicker1Close1 = FALSE; }
		if (nCnt > 1) { pDY10->oSortPicker1Open2 = TRUE; pDY10->oSortPicker1Close2 = FALSE; }
		if (nCnt > 2) { pDY10->oSortPicker1Open3 = TRUE; pDY10->oSortPicker1Close3 = FALSE; }
		if (nCnt > 3) { pDY10->oSortPicker1Open4 = TRUE; pDY10->oSortPicker1Close4 = FALSE; }
	}
	else if (nSub == 2) {
		if (nCnt > 0) { pDY10->oSortPicker1Open2 = TRUE; pDY10->oSortPicker1Close2 = FALSE; }
		if (nCnt > 1) { pDY10->oSortPicker1Open3 = TRUE; pDY10->oSortPicker1Close3 = FALSE; }
		if (nCnt > 2) { pDY10->oSortPicker1Open4 = TRUE; pDY10->oSortPicker1Close4 = FALSE; }
	}
	else if (nSub == 3) {
		if (nCnt > 0) { pDY10->oSortPicker1Open3 = TRUE; pDY10->oSortPicker1Close3 = FALSE; }
		if (nCnt > 1) { pDY10->oSortPicker1Open4 = TRUE; pDY10->oSortPicker1Close4 = FALSE; }
	}
	else if (nSub == 4) {
		if (nCnt > 0) { pDY10->oSortPicker1Open4 = TRUE; pDY10->oSortPicker1Close4 = FALSE; }
	}
	
	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_SortPicker1OpenMulti(int nSub, int nCnt)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();

	if (nSub == 1) {
		if (nCnt > 0 && !pDX10->iSortPicker1Open1) return FALSE;
		if (nCnt > 1 && !pDX10->iSortPicker1Open2) return FALSE;
		if (nCnt > 2 && !pDX10->iSortPicker1Open3) return FALSE;
		if (nCnt > 3 && !pDX10->iSortPicker1Open4) return FALSE;
	}
	else if (nSub == 2) {
		if (nCnt > 0 && !pDX10->iSortPicker1Open2) return FALSE;
		if (nCnt > 1 && !pDX10->iSortPicker1Open3) return FALSE;
		if (nCnt > 2 && !pDX10->iSortPicker1Open4) return FALSE;
	}
	else if (nSub == 3) {
		if (nCnt > 0 && !pDX10->iSortPicker1Open3) return FALSE;
		if (nCnt > 1 && !pDX10->iSortPicker1Open4) return FALSE;
	}
	else if (nSub == 4) {
		if (nCnt > 0 && !pDX10->iSortPicker1Open4) return FALSE;
	}
	
	return TRUE;
}

void CCommon::Set_SortPicker1CloseMulti(int nSub, int nCnt)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();

	if (nSub == 1) {
		if (nCnt > 0) { pDY10->oSortPicker1Open1 = FALSE; pDY10->oSortPicker1Close1 = TRUE; }
		if (nCnt > 1) { pDY10->oSortPicker1Open2 = FALSE; pDY10->oSortPicker1Close2 = TRUE; }
		if (nCnt > 2) { pDY10->oSortPicker1Open3 = FALSE; pDY10->oSortPicker1Close3 = TRUE; }
		if (nCnt > 3) { pDY10->oSortPicker1Open4 = FALSE; pDY10->oSortPicker1Close4 = TRUE; }
	}
	else if (nSub == 2) {
		if (nCnt > 0) { pDY10->oSortPicker1Open2 = FALSE; pDY10->oSortPicker1Close2 = TRUE; }
		if (nCnt > 1) { pDY10->oSortPicker1Open3 = FALSE; pDY10->oSortPicker1Close3 = TRUE; }
		if (nCnt > 2) { pDY10->oSortPicker1Open4 = FALSE; pDY10->oSortPicker1Close4 = TRUE; }
	}
	else if (nSub == 3) {
		if (nCnt > 0) { pDY10->oSortPicker1Open3 = FALSE; pDY10->oSortPicker1Close3 = TRUE; }
		if (nCnt > 1) { pDY10->oSortPicker1Open4 = FALSE; pDY10->oSortPicker1Close4 = TRUE; }
	}
	else if (nSub == 4) {
		if (nCnt > 0) { pDY10->oSortPicker1Open4 = FALSE; pDY10->oSortPicker1Close4 = TRUE; }
	}
	
	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_SortPicker1CloseMulti(int nSub, int nCnt)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();

	if (nSub == 1) {
		if (nCnt > 0 && !pDX10->iSortPicker1Open1) return FALSE;
		if (nCnt > 1 && !pDX10->iSortPicker1Open2) return FALSE;
		if (nCnt > 2 && !pDX10->iSortPicker1Open3) return FALSE;
		if (nCnt > 3 && !pDX10->iSortPicker1Open4) return FALSE;
	}
	else if (nSub == 2) {
		if (nCnt > 0 && !pDX10->iSortPicker1Open2) return FALSE;
		if (nCnt > 1 && !pDX10->iSortPicker1Open3) return FALSE;
		if (nCnt > 2 && !pDX10->iSortPicker1Open4) return FALSE;
	}
	else if (nSub == 3) {
		if (nCnt > 0 && !pDX10->iSortPicker1Open3) return FALSE;
		if (nCnt > 1 && !pDX10->iSortPicker1Open4) return FALSE;
	}
	else if (nSub == 4) {
		if (nCnt > 0 && !pDX10->iSortPicker1Open4) return FALSE;
	}
	
	return TRUE;
}

// 8. Sort Picker2 I/O 함수
void CCommon::Set_SortPicker2Up(int nSub)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();
	if (nSub == 0 || nSub == 1) pDY11->oSortPicker2Down1 = FALSE;
	if (nSub == 0 || nSub == 2) pDY11->oSortPicker2Down2 = FALSE;
	if (nSub == 0 || nSub == 3) pDY11->oSortPicker2Down3 = FALSE;
	if (nSub == 0 || nSub == 4) pDY11->oSortPicker2Down4 = FALSE;
	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_SortPicker2Up(int nSub)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	if (nSub == 0) {
		if (pDX11->iSortPicker2Up1 && !pDX11->iSortPicker2Down1 &&
			pDX11->iSortPicker2Up2 && !pDX11->iSortPicker2Down2 &&
			pDX11->iSortPicker2Up3 && !pDX11->iSortPicker2Down3 &&
			pDX11->iSortPicker2Up4 && !pDX11->iSortPicker2Down4) return TRUE;
	}
	else if (nSub == 1 && pDX11->iSortPicker2Up1 && !pDX11->iSortPicker2Down1) return TRUE;
	else if (nSub == 2 && pDX11->iSortPicker2Up2 && !pDX11->iSortPicker2Down2) return TRUE;
	else if (nSub == 3 && pDX11->iSortPicker2Up3 && !pDX11->iSortPicker2Down3) return TRUE;
	else if (nSub == 4 && pDX11->iSortPicker2Up4 && !pDX11->iSortPicker2Down4) return TRUE;
	return FALSE;
}

void CCommon::Set_SortPicker2Down(int nSub)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	if (nSub == 0 || nSub == 1) pDY11->oSortPicker2Down1 = TRUE;
	if (nSub == 0 || nSub == 2) pDY11->oSortPicker2Down2 = TRUE;
	if (nSub == 0 || nSub == 3) pDY11->oSortPicker2Down3 = TRUE;
	if (nSub == 0 || nSub == 4) pDY11->oSortPicker2Down4 = TRUE;
	
	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_SortPicker2Down(int nSub)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();

	if (nSub == 0) {
		if (!pDX11->iSortPicker2Up1 && pDX11->iSortPicker2Down1 &&
			!pDX11->iSortPicker2Up2 && pDX11->iSortPicker2Down2 &&
			!pDX11->iSortPicker2Up3 && pDX11->iSortPicker2Down3 &&
			!pDX11->iSortPicker2Up4 && pDX11->iSortPicker2Down4) return TRUE;
	}
	else if (nSub == 1 && !pDX11->iSortPicker2Up1 && pDX11->iSortPicker2Down1) return TRUE;
	else if (nSub == 2 && !pDX11->iSortPicker2Up2 && pDX11->iSortPicker2Down2) return TRUE;
	else if (nSub == 3 && !pDX11->iSortPicker2Up3 && pDX11->iSortPicker2Down3) return TRUE;
	else if (nSub == 4 && !pDX11->iSortPicker2Up4 && pDX11->iSortPicker2Down4) return TRUE;
	
	return FALSE;
}

void CCommon::Set_SortPicker2Open(int nSub)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();
	if (nSub == 0 || nSub ==  1) { pDY11->oSortPicker2Open1 = TRUE; pDY11->oSortPicker2Close1 = FALSE; }
	if (nSub == 0 || nSub ==  2) { pDY11->oSortPicker2Open2 = TRUE; pDY11->oSortPicker2Close2 = FALSE; }
	if (nSub == 0 || nSub ==  3) { pDY11->oSortPicker2Open3 = TRUE; pDY11->oSortPicker2Close3 = FALSE; }
	if (nSub == 0 || nSub ==  4) { pDY11->oSortPicker2Open4 = TRUE; pDY11->oSortPicker2Close4 = FALSE; }
	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_SortPicker2Open(int nSub)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	if (nSub == 0) {
		if (pDX11->iSortPicker2Open1 && pDX11->iSortPicker2Open2 &&
			pDX11->iSortPicker2Open3 && pDX11->iSortPicker2Open4) return TRUE;
	}
	else if (nSub ==  1 && pDX11->iSortPicker2Open1) return TRUE;
	else if (nSub ==  2 && pDX11->iSortPicker2Open2) return TRUE;
	else if (nSub ==  3 && pDX11->iSortPicker2Open3) return TRUE;
	else if (nSub ==  4 && pDX11->iSortPicker2Open4) return TRUE;
	return FALSE;
}

void CCommon::Set_SortPicker2Close(int nSub)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();
	if (nSub == 0 || nSub ==  1) { pDY11->oSortPicker2Open1 = FALSE; pDY11->oSortPicker2Close1 = TRUE; }
	if (nSub == 0 || nSub ==  2) { pDY11->oSortPicker2Open2 = FALSE; pDY11->oSortPicker2Close2 = TRUE; }
	if (nSub == 0 || nSub ==  3) { pDY11->oSortPicker2Open3 = FALSE; pDY11->oSortPicker2Close3 = TRUE; }
	if (nSub == 0 || nSub ==  4) { pDY11->oSortPicker2Open4 = FALSE; pDY11->oSortPicker2Close4 = TRUE; }
	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_SortPicker2Close(int nSub)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	if (nSub == 0) {
		if (pDX11->iSortPicker2Exist1 && pDX11->iSortPicker2Exist2 &&
			pDX11->iSortPicker2Exist3 && pDX11->iSortPicker2Exist4) return TRUE;
	}
	else if (nSub ==  1 && pDX11->iSortPicker2Exist1) return TRUE;
	else if (nSub ==  2 && pDX11->iSortPicker2Exist2) return TRUE;
	else if (nSub ==  3 && pDX11->iSortPicker2Exist3) return TRUE;
	else if (nSub ==  4 && pDX11->iSortPicker2Exist4) return TRUE;
	return FALSE;
}



BOOL CCommon::Get_SortPicker2AllNotChk()
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	if (!pDX11->iSortPicker2Exist1 && !pDX11->iSortPicker2Exist2 && !pDX11->iSortPicker2Exist3 && !pDX11->iSortPicker2Exist4) return TRUE;
	return FALSE;
}

void CCommon::Set_SortPicker2DownMulti(int nSub, int nCnt)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	if (nSub == 1) {
		if (nCnt > 0) pDY11->oSortPicker2Down1 = TRUE;
		if (nCnt > 1) pDY11->oSortPicker2Down2 = TRUE;
		if (nCnt > 2) pDY11->oSortPicker2Down3 = TRUE;
		if (nCnt > 3) pDY11->oSortPicker2Down4 = TRUE;
	}
	else if (nSub == 2) {
		if (nCnt > 0) pDY11->oSortPicker2Down2 = TRUE;
		if (nCnt > 1) pDY11->oSortPicker2Down3 = TRUE;
		if (nCnt > 2) pDY11->oSortPicker2Down4 = TRUE;
	}
	else if (nSub == 3) {
		if (nCnt > 0) pDY11->oSortPicker2Down3 = TRUE;
		if (nCnt > 1) pDY11->oSortPicker2Down4 = TRUE;
	}
	else if (nSub == 4) {
		if (nCnt > 0) pDY11->oSortPicker2Down4 = TRUE;
	}
	
	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_SortPicker2DownMulti(int nSub, int nCnt)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();

	if (nSub == 1) {
		if (nCnt > 0 && (pDX11->iSortPicker2Up1 || !pDX11->iSortPicker2Down1)) return FALSE;
		if (nCnt > 1 && (pDX11->iSortPicker2Up2 || !pDX11->iSortPicker2Down2)) return FALSE;
		if (nCnt > 2 && (pDX11->iSortPicker2Up3 || !pDX11->iSortPicker2Down3)) return FALSE;
		if (nCnt > 3 && (pDX11->iSortPicker2Up4 || !pDX11->iSortPicker2Down4)) return FALSE;
	}
	else if (nSub == 2) {
		if (nCnt > 0 && (pDX11->iSortPicker2Up2 || !pDX11->iSortPicker2Down2)) return FALSE;
		if (nCnt > 1 && (pDX11->iSortPicker2Up3 || !pDX11->iSortPicker2Down3)) return FALSE;
		if (nCnt > 2 && (pDX11->iSortPicker2Up4 || !pDX11->iSortPicker2Down4)) return FALSE;
	}
	else if (nSub == 3) {
		if (nCnt > 0 && (pDX11->iSortPicker2Up3 || !pDX11->iSortPicker2Down3)) return FALSE;
		if (nCnt > 1 && (pDX11->iSortPicker2Up4 || !pDX11->iSortPicker2Down4)) return FALSE;
	}
	else if (nSub == 4) {
		if (nCnt > 0 && (pDX11->iSortPicker2Up4 || !pDX11->iSortPicker2Down4)) return FALSE;
	}
	
	return TRUE;
}

void CCommon::Set_SortPicker2OpenMulti(int nSub, int nCnt)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	if (nSub == 1) {
		if (nCnt > 0) { pDY11->oSortPicker2Open1 = TRUE; pDY11->oSortPicker2Close1 = FALSE; }
		if (nCnt > 1) { pDY11->oSortPicker2Open2 = TRUE; pDY11->oSortPicker2Close2 = FALSE; }
		if (nCnt > 2) { pDY11->oSortPicker2Open3 = TRUE; pDY11->oSortPicker2Close3 = FALSE; }
		if (nCnt > 3) { pDY11->oSortPicker2Open4 = TRUE; pDY11->oSortPicker2Close4 = FALSE; }
	}
	else if (nSub == 2) {
		if (nCnt > 0) { pDY11->oSortPicker2Open2 = TRUE; pDY11->oSortPicker2Close2 = FALSE; }
		if (nCnt > 1) { pDY11->oSortPicker2Open3 = TRUE; pDY11->oSortPicker2Close3 = FALSE; }
		if (nCnt > 2) { pDY11->oSortPicker2Open4 = TRUE; pDY11->oSortPicker2Close4 = FALSE; }
	}
	else if (nSub == 3) {
		if (nCnt > 0) { pDY11->oSortPicker2Open3 = TRUE; pDY11->oSortPicker2Close3 = FALSE; }
		if (nCnt > 1) { pDY11->oSortPicker2Open4 = TRUE; pDY11->oSortPicker2Close4 = FALSE; }
	}
	else if (nSub == 4) {
		if (nCnt > 0) { pDY11->oSortPicker2Open4 = TRUE; pDY11->oSortPicker2Close4 = FALSE; }
	}

	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_SortPicker2OpenMulti(int nSub, int nCnt)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();

	if (nSub == 1) {
		if (nCnt > 0 && !pDX11->iSortPicker2Open1) return FALSE;
		if (nCnt > 1 && !pDX11->iSortPicker2Open2) return FALSE;
		if (nCnt > 2 && !pDX11->iSortPicker2Open3) return FALSE;
		if (nCnt > 3 && !pDX11->iSortPicker2Open4) return FALSE;
	}
	else if (nSub == 2) {
		if (nCnt > 0 && !pDX11->iSortPicker2Open2) return FALSE;
		if (nCnt > 1 && !pDX11->iSortPicker2Open3) return FALSE;
		if (nCnt > 2 && !pDX11->iSortPicker2Open4) return FALSE;
	}
	else if (nSub == 3) {
		if (nCnt > 0 && !pDX11->iSortPicker2Open3) return FALSE;
		if (nCnt > 1 && !pDX11->iSortPicker2Open4) return FALSE;
	}
	else if (nSub == 4) {
		if (nCnt > 0 && !pDX11->iSortPicker2Open4) return FALSE;
	}
	
	return TRUE;
}

void CCommon::Set_SortPicker2CloseMulti(int nSub, int nCnt)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	if (nSub == 1) {
		if (nCnt > 0) { pDY11->oSortPicker2Open1 = FALSE; pDY11->oSortPicker2Close1 = TRUE; }
		if (nCnt > 1) { pDY11->oSortPicker2Open2 = FALSE; pDY11->oSortPicker2Close2 = TRUE; }
		if (nCnt > 2) { pDY11->oSortPicker2Open3 = FALSE; pDY11->oSortPicker2Close3 = TRUE; }
		if (nCnt > 3) { pDY11->oSortPicker2Open4 = FALSE; pDY11->oSortPicker2Close4 = TRUE; }
	}
	else if (nSub == 2) {
		if (nCnt > 0) { pDY11->oSortPicker2Open2 = FALSE; pDY11->oSortPicker2Close2 = TRUE; }
		if (nCnt > 1) { pDY11->oSortPicker2Open3 = FALSE; pDY11->oSortPicker2Close3 = TRUE; }
		if (nCnt > 2) { pDY11->oSortPicker2Open4 = FALSE; pDY11->oSortPicker2Close4 = TRUE; }
	}
	else if (nSub == 3) {
		if (nCnt > 0) { pDY11->oSortPicker2Open3 = FALSE; pDY11->oSortPicker2Close3 = TRUE; }
		if (nCnt > 1) { pDY11->oSortPicker2Open4 = FALSE; pDY11->oSortPicker2Close4 = TRUE; }
	}
	else if (nSub == 4) {
		if (nCnt > 0) { pDY11->oSortPicker2Open4 = FALSE; pDY11->oSortPicker2Close4 = TRUE; }
	}
	
	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_SortPicker2CloseMulti(int nSub, int nCnt)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();

	if (nSub == 1) {
		if (nCnt > 0 && !pDX11->iSortPicker2Open1) return FALSE;
		if (nCnt > 1 && !pDX11->iSortPicker2Open2) return FALSE;
		if (nCnt > 2 && !pDX11->iSortPicker2Open3) return FALSE;
		if (nCnt > 3 && !pDX11->iSortPicker2Open4) return FALSE;
	}
	else if (nSub == 2) {
		if (nCnt > 0 && !pDX11->iSortPicker2Open2) return FALSE;
		if (nCnt > 1 && !pDX11->iSortPicker2Open3) return FALSE;
		if (nCnt > 2 && !pDX11->iSortPicker2Open4) return FALSE;
	}
	else if (nSub == 3) {
		if (nCnt > 0 && !pDX11->iSortPicker2Open3) return FALSE;
		if (nCnt > 1 && !pDX11->iSortPicker2Open4) return FALSE;
	}
	else if (nSub == 4) {
		if (nCnt > 0 && !pDX11->iSortPicker2Open4) return FALSE;
	}
	
	return TRUE;
}

// 9. Empty Tray X I/O 함수
void CCommon::Set_EmptyTrayXUp()
{
	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();

	pDY13->oEmptyTrans1Up = TRUE; pDY13->oEmptyTrans1Down = FALSE;
	g_objAJinAXL.Write_Output(13);
}

BOOL CCommon::Get_EmptyTrayXUp()
{
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	if (!pDX13->iEmptyTrans1Up || pDX13->iEmptyTrans1Down) return FALSE;
	return TRUE;
}

void CCommon::Set_EmptyTrayXDown()
{
	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();

	pDY13->oEmptyTrans1Up = FALSE; pDY13->oEmptyTrans1Down = TRUE;
	g_objAJinAXL.Write_Output(13);
}

BOOL CCommon::Get_EmptyTrayXDown()
{
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	if (pDX13->iEmptyTrans1Up || !pDX13->iEmptyTrans1Down) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_EmptyTrayXMasterSlaveIn()
{
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	if (!pDX13->iEmptyTrans1MasterIn || pDX13->iEmptyTrans1MasterOut) return FALSE;
	if (!pDX13->iEmptyTrans1SlaveIn  || pDX13->iEmptyTrans1SlaveOut) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_EmptyTrayXMasterSlaveOut()
{
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	if (pDX13->iEmptyTrans1MasterIn || !pDX13->iEmptyTrans1MasterOut) return FALSE;
	if (pDX13->iEmptyTrans1SlaveIn  || !pDX13->iEmptyTrans1SlaveOut) return FALSE;
	return TRUE;
}

// 10. Empty Tray Y I/O 함수
void CCommon::Set_EmptyTrayYUp()
{
	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();

	pDY13->oEmptyTrans2Up = TRUE; pDY13->oEmptyTrans2Down = FALSE;
	g_objAJinAXL.Write_Output(13);
}

BOOL CCommon::Get_EmptyTrayYUp()
{
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	if (!pDX13->iEmptyTrans2Up || pDX13->iEmptyTrans2Down) return FALSE;
	return TRUE;
}

void CCommon::Set_EmptyTrayYDown()
{
	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();

	pDY13->oEmptyTrans2Up = FALSE; pDY13->oEmptyTrans2Down = TRUE;
	g_objAJinAXL.Write_Output(13);
}

BOOL CCommon::Get_EmptyTrayYDown()
{
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	if (pDX13->iEmptyTrans2Up || !pDX13->iEmptyTrans2Down) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_EmptyTrayYMasterSlaveIn()
{
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	if (!pDX13->iEmptyTrans2MasterIn || pDX13->iEmptyTrans2MasterOut) return FALSE;
	if (!pDX13->iEmptyTrans2SlaveIn  || pDX13->iEmptyTrans2SlaveOut) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_EmptyTrayYMasterSlaveOut()
{
	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();

	if (pDX13->iEmptyTrans2MasterIn || !pDX13->iEmptyTrans2MasterOut) return FALSE;
	if (pDX13->iEmptyTrans2SlaveIn  || !pDX13->iEmptyTrans2SlaveOut) return FALSE;
	return TRUE;
}

// 11. Tray I/O 함수
BOOL CCommon::Get_LoadTrayMasterSlaveIn()
{
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();

	if (!pDX01->iLoadStageMasterIn || pDX01->iLoadStageMasterOut) return FALSE;
	if (!pDX01->iLoadStageSlaveIn  || pDX01->iLoadStageSlaveOut) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_LoadTrayMasterSlaveOut()
{
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();

	if (pDX01->iLoadStageMasterIn || !pDX01->iLoadStageMasterOut) return FALSE;
	if (pDX01->iLoadStageSlaveIn  || !pDX01->iLoadStageSlaveOut) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_GoodTray1MasterSlaveIn()
{
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();

	if (!pDX12->iGoodStage1MasterIn || pDX12->iGoodStage1MasterOut) return FALSE;
	if (!pDX12->iGoodStage1SlaveIn  || pDX12->iGoodStage1SlaveOut) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_GoodTray1MasterSlaveOut()
{
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();

	if (pDX12->iGoodStage1MasterIn || !pDX12->iGoodStage1MasterOut) return FALSE;
	if (pDX12->iGoodStage1SlaveIn  || !pDX12->iGoodStage1SlaveOut) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_GoodTray2MasterSlaveIn()
{
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();

	if (!pDX12->iGoodStage2MasterIn || pDX12->iGoodStage2MasterOut) return FALSE;
	if (!pDX12->iGoodStage2SlaveIn  || pDX12->iGoodStage2SlaveOut) return FALSE;
	return TRUE;
}

BOOL CCommon::Get_GoodTray2MasterSlaveOut()
{
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();

	if (pDX12->iGoodStage2MasterIn || !pDX12->iGoodStage2MasterOut) return FALSE;
	if (pDX12->iGoodStage2SlaveIn  || !pDX12->iGoodStage2SlaveOut) return FALSE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// 1. Info Btm1 Picker I/O 함수

void CCommon::Set_InfoBtm1PickerVacOn(int nPos, int nRow, int nNo, int nLine)
{
#ifdef DRY_RUN_TEST
	return;
#endif
	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();
	int *pInfo = NULL;

	if (nPos == INFO_STAGE) {
		//pInfo = Get_AngleTrayInfo(nNo, nLine);
		if (nRow == 0) { pDY04->oBtm1PickerVac01 = TRUE; pDY04->oBtm1PickerAir01 = FALSE; }
		if (nRow == 0) { pDY04->oBtm1PickerVac02 = TRUE; pDY04->oBtm1PickerAir02 = FALSE; }
		if (nRow == 0) { pDY04->oBtm1PickerVac03 = TRUE; pDY04->oBtm1PickerAir03 = FALSE; }
		if (nRow == 0) { pDY04->oBtm1PickerVac04 = TRUE; pDY04->oBtm1PickerAir04 = FALSE; }
		if (nRow == 1) { pDY04->oBtm1PickerVac05 = TRUE; pDY04->oBtm1PickerAir05 = FALSE; }
		if (nRow == 1) { pDY04->oBtm1PickerVac06 = TRUE; pDY04->oBtm1PickerAir06 = FALSE; }
		if (nRow == 1) { pDY04->oBtm1PickerVac07 = TRUE; pDY04->oBtm1PickerAir07 = FALSE; }
		if (nRow == 1) { pDY04->oBtm1PickerVac08 = TRUE; pDY04->oBtm1PickerAir08 = FALSE; }
	}
	if (nPos == INFO_PICK) {
		pInfo = gData.InfoBtm1Pick;
		if (pInfo[0] > 0) { pDY04->oBtm1PickerVac01 = TRUE; pDY04->oBtm1PickerAir01 = FALSE; }
		else			  { pDY04->oBtm1PickerVac01 = FALSE; }
		if (pInfo[1] > 0) { pDY04->oBtm1PickerVac02 = TRUE; pDY04->oBtm1PickerAir02 = FALSE; }
		else			  { pDY04->oBtm1PickerVac02 = FALSE; }
		if (pInfo[2] > 0) { pDY04->oBtm1PickerVac03 = TRUE; pDY04->oBtm1PickerAir03 = FALSE; }
		else			  { pDY04->oBtm1PickerVac03 = FALSE; }
		if (pInfo[3] > 0) { pDY04->oBtm1PickerVac04 = TRUE; pDY04->oBtm1PickerAir04 = FALSE; }
		else			  { pDY04->oBtm1PickerVac04 = FALSE; }
		if (pInfo[4] > 0) { pDY04->oBtm1PickerVac05 = TRUE; pDY04->oBtm1PickerAir05 = FALSE; }
		else			  { pDY04->oBtm1PickerVac05 = FALSE; }
		if (pInfo[5] > 0) { pDY04->oBtm1PickerVac06 = TRUE; pDY04->oBtm1PickerAir06 = FALSE; }
		else			  { pDY04->oBtm1PickerVac06 = FALSE; }
		if (pInfo[6] > 0) { pDY04->oBtm1PickerVac07 = TRUE; pDY04->oBtm1PickerAir07 = FALSE; }
		else			  { pDY04->oBtm1PickerVac07 = FALSE; }
		if (pInfo[7] > 0) { pDY04->oBtm1PickerVac08 = TRUE; pDY04->oBtm1PickerAir08 = FALSE; }
		else			  { pDY04->oBtm1PickerVac08 = FALSE; }

	}
	g_objAJinAXL.Write_Output(4);
}

BOOL CCommon::Get_InfoBtm1PickerVacOn()
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	int *pInfo = gData.InfoBtm1Pick;

	if (((pInfo[0] > 0 && pDX04->iBtm1PickerVac01 && pDX03->iBtm1PickerExist01) || pInfo[0] == 0 )&&
		((pInfo[1] > 0 && pDX04->iBtm1PickerVac02 && pDX03->iBtm1PickerExist02) || pInfo[1] == 0) &&
		((pInfo[2] > 0 && pDX04->iBtm1PickerVac03 && pDX03->iBtm1PickerExist03) || pInfo[2] == 0) &&
		((pInfo[3] > 0 && pDX04->iBtm1PickerVac04 && pDX03->iBtm1PickerExist04) || pInfo[3] == 0) &&
		((pInfo[4] > 0 && pDX04->iBtm1PickerVac05 && pDX03->iBtm1PickerExist05) || pInfo[4] == 0) &&
		((pInfo[5] > 0 && pDX04->iBtm1PickerVac06 && pDX03->iBtm1PickerExist06) || pInfo[5] == 0) &&
		((pInfo[6] > 0 && pDX04->iBtm1PickerVac07 && pDX03->iBtm1PickerExist07) || pInfo[6] == 0) &&
		((pInfo[7] > 0 && pDX04->iBtm1PickerVac08 && pDX03->iBtm1PickerExist08) || pInfo[7] == 0)) return TRUE;

	return FALSE;
}

BOOL CCommon::Check_Btm1PickerVacOn(int nSNo, int nLine, int nRow)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif

#ifndef AJIN_BOARD_USE 
	return TRUE;
#endif
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	int *pInfo = Get_AngleTrayInfo(nSNo, nLine);
	int nInfoCnt = 0;
	int nVacOnCnt = 0;

	for (int i = 0; i < gData.nBtmPickQt; i++) {
		if (pInfo[i] == 0) nInfoCnt++;
	}
	if (nInfoCnt == gData.nBtmPickQt) {
		// Picker 수량만큼 모듈을 집는 상황이면 잔량 확인 안해도 된다.
		// Tray Info 기준으로 모듈 픽업 상태 check하여 Alarm처리.
		if (nRow == 0) {
			if (((pInfo[0] > 0 && pDX04->iBtm1PickerVac01 && pDX03->iBtm1PickerExist01) || pInfo[0] == 0) &&
				((pInfo[1] > 0 && pDX04->iBtm1PickerVac02 && pDX03->iBtm1PickerExist02) || pInfo[1] == 0) &&
				((pInfo[2] > 0 && pDX04->iBtm1PickerVac03 && pDX03->iBtm1PickerExist03) || pInfo[2] == 0) &&
				((pInfo[3] > 0 && pDX04->iBtm1PickerVac04 && pDX03->iBtm1PickerExist04) || pInfo[3] == 0)) return TRUE;
		}
		if (nRow == 1) {
			if (((pInfo[0] > 0 && pDX04->iBtm1PickerVac05 && pDX03->iBtm1PickerExist05) || pInfo[0] == 0) &&
				((pInfo[1] > 0 && pDX04->iBtm1PickerVac06 && pDX03->iBtm1PickerExist06) || pInfo[1] == 0) &&
				((pInfo[2] > 0 && pDX04->iBtm1PickerVac07 && pDX03->iBtm1PickerExist07) || pInfo[2] == 0) &&
				((pInfo[3] > 0 && pDX04->iBtm1PickerVac08 && pDX03->iBtm1PickerExist08) || pInfo[3] == 0)) return TRUE;
		}

		// Picker Alarm 번호 저장.
		CString strErr = "";
		if (nRow == 0) {
			if (pInfo[0] > 0 && (pDX04->iBtm1PickerVac01 && pDX03->iBtm1PickerExist01)) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; gData.nErrBtm1PickIdx = 0; }
			if (pInfo[1] > 0 && (pDX04->iBtm1PickerVac02 && pDX03->iBtm1PickerExist02)) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; gData.nErrBtm1PickIdx = 1; }
			if (pInfo[2] > 0 && (pDX04->iBtm1PickerVac03 && pDX03->iBtm1PickerExist03)) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; gData.nErrBtm1PickIdx = 2; }
			if (pInfo[3] > 0 && (pDX04->iBtm1PickerVac04 && pDX03->iBtm1PickerExist04)) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; gData.nErrBtm1PickIdx = 3; }
		}
		if (nRow == 1) {
			if (pInfo[0] > 0 && (pDX04->iBtm1PickerVac05 && pDX03->iBtm1PickerExist05)) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; gData.nErrBtm1PickIdx = 4; }
			if (pInfo[1] > 0 && (pDX04->iBtm1PickerVac06 && pDX03->iBtm1PickerExist06)) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; gData.nErrBtm1PickIdx = 5; }
			if (pInfo[2] > 0 && (pDX04->iBtm1PickerVac07 && pDX03->iBtm1PickerExist07)) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; gData.nErrBtm1PickIdx = 6; }
			if (pInfo[3] > 0 && (pDX04->iBtm1PickerVac08 && pDX03->iBtm1PickerExist08)) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; gData.nErrBtm1PickIdx = 7; }
		}
		if (strErr != "") {
			strErr += "번 Picker Vacuum/Check 상태 확인 하여 주십시오.";
			gData.sErrBtm1PickNo = strErr;
		}
		return FALSE;	
	}

	if (nRow == 0) {
		if (pDX04->iBtm1PickerVac01 && pDX03->iBtm1PickerExist01) nVacOnCnt++;
		if (pDX04->iBtm1PickerVac02 && pDX03->iBtm1PickerExist02) nVacOnCnt++;
		if (pDX04->iBtm1PickerVac03 && pDX03->iBtm1PickerExist03) nVacOnCnt++;
		if (pDX04->iBtm1PickerVac04 && pDX03->iBtm1PickerExist04) nVacOnCnt++;

		if (nVacOnCnt != nInfoCnt) return FALSE;	// 정보 불일치 or Vacuum Alarm

		if (pDX04->iBtm1PickerVac01 && pDX03->iBtm1PickerExist01) gData.InfoAngleTray[nSNo-1][nLine][0] = 9;
		else													  gData.InfoAngleTray[nSNo-1][nLine][0] = 0;
		if (pDX04->iBtm1PickerVac02 && pDX03->iBtm1PickerExist02) gData.InfoAngleTray[nSNo-1][nLine][1] = 9;
		else													  gData.InfoAngleTray[nSNo-1][nLine][1] = 0;
		if (pDX04->iBtm1PickerVac03 && pDX03->iBtm1PickerExist03) gData.InfoAngleTray[nSNo-1][nLine][2] = 9;
		else													  gData.InfoAngleTray[nSNo-1][nLine][2] = 0;
		if (pDX04->iBtm1PickerVac04 && pDX03->iBtm1PickerExist04) gData.InfoAngleTray[nSNo-1][nLine][3] = 9;
		else													  gData.InfoAngleTray[nSNo-1][nLine][3] = 0;

	} else {
		if (pDX04->iBtm1PickerVac05 && pDX03->iBtm1PickerExist05) nVacOnCnt++;
		if (pDX04->iBtm1PickerVac06 && pDX03->iBtm1PickerExist06) nVacOnCnt++;
		if (pDX04->iBtm1PickerVac07 && pDX03->iBtm1PickerExist07) nVacOnCnt++;
		if (pDX04->iBtm1PickerVac08 && pDX03->iBtm1PickerExist08) nVacOnCnt++;

		if (nVacOnCnt != nInfoCnt) return FALSE;	// 정보 불일치 or Vacuum Alarm

		if (pDX04->iBtm1PickerVac05 && pDX03->iBtm1PickerExist05) gData.InfoAngleTray[nSNo-1][nLine][0] = 9;
		else													  gData.InfoAngleTray[nSNo-1][nLine][0] = 0;
		if (pDX04->iBtm1PickerVac06 && pDX03->iBtm1PickerExist06) gData.InfoAngleTray[nSNo-1][nLine][1] = 9;
		else													  gData.InfoAngleTray[nSNo-1][nLine][1] = 0;
		if (pDX04->iBtm1PickerVac07 && pDX03->iBtm1PickerExist07) gData.InfoAngleTray[nSNo-1][nLine][2] = 9;
		else													  gData.InfoAngleTray[nSNo-1][nLine][2] = 0;
		if (pDX04->iBtm1PickerVac08 && pDX03->iBtm1PickerExist08) gData.InfoAngleTray[nSNo-1][nLine][3] = 9;
		else													  gData.InfoAngleTray[nSNo-1][nLine][3] = 0;
	}

	return TRUE;
}

void CCommon::Set_InfoBtm1PickerClose(int nPos, int nRow, int nNo, int nLine)
{

	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	int *pInfo = NULL;

	if (nPos == INFO_STAGE) {
		//pInfo = Get_AngleTrayInfo(nNo, nLine);
		if (nRow == 0) { pDY03->oBtm1PickerOpen01 = FALSE; pDY03->oBtm1PickerClose01 = TRUE; }
		if (nRow == 0) { pDY03->oBtm1PickerOpen02 = FALSE; pDY03->oBtm1PickerClose02 = TRUE; }
		if (nRow == 0) { pDY03->oBtm1PickerOpen03 = FALSE; pDY03->oBtm1PickerClose03 = TRUE; }
		if (nRow == 0) { pDY03->oBtm1PickerOpen04 = FALSE; pDY03->oBtm1PickerClose04 = TRUE; }
		if (nRow == 1) { pDY03->oBtm1PickerOpen05 = FALSE; pDY03->oBtm1PickerClose05 = TRUE; }
		if (nRow == 1) { pDY03->oBtm1PickerOpen06 = FALSE; pDY03->oBtm1PickerClose06 = TRUE; }
		if (nRow == 1) { pDY03->oBtm1PickerOpen07 = FALSE; pDY03->oBtm1PickerClose07 = TRUE; }
		if (nRow == 1) { pDY03->oBtm1PickerOpen08 = FALSE; pDY03->oBtm1PickerClose08 = TRUE; }
	}
	if (nPos == INFO_PICK) {
		pInfo = gData.InfoBtm1Pick;
		if (pInfo[0] > 0) { pDY03->oBtm1PickerOpen01 = FALSE; pDY03->oBtm1PickerClose01 = TRUE; }
		else			  { pDY03->oBtm1PickerOpen01 = TRUE;  pDY03->oBtm1PickerClose01 = FALSE; }
		if (pInfo[1] > 0) { pDY03->oBtm1PickerOpen02 = FALSE; pDY03->oBtm1PickerClose02 = TRUE; }
		else			  { pDY03->oBtm1PickerOpen02 = TRUE;  pDY03->oBtm1PickerClose02 = FALSE; }
		if (pInfo[2] > 0) { pDY03->oBtm1PickerOpen03 = FALSE; pDY03->oBtm1PickerClose03 = TRUE; }
		else			  { pDY03->oBtm1PickerOpen03 = TRUE;  pDY03->oBtm1PickerClose03 = FALSE; }
		if (pInfo[3] > 0) { pDY03->oBtm1PickerOpen04 = FALSE; pDY03->oBtm1PickerClose04 = TRUE; }
		else			  { pDY03->oBtm1PickerOpen04 = TRUE;  pDY03->oBtm1PickerClose04 = FALSE; }
		if (pInfo[4] > 0) { pDY03->oBtm1PickerOpen05 = FALSE; pDY03->oBtm1PickerClose05 = TRUE; }
		else			  { pDY03->oBtm1PickerOpen05 = TRUE;  pDY03->oBtm1PickerClose05 = FALSE; }
		if (pInfo[5] > 0) { pDY03->oBtm1PickerOpen06 = FALSE; pDY03->oBtm1PickerClose06 = TRUE; }
		else			  { pDY03->oBtm1PickerOpen06 = TRUE;  pDY03->oBtm1PickerClose06 = FALSE; }
		if (pInfo[6] > 0) { pDY03->oBtm1PickerOpen07 = FALSE; pDY03->oBtm1PickerClose07 = TRUE; }
		else			  { pDY03->oBtm1PickerOpen07 = TRUE;  pDY03->oBtm1PickerClose07 = FALSE; }
		if (pInfo[7] > 0) { pDY03->oBtm1PickerOpen08 = FALSE; pDY03->oBtm1PickerClose08 = TRUE; }
		else			  { pDY03->oBtm1PickerOpen08 = TRUE;  pDY03->oBtm1PickerClose08 = FALSE; }

	}
	g_objAJinAXL.Write_Output(3);
}

BOOL CCommon::Get_InfoBtm1PickerClose()
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif

	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	int *pInfo = gData.InfoBtm1Pick;

	if (((pInfo[0] > 0 && pDX03->iBtm1PickerExist01) || pInfo[0] == 0 )&&
		((pInfo[1] > 0 && pDX03->iBtm1PickerExist02) || pInfo[1] == 0) &&
		((pInfo[2] > 0 && pDX03->iBtm1PickerExist03) || pInfo[2] == 0) &&
		((pInfo[3] > 0 && pDX03->iBtm1PickerExist04) || pInfo[3] == 0) &&
		((pInfo[4] > 0 && pDX03->iBtm1PickerExist05) || pInfo[4] == 0) &&
		((pInfo[5] > 0 && pDX03->iBtm1PickerExist06) || pInfo[5] == 0) &&
		((pInfo[6] > 0 && pDX03->iBtm1PickerExist07) || pInfo[6] == 0) &&
		((pInfo[7] > 0 && pDX03->iBtm1PickerExist08) || pInfo[7] == 0)) return TRUE;

	return FALSE;
}

// 2. Info Inspection I/O 함수
void CCommon::Set_InfoInspectVacOn(int nNo, int nPos)
{
#ifdef DRY_RUN_TEST
	return;
#endif
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	//int *pInfo = gData.InfoInspect[nNo - 1];
	int *pInfo = gData.InfoBtm1Pick;
	if (nPos == 1) pInfo = gData.InfoInspect[nNo - 1];

	if (nNo == 3) {			// Inspect 3
		if (pInfo[0] > 0) pDY06->oInspectStage3Vac01 = TRUE;
		else			  pDY06->oInspectStage3Vac01 = FALSE;
		if (pInfo[1] > 0) pDY06->oInspectStage3Vac02 = TRUE;
		else			  pDY06->oInspectStage3Vac02 = FALSE;
		if (pInfo[2] > 0) pDY06->oInspectStage3Vac03 = TRUE;
		else			  pDY06->oInspectStage3Vac03 = FALSE;
		if (pInfo[3] > 0) pDY06->oInspectStage3Vac04 = TRUE;
		else			  pDY06->oInspectStage3Vac04 = FALSE;
		if (pInfo[4] > 0) pDY06->oInspectStage3Vac05 = TRUE;
		else			  pDY06->oInspectStage3Vac05 = FALSE;
		if (pInfo[5] > 0) pDY06->oInspectStage3Vac06 = TRUE;
		else			  pDY06->oInspectStage3Vac06 = FALSE;
		if (pInfo[6] > 0) pDY06->oInspectStage3Vac07 = TRUE;
		else			  pDY06->oInspectStage3Vac07 = FALSE;
		if (pInfo[7] > 0) pDY06->oInspectStage3Vac08 = TRUE;
		else			  pDY06->oInspectStage3Vac08 = FALSE;

	} else if (nNo == 2) {	// Inspect 2
		if (pInfo[0] > 0) pDY06->oInspectStage2Vac01 = TRUE;
		else			  pDY06->oInspectStage2Vac01 = FALSE;
		if (pInfo[1] > 0) pDY06->oInspectStage2Vac02 = TRUE;
		else			  pDY06->oInspectStage2Vac02 = FALSE;
		if (pInfo[2] > 0) pDY06->oInspectStage2Vac03 = TRUE;
		else			  pDY06->oInspectStage2Vac03 = FALSE;
		if (pInfo[3] > 0) pDY06->oInspectStage2Vac04 = TRUE;
		else			  pDY06->oInspectStage2Vac04 = FALSE;
		if (pInfo[4] > 0) pDY06->oInspectStage2Vac05 = TRUE;
		else			  pDY06->oInspectStage2Vac05 = FALSE;
		if (pInfo[5] > 0) pDY06->oInspectStage2Vac06 = TRUE;
		else			  pDY06->oInspectStage2Vac06 = FALSE;
		if (pInfo[6] > 0) pDY06->oInspectStage2Vac07 = TRUE;
		else			  pDY06->oInspectStage2Vac07 = FALSE;
		if (pInfo[7] > 0) pDY06->oInspectStage2Vac08 = TRUE;
		else			  pDY06->oInspectStage2Vac08 = FALSE;

	} else if (nNo == 1) {	// Inspect 1
		if (pInfo[0] > 0) pDY05->oInspectStage1Vac01 = TRUE;
		else			  pDY05->oInspectStage1Vac01 = FALSE;
		if (pInfo[1] > 0) pDY05->oInspectStage1Vac02 = TRUE;
		else			  pDY05->oInspectStage1Vac02 = FALSE;
		if (pInfo[2] > 0) pDY05->oInspectStage1Vac03 = TRUE;
		else			  pDY05->oInspectStage1Vac03 = FALSE;
		if (pInfo[3] > 0) pDY05->oInspectStage1Vac04 = TRUE;
		else			  pDY05->oInspectStage1Vac04 = FALSE;
		if (pInfo[4] > 0) pDY05->oInspectStage1Vac05 = TRUE;
		else			  pDY05->oInspectStage1Vac05 = FALSE;
		if (pInfo[5] > 0) pDY05->oInspectStage1Vac06 = TRUE;
		else			  pDY05->oInspectStage1Vac06 = FALSE;
		if (pInfo[6] > 0) pDY05->oInspectStage1Vac07 = TRUE;
		else			  pDY05->oInspectStage1Vac07 = FALSE;
		if (pInfo[7] > 0) pDY05->oInspectStage1Vac08 = TRUE;
		else			  pDY05->oInspectStage1Vac08 = FALSE;
	}
	
	if (nNo == 1) g_objAJinAXL.Write_Output(5);
	else		  g_objAJinAXL.Write_Output(6);
}

BOOL CCommon::Get_InfoInspectVacOn(int nNo, int nPos)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	int *pInfo = gData.InfoInspect[nNo - 1];

	if (nNo == 3) {			// Inspect 3
		if (((pDX06->iInspectStage3Vac01 && pInfo[0] > 0) || pInfo[0] == 0) &&
			((pDX06->iInspectStage3Vac02 && pInfo[1] > 0) || pInfo[1] == 0) &&
			((pDX06->iInspectStage3Vac03 && pInfo[2] > 0) || pInfo[2] == 0) &&
			((pDX06->iInspectStage3Vac04 && pInfo[3] > 0) || pInfo[3] == 0) &&
			((pDX06->iInspectStage3Vac05 && pInfo[4] > 0) || pInfo[4] == 0) &&
			((pDX06->iInspectStage3Vac06 && pInfo[5] > 0) || pInfo[5] == 0) &&
			((pDX06->iInspectStage3Vac07 && pInfo[6] > 0) || pInfo[6] == 0) &&
			((pDX06->iInspectStage3Vac08 && pInfo[7] > 0) || pInfo[7] == 0)) return TRUE;

	} else if (nNo == 2) {	// Inspect 2
		if (((pDX06->iInspectStage2Vac01 && pInfo[0] > 0) || pInfo[0] == 0) &&
			((pDX06->iInspectStage2Vac02 && pInfo[1] > 0) || pInfo[1] == 0) &&
			((pDX06->iInspectStage2Vac03 && pInfo[2] > 0) || pInfo[2] == 0) &&
			((pDX06->iInspectStage2Vac04 && pInfo[3] > 0) || pInfo[3] == 0) &&
			((pDX06->iInspectStage2Vac05 && pInfo[4] > 0) || pInfo[4] == 0) &&
			((pDX06->iInspectStage2Vac06 && pInfo[5] > 0) || pInfo[5] == 0) &&
			((pDX06->iInspectStage2Vac07 && pInfo[6] > 0) || pInfo[6] == 0) &&
			((pDX06->iInspectStage2Vac08 && pInfo[7] > 0) || pInfo[7] == 0)) return TRUE;

	} else if (nNo == 1) {	// Inspect 1
		if (((pDX05->iInspectStage1Vac01 && pInfo[0] > 0) || pInfo[0] == 0) &&
			((pDX05->iInspectStage1Vac02 && pInfo[1] > 0) || pInfo[1] == 0) &&
			((pDX05->iInspectStage1Vac03 && pInfo[2] > 0) || pInfo[2] == 0) &&
			((pDX05->iInspectStage1Vac04 && pInfo[3] > 0) || pInfo[3] == 0) &&
			((pDX05->iInspectStage1Vac05 && pInfo[4] > 0) || pInfo[4] == 0) &&
			((pDX05->iInspectStage1Vac06 && pInfo[5] > 0) || pInfo[5] == 0) &&
			((pDX05->iInspectStage1Vac07 && pInfo[6] > 0) || pInfo[6] == 0) &&
			((pDX05->iInspectStage1Vac08 && pInfo[7] > 0) || pInfo[7] == 0)) return TRUE;
	}
	return FALSE;
}

// 3. Info Btm2 Picker I/O 함수

void CCommon::Set_InfoBtm2VacOn()
{
#ifdef DRY_RUN_TEST
	return;
#endif
	DY_DATA_08 *pDY08 = g_objAJinAXL.Get_pDY08();
	int *pInfo = gData.InfoBtm2Pick;

	if (pInfo[0] > 0) { pDY08->oBtm2PickerVac01 = TRUE; pDY08->oBtm2PickerAir01 = FALSE; }
	if (pInfo[1] > 0) { pDY08->oBtm2PickerVac02 = TRUE;	pDY08->oBtm2PickerAir02 = FALSE; }
	if (pInfo[2] > 0) { pDY08->oBtm2PickerVac03 = TRUE;	pDY08->oBtm2PickerAir03 = FALSE; }
	if (pInfo[3] > 0) { pDY08->oBtm2PickerVac04 = TRUE;	pDY08->oBtm2PickerAir04 = FALSE; }
	if (pInfo[4] > 0) { pDY08->oBtm2PickerVac05 = TRUE;	pDY08->oBtm2PickerAir05 = FALSE; }
	if (pInfo[5] > 0) { pDY08->oBtm2PickerVac06 = TRUE; pDY08->oBtm2PickerAir06 = FALSE; }
	if (pInfo[6] > 0) { pDY08->oBtm2PickerVac07 = TRUE;	pDY08->oBtm2PickerAir07 = FALSE; }
	if (pInfo[7] > 0) { pDY08->oBtm2PickerVac08 = TRUE;	pDY08->oBtm2PickerAir08 = FALSE; }

	g_objAJinAXL.Write_Output(8);
}

BOOL CCommon::Get_InfoBtm2VacOn()
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	int *pInfo = gData.InfoBtm2Pick;

	if (((pDX08->iBtm2PickerVac01 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((pDX08->iBtm2PickerVac02 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((pDX08->iBtm2PickerVac03 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((pDX08->iBtm2PickerVac04 && pInfo[3] > 0) || pInfo[3] == 0) &&
		((pDX08->iBtm2PickerVac05 && pInfo[4] > 0) || pInfo[4] == 0) &&
		((pDX08->iBtm2PickerVac06 && pInfo[5] > 0) || pInfo[5] == 0) &&
		((pDX08->iBtm2PickerVac07 && pInfo[6] > 0) || pInfo[6] == 0) &&
		((pDX08->iBtm2PickerVac08 && pInfo[7] > 0) || pInfo[7] == 0)) return TRUE;

	if (pInfo[0] > 0 && !pDX08->iBtm2PickerVac01) gData.nErrBtm2PickIdx = 0;
	if (pInfo[1] > 0 && !pDX08->iBtm2PickerVac02) gData.nErrBtm2PickIdx = 1;
	if (pInfo[2] > 0 && !pDX08->iBtm2PickerVac03) gData.nErrBtm2PickIdx = 2;
	if (pInfo[3] > 0 && !pDX08->iBtm2PickerVac04) gData.nErrBtm2PickIdx = 3;
	if (pInfo[4] > 0 && !pDX08->iBtm2PickerVac05) gData.nErrBtm2PickIdx = 4;
	if (pInfo[5] > 0 && !pDX08->iBtm2PickerVac06) gData.nErrBtm2PickIdx = 5;
	if (pInfo[6] > 0 && !pDX08->iBtm2PickerVac07) gData.nErrBtm2PickIdx = 6;
	if (pInfo[7] > 0 && !pDX08->iBtm2PickerVac08) gData.nErrBtm2PickIdx = 7;

	return FALSE;
}

void CCommon::Set_InfoBtm2Close()
{
	DY_DATA_07 *pDY07 = g_objAJinAXL.Get_pDY07();
	int *pInfo = gData.InfoBtm2Pick;

	if (pInfo[0] > 0) { pDY07->oBtm2PickerOpen01 = FALSE; pDY07->oBtm2PickerClose01 = TRUE; }
	if (pInfo[1] > 0) { pDY07->oBtm2PickerOpen02 = FALSE; pDY07->oBtm2PickerClose02 = TRUE; }
	if (pInfo[2] > 0) { pDY07->oBtm2PickerOpen03 = FALSE; pDY07->oBtm2PickerClose03 = TRUE; }
	if (pInfo[3] > 0) { pDY07->oBtm2PickerOpen04 = FALSE; pDY07->oBtm2PickerClose04 = TRUE; }
	if (pInfo[4] > 0) { pDY07->oBtm2PickerOpen05 = FALSE; pDY07->oBtm2PickerClose05 = TRUE; }
	if (pInfo[5] > 0) { pDY07->oBtm2PickerOpen06 = FALSE; pDY07->oBtm2PickerClose06 = TRUE; }
	if (pInfo[6] > 0) { pDY07->oBtm2PickerOpen07 = FALSE; pDY07->oBtm2PickerClose07 = TRUE; }
	if (pInfo[7] > 0) { pDY07->oBtm2PickerOpen08 = FALSE; pDY07->oBtm2PickerClose08 = TRUE; }

	g_objAJinAXL.Write_Output(7);
}

BOOL CCommon::Get_InfoBtm2Close()
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	int *pInfo = gData.InfoBtm2Pick;

	if (((!pDX07->iBtm2PickerOpen01 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((!pDX07->iBtm2PickerOpen02 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((!pDX07->iBtm2PickerOpen03 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((!pDX07->iBtm2PickerOpen04 && pInfo[3] > 0) || pInfo[3] == 0) &&
		((!pDX07->iBtm2PickerOpen05 && pInfo[4] > 0) || pInfo[4] == 0) &&
		((!pDX07->iBtm2PickerOpen06 && pInfo[5] > 0) || pInfo[5] == 0) &&
		((!pDX07->iBtm2PickerOpen07 && pInfo[6] > 0) || pInfo[6] == 0) &&
		((!pDX07->iBtm2PickerOpen08 && pInfo[7] > 0) || pInfo[7] == 0)) return TRUE;

	return FALSE;
}

BOOL CCommon::Get_InfoBtm2Check()
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	int *pInfo = gData.InfoBtm2Pick;

	if (((pDX07->iBtm2PickerExist01 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((pDX07->iBtm2PickerExist02 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((pDX07->iBtm2PickerExist03 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((pDX07->iBtm2PickerExist04 && pInfo[3] > 0) || pInfo[3] == 0) &&
		((pDX07->iBtm2PickerExist05 && pInfo[4] > 0) || pInfo[4] == 0) &&
		((pDX07->iBtm2PickerExist06 && pInfo[5] > 0) || pInfo[5] == 0) &&
		((pDX07->iBtm2PickerExist07 && pInfo[6] > 0) || pInfo[6] == 0) &&
		((pDX07->iBtm2PickerExist08 && pInfo[7] > 0) || pInfo[7] == 0)) return TRUE;

	return FALSE;
}

// 4. Info Sort Picker1 I/O 함수
void CCommon::Set_InfoSortPicker1Down(int nPos)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	int *pInfo = gData.InfoSortPick[0];
	if (nPos == 1) pInfo = gData.InfoNgBuffer[1];

	if (pInfo[0] > 0) { pDY10->oSortPicker1Down1 = TRUE; }
	if (pInfo[1] > 0) { pDY10->oSortPicker1Down2 = TRUE; }
	if (pInfo[2] > 0) { pDY10->oSortPicker1Down3 = TRUE; }
	if (pInfo[3] > 0) { pDY10->oSortPicker1Down4 = TRUE; }

	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_InfoSortPicker1Down(int nPos)
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	int *pInfo = gData.InfoSortPick[0];
	if (nPos == 1) pInfo = gData.InfoNgBuffer[1];

	if (((pDX10->iSortPicker1Down1 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((pDX10->iSortPicker1Down2 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((pDX10->iSortPicker1Down3 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((pDX10->iSortPicker1Down4 && pInfo[3] > 0) || pInfo[3] == 0)) return TRUE;

	return FALSE;
}

void CCommon::Set_InfoSortPicker1Close(int nPos)
{
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	int *pInfo = gData.InfoSortPick[0];
	if (nPos == 1) pInfo = gData.InfoNgBuffer[1];

	if (pInfo[0] > 0) { pDY10->oSortPicker1Open1 = FALSE; pDY10->oSortPicker1Close1 = TRUE; }
	if (pInfo[1] > 0) { pDY10->oSortPicker1Open2 = FALSE; pDY10->oSortPicker1Close2 = TRUE; }
	if (pInfo[2] > 0) { pDY10->oSortPicker1Open3 = FALSE; pDY10->oSortPicker1Close3 = TRUE; }
	if (pInfo[3] > 0) { pDY10->oSortPicker1Open4 = FALSE; pDY10->oSortPicker1Close4 = TRUE; }

	g_objAJinAXL.Write_Output(10);
}

BOOL CCommon::Get_InfoSortPicker1Close()
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	int *pInfo = gData.InfoSortPick[0];

	if (((!pDX10->iSortPicker1Open1 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((!pDX10->iSortPicker1Open2 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((!pDX10->iSortPicker1Open3 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((!pDX10->iSortPicker1Open4 && pInfo[3] > 0) || pInfo[3] == 0)) return TRUE;

	return FALSE;
}

BOOL CCommon::Get_InfoSortPicker1Check()
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	int *pInfo = gData.InfoSortPick[0];

	if (((pDX10->iSortPicker1Exist1 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((pDX10->iSortPicker1Exist2 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((pDX10->iSortPicker1Exist3 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((pDX10->iSortPicker1Exist4 && pInfo[3] > 0) || pInfo[3] == 0)) return TRUE;

	return FALSE;
}

// 5. Info Sort Picker2 I/O 함수
void CCommon::Set_InfoSortPicker2Down(int nPos)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();
	int *pInfo = gData.InfoSortPick[1];
	if (nPos == 1) pInfo = gData.InfoNgBuffer[0];

	if (pInfo[0] > 0) { pDY11->oSortPicker2Down1 = TRUE; }
	if (pInfo[1] > 0) { pDY11->oSortPicker2Down2 = TRUE; }
	if (pInfo[2] > 0) { pDY11->oSortPicker2Down3 = TRUE; }
	if (pInfo[3] > 0) { pDY11->oSortPicker2Down4 = TRUE; }

	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_InfoSortPicker2Down(int nPos)
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	int *pInfo = gData.InfoSortPick[1];
	if (nPos == 1) pInfo = gData.InfoNgBuffer[0];

	if (((pDX11->iSortPicker2Down1 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((pDX11->iSortPicker2Down2 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((pDX11->iSortPicker2Down3 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((pDX11->iSortPicker2Down4 && pInfo[3] > 0) || pInfo[3] == 0)) return TRUE;

	return FALSE;
}

void CCommon::Set_InfoSortPicker2Close(int nPos)
{
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();
	int *pInfo = gData.InfoSortPick[1];
	if (nPos == 1) pInfo = gData.InfoNgBuffer[0];

	if (pInfo[0] > 0) { pDY11->oSortPicker2Open1 = FALSE; pDY11->oSortPicker2Close1 = TRUE; }
	if (pInfo[1] > 0) { pDY11->oSortPicker2Open2 = FALSE; pDY11->oSortPicker2Close2 = TRUE; }
	if (pInfo[2] > 0) { pDY11->oSortPicker2Open3 = FALSE; pDY11->oSortPicker2Close3 = TRUE; }
	if (pInfo[3] > 0) { pDY11->oSortPicker2Open4 = FALSE; pDY11->oSortPicker2Close4 = TRUE; }

	g_objAJinAXL.Write_Output(11);
}

BOOL CCommon::Get_InfoSortPicker2Close()
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	int *pInfo = gData.InfoSortPick[1];

	if (((!pDX11->iSortPicker2Open1 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((!pDX11->iSortPicker2Open2 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((!pDX11->iSortPicker2Open3 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((!pDX11->iSortPicker2Open4 && pInfo[3] > 0) || pInfo[3] == 0)) return TRUE;

	return FALSE;
}

BOOL CCommon::Get_InfoSortPicker2Check()
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	int *pInfo = gData.InfoSortPick[1];

	if (((pDX11->iSortPicker2Exist1 && pInfo[0] > 0) || pInfo[0] == 0) &&
		((pDX11->iSortPicker2Exist2 && pInfo[1] > 0) || pInfo[1] == 0) &&
		((pDX11->iSortPicker2Exist3 && pInfo[2] > 0) || pInfo[2] == 0) &&
		((pDX11->iSortPicker2Exist4 && pInfo[3] > 0) || pInfo[3] == 0)) return TRUE;

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Ng Buffer I/O 함수
void CCommon::Set_NgBufferVacOff(int nNo)
{
#ifdef DRY_RUN_TEST
	return;
#endif
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();

	if (nNo == 1) {
		pDY11->oNgBufferVac05 = FALSE;
		pDY11->oNgBufferVac06 = FALSE;
		pDY11->oNgBufferVac07 = FALSE;
		pDY11->oNgBufferVac08 = FALSE;

		g_objAJinAXL.Write_Output(11);
	} 

	if (nNo == 2) {
		pDY10->oNgBufferVac01 = FALSE;
		pDY10->oNgBufferVac02 = FALSE;
		pDY10->oNgBufferVac03 = FALSE;
		pDY10->oNgBufferVac04 = FALSE;

		g_objAJinAXL.Write_Output(10);
	}	
}

void CCommon::Set_InfoNgBufferVacOn(int nNo)
{
#ifdef DRY_RUN_TEST
	return;
#endif
	DY_DATA_10 *pDY10 = g_objAJinAXL.Get_pDY10();
	DY_DATA_11 *pDY11 = g_objAJinAXL.Get_pDY11();
	int *pInfo = gData.InfoNgBuffer[nNo-1];

	if (nNo == 1) {
		pInfo = gData.InfoNgBuffer[1];

		if (pInfo[0] > 0) { pDY11->oNgBufferVac05 = TRUE;}
		if (pInfo[1] > 0) { pDY11->oNgBufferVac06 = TRUE;}
		if (pInfo[2] > 0) { pDY11->oNgBufferVac07 = TRUE;}
		if (pInfo[3] > 0) { pDY11->oNgBufferVac08 = TRUE;}
		g_objAJinAXL.Write_Output(11);
	} 

	if (nNo == 2) {
		pInfo = gData.InfoNgBuffer[0];

		if (pInfo[0] > 0) { pDY10->oNgBufferVac01 = TRUE;}
		if (pInfo[1] > 0) { pDY10->oNgBufferVac02 = TRUE;}
		if (pInfo[2] > 0) { pDY10->oNgBufferVac03 = TRUE;}
		if (pInfo[3] > 0) { pDY10->oNgBufferVac04 = TRUE;}
		g_objAJinAXL.Write_Output(10);
	}		
}

BOOL CCommon::Get_InfoNgBufferVacOn(int nNo)
{
#ifdef DRY_RUN_TEST
	return TRUE;
#endif
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	int *pInfo = gData.InfoNgBuffer[nNo-1];

	if (nNo == 1) {
		pInfo = gData.InfoNgBuffer[1];
		if (((pDX11->iNgBufferVac05 && pInfo[0] > 0) || pInfo[0] == 0) &&
			((pDX11->iNgBufferVac06 && pInfo[1] > 0) || pInfo[1] == 0) &&
			((pDX11->iNgBufferVac07 && pInfo[2] > 0) || pInfo[2] == 0) &&
			((pDX11->iNgBufferVac08 && pInfo[3] > 0) || pInfo[3] == 0)) return TRUE;
	} 

	if (nNo == 2) {
		pInfo = gData.InfoNgBuffer[0];
		if (((pDX10->iNgBufferVac01 && pInfo[0] > 0) || pInfo[0] == 0) &&
			((pDX10->iNgBufferVac02 && pInfo[1] > 0) || pInfo[1] == 0) &&
			((pDX10->iNgBufferVac03 && pInfo[2] > 0) || pInfo[2] == 0) &&
			((pDX10->iNgBufferVac04 && pInfo[3] > 0) || pInfo[3] == 0)) return TRUE;
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Load Tray Info
int *CCommon::Get_AngleTrayInfo(int nSNo, int nLine)
{
	static int nInfo[8] = { 0 };

	int nENo = gData.nTrayX;
	for (int i = 0; i < nENo; i++) {
		nInfo[i] = gData.InfoAngleTray[nSNo-1][nLine][i];
	}
	return nInfo;
}
///////////////////////////////////////////////////////////////////////////////

CString CCommon::Get_Btm1PickerVacErr()
{
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	int *pInfo = gData.InfoBtm1Pick;
	
	CString strErr, strSNo;
	strErr = "";
	if ((!pDX04->iBtm1PickerVac01 || !pDX03->iBtm1PickerExist01) && pInfo[0] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
	if ((!pDX04->iBtm1PickerVac02 || !pDX03->iBtm1PickerExist02) && pInfo[1] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
	if ((!pDX04->iBtm1PickerVac03 || !pDX03->iBtm1PickerExist03) && pInfo[2] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
	if ((!pDX04->iBtm1PickerVac04 || !pDX03->iBtm1PickerExist04) && pInfo[3] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
	if ((!pDX04->iBtm1PickerVac05 || !pDX03->iBtm1PickerExist05) && pInfo[4] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; }
	if ((!pDX04->iBtm1PickerVac06 || !pDX03->iBtm1PickerExist06) && pInfo[5] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; }
	if ((!pDX04->iBtm1PickerVac07 || !pDX03->iBtm1PickerExist07) && pInfo[6] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; }
	if ((!pDX04->iBtm1PickerVac08 || !pDX03->iBtm1PickerExist08) && pInfo[7] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; }

	if (strErr != "") {
		strErr += "번 Picker Vacuum/Check 상태 확인 하여 주십시오.";

		if (gData.nErrAngleStageNo != 0) {
			strSNo.Format("%d번 Stage.", gData.nErrAngleStageNo);
			strErr += strSNo;
			gData.nErrAngleStageNo = 0;
		}
	}
	return strErr;
}

CString CCommon::Get_Btm1PickerOpenChkErr()
{
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	
	CString strErr, strSNo;
	strErr = "";
	if (( !pDX03->iBtm1PickerOpen01) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
	if (( !pDX03->iBtm1PickerOpen02) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
	if (( !pDX03->iBtm1PickerOpen03) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
	if (( !pDX03->iBtm1PickerOpen04) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
	if (( !pDX03->iBtm1PickerOpen05) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; }
	if (( !pDX03->iBtm1PickerOpen06) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; }
	if (( !pDX03->iBtm1PickerOpen07) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; }
	if (( !pDX03->iBtm1PickerOpen08) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; }

	if (strErr != "") {
		strErr += "번 Picker Open 센서 확인 하여 주십시오.";
	}
	return strErr;
}


CString CCommon::Get_Btm1PickerCloseChkErr()
{
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();

	int *pInfo = gData.InfoBtm1Pick;

	CString strErr, strSNo;
	strErr = "";
	if (( !pDX03->iBtm1PickerExist01) && pInfo[0] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
	if (( !pDX03->iBtm1PickerExist02) && pInfo[1] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
	if (( !pDX03->iBtm1PickerExist03) && pInfo[2] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
	if (( !pDX03->iBtm1PickerExist04) && pInfo[3] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
	if (( !pDX03->iBtm1PickerExist05) && pInfo[4] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; }
	if (( !pDX03->iBtm1PickerExist06) && pInfo[5] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; }
	if (( !pDX03->iBtm1PickerExist07) && pInfo[6] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; }
	if (( !pDX03->iBtm1PickerExist08) && pInfo[7] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; }

	if (strErr != "") {
		strErr += "번 Picker CM Close 상태 확인 하여 주십시오.";
	}
	return strErr;
}

CString CCommon::Get_Btm1PickerDownChkErr()
{
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();

	CString strErr, strSNo;
	strErr = "";
	if (( pDX03->iBtm1PickerUp01 || !pDX03->iBtm1PickerDown01) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
	if (( pDX03->iBtm1PickerUp02 || !pDX03->iBtm1PickerDown02) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
	if (( pDX03->iBtm1PickerUp03 || !pDX03->iBtm1PickerDown03) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
	if (( pDX03->iBtm1PickerUp04 || !pDX03->iBtm1PickerDown04) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
	if (( pDX04->iBtm1PickerUp05 || !pDX04->iBtm1PickerDown05) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; }
	if (( pDX04->iBtm1PickerUp06 || !pDX04->iBtm1PickerDown06) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; }
	if (( pDX04->iBtm1PickerUp07 || !pDX04->iBtm1PickerDown07) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; }
	if (( pDX04->iBtm1PickerUp08 || !pDX04->iBtm1PickerDown08) ) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; }

	if (strErr != "") {
		strErr += "번 Picker Up or Down 센서 확인 하여 주십시오.";
	}
	return strErr;
}

CString CCommon::Get_Btm2PickerVacErr()
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	int *pInfo = gData.InfoBtm2Pick;

	CString strErr;
	strErr = "";
	if ((!pDX08->iBtm2PickerVac01 || !pDX07->iBtm2PickerExist01) && pInfo[0] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
	if ((!pDX08->iBtm2PickerVac02 || !pDX07->iBtm2PickerExist02) && pInfo[1] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
	if ((!pDX08->iBtm2PickerVac03 || !pDX07->iBtm2PickerExist03) && pInfo[2] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
	if ((!pDX08->iBtm2PickerVac04 || !pDX07->iBtm2PickerExist04) && pInfo[3] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
	if ((!pDX08->iBtm2PickerVac05 || !pDX07->iBtm2PickerExist05) && pInfo[4] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; }
	if ((!pDX08->iBtm2PickerVac06 || !pDX07->iBtm2PickerExist06) && pInfo[5] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; }
	if ((!pDX08->iBtm2PickerVac07 || !pDX07->iBtm2PickerExist07) && pInfo[6] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; }
	if ((!pDX08->iBtm2PickerVac08 || !pDX07->iBtm2PickerExist08) && pInfo[7] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; }

	if (strErr != "") { 
		strErr += "번 Picker Vacuum/Check 상태 확인 하여 주십시오.";
	}
	return strErr;
}


CString CCommon::Get_Btm2PickerCloseChkErr()
{
	DX_DATA_07 *pDX07 = g_objAJinAXL.Get_pDX07();
	DX_DATA_08 *pDX08 = g_objAJinAXL.Get_pDX08();
	int *pInfo = gData.InfoBtm2Pick;

	CString strErr;
	strErr = "";
	if ((!pDX07->iBtm2PickerExist01) && pInfo[0] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
	if ((!pDX07->iBtm2PickerExist02) && pInfo[1] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
	if ((!pDX07->iBtm2PickerExist03) && pInfo[2] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
	if ((!pDX07->iBtm2PickerExist04) && pInfo[3] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
	if ((!pDX07->iBtm2PickerExist05) && pInfo[4] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; }
	if ((!pDX07->iBtm2PickerExist06) && pInfo[5] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; }
	if ((!pDX07->iBtm2PickerExist07) && pInfo[6] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; }
	if ((!pDX07->iBtm2PickerExist08) && pInfo[7] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; }

	if (strErr != "") { 
		strErr += "번 Picker Close 센서 상태 확인 하여 주십시오.";
	}
	return strErr;
}

CString CCommon::Get_Sort1PickerCheckErr()
{
	DX_DATA_10 *pDX10 = g_objAJinAXL.Get_pDX10();
	int *pInfo = gData.InfoBtm2Pick;

	CString strErr, strSNo;
	strErr = "";
	if (!pDX10->iSortPicker1Exist1 && pInfo[0] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
	if (!pDX10->iSortPicker1Exist2 && pInfo[1] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
	if (!pDX10->iSortPicker1Exist3 && pInfo[2] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
	if (!pDX10->iSortPicker1Exist4 && pInfo[3] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
	
	if (strErr != "") {
		strErr += "번 Picker Check Sensor 상태를 확인 하여 주십시오.";

		if (gData.nErrBufferStageNo != 0) {
			strSNo.Format("%d번 Stage.", gData.nErrBufferStageNo);
			strErr += strSNo;
			gData.nErrBufferStageNo = 0;
		}
	}
	return strErr;
}

CString CCommon::Get_Sort2PickerCheckErr()
{
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	int *pInfo = gData.InfoBtm2Pick;

	CString strErr, strSNo;
	strErr = "";
	if (!pDX11->iSortPicker2Exist1 && pInfo[0] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
	if (!pDX11->iSortPicker2Exist2 && pInfo[1] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
	if (!pDX11->iSortPicker2Exist3 && pInfo[2] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
	if (!pDX11->iSortPicker2Exist4 && pInfo[3] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
	
	if (strErr != "") {
		strErr += "번 Picker Check Sensor 상태를 확인 하여 주십시오.";

		if (gData.nErrBufferStageNo != 0) {
			strSNo.Format("%d번 Stage.", gData.nErrBufferStageNo);
			strErr += strSNo;
			gData.nErrBufferStageNo = 0;
		}
	}
	return strErr;
}

CString CCommon::Get_JigErrMessage()
{
	CString strUnit[8] = {"BTM1:", "Insp1 Top1:", "Insp2 Top1:", "Insp3 Top1:", "Insp1 Top2:", "Insp2 Top2:", "Insp3 Top2:", "BTM2:"};
	CString strErr[8] = {"", };
	CString strNo = "";
	CString strMsg;

	for (int i = 0; i < 8; i++) {	// Unit
		for (int j = 0; j < 10; j++) {	// Jig	
			if (gData.dJigNgOver[i][j] > 1.0){
				if (strErr[i] == "") strErr[i] += strUnit[i];
				strNo.Format("%d(%dea/%0.1lf%%). ", j+1, gData.nJigNgOverCnt[i][j], gData.dJigNgOver[i][j]);
				strErr[i] += strNo;
			}
		}
	}

	strMsg.Format("%s%s%s%s%s%s%s%s", strErr[0], strErr[1], strErr[2], strErr[3], strErr[4], strErr[5], strErr[6], strErr[7]);

	return strMsg;
}



CString CCommon::Get_InspectVacOnChkErr(int nNo)
{
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();
	int *pInfo = gData.InfoInspect[nNo - 1];

	CString strErr, strSNo;
	strErr = "";

	if(nNo == 1){
		if (( !pDX05->iInspectStage1Vac01) && pInfo[0] > 0 ) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
		if (( !pDX05->iInspectStage1Vac02) && pInfo[1] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
		if (( !pDX05->iInspectStage1Vac03) && pInfo[2] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
		if (( !pDX05->iInspectStage1Vac04) && pInfo[3] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
		if (( !pDX05->iInspectStage1Vac05) && pInfo[4] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; }
		if (( !pDX05->iInspectStage1Vac06) && pInfo[5] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; }
		if (( !pDX05->iInspectStage1Vac07) && pInfo[6] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; }
		if (( !pDX05->iInspectStage1Vac08) && pInfo[7] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; }
	}
	else if(nNo == 2){
		if (( !pDX06->iInspectStage2Vac01) && pInfo[0] > 0 ) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
		if (( !pDX06->iInspectStage2Vac02) && pInfo[1] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
		if (( !pDX06->iInspectStage2Vac03) && pInfo[2] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
		if (( !pDX06->iInspectStage2Vac04) && pInfo[3] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
		if (( !pDX06->iInspectStage2Vac05) && pInfo[4] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; }
		if (( !pDX06->iInspectStage2Vac06) && pInfo[5] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; }
		if (( !pDX06->iInspectStage2Vac07) && pInfo[6] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; }
		if (( !pDX06->iInspectStage2Vac08) && pInfo[7] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; }
	}
	else if(nNo == 3){
		if (( !pDX06->iInspectStage3Vac01) && pInfo[0] > 0 ) { if (strErr == "") { strErr = "# ==> "; } strErr += "1. "; }
		if (( !pDX06->iInspectStage3Vac02) && pInfo[1] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "2. "; }
		if (( !pDX06->iInspectStage3Vac03) && pInfo[2] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "3. "; }
		if (( !pDX06->iInspectStage3Vac04) && pInfo[3] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "4. "; }
		if (( !pDX06->iInspectStage3Vac05) && pInfo[4] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "5. "; }
		if (( !pDX06->iInspectStage3Vac06) && pInfo[5] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "6. "; }
		if (( !pDX06->iInspectStage3Vac07) && pInfo[6] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "7. "; }
		if (( !pDX06->iInspectStage3Vac08) && pInfo[7] > 0) { if (strErr == "") { strErr = "# ==> "; } strErr += "8. "; }
	}

	
	if (strErr != "") {
		strErr += "번 Stage Vac 센서 확인 하여 주십시오.";
	}
	return strErr;
}

void CCommon::Get_HardInfo(CString &strHardDisk)
{
	ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

	// GetDiskFreeSpaceEx 호출
	if (GetDiskFreeSpaceEx("C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
		// GB로 변환하여 출력
		double totalGB = static_cast<double>(totalBytes.QuadPart) / (1024 * 1024 * 1024);
		double freeGB = static_cast<double>(totalFreeBytes.QuadPart) / (1024 * 1024 * 1024);
		double usedGB = totalGB - freeGB;

		strHardDisk.Format("%lf", freeGB);
	}
}

void CCommon::Get_RAMSize(CString &strRAM)
{
	ULONGLONG Size;
	GetPhysicallyInstalledSystemMemory(&Size);
	strRAM.Format(_T("%llu"), ((Size/1024)/1024));
}

void CCommon::Get_CPUInfo(CString& strCPU)
{
	HRESULT hres;

	// Initialize COM
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) {
		//std::cout << "Failed to initialize COM library." << std::endl;
		return;
	}

	// Set general COM security levels
	hres = CoInitializeSecurity(
		NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(hres)) {
		//std::cout << "Failed to initialize security." << std::endl;
		CoUninitialize();
		return;
	}

	// Obtain the WMI locator
	IWbemLocator *pLoc = NULL;
	hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
	if (FAILED(hres)) {
		//std::cout << "Failed to create IWbemLocator object." << std::endl;
		CoUninitialize();
		return;
	}

	// Connect to WMI
	IWbemServices *pSvc = NULL;
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
	if (FAILED(hres)) {
		//std::cout << "Failed to connect to WMI." << std::endl;
		pLoc->Release();
		CoUninitialize();
		return;
	}

	// Set security levels on the proxy
	hres = CoSetProxyBlanket(
		pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hres)) {
		//std::cout << "Failed to set proxy blanket." << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return;
	}

	// Query for CPU information
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_Processor"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL, &pEnumerator);
	if (FAILED(hres)) {
		//std::cout << "Query for CPU information failed." << std::endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return;
	}

	// Retrieve the data
	IWbemClassObject *pClassObject;
	ULONG uReturn = 0;
	while (pEnumerator) {
		hres = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn);
		if (0 == uReturn) {
			break;
		}

		VARIANT vtProp;
		pClassObject->Get(L"Name", 0, &vtProp, 0, 0);
		strCPU = vtProp.bstrVal;
		//std::wcout << "CPU Name : " << vtProp.bstrVal << std::endl;
		VariantClear(&vtProp);

		pClassObject->Release();
	}

	// Cleanup
	pSvc->Release();
	pLoc->Release();
	CoUninitialize();


}