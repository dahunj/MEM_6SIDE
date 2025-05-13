#pragma once

// CTCPSocketCS 명령 대상입니다.

#include <afxsock.h>

#define UM_TCP_RECEIVE WM_USER+9999

class CTCPSocketCS : public CSocket
{
public:
	CTCPSocketCS();
	virtual ~CTCPSocketCS();

	HWND m_hWnd;
	void CTCPSocketCS::Init(HWND hWnd);
	
	virtual void OnReceive(int nErrorCode);
};


