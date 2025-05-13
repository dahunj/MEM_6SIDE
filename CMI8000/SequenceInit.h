// SequenceInit.h : 헤더 파일
//
#pragma once

#include "AJinAXL.h"
#include "DataManager.h"
#include "LoopTimer.h"

class CSequenceInit
{
public:
	CSequenceInit();
	virtual ~CSequenceInit();
	
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

	int			m_niMainInitCase;		//  0. (Error : 1000)
	int			m_niLoadPickCase;		//  1. (Error : 1100)
	int			m_niAngleUnitCase;		//  2. (Error : 1200)
	int			m_niBtm1PickCase;		//  3. (Error : 1300)
	int			m_niBtm2PickCase;		//  4. (Error : 1400)
	int			m_niSortPickCase;		//  5. (Error : 1500)
	int			m_niLoadStageCase;		//  6. (Error : 1600)
	int			m_niAngleStageCase; 	//  7. (Error : 1700)
	int			m_niInspStageCase;		//  8. (Error : 1800)
	int			m_niBuffStageCase;		//  9. (Error : 1900)
	int			m_niGoodStageCase;		// 10. (Error : 2000)
	int			m_niNgStageCase;		// 11. (Error : 2100)
	int			m_niEmpTrans1Case;		// 12. (Error : 2200)
	int			m_niEmpTrans2Case;		// 13. (Error : 2300)

	CLoopTimer	m_tiMainInitLoop;		//  0. (Error : 1000)
	CLoopTimer	m_tiLoadPickLoop;		//  1. (Error : 1100)
	CLoopTimer	m_tiAngleUnitLoop;		//  2. (Error : 1200)
	CLoopTimer	m_tiBtm1PickLoop;		//  3. (Error : 1300)
	CLoopTimer	m_tiBtm2PickLoop;		//  4. (Error : 1400)
	CLoopTimer	m_tiSortPickLoop;		//  5. (Error : 1500)
	CLoopTimer	m_tiLoadStageLoop;		//  6. (Error : 1600)
	CLoopTimer	m_tiAngleStageLoop; 	//  7. (Error : 1700)
	CLoopTimer	m_tiInspStageLoop;		//  8. (Error : 1800)
	CLoopTimer	m_tiBuffStageLoop;		//  9. (Error : 1900)
	CLoopTimer	m_tiGoodStageLoop;		// 10. (Error : 2000)
	CLoopTimer	m_tiNgStageLoop;		// 11. (Error : 2100)
	CLoopTimer	m_tiEmpTrans1Loop;		// 12. (Error : 2200)
	CLoopTimer	m_tiEmpTrans2Loop;		// 13. (Error : 2300)

	BOOL		m_bInitComplete;
	
protected:
	BOOL		m_bThreadInitial;
	CWinThread	*m_pThreadInitial;
	static UINT	Thread_Initial(LPVOID lpVoid);

private:
	BOOL Initial_MainInit();			//  0. (Error : 1000)
	BOOL Initial_LoadPicker();			//  1. (Error : 1100)
	BOOL Initial_AngleUnit();			//  2. (Error : 1200)
	BOOL Initial_Btm1Picker();			//  3. (Error : 1300)
	BOOL Initial_Btm2Picker();			//  4. (Error : 1400)
	BOOL Initial_SortPicker();			//  5. (Error : 1500)
	BOOL Initial_LoadStage();			//  6. (Error : 1600)
	BOOL Initial_AngleStage();			//  7. (Error : 1700)
	BOOL Initial_InspectStage();		//  8. (Error : 1800)
	BOOL Initial_BufferStage();			//  9. (Error : 1900)
	BOOL Initial_GoodStage();			// 10. (Error : 2000)
	BOOL Initial_NgStage();				// 11. (Error : 2100)
	BOOL Initial_EmptyTrans1();			// 12. (Error : 2200)
	BOOL Initial_EmptyTrans2();			// 13. (Error : 2300)
	
	BOOL Initial_Simulation();			// 99. (Simulation)


public:
	BOOL Get_ThreadInitial() { return m_bThreadInitial; }
	BOOL Get_InitComplete() { return m_bInitComplete; }
	void Set_InitComplete(BOOL bInitComp) { m_bInitComplete = bInitComp; }	
	
	int *Get_InitialCase();

	void Begin_InitialThread();
	void End_InitialThread();
	void Init_Replay();
	void Reset_InitialCase();

	BOOL Is_InitThreadRun() { return m_bThreadInitial; }
};

extern CSequenceInit g_objSequenceInit;

///////////////////////////////////////////////////////////////////////////////
