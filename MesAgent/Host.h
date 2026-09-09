// Host.h : 헤더 파일
//
#pragma once

#include "./CXml/Xml.h"

using namespace JWXml;

// CHost

class CHost : public CWnd
{
	DECLARE_DYNAMIC(CHost)

public:
	CHost();
	virtual ~CHost();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnServerAccept(WPARAM wLocalPort, LPARAM lClientIdx);
	afx_msg LRESULT OnServerReceive(WPARAM wLocalPort, LPARAM lClientIdx);
	afx_msg LRESULT OnServerRemove(WPARAM wLocalPort, LPARAM lClientIdx);

private:
	CServerSocketCS	m_Server;
	UINT			m_nLPort;

	BOOL	m_bConnected;
	BOOL	m_bHostOnline;
	CString	m_strRecvCmd;

	CXml	m_xml;

	int		m_nRecvCmdCount;	// 4Byte
	int		m_nSendCmdCount;

	CString m_strStFn;	// StreamFunction (S1F1, S2F3, S2F31, S2F49, S6F12)
	CString m_strRcmd;	// RCMD Command (START, CANCEL, DATA, PERMIT)

	CString m_strName;
	CString m_strStream;
	CString m_strFunction;

	DWORD	m_dwLastTime;	// 마지막 통신 시간
	CString m_strSetTime;	// Host 설정 시간

	CString m_strCEID;
	CString m_strRPTID;

	CString m_strDisplay;	// Teminal Display Message

private:
	BOOL Extract_Xml(CString sXmlData);

	void Get_S1F1_Ready();				// Are You There Request
	void Get_S1F3_State();				// Equip Status Request
	void Get_S2F3_Link();				// Link Test Request
	void Get_S2F31_Time();				// Date and Time Set Request

	void Reply_HeartBeat();				// Heart Beat
	

	void Get_S2F49_PPSelect();
	
	void Get_S2F49_PP_UPLOAD_CONFIRM();
	void Get_S2F49_PP_UPLOAD_FAIL();

	void Get_S2F49_LOT_START();
	void Get_S2F49_LOT_ID_FAIL();

	void Get_S7F19_PPListRequest();		// Current EPPD Request
			
	void Send_Command(CString sSend, BOOL bReply, CString sStFn, CString sRcmd="");	// XML

public:
	void Initialize();
	void Terminate();

	BOOL Is_Connected() { return m_bConnected; }
	BOOL Is_HostOnline() { return m_bHostOnline; }
	DWORD Get_LastTime() { return m_dwLastTime; }

	void Set_S1F1_Ready();		// Are You There Request
	void Set_S5F1_AlarmReport(int nFlag, CString sErrNo, CString sErrMsg);	// nFlag(1:Alarm, 0:해제) Alarm Report Send
	void Set_S7F26();
	
	void Set_S6F11_ControlState(int nState);	// 1:Online, 2:Offline
	void Set_S6F11_EquipState(int nState, CString sErrNo, CString sCategory, CString sErrMsg);	// 2:Idle, 5:Run, 6:Down
	
	void Set_S6F11_LotIDReport(CString sType, CString sLotID, CString sPortNo, CString sRecipe);
	void Set_S6F11_PPSelectedReport(CString sLotId, CString sRecipeId);

	void Set_S6F11_LotStartedReport(CString sOperID, CString sLotID, CString sRecipe, CString sCMCount);
	void Set_S6F11_ProductCompletedReport(CString sOperID, CString sLotID, CString sTrayNo, CString sCMNo, CString sResult, CString sReasonCode, CString sBarcode, CString sUnitNo);
	
	void Set_S6F11_LotEnd(CString sLotId, CString sRecipe, int nCount, int nOk, int nNg, int nBNg);
	void Set_S6F11_PPUploadCompleted(CString sLotID, CString sRecipe);

	void Set_S6F11_LotAbort(CString sLotId, CString sRecipe);	// Lot Suspended Report
	void Set_S6F11_IdleSet();
	void Set_S6F11_IdleReset();
	void Set_S6F11_IdleReport();

	//////////
	void Set_S2F50_PPSelect(int nFail);
	
	void Set_S2F50_PP_UPLOAD_CONFIRM();
	void Set_S2F50_PP_UPLOAD_FAIL();
	
	void Set_S2F50_LOT_START();
	void Set_S2F50_LOT_ID_FAIL();
	
	void Set_SSF0_Abort_Transaction(CString sStream);

	void Set_S9F3_Unrecognized_Stream();
	void Set_S9F5_Unrecognized_Function();

	void Set_S9F9_T_Timeout();
	void Set_S9F13_Timeout();	// Conversation Timeout

	void Set_S7F20_PPListReply(CString sName);

	void Test_Command();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

extern CHost g_objHost;

///////////////////////////////////////////////////////////////////////////////
