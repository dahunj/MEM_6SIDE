#pragma once


const int INSPECTOR_ALL		= 0;	// PC1, PC2, PC3, PC4
const int INSPECTOR_PC1		= 1;	// Angle, Align, Btm1
const int INSPECTOR_PC2		= 2;	// Top1
const int INSPECTOR_PC3		= 3;	// Top2(Top2, Top3)
const int INSPECTOR_PC4		= 4;	// Btm2(Btm2, Btm3)



// CVisionCommManager

class CVisionCommManager : public CWnd
{
	DECLARE_DYNAMIC(CVisionCommManager)

public:
	CVisionCommManager();
	virtual ~CVisionCommManager();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnUdpReceive(WPARAM wLocalPort, LPARAM lParam);


private:
	CUdpSocketCS	m_UdpVisionPC1;
	CUdpSocketCS	m_UdpVisionPC2;
	CUdpSocketCS	m_UdpVisionPC3;
	CUdpSocketCS	m_UdpVisionPC4;

	CString m_strRecvCmd;

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

	void DoEvents(int nSleep = 0);

	void Get_ConnectRequest(int nInspector);
	void Get_ConnectReply(int nInspector);
	void Get_ConnectEnd(int nInspector);
	void Get_StatusRequest(int nInspector);
	void Get_StatusReply(int nInspector, CString sStatus);
	void Get_StatusUpdate(int nInspector, CString sStatus);

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

	
};

extern CVisionCommManager g_objVIsionManager;

