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

// Library 추가
#include "CSIniFile.h" 
#include "CSServerSocket.h"
#ifdef _DEBUG
	#pragma comment(lib, "CSIniFileD.lib")
	#pragma comment(lib, "CSServerSocketD.lib")
#else
	#pragma comment(lib, "CSIniFileR.lib")
	#pragma comment(lib, "CSServerSocketR.lib")
#endif

#define MAIN_VERSION	"V1.1.0"

///////////////////////////////////////////////////////////////////////////////

extern CString gsCurrentDir;	// 현재 프로젝트 폴더

typedef struct {
	int			nAgentType;		// 0:UAO, 1:CAP
	int			nHostPort;
	CString		sEquipId;
	CString		sVersion;
	BOOL		bHandlerLog;
	BOOL		bHostLog;
	CString		sErrFile;
	CString		sOperId;
	int			nPreEquipState;	// 1:Run, 4:Idle, 5:Down
	int			nCurEquipState;	// 1:Run, 4:Idle, 5:Down
	CString		sReCmId[100];	// Retest Modle => Max 100
	CString		sRecipe;

	CString		sBodyData[8];
} GLOVAL_DATA;

typedef struct {
	CString		sHostLotId;
	CString		sHostRecipe;
	CString		sHostCMCount;
	
	CString		sRTSTID;
	CString		sLabelType;

	CString		sFailCode;
	CString		sFailText;

	CString		sHostLdMGZId;
	CString		sHostUldMGZId;
	CString		sHostTrayID;
	
	int			nHostLensCount;
	CString		sHostRtstId;	// ReTest Lot-ID
	CString		sHostLabel;		// AVI NG, CAP OK, AVI OK

	CString		sCancelCode;
	CString		sCancelText;
		
	int			nHostType;		
	

	BOOL		bC_TimoutStarted[20];
	DWORD		dwC_Timeout[20];

	BOOL		bT_TimoutStarted[20];
	DWORD		dwT_Timeout[20];

	


} GLOVAL_MES;

typedef struct {
	CString		sIdleCode;
	CString		sIdleText;
	CString		sIdleSTime;
	CString		sIdleETime;
} GLOVAL_IDLE;

typedef struct {
	int			nAlmSet;
	CString		sAlmNo;
	CString		sAlmCat;
	CString		sAlmMsg;
} GLOVAL_ALM;


struct eC_Timeout
{
	enum Name
	{
		LOT_REPORT = 0,
		PP_SELECTED = 1,		
	};
};

struct eT_Timeout
{
	enum Name
	{
		eControlState = 0,
		eEquipState = 1,
		eLotIDReport = 2,
		ePPSelected = 3,
		eLotStarted = 4,
		eProductCompleted = 5,
		eLotEnd = 6,
		eLotAbort = 7,
		eIdleSet = 8,
		eIdleReset = 9,
		eIdleReport = 10,
	};
};


extern  GLOVAL_DATA	gData;
extern  GLOVAL_MES	gMes;
extern  GLOVAL_IDLE	gIdle;
extern  GLOVAL_ALM	gAlarm;
