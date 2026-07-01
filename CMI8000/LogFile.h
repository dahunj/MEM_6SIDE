// LogFile.h : 헤더 파일
//
#pragma once

class CLogFile  
{
public:
	CLogFile();
	virtual ~CLogFile();

public:
	void Create_Folder(CString sPath);

	void Save_AlarmLog(const CString& sLog);
	void Save_AlarmResetLog(const CString& sLog);
	void Save_HandlerLog(const CString& sLog);
	void Save_SaveRunTimeLog(const CString& sLog);
	void Save_InspectorLog(const CString& sLog);
	void Save_JobListLog(const CString& sLog, BOOL bMode=FALSE);
	void Save_LotResult(int nPNo,const  CString& sLog);
	void Save_LotLog(int nPNo);
	void Save_MesAgentLog(const CString& sLog);
	void Save_DispatcherLog(const CString& sLog);
	void Save_CapAttachLog(const CString& sLog);
	void Save_LotTime(int nPNo, const CString& sLog);
	void Save_LotError(const CString& sLog, int nPNo=0);
	void Save_ECMLog(int nType, const CString& strLog);	//nType:1[Alarm], 2[Joblist] 3[Inspect]
	void Save_TestLog(const CString& sLog);
	void Save_OperatingRatio(const CString& sLog);
	void Save_FocusAxisLog(int Axis, const CString& sLog);
	void Save_RosWaitLog(int nPNo, const CString& sLog);
	void Save_InspectWaitLog(int nPNo, const CString& sLog);
	void Save_UnloadingTime(int nSNo, int nPNo);
	void Save_MachineStopLog(const CString& sLog);
	void Save_PickerLog(const CString& sLog);
	
	void Save_MCCLog(const CString& sLog);
	void Save_MotionLog(const CString& sLog);


	void Save_TerminalLog(const CString& sLog);

	void Save_DoorInterlock(int nPNo, const CString& sLog, BOOL bfirst);
	void Save_PCLog(int nPNo, const CString& sLog);

	void Save_PositionLog(int nPNo, int nTNo, int nCmNo, int nAxis, int nMoveIdx);

	void Save_AverageCycle(int nPNo);
	
	void Save_DailyLot(const CString& sLog);

	void Save_OutTray(const CString& strOut, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo);
	void Save_ECMOutTray(const CString& sLog, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo);
	void Save_CmTrackingLog(const CString& strOut, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo);
	void Save_ECMTracking(const CString& sLog, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo);

	// Log 표준화
	void Save_StdMotionLog(CString sType, int nZone, int nCase, int nValue, CString sZone, CString sMsg);
	void Save_EfficiencyLog(int nCase, CString sStatus, int nCode, CString sNote);
	void Get_ZoneMsg(int nZone, int nCase, CString &sZone, CString &sMsg);

	void	Get_PosName(int nAxis, int nMoveIdx, CString &sName);
};

extern CLogFile g_objLogFile;

///////////////////////////////////////////////////////////////////////////////
