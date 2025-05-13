// WorkInfoDlg.h : 헤더 파일
//
#pragma once

// CWorkInfoDlg 대화 상자입니다.

class CWorkInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkInfoDlg)

public:
	CWorkInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorkInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORK_INFO_DLG };
	CGroupCS	m_Group[3];
	CLabelCS	m_Label[9];
	CLedRectCS		m_ledBtm1PickInfo[10];
	CLedRectCS		m_ledBtm2PickInfo[10];
	CLedRectCS		m_ledSortPick1Info[5];
	CLedRectCS		m_ledSortPick2Info[5];
	CLedRectCS		m_ledInspect1Info[10];
	CLedRectCS		m_ledInspect2Info[10];
	CLedRectCS		m_ledInspect3Info[10];
	CLedRectCS		m_ledNgBuffer1Info[5];
	CLedRectCS		m_ledNgBuffer2Info[5];
	CLedCS			m_ledVisScanDone[5];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void Initial_Controls();
	void Display_Status();
};

///////////////////////////////////////////////////////////////////////////////
