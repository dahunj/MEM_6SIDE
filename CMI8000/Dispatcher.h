// Dispatcher.h : 헤더 파일
//
#pragma once

// CDispatcher

class CDispatcher : public CWnd
{
	DECLARE_DYNAMIC(CDispatcher)

public:
	CDispatcher();
	virtual ~CDispatcher();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUdpClientReceive(WPARAM wLocalPort, LPARAM lParam);

private:
	CUdpRecvCS	m_UdpRecv;
	CUdpSendCS	m_UdpSend;

	BOOL	m_bConnected;
	CString	m_strRecvCmd;
	
	BOOL	m_bJudgeReq[2][30][40];		// 판정 요청
	BOOL	m_bJudgeDone[2][30][40];	// 판정 완료
	DWORD	m_dwReqStart[2][30][40];	// 요청 시작

	void Get_ConnectRequest(CString sIp);
	void Get_ConnectReply();
	void Get_ConnectEnd();
	void Get_InfoRequest();
	void Get_StatusRequest();
	void Get_StatusReply(CString sStatus);
	void Get_StatusUpdate(CString sStatus);
	void Get_JudgeReply(CString sModel, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sBarcode, CString sCode, CString sJudge);

	void Send_Command(CString strSend);

public:
	BOOL Initialize();
	void Terminate();

	void Reset_JudgeData(int nPortNo);

	void Set_ConnectRequest();
	void Set_ConnectReply();
	void Set_ConnectEnd();
	void Set_StatusRequest();
	void Set_StatusReply(int nStatus);
	void Set_StatusUpdate(int nStatus);
	void Set_LotStart(int nPortNo);
	void Set_LotEnd(int nPortNo);
	void Set_InfoReply();
	void Set_JudgeRequest(int nPortNo, int nTrayNo, int nCmNo);
	void Set_TimeUpdate();
	void Set_DispatcherIp(CString sIp);

	BOOL Is_Connected() { return m_bConnected; }
	BOOL Is_JudgeDone(int nPortNo, int nTrayNo, int nCmNo);
	void Set_JudgeDone(int nPortNo, int nTrayNo, int nCmNo) { m_bJudgeDone[nPortNo-1][nTrayNo-1][nCmNo-1] = TRUE; }

	void Test_Command(int nNo);
};

extern CDispatcher g_objDispatcher;

///////////////////////////////////////////////////////////////////////////////
