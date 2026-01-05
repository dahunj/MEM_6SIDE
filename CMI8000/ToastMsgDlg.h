// ToastMsgDlg.h
#pragma once

class CToastMsgDlg : public CDialogEx
{
public:
	CToastMsgDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_TOASTMSG_DLG };

	void ShowToast(LPCTSTR text, int alpha = 200, UINT autoCloseMs = 2000);

	// 추가: 색상/배경 설정
	void SetColors(COLORREF bgColor, COLORREF textColor, COLORREF borderColor);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);

	// 추가: STATIC 색상/투명 처리
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

private:
	void ApplyAlpha(int alpha);
	void RepositionNearParent();

private:
	CString   m_text;
	int       m_alpha;
	UINT      m_autoCloseMs;
	CStatic   m_msgStatic;

	// 추가: 커스텀 색상
	COLORREF  m_bgColor;
	COLORREF  m_textColor;
	COLORREF  m_borderColor;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};