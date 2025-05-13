// Inspector.h : 헤더 파일
//
#pragma once

const int INSPECTOR_ALL		= 0;	// PC1, PC2, PC3, PC4
const int INSPECTOR_PC1		= 1;	// Angle, Align, Btm1
const int INSPECTOR_PC2		= 2;	// Top1
const int INSPECTOR_PC3		= 3;	// Top2(Top2, Top3)
const int INSPECTOR_PC4		= 4;	// Btm2(Btm2, Btm3)

// CInspector

class CInspector : public CWnd
{
	DECLARE_DYNAMIC(CInspector)

public:
	CInspector();
	virtual ~CInspector();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnUdpReceive(WPARAM wLocalPort, LPARAM lParam);

private:
	CUdpSocketCS	m_UdpVisionPC1;
	CUdpSocketCS	m_UdpVisionPC2;
	CUdpSocketCS	m_UdpVisionPC3;
	CUdpSocketCS	m_UdpVisionPC4;

	CString		m_strRecvCmd;

	BOOL	m_bConnectPC1;
	BOOL	m_bConnectPC2;
	BOOL	m_bConnectPC3;
	BOOL	m_bConnectPC4;

	int		m_nStatusPC1;		// Vision PC1 상태 (0:Not Ready, 1:Ready)
	int		m_nStatusPC2;		// Vision PC2 상태 (0:Not Ready, 1:Ready)
	int		m_nStatusPC3;		// Vision PC3 상태 (0:Not Ready, 1:Ready)
	int		m_nStatusPC4;		// Vision PC4 상태 (0:Not Ready, 1:Ready)

	BOOL	m_bLotReady1;
	BOOL	m_bLotReady2;
	BOOL	m_bLotReady3;
	BOOL	m_bLotReady4;

	BOOL	m_bHddVision[4][2];	// [PC][0:Reply,1:Status]

	int		m_nB1_SPScanReq, m_nB1_AGScanReq, m_nT1ScanReq, m_nT2ScanReq, m_nB2ScanReq, m_nB1_3DScanReq;
	int		m_nB1_SPScanCnt, m_nB1_AGScanCnt, m_nT1ScanCnt, m_nT2ScanCnt, m_nB2ScanCnt, m_nB1_3DScanCnt;
	int		m_nFOBModeCnt;	// Vision PC 4대에서 모두 받아야한다.
	int		m_nFOBMode[4];	// Vision PC 4대에서 모두 받아야한다.

	double	m_dAlignX[32], m_dAlignY[32], m_dAlignQ[32];

	void DoEvents(int nSleep = 0);

	void Get_ConnectRequest(int nInspector);
	void Get_ConnectReply(int nInspector);
	void Get_ConnectEnd(int nInspector);
	void Get_StatusRequest(int nInspector);
	void Get_StatusReply(int nInspector, CString sStatus);
	void Get_StatusUpdate(int nInspector, CString sStatus);

	void Get_LotReady(int nInspector, CString sLotId, CString sPortNo, CString sProgVer, CString sParaVer);
	void Get_ReloadRequest(int nInspector, CString sPc);
	void Get_ScanComplete(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo);
	void Get_InspectComplete(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sJudge, CString sNGCode);
	void Get_AMoveRequest(int nInspector, CString sGbn, CString sZ);
	void Get_PositionRequest(int nInspector, CString sGbn);
	void Get_AlignComplete(int nInspector, CString sLotId, CString sPortNo, CString sTrayNo, CString sSeqNo, CString sCmCnt, CString sResult);
	void Get_ShiftRequest(int nInspector, CString sGbn);
	void Get_SMoveRequest(int nInspector, CString sGbn, CString sZ);
	void Get_BarcodeResult(int nInspector, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sBarcode);
	void Get_ErrorRequest(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sErrNo);
	void Get_AlarmRequest(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sErrNo, CString sInfo1, CString sInfo2, CString sInfo3);
	void Get_FOBUpdate(int nInspector, CString sStatus);
	void Get_FOBReply(int nInspector, CString sStatus);
	void Get_APDReply(int nInspector, CString strRecv);


	void Get_TimeUpdate(int nInspector, CString sTime);
	
	void Get_HeartBeat(int nInspector);
	void Get_3DStart(int nInspector);

	void Send_Command(int nInspector, CString strSend);
	void Exception_Log(CString sFunc, CString sGbn, int nCase);	// Recevie Exception Log
	
public:
	void Initialize();
	void Terminate();

	void Set_ConnectRequest(int nInspector);
	void Set_ConnectReply(int nInspector);
	void Set_ConnectEnd(int nInspector);
	void Set_StatusRequest(int nInspector);
	void Set_StatusReply(int nInspector, int nStatus);
	void Set_StatusUpdate(int nInspector, int nStatus);

	void Set_LotStart(int nInspector, CString sLotId, int nPortNo, CString sProgVer, CString sParaVer);
	void Set_LotEnd(int nInspector, CString sLotId, int nPortNo);
	void Set_LotRdyCom(int nInspector, CString sLotId);

	void Set_LoadComplete(int nInspector, CString sGbn, CString sLotId, int nPortNo, int nTNo1, int nTNo2, int nTNo3, int nTNo4, int nCNo1, int nCNo2, int nCNo3, int nCNo4);
	void Set_3DLoadComplete(int nInspector, CString sGbn, CString sLotId, int nPortNo, 
		int nTNo1, int nTNo2, int nTNo3, int nTNo4, int nTNo5, int nTNo6, int nTNo7, int nTNo8,
		int nCNo1, int nCNo2, int nCNo3, int nCNo4, int nCNo5, int nCNo6, int nCNo7, int nCNo8);
	void Set_ReloadComplete(int nInspector, CString sPc);
	void Set_CycleStop(int nInspector);
	void Set_MoveComplete(int nInspector, CString sGbn);
	void Set_PositionReply(int nInspector, CString sGbn, double dZ);
	void Set_AlignRequest(int nInspector, CString sLotId, int nPortNo, int nTrayNo, int nSeqNo);
	void Set_ShiftComplete(int nInspector, CString sGbn);
	void Set_SMoveComplete(int nInspector, CString sGbn);
	void Set_BarcodeUpdate(int nInspector, CString sLotId, int nPortNo, int nTrayNo, int nCmNo, CString sBarcode);
	void Set_InitialRequest(int nInspector);
	void Set_TimeUpdate(int nInspector);
	void Set_FOBRequest(int nInspector);
	void Set_ApdRequest(int nInspector, CString sGbn, int nPortNo);
	void Set_3DENDComplete(int nInspector);


	int  Get_VisionStatus(int nInspector);
	BOOL Check_LotReady();
	
	BOOL Get_HddStatus(int nInspector) { return m_bHddVision[nInspector-1][1]; }
	void Set_HddStatus(int nInspector, BOOL full) {m_bHddVision[nInspector-1][1] = full;}

	void Test_Command();

	double Get_AlignX(int nIdx) { return m_dAlignX[nIdx]; }
	double Get_AlignY(int nIdx) { return m_dAlignY[nIdx]; }
	double Get_AlignQ(int nIdx) { return m_dAlignQ[nIdx]; }

	int Get_Btm1SPScanRequest() {return m_nB1_SPScanReq;}
	int Get_Btm1AGScanRequest() {return m_nB1_AGScanReq;}
	int Get_Btm2ScanRequest() {return m_nB2ScanReq;}
	int Get_Top1ScanRequest() {return m_nT1ScanReq;}
	int Get_Top2ScanRequest() {return m_nT2ScanReq;}

	void Set_InspectResult(int nPC, int nPortNo, int nTrayNo, int nCmNo);
};

extern CInspector g_objInspector;

///////////////////////////////////////////////////////////////////////////////
