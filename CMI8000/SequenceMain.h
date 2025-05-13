// SequenceMain.h : 헤더 파일
//
#pragma once

#include "AJinAXL.h"
#include "DataManager.h"
#include "LoopTimer.h"

const double TRAY_WIDTH = 260.0;	// Good Tray 1/2 간섭없는 간격

class CSequenceMain  
{
public:
	CSequenceMain();
	virtual ~CSequenceMain();
	
private:
	AXIS_STATUS *m_pStatus[AXIS_COUNT];
	AXIS_PARAM	*m_pParam[AXIS_COUNT];
	
	DX_DATA_00	*m_pDX00;	// Input (X0000 - X0031)
	DX_DATA_01	*m_pDX01;	// Input (X0100 - X0131)
	DX_DATA_02	*m_pDX02;	// Input (X0200 - X0231)
	DX_DATA_03	*m_pDX03;	// Input (X0300 - X0331)
	DX_DATA_04	*m_pDX04;	// Input (X0400 - X0431)
	DX_DATA_05	*m_pDX05;	// Input (X0500 - X0531)
	DX_DATA_06	*m_pDX06;	// Input (X0600 - X0631)
	DX_DATA_07	*m_pDX07;	// Input (X0700 - X0731)
	DX_DATA_08	*m_pDX08;	// Input (X0800 - X0831)
	DX_DATA_09	*m_pDX09;	// Input (X0900 - X0931)
	DX_DATA_10	*m_pDX10;	// Input (X1000 - X1031)
	DX_DATA_11	*m_pDX11;	// Input (X1100 - X1131)
	DX_DATA_12	*m_pDX12;	// Input (X1200 - X1231)
	DX_DATA_13	*m_pDX13;	// Input (X1300 - X1331)
	DX_DATA_14	*m_pDX14;	// Input (X1400 - X1431)
	DX_DATA_15	*m_pDX15;	// Input (X1500 - X1531)

	DY_DATA_00	*m_pDY00;	// Output (Y0000 - Y0031)
	DY_DATA_01	*m_pDY01;	// Output (Y0100 - Y0131)
	DY_DATA_02	*m_pDY02;	// Output (Y0200 - Y0231)
	DY_DATA_03	*m_pDY03;	// Output (Y0300 - Y0331)
	DY_DATA_04	*m_pDY04;	// Output (Y0400 - Y0431)
	DY_DATA_05	*m_pDY05;	// Output (Y0500 - Y0531)
	DY_DATA_06	*m_pDY06;	// Output (Y0600 - Y0631)
	DY_DATA_07	*m_pDY07;	// Output (Y0700 - Y0731)
	DY_DATA_08	*m_pDY08;	// Output (Y0800 - Y0831)
	DY_DATA_09	*m_pDY09;	// Output (Y0900 - Y0931)
	DY_DATA_10	*m_pDY10;	// Output (Y1000 - Y1031)
	DY_DATA_11	*m_pDY11;	// Output (Y1100 - Y1131)
	DY_DATA_12	*m_pDY12;	// Output (Y1200 - Y1231)
	DY_DATA_13	*m_pDY13;	// Output (Y1300 - Y1331)
	DY_DATA_14	*m_pDY14;	// Output (Y1400 - Y1431)
	DY_DATA_15	*m_pDY15;	// Output (Y1500 - Y1531)
	
	EQUIP_DATA	*m_pEquipData;
	MOVE_DATA	*m_pMoveData;
	ROS_DATA	*m_pRosData;

	CString		m_strLog;	// Log String

protected:
	CWinThread	*m_pThreadVacuumB1p;
	CWinThread	*m_pThreadVacuumB2p;
	CWinThread	*m_pThreadVacuumSp1;
	CWinThread	*m_pThreadVacuumSp2;
	CWinThread	*m_pThreadVacuumB2pMulti;
	CWinThread	*m_pThreadVacuumSp1Multi;
	CWinThread	*m_pThreadVacuumSp2Multi;
	CWinThread	*m_pThreadBeep;
	CWinThread	*m_pThreadNgFullBeep;
	static UINT	Thread_Vacuum_B1p(LPVOID lpVoid);
	static UINT	Thread_Vacuum_B2p(LPVOID lpVoid);
	static UINT	Thread_Vacuum_Sp1(LPVOID lpVoid);
	static UINT	Thread_Vacuum_Sp2(LPVOID lpVoid);
	static UINT	Thread_Vacuum_B2p_Multi(LPVOID lpVoid);
	static UINT	Thread_Vacuum_Sp1_Multi(LPVOID lpVoid);
	static UINT	Thread_Vacuum_Sp2_Multi(LPVOID lpVoid);
	static UINT	Thread_Beep(LPVOID lpVoid);
	static UINT	Thread_NgFullBeep(LPVOID lpVoid);

	BOOL		m_bThreadMainRun;
	CWinThread	*m_pThreadMainRun;
	static UINT	Thread_MainRun(LPVOID lpVoid);

private:
	void Vacuum_Btm1Picker(int nSub);	// Thread
	void Vacuum_Btm2Picker(int nSub);	// Thread
	void Vacuum_SortPicker1(int nSub);	// Thread
	void Vacuum_SortPicker2(int nSub);	// Thread
	void Vacuum_Btm2PickerMulti(int nSub);	// Thread
	void Vacuum_SortPicker1Multi(int nSub);	// Thread
	void Vacuum_SortPicker2Multi(int nSub);	// Thread

private:
	int			m_nLoadTrayCase;		//  1. (Error : 3100)
	int			m_nLoadPickCase;		//  2. (Error : 3200)
	int			m_nVisAngleCase;		//  3. (Error : 3300)
	int			m_nAngleTray1Case;		//  4. (Error : 3400)
	int			m_nAngleTray2Case;		//  5. (Error : 3500)
	int			m_nBtm1PickCase;		//  6. (Error : 3600)
	int			m_nInspect1Case;		//  7. (Error : 3700)	Center
	int			m_nInspect2Case;		//  8. (Error : 3800)	Front
	int			m_nInspect3Case;		//  9. (Error : 3900)	Rear
	int			m_nBtm2PickCase;		// 10. (Error : 4000)
	int			m_nBuffTray1Case;		// 11. (Error : 4100)
	int			m_nBuffTray2Case;		// 12. (Error : 4200)
	int			m_nSortPick1Case;		// 13. (Error : 4300)
	int			m_nSortPick2Case;		// 14. (Error : 4400)
	int			m_nGoodTray1Case;		// 15. (Error : 4500)
	int			m_nGoodTray2Case;		// 16. (Error : 4600)
	int			m_nNgTrayCase;			// 17. (Error : 4700)
	int			m_nEmptyTrayXCase;		// 18. (Error : 4800)
	int			m_nEmptyTrayElCase;		// 19. (Error : 4900)
	int			m_nEmptyTrayYCase;		// 20. (Error : 5000)	

	DWORD		m_dwFirstLoad;			//  Lot Start 후 첫 모듈이 Main Index에 안착되기까지의 시간
	DWORD		m_dwLastUnLoad;			//  마지막 모듈이 Main Index 안착 후 Lot End 되는 시점까지

	DWORD		m_dwLoadTray;			//  1. (Error : 3100)
	DWORD		m_dwTrayPick;			//  2. (Error : 3200)
	DWORD		m_dwVisAngle;			//  3. (Error : 3300)
	DWORD		m_dwAngleTray1;			//  4. (Error : 3400)
	DWORD		m_dwAngleTray2;			//  5. (Error : 3500)
	DWORD		m_dwBtm1Pick;			//  6. (Error : 3600)
	DWORD		m_dwInspect1;			//  7. (Error : 3700)
	DWORD		m_dwInspect2;			//  8. (Error : 3800)
	DWORD		m_dwInspect3;			//  9. (Error : 3900)
	DWORD		m_dwBtm2Pick;			// 10. (Error : 4000)
	DWORD		m_dwBuffTray1;			// 11. (Error : 4100)
	DWORD		m_dwBuffTray2;			// 12. (Error : 4200)
	DWORD		m_dwSortPick1;			// 13. (Error : 4300)
	DWORD		m_dwSortPick2;			// 14. (Error : 4400)
	DWORD		m_dwGoodTray1;			// 15. (Error : 4500)
	DWORD		m_dwGoodTray2;			// 16. (Error : 4600)
	DWORD		m_dwNgTray;				// 17. (Error : 4700)
	DWORD		m_dwEmptyTrayX;			// 18. (Error : 4800)
	DWORD		m_dwEmptyTrayEl;		// 19. (Error : 4900)
	DWORD		m_dwEmptyTrayY;			// 20. (Error : 5000)

	CLoopTimer	m_tLoadTrayLoop;		//  1. (Error : 3100)
	CLoopTimer	m_tLoadPickLoop;		//  2. (Error : 3200)
	CLoopTimer	m_tVisAngleLoop;		//  3. (Error : 3300)
	CLoopTimer	m_tAngleTray1Loop;		//  4. (Error : 3400)
	CLoopTimer	m_tAngleTray2Loop;		//  5. (Error : 3500)
	CLoopTimer	m_tBtm1PickLoop;		//  6. (Error : 3600)
	CLoopTimer	m_tInspect1Loop;		//  7. (Error : 3700)
	CLoopTimer	m_tInspect2Loop;		//  8. (Error : 3800)
	CLoopTimer	m_tInspect3Loop;		//  9. (Error : 3900)
	CLoopTimer	m_tBtm2PickLoop;		// 10. (Error : 4000)
	CLoopTimer	m_tBuffTray1Loop;		// 11. (Error : 4100)
	CLoopTimer	m_tBuffTray2Loop;		// 12. (Error : 4200)
	CLoopTimer	m_tSortPick1Loop;		// 13. (Error : 4300)
	CLoopTimer	m_tSortPick2Loop;		// 14. (Error : 4400)
	CLoopTimer	m_tGoodTray1Loop;		// 15. (Error : 4500)
	CLoopTimer	m_tGoodTray2Loop;		// 16. (Error : 4600)
	CLoopTimer	m_tNgTrayLoop;			// 17. (Error : 4700)
	CLoopTimer	m_tEmptyTrayXLoop;		// 18. (Error : 4800)
	CLoopTimer	m_tEmptyTrayElLoop;		// 19. (Error : 4900)
	CLoopTimer	m_tEmptyTrayYLoop;		// 20. (Error : 5000)

	BOOL	m_bLoadLotEnd;			// Load Stage 에서 판단
	BOOL	m_bUnloadLotEnd;		// Sort Picker 에서 판단

	BOOL	m_bLotLoadEnable[2];		// 2nd Lot 투입하기 위해...// Load Port 트레이 로드 완료하고 대기 중일때 TRUE;
	BOOL	m_bBtm1ModulePick;			// "Btm1 Picker line 1"이 첫번째 트레이의 마지막 라인을 집을때, 두번째 트레이는 "Btm1 Picker line2" 위치에 미리 들어올때 TRUE

#ifdef VISION_REPEAT
	int		m_nStartIndexJig;		// 비전 반복성 검증시 첫번째 Index
#endif

	int		m_nBtm2PickRow;
	int		m_nBtm2PickMultiCnt;		// Trans Picker Multi Pick Count
	int		m_nSortPick1MultiCnt;		// Trans Picker Multi Pick Count
	int		m_nSortPick2MultiCnt;		// Trans Picker Multi Pick Count

	int		m_nAnglePortTrayCnt;	// Angle Port Tray Count
	BOOL	m_bAngleInspComplete;	// True:Inspection complete

private:
	BOOL LoadTray_Run();			//  1. (Error : 3100)
	BOOL LoadPicker_Run();			//  2. (Error : 3200)
	BOOL VisionAngle_Run();			//  3. (Error : 3300)
	BOOL AngleTray1_Run();			//  4. (Error : 3400)
	BOOL AngleTray2_Run();			//  5. (Error : 3500)
	BOOL Btm1Picker_Run();			//  6. (Error : 3600)
	BOOL Inspection1_Run();			//  7. (Error : 3700)
	BOOL Inspection2_Run();			//  8. (Error : 3800)
	BOOL Inspection3_Run();			//  9. (Error : 3900)
	BOOL Btm2Picker_Run();			// 10. (Error : 4000)
	BOOL BufferTray1_Run();			// 11. (Error : 4100)
	BOOL BufferTray2_Run();			// 12. (Error : 4200)
	BOOL SortPicker1_Run();			// 13. (Error : 4300)
	BOOL SortPicker2_Run();			// 14. (Error : 4400)
	BOOL GoodTray1_Run();			// 15. (Error : 4500)
	BOOL GoodTray2_Run();			// 16. (Error : 4600)
	BOOL NgTray_Run();				// 17. (Error : 4700)
	BOOL EmptyTrayX_Run();			// 18. (Error : 4800)
	BOOL EmptyTrayElevator_Run();	// 19. (Error : 4900)
	BOOL EmptyTrayY_Run();			// 20. (Error : 5000)
		
	int  iGoodTrayBufferCount;
	BOOL Run_Simulation();

	BOOL LotEnd_Run();
	BOOL Check_VisionHdd();

	void Job_LotStart(int nPort);
	void Job_LotEnd(int nPort, int nGTNo);
	void Beep_Post(int nState, int nTime);

	void Init_AnglePortTray(int nPNo);
	void Init_AngleTray(int nIdx);
	void Init_BufferTray();
	void Init_GoodTray();

	BOOL Check_AnglePortTrayEmpty();
	BOOL Check_AngleTrayEmpty(int nNo=0);
	BOOL Check_AngleTrayLineEmpty(int nNo, int nLine);
	BOOL Check_Btm1PickerEmpty();
	BOOL Check_InspetionEmpty(int nNo);
	BOOL Check_Btm2PickerEmpty();
	BOOL Check_SortPickerEmpty(int nNo);
	BOOL Check_BufferEmpty(int nNo=0);
	BOOL Check_BufferTrayLineEmpty(int nNo, int nLine);
	BOOL Check_GoodTrayEmpty();
	
	BOOL Check_Btm1PickerFull();
	BOOL Check_BufferFull(int nNo);
	BOOL Check_GoodTrayFull();
	BOOL Check_NgTrayFull();
	BOOL Check_NgBufferFull(int nNo);
	BOOL Check_NgBufferEmpty(int nNo);

	BOOL Select_AngleScanPos(int &nAnglePosX, int &nAnglePosY);
	BOOL Select_BtmScanPos(int nPos, int nScanCnt);
	BOOL Select_Top1ScanPos(int nPos, int nScanCnt);
	BOOL Select_Top2ScanPos(int nPos, int nScanCnt);

	BOOL Select_AngleTrayPos(int nNo, int &nTrayPosY, int &nRow);
	BOOL Select_Btm1PickPos(int &nPos);
	BOOL Select_Btm2PickPos(int &nPos);
	BOOL Select_SortPickGoodPos(int nNo, int &nPos, int &nCnt);
	BOOL Select_SortPickNgPos(int nNo, int &nPos, int &nCnt, BOOL bBuffer=FALSE);
	int  Get_NgTrayNumber(int nInfo);
	BOOL Select_BufferTrayDownPos(int nNo, int &nTrayPosY, int &nRow);
	BOOL Select_BufferTrayUpPos(int nNo, int &nTrayPosX, int &nTrayPosY);
	BOOL Select_GoodTrayPos(int &nTrayPosX, int &nTrayPosY);
	BOOL Select_NgTrayPos(int nPickNo, int &nTrayNo, int &nTrayPosX, int &nTrayPosY);
	BOOL Select_NgBufferPos(int nPickNo, int &nTrayPosX);

	void Set_InspectJigNo(int nType, int nScanCnt, int &nINo1, int &nINo2, int &nINo3, int &nINo4);
	void Set_3DInspectJigNo(int nType, int nScanCnt, int &nINo1, int &nINo2, int &nINo3, int &nINo4, int &nINo5, int &nINo6, int &nINo7, int &nINo8);

	BOOL Check_InspectDone(int nPortNo, int nTrayNo, int nCmNo, int &Info, int nSortNo);
	BOOL Check_InspectDone2(int nPortNo, int nTrayNo, int nCmNo, int &Info, int nSortNo);
	void Set_InspectError(int nPortNo, int nTrayNo, int nCmNo);
	void Set_ScanError(int nErrNo, int nTrayNo1, int nTrayNo2, int nTrayNo3, int nTrayNo4, int nCmNo1, int nCmNo2, int nCmNo3, int nCmNo4);
	void Write_LotJudge(int nPortNo, int nTrayNo, int nCmNo, int nInfo, int nSpNo);

	BOOL Check_LoadLotEnd(int nPNo, int nMode=0);
	BOOL Check_LoadLotEndBtm1(int nPNo, int angletrayNo);
	BOOL Check_UnloadLotEnd(int nMode=0);

	BOOL Check_Btm2LoadEnd(int nPNo);	//1Lot이 Btm2 검사까지 끝났는지 확인
	BOOL Check_LoadTrayLoading(int nPNo);
	BOOL Check_InspectLotEnd(int nPNo, int nSortPickNo);
	BOOL Check_NgBufferLotEnd(int nPNo);

	BOOL Get_VisionInspectUse();
	int	 Get_GoodTrayCmCnt();

public:
	double	m_dAlignZ, m_dAngleZ, m_dAngleX, m_dAngleY, m_dAngleR, m_dBtm1Z, m_dTop1Z, m_dTop1MirrorZ, m_dTop2Z, m_dBtm2Z;
	BOOL	m_bReqShiftB1Y, m_bReqMirrorT1Z, m_bReqShiftT2Y;
	DWORD	m_dwULPickUpTime;		// Tray 1번 모듈 집은 시간.
	DWORD	m_dwULCycleTime;		//  Unload 시작한 Tray와 다음 Tray Unload 시작되는 Cycle Time 저장해준다.

	void Begin_MainRunThread();
	void End_MainRunThread();
	void Run_Replay();
	
	BOOL Is_MainThreadRun() { return m_bThreadMainRun; }
	
	void Reset_MainRunCase();
	int *Get_pMainRunCase();
	int  Get_MainRunCase(int nRun);
	void Set_MainRunCase(int nRun, int nCase);
	void Set_MainRunLoop(int nRun, int nLoop);

	BOOL Get_IsAutoRun();
	void Set_ClearRunData(int nType);

	BOOL Get_LotLoadEnable(int nNo) { return m_bLotLoadEnable[nNo]; }
	void Set_LotLoadEnable(BOOL bOn, int nNo) { m_bLotLoadEnable[nNo] = bOn; }

	void Init_NgTray(int nIdx);

#ifdef VISION_REPEAT
	void Set_StartIndexJig(int nJig) { m_nStartIndexJig = nJig; }
#endif

	void Test_CSKIM();	// Run Test
};

extern CSequenceMain g_objSequenceMain;

///////////////////////////////////////////////////////////////////////////////
