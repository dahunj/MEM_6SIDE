#pragma once

#include "TCPSocketCS.h"
// CLaserComm

class CLaserComm : public CWnd
{
	DECLARE_DYNAMIC(CLaserComm)

public:
	CLaserComm();
	virtual ~CLaserComm();
	
	CTCPSocketCS m_client;

	BOOL m_bConnection;

	void Initialize();
	void Connect();
	void Terminate();
	BOOL getConnection();

	void Set_M0();
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg LPARAM OnTcpReceive(UINT wParam, LPARAM lParam);
};

extern CLaserComm g_objLaserComm;

