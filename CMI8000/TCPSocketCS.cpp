// TCPSocketCS.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMI8000.h"
#include "TCPSocketCS.h"


// CTCPSocketCS

CTCPSocketCS::CTCPSocketCS()
{
}

CTCPSocketCS::~CTCPSocketCS()
{
}


// CTCPSocketCS 멤버 함수


void CTCPSocketCS::OnReceive(int nErrorCode)
{
	SendMessage(m_hWnd, UM_TCP_RECEIVE, 0, 0);
	CSocket::OnReceive(nErrorCode);
}

void CTCPSocketCS::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

