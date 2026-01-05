// ToastMsgDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMI8000.h"
#include "ToastMsgDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_AUTOCLOSE  1

CToastMsgDlg::CToastMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOASTMSG_DLG, pParent)
	, m_alpha(200)
	, m_autoCloseMs(2000)
	, m_bgColor(RGB(30, 30, 30))
	, m_textColor(RGB(255, 255, 255))
	, m_borderColor(RGB(80, 80, 80))
{
}

BEGIN_MESSAGE_MAP(CToastMsgDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()   // 추가
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CToastMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// STATIC 컨트롤 서브클래싱
	m_msgStatic.SubclassDlgItem(IDC_STATIC_MSG, this);

	// 팝업 느낌: 작업표시줄/Alt-Tab에 덜 뜨게
	ModifyStyle(WS_CAPTION, 0);
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);

	// 반투명 설정을 위해 layered window 확장 스타일 추가
	ModifyStyleEx(0, WS_EX_LAYERED);

	// 상단에 띄우고 싶으면 주석 해제
	// ModifyStyleEx(0, WS_EX_TOPMOST);

	ApplyAlpha(m_alpha);

	return TRUE;
}

HBRUSH CToastMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		// 내 메시지 스태틱만 처리하고 싶으면 ID로 필터링
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_MSG)
		{
			pDC->SetBkMode(TRANSPARENT);           // 배경 투명
			pDC->SetTextColor(m_textColor);        // 글자색

			// 스태틱 배경을 칠하지 않게(=아래 다이얼로그 배경이 보이게)
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	}
	return hbr;
}


void CToastMsgDlg::SetColors(COLORREF bgColor, COLORREF textColor, COLORREF borderColor)
{
	m_bgColor = bgColor;
	m_textColor = textColor;
	m_borderColor = borderColor;

	// 즉시 반영
	Invalidate(TRUE);
	if (::IsWindow(m_msgStatic.GetSafeHwnd()))
		m_msgStatic.Invalidate(TRUE);
}

void CToastMsgDlg::ApplyAlpha(int alpha)
{
	if (alpha < 0) alpha = 0;
	if (alpha > 255) alpha = 255;

	m_alpha = alpha;

	// LWA_ALPHA: 전체 윈도우 알파
	::SetLayeredWindowAttributes(GetSafeHwnd(), 0, (BYTE)m_alpha, LWA_ALPHA);
}

void CToastMsgDlg::RepositionNearParent()
{
	CRect rcMe;
	GetWindowRect(&rcMe);

	// 기본은 부모 중앙, 부모 없으면 화면 중앙
	CRect rcBase;
	CWnd* pParent = GetParent();

	if (pParent && ::IsWindow(pParent->GetSafeHwnd()))
	{
		pParent->GetWindowRect(&rcBase);
	}
	else
	{
		rcBase.SetRect(0, 0,
			::GetSystemMetrics(SM_CXSCREEN),
			::GetSystemMetrics(SM_CYSCREEN));
	}

	int w = rcMe.Width();
	int h = rcMe.Height();

	int x = rcBase.left + (rcBase.Width() - w) / 2;
	int y = rcBase.top  + (rcBase.Height() - h) / 2;

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CToastMsgDlg::ShowToast(LPCTSTR text, int alpha /*=200*/, UINT autoCloseMs /*=2000*/)
{
	m_text = text ? text : _T("");
	m_autoCloseMs = autoCloseMs;
	m_alpha = alpha;

	if (!::IsWindow(GetSafeHwnd()))
	{
		// 모달리스 생성
		Create(IDD_TOASTMSG_DLG, GetParent());
	}

	// 텍스트 적용
	if (::IsWindow(m_msgStatic.GetSafeHwnd()))
		m_msgStatic.SetWindowText(m_text);

	// 투명도 적용
	ApplyAlpha(m_alpha);

	// 위치
	RepositionNearParent();

	// 표시 (포커스 빼앗지 않게)
	ShowWindow(SW_SHOWNOACTIVATE);
	UpdateWindow();

	// 자동 닫힘 타이머
	/*KillTimer(TIMER_AUTOCLOSE);
	if (m_autoCloseMs > 0)
	SetTimer(TIMER_AUTOCLOSE, m_autoCloseMs, NULL);*/
}

void CToastMsgDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_AUTOCLOSE)
	{
		KillTimer(TIMER_AUTOCLOSE);
		ShowWindow(SW_HIDE);   // 완전 종료 대신 숨김(재사용)
		return;
	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CToastMsgDlg::OnEraseBkgnd(CDC* pDC)
{
	// 깜빡임 줄이기: 배경 지우지 않고 OnPaint에서 채움
	return TRUE;
}

void CToastMsgDlg::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(&rc, m_bgColor);                       // 배경색
	dc.Draw3dRect(&rc, m_borderColor, m_borderColor);       // 테두리색
}

//void CToastMsgDlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// 클릭하면 닫기(숨김)
//	KillTimer(TIMER_AUTOCLOSE);
//	ShowWindow(SW_HIDE);
//	CDialogEx::OnLButtonDown(nFlags, point);
//}

void CToastMsgDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	RECT rect;
	GetWindowRect(&rect);
	if( ( point.x < rect.right  || point.x > rect.left ) ||
		( point.y < rect.top || point.y > rect.bottom ) )
	{
		DefWindowProc(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}
