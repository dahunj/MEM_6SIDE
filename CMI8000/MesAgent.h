#pragma once


// CMesAgent

class CMesAgent : public CWnd
{
	DECLARE_DYNAMIC(CMesAgent)

public:
	CMesAgent();
	virtual ~CMesAgent();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnClientConnect(WPARAM wConnect, LPARAM lParam);
	afx_msg LRESULT OnClientReceive(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CClientSocketCS m_Client;

	BOOL	m_bConnected;
	BOOL	m_bHostOnline;
	CString m_strRecvCmd;


private:
	void Get_ReciveData();
	void Get_ControlState(CString sFlag);	// 1:Online, 2:Offline
	void Get_TimeSync();

	//new 
	void Get_PPSelect(CString sLotId, CString sRecipe, CString sOperID);
	void Get_LotIDFail(CString sLotId, CString sRTSTID, CString sLabelType, CString sCode, CString sText);
		
	void Get_LotStart(CString sLotId, CString sRecipe, CString sCMCount);
			
	//old 
	//void Get_PPUpload_Confirm(CString sRecipeID);
	//void Get_PPUpload_Fail(CString sRecipeID, CString sFailCode, CString sFailText);
	
	void Send_Command(CString sSend);

public:
	void Initialize();
	void Terminate();

	BOOL Is_Connected() { return m_bConnected; }
	BOOL Is_HostOnline() { return m_bHostOnline; }
	BOOL Exist_Recipe(CString sRecipe);

	void Set_OperUpdate(CString sOperId);				// Operator ID 변경시 보고
	void Set_ControlState(int nFlag, CString sOperId);	// 1:Onine, 2:Offline
	void Set_EquipState(int nFlag);						// 
	void Set_ErrorUpdate(int nFlag, CString sErrNo);	// 0:해제, 1:발생
		
	void Set_IdleReport(CString sOperId, CString sSTime, CString sETime, CString sCode, CString sType);	//1:Start, 2:End

	//new
	void Set_LotIDReport(int nType, CString sLotID, int nPortNo, CString sRecipe);	
	void Set_PPSelectedReport(CString sLotId, CString sRecipeId);

	void Set_LotStartedReport(CString sOperID, CString sLotId, CString sRecipe, CString sCMCount);
	void Set_ProductCompletedReport(CString sOperID, CString sLotID, int nTrayNo, int nCMNo,  CString sResult, CString sReasonCode, CString sCMBarcode, int UnitNo);
	
	//void Set_LotCompleteReport(CString sLotID, int nPortNo, CString sRecipe, int nCmTotal, int nRealTotal, int nGoodCnt, int nBadCnt);
	//old 
	//void Set_PPUploadCompletedReport(CString sLotId, CString sMGZId, CString sRecipeId);
			
};


extern CMesAgent g_objMesAgent;

