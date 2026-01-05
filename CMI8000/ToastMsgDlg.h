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

	// 추가: 폰트 설정
	// heightPt: 포인트 크기(예: 12, 14, 16)
	// bold: true면 Bold
	// italic/underline: 필요시
	// faceName: "Segoe UI", "맑은 고딕" 등
	void SetMessageFont(int heightPt,
		bool bold,
		bool italic = false,
		bool underline = false,
		LPCTSTR faceName = _T("Segoe UI"));

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
	void ApplyFontToStatic();   // 추가

private:
	CString   m_text;
	int       m_alpha;
	UINT      m_autoCloseMs;
	CStatic   m_msgStatic;
	
	// 추가: 커스텀 색상
	COLORREF  m_bgColor;
	COLORREF  m_textColor;
	COLORREF  m_borderColor;

	// 추가: 폰트 보관 (반드시 멤버로 유지)
	CFont     m_msgFont;
	LOGFONT   m_lf;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};