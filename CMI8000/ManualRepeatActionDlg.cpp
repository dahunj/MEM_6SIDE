// ManualRepeatActionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMI8000.h"
#include "ManualRepeatActionDlg.h"
#include "afxdialogex.h"


// CManualRepeatActionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualRepeatActionDlg, CDialogEx)

CManualRepeatActionDlg::CManualRepeatActionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualRepeatActionDlg::IDD, pParent)
{

}

CManualRepeatActionDlg::~CManualRepeatActionDlg()
{
}

void CManualRepeatActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_0, m_Group[0]);
	for(inti = 0; i < 3; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);

}


BEGIN_MESSAGE_MAP(CManualRepeatActionDlg, CDialogEx)
END_MESSAGE_MAP()


// CManualRepeatActionDlg 메시지 처리기입니다.
