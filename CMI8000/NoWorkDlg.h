// NoWorkDlg.h : 헤더 파일
//
#pragma once

#include "afxcmn.h"

// CNoWorkDlg 대화 상자입니다.

// 비가동 집계를 위한 클래스
class CNoWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNoWorkDlg)

public:
	CNoWorkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNoWorkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NO_WORK_DLG };
	CGroupCS	m_Group[3];
	CLabelCS	m_lblNoWorkTitle;
	CButtonCS	m_btnStopReason[24];
	CLabelCS	m_lblNoWorkTime[4];
	CStaticCS	m_stcNoWorkUser;
	CStaticCS	m_stcNoWorkStart;
	CStaticCS	m_stcNoWorkEnd;
	CStaticCS	m_stcNoWorkTerm;
	CButtonCS	m_btnNoWorkExit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBtnStopReasonClick(UINT nID);
	afx_msg void OnStnClickedStcNoWorkUser();
	afx_msg void OnBnClickedBtnNoWorkExit();

private:
	CString		m_strUser;
	CString		m_strCode;
	CString		m_strText;
	CString		m_strData[24][3];

	BOOL		m_bNoWorkAuto;
	BOOL		m_bNoWorkStart;

	DWORD		m_dwNoWorkStart;
	CString		m_strTimeS, m_strTimeE;

private:
	void Initial_Controls();
	BOOL Read_StopLossList();
	void NoWork_Start();
	void NoWork_End();
	void Set_NoWorkReport();
	void Reset_NoWorkReport();
	void Auto_NoWorkReport();

public:
	void Set_NoWorkAuto(BOOL bAuto) { m_bNoWorkAuto = bAuto; }
};

extern CNoWorkDlg g_dlgNoWork;
