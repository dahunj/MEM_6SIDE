// Handler.h : 헤더 파일
//
#pragma once

// CHandler

#define HANDLER_PORT	10000	// Local Port for Handler

class CHandler : public CWnd
{
	DECLARE_DYNAMIC(CHandler)

public:
	CHandler();
	virtual ~CHandler();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnServerAccept(WPARAM wLocalPort, LPARAM lClientIdx);
	afx_msg LRESULT OnServerReceive(WPARAM wLocalPort, LPARAM lClientIdx);
	afx_msg LRESULT OnServerRemove(WPARAM wLocalPort, LPARAM lClientIdx);

private:
	CServerSocketCS	m_Server;
	UINT			m_nLPort;

	BOOL	m_bConnected;
	CString	m_strRecvCmd;

private:
	void Get_OperUpdate(CString sOperId);
	void Get_ControlState(CString sFlag, CString sOperId);	// 1:Online, 2:Offline
	void Get_EquipState(CString sState);					// 1:Run, 4:Idle, 5:Down
	void Get_ErrorUpdate(CString sFlag, CString sErrNo, CString sCategory);

	//new
	void Get_LotIDReport(CString sType, CString sLotID, CString sPortNo, CString sRecipe);
	void Get_PPSelectedReport(CString sLotID, CString sRecipeId);

	void Get_LotStartedReport(CString sOperID, CString sLotID, CString sRecipe, CString sCMCount);

	//old 
	void Get_MGZIDReport(CString sType, CString sMGZId);
	
	void Get_PPUploadCompletedReport(CString sLotID, CString sMGZId, CString sRecipeId);	

	void Get_TrayIDReport(CString sType, CString sTrayID);
	void Get_TrayStartedReport(CString sLotID, CString sTrayID, CString sRecipeID);

	void Send_Command(CString sSend);

public:
	void Initialize();
	void Terminate();

	BOOL Is_Connected() { return m_bConnected; }

	void Set_ControlState(int nFlag);	// 1:Online, 2:Offline
	void Set_ErrorReply();	// Error Update 응답

	void Set_TerminalDisplay(CString sDisplay);
	void Set_TimeSync();
	
	//New 
	void Set_PPSelect();
	void Set_MGZ_Cancel();

	void Set_MGZ_Confirm();
	void Set_PP_Upload_Confirm();
	void Set_PP_Upload_Fail();

	void Set_Lot_Start();
	void Set_Lot_ID_Fail();

	void Set_TrayID_Confirm();
	void Set_Tray_Cancel();
};

extern CHandler g_objHandler;

///////////////////////////////////////////////////////////////////////////////
