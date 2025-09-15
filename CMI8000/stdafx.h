// stdafx.h : ���� ���������? ���� ���������? �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ���? �ִ� ���� �����Դϴ�.
#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows �������? �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ���������� ����˴ϴ�?.

// MFC�� ���� �κа� ���� ������ ���? �޽����� ���� �����? �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�?.
#include <afxext.h>         // MFC Ȯ���Դϴ�.

#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

///////////////////////////////////////////////////////////////////////////////

// Library Add
#include "CSControls.h"
#include "CSIniFile.h" 
#include "CSComPort.h"
#include "CSUdpSocket.h"
#include "CSClientSocket.h"
#include "CSGrid.h"
#include "CSUdpClient.h"
#include <afxsock.h>
#include <math.h>
#ifdef _DEBUG
	#pragma comment(lib, "CSControlsD.lib")
	#pragma comment(lib, "CSIniFileD.lib")
	#pragma comment(lib, "CSComPortD.lib")
	#pragma comment(lib, "CSUdpSocketD.lib")
	#pragma comment(lib, "CSClientSocketD.lib")
	#pragma comment(lib, "CSGridD.lib")
	#pragma comment(lib, "CSUdpClientD.lib")
#else
	#pragma comment(lib, "CSControlsR.lib")
	#pragma comment(lib, "CSIniFileR.lib")
	#pragma comment(lib, "CSComPortR.lib")
	#pragma comment(lib, "CSUdpSocketR.lib")
	#pragma comment(lib, "CSClientSocketR.lib")
	#pragma comment(lib, "CSGridR.lib")
	#pragma comment(lib, "CSUdpClientR.lib")
#endif


#define MAIN_VERSION	_T("V 3.5.1.2 MEM")


//�ּ�ó���ϸ� SIMULATION 
#define AJIN_BOARD_USE 
#define LOT_BARCODE_USE

// 2~5ȣ�� �ɼ�
#define EDITION_2ND 		// 2~5ȣ��

// �׽�Ʈ �� �ɼ�
//#define DRY_RUN_TEST		// Dry Run Test �� ���?

// EVMS ���? �ɼ� (2024.01.26)
// #define USE_AVI_EVMS		// EVMS ���� ���?

#define SIM_WAITTIMES	 1		//
#define SIM_WAITTIMEM	 1		//

#define	VELOCITY_WEIGHT	 8

///////////////////////////////////////////////////////////////////////////////

extern CString gsCurrentDir;	// ���� ������Ʈ ����
extern CString gsComputerName;	// ���� ��ǻ�� �̸�

typedef struct {
	CString sEnvPath;				// ȯ�漳�� ���� ���?
	CString sLogPath;				// LOG ���� ���?

	CString	sLotID[2];
	CString	sOperID;				// Operator
	CString	sRecipe;				// Model

	int		nTrayUseCount[2];		// �����? �Է��� Tray ����
	int		nCmUseCount[2];			// �����? �Է��� CM ����
	int		nCmMaxCount;			// �� Ʈ������ �ִ� CM ����

	int		nLPNo;					// Load �� Port No
	int		nLoadTrayCount[2];
	int		nEmptyTrayCount;
	int		nGoodTrayCount;
	int		nGoodStageTrayNo[2];	// GoodTraycount ���� Stage ���� �������ش�.
	int		nNgTrayNo;
	int		nULPNo;					// Unload Port No
	int		nErrPNo;

	int		InfoAnglePortTray[5][8];// Angle Port Tray ���� ǥ�� (0:Not Use, 9:Use)
	int		InfoAngleTray[2][5][8];	// Load Tray ���� ǥ�� (0:Not Use, 9:Use)
	int		InfoBuffTray[2][5][8];	// Buffer Tray ���� ǥ�� (0:Not Use, 1:Exist)
	int		InfoGoodTray[5][8];		// Good Tray ���� ǥ�� (0:Empty, 1:Exist)
	int		InfoNgTray[4][5][8];	// NG Tray ���� ǥ�� (0:Empty, 2:Exist)
	int		InfoNgBuffer[2][5];		// NG Buffer ���� ǥ�� (0:Empty, 2:Exist)

	int		InfoBtm1Pick[10];		// 0:�»��?
	int		InfoInspect[3][10];		// 0~2:����������ȣ, 0:�»��?
	int		InfoBtm2Pick[10];		// 0:�»��?
	int		InfoSortPick[2][5];		// 0: Rear 1: Front, 0~5:�������� 0 

	int		nPNoTrayPick;			// Tray Picker Port No (1, 2)
	int		nPNoAnglePort[2];		// Angle Port Port No (1, 2)
	int		nPNoAngleTray[2];		// Angle Tray Port No (1, 2)
	int		nPNoBtm1Pick;			// Btm1 Picker Port No (1, 2)
	int		nPNoInspect[3];			// Inspect Stage Port No (1, 2)
	int		nPNoBtm2Pick;			// Btm2 Picker Port No (1, 2)
	int		nPNoBuffTray[2];		// Buffer Tray Port No (1, 2)
	int		nPNoSortPick[2];		// Sort Picker Port No (1, 2)
	int		nPNoGoodTray;			// Good Tray Port No (1, 2)
	int		nPNoNgTray;				// NG Tray Port No (1, 2)
	int		nPNoNgBuffer[2];		// NG Buffer Port No (1, 2)

	int		nTNoTrayPick;			// Tray Picker Tray No (1 ~ 25)
	int		nTNoAnglePort[2];		// Angle Port Tray No (1 ~ 25)
	int		nTNoAngleTray[2];		// Angle Tray Tray No (1 ~ 25)
	int		nTNoBtm1Pick[10];		// Btm1 Picker Tray No (1 ~ 25)
	int		nTNoInspect[3][10];		// Inspect Stage Tray No (1 ~ 25)
	int		nTNoBtm2Pick[10];		// Btm2 Picker Tray No (1 ~ 25)
	int		nTNoBuffTray[2][5][8];	// Buffer Tray Tray No (1 ~ 25)
	int		nTNoSortPick[2][5];		// Main Index Tray (1 ~ 25)
	int		nTNoNgBuffer[2][5];

	int		nCNoBtm1Pick[10];		// Load Picker CM No R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoInspect[3][10];		// Load Buffer CM No R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoBtm2Pick[10];		// Load Index CM No R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoBuffTray[2][5][8];	// Load Index CM No R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoSortPick[2][5];		// Main Index CM R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoNgBuffer[2][5];

	BOOL	bReload[7];				// 0:Angle, 1:Btm1_Specular, 2:Top1, 3:Top2, 4:Btm2 5:Btm1_Angle 6:Btm1_3D
	BOOL	bScanDone[7];			// 0:Angle, 1:Btm1_Specular, 2:Top1, 3:Top2, 4:Btm2, 5:Btm1_Angle 6:Btm1_3D
	// Port ���� �����ؾ���.
	BYTE	byInspectDone[2][30][40];	// �˻�Ϸ�? (5bit ���? ==> 0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2)
	int		nInspectInfo[2][30][40];	// �˻��� (0:Empty, 1:Good, 2:Normal, 3:Mes, 4:N1, 5:N2)
	CString sNGData[2][30][40][7];		// NG �ڵ�  (0:Angle, 1:Btm1_SP, 2:Top1, 3:Top2, 4:Btm2 5:Btm1_AG 6:Btm1_3D)
	char	cJudgeCode1[30][40][7];	// �����ڵ� (0:Angle, 1:Btm1_SP, 2:Top1, 3:Top2, 4:Btm2 5:Btm1_AG 6:Btm1_3D)
	char    cJudgeCode2[30][40][7];
	
	CString sSortWaitStartTime[2];
	BOOL	bRosDone[2][30][40];		// ROS ���� ���? Log ��

	BOOL	bCycleStop;				// Run �߰��� ���⶧ (�ʱ�ȭ �ʿ�)

	CString	sErrBtm1PickNo;			// Error �� ó�� (Picker 1 ~ 10)
	int		nErrBtm1PickIdx;		// MTBA ���� �α� (2024.01.24)
	int		nErrBtm2PickIdx;		// MTBA ���� �α� (2024.01.24)

	BOOL	bNgTrayEnd;				// Lot ���� �ǰ� NG Door ���� ���� �ְ� ���ش�. 
	BOOL	bNGTrayWait;			// NG ���? �����ϰ� �ٽ� ���� �Ҷ����� ���?
	BOOL	bGoodTrayLotEnd[2];
	BOOL	bGoodTrayWait;			// Good ���? ���� �Ϸ����? ���?
	BOOL	bEmptyZWait;			// Empty Z Port Full Tray

	BOOL	bLoadLampOn[2];			// Load Port Lamp ������ ������ Lot End�Ǵ� �������� �ٲ��ش�.

	BOOL	bTop1MirrorErr[3];		// Top1 Mirror Down Error : TRUE
	int		nTop1MirrorErrNo;		// Top1 Mirror Error No : TRUE

	CString sGoodTray1LoadTime;
	CString sGoodTray1UnloadTime;
	CString sGoodTray2LoadTime;
	CString sGoodTray2UnloadTime;
	DWORD	dwGoodTray1LoadingTime;
	DWORD	dwGoodTray1UnloadingTime;
	DWORD	dwGoodTray2LoadingTime;
	DWORD	dwGoodTray2UnloadingTime;

	DWORD	dwBtm1ScanTime;			// Load Complete ���� Scan Complete����
	DWORD	dwTop1ScanTime;			// Load Complete ���� Scan Complete����
	DWORD	dwTop2ScanTime;			// Load Complete ���� Scan Complete����
	DWORD	dwBtm2ScanTime;			// Load Complete ���� Scan Complete����

	DWORD	dwMStopStartTime;		// �۾��� ���� �˶�, Stop ��ưor����ġ ������ ���? �������� 
	DWORD	dwMStopEndTime;
	DWORD	dwMStopProcTime;

	int		nTrayX;					// Tray X Count (R04A:4, R04B:5, R04C:8)
	int		nTrayY;					// Tray Y Count (R04A:5, R04B:3, R04C:4)
	int		nScanCnt;				// Scan Count (R04A:2, R04B:2, R04C:1)
	int		nBtmPickQt;				// Bottom Picker Qt (R04A:5, R04B:5, R04C:4)
	int		nSortPickQt;			// Sort Picker Qt (R04A:4, R04B:5, R04C:4)

	BOOL	bBuffTrayChange;		//
	BOOL	bContinueLotEnd;

	BOOL	bVisionVerErr;			// Vision ���α׷� �����̳� �Ķ����? ������ �޶� �˶��� ���� �ٽ� LotStart ��������Ѵ�?.
	BOOL	bFirstLotStart;
	BOOL	bMesFirstLot;

	double	dEmptyPortZUpOffset;	// Empty Port Z�� Up Offset ��ġ ���� ���ش�.
	double	dEmptyPortZDownOffset;	// Empty Port Z�� Down Offset ��ġ ���� ���ش�.
	BOOL	bEmptyFull;

	int		nErrAngleStageNo;
	int		nErrBufferStageNo;

	CString	sErrVisionLotID;		// �������� �˶��� ������ LotID ����.

	BOOL	bBuffStageMove;			// Sort Picker �����Ҷ� Buffer Y���� ���������� �ٸ� Sort Picker �����Ҷ��� Buffer Y���� �����̸� �ȵȴ�.
	BOOL	bLotEndBeep;			// NG Full �˸��� ������ LotEnd �˸����� Skip �ȵǰ� ���ش�.

	int		nMesPortNo;				// ���Ӷ� �� MES �˶� �߻��ϸ� �ٽ� �����Ҽ� �ְ� ���ش�.

	int		nSp1Timer;
	int		nSp2Timer;

	BOOL	bAlignReply;
	BOOL	bEmptyZTopCheckRepeat;	// Empty Z�� Top Check Sensor Ȯ���� 2�����ش�.

	int		nBtWorkSortPickNo;	// Buffer Tray �۾� ���� Sort Picker No.

	// ������ Tray�� LotEnd�� �Ǹ� ������ ���⶧���� �̸� �����صд�.
	CString sCLotID[2];				// Cap ���? ���� LotID
	int		nCPortNo[2];			// Cap ���? ���� Port No
	int		nCGoodTrayCount[2];		// Cap ���? ���� stage�� Tray No
	int		nCTrayCmCnt[2];			// Cap ���? Tray ������ stage�� Tray�� ����ִ�? ����
	BOOL	bCapTrayUnload;			// Cap ��񿡼�? Ʈ���̸� ���������� Ȯ��
	BOOL	bCapTrayLoad;			// Good Port�� Ʈ���̸� ����������
	int		nLastTrayNo;			// ������ Tray ��ȣ ����

	CString sListName;
	CString sYYMMDD;
	CString sNGFileName;

	CString sAlarmTime[3];
	CString sAlarmList[3];

	CString	sComName;

	// Log ǥ��ȭ
	CString sStdMotionFile;
	CString sEfficiencyFile;
	int		nStdEqNo;	// Log ǥ��ȭ (������?)
	int		nStdLotNo;	// Log ǥ��ȭ (�Ϸ� �������� �۾��� Lot ��ȣ)
	int		nStdLogDay;	// Log ǥ��ȭ (������?)

	int		nVisionFOBMode;	// Vision���� FOB ���� ��ŸƮ�ϸ� �޼���â ǥ��, ������ FOB ���� �����ϸ� ���� �����ϵ��� ���ش�.

	int		nCmJigNo[2][30][40][10];	// CM Jig ���� ���� [Port][Tray][CM][Unit]
										// Unit 0:Angle Stage, 1:BTM1 Picker, 2:Inspection Stage No, 3:Inspection Stage Jig No, 4:BTM2 Picker
										// Unit 5:Buffer Stage No, 6:Buffer Stage Pocket No, 7:Sort Picker No, 8:Sort Picker Jig No
	int		nJigNgCnt[2][8][10];		// ���׺� �ҷ� ī��Ʈ [Port][Unit][Jig]
										// Unit 0: BTM1, 1:Insp1 Top1, 2:Insp2 Top1, 3:Insp3 Top1, 4:Insp1 Top2, 5:Insp1 Top2, 6:Insp3 Top2, 7:BTM2
	double	dJigNgOver[8][10];			// Error�� ǥ���ϱ� ���� ���׺� �ҷ����� ����ġ���� �Ѿ����� ����.
	int		nJigNgOverCnt[8][10];		// Error�� ǥ���ϱ� ���� ���׺� �ҷ����� ����ġ���� �Ѿ����� ����. 

	CString	sAPDNgItem;			// APD Result ���� Ȯ�� ���? NG Item.
	BOOL	bAPDResultErr;			// APD Result ���� Ȯ�� ���?.
	BOOL	bAviApdRequest;			// Vision APD ������ ��û
	BOOL	bAviApdReceive[6];		// Vision�� APD ������ �޾ҳ� Ȯ��(0~4), APD ���? �Ϸ�(6) �� Lotend
	int		nAviApdCnt[5];			// Vision APD �̹��� GV�� ����
	CString sVisionGV[5][20];		// 6side Vision 5��(0:AG, 1:BTM1, 2:TOP1, 3:TOP2, 4:BTM2), �̹��� ������ ������ �ִ� 10����.
	double	dAssyLoadCellAvg;		// APD�� �Է��� CapForce ��.

	BOOL	b3DStart;
	CString sLaserData1;
	CString sLaserData2;

	BOOL	b3DGrabFailErr;

	DWORD	dwSkipTime_Sort1;		// �۾��� ���� �˶�, Stop ��ưor����ġ ������ ���? �������� 
	DWORD	dwSkipTime_Sort2;

	BOOL	bWasUnlock;
	CTime	tDoorUnlockStartMin;
	CTime	tDoorUnlockStart;
	CTime	tDoorUnlockEnd;
	CTimeSpan tsDoorUnlockTotal;

	int		nDoorLockTime;		//Min
	DWORD	dwDoorStartTime;

	double	dEmptyPort_Z_Limit;

	int		nPNoMESNG[24]; //MES�� �ǹ� ���� ����ġ ���� �ִ� 24�� ���� 
	int		nTNoMESNG[24];
	int		nCmNoMESNG[24];

	BOOL	bCapVisionAlarm;
	BOOL	bSortPickCompletelyLotEnd;
	
} GLOVAL_DATA;

typedef struct {
	CString	sLotID[2];
	CString sStartTime[2];
	CString sEndTime[2];
	CString sMESStartTime[2];
	DWORD	dwLotStart[2];
	DWORD	dwLotEnd[2];
	int		nTrayCount[2];
	int		nCmCount[2];
	double  dTackTime;
	int		nGoodCount[2];
	int		nNgCount[2];
	int		nSNgCount[2][6];			// Special NG (MES, N1, N2)

	int		nMatchingNgCount[2][6];		//MES���� �ǹ� ���� ���� ��ŭ ���� ���� 
	
	int		nBsNgCount[2];				// Barcode Shift
	int		nRosRequest[2];
	int		nRosGood[2];
	int		nRosNg[2];
	int		nRosRepair[2];
	int		nRosTimeOut[2];

	// Inline Mode�϶� ĸ ���� �Ϸ� �� MES �ϰ��ϵ��� ���� ����.
	CString sCALotID[2];
	CString sCAStartTime[2];
	CString sCAEndTime[2];
	DWORD	dwCALotStart[2];
	DWORD	dwCALotEnd[2];
	int		nCATrayCount[2];
	int		nCACmCount[2];
	int		nCAGoodCount[2];
	int		nCANgCount[2];
	int		nCASNgCount[2][6];	

	int		nErrorCount;
	DWORD	dwRunTime;
	DWORD	dwStopTime;
	DWORD	dwErrorTime;

	BOOL	bLotEndComplete[2];

	double	dAverageCycle[20][2][15];

	int		nNGT;
	int		nNGC;
	int		nGDT;
	int		nG1DC;

	DWORD   dwUphStart;
} GLOVAL_LOT;

typedef struct {
	BOOL	bBegin;
	CString	sLotID;
	int		nAlmNo;
	CString sAlmMsg;
	CString sStartTime;
	CString sEndTime;
	DWORD	dwStartTime;
	DWORD	dwEndTime;
	DWORD	dwProcTime;

	double  dMotionChkPos;
	double  dMotionPos[46];

	BOOL	bBufferUpStatus[2]; //0: buffer 1, 1: buffer 2
	BOOL	bBufferDownStatus[2];
} GLOVAL_ALM;

typedef struct {
	double  dTaktTime;
	double	dTakt[24][50];	// �ð��� �� Lot�� Takt (�ð��� �ִ� 50��)
	int		nLotCount[24];	// �ð��� Lot ���� (0~23)
	int		nCmCount[2];	// �ְ�(0:7~19h), �߰�(1:19~7h)
} GLOVAL_UPH;

typedef struct {
	int		nLotStatus;				// 0:Stop, 1:Start Send, 2:Start Receive
	DWORD	dwLotStartTime;			// Lot Start time

	// Host ������ Data
	CString		sHostLotID;			//Host ���� Lot ID
	CString		sHostRecipe;		//Host ���� Recipe ID
	CString		sHostModel;			//Host ���� Recipe ID
	int			nHostCount;			//Host ���� CM �Ѱ���
	CString		sHostCancelCode;	//Host Cancel ���� code
	CString		sHostCancelText;	//Host Cancel ���� ����

	// ���? ���� Data	// LJH Port���� �����ؾ����� ���ؾ��Ѵ�.
	CString		sBarID[2][30][40];		//Barcode ID(���?) [30]:Tray No, [42]:Pocket No
	CString		sJudge[2][30][40];		//N:NG, H:Special NG, M:���ɺҷ�(MES NG)
	CString		sNGCode[2][30][40];		//��ǥNG Code
} GLOVAL_MES;

extern GLOVAL_DATA	gData;
extern GLOVAL_LOT	gLot;
extern GLOVAL_ALM	gAlm;
extern GLOVAL_UPH	gUph;
extern GLOVAL_MES	gMes;
