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

	void Get_LotAbort(CString sLotId, CString sRecipe);
	void Get_IdleSet(CString sOperId, CString sCode);
	void Get_IdleReset(CString sOperId, CString sCode);
	void Get_IdleReport(CString sOperId, CString sCode, CString sText, CString sSTime, CString sETime);
	
	void Get_LotIDReport(CString sType, CString sLotID, CString sPortNo, CString sRecipe);
	void Get_PPSelectedReport(CString sLotID, CString sRecipeId);

	void Get_LotStartedReport(CString sOperID, CString sLotID, CString sRecipe, CString sCMCount);
	void Get_ProductCompletedReport(CString sOperID, CString sLotID, CString sTNo, CString sCNo, CString sResult, CString sReasonCode, CString sBarcode, CString sUnitNo);
	
	//void Get_PPUploadCompletedReport(CString sLotID, CString sMGZId, CString sRecipeId);	
	
	void Send_Command(CString sSend);

public:
	void Initialize();
	void Terminate();

	BOOL Is_Connected() { return m_bConnected; }

	void Set_ControlState(int nFlag);	// 1:Online, 2:Offline
	void Set_ErrorReply();	// Error Update 응답

	void Set_TerminalDisplay(CString sDisplay);
	void Set_TimeSync();
	
	void Set_PPSelect();
	
	void Set_Lot_Start();
	void Set_Lot_ID_Fail();
	
	//void Set_PP_Upload_Confirm();
	//void Set_PP_Upload_Fail();




};

extern CHandler g_objHandler;

///////////////////////////////////////////////////////////////////////////////
