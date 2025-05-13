// DataManager.h : 헤더 파일
//
#pragma once

///////////////////////////////////////////////////////////////////////////////
// Equip Data
typedef struct tag_EQUIP_DATA {
	CString sEquipName;
	int		nLotBarcodePort;
	int		nScreenOff;
	int		nNoWorkTime;
	int		nLogDate;
	BOOL	bManualTaktTest;
	BOOL	bUseDoorLock;
	BOOL	bUseVisionAlign;
	BOOL	bUseInspectAngle;
	BOOL	bUseInspectBtm1Specular;
	BOOL	bUseInspectBtm1Angle;
	BOOL	bUseInspectBtm13D;
	BOOL	bUseInspectTop1;
	BOOL	bUseInspectTop2;
	BOOL	bUseInspectBtm2;
	BOOL	bUseDispatcher;
	BOOL	bUseMesNg;
	BOOL	bUseMES;
	BOOL	bUseBtm2PickUpDown;	// Btm2 Picker 6~10 Up/Down
	BOOL	bUseRosSkip;
	BOOL	bUseInspectSkip;
	BOOL	bUseApdAlarm;		// APD 결과 송수신 과정에서 MES와 문제가 생기면 조치할수 있게 옵션화
	BOOL	bUsePMTrigger;
	BOOL	bUse2ndAVI;
	double	dTrayPitchX;
	double	dTrayPitchY;
	double	dInspectPitchX;
	double	dInspectPitchY;
	double	dNgBufferPitchX;
	double	dInspectALOffset;
	double	dInspectALBackOffset;
	double	dInspectTop2ShiftX;
	double	dInspectBTM1ShiftX;
	int		nVacOffDelay[4];	// 0:Btm1Picker,1:Btm2Picker,2:SortPicker1,3:SortPicker2
	int		nVacOffRepeat[4];	// 0:Btm1Picker,1:Btm2Picker,2:SortPicker1,3:SortPicker2
	int		nDelayAdd[5];		// 0:Btm1Picker,1:Btm2Picker,2:SortPicker1,3:SortPicker2,4:SortPicker 검사 대기
	BOOL	bTower[6][4];		// [0:None,1:Init,2:Ready,3:Run,4:Alarm,5:Error][0:Green,1:Yellow,2:Red,3:Flicker]
	BOOL	bBuzzer[2][6];		// [0::Alarm,1:Error][0:Bit0,1:Bit1,2:Bit2,3:Bit3,4:Bit4,5:Flicker]
	CString	sPasswordMt;
	CString	sPasswordSi;
	BOOL	bResultTestUse;		// Result Test Use
	int		nResultTestN1;		// Result Test N1 Ratio (0~100)
	int		nResultTestN2;		// Result Test N2 Ratio (0~100)
	int		nResultTestN3;		// Result Test N3 Ratio (0~100)
	int		nResultTestN4;		// Result Test N4 Ratio (0~100)
	CString	sVisionProgVer;
	CString	sVisionParaVer;
	BOOL	bUseFocusLog;
	int		nTmrFocusLog;
	int		nJigNgRate;
	int		nJigNgCnt;
	double  dTriggerStart;
	double  dTriggerEnd;
	double  dTriggerPeriod;
	double  dTriggerVel;

	BOOL	bUseInlineMode;
	CString	sCapAttachIp;
} EQUIP_DATA;

///////////////////////////////////////////////////////////////////////////////
// Move Data
typedef struct tag_MOVE_DATA {
	double  dLoadStageX[3];			// AXIS 00	[3]
	double  dLoadStageZ[4];			// AXIS 01	[4]
	double  dLoadPickerY[2];		// AXIS 02	[2]
	double  dAngleUnitY[2];			// AXIS 03	[2]
	double  dAngleUnitX[2];			// AXIS 04	[2]
	double  dAngleUnitR[2];			// AXIS 05	[2]
	double  dVisionAngleZ[1];		// AXIS 06	[1]
	double  dVisonAlignZ[2];		// AXIS 07	[2]
	double  dAngleStage1Y[5];		// AXIS 08	[4]
	double  dAngleStage1Z[6];		// AXIS 09	[3]
	double  dAngleStage2Y[5];		// AXIS 10	[4]
	double  dAngleStage2Z[6];		// AXIS 11	[3]
	double  dBtm1PickerX[9];		// AXIS 12	[9]
	double  dBtm1PickerZ[6];		// AXIS 13	[6]
	double  dBtm1PickerP1[2];		// AXIS 14	[2]
	double  dBtm1PickerP2[2];		// AXIS 15	[2]
	double  dBtm1ShiftY[2];			// AXIS 16	[2]
	double  dInspectStage1X[6];		// AXIS 17	[5]
	double  dInspectStage2X[6];		// AXIS 18	[5]
	double  dInspectStage3X[6];		// AXIS 19	[5]
	double  dModuleAlignY[3];		// AXIS 20	[3]
	double  dModuleAlignZ[2];		// AXIS 21	[2]
	double  dVisionTop1Z[1];		// AXIS 22	[1]
	double  dTop1MirrorZ[2];		// AXIS 23	[2]
	double  dTop2ShiftY[2];			// AXIS 24	[2]
	double  dVisionTop2Z[1];		// AXIS 25	[1]
	double  dBtm2PickerX[6];		// AXIS 26	[6]
	double  dBtm2PickerZ[4];		// AXIS 27	[4]
	double  dBtm2PickerP1[2];		// AXIS 28	[2]
	double  dBtm2PickerP2[2];		// AXIS 29	[2]
	double  dBufferStage1Y[6];		// AXIS 30	[4]
	double  dBufferStage2Y[6];		// AXIS 31	[4]
	double  dSortPicker1X[7];		// AXIS 32	[5]
	double  dSortPicker1Z[6];		// AXIS 33	[4]
	double  dSortPicker1P[2];		// AXIS 34	[1]
	double  dSortPicker2X[7];		// AXIS 35	[5]
	double  dSortPicker2Z[6];		// AXIS 36	[4]
	double  dSortPicker2P[2];		// AXIS 37	[1]
	double  dGoodStage1Y[4];		// AXIS 38	[4]
	double  dGoodStage1Z[5];		// AXIS 39	[4]
	double  dGoodStage2Y[4];		// AXIS 40	[4]
	double  dGoodStage2Z[5];		// AXIS 41	[4]
	double  dNgStageY[5];			// AXIS 42	[5]
	double  dEmptyTrans1X[4];		// AXIS 43	[2]
	double  dEmptyPortZ[3];			// AXIS 44	[2]
	double  dEmptyTrans2Y[3];		// AXIS 45	[2]
} MOVE_DATA;

///////////////////////////////////////////////////////////////////////////////
// Idle Code Data
typedef struct tag_IDLE_DATA {
	int		nCount;
	CString	sCode[100];
	CString	sText[100];
} IDLE_DATA;

///////////////////////////////////////////////////////////////////////////////
// ROS Data
typedef struct tag_ROS_DATA {
	CString sDispatcherIp;
	CString sHandlerIp;
	int		nJudgeTimeOver;
	BOOL	bJudgeSpecialNg;
} ROS_DATA;

///////////////////////////////////////////////////////////////////////////////


class CDataManager
{
public:
	CDataManager();
	virtual ~CDataManager();

private:
	EQUIP_DATA	m_EquipData;
	MOVE_DATA	m_MoveData;
	IDLE_DATA	m_IdleData;
	ROS_DATA	m_RosData;

public:	
	void Reset_EquipData();
	void Reset_MoveData();
	void Reset_IdleData();
	void Reset_RosData();

	BOOL Read_EquipData();
	BOOL Read_MoveData();
	BOOL Read_IdleData();
	BOOL Read_RosData();

	BOOL Read_ModelData();
	void Save_ModelData();

	EQUIP_DATA *Get_pEquipData() { return &m_EquipData; }
	MOVE_DATA *Get_pMoveData() { return &m_MoveData; }
	IDLE_DATA *Get_pIdleData() { return &m_IdleData; }
	ROS_DATA *Get_pRosData() { return &m_RosData; }
};

extern CDataManager g_objDataManager;

///////////////////////////////////////////////////////////////////////////////
