// MainDlg.h : 헤더 파일
//
#pragma once

#include "VisionCommManager.h"

// CMainDlg 대화 상자
class CMainDlg : public CDialogEx
{
// 생성입니다.
public:
	CMainDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnUdpReceive(WPARAM wLocalPort, LPARAM lParam);
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnResetHost();
	afx_msg void OnLbnDblclkLstMsg();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnMsgClear();
	afx_msg void OnBnClickedBtnExit();

private:
	CUdpSocketCS m_Udp;
	UINT		 m_nLPort;	// Local Port
	UINT		 m_nHPort;	// Host Port
	CString		 m_strHIp;	// Host IP
};
