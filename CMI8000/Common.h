// Common.h : 헤더 파일
//
#pragma once

#define FORWARD 0
#define BACKWARD 1


#define AUTO_COUNT			20

#define AUTO_LOAD_TRAY		0	// Load1
#define AUTO_LOAD_PICKER	1	// Load1
#define AUTO_VISION_ANGLE	2	// Vision Angle
#define AUTO_ANGLE_TRAY1	3	// Load2
#define AUTO_ANGLE_TRAY2	4	// Load2
#define AUTO_VISION_BTM1	5	// BTM1
#define AUTO_INSPECTION1	6	// TOP1, TOP2
#define AUTO_INSPECTION2	7	// TOP1, TOP2
#define AUTO_INSPECTION3	8	// TOP1, TOP2
#define AUTO_VISION_BTM2	9	// BTM2
#define AUTO_BUFFER1		10	// Unload
#define AUTO_BUFFER2		11	// Unload
#define AUTO_SORT_PICKER1	12	// Unload
#define AUTO_SORT_PICKER2	13	// Unload
#define AUTO_GOOD_TRAY1		14	// Unload
#define AUTO_GOOD_TRAY2		15	// Unload
#define AUTO_NG_TRAY		16	// Unload
#define AUTO_EMPTY_TRAY_X	17	// Unload
#define AUTO_EMPTY_TRAY_EL	18	// Unload
#define AUTO_EMPTY_TRAY_Y	19	// Unload

#define INFO_STAGE		0
#define INFO_PICK		1

#define BTM1	0
#define TOP1	1
#define TOP2	2
#define BTM2	3

#define ANGLE_STAGE   0	// Stage No 저장. 1 or 2
#define BTM1_PICK	  1	// Picker Jig 번호 저장. 1~10
#define INSP_STAGE_NO 2	// Stage 번호 저장. 1~3
#define INSP_JIG_NO	  3	// Stage Jig 번호 저장. 1~10
#define BTM2_PICK     4	// Picker Jig 번호 저장. 1~10
#define BUF_STAGE_NO  5	// Stage 번호 저장. 1~2
#define BUF_JIG_NO	  6	// Stage Jig 번호 저장. 1~10
#define SORT_PICK_NO  7	// Picker 번호 저장. 1~2
#define SORT_JIG_NO   8	// Picker Jig 번호 저장. 1~5

const double GOOD_TRAY_WIDTH = 360.0;	// Good Tray 1/2 간섭없는 간격

// CCommon

class CCommon : public CWnd
{
	DECLARE_DYNAMIC(CCommon)

public:
	CCommon();
	virtual ~CCommon();

protected:
	DECLARE_MESSAGE_MAP()

private:
	LONGLONG	m_nFreq;		// Frequence

	
	LONG g_initDone;
	LONG g_lock;

public:
	void Lock()
	{
		while (InterlockedCompareExchange(&g_lock, 1, 0) != 0)
			Sleep(0);
	}
	void Unlock()
	{
		InterlockedExchange(&g_lock, 0);
	}
	ULONGLONG	GetTickCount64Compat();

	void DoEvents();
	void uSleep(int msec);
	void MakeFolder(CString sPath);
	void Save_MotionPos();
	int	 Check_MotionPos();

	
	
	BOOL Check_Position(int nAxis, int nMoveIdx, double dRange = 0.1);
	void Move_Position(int nAxis, int nMoveIdx);

	void Show_Error(int nErrNo);
	void Set_ErrorSubMessage(CString sErrSubMsg);
	void Show_Alarm(CString sMsg, BOOL bOn=TRUE);
	int Show_MsgBox(int nBtnCnt, CString sMsg);
	int Show_NumPad(CString sOld, CString &sNew, int nAxis = -1);
	int Show_KeyPad(CString &sKey);
	int Show_Password(int &nUser);

	BOOL Check_MainEmgAir();
	BOOL Check_MainDoor(BOOL bAuto=FALSE);
	BOOL Check_PortArea(BOOL bAuto=FALSE);
	BOOL Check_PortFull();
	BOOL Check_TrayFull();

	CString Get_StateOfDoors();

	BOOL Check_ServoOn();
	BOOL Check_DirveAlarm();
	BOOL Check_EndLimit();
	BOOL Check_HomeDone();

	void Locking_MainDoor(BOOL bLock, BOOL bAuto=FALSE);
	void Locking_PortSlide(BOOL bLock, int nPart=0);

	void Btm1BlowSunctionOnOff(BOOL bOn);
	void TopBlowSunctionOnOff(BOOL bOn);
	///////////////////////////////////////////////////////////////////////////
	void Get_RAMSize(CString &strRAM);
	void Get_CPUInfo(CString &strCPU);
	void Get_HardInfo(CString &strHardDisk);

	int Get_Random(int nStart, int nEnd);
	void Backup_File(CString strPath, CString strName);	// Data Backup

	///////////////////////////////////////////////////////////////////////////
	// 1. Load Port I/O 함수
	void Set_LoadPortSupportIn(int nNo);
	BOOL Get_LoadPortSupportIn(int nNo);
	void Set_LoadPortSupportOut(int nNo);
	BOOL Get_LoadPortSupportOut(int nNo);

	// 2. Load Tray Picker I/O 함수
	void Set_LoadPickerUp();
	BOOL Get_LoadPickerUp();
	void Set_LoadPickerDown();
	BOOL Get_LoadPickerDown();
	BOOL Get_LoadPickerMasterSlaveIn();
	BOOL Get_LoadPickerMasterSlaveOut();

	// 3. Angle Port I/O 함수
	void Set_AnglePortSupportIn();
	BOOL Get_AnglePortSupportIn();
	void Set_AnglePortSupportOut();
	BOOL Get_AnglePortSupportOut();
	BOOL Get_AnglePortMasterSlaveIn();
	BOOL Get_AnglePortMasterSlaveOut();

	// 4. Btm1 Picker I/O 함수
	void Set_Btm1Picker2RowUp();
	BOOL Get_Btm1Picker2RowUp();
 	void Set_Btm1PickerDown();		// ALL Down
	void Set_Btm1PickerRow2Down();
 	BOOL Get_Btm1PickerDown(int nSub);
	void Set_Btm1PickerVacOff(int nSub);
	BOOL Get_Btm1PickerVacOff(int nSub);
	void Set_Btm1PickerAirOff(int nSub);
	void Set_Btm1PickerOpen(int nSub);
	BOOL Get_Btm1PickerOpen(int nSub);
	void Set_Btm1PickerClose(int nSub);
	BOOL Get_Btm1PickerClose(int nSub);
	BOOL Get_Btm1PickerAllNotChk();	

	// 5. Inspection I/O 함수
	void Set_InspectStage1VacOff(int nSub);
	BOOL Get_InspectStage1VacOff(int nSub);
	void Set_InspectStage2VacOff(int nSub);
	BOOL Get_InspectStage2VacOff(int nSub);
	void Set_InspectStage3VacOff(int nSub);
	BOOL Get_InspectStage3VacOff(int nSub);

	void Set_InspectVacOn(int nNo, int nSub);
	BOOL Get_InspectVacOn(int nNo, int nSub);
	void Set_InspectVacOff(int nNo, int nSub);
	BOOL Get_InspectVacOff(int nNo, int nSub);

	// 6. Btm2 Picker I/O 함수
	void Set_Btm2PickerUp(int nSub);
	BOOL Get_Btm2PickerUp(int nSub);
	void Set_Btm2PickerDown();		// ALL Down
 	BOOL Get_Btm2PickerDown(int nSub);
	void Set_Btm2PickerVacOff(int nSub);
	BOOL Get_Btm2PickerVacOff(int nSub);
	void Set_Btm2PickerAirOff(int nSub);
	void Set_Btm2PickerOpen(int nSub, int nRow);
	BOOL Get_Btm2PickerOpen(int nSub, int nRow);
	void Set_Btm2PickerOpen(int nSub);
	BOOL Get_Btm2PickerOpen(int nSub);
	void Set_Btm2PickerClose(int nSub);
	BOOL Get_Btm2PickerClose(int nSub);
	BOOL Get_Btm2PickerAllNotChk();

	void Set_Btm2PickerVacOffMulti(int nSub, int nCnt, int nRow=0);
	BOOL Get_Btm2PickerVacOffMulti(int nSub, int nCnt, int nRow=0);
	void Set_Btm2PickerAirOffMulti(int nSub, int nCnt, int nRow=0);

	// 7. Sort Picker1 I/O 함수
	void Set_SortPicker1Up(int nSub);
	BOOL Get_SortPicker1Up(int nSub);
	void Set_SortPicker1Down(int nSub);
	BOOL Get_SortPicker1Down(int nSub);
	void Set_SortPicker1Open(int nSub);
	BOOL Get_SortPicker1Open(int nSub);
	void Set_SortPicker1Close(int nSub);
	BOOL Get_SortPicker1Close(int nSub);
	BOOL Get_SortPicker1AllNotChk();

	void Set_SortPicker1DownMulti(int nSub, int nCnt);
	BOOL Get_SortPicker1DownMulti(int nSub, int nCnt);
	void Set_SortPicker1OpenMulti(int nSub, int nCnt);
	BOOL Get_SortPicker1OpenMulti(int nSub, int nCnt);
	void Set_SortPicker1CloseMulti(int nSub, int nCnt);
	BOOL Get_SortPicker1CloseMulti(int nSub, int nCnt);

	// 8. Sort Picker2 I/O 함수
	void Set_SortPicker2Up(int nSub);
	BOOL Get_SortPicker2Up(int nSub);
	void Set_SortPicker2Down(int nSub);
	BOOL Get_SortPicker2Down(int nSub);
	void Set_SortPicker2Open(int nSub);
	BOOL Get_SortPicker2Open(int nSub);
	void Set_SortPicker2Close(int nSub);
	BOOL Get_SortPicker2Close(int nSub);
	BOOL Get_SortPicker2AllNotChk();

	void Set_SortPicker2DownMulti(int nSub, int nCnt);
	BOOL Get_SortPicker2DownMulti(int nSub, int nCnt);
	void Set_SortPicker2OpenMulti(int nSub, int nCnt);
	BOOL Get_SortPicker2OpenMulti(int nSub, int nCnt);
	void Set_SortPicker2CloseMulti(int nSub, int nCnt);
	BOOL Get_SortPicker2CloseMulti(int nSub, int nCnt);

	// 9. Empty Tray X I/O 함수
	void Set_EmptyTrayXUp();
	BOOL Get_EmptyTrayXUp();
	void Set_EmptyTrayXDown();
	BOOL Get_EmptyTrayXDown();
	BOOL Get_EmptyTrayXMasterSlaveIn();
	BOOL Get_EmptyTrayXMasterSlaveOut();

	// 10. Empty Tray Y I/O 함수
	void Set_EmptyTrayYUp();
	BOOL Get_EmptyTrayYUp();
	void Set_EmptyTrayYDown();
	BOOL Get_EmptyTrayYDown();
	BOOL Get_EmptyTrayYMasterSlaveIn();
	BOOL Get_EmptyTrayYMasterSlaveOut();

	// 11. Tray I/O 함수
	BOOL Get_LoadTrayMasterSlaveIn();
	BOOL Get_LoadTrayMasterSlaveOut();
	BOOL Get_GoodTray1MasterSlaveIn();
	BOOL Get_GoodTray1MasterSlaveOut();
	BOOL Get_GoodTray2MasterSlaveIn();
	BOOL Get_GoodTray2MasterSlaveOut();
	
	///////////////////////////////////////////////////////////////////////////
	// 1. Info Btm1 Picker I/O 함수
	void Set_InfoBtm1PickerVacOn(int nPos, int nRow=2, int nNo=0, int nLine=0);
	BOOL Get_InfoBtm1PickerVacOn();
	BOOL Check_Btm1PickerVacOn(int nSNo, int nLine, int nRow);
	void Set_InfoBtm1PickerClose(int nPos, int nRow=2, int nNo=0, int nLine=0);
	BOOL Get_InfoBtm1PickerClose();

	// 2. Info Inspection I/O 함수
	void Set_InfoInspectVacOn(int nNo, int nPos=0);
	BOOL Get_InfoInspectVacOn(int nNo, int nPos=0);

	// 3. Info Btm2 Picker I/O 함수
	void Set_InfoBtm2VacOn();
	BOOL Get_InfoBtm2VacOn();
	void Set_InfoBtm2Close();
	BOOL Get_InfoBtm2Close();
	BOOL Get_InfoBtm2Check();

	// 4. Info Sort Picker1 I/O 함수
	void Set_InfoSortPicker1Down(int nPos=0);
	BOOL Get_InfoSortPicker1Down(int nPos=0);
	void Set_InfoSortPicker1Close(int nPos=0);
	BOOL Get_InfoSortPicker1Close();
	BOOL Get_InfoSortPicker1Check();

	// 5. Info Sort Picker2 I/O 함수
	void Set_InfoSortPicker2Down(int nPos=0);
	BOOL Get_InfoSortPicker2Down(int nPos=0);
	void Set_InfoSortPicker2Close(int nPos=0);
	BOOL Get_InfoSortPicker2Close();
	BOOL Get_InfoSortPicker2Check();

	///////////////////////////////////////////////////////////////////////////
	//Ng Buffer I/O 함수
	void Set_NgBufferVacOff(int nNo);
	void Set_InfoNgBufferVacOn(int nNo);
	BOOL Get_InfoNgBufferVacOn(int nNo);

	///////////////////////////////////////////////////////////////////////////
	// Load Tray Info
	int *Get_AngleTrayInfo(int nSNo, int nLine);
	int Get_AngleTrayLine();

	///////////////////////////////////////////////////////////////////////////
	CString Get_Btm1PickerVacErr();

	CString Get_Btm1PickerOpenChkErr();
	CString Get_Btm1PickerCloseChkErr();
	CString Get_Btm1PickerDownChkErr();

	CString Get_InspectVacOnChkErr(int nNo);
	

	CString Get_Btm2PickerVacErr();
	CString Get_Btm2PickerCloseChkErr();


	CString Get_Sort1PickerCheckErr();
	CString Get_Sort2PickerCheckErr();

	CString Get_JigErrMessage();
};

extern CCommon g_objCommon;

///////////////////////////////////////////////////////////////////////////////
