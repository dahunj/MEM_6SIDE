// CapAttach.h : 헤더 파일
//
#pragma once

// CCapAttach

class CCapAttach : public CWnd
{
	DECLARE_DYNAMIC(CCapAttach)

public:
	CCapAttach();
	virtual ~CCapAttach();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUdpReceive(WPARAM wLocalPort, LPARAM lParam);

private:
	CUdpSocketCS	m_UdpCapAttach;

	CString		m_strRecvCmd;

	BOOL	m_bOpened;
	BOOL	m_bConnected;
	int		m_nStatusCapAttach;

	void Get_ConnectRequest();
	void Get_ConnectReply();
	void Get_ConnectEnd();
	void Get_StatusRequest();
	void Get_StatusReply(CString sStatus);
	void Get_StatusUpdate(CString sStatus);
	void Get_TrayUnload();
	void Get_ApdReply(CString sCapForceAvg);

	void Send_Command(CString strSend);

public:
	BOOL Initialize();
	void Terminate();

	void Set_ConnectRequest();
	void Set_ConnectReply();
	void Set_ConnectEnd();
	void Set_StatusRequest();
	void Set_StatusReply(int nStatus);
	void Set_StatusUpdate(int nStatus);
	void Set_TimeUpdate();
	void Set_LotStart(int nPortNo);
	void Set_LotEnd(int nPortNo);
	void Set_TrayLoad(int nPortNo, int nSNo);
	void Set_TrayEnd(int nPortNo);
	void Set_BarcodeUpdate(int nPortNo, int nTrayNo, int nCmNo, CString sBarcode);
	void Set_ApdRequest();

	BOOL Is_Opened() { return m_bOpened; }
	int  Is_StatusCapAttach() { return m_nStatusCapAttach; }

	void Test_Command(int nNo);
};

extern CCapAttach g_objCapAttach;

///////////////////////////////////////////////////////////////////////////////
