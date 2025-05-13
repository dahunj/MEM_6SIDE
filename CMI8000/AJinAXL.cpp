// AJinAXL.cpp : 구현 파일
//
#include "stdafx.h"
#include "AJinAXL.h"
#include "CMI8000.h"
#include <math.h>
#include "MESInterface.h"

// AJin Board Library
#include "AXL.h"
#include "AXM.h"
#include "AXD.h"
#include "AXDev.h"
#pragma comment (lib, "AXL.lib")

CAJinAXL g_objAJinAXL;

CAJinAXL::CAJinAXL(void)
{
	for (int i = 0; i < DIO_MODULE_COUNT; i++) { DXY_DATA *pDX = Get_pDX(i); pDX->nValue = 0; }
	for (int i = 0; i < DIO_MODULE_COUNT; i++) { DXY_DATA *pDY = Get_pDY(i); pDY->nValue = 0; }

	for (int i = 0; i < AXIS_COUNT; i++) m_strAxisName[i] = _T("");

	m_pThreadAJin = NULL;
	m_bThreadAJin = FALSE;

	m_bReadVelocity = FALSE;
}

CAJinAXL::~CAJinAXL(void)
{
}


void CAJinAXL::SetEncoderActPos(int nAxis, double dPos)
{
	DWORD dwReturn = AxmStatusSetActPos(nAxis, dPos);
	if (dwReturn != AXT_RT_SUCCESS) AfxMessageBox("Error");

}


BOOL CAJinAXL::Initialize()
{
	DWORD dwReturn;
#ifdef AJIN_BOARD_USE
	dwReturn = AxlOpenNoReset(7);
	if (dwReturn != AXT_RT_SUCCESS) return FALSE;

	// DIO
	long lDIOCount;
	dwReturn = AxdInfoGetModuleCount(&lDIOCount);
	if (dwReturn != AXT_RT_SUCCESS) return FALSE;
	if (lDIOCount < DIO_MODULE_COUNT) return FALSE;

	Read_Input();
	Read_Output();

	// Door Interlock
	m_DY15.oModeSelect = TRUE; Write_Output(15); theApp.uSleep(500);
	m_DY15.oSafetyReset = TRUE;  Write_Output(15); theApp.uSleep(500);
	m_DY15.oSafetyReset = FALSE; Write_Output(15); theApp.uSleep(500);
	m_DY15.oSafetyReset = TRUE;  Write_Output(15); theApp.uSleep(500);
	m_DY15.oSafetyReset = FALSE; Write_Output(15); theApp.uSleep(500);

	Alarm_Reset(-1);	// All Reset

	// Motion
	long lAxisCount;
	dwReturn = AxmInfoGetAxisCount(&lAxisCount);
	if (dwReturn != AXT_RT_SUCCESS) return FALSE;
	if (lAxisCount < AXIS_COUNT) return FALSE;

	CString strMotFile = gData.sEnvPath + "\\AJinSetting.mot";
	dwReturn = AxmMotLoadParaAll((LPSTR)(LPCSTR)strMotFile);
	if (dwReturn != AXT_RT_SUCCESS) return FALSE;
#endif

	Read_AxisList();	// Axis Name & Param

	for (long i = 0; i < AXIS_COUNT; i++) Set_ServoOn(i);

	dwReturn = AxmMoveEStop(AX_PM_TRIGGER);
	dwReturn = AxmSignalServoOn(AX_PM_TRIGGER, TRUE);

	m_bThreadAJin = TRUE;
	m_pThreadAJin = AfxBeginThread(Thread_AJin, NULL);

	return TRUE;
}

void CAJinAXL::Terminate()
{
	if (m_pThreadAJin) {
		m_bThreadAJin = FALSE;
		WaitForSingleObject(m_pThreadAJin->m_hThread, INFINITE);
	}

	for (long i = 0; i < AXIS_COUNT; i++) Set_ServoOff(i);

#ifdef AJIN_BOARD_USE
	if (AxlIsOpened()) AxlClose();
#endif
}

void CAJinAXL::Read_Input()
{
#ifdef AJIN_BOARD_USE
	AxdiReadInportDword( 0, 0, &m_DX00.nValue);
	AxdiReadInportDword( 1, 0, &m_DX01.nValue);
	AxdiReadInportDword( 2, 0, &m_DX02.nValue);
	AxdiReadInportDword( 3, 0, &m_DX03.nValue);
	AxdiReadInportDword( 4, 0, &m_DX04.nValue);
	AxdiReadInportDword( 5, 0, &m_DX05.nValue);
	AxdiReadInportDword( 6, 0, &m_DX06.nValue);
	AxdiReadInportDword( 7, 0, &m_DX07.nValue);
	AxdiReadInportDword( 8, 0, &m_DX08.nValue);
	AxdiReadInportDword( 9, 0, &m_DX09.nValue);
	AxdiReadInportDword(10, 0, &m_DX10.nValue);
	AxdiReadInportDword(11, 0, &m_DX11.nValue);
	AxdiReadInportDword(12, 0, &m_DX12.nValue);
	AxdiReadInportDword(13, 0, &m_DX13.nValue);
	AxdiReadInportDword(14, 0, &m_DX14.nValue);
	AxdiReadInportDword(15, 0, &m_DX15.nValue);
#endif
}

void CAJinAXL::Read_Output()
{
#ifdef AJIN_BOARD_USE
	AxdoReadOutportDword(16, 0, &m_DY00.nValue);
	AxdoReadOutportDword(17, 0, &m_DY01.nValue);
	AxdoReadOutportDword(18, 0, &m_DY02.nValue);
	AxdoReadOutportDword(19, 0, &m_DY03.nValue);
	AxdoReadOutportDword(20, 0, &m_DY04.nValue);
	AxdoReadOutportDword(21, 0, &m_DY05.nValue);
	AxdoReadOutportDword(22, 0, &m_DY06.nValue);
	AxdoReadOutportDword(23, 0, &m_DY07.nValue);
	AxdoReadOutportDword(24, 0, &m_DY08.nValue);
	AxdoReadOutportDword(25, 0, &m_DY09.nValue);
	AxdoReadOutportDword(26, 0, &m_DY10.nValue);
	AxdoReadOutportDword(27, 0, &m_DY11.nValue);
	AxdoReadOutportDword(28, 0, &m_DY12.nValue);
	AxdoReadOutportDword(29, 0, &m_DY13.nValue);
	AxdoReadOutportDword(30, 0, &m_DY14.nValue);
	AxdoReadOutportDword(31, 0, &m_DY15.nValue);
#endif
}

void CAJinAXL::Write_Output(int nModule)
{
#ifdef AJIN_BOARD_USE
	if (nModule ==  0) AxdoWriteOutportDword(16, 0, m_DY00.nValue);
	if (nModule ==  1) AxdoWriteOutportDword(17, 0, m_DY01.nValue);
	if (nModule ==  2) AxdoWriteOutportDword(18, 0, m_DY02.nValue);
	if (nModule ==  3) AxdoWriteOutportDword(19, 0, m_DY03.nValue);
	if (nModule ==  4) AxdoWriteOutportDword(20, 0, m_DY04.nValue);
	if (nModule ==  5) AxdoWriteOutportDword(21, 0, m_DY05.nValue);
	if (nModule ==  6) AxdoWriteOutportDword(22, 0, m_DY06.nValue);
	if (nModule ==  7) AxdoWriteOutportDword(23, 0, m_DY07.nValue);
	if (nModule ==  8) AxdoWriteOutportDword(24, 0, m_DY08.nValue);
	if (nModule ==  9) AxdoWriteOutportDword(25, 0, m_DY09.nValue);
	if (nModule == 10) AxdoWriteOutportDword(26, 0, m_DY10.nValue);
	if (nModule == 11) AxdoWriteOutportDword(27, 0, m_DY11.nValue);
	if (nModule == 12) AxdoWriteOutportDword(28, 0, m_DY12.nValue);
	if (nModule == 13) AxdoWriteOutportDword(29, 0, m_DY13.nValue);
	if (nModule == 14) AxdoWriteOutportDword(30, 0, m_DY14.nValue);
	if (nModule == 15) AxdoWriteOutportDword(31, 0, m_DY15.nValue);
#else
	Sim_SetOutToIn(nModule);
#endif
}

void CAJinAXL::Read_MotionStatus()
{
#ifdef AJIN_BOARD_USE
	DWORD dwStatus;
	for (int i = 0; i < AXIS_COUNT; i++) {
		//AxmStatusGetCmdPos(i, &m_Status[i].dPos);		// Position Reading
		AxmStatusGetActPos(i, &m_Status[i].dPos);		// Position Reading

		if (m_bReadVelocity) AxmStatusReadVel(i, &m_Status[i].dVel);	// Velocity Reading

		AxmSignalIsServoOn(i, &dwStatus);								// Servo-On Reading
		m_Status[i].bSOn = (BOOL)dwStatus;

		AxmSignalReadInputBit(i, UIO_INP0, &dwStatus);	// Origin Limit (In0 : Home)
		m_Status[i].bOrg = (BOOL)dwStatus;

		AxmStatusReadMechanical(i, &dwStatus);	// Mechanical Input
		m_Status[i].bELP = (BOOL)((dwStatus >> 0) & 1);	// Positive Limit (bit0)
		m_Status[i].bELN = (BOOL)((dwStatus >> 1) & 1);	// Negative Limit (bit1)
		m_Status[i].bALM = (BOOL)((dwStatus >> 4) & 1);	// Alarm (bit4)
		m_Status[i].bInP = (BOOL)((dwStatus >> 5) & 1);	// In-Position (bit5)

		AxmStatusReadInMotion(i, &dwStatus);			// Motion Running
		m_Status[i].bRun = (BOOL)dwStatus;

		AxmHomeGetResult(i, &dwStatus);					// Home Done
		m_Status[i].bHom = (dwStatus == HOME_SUCCESS ? TRUE : FALSE);
	}
#endif
}

void CAJinAXL::Set_ServoOn(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmMoveEStop(nAxis);
	AxmSignalServoOn(nAxis, TRUE);
#endif
}

void CAJinAXL::Set_ServoOff(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmMoveEStop(nAxis);
	AxmSignalServoOn(nAxis, FALSE);
#endif
}

void CAJinAXL::Home_Search(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmHomeSetStart(nAxis);
	m_Status[nAxis].bHom = FALSE; m_Status[nAxis].bRun = TRUE;
#else
	m_Status[nAxis].bHom = TRUE;
#endif
}

void CAJinAXL::Set_Home(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmStatusSetActPos(nAxis, 0.0);
	AxmStatusSetCmdPos(nAxis, 0.0);
	AxmHomeSetResult(nAxis, HOME_SUCCESS);
#endif
}

void CAJinAXL::Move_Absolute(int nAxis, double dPos, double dSpeed)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_ABS_MODE);
	if (dSpeed <= 0.0) dSpeed = 1.0;
	double	dVel = m_Param[nAxis].dSpeedM * dSpeed;
	double	dAcc = m_Param[nAxis].dAccel;
	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dAcc);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#else
	Sim_SetMotion(1, nAxis, dPos);
#endif
}

void CAJinAXL::Move_Absolute_Vel(int nAxis, double dPos, double overrideSpeed)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_ABS_MODE);
	double	dVel = overrideSpeed;
	double	dAcc = m_Param[nAxis].dAccel;
	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dAcc);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#else
	Sim_SetMotion(1,nAxis, dPos);
#endif

}

void CAJinAXL::Move_AbsSlow(int nAxis, double dPos)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_ABS_MODE);
	double	dVel = m_Param[nAxis].dSpeedM / 5.0;
	double	dAcc = m_Param[nAxis].dAccel;
	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dAcc);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#else
	Sim_SetMotion(1,nAxis, dPos);
#endif
}

void CAJinAXL::Move_Relative(int nAxis, double dPos, double dSpeed)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_REL_MODE);
	double	dVel = m_Param[nAxis].dSpeedM;
	double	dAcc = m_Param[nAxis].dAccel;
	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dAcc);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#else
	Sim_SetMotion(2,nAxis, dPos);
#endif
}

void CAJinAXL::Jog_Positive(int nAxis)
{
#ifdef AJIN_BOARD_USE
	double	dVel = m_Param[nAxis].dSpeedJ;
	double	dAcc = m_Param[nAxis].dSpeedJ * 4.0;
	AxmMoveVel(nAxis, dVel, dAcc, dAcc);
#endif
}

void CAJinAXL::Jog_Negative(int nAxis)
{
#ifdef AJIN_BOARD_USE
	double	dVel = m_Param[nAxis].dSpeedJ * -1.0;
	double	dAcc = m_Param[nAxis].dSpeedJ * 4.0;
	AxmMoveVel(nAxis, dVel, dAcc, dAcc);
#endif
}

void CAJinAXL::Stop_Motion(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmMoveSStop(nAxis);
#endif
}

void CAJinAXL::EStop_Motion(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmMoveEStop(nAxis);
#endif
}

void CAJinAXL::Alarm_Reset(int nAxis)
{
#ifdef AJIN_BOARD_USE
	int nAxStart = (nAxis == -1 ? 0 : nAxis);
	int nAxEnd = (nAxis == -1 ? AXIS_COUNT : nAxis + 1);
	for (int i = nAxStart; i < nAxEnd; i++) AxmMoveEStop(i);
	Sleep(200);
	for (int i = nAxStart; i < nAxEnd; i++) AxmSignalServoAlarmReset(i, TRUE);
	Sleep(200);
	for (int i = nAxStart; i < nAxEnd; i++) AxmSignalServoAlarmReset(i, FALSE);
#endif
}

void CAJinAXL::Move_Abs_Override(int nAxis, double dPos, double dAt)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_ABS_MODE);
	double	dVel = m_Param[nAxis].dSpeedM;
	double	dAcc = m_Param[nAxis].dSpeedM * 4.0;
	double dMaxVel;
	AxmMotGetMaxVel(nAxis, &dMaxVel);

	AxmOverrideSetMaxVel(nAxis, dMaxVel);	// 오버라이드 최대값 설정
	AxmOverrideVelAtPos (nAxis, dPos, dVel, dAcc, dAcc, dAt, dVel/4.0, COMMAND);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#else
	Sim_SetMotion(3, nAxis, dPos);
#endif
}

void CAJinAXL::Move_Rel_Override(int nAxis, double dPos, double dAt)
{
#ifdef AJIN_BOARD_USE
	AxmMotSetAbsRelMode(nAxis, POS_REL_MODE);
	double	dVel = m_Param[nAxis].dSpeedM;
	double	dAcc = m_Param[nAxis].dSpeedM * 4.0;
	double dMaxVel;
	AxmMotGetMaxVel(nAxis, &dMaxVel);
	
	AxmOverrideSetMaxVel(nAxis, dMaxVel);	// 오버라이드 최대값 설정
	AxmOverrideVelAtPos (nAxis, dPos, dVel, dAcc, dAcc, dAt, dVel/4.0, COMMAND);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#endif
}

BOOL CAJinAXL::Is_Done(int nAxis)
{
#ifdef AJIN_BOARD_USE
	if (m_Status[nAxis].bRun) return FALSE;
	if (!m_Status[nAxis].bInP) return FALSE;
#endif
	return TRUE;
}

BOOL CAJinAXL::Is_Home(int nAxis)
{
#ifdef AJIN_BOARD_USE
	if (!m_Status[nAxis].bHom) return FALSE;
	if (m_Status[nAxis].bRun) return FALSE;
#endif
	return TRUE;
}

BOOL CAJinAXL::Is_MoveDone(int nAxis, double dPos, double dRange)
{
#ifdef AJIN_BOARD_USE
	if (!Is_Done(nAxis)) return FALSE;
	if (fabs(m_Status[nAxis].dPos - dPos) > dRange) return FALSE;
#endif
	return TRUE;
}

void CAJinAXL::Start_Trigger(int nAxis, double dStartPos, double dEndPos, double dPeriod, double dWidth)
{
#ifdef AJIN_BOARD_USE
	DWORD dwReturn = AxmTriggerSetReset(nAxis);

	double dTrigTime = dPeriod *2 ;	// mm->usec

	// 1. Command Position ****************************************************
// 	AxmTriggerSetTimeLevel(nAxis, dTrigTime, HIGH, COMMAND, DISABLE);
	// 2. Actual Position *****************************************************
	dwReturn = AxmTriggerSetTimeLevel(nAxis, dTrigTime, HIGH, ACTUAL, DISABLE);
	//*************************************************************************
	
	DWORD dwCode = AxmTriggerSetBlock(nAxis, dStartPos, dEndPos, dPeriod);
	if (dwCode != AXT_RT_SUCCESS) AfxMessageBox("Trigger Setting Error");
#endif
}

void CAJinAXL::Stop_Trigger(int nAxis)
{
#ifdef AJIN_BOARD_USE
	DWORD dwReturn = AxmTriggerSetReset(nAxis);
#endif
}

void CAJinAXL::Start_Scan(int nAxis, double dPos, double dVel, double dTrigStart, double dTrigEnd, double dPeriod, double dWidth)
{
#ifdef AJIN_BOARD_USE
	// Trigger Setting
	AxmTriggerSetReset(nAxis);
	
	dPeriod /= 1000;
	dWidth /= 1000;

	double dTrigTime = dWidth / dVel * 1000000.0;	// mm->usec

	//dTrigTime = 20.0;	// mm->usec

	// 1. Command Position ****************************************************
// 	AxmTriggerSetTimeLevel(nAxis, dTrigTime, HIGH, COMMAND, DISABLE);
	// 2. Actual Position *****************************************************
	AxmTriggerSetTimeLevel(nAxis, dTrigTime, HIGH, ACTUAL, DISABLE);
	//*************************************************************************
	
	DWORD dwCode = AxmTriggerSetBlock(nAxis, dTrigStart, dTrigEnd, dPeriod);
	if (dwCode != AXT_RT_SUCCESS) AfxMessageBox("Trigger Setting Error");

	// Scan Move
	AxmMotSetAbsRelMode(nAxis, POS_ABS_MODE);
	double	dAcc = dVel * 10.0;
	AxmMoveStartPos(nAxis, dPos, dVel, dAcc, dAcc);
	m_Status[nAxis].bInP = FALSE; m_Status[nAxis].bRun = TRUE;
#endif
}

void CAJinAXL::Stop_Scan(int nAxis)
{
#ifdef AJIN_BOARD_USE
	AxmTriggerSetReset(nAxis);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Motion Param Read / Write Function

void CAJinAXL::Read_AxisList()
{
	CIniFileCS INI(gData.sEnvPath + "\\AxisList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("AxisList.ini File Not Found!!!");
		return;
	}

	CString strSection, strName;
	for (int i = 0; i < AXIS_COUNT; i++) {
		strSection.Format("AXIS_%02d", i);

		strName = INI.Get_String(strSection, "NAME", "");
		m_strAxisName[i].Format("[%02d] %s", i, strName);
		m_Param[i].dSpeedM = INI.Get_Double(strSection, "MOVE", 0.0);
		m_Param[i].dSpeedJ = INI.Get_Double(strSection, "JOG", 0.0);
		m_Param[i].dAccel = INI.Get_Double(strSection, "ACC", 0.0);
	}
}

void CAJinAXL::Read_MotionParam(int nAxis)
{
	CIniFileCS INI(gData.sEnvPath + "\\AxisList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("AxisList.ini File Not Found!!!");
		return;
	}
	
	CString strSection;
	strSection.Format("AXIS_%02d", nAxis);
	m_Param[nAxis].dSpeedM = INI.Get_Double(strSection, "MOVE", 0.0);
	m_Param[nAxis].dSpeedJ = INI.Get_Double(strSection, "JOG", 0.0);
	m_Param[nAxis].dAccel = INI.Get_Double(strSection, "ACC", 0.0);
}

void CAJinAXL::Save_MotionParam(int nAxis, double dSpeedM, double dSpeedJ, double dAccel)
{
	CIniFileCS INI(gData.sEnvPath + "\\AxisList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("AxisList.ini File Not Found!!!");
		return;
	}
	
	CString strSection;
	strSection.Format("AXIS_%02d", nAxis);
	INI.Set_Double(strSection, "MOVE", dSpeedM, "%0.3lf");
	INI.Set_Double(strSection, "JOG", dSpeedJ, "%0.3lf");
	INI.Set_Double(strSection, "ACC", dAccel, "%0.3lf");

	// RMS 항목
	Save_RmsMotionSpeed(nAxis, dSpeedM, dAccel);
}

void CAJinAXL::Save_MotionParamForAll(int nAxis)
{
	CIniFileCS INI(gData.sEnvPath + "\\AllParam.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("AllParam.ini File Not Found!!!");
		return;
	}

	CIniFileCS INIRead(gData.sEnvPath + "\\AxisList.ini");
	if (!INIRead.Check_File()) {
		AfxMessageBox("AxisList.ini File Not Found!!!");
		return;
	}

	CString strSection;
	strSection.Format("AXIS_%02d", nAxis);
	m_Param[nAxis].dSpeedM = INIRead.Get_Double(strSection, "MOVE", 0.0);
	m_Param[nAxis].dSpeedJ = INIRead.Get_Double(strSection, "JOG", 0.0);
	m_Param[nAxis].dAccel = INIRead.Get_Double(strSection, "ACC", 0.0);

	
	strSection.Format("AXIS_%02d", nAxis);
	INI.Set_Double(strSection, "MOVE", m_Param[nAxis].dSpeedM, "%0.3lf");
	INI.Set_Double(strSection, "JOG", m_Param[nAxis].dSpeedJ, "%0.3lf");
	INI.Set_Double(strSection, "ACC", m_Param[nAxis].dAccel, "%0.3lf");

	
}

void CAJinAXL::Save_RmsMotionSpeed(int nAxis, double dSpeedM, double dAccel)
{	
	CString strAxis = "", strName, strSpeed, strAccel;

	if		(nAxis == AX_LOAD_STAGE_X)		strAxis = "Load Stage X";
	else if (nAxis == AX_LOAD_STAGE_Z)		strAxis = "Load Stage Z";
	else if (nAxis == AX_LOAD_PICKER_Y)		strAxis = "Load Picker Y";
	else if (nAxis == AX_ANGLE_UNIT_Y)		strAxis = "Angle Unit Y";
	else if (nAxis == AX_ANGLE_UNIT_X)		strAxis = "Angle Unit X";
	else if (nAxis == AX_ANGLE_UNIT_R)		strAxis = "Angle Unit R";
	else if (nAxis == AX_VISION_ANGLE_Z)	strAxis = "Vision Angle Z";
	else if (nAxis == AX_VISION_ALIGN_Z)	strAxis = "Vision Align Z";
	else if (nAxis == AX_ANGLE_STAGE1_Y)	strAxis = "Angle Stage1 Y";
	else if (nAxis == AX_ANGLE_STAGE1_Z)	strAxis = "Angle Stage1 Z";
	else if (nAxis == AX_ANGLE_STAGE2_Y)	strAxis = "Angle Stage2 Y";
	else if (nAxis == AX_ANGLE_STAGE2_Z)	strAxis = "Angle Stage2 Z";
	else if (nAxis == AX_BTM1_PICKER_X)		strAxis = "BTM1 Picker X";
	else if (nAxis == AX_BTM1_PICKER_Z)		strAxis = "BTM1 Picker Z";
	else if (nAxis == AX_BTM1_PICKER_P1)	strAxis = "BTM1 Picker P1";
	else if (nAxis == AX_BTM1_PICKER_P2)	strAxis = "BTM1 Picker P2";
	else if (nAxis == AX_INSPECT_STAGE1_X)	strAxis = "Inspection Stage1 X";
	else if (nAxis == AX_INSPECT_STAGE2_X)	strAxis = "Inspection Stage2 X";
	else if (nAxis == AX_INSPECT_STAGE3_X)	strAxis = "Inspection Stage3 X";
	else if (nAxis == AX_MODULE_ALIGN_Y)	strAxis = "Module Align Y";
	else if (nAxis == AX_MODULE_ALIGN_Z)	strAxis = "Module Align Z";
	else if (nAxis == AX_VISION_TOP1_Z)		strAxis = "Vision TOP1 Z";
	else if (nAxis == AX_TOP1_MIRROR_Z)		strAxis = "TOP1 Mirror Z";
	else if (nAxis == AX_TOP2_SHIFT_Y)		strAxis = "TOP2 Shift Y";
	else if (nAxis == AX_VISION_TOP2_Z)		strAxis = "Vision TOP2 Z";
	else if (nAxis == AX_BTM2_PICKER_X)		strAxis = "BTM2 Picker X";
	else if (nAxis == AX_BTM2_PICKER_Z)		strAxis = "BTM2 Picker Z";
	else if (nAxis == AX_BTM2_PICKER_P1)	strAxis = "BTM2 Picker P1";
	else if (nAxis == AX_BTM2_PICKER_P2)	strAxis = "BTM2 Picker P2";
	else if (nAxis == AX_BUFFER_STAGE1_Y)	strAxis = "Buffer Stage1 Y";
	else if (nAxis == AX_BUFFER_STAGE2_Y)	strAxis = "Buffer Stage2 Y";
	else if (nAxis == AX_SORT_PICKER1_X)	strAxis = "Sort Picker1 X";
	else if (nAxis == AX_SORT_PICKER1_Z)	strAxis = "Sort Picker1 Z";
	else if (nAxis == AX_SORT_PICKER1_P)	strAxis = "Sort Picker1 P";
	else if (nAxis == AX_SORT_PICKER2_X)	strAxis = "Sort Picker2 X";
	else if (nAxis == AX_SORT_PICKER2_Z)	strAxis = "Sort Picker2 Z";
	else if (nAxis == AX_SORT_PICKER2_P)	strAxis = "Sort Picker2 P";
	else if (nAxis == AX_GOOD_STAGE1_Y)		strAxis = "Good Stage1 Y";
	else if (nAxis == AX_GOOD_STAGE1_Z)		strAxis = "Good Stage1 Z";
	else if (nAxis == AX_GOOD_STAGE2_Y)		strAxis = "Good Stage2 Y";
	else if (nAxis == AX_GOOD_STAGE2_Z)		strAxis = "Good Stage2 Z";
	else if (nAxis == AX_NG_STAGE_Y)		strAxis = "NG Stage Y";
	else if (nAxis == AX_EMPTY_TRANS1_X)	strAxis = "Empty Trans1 X";
	else if (nAxis == AX_EMPTY_PORT_Z)		strAxis = "Empty Port Z";
	else if (nAxis == AX_EMPTY_TRANS2_Y)	strAxis = "Empty Trans2 Y";
	else if (nAxis == AX_BTM1_SHIFT_Y)		strAxis = "Btm1 Angle23 Y";

	if (strAxis == "") return;

	strSpeed.Format("%0.3lf", dSpeedM);
	strAccel.Format("%0.3lf", dAccel);

	strName.Format("%s Speed", strAxis); g_objMES.Save_AviRmsData(strName, strSpeed);
	strName.Format("%s Accel", strAxis); g_objMES.Save_AviRmsData(strName, strAccel);
}

/////////////////////////////////////////////////////////////////////////////
// Thread Function 
UINT CAJinAXL::Thread_AJin(LPVOID lpVoid)
{
	while (g_objAJinAXL.m_bThreadAJin) {
		g_objAJinAXL.Read_Input();
		g_objAJinAXL.Read_MotionStatus();
		Sleep(5);
	}
	g_objAJinAXL.m_bThreadAJin = FALSE;
	g_objAJinAXL.m_pThreadAJin = NULL;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CAJinAXL::Use_OrgAxis(int nAxis)
{
	switch (nAxis) {
	case AX_ANGLE_UNIT_R:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL CAJinAXL::Use_ElpAxis(int nAxis)
{
	if (Use_OrgAxis(nAxis)) return FALSE;
	else return TRUE;
}

BOOL CAJinAXL::Use_ElnAxis(int nAxis)
{
	if (Use_OrgAxis(nAxis)) return FALSE;
	else return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

DXY_DATA *CAJinAXL::Get_pDX(int nIndex)
{
	if (nIndex ==  0) return (DXY_DATA*)&m_DX00;
	if (nIndex ==  1) return (DXY_DATA*)&m_DX01;
	if (nIndex ==  2) return (DXY_DATA*)&m_DX02;
	if (nIndex ==  3) return (DXY_DATA*)&m_DX03;
	if (nIndex ==  4) return (DXY_DATA*)&m_DX04;
	if (nIndex ==  5) return (DXY_DATA*)&m_DX05;
	if (nIndex ==  6) return (DXY_DATA*)&m_DX06;
	if (nIndex ==  7) return (DXY_DATA*)&m_DX07;
	if (nIndex ==  8) return (DXY_DATA*)&m_DX08;
	if (nIndex ==  9) return (DXY_DATA*)&m_DX09;
	if (nIndex == 10) return (DXY_DATA*)&m_DX10;
	if (nIndex == 11) return (DXY_DATA*)&m_DX11;
	if (nIndex == 12) return (DXY_DATA*)&m_DX12;
	if (nIndex == 13) return (DXY_DATA*)&m_DX13;
	if (nIndex == 14) return (DXY_DATA*)&m_DX14;
	if (nIndex == 15) return (DXY_DATA*)&m_DX15;
	return NULL;
}

DXY_DATA *CAJinAXL::Get_pDY(int nIndex)
{
	if (nIndex ==  0) return (DXY_DATA*)&m_DY00;
	if (nIndex ==  1) return (DXY_DATA*)&m_DY01;
	if (nIndex ==  2) return (DXY_DATA*)&m_DY02;
	if (nIndex ==  3) return (DXY_DATA*)&m_DY03;
	if (nIndex ==  4) return (DXY_DATA*)&m_DY04;
	if (nIndex ==  5) return (DXY_DATA*)&m_DY05;
	if (nIndex ==  6) return (DXY_DATA*)&m_DY06;
	if (nIndex ==  7) return (DXY_DATA*)&m_DY07;
	if (nIndex ==  8) return (DXY_DATA*)&m_DY08;
	if (nIndex ==  9) return (DXY_DATA*)&m_DY09;
	if (nIndex == 10) return (DXY_DATA*)&m_DY10;
	if (nIndex == 11) return (DXY_DATA*)&m_DY11;
	if (nIndex == 12) return (DXY_DATA*)&m_DY12;
	if (nIndex == 13) return (DXY_DATA*)&m_DY13;
	if (nIndex == 14) return (DXY_DATA*)&m_DY14;
	if (nIndex == 15) return (DXY_DATA*)&m_DY15;
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void CAJinAXL::Save_AxisList()
{
	CIniFileCS INI(gData.sEnvPath + "\\AxisList.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("AxisList.ini File Not Found!!!");
		return;
	}

	CString strAxis[46];

	strAxis[0 ] = "LOAD STAGE X";
	strAxis[1 ] = "LOAD STAGE Z";
	strAxis[2 ] = "LOAD PICKER Y";
	strAxis[3 ] = "ANGLE UNIT Y";
	strAxis[4 ] = "ANGLE UNIT X";
	strAxis[5 ] = "ANGLE UNIT R";
	strAxis[6 ] = "VISION ANGLE Z";
	strAxis[7 ] = "VISION ALIGN Z";
	strAxis[8 ] = "ANGLE STAGE1 Y";
	strAxis[9 ] = "ANGLE STAGE1 Z";
	strAxis[10] = "ANGLE STAGE2 Y";
	strAxis[11] = "ANGLE STAGE2 Z";
	strAxis[12] = "BTM1 PICKER X";
	strAxis[13] = "BTM1 PICKER Z";
	strAxis[14] = "BTM1 PICKER P1";
	strAxis[15] = "BTM1 PICKER P2";
	strAxis[16] = "BTM1 SHIFT Y";
	strAxis[17] = "INSPECT STAGE1 X";
	strAxis[18] = "INSPECT STAGE2 X";
	strAxis[19] = "INSPECT STAGE3 X";
	strAxis[20] = "MODULE ALIGN Y";
	strAxis[21] = "MODULE ALIGN Z";
	strAxis[22] = "VISION TOP1 Z";
	strAxis[23] = "TOP1 MIRROR Z";
	strAxis[24] = "TOP2 SHIFT Y";
	strAxis[25] = "VISION TOP2 Z";
	strAxis[26] = "BTM2 PICKER X";
	strAxis[27] = "BTM2 PICKER Z";
	strAxis[28] = "BTM2 PICKER P1";
	strAxis[29] = "BTM2 PICKER P2";
	strAxis[30] = "BUFFER STAGE1 Y";
	strAxis[31] = "BUFFER STAGE2 Y";
	strAxis[32] = "SORT PICKER1 X";
	strAxis[33] = "SORT PICKER1 Z";
	strAxis[34] = "SORT PICKER1 P";
	strAxis[35] = "SORT PICKER2 X";
	strAxis[36] = "SORT PICKER2 Z";
	strAxis[37] = "SORT PICKER2 P";
	strAxis[38] = "GOOD STAGE1 Y";
	strAxis[39] = "GOOD STAGE1 Z";
	strAxis[40] = "GOOD STAGE2 Y";
	strAxis[41] = "GOOD STAGE2 Z";
	strAxis[42] = "NG STAGE Y";
	strAxis[43] = "EMPTY TRANS1 X";
	strAxis[44] = "EMPTY PORT Z";
	strAxis[45] = "EMPTY TRANS2 Y";

	CString strSection, strName;
	for (int i = 0; i < AXIS_COUNT; i++) {
		strSection.Format("AXIS_%02d", i);
		INI.Set_String(strSection, "NAME", strAxis[i]);
	}
}



///////////////////////////////////////////////////////////////////////////////

void CAJinAXL::Sim_SetMotion(int nNo, int nAxis, double dPos)
{
	Sleep(SIM_WAITTIMEM);
	if		(nNo == 1)	g_objAJinAXL.m_Status[nAxis].dPos = dPos;
	else if (nNo == 2)	g_objAJinAXL.m_Status[nAxis].dPos = g_objAJinAXL.m_Status[nAxis].dPos + dPos;
	else if (nNo == 3)	g_objAJinAXL.m_Status[nAxis].dPos = dPos;
}

void CAJinAXL::Sim_SetOutToIn(int nNo)
{
	Sleep(SIM_WAITTIMES);
	BOOL bT = TRUE;
	BOOL bF = FALSE;

	if (nNo==0) {
		m_DX00.iLoadPort1Support1In	 = m_DY00.oLoadPort1SupportIn;
		m_DX00.iLoadPort1Support2In	 = m_DY00.oLoadPort1SupportIn;

		m_DX00.iLoadPort1Support1Out		 = m_DY00.oLoadPort1SupportOut;
		m_DX00.iLoadPort1Support2Out		 = m_DY00.oLoadPort1SupportOut;
		
		m_DX00.iLoadPort1SlideLock		 = m_DY00.oLoadPort1SlideLock;
		m_DX00.iLoadPort1SlideUnlock		 = m_DY00.oLoadPort1SlideUnlock;

		m_DX00.iLoadPort2Support1In		 = m_DY00.oLoadPort2SupportIn;
		m_DX00.iLoadPort2Support1Out		 = m_DY00.oLoadPort2SupportOut;

		m_DX00.iLoadPort2Support2In		 = m_DY00.oLoadPort2SupportIn;
		m_DX00.iLoadPort2Support2Out		 = m_DY00.oLoadPort2SupportOut;

		m_DX00.iLoadPort2SlideLock		 = m_DY00.oLoadPort2SlideLock;
		m_DX00.iLoadPort2SlideUnlock		 = m_DY00.oLoadPort2SlideUnlock;
	}

	if (nNo==1) {
		if(m_DY01.oLoadStageMasterIn == FALSE) 
		{
			m_DX01.iLoadStageMasterIn = FALSE;
			m_DX01.iLoadStageMasterOut = TRUE;
		}
		else 
		{
			m_DX01.iLoadStageMasterIn = TRUE;
			m_DX01.iLoadStageMasterOut = FALSE;
		}
		
		if(m_DY01.oLoadStageSlaveIn == FALSE)
		{ 
			m_DX01.iLoadStageSlaveIn = FALSE;
			m_DX01.iLoadStageSlaveOut = TRUE;
		}
		else 
		{ 
			m_DX01.iLoadStageSlaveIn = TRUE;
			m_DX01.iLoadStageSlaveOut = FALSE;
		}
		m_DX01.iLoadPickerUp  = m_DY01.oLoadPickerUp;
		m_DX01.iLoadPickerDown = m_DY01.oLoadPickerDown;

		m_DX01.iLoadPickerMasterIn   = m_DY01.oLoadPickerMasterIn;
		m_DX01.iLoadPickerMasterOut  = m_DY01.oLoadPickerMasterOut;
		m_DX01.iLoadPickerSlaveIn   = m_DY01.oLoadPickerSlaveIn;
		m_DX01.iLoadPickerSlaveOut  = m_DY01.oLoadPickerSlaveOut;
		
	}

	if (nNo==2) {
		m_DX02.iAnglePortSupport1In    = m_DY02.oAnglePortSupportIn;
		m_DX02.iAnglePortSupport1Out  = m_DY02.oAnglePortSupportOut;
		m_DX02.iAnglePortSupport2In    = m_DY02.oAnglePortSupportIn;
		m_DX02.iAnglePortSupport2Out  = m_DY02.oAnglePortSupportOut;
		m_DX02.iAnglePortMasterIn    = m_DY02.oAnglePortMasterIn;
		m_DX02.iAnglePortMasterOut  = m_DY02.oAnglePortMasterOut;
		m_DX02.iAnglePortSlaveIn    = m_DY02.oAnglePortSlaveIn;
		m_DX02.iAnglePortSlaveOut  = m_DY02.oAnglePortSlaveOut;

		if(m_DY02.oAngleStage1AlignIn == FALSE)
		{
			m_DX02.iAngleStage1AlignIn = FALSE;
			m_DX02.iAngleStage1AlignOut = TRUE;
		}
		else
		{
			m_DX02.iAngleStage1AlignIn    = TRUE;
			m_DX02.iAngleStage1AlignOut = FALSE;
		}
		
		if(m_DY02.oAngleStage1Rotate == FALSE)
		{
			m_DX02.iAngleStage1Rotate = FALSE;
			m_DX02.iAngleStage1Normal = TRUE;
		}
		else
		{
			m_DX02.iAngleStage1Rotate  = TRUE;
			m_DX02.iAngleStage1Normal = FALSE;
		}

		if(m_DY02.oAngleStage2AlignIn == FALSE)
		{
			m_DX02.iAngleStage2AlignIn = FALSE;
			m_DX02.iAngleStage2AlignOut = TRUE;
		}
		else
		{
			m_DX02.iAngleStage2AlignIn    = TRUE;
			m_DX02.iAngleStage2AlignOut = FALSE;
		}

		if(m_DY02.oAngleStage2Rotate == FALSE) 
		{
			m_DX02.iAngleStage2Rotate = FALSE;
			m_DX02.iAngleStage2Normal = TRUE;
		}
		else
		{
			m_DX02.iAngleStage2Rotate = TRUE;
			m_DX02.iAngleStage2Normal = FALSE;
		}

	}

	if (nNo==3) {
		m_DX03.iBtm1PickerOpen01			 = m_DY03.oBtm1PickerOpen01;
		m_DX03.iBtm1PickerOpen02	     	 = m_DY03.oBtm1PickerOpen02;
		m_DX03.iBtm1PickerOpen03			 = m_DY03.oBtm1PickerOpen03;
		m_DX03.iBtm1PickerOpen04	     	 = m_DY03.oBtm1PickerOpen04;
		m_DX03.iBtm1PickerOpen05			 = m_DY03.oBtm1PickerOpen05;
		m_DX03.iBtm1PickerOpen06	     	 = m_DY03.oBtm1PickerOpen06;
		m_DX03.iBtm1PickerOpen07			 = m_DY03.oBtm1PickerOpen07;
		m_DX03.iBtm1PickerOpen08	     	 = m_DY03.oBtm1PickerOpen08;
		
		//close되면 Exist 인가? 체크해 봐야 한다.
		m_DX03.iBtm1PickerExist01			 = m_DY03.oBtm1PickerClose01;
		m_DX03.iBtm1PickerExist02	     	 = m_DY03.oBtm1PickerClose02;
		m_DX03.iBtm1PickerExist03			 = m_DY03.oBtm1PickerClose03;
		m_DX03.iBtm1PickerExist04	     	 = m_DY03.oBtm1PickerClose04;
		m_DX03.iBtm1PickerExist05			 = m_DY03.oBtm1PickerClose05;
		m_DX03.iBtm1PickerExist06	     	 = m_DY03.oBtm1PickerClose06;
		m_DX03.iBtm1PickerExist07			 = m_DY03.oBtm1PickerClose07;
		m_DX03.iBtm1PickerExist08	     	 = m_DY03.oBtm1PickerClose08;

		//if up 이면??
		if(m_DY03.oBtm1PickerDown01 == TRUE){
			m_DX03.iBtm1PickerDown01 = TRUE;
			m_DX03.iBtm1PickerUp01 = FALSE;
		}
		else{
			m_DX03.iBtm1PickerDown01 = FALSE;
			m_DX03.iBtm1PickerUp01 = TRUE;
		}

		if(m_DY03.oBtm1PickerDown02 == TRUE){
			m_DX03.iBtm1PickerDown02 = TRUE;
			m_DX03.iBtm1PickerUp02 = FALSE;
		}
		else{
			m_DX03.iBtm1PickerDown01 = FALSE;
			m_DX03.iBtm1PickerUp01 = TRUE;
		}

		if(m_DY03.oBtm1PickerDown03 == TRUE){
			m_DX03.iBtm1PickerDown03 = TRUE;
			m_DX03.iBtm1PickerUp03 = FALSE;
		}
		else{
			m_DX03.iBtm1PickerDown03 = FALSE;
			m_DX03.iBtm1PickerUp03 = TRUE;
		}


		if(m_DY03.oBtm1PickerDown04 == TRUE){
			m_DX03.iBtm1PickerDown04 = TRUE;
			m_DX03.iBtm1PickerUp04 = FALSE;
		}
		else{
			m_DX03.iBtm1PickerDown04 = FALSE;
			m_DX03.iBtm1PickerUp04 = TRUE;
		}
	}

	if (nNo==4) {
		m_DX04.iBtm1PickerVac01 = m_DY04.oBtm1PickerVac01;
		m_DX04.iBtm1PickerVac02 = m_DY04.oBtm1PickerVac02;
		m_DX04.iBtm1PickerVac03 = m_DY04.oBtm1PickerVac03;
		m_DX04.iBtm1PickerVac04 = m_DY04.oBtm1PickerVac04;
		m_DX04.iBtm1PickerVac05 = m_DY04.oBtm1PickerVac05;
		m_DX04.iBtm1PickerVac06 = m_DY04.oBtm1PickerVac06;
		m_DX04.iBtm1PickerVac07 = m_DY04.oBtm1PickerVac07;
		m_DX04.iBtm1PickerVac08 = m_DY04.oBtm1PickerVac08;


		//if up 이면 ?
		if(m_DY04.oBtm1PickerDown05 == TRUE){
			m_DX04.iBtm1PickerDown05 = TRUE;
			m_DX04.iBtm1PickerUp05 = FALSE;
		}
		else{
			m_DX04.iBtm1PickerDown05 = FALSE;
			m_DX04.iBtm1PickerUp05 = TRUE;
		}
		if(m_DY04.oBtm1PickerDown06 == TRUE){
			m_DX04.iBtm1PickerDown06 = TRUE;
			m_DX04.iBtm1PickerUp06 = FALSE;
		}
		else{
			m_DX04.iBtm1PickerDown06 = FALSE;
			m_DX04.iBtm1PickerUp06 = TRUE;
		}
		if(m_DY04.oBtm1PickerDown07 == TRUE){
			m_DX04.iBtm1PickerDown07 = TRUE;
			m_DX04.iBtm1PickerUp07 = FALSE;
		}
		else{
			m_DX04.iBtm1PickerDown07 = FALSE;
			m_DX04.iBtm1PickerUp07 = TRUE;
		}
		if(m_DY04.oBtm1PickerDown08 == TRUE){
			m_DX04.iBtm1PickerDown08 = TRUE;
			m_DX04.iBtm1PickerUp08 = FALSE;
		}
		else{
			m_DX04.iBtm1PickerDown08 = FALSE;
			m_DX04.iBtm1PickerUp08 = TRUE;
		}		
	}

	if (nNo==5) {
		m_DX05.iInspectStage1Up  = m_DY05.oInspectStage1Up;
		m_DX05.iInspectStage1Down  = m_DY05.oInspectStage1Down;

		m_DX05.iInspectStage2Up  = m_DY05.oInspectStage2Up;
		m_DX05.iInspectStage2Down  = m_DY05.oInspectStage2Down;
		m_DX05.iInspectStage2Fwd  = m_DY05.oInspectStage2Fwd;
		m_DX05.iInspectStage2Bwd  = m_DY05.oInspectStage2Bwd;
		
		m_DX05.iInspectStage3Up  = m_DY05.oInspectStage3Up;
		m_DX05.iInspectStage3Down  = m_DY05.oInspectStage3Down;
		m_DX05.iInspectStage3Fwd  = m_DY05.oInspectStage3Fwd;
		m_DX05.iInspectStage3Bwd  = m_DY05.oInspectStage3Bwd;

		m_DX05.iInspectStage1Vac01  = m_DY05.oInspectStage1Vac01;
		m_DX05.iInspectStage1Vac02  = m_DY05.oInspectStage1Vac02;
		m_DX05.iInspectStage1Vac03  = m_DY05.oInspectStage1Vac03;
		m_DX05.iInspectStage1Vac04  = m_DY05.oInspectStage1Vac04;
		m_DX05.iInspectStage1Vac05  = m_DY05.oInspectStage1Vac05;
		m_DX05.iInspectStage1Vac06  = m_DY05.oInspectStage1Vac06;
		m_DX05.iInspectStage1Vac07  = m_DY05.oInspectStage1Vac07;
		m_DX05.iInspectStage1Vac08  = m_DY05.oInspectStage1Vac08;
	
	}

	if (nNo==6) {
		m_DX06.iInspectStage2Vac01  = m_DY06.oInspectStage2Vac01;
		m_DX06.iInspectStage2Vac02  = m_DY06.oInspectStage2Vac02;
		m_DX06.iInspectStage2Vac03  = m_DY06.oInspectStage2Vac03;
		m_DX06.iInspectStage2Vac04  = m_DY06.oInspectStage2Vac04;
		m_DX06.iInspectStage2Vac05  = m_DY06.oInspectStage2Vac05;
		m_DX06.iInspectStage2Vac06  = m_DY06.oInspectStage2Vac06;
		m_DX06.iInspectStage2Vac07  = m_DY06.oInspectStage2Vac07;
		m_DX06.iInspectStage2Vac08  = m_DY06.oInspectStage2Vac08;

		m_DX06.iInspectStage3Vac01  = m_DY06.oInspectStage3Vac01;
		m_DX06.iInspectStage3Vac02  = m_DY06.oInspectStage3Vac02;
		m_DX06.iInspectStage3Vac03  = m_DY06.oInspectStage3Vac03;
		m_DX06.iInspectStage3Vac04  = m_DY06.oInspectStage3Vac04;
		m_DX06.iInspectStage3Vac05  = m_DY06.oInspectStage3Vac05;
		m_DX06.iInspectStage3Vac06  = m_DY06.oInspectStage3Vac06;
		m_DX06.iInspectStage3Vac07  = m_DY06.oInspectStage3Vac07;
		m_DX06.iInspectStage3Vac08  = m_DY06.oInspectStage3Vac08;

		m_DX06.iCmAlignSlaveClose  = m_DY06.oCmAlignSlaveClose;
		m_DX06.iCmAlignSlaveOpen  = m_DY06.oCmAlignSlaveOpen;

		m_DX06.iCmAlignMasterClose  = m_DY06.oCmAlignMasterClose;
		m_DX06.iCmAlignMasterOpen  = m_DY06.oCmAlignMasterOpen;

		m_DX06.iTop1Mirror1Up  = m_DY06.oTop1Mirror12Up;
		m_DX06.iTop1Mirror1Down  = m_DY06.oTop1Mirror12Down;
		m_DX06.iTop1Mirror2Up  = m_DY06.oTop1Mirror12Up;
		m_DX06.iTop1Mirror2Down  = m_DY06.oTop1Mirror12Down;

		m_DX06.iCmAlignMasterClose  = m_DY06.oCmAlignMasterClose;
		m_DX06.iCmAlignMasterOpen  = m_DY06.oCmAlignMasterOpen;
	}

	if (nNo==7) {
		m_DX07.iBtm2PickerOpen01			 = m_DY07.oBtm2PickerOpen01;
		m_DX07.iBtm2PickerOpen02	     	 = m_DY07.oBtm2PickerOpen02;
		m_DX07.iBtm2PickerOpen03			 = m_DY07.oBtm2PickerOpen03;
		m_DX07.iBtm2PickerOpen04	     	 = m_DY07.oBtm2PickerOpen04;
		m_DX07.iBtm2PickerOpen05			 = m_DY07.oBtm2PickerOpen05;
		m_DX07.iBtm2PickerOpen06	     	 = m_DY07.oBtm2PickerOpen06;
		m_DX07.iBtm2PickerOpen07			 = m_DY07.oBtm2PickerOpen07;
		m_DX07.iBtm2PickerOpen08	     	 = m_DY07.oBtm2PickerOpen08;

		//close되면 Exist 인가? 체크해 봐야 한다.
		m_DX07.iBtm2PickerExist01			 = m_DY07.oBtm2PickerClose01;
		m_DX07.iBtm2PickerExist02	     	 = m_DY07.oBtm2PickerClose02;
		m_DX07.iBtm2PickerExist03			 = m_DY07.oBtm2PickerClose03;
		m_DX07.iBtm2PickerExist04	     	 = m_DY07.oBtm2PickerClose04;
		m_DX07.iBtm2PickerExist05			 = m_DY07.oBtm2PickerClose05;
		m_DX07.iBtm2PickerExist06	     	 = m_DY07.oBtm2PickerClose06;
		m_DX07.iBtm2PickerExist07			 = m_DY07.oBtm2PickerClose07;
		m_DX07.iBtm2PickerExist08	     	 = m_DY07.oBtm2PickerClose08;

		if(m_DY07.oBtm2PickerDown01 == TRUE){
			m_DX07.iBtm2PickerDown01 = TRUE;
			m_DX07.iBtm2PickerUp01 = FALSE;
		}
		else{
			m_DX07.iBtm2PickerDown01 = FALSE;
			m_DX07.iBtm2PickerUp01 = TRUE;
		}

		if(m_DY07.oBtm2PickerDown02 == TRUE){
			m_DX07.iBtm2PickerDown02 = TRUE;
			m_DX07.iBtm2PickerUp02 = FALSE;
		}
		else{
			m_DX07.iBtm2PickerDown01 = FALSE;
			m_DX07.iBtm2PickerUp01 = TRUE;
		}

		if(m_DY07.oBtm2PickerDown03 == TRUE){
			m_DX07.iBtm2PickerDown03 = TRUE;
			m_DX07.iBtm2PickerUp03 = FALSE;
		}
		else{
			m_DX07.iBtm2PickerDown03 = FALSE;
			m_DX07.iBtm2PickerUp03 = TRUE;
		}


		if(m_DY07.oBtm2PickerDown04 == TRUE){
			m_DX07.iBtm2PickerDown04 = TRUE;
			m_DX07.iBtm2PickerUp04 = FALSE;
		}
		else{
			m_DX07.iBtm2PickerDown04 = FALSE;
			m_DX07.iBtm2PickerUp04 = TRUE;
		}

	}

	if (nNo==8) {
		m_DX08.iBtm2PickerVac01 = m_DY08.oBtm2PickerVac01;
		m_DX08.iBtm2PickerVac02 = m_DY08.oBtm2PickerVac02;
		m_DX08.iBtm2PickerVac03 = m_DY08.oBtm2PickerVac03;
		m_DX08.iBtm2PickerVac04 = m_DY08.oBtm2PickerVac04;
		m_DX08.iBtm2PickerVac05 = m_DY08.oBtm2PickerVac05;
		m_DX08.iBtm2PickerVac06 = m_DY08.oBtm2PickerVac06;
		m_DX08.iBtm2PickerVac07 = m_DY08.oBtm2PickerVac07;
		m_DX08.iBtm2PickerVac08 = m_DY08.oBtm2PickerVac08;

		if(m_DY08.oBtm2PickerDown05 == TRUE){
			m_DX08.iBtm2PickerDown05 = TRUE;
			m_DX08.iBtm2PickerUp05 = FALSE;
		}
		else{
			m_DX08.iBtm2PickerDown05 = FALSE;
			m_DX08.iBtm2PickerUp05 = TRUE;
		}

		if(m_DY08.oBtm2PickerDown06 == TRUE){
			m_DX08.iBtm2PickerDown06 = TRUE;
			m_DX08.iBtm2PickerUp06 = FALSE;
		}
		else{
			m_DX08.iBtm2PickerDown06 = FALSE;
			m_DX08.iBtm2PickerUp06 = TRUE;
		}

		if(m_DY08.oBtm2PickerDown07 == TRUE){
			m_DX08.iBtm2PickerDown07 = TRUE;
			m_DX08.iBtm2PickerUp07 = FALSE;
		}
		else{
			m_DX08.iBtm2PickerDown07 = FALSE;
			m_DX08.iBtm2PickerUp07 = TRUE;
		}


		if(m_DY08.oBtm2PickerDown08 == TRUE){
			m_DX08.iBtm2PickerDown08 = TRUE;
			m_DX08.iBtm2PickerUp08 = FALSE;
		}
		else{
			m_DX08.iBtm2PickerDown08 = FALSE;
			m_DX08.iBtm2PickerUp08 = TRUE;
		}
	}

	if (nNo==9) {
		m_DX09.iBufferStage1Vacuum = m_DY09.oBufferStage1Vacuum;


		if(m_DY09.oBufferStage1Rotate == FALSE)
		{
			m_DX09.iBufferStage1Rotate = FALSE;
			m_DX09.iBufferStage1Normal = TRUE;
		}
		else
		{
			m_DX09.iBufferStage1Rotate = TRUE;
			m_DX09.iBufferStage1Normal = FALSE;
		}
				
		m_DX09.iBufferStage1Up = m_DY09.oBufferStage1Up;
		m_DX09.iBufferStage1Down = m_DY09.oBufferStage1Down;

		m_DX09.iBufferStage2Vacuum = m_DY09.oBufferStage2Vacuum;

		if(m_DY09.oBufferStage2Rotate == FALSE)
		{
			m_DX09.iBufferStage2Rotate = FALSE;
			m_DX09.iBufferStage2Normal = TRUE;
		}
		else
		{
			m_DX09.iBufferStage2Rotate = TRUE;
			m_DX09.iBufferStage2Normal = FALSE;
		}

		m_DX09.iBufferStage2Up = m_DY09.oBufferStage2Up;
		m_DX09.iBufferStage2Down = m_DY09.oBufferStage2Down;

		m_DX09.iNgPortSlideLock = m_DY09.oNgPortSlideLock;
		m_DX09.iNgPortSlideUnlock = m_DY09.oNgPortSlideUnlock;
	}

	if (nNo==10) {
		if(m_DY10.oSortPicker1Down1 == FALSE)
		{
			m_DX10.iSortPicker1Down1 = FALSE;
			m_DX10.iSortPicker1Up1 = TRUE;			
		}
		else{ 
			m_DX10.iSortPicker1Down1 = TRUE;
			m_DX10.iSortPicker1Up1 = FALSE;		
		}

		if(m_DY10.oSortPicker1Down2 == FALSE)
		{
			m_DX10.iSortPicker1Down2 = FALSE;
			m_DX10.iSortPicker1Up2 = TRUE;
		}
		else
		{
			m_DX10.iSortPicker1Down2 = TRUE;
			m_DX10.iSortPicker1Up2 = FALSE;
		}

		if(m_DY10.oSortPicker1Down3 == FALSE)
		{
			m_DX10.iSortPicker1Down3 = FALSE;
			m_DX10.iSortPicker1Up3 = TRUE;
		}
		else
		{
			m_DX10.iSortPicker1Down3 = TRUE;
			m_DX10.iSortPicker1Up3 = FALSE;
		}

		if(m_DY10.oSortPicker1Down4 == FALSE)
		{
			m_DX10.iSortPicker1Down4 = FALSE;
			m_DX10.iSortPicker1Up4 = TRUE;
		}
		else
		{
			m_DX10.iSortPicker1Down4 = TRUE;
			m_DX10.iSortPicker1Up4 = FALSE;
		}
			
		m_DX10.iSortPicker1Open1 = m_DY10.oSortPicker1Open1;
		m_DX10.iSortPicker1Open2 = m_DY10.oSortPicker1Open2;
		m_DX10.iSortPicker1Open3 = m_DY10.oSortPicker1Open3;
		m_DX10.iSortPicker1Open4 = m_DY10.oSortPicker1Open4;

		m_DX10.iSortPicker1Exist1 = m_DY10.oSortPicker1Close1;
		m_DX10.iSortPicker1Exist2 = m_DY10.oSortPicker1Close2;
		m_DX10.iSortPicker1Exist3 = m_DY10.oSortPicker1Close3;
		m_DX10.iSortPicker1Exist4 = m_DY10.oSortPicker1Close4;

		m_DX10.iNgBufferVac01 = m_DY10.oNgBufferVac01;
		m_DX10.iNgBufferVac02 = m_DY10.oNgBufferVac02;
		m_DX10.iNgBufferVac03 = m_DY10.oNgBufferVac03;
		m_DX10.iNgBufferVac04 = m_DY10.oNgBufferVac04;
	}

	if (nNo==11) {
		if(m_DY11.oSortPicker2Down1 == FALSE)
		{
			m_DX11.iSortPicker2Down1 = FALSE;
			m_DX11.iSortPicker2Up1 = TRUE;			
		}
		else{ 
			m_DX11.iSortPicker2Down1 = TRUE;
			m_DX11.iSortPicker2Up1 = FALSE;		
		}

		if(m_DY11.oSortPicker2Down2 == FALSE)
		{
			m_DX11.iSortPicker2Down2 = FALSE;
			m_DX11.iSortPicker2Up2 = TRUE;
		}
		else
		{
			m_DX11.iSortPicker2Down2 = TRUE;
			m_DX11.iSortPicker2Up2 = FALSE;
		}
			
		if(m_DY11.oSortPicker2Down3 == FALSE)
		{
			m_DX11.iSortPicker2Down3 = FALSE;
			m_DX11.iSortPicker2Up3 = TRUE;
		}
		else
		{
			m_DX11.iSortPicker2Down3 = TRUE;
			m_DX11.iSortPicker2Up3 = FALSE;
		}

		if(m_DY11.oSortPicker2Down4 == FALSE)
		{
			m_DX11.iSortPicker2Down4 = FALSE;
			m_DX11.iSortPicker2Up4 = TRUE;
		}
		else
		{
			m_DX11.iSortPicker2Down4 = TRUE;
			m_DX11.iSortPicker2Up4 = FALSE;
		}
		

		m_DX11.iSortPicker2Open1 = m_DY11.oSortPicker2Open1;
		m_DX11.iSortPicker2Open2 = m_DY11.oSortPicker2Open2;
		m_DX11.iSortPicker2Open3 = m_DY11.oSortPicker2Open3;
		m_DX11.iSortPicker2Open4 = m_DY11.oSortPicker2Open4;

		m_DX11.iSortPicker2Exist1 = m_DY11.oSortPicker2Close1;
		m_DX11.iSortPicker2Exist2 = m_DY11.oSortPicker2Close2;
		m_DX11.iSortPicker2Exist3 = m_DY11.oSortPicker2Close3;
		m_DX11.iSortPicker2Exist4 = m_DY11.oSortPicker2Close4;

		m_DX11.iNgBufferVac05 = m_DY11.oNgBufferVac05;
		m_DX11.iNgBufferVac06 = m_DY11.oNgBufferVac06;
		m_DX11.iNgBufferVac07 = m_DY11.oNgBufferVac07;
		m_DX11.iNgBufferVac08 = m_DY11.oNgBufferVac08;
	}

	if (nNo==12) {
		m_DX12.iGoodPortSlideLock = m_DY12.oGoodPortSlideLock;
		m_DX12.iGoodPortSlideUnlock = m_DY12.oGoodPortSlideUnlock;

		if(m_DY12.oGoodTrayBuffSupport1In == TRUE){
			m_DX12.iGoodTrayBuffSupport1In = TRUE;
			m_DX12.iGoodTrayBuffSupport1Out = FALSE;
		}
		else{
			m_DX12.iGoodTrayBuffSupport1In = FALSE;
			m_DX12.iGoodTrayBuffSupport1Out = TRUE;
		}

		if(m_DY12.oGoodTrayBuffSupport2In == TRUE){
			m_DX12.iGoodTrayBuffSupport2In = TRUE;
			m_DX12.iGoodTrayBuffSupport2Out = FALSE;
		}
		else{
			m_DX12.iGoodTrayBuffSupport2In = FALSE;
			m_DX12.iGoodTrayBuffSupport2Out = TRUE;
		}
		
		if(m_DY12.oGoodStage1MasterIn == FALSE)
		{
			m_DX12.iGoodStage1MasterIn = FALSE;
			m_DX12.iGoodStage1MasterOut = TRUE;
		}
		else{
			m_DX12.iGoodStage1MasterIn = TRUE;
			m_DX12.iGoodStage1MasterOut = FALSE;
		}

		if(m_DY12.oGoodStage1SlaveIn == FALSE)
		{
			m_DX12.iGoodStage1SlaveIn = FALSE;
			m_DX12.iGoodStage1SlaveOut = TRUE;
		}
		else{
			m_DX12.iGoodStage1SlaveIn = TRUE;
			m_DX12.iGoodStage1SlaveOut = FALSE;
		}

		

		if(m_DY12.oGoodStage2MasterIn == FALSE)
		{
			m_DX12.iGoodStage2MasterIn = FALSE;
			m_DX12.iGoodStage2MasterOut = TRUE;
		}
		else{
			m_DX12.iGoodStage2MasterIn = TRUE;
			m_DX12.iGoodStage2MasterOut = FALSE;
		}

		if(m_DY12.oGoodStage2SlaveIn == FALSE)
		{
			m_DX12.iGoodStage2SlaveIn = FALSE;
			m_DX12.iGoodStage2SlaveOut = TRUE;
		}
		else{
			m_DX12.iGoodStage2SlaveIn = TRUE;
			m_DX12.iGoodStage2SlaveOut = FALSE;
		}

	}

	if (nNo==13) {
		m_DX13.iEmptyTrans1Up = m_DY13.oEmptyTrans1Up;
		m_DX13.iEmptyTrans1Down = m_DY13.oEmptyTrans1Down;
		m_DX13.iEmptyTrans1MasterIn = m_DY13.oEmptyTrans1MasterIn;
		m_DX13.iEmptyTrans1MasterOut = m_DY13.oEmptyTrans1MasterOut;
		m_DX13.iEmptyTrans1SlaveIn = m_DY13.oEmptyTrans1SlaveIn;
		m_DX13.iEmptyTrans1SlaveOut = m_DY13.oEmptyTrans1SlaveOut;

		m_DX13.iEmptyTrans2Up = m_DY13.oEmptyTrans2Up;
		m_DX13.iEmptyTrans2Down = m_DY13.oEmptyTrans2Down;

		m_DX13.iEmptyTrans2MasterIn = m_DY13.oEmptyTrans2MasterIn;
		m_DX13.iEmptyTrans2MasterOut = m_DY13.oEmptyTrans2MasterOut;
		m_DX13.iEmptyTrans2SlaveIn = m_DY13.oEmptyTrans2SlaveIn;
		m_DX13.iEmptyTrans2SlaveOut = m_DY13.oEmptyTrans2SlaveOut;
	}

	if (nNo==14) {

		m_DX14.iStartSw		 = bF;
		m_DX14.iStopSw		 = bF;
		m_DX14.iResetSw		 = bF;
	}

	if (nNo==15) {

		m_DX15.iDoor01Unlock = m_DY15.oDoor01Unlock;
		m_DX15.iDoor02Unlock = m_DY15.oDoor02Unlock;
		m_DX15.iDoor03Unlock = m_DY15.oDoor03Unlock;
		m_DX15.iDoor04Unlock = m_DY15.oDoor04Unlock;
		m_DX15.iDoor05Unlock = m_DY15.oDoor05Unlock;
		m_DX15.iDoor06Unlock = m_DY15.oDoor06Unlock;
		m_DX15.iDoor07Unlock = m_DY15.oDoor07Unlock;
		m_DX15.iDoor08Unlock = m_DY15.oDoor08Unlock;
		m_DX15.iDoor09Unlock = m_DY15.oDoor09Unlock;
		m_DX15.iDoor10Unlock = m_DY15.oDoor10Unlock;
		m_DX15.iDoor11Unlock = m_DY15.oDoor11Unlock;
		m_DX15.iDoor12Unlock = m_DY15.oDoor12Unlock;
		m_DX15.iDoor13Unlock = m_DY15.oDoor13Unlock;
		m_DX15.iDoor14Unlock = m_DY15.oDoor14Unlock;
		m_DX15.iDoor15Unlock = m_DY15.oDoor15Unlock;
		m_DX15.iDoor16Unlock = m_DY15.oDoor16Unlock;
		m_DX15.iDoor17Unlock = m_DY15.oDoor17Unlock;
		m_DX15.iDoor18Unlock = m_DY15.oDoor18Unlock;
		m_DX15.iDoor19Unlock = m_DY15.oDoor19Unlock;
		m_DX15.iDoor20Unlock = m_DY15.oDoor20Unlock;
		
	}
}
