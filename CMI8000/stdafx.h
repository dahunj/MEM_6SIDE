// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#include <afxdisp.h>        // MFC 자동화 클래스입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


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


#define MAIN_VERSION	_T("V 3.5.1.28 MEM")


//---------------주석처리하면 SIMULATION-------------------------- 
#define AJIN_BOARD_USE 
#define LOT_BARCODE_USE

// 2~5호기 옵션
#define EDITION_2ND 		// 2~5호기

// 테스트 런 옵션
//#define DRY_RUN_TEST		// Dry Run Test 시 사용

// EVMS 사용 옵션 (2024.01.26)
// #define USE_AVI_EVMS		// EVMS 폴더 사용

#define SIM_WAITTIMES	 1		//
#define SIM_WAITTIMEM	 1		//

#define	VELOCITY_WEIGHT	 8

///////////////////////////////////////////////////////////////////////////////

extern CString gsCurrentDir;	// 현재 프로젝트 폴더
extern CString gsComputerName;	// 현재 컴퓨터 이름

typedef struct {
	CString sEnvPath;				// 환경설정 파일 경로
	CString sLogPath;				// LOG 파일 경로

	CString	sLotID[2];
	CString	sOperID;				// Operator
	CString	sRecipe;				// Model

	int		nTrayUseCount[2];		// 사용자 입력한 Tray 갯수
	int		nCmUseCount[2];			// 사용자 입력한 CM 갯수
	int		nCmMaxCount;			// 한 트레이의 최대 CM 갯수

	int		nLPNo;					// Load 쪽 Port No
	int		nLoadTrayCount[2];
	int		nEmptyTrayCount;
	int		nGoodTrayCount;
	int		nGoodStageTrayNo[2];	// GoodTraycount 값을 Stage 별로 저장해준다.
	int		nNgTrayNo;
	int		nULPNo;					// Unload Port No
	int		nErrPNo;

	int		InfoAnglePortTray[5][8];// Angle Port Tray 정보 표시 (0:Not Use, 9:Use)
	int		InfoAngleTray[2][5][8];	// Load Tray 정보 표시 (0:Not Use, 9:Use)
	int		InfoBuffTray[2][5][8];	// Buffer Tray 정보 표시 (0:Not Use, 1:Exist)
	int		InfoGoodTray[5][8];		// Good Tray 정보 표시 (0:Empty, 1:Exist)
	int		InfoNgTray[4][5][8];	// NG Tray 정보 표시 (0:Empty, 2:Exist)
	int		InfoNgBuffer[2][5];		// NG Buffer 정보 표시 (0:Empty, 2:Exist)

	int		InfoBtm1Pick[10];		// 0:좌상단
	int		InfoInspect[3][10];		// 0~2:스테이지번호, 0:좌상단
	int		InfoBtm2Pick[10];		// 0:좌상단
	int		InfoSortPick[2][5];		// 0: Rear 1: Front, 0~5:좌측부터 0 

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
	// Port 별로 저장해야함.
	BYTE	byInspectDone[2][30][40];	// 검사완료 (5bit 사용 ==> 0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2)
	int		nInspectInfo[2][30][40];	// 검사결과 (0:Empty, 1:Good, 2:Normal, 3:Mes, 4:N1, 5:N2)
	CString sNGData[2][30][40][7];		// NG 코드  (0:Angle, 1:Btm1_SP, 2:Top1, 3:Top2, 4:Btm2 5:Btm1_AG 6:Btm1_3D)
	char	cJudgeCode1[30][40][7];	// 판정코드 (0:Angle, 1:Btm1_SP, 2:Top1, 3:Top2, 4:Btm2 5:Btm1_AG 6:Btm1_3D)
	char    cJudgeCode2[30][40][7];
	
	CString sSortWaitStartTime[2];
	BOOL	bRosDone[2][30][40];		// ROS 판정 대기 Log 용

	BOOL	bCycleStop;				// Run 중간에 멈출때 (초기화 필요)

	CString	sErrBtm1PickNo;			// Error 상세 처리 (Picker 1 ~ 10)
	int		nErrBtm1PickIdx;		// MTBA 개선 로그 (2024.01.24)
	int		nErrBtm2PickIdx;		// MTBA 개선 로그 (2024.01.24)

	BOOL	bNgTrayEnd;				// Lot 종료 되고 NG Door 먼저 열수 있게 해준다. 
	BOOL	bNGTrayWait;			// NG 모듈 제거하고 다시 투입 할때까지 대기
	BOOL	bGoodTrayLotEnd[2];
	BOOL	bGoodTrayWait;			// Good 모듈 배출 완료까지 대기
	BOOL	bEmptyZWait;			// Empty Z Port Full Tray

	BOOL	bLoadLampOn[2];			// Load Port Lamp 켜지는 시점을 Lot End되는 시점으로 바꿔준다.

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

	DWORD	dwBtm1ScanTime;			// Load Complete 부터 Scan Complete까지
	DWORD	dwTop1ScanTime;			// Load Complete 부터 Scan Complete까지
	DWORD	dwTop2ScanTime;			// Load Complete 부터 Scan Complete까지
	DWORD	dwBtm2ScanTime;			// Load Complete 부터 Scan Complete까지

	DWORD	dwMStopStartTime;		// 작업자 기인 알람, Stop 버튼or스위치 눌러서 장비 세웠을때 
	DWORD	dwMStopEndTime;
	DWORD	dwMStopProcTime;

	int		nTrayX;					// Tray X Count (R04A:4, R04B:5, R04C:8)
	int		nTrayY;					// Tray Y Count (R04A:5, R04B:3, R04C:4)
	int		nScanCnt;				// Scan Count (R04A:2, R04B:2, R04C:1)
	int		nBtmPickQt;				// Bottom Picker Qt (R04A:5, R04B:5, R04C:4)
	int		nSortPickQt;			// Sort Picker Qt (R04A:4, R04B:5, R04C:4)

	BOOL	bBuffTrayChange;		//
	BOOL	bContinueLotEnd;

	BOOL	bVisionVerErr;			// Vision 프로그램 버전이나 파라미터 버전이 달라 알람이 나면 다시 LotStart 보내줘야한다.
	BOOL	bFirstLotStart;
	BOOL	bMesFirstLot;

	double	dEmptyPortZUpOffset;	// Empty Port Z축 Up Offset 위치 저장 해준다.
	double	dEmptyPortZDownOffset;	// Empty Port Z축 Down Offset 위치 저장 해준다.
	BOOL	bEmptyFull;

	int		nErrAngleStageNo;
	int		nErrBufferStageNo;

	CString	sErrVisionLotID;		// 비전에서 알람을 보내준 LotID 저장.

	BOOL	bBuffStageMove;			// Sort Picker 복귀할때 Buffer Y축이 움직였으면 다른 Sort Picker 복귀할때는 Buffer Y축을 움직이면 안된다.
	BOOL	bLotEndBeep;			// NG Full 알림음 때문에 LotEnd 알림음이 Skip 안되게 해준다.

	int		nMesPortNo;				// 연속랏 중 MES 알람 발생하면 다시 시작할수 있게 해준다.

	int		nSp1Timer;
	int		nSp2Timer;

	BOOL	bAlignReply;
	BOOL	bEmptyZTopCheckRepeat;	// Empty Z축 Top Check Sensor 확인을 2번해준다.

	int		nBtWorkSortPickNo;	// Buffer Tray 작업 중인 Sort Picker No.

	// 마지막 Tray는 LotEnd가 되면 정보가 없기때문에 미리 저장해둔다.
	CString sCLotID[2];				// Cap 장비에 보낼 LotID
	int		nCPortNo[2];			// Cap 장비에 보낼 Port No
	int		nCGoodTrayCount[2];		// Cap 장비에 보낼 stage별 Tray No
	int		nCTrayCmCnt[2];			// Cap 장비에 Tray 보낼때 stage별 Tray에 들어있는 모듈수
	BOOL	bCapTrayUnload;			// Cap 장비에서 트레이를 가져갔는지 확인
	BOOL	bCapTrayLoad;			// Good Port에 트레이를 적재했을때
	int		nLastTrayNo;			// 마지막 Tray 번호 저장

	CString sListName;
	CString sYYMMDD;
	CString sNGFileName;

	CString sAlarmTime[3];
	CString sAlarmList[3];

	CString	sComName;

	// Log 표준화
	CString sStdMotionFile;
	CString sEfficiencyFile;
	int		nStdEqNo;	// Log 표준화 (설비번호)
	int		nStdLotNo;	// Log 표준화 (하루 기준으로 작업한 Lot 번호)
	int		nStdLogDay;	// Log 표준화 (설비번호)

	int		nVisionFOBMode;	// Vision에서 FOB 모드로 스타트하면 메세지창 표시, 구동중 FOB 모드로 변경하면 설비 정지하도록 해준다.

	int		nCmJigNo[2][30][40][10];	// CM Jig 정보 저장 [Port][Tray][CM][Unit]
										// Unit 0:Angle Stage, 1:BTM1 Picker, 2:Inspection Stage No, 3:Inspection Stage Jig No, 4:BTM2 Picker
										// Unit 5:Buffer Stage No, 6:Buffer Stage Pocket No, 7:Sort Picker No, 8:Sort Picker Jig No
	int		nJigNgCnt[2][8][10];		// 지그별 불량 카운트 [Port][Unit][Jig]
										// Unit 0: BTM1, 1:Insp1 Top1, 2:Insp2 Top1, 3:Insp3 Top1, 4:Insp1 Top2, 5:Insp1 Top2, 6:Insp3 Top2, 7:BTM2
	double	dJigNgOver[8][10];			// Error에 표시하기 위해 지그별 불량율이 설정치보다 넘었을때 저장.
	int		nJigNgOverCnt[8][10];		// Error에 표시하기 위해 지그별 불량율이 설정치보다 넘었을때 저장. 

	CString	sAPDNgItem;			// APD Result 파일 확인 결과 NG Item.
	BOOL	bAPDResultErr;			// APD Result 파일 확인 결과.
	BOOL	bAviApdRequest;			// Vision APD 데이터 요청
	BOOL	bAviApdReceive[6];		// Vision별 APD 데이터 받았나 확인(0~4), APD 기록 완료(6) 후 Lotend
	int		nAviApdCnt[5];			// Vision APD 이미지 GV값 개수
	CString sVisionGV[5][20];		// 6side Vision 5개(0:AG, 1:BTM1, 2:TOP1, 3:TOP2, 4:BTM2), 이미지 수량은 비전당 최대 10여장.
	double	dAssyLoadCellAvg;		// APD에 입력할 CapForce 값.

	BOOL	b3DStart;
	CString sLaserData1;
	CString sLaserData2;

	BOOL	b3DGrabFailErr;

	DWORD	dwSkipTime_Sort1;		// 작업자 기인 알람, Stop 버튼or스위치 눌러서 장비 세웠을때 
	DWORD	dwSkipTime_Sort2;

	BOOL	bWasUnlock;
	CTime	tDoorUnlockStartMin;
	CTime	tDoorUnlockStart;
	CTime	tDoorUnlockEnd;
	CTimeSpan tsDoorUnlockTotal;

	int		nDoorLockTime;		//Min
	DWORD	dwDoorStartTime;

	double	dEmptyPort_Z_Limit;

	int		nPNoMESNG[24]; //MES와 실물 수량 불일치 갯수 최대 24개 가정 
	int		nTNoMESNG[24];
	int		nCmNoMESNG[24];

	BOOL	bCapVisionAlarm;
	BOOL	bSortPickCompletelyLotEnd;

	BOOL	bPullForce;
	BOOL	bPullForceEnd;

	BOOL	bUseDryRun;

	int		nTrayCntNG[2][4];			//port 1,2 구분		// 0  1
															// 2  3
 	int		nLogInLevel;
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
	double  dTackTime_Unload;
	double  dTactTime_RunTime;
	double  dTactTime_StoETime; //Start to End 

	int		nGoodCount[2];
	int		nNgCount[2];
	int		nSNgCount[2][6];			// Special NG (MES, N1, N2)

	int		nMatchingNgCount[2][6];		//MES보다 실물 수량 적은 만큼 갯수 증가 
	
	int		nBsNgCount[2];				// Barcode Shift
	int		nRosRequest[2];
	int		nRosGood[2];
	int		nRosNg[2];
	int		nRosRepair[2];
	int		nRosTimeOut[2];

	// Inline Mode일때 캡 조립 완료 후 MES 완공하도록 정보 저장.
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

	int		nErrorCount[2];//port 1,2 
	DWORD	dwRunTime[2]; // 
	DWORD	dwStopTime[2];  //stop time = stop S/W time  + error Time, error 로 인한 정지 시간을 포함 한다. 
	DWORD	dwErrorTime[2]; //

	BOOL	bLotEndComplete[2];

	double	dAverageCycle[20][2][15];

	int		nNGT;
	int		nNGC;
	int		nGDT;
	int		nG1DC;

	DWORD   dwFirstSortPickMoment[2];
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

	int		nAlmCnt[2]; // port 1,2 

} GLOVAL_ALM;

typedef struct {
	double  dTaktTime;
	double	dTakt[24][50];	// 시간당 각 Lot의 Takt (시간당 최대 50개)
	int		nLotCount[24];	// 시간당 Lot 수량 (0~23)
	int		nCmCount[2];	// 주간(0:7~19h), 야간(1:19~7h)

	DWORD	dwTactEachPre[2][6]; //2 sort picker, sort pick max count 6
	DWORD	dwTactEachPost[2][6]; //2 sort picker, sort pick max count 6
} GLOVAL_UPH;

typedef struct {
	int		nLotStatus;				// 0:Stop, 1:Start Send, 2:Start Receive
	DWORD	dwLotStartTime;			// Lot Start time

	// Host 전공정 Data
	CString		sHostLotID;			//Host 수신 Lot ID
	CString		sHostRecipe;		//Host 수신 Recipe ID
	CString		sHostModel;			//Host 수신 Recipe ID
	int			nHostCount;			//Host 수신 CM 총갯수
	CString		sHostCancelCode;	//Host Cancel 수신 code
	CString		sHostCancelText;	//Host Cancel 수신 내용

	// 장비 실적 Data	// LJH Port별로 관리해야할지 정해야한다.
	CString		sBarID[2][30][40];		//Barcode ID(장비) [30]:Tray No, [42]:Pocket No
	CString		sJudge[2][30][40];		//N:NG, H:Special NG, M:성능불량(MES NG)
	CString		sNGCode[2][30][40];		//대표NG Code
} GLOVAL_MES;

extern GLOVAL_DATA	gData;
extern GLOVAL_LOT	gLot;
extern GLOVAL_ALM	gAlm;
extern GLOVAL_UPH	gUph;
extern GLOVAL_MES	gMes;

enum PORTNO
{
	PORT1 = 0,
	PORT2 = 1,
};

#define BTM1_PICKER_Z_Ready 0
#define BTM1_PICKER_Z_TrayDown 1
#define BTM1_PICKER_Z_BTM1SPDown 2
#define	BTM1_PICKER_Z_Inspect 3
#define	BTM1_PICKER_Z_BTM1AG 4
#define	BTM1_PICKER_Z_BTM13D 5


#define MODULE_ALIGN_Z_StopDown 1
#define TOP1_MIRROR_Z_WorkDown 1

#define BTM2_PICKER_Z_InspectDown 1
#define BTM2_PICKER_Z_Btm2Down 2
#define BTM2_PICKER_Z_BufferDown 3



#define SORT_PICKER1_Z_Ready	0
#define SORT_PICKER1_Z_Buffer1Down	1
#define SORT_PICKER1_Z_GoodDown	2
#define SORT_PICKER1_Z_NGDown	3
#define SORT_PICKER1_Z_NGBufferDown	4
#define SORT_PICKER1_Z_Buffer2Down 5


#define SORT_PICKER2_Z_Ready	0
#define SORT_PICKER2_Z_Buffer1Down	1
#define SORT_PICKER2_Z_GoodDown	2
#define SORT_PICKER2_Z_NGDown	3
#define SORT_PICKER2_Z_NGBufferDown	4
#define SORT_PICKER2_Z_Buffer2Down 5

#define GoodStage1_Z_MovingUp	1
#define GoodStage1_Z_UnloadUp	3


#define GoodStage2_Z_MovingUp	1
#define GoodStage2_Z_UnloadUp	3