#pragma once


// CManualRepeatActionDlg 대화 상자입니다.

class CManualRepeatActionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualRepeatActionDlg)

public:
	CManualRepeatActionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualRepeatActionDlg();

	CGroupCS	m_Group[1];
	CLabelCS	m_Label[3];
	CComboCS	m_cboPicker;
	CComboCS	m_cboPickNum;
	CEditCS		m_edtDelay;


// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_REPEAT_ACTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
