// LogFile.cpp: implementation of the CLogFile class.
//
#include "stdafx.h"
#include "LogFile.h"
#include "AJinDefine.h"
#include "DataManager.h"
#include "Common.h"

// CLogFile
CLogFile g_objLogFile;

CCriticalSection g_csAlarmLog;
CCriticalSection g_csAlarmResetLog;
CCriticalSection g_csHandlerLog;
CCriticalSection g_csRunTimeLog;
CCriticalSection g_csInspectorLog;
CCriticalSection g_csJobListLog;
CCriticalSection g_csMesAgentLog;
CCriticalSection g_csDispatcherLog;
CCriticalSection g_csCapAttachLog;
CCriticalSection g_csDailyLotLog;
CCriticalSection g_csRosWaitLog;
CCriticalSection g_csInspectWaitLog;
CCriticalSection g_csECMLog;
CCriticalSection g_csStdMotionLog;
CCriticalSection g_csEfficiencyLog;
CCriticalSection g_csMachineStopLog;
CCriticalSection g_csCmTrackingLog;
CCriticalSection g_csMCCLog;
CCriticalSection g_csBarcodeLog;

CLogFile::CLogFile()
{
}

CLogFile::~CLogFile()
{
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Create_Folder(CString sPath)
{
	if (sPath == _T("")) return;
	if (sPath.Right(1) == _T("\\")) sPath = sPath.Left(sPath.GetLength() - 1);
	if (GetFileAttributes(sPath) != -1) return;	// Directory Exist!!!

	int nFound = sPath.ReverseFind('\\');
	Create_Folder(sPath.Left(nFound));

	CreateDirectory(sPath, NULL);
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_AlarmLog(CString sLog)
{
	g_csAlarmLog.Lock();

	CString strPath = gData.sLogPath + "\\Alarm";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_Alarm.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csAlarmLog.Unlock();
}

void CLogFile::Save_AlarmResetLog(CString sLog)
{
	g_csAlarmResetLog.Lock();

	CString strPath = gData.sLogPath + "\\AlarmReset";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_AlarmReset.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csAlarmResetLog.Unlock();
}

void CLogFile::Save_HandlerLog(CString sLog)
{
	g_csHandlerLog.Lock();

	CString strPath = gData.sLogPath + "\\Handler";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_Handler.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csHandlerLog.Unlock();

	Save_ECMLog(4, sLog);
}

void CLogFile::Save_SaveRunTimeLog(CString sLog)
{
	g_csRunTimeLog.Lock();

	CString strPath = gData.sLogPath + "\\RunTime";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_RunTime.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csRunTimeLog.Unlock();
}

void CLogFile::Save_InspectorLog(CString sLog)
{
	g_csInspectorLog.Lock();

	CString strPath = gData.sLogPath + "\\Inspector";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_Inspector.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d],%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csInspectorLog.Unlock();
}

void CLogFile::Save_JobListLog(CString sLog, BOOL bMode)
{
	g_csJobListLog.Lock();

	CString strPath = gData.sLogPath + "\\JobList";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString sTitle, strFile, strSave;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	sTitle.Format("Time,lotNum,Start_Time,End_Time,Run_Time,Tray_Count,CM_Count,Tack,Good_Count,NG_Count,NG1_Count,NG2_Count,NG3_Count,NG4_Count,MESNG_Count\r\n");
	strFile.Format("%s\\%04d%02d%02d_JobList.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csJobListLog.Unlock();

	if (bMode == TRUE) Save_ECMLog(2, sLog);
}

void CLogFile::Save_LotResult(int nPNo, CString sLog)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	CTime tNow = CTime::GetCurrentTime();
	CTime tLog = tNow - CTimeSpan(7 * 60 * 60);

	strPath3.Format("%s\\SPC\\%04d\\%02d\\%02d", gData.sLogPath, tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());
	Create_Folder(strPath3);

	CString strFile1, strFile2, strFile3, strTitle, strDate, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d07_Lot_Result.csv", strPath1, gData.sLotID[nPNo], tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());
	strFile2.Format("%s\\%s_%04d%02d%02d07_Lot_Result.csv", strPath2, gData.sLotID[nPNo], tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());
	strFile3.Format("%s\\%s_Lot_Result.csv", strPath3, gData.sLotID[nPNo]);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("날짜,Lot Start,Lot End,Term,호기,Lot ID,투입수,양품수,불량수,불량률,1차외관(N1),2차외관(N2),MES(M)\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strDate.Format("%02d/%02d", tNow.GetMonth(), tNow.GetDay());

		strSave.Format("%s,%s\r\n", strDate, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		//CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_MesAgentLog(CString sLog)
{
	g_csMesAgentLog.Lock();

	CString strPath = gData.sLogPath + "\\MESAgent";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_MesAgent.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csMesAgentLog.Unlock();
}

void CLogFile::Save_DispatcherLog(CString sLog)
{
	g_csDispatcherLog.Lock();

	CString strPath = gData.sLogPath + "\\Dispatcher";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_Dispatcher.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csDispatcherLog.Unlock();
}

void CLogFile::Save_CapAttachLog(CString sLog)
{
	g_csCapAttachLog.Lock();

	CString strPath = gData.sLogPath + "\\CapAttach";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_CapAttach.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csCapAttachLog.Unlock();
}

void CLogFile::Save_PCLog(int nPNo, CString sLog)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	
	Create_Folder(strPath1);
	Create_Folder(strPath2);
	
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile1, strFile2, strTitle, strTime, strPcName, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_PC.csv", strPath1, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_PC.csv", strPath2, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Time,Station,LotID,ModuleConfig,CPU,RAM,HardDisk\r\n");
	
	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		GetComputerName(szPcName, &dwNameSize);

		strSave.Format("%s,%s,%s\r\n", strTime, szPcName, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup
	
	} catch (CFileException *pEx) {
		pEx->Delete();
	}


}

void CLogFile::Save_LotTime(int nPNo, CString sLog)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	SYSTEMTIME time;
	GetLocalTime(&time);

	strPath3.Format("%s\\SPC\\%04d\\%02d\\%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath3);

	CString strFile1, strFile2, strFile3, strTitle, strTime, strPcName, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_LOT_TIME.csv", strPath1, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_LOT_TIME.csv", strPath2, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s\\%s_LOT_TIME.csv", strPath3, gData.sLotID[nPNo-1]);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;
		
	strTitle.Format("Time,Station,Machine,Version,LotID,TrayNo,CmNo,Barcode,Picker Number,AG,B1SP,T1,T2,B2,B1AG,B13D,CODE,Result\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		GetComputerName(szPcName, &dwNameSize);

		strSave.Format("%s,%s,%s\r\n", strTime, szPcName, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_LotError(CString sLog, int nPNo)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	SYSTEMTIME time;
	GetLocalTime(&time);

	strPath3.Format("%s\\SPC\\%04d\\%02d\\%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath3);

	int nNo = nPNo-1;
	if (nNo < 0) {
		nNo = gData.nULPNo-1;
		if (nNo < 0) nNo = gData.nLPNo-1;
		if (nNo < 0) nNo = 0;
	}

	CString strFile1, strFile2, strFile3, strTitle, strDateTime, strPcName, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_AVIError.csv", strPath1, gData.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_AVIError.csv", strPath2, gData.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s\\%s_AVIError.csv", strPath3, gData.sLotID[nNo]);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Time,Station,Model,Version,LotNum,Event,Error Code,event,ActionTime\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strDateTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		GetComputerName(szPcName, &dwNameSize);

		strSave.Format("%s,%s,%s\r\n", strDateTime, szPcName, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		//CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_ECMLog(int nType, CString strLog)	//nType:1[Alarm], 2[Joblist] 3[Inspect]
{
	g_csECMLog.Lock();

	CString strFile, sTitle, strTime, strSave;

	CString strPath = "D:\\EVMS\\TP\\Log\\";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	int nNo = gData.nULPNo-1;
	if (nNo < 0) nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;

	if (nType == 1) sTitle.Format("Time,Station,Type,lotNum,Error Code,Error,Start_Time,End_Time,Lead_Time\r\n");
	if (nType == 2) sTitle.Format("Time,Station,Type,lotNum,Start_Time,End_Time,Tack_Time,Tray_Count,CM_Count,Tack,Good_Count,NG_Count,N1_Count,N2_Count,N3_Count,MESNG_Count\r\n");
	if (nType == 3) sTitle.Format("Time,Station,Type,lotNum,Load_Pick,Inspect,Barcode,NG_Pick,Good_Pick,Trans_Pick\r\n");
	if (nType == 4) sTitle.Format("Time,Station,Type\r\n");
	

	if (nType == 1) strFile.Format("%s%s_%04d%02d%02d%02d_Alarm.csv", strPath, gAlm.sLotID, time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 2) strFile.Format("%s%s_%04d%02d%02d%02d_JobList.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 3) strFile.Format("%s%s_%04d%02d%02d%02d_Inspector.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 4) strFile.Format("%s%s_%04d%02d%02d%02d_Handler.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);


	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

		strSave.Format("%s,%s,%s,%s\r\n", strTime, gData.sComName, gData.sRecipe, strLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	g_csECMLog.Unlock();
}

void CLogFile::Save_DailyLot(CString sLog)
{
	g_csDailyLotLog.Lock();

	CString strPath = gData.sLogPath + "\\DailyLot";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave, strTitle;
	strFile.Format("%s\\%04d%02d%02d_DailyLot.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time \tLot_ID \tTotal \tGood \tNG \tRosRequest \tRosGood \tRosNg \tRosRepair \tRosTimeOut \tFiltering1 \tFiltering2 \tRateAvi \tFinal \tTakt\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csDailyLotLog.Unlock();
}


///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_TestLog(CString sLog)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath = gData.sLogPath + "\\TEST";

	Create_Folder(strPath);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_Test.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_LotLog(int nPNo)
{
	CString strPath, strFile, strDateTime, sTemp, sData, sInsResult, sBarResult;
	int nNo = nPNo-1;
	if (nNo < 0) nNo = 0;

	SYSTEMTIME time;
	GetLocalTime(&time);

	strPath.Format("%s\\LotData\\%04d\\%02d\\%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	if (gLot.sLotID[nNo] == "") gLot.sLotID[nNo] = "LOT_ID_DEFAULT";

	strDateTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	strFile.Format("%s\\%s_%s.txt", strPath, gLot.sLotID[nNo], strDateTime);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	sTemp.Format("LotID,%s,Start_Time,%s,End_Time,%s,Tray_Count,%02d,CM_Count,%04d,Tack,%0.7lf,\r\n\r\n", gLot.sLotID[nNo], gLot.sStartTime[nNo], gLot.sEndTime[nNo], gLot.nTrayCount[nNo], gLot.nCmCount[nNo], gLot.dTackTime);
	sData.Format("%sTray_No,Pocket_No,ID,Inspection Result,ID Result,\r\n", sTemp);
	file.Write(sData, sData.GetLength());

	try {
		int	nCmCount = 0;
		for (int i = 0; i < 30; i++) {
			if (nCmCount > gLot.nCmCount[nNo]) break;

			for (int j = 0; j < 40; j++) {
				nCmCount++;
				if (nCmCount > gLot.nCmCount[nNo]) break;

				file.SeekToEnd();

				sData.Format("%d,%d,%s,%s,%s,\r\n", i+1, j+1, gMes.sBarID[nNo][i][j], gMes.sJudge[nNo][i][j], gMes.sNGCode[nNo][i][j]);
				file.Write(sData, sData.GetLength());
			}
		}

		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_OutTray(CString strOut, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo)
{
	int nPx = nPortNo - 1;
	int nTx = nTrayNo - 1;
	int nCx = nCmNo - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	int nNo = 0;
	nNo = gData.nPNoSortPick[0];
	if (nNo == 0) nNo = gData.nPNoSortPick[1];

	CString strPath, strFile, strTitle, strSave, strJudge;
	strPath.Format("%s\\OutTray\\%04d-%02d-%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	strFile.Format("%s\\%s_%s.csv", strPath, gLot.sLotID[nNo-1], strOut);	// Good, NG-1, NG-2, NG-3, NG-4

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Count,Position,Barcode,LoadTray,LoadPos,Judge\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		int nPos = nPosY * gData.nTrayX + nPosX + 1;

		//검사결과 (0:Empty, 1:Good, 2:Normal, 4:N1, 5:N2, 6:N3, 7:BS, 8:N4)
		int nJudge = gData.nInspectInfo[nPx][nTx][nCx];
		strJudge = (nJudge == 1 ? "G" : (nJudge == 2 ? "N" : (nJudge == 4 ? "N1" : (nJudge == 5 ? "N2" : (nJudge == 6 ? "N3" : (nJudge == 7 ? "B" : (nJudge == 8 ? "N4" : " ")))))));
		strSave.Format("%d,%d,%s,%d,%d,%s\r\n", nTrayCount, nPos, gMes.sBarID[nPx][nTx][nCx], nTrayNo, nCmNo, strJudge);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
	Save_ECMOutTray(strSave, nTrayCount, nPosX, nPosY, nPortNo, nTrayNo, nCmNo);
}

void CLogFile::Save_ECMOutTray(CString sLog, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo)
{
	int nPx = nPortNo - 1;
	int nTx = nTrayNo - 1;
	int nCx = nCmNo - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave, strJudge;
	strPath = "D:\\EVMS\\TP\\Log";
	Create_Folder(strPath);

	if (gLot.sLotID[nPortNo-1] == "") gLot.sLotID[nPortNo-1] = "LOT_ID";
	strFile.Format("%s\\%s_%04d%02d%02d%02d_OutTray.csv", strPath, gLot.sLotID[nPortNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);

	int nPos = nPosY * gData.nTrayX + nPosX + 1; 

	//검사결과 (0:Empty, 1:Good, 2:Normal, 4:N1, 5:N2, 6:N3, 7:BS, 8:N4)
	int nJudge = gData.nInspectInfo[nPx][nTx][nCx];
	strJudge = (nJudge == 1 ? "G" : (nJudge == 2 ? "N" : (nJudge == 4 ? "N1" : (nJudge == 5 ? "N2" : (nJudge == 6 ? "N3" : (nJudge == 7 ? "B" : (nJudge == 8 ? "N4" : " ")))))));
	strSave.Format("%d,%d,%s,%d,%d,%s\r\n", nTrayCount, nPos, gMes.sBarID[nPx][nTx][nCx], nTrayNo, nCmNo, strJudge);
	strTitle.Format("Count,Position,Barcode,LoadTray,LoadPos,Judge\r\n");

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_CmTrackingLog(CString strOut, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo)
{
	g_csCmTrackingLog.Lock();

	int nPx = nPortNo - 1;
	int nTx = nTrayNo - 1;
	int nCx = nCmNo - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave, strJudge;
	strPath.Format("%s\\Tracking\\%04d-%02d-%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	if (gLot.sLotID[nPx] == "") gLot.sLotID[nPx] = "LOT_ID";
	strFile.Format("%s\\%s_Tracking.csv", strPath, gLot.sLotID[nPx]);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Time,Barcode,Judge,Port No,Tray No,CM No,Angle Stage,BTM1 Picker,Insp Stage,Insp Jig,BTM2 Picker,Buffer Stage,Buffer Pocket,Sort Picker,Sort Picker Jig,NG Tray,NG Y,NG X,Good Tray,Good Y,Good X\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		//검사결과 (0:Empty, 1:Good, 2:Normal, 4:N1, 5:N2, 6:N3, 7:BS, 8:N4)
		int nJudge = gData.nInspectInfo[nPx][nTx][nCx];
		strJudge = (nJudge == 1 ? "G" : (nJudge == 2 ? "N" : (nJudge == 4 ? "N1" : (nJudge == 5 ? "N2" : (nJudge == 6 ? "N3" : (nJudge == 7 ? "B" : (nJudge == 8 ? "N4" : " ")))))));
		
		int nAStageNo, nBtm1Pick, nIStageNo, nIStageJig, Btm2Pick, nBStageNo, nBStagePocket, nSortPickNo, nSortPickJig;
		nAStageNo		= gData.nCmJigNo[nPx][nTx][nCx][ANGLE_STAGE];
		nBtm1Pick		= gData.nCmJigNo[nPx][nTx][nCx][BTM1_PICK];
		nIStageNo		= gData.nCmJigNo[nPx][nTx][nCx][INSP_STAGE_NO];
		nIStageJig		= gData.nCmJigNo[nPx][nTx][nCx][INSP_JIG_NO];
		Btm2Pick		= gData.nCmJigNo[nPx][nTx][nCx][BTM2_PICK];
		nBStageNo		= gData.nCmJigNo[nPx][nTx][nCx][BUF_STAGE_NO];
		nBStagePocket	= gData.nCmJigNo[nPx][nTx][nCx][BUF_JIG_NO];
		nSortPickNo		= gData.nCmJigNo[nPx][nTx][nCx][SORT_PICK_NO];
		nSortPickJig	= gData.nCmJigNo[nPx][nTx][nCx][SORT_JIG_NO];

		if (strOut == "GOOD") {
			strSave.Format("%04d-%02d-%02d %02d:%02d:%02d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n", 
				time.wYear, time.wMonth, time.wDay,
				time.wHour, time.wMinute, time.wSecond, gMes.sBarID[nPx][nTx][nCx], strJudge, nPortNo, nTrayNo, nCmNo, 
				nAStageNo, nBtm1Pick, nIStageNo, nIStageJig, Btm2Pick, nBStageNo, nBStagePocket, nSortPickNo, nSortPickJig,
				0, 0, 0, nTrayCount, nPosY+1, nPosX+1);
		} else {
			strSave.Format("%04d-%02d-%02d %02d:%02d:%02d,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n", 
				time.wYear, time.wMonth, time.wDay,
				time.wHour, time.wMinute, time.wSecond, gMes.sBarID[nPx][nTx][nCx], strJudge, nPortNo, nTrayNo, nCmNo, 
				nAStageNo, nBtm1Pick, nIStageNo, nIStageJig, Btm2Pick, nBStageNo, nBStagePocket, nSortPickNo, nSortPickJig,
				nTrayCount+1, nPosY+1, nPosX+1, 0, 0, 0);
		}
		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
	Save_ECMTracking(strSave, nTrayCount, nPosX, nPosY, nPortNo, nTrayNo, nCmNo);

	g_csCmTrackingLog.Unlock();
}

void CLogFile::Save_ECMTracking(CString sLog, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo)
{
	int nPx = nPortNo - 1;
	int nTx = nTrayNo - 1;
	int nCx = nCmNo - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle;
	strPath = "D:\\EVMS\\TP\\Log";
	Create_Folder(strPath);

	if (gLot.sLotID[nPx] == "") gLot.sLotID[nPx] = "LOT_ID";
	strFile.Format("%s\\%s_%04d%02d%02d%02d_AVITracking.csv", strPath, gLot.sLotID[nPortNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);

	int nPos = nPosY * gData.nTrayX + nPosX + 1; 

	strTitle.Format("Time,Barcode,Judge,Port No,Tray No,CM No,Angle Stage,BTM1 Picker,Insp Stage,Insp Jig,BTM2 Picker,Buffer Stage,Buffer Pocket,Sort Picker,Sort Picker Jig,NG Tray,NG Y,NG X,Good Tray,Good Y,Good X\r\n");

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		file.Write(sLog, sLog.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_OperatingRatio(CString sLog)	// 가동률 작업 중
{
	CString strPath = gData.sLogPath + "\\OperatingRatio";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave, strTitle;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,호기,Lot ID,Lot Start,Lot End,Cycle Time,CM(EA),Run Time,Stop Time,Error Time,Error Count,수율,UPH,MTB\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
			DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
			GetComputerName(szPcName, &dwNameSize);

			strSave.Format("[%02d:%02d:%02d %03d],%s,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, szPcName, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_FocusAxisLog(int Axis, CString sLog)
{
	CString strPath = gData.sLogPath + "\\FocusAxis";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strTitle, strSave;
	if (Axis == AX_VISION_ANGLE_Z) { strFile.Format("%s\\Angle_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
	if (Axis == AX_BTM1_PICKER_Z)  { strFile.Format("%s\\Bottom1_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
	if (Axis == AX_VISION_TOP1_Z)  { strFile.Format("%s\\Top1_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
	if (Axis == AX_VISION_TOP2_Z)  { strFile.Format("%s\\Top2_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
	if (Axis == AX_BTM2_PICKER_Z)  { strFile.Format("%s\\Bottom2_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
//	strFile.Format("%s\\%04d-%02d-%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Teach,Encoder\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_RosWaitLog(int nPNo, CString sLog)
{
	g_csRosWaitLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave, strLotID;
	strPath.Format("%s\\ResultWait\\%04d-%02d-%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	strLotID = gLot.sLotID[nPNo-1];
	if (strLotID == "") strLotID = "LOT_ID";

	strFile.Format("%s\\%s_ROS.csv", strPath, strLotID);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Port_No,Tray_No,CM_No,Start_Time,Wait_Time\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csRosWaitLog.Unlock();
}

void CLogFile::Save_InspectWaitLog(int nPNo, CString sLog)
{
	g_csInspectWaitLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave, strLotID;
	strPath.Format("%s\\ResultWait\\%04d-%02d-%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	strLotID = gLot.sLotID[nPNo-1];
	if (strLotID == "") strLotID = "LOT_ID";

	strFile.Format("%s\\%s_Inspect.csv", strPath, strLotID);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Port_No,Tray_No,CM_No,Start_Time,Wait_Time\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csInspectWaitLog.Unlock();
}

void CLogFile::Save_UnloadingTime(int nSNo, int nPNo)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	int nPort = nPNo - 1;
	if (nPort < 0) nPort = 0;

	int nStage = nSNo - 1;
	if (nStage < 0) nStage = 0;

	SYSTEMTIME time;
	GetLocalTime(&time);

	strPath3.Format("%s\\SPC\\%04d\\%02d\\%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath3);

	CString strFile1, strFile2, strFile3, strTitle, strTime, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_Unloading_Time.csv", strPath1, gData.sLotID[nPort], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_Unloading_Time.csv", strPath2, gData.sLotID[nPort], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s\\%s_Unloading_Time.csv", strPath3, gData.sLotID[nPort]);

	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(szPcName, &dwNameSize);

	double dWorkingTime = 0.0, dOverlapTime = 0.0;
	if (nSNo == 1) {
		dWorkingTime= (double)(gData.dwGoodTray1UnloadingTime - gData.dwGoodTray1LoadingTime) / 1000.0;
		if (gData.dwGoodTray2LoadingTime < 1) dOverlapTime = 0;
		else								  dOverlapTime = (double)(gData.dwGoodTray1UnloadingTime - gData.dwGoodTray2LoadingTime) / 1000.0;

		strSave.Format("%s,%s,%s,%s,%s,%s,%0.3lf,%0.3lf,%d,%d\r\n", strTime, szPcName, gData.sRecipe, gData.sLotID[nPort], gData.sGoodTray1LoadTime, gData.sGoodTray1UnloadTime,
			dWorkingTime, dOverlapTime, gData.nCGoodTrayCount[nStage], nSNo);

	} else {
		dWorkingTime= (double)(gData.dwGoodTray2UnloadingTime - gData.dwGoodTray2LoadingTime) / 1000.0;
		if (gData.dwGoodTray1LoadingTime < 1) dOverlapTime = 0;
		else								  dOverlapTime = (double)(gData.dwGoodTray2UnloadingTime - gData.dwGoodTray1LoadingTime) / 1000.0;

		strSave.Format("%s,%s,%s,%s,%s,%s,%0.3lf,%0.3lf,%d,%d\r\n", strTime, szPcName, gData.sRecipe, gData.sLotID[nPort], gData.sGoodTray2LoadTime, gData.sGoodTray2UnloadTime,
			dWorkingTime, dOverlapTime, gData.nCGoodTrayCount[nStage], nSNo);
	}
	

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Time,STATION,TYPE,LOTNUM,Loading Time,Unloading Time,Work Time,Overlap Time,Tray Number,Shuttle Number\r\n");
	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		file.Write(strSave, strSave.GetLength());
		file.Close();

		//CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_MachineStopLog(CString sLog)
{
	g_csMachineStopLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave;
	strPath.Format("%s\\MachineStop", gData.sLogPath);
	Create_Folder(strPath);

	strFile.Format("%s\\%04d%02d%02d_MachineStop.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Comment,Lead_Time\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csMachineStopLog.Unlock();
}

void CLogFile::Save_PickerLog(CString sLog)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath = gData.sLogPath + "\\Picker";
	Create_Folder(strPath);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_Picker.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d.%03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_AverageCycle(int nPNo)
{
	return; // 안쓰는 로그
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strZone, strMsg, strSave, strLog;
	strPath.Format("%s\\LotJobList\\%04d-%02d-%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	strFile.Format("%s\\%s_Average.csv", strPath, gLot.sLotID[nPNo]);	

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			for (int i = 1; i <= 20; i++) {
				for (int j = 1; j <= 15; j++) {
					Get_ZoneMsg(i, j, strZone, strMsg);
					if (strMsg == "") break;

					strSave.Format("%s,%d,%s", strZone, j, strMsg);
					double dTemp = gLot.dAverageCycle[i-1][1][j-1] == 0 ? 0.0 : gLot.dAverageCycle[i-1][1][j-1] / gLot.dAverageCycle[i-1][0][j-1];
					strLog.Format("%02d:%02d:%02d %03d,%s,%0.3lf\r\n",time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strSave, dTemp);
					file.Write(strLog, strLog.GetLength());
				}
			}
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::VisionTakt_Start(int nVision)
{
	if (nVision == BTM1)		gData.dwBtm1ScanTime = GetTickCount();
	else if (nVision == TOP1)	gData.dwTop1ScanTime = GetTickCount();
	else if (nVision == TOP2)	gData.dwTop2ScanTime = GetTickCount();
	else if (nVision == BTM2)	gData.dwBtm2ScanTime = GetTickCount();
}

void CLogFile::VisionTakt_Save(int nFun, int nId, int nVision)
{
	int nFunNo = nFun + 1;	// Auto 시퀀스 넘버와 MCC 시퀀스 번호가 1차이 난다.
	CString strFun, strLog, strMsg;
	switch (nFunNo) {
	case 6:		// Btm1Picker
		strFun = "Btm1Picker";
		switch (nId) {
		case 21: strMsg = "Bottom1 Vision Stiffener Scan Complete"; break;
		case 22: strMsg = "Bottom1 Vision Connector Stiffener Scan Complete"; break;
		}
		break;
	case 7:		// Inspection1
		strFun = "Inspection1";
		switch (nId) {
		case  21: strMsg = "Top1 Module Position Inspection Complete"; break;
		case  22: strMsg = "Top1 Scan Complete"; break;
		case  23: strMsg = "Top2 FPCB Scan Complete"; break;
		case  24: strMsg = "Top2 Lens/Barrel/Sidefill Scan Complete"; break;
		}
		break;
	case 8:		// Inspection2
		strFun = "Inspection2";
		switch (nId) {
		case  21: strMsg = "Top1 Module Position Inspection Complete"; break;
		case  22: strMsg = "Top1 Scan Complete"; break;
		case  23: strMsg = "Top2 FPCB Scan Complete"; break;
		case  24: strMsg = "Top2 Lens/Barrel/Sidefill Scan Complete"; break;
		}
		break;
	case 9:		// Inspection3
		strFun = "Inspection3";
		switch (nId) {
		case  21: strMsg = "Top1 Module Position Inspection Complete"; break;
		case  22: strMsg = "Top1 Scan Complete"; break;
		case  23: strMsg = "Top2 FPCB Scan Complete"; break;
		case  24: strMsg = "Top2 Lens/Barrel/Sidefill Scan Complete"; break;
		}
		break;
	case 10:	// Btm2Picker
		strFun = "Btm2Picker";
		switch (nId) {
		case 21: strMsg = "Bottom2 Vision Scan Complete"; break;
		}
		break;
	default:
		break;
	}

	double dTime = 0.0;
	if (nVision == BTM1) dTime = (GetTickCount() - gData.dwBtm1ScanTime) / 1000.0;
	if (nVision == TOP1) dTime = (GetTickCount() - gData.dwTop1ScanTime) / 1000.0;
	if (nVision == TOP2) dTime = (GetTickCount() - gData.dwTop2ScanTime) / 1000.0;
	if (nVision == BTM2) dTime = (GetTickCount() - gData.dwBtm2ScanTime) / 1000.0;

	strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf", nFunNo, strFun, nId, strMsg, dTime);
	g_objLogFile.Save_HandlerLog(strLog);
}

///////////////////////////////////////////////////////////////////////////////
// Log 표준화 230202

void CLogFile::Save_StdMotionLog(CString sType, int nZone, int nCase, int nValue, CString sZone, CString sMsg)
{
	g_csStdMotionLog.Lock();

	CString strPath, strFile, strKey, strCycle, strTitle, strLog, strSave;

	strPath.Format("D:\\EVMS\\TP\\LOGL");
	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	// 파일명 : LotID_생성년월일시_공정명_Normal_#호기번호_PC이름_모델명_순번.csv => Lot Start시 생성
	strFile.Format("%s\\%s", strPath, gData.sStdMotionFile);

	strKey.Format("[CH_DV]%s%03d%03d[1]", sType, nZone, nCase);

	if (sType == "Y" && nValue == 1) strCycle = "";	// 선행동작 (시작시 공란)
	else strCycle.Format("%s%03d", sType, nCase-1);	// 선행동작 표시

	strLog.Format("%s,%d,%s,%s,%s", strKey, nValue, sZone, strCycle, sMsg);

	strTitle = "Time,Key,Value,Index,CycleIndex,Note\r\n";

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());
			strSave.Format("%04d%02d%02d%02d%02d%02d.%03d,%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strLog);
			file.Write(strSave, strSave.GetLength());
			file.Close();
		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csStdMotionLog.Unlock();
}

void CLogFile::Save_EfficiencyLog(int nZone, CString sStatus, int nCode, CString sNote)
{
	g_csEfficiencyLog.Lock();

	CString strPath, strFile, strZone, strMsg, strTitle, strLog, strSave;

	strPath.Format("D:\\EVMS\\TP\\LOGL");
	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	//파일명 : LotID_생성년월일시_공정명_Efficiency_#호기번호_PC이름_모델명_순번.csv
	strFile.Format("%s\\%s", strPath, gData.sEfficiencyFile);

	Get_ZoneMsg(nZone, 0, strZone, strMsg);

	strLog.Format("%s-%s-%04d,%s", sStatus, strZone, nCode, sNote);

	strTitle = "Time,Code,Comment\r\n";

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());
			strSave.Format("%04d-%02d-%02d-%02d-%02d-%02d.%03d,%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strLog);
			file.Write(strSave, strSave.GetLength());
			file.Close();
		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csEfficiencyLog.Unlock();
}



void CLogFile::Save_MCCLog(CString sLog, int nPNo)
{
	g_csMCCLog.Lock();

	CString strPath = gData.sLogPath + "\\MCC";
	CString strPath2 = "D:\\EVMS\\TP\\Log";

	Create_Folder(strPath);
	//Create_Folder(strPath2);


	SYSTEMTIME time;
	GetLocalTime(&time);

	

	CString strFile, strFile2, strSave;
	strFile.Format("%s\\%s_%04d%02d%02d%02d_MCC.txt", strPath, gData.sLotID[nPNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	//strFile2.Format("%s\\%s_%04d%02d%02d%02d_MCC.csv", strPath2, gData.sLotID[nPNo], time.wYear, time.wMonth, time.wDay, time.wHour);


	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	//CopyFile(strFile, strFile2, FALSE);

	g_csMCCLog.Unlock();

}



void CLogFile::Save_MCCLog(CString sLog)
{
	g_csMCCLog.Lock();

	CString strPath = gData.sLogPath + "\\MCC";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d_MCC.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csMCCLog.Unlock();

}

void CLogFile::Save_DoorInterlock(int nPNo, CString sLog, BOOL bfirst)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	SYSTEMTIME time;
	GetLocalTime(&time);

	strPath3.Format("%s\\SPC\\%04d\\%02d\\%02d", gData.sLogPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath3);

	CString strFile1, strFile2, strFile3, strTitle, strTime, strPcName, strSave;
	//strFile1.Format("%s\\%s_%04d%02d%02d%02d_InterlockResult.csv", strPath1, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile1.Format("%s\\GSY827ADOOR1_%04d%02d%02d%02d_InterlockResult.csv", strPath1, time.wYear, time.wMonth, time.wDay,time.wHour);
	strFile2.Format("%s\\GSY827ADOOR1_%04d%02d%02d%02d_InterlockResult.csv", strPath2, time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s\\GSY827ADOOR1_InterlockResult.csv", strPath3);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;
	

	
	strTitle.Format("Time,Station,lotNum,barcode,SWVersion,State,site,Line,Machine,operator, result, Interlock, Interlock off Time, Interlock Coverage, Door I/L 1, Door I/L 2, Door I/L 3, Door I/L 4, Door I/L 5, Door I/L 6, Door I/L 7, Door I/L 8, Door I/L 9, Door I/L 10,Door I/L 11, Door I/L 12, Door I/L 13, Door I/L 14, Door I/L 15, Door I/L 16,Door I/L 17, Door I/L 18, Door I/L 19, Door I/L 20\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		if(bfirst) strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, 0, 0, 0);
		else strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		
		char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		GetComputerName(szPcName, &dwNameSize);

		strSave.Format("%s,%s,%s\r\n", strTime, szPcName, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
	
}



///////////////////////////////////////////////////////////////////////////////
// LoopTimer 에서 위치 변경. (Log 표준화, Spc 로그 등등)

void CLogFile::Get_ZoneMsg(int nZone, int nCase, CString &sZone, CString &sMsg)
{
	switch (nZone) {
	case 1:		// LoadTray
		sZone = "LoadTray";
		switch (nCase) {
		case 1: sMsg = "Load Tray, Port check"; break;
		case 2: sMsg = "Tray X Move to Load Position"; break;
		case 3: sMsg = "Check Position, job Ready"; break;
		case 4: sMsg = "Tray Z Move to Support Up Position"; break;
		case 5: sMsg = "Port Support Out"; break;
		case 6: sMsg = "Tray Z Move to Support Down Position"; break;
		case 7: sMsg = "Port Support In"; break;
		case 8: sMsg = "Tray Z Move to Ready Down Position"; break;
		case 9: sMsg = "Tray Master In"; break;
		case 10: sMsg = "Tray Slave In"; break;
		case 11: sMsg = "Check Tray Master/Slave In"; break;
		case 12: sMsg = "Slave/Master Out"; break;
		case 13: sMsg = "Master/Slave In"; break;
		case 15: sMsg = "Tray X Move to Unload Position"; break;
		case 16: sMsg = "Tray Z Move to Unload Up Position"; break;
		case 17: sMsg = "Set Lot Start"; break;
		case 18: sMsg = "Check Lot Ready"; break;
		case 21: sMsg = "Tray Z Move to Ready Down Position"; break;
		case 22: sMsg = "Port Check"; break;
		case 23: sMsg = "Tray X Move to Load Position"; break;
		case 24: sMsg = "Check trays are all out or not"; break;
		case 30: sMsg = "tray loading checked after waiting it is loaded";break;

		}
		break;
	case 2:		// LoadPicker
		sZone = "LoadPicker";
		switch (nCase) {
		case 1: sMsg = "Z Cylinder Down"; break;
		case 2: sMsg = "Load Tray Slave Out"; break;
		case 3: sMsg = "Load Tray Master Out"; break;
		case 4: sMsg = "Picker Master In"; break;
		case 5: sMsg = "Picker Slave In"; break;
		case 6: sMsg = "Picker Z Cylinder Up"; break;
		case 7: sMsg = "check load picker is picking or not(Existence)"; break;
		case 10: sMsg = "Check Angle port Emptied"; break;
		case 11: sMsg = "Picker Y Move to Angle Port Unload Position"; break;
		case 12: sMsg = "Angle Port Check (support in/master-slave out), Picker Down"; break;
		case 13: sMsg = "Picker Slave Out"; break;
		case 14: sMsg = "Picker Master Out"; break;
		case 15: sMsg = "Info Exchange,Picker Up"; break;
		case 16: sMsg = "Y Axis Move to Load Tray Position"; break;
		case 17: sMsg = "Position Check"; break;
		
		}
		break;
	case 3:		// VisionAngle
		sZone = "VisionAngle";
		switch (nCase) {
		case 0: sMsg = "check angle port and angle inspection option"; break;
		case 1: sMsg = "Master out"; break;
		case 2: sMsg  ="Slave out"; break;
		case 3: sMsg = "check master/slave out and init angle port tray"; break;
		case 8: sMsg = " Align Z Move to Ready Up Position"; break;
		case 10: sMsg = "Angle Z move to ready up "; break;
		case 11: sMsg  ="Angle Z position check and focus z data save "; break;
		case 12: sMsg  ="Angle Y/X/R move to position for inspection(pitch move)"; break;
		case 13: sMsg  ="Set load complete[S]"; break;
		case 15: sMsg  =""; break;
		case 16: sMsg  ="Angle Z mote to focus"; break;
		case 17: sMsg  ="Send Move Complete"; break;
		case 20: sMsg  ="Angle Unit Move to Ready Position"; break;
		case 21: sMsg  ="Angle port Slave Out"; break;
		case 22: sMsg  ="Angle port Master Out"; break;
		case 23: sMsg  ="Master/Slave Check "; break;
		case 24: sMsg  ="Angle Unit Position Check"; break;
		}
		break;
	case 4:		// AngleTray1
		sZone = "AngleTray1";
		switch (nCase) {
		case  1: sMsg = "Z Axis Move to Support Up Position or Wait Pos[S]"; break;
		case  2: sMsg = "Z Axis Move to Support Up Pos from Wait pos[S]";break;
		case  3: sMsg = "Angle Port Support Out & Stage Align In"; break;
		case  4: sMsg = "Info Exchange & Z Move to Support Ready"; break;
		case  5: sMsg = "Angle Port Support In"; break;
		case  7: sMsg = "Safety check whether btm1 Picker picking in Angle tray 2 or not"; break;
		case  8: sMsg = "Y Move to Ready Position"; break;
		case  9: sMsg = "Z Move to Pick Up Position"; break;
		case 15: sMsg = "Safety Checked"; break;
		case 16: sMsg = "Y Axis Move to Btm1 Picker L1 or L2 Position"; break;
		case 20: sMsg = "Z Stage 20mm Down[S]"; break;
		case 21: sMsg = "Z Stage 20mm Down"; break;
		case 22: sMsg = "Y Axis Empty Tray Unload Position[S]"; break;
		case 23: sMsg = "Y Axis Empty Tray Unload Position"; break;
		case 24: sMsg = "Z Axis Move To Tray Pick Up Position"; break;
		case 30: sMsg = "Z Axis Move to Picker Up Position[S]"; break;
		case 31: sMsg = "Z Axis Move to Picker Up Position"; break;
		case 50: sMsg = "Z Axis Move to Ready Position"; break;
		case 51: sMsg = "Z Axis Move to Picker Up Position"; break;	
		case 52: sMsg = "Y Move to Load pos"; break;
		case 53: sMsg = "Tray Align Out[S]"; break;	
		}
		break;
	case 5:		// AngleTray2
		sZone = "AngleTray2";
		switch (nCase) {
		case  1: sMsg = "Z Axis Move to Support Up Position or Wait Pos[S]"; break;
		case  2: sMsg = "Z Axis Move to Support Up Pos from Wait pos[S]";break;
		case  3: sMsg = "Angle Port Support Out & Stage Align In"; break;
		case  4: sMsg = "Info Exchange & Z Move to Support Ready"; break;
		case  5: sMsg = "Angle Port Support In"; break;
		case  7: sMsg = "Safety check whether btm1 Picker picking in Angle tray 2 or not"; break;
		case  8: sMsg = "Y Move to Ready Position"; break;
		case  9: sMsg = "Z Move to Pick Up Position"; break;
		case 15: sMsg = "Safety Checked"; break;
		case 16: sMsg = "Y Axis Move to Btm1 Picker L1 or L2 Position"; break;
		case 20: sMsg = "Z Stage 20mm Down[S]"; break;
		case 21: sMsg = "Z Stage 20mm Down"; break;
		case 22: sMsg = "Y Axis Empty Tray Unload Position[S]"; break;
		case 23: sMsg = "Y Axis Empty Tray Unload Position"; break;
		case 24: sMsg = "Z Axis Move To Tray Pick Up Position"; break;
		case 30: sMsg = "Z Axis Move to Picker Up Position[S]"; break;
		case 31: sMsg = "Z Axis Move to Picker Up Position"; break;
		case 50: sMsg = "Z Axis Move to Ready Position"; break;
		case 51: sMsg = "Z Axis Move to Picker Up Position"; break;	
		case 52: sMsg = "Y Move to Load pos"; break;
		case 53: sMsg = "Tray Align Out[S]"; break;	
		}
		break;
	case 6:		// Btm1Picker
		sZone = "Btm1Picker";
		switch (nCase) {
		case  1: sMsg = "Angle Tray Empty Checked"; break;
		case  2: sMsg = "Angle Tray Y & Btm1 Picker X Move to Pick Position"; break;
		case  3: sMsg = "Btm1 Picker Down"; break;
		case  4: sMsg = "Btm1 Picker Grip Close, Vac on"; break;
		case  5: sMsg = "Btm1 Picker Up & Info exchange"; break;
		case  7: sMsg = "X & Z Axis Move To Btm1 Specular Inspection Position[S]"; break;
		case  9: sMsg = "Specular Vision & Set Load Complete[S]"; break;
		case 15: sMsg = "X Move to Pitch & Inspection End Check[S]"; break;
		case 29: sMsg = "Set Load Complete(Angle)[S]"; break;
		case 35: sMsg = "X Move to Pitch(Angle) or Move to 3D Start Position[S]"; break;
		case 40: sMsg = "Btm1 X Move to 3D Scan Start position"; break;
		case 41: sMsg = "Load Complete (3D)[S]"; break;
		case 43: sMsg = "Btm1 3D Picker Z Focus Move"; break;
		case 44: sMsg = "Send Move Complete"; break;
		case 45: sMsg = "Uniform Move Start and End(3D Scan)"; break;
		case 47: sMsg = "X Move to Inspection Stage & P1 & P2 & Z Ready[S]"; break;
		case 22: sMsg = "Picker Down & Inspection Stage Vac On"; break;
		case 23: sMsg = "Info Exchange & Btm1 Picker Open & Vac Off"; break;
		case 24: sMsg = "Picker Up"; break;
		case 25: sMsg = "X & P1 & P2 Move to Load Position"; break;
		case 26: sMsg = "Position Check (X & P1 & P2)[S]"; break;
		
		}
		break;
	case 7:		// Inspection1
		sZone = "Inspection1";
		switch (nCase) {
		case  1: sMsg = "X Move to Module Align or Wait(Align) Position[S]"; break;
		case  2: sMsg = "Align Z Axis Down"; break;
		case  3: sMsg = "CM Align Master Close"; break;
		case  9: sMsg = "CM Align Vac Off & Slave Close"; break;
		case  4: sMsg = "CM Align Vac On & Slave Open"; break;
		case  5: sMsg = "CM Align Master Open"; break;
		case  6: sMsg = "Align Z Axis, Ready Up Position"; break;
		case  7: sMsg = "Move to Top1 Position[S]"; break;
		case  15: sMsg = "Top1 Z & Mirror Z ready up"; break;
		case  16: sMsg = "X Pitch Move to Next Scan or Scan End[S]"; break;
		case  8: sMsg = "Send Load Complete(Top1)[S]"; break;
		case 11: sMsg = "Top1 Z Focus Move"; break;
		case 12: sMsg = "Send Move Complete[S]"; break;
		case 13: sMsg = "Top1 Mirror Z Move"; break;
		case 14: sMsg = "Send Move Complete[S]"; break;
		case 17: sMsg = "Move to Top2 Position[S]"; break;
		case 25: sMsg = "X Move to Next Pitch for Scan or Move to Unload Position[S]"; break;
		case 19: sMsg = "Send Load Complete(Top2)[S]"; break;
		case 21: sMsg = "Top2 Z Focus Move"; break;
		case 22: sMsg = "Send Move Complete(AMOVE)[S]"; break;
		case 23: sMsg = "Top2 Shift X & Y Move"; break;
		case 24: sMsg = "Send Move Complete(SMOVE)[S]"; break;
		case 26: sMsg = "Unload Position Check and Wait Start[S]"; break;
		case 31: sMsg = "Safety Check, Stage Down"; break;
		case 32: sMsg = "Stage Down Check and Wait Start[S]"; break;
		case 51: sMsg = "Move to Load Position"; break;
		case 71: sMsg = "Stage Up"; break;

		}
		break;
	case 8:		// Inspection2
		sZone = "Inspection2";
		switch (nCase) {
		case  1: sMsg = "X Move to Module Align or Wait(Align) Position[S]"; break;
		case  2: sMsg = "Align Z Axis Down"; break;
		case  3: sMsg = "CM Align Master Close"; break;
		case  9: sMsg = "CM Align Vac Off & Slave Close"; break;
		case  4: sMsg = "CM Align Vac On & Slave Open"; break;
		case  5: sMsg = "CM Align Master Open"; break;
		case  6: sMsg = "Align Z Axis, Ready Up Position"; break;
		case  7: sMsg = "Move to Top1 Position[S]"; break;
		case  15: sMsg = "Top1 Z & Mirror Z ready up"; break;
		case  16: sMsg = "X Pitch Move to Next Scan or Scan End[S]"; break;
		case  8: sMsg = "Send Load Complete(Top1)[S]"; break;
		case 11: sMsg = "Top1 Z Focus Move"; break;
		case 12: sMsg = "Send Move Complete[S]"; break;
		case 13: sMsg = "Top1 Mirror Z Move"; break;
		case 14: sMsg = "Send Move Complete[S]"; break;
		case 17: sMsg = "Move to Top2 Position[S]"; break;
		case 25: sMsg = "X Move to Next Pitch for Scan or Move to Unload Position[S]"; break;
		case 19: sMsg = "Send Load Complete(Top2)[S]"; break;
		case 21: sMsg = "Top2 Z Focus Move"; break;
		case 22: sMsg = "Send Move Complete(AMOVE)[S]"; break;
		case 23: sMsg = "Top2 Shift X & Y Move"; break;
		case 24: sMsg = "Send Move Complete(SMOVE)[S]"; break;
		case 26: sMsg = "Unload Position Check and Wait Start[S]"; break;
		case 31: sMsg = "Safety Check, Stage Down"; break;
		case 32: sMsg = "Stage Down Check and Wait Start[S]"; break;
		case 51: sMsg = "Move to Load Position"; break;
		case 71: sMsg = "Stage Up"; break;
		}
		break;
	case 9:		// Inspection3
		sZone = "Inspection3";
		switch (nCase) {
		case  1: sMsg = "X Move to Module Align or Wait(Align) Position[S]"; break;
		case  2: sMsg = "Align Z Axis Down"; break;
		case  3: sMsg = "CM Align Master Close"; break;
		case  9: sMsg = "CM Align Vac Off & Slave Close"; break;
		case  4: sMsg = "CM Align Vac On & Slave Open"; break;
		case  5: sMsg = "CM Align Master Open"; break;
		case  6: sMsg = "Align Z Axis, Ready Up Position"; break;
		case  7: sMsg = "Move to Top1 Position[S]"; break;
		case  15: sMsg = "Top1 Z & Mirror Z ready up"; break;
		case  16: sMsg = "X Pitch Move to Next Scan or Scan End[S]"; break;
		case  8: sMsg = "Send Load Complete(Top1)[S]"; break;
		case 11: sMsg = "Top1 Z Focus Move"; break;
		case 12: sMsg = "Send Move Complete[S]"; break;
		case 13: sMsg = "Top1 Mirror Z Move"; break;
		case 14: sMsg = "Send Move Complete[S]"; break;
		case 17: sMsg = "Move to Top2 Position[S]"; break;
		case 25: sMsg = "X Move to Next Pitch for Scan or Move to Unload Position[S]"; break;
		case 19: sMsg = "Send Load Complete(Top2)[S]"; break;
		case 21: sMsg = "Top2 Z Focus Move"; break;
		case 22: sMsg = "Send Move Complete(AMOVE)[S]"; break;
		case 23: sMsg = "Top2 Shift X & Y Move"; break;
		case 24: sMsg = "Send Move Complete(SMOVE)[S]"; break;
		case 26: sMsg = "Unload Position Check and Wait Start[S]"; break;
		case 31: sMsg = "Safety Check, Stage Down"; break;
		case 32: sMsg = "Stage Down Check and Wait Start[S]"; break;
		case 51: sMsg = "Move to Load Position"; break;
		case 71: sMsg = "Stage Up"; break;
		}
		break;
	case 10:	// Btm2Picker
		sZone = "Btm2Picker";
		switch (nCase) {
		case  1: sMsg = "Working Stage Number checked"; break;
		case  2: sMsg = "Picker Z & I/O Down at inspection stage"; break;
		case  3: sMsg = "Info exchange & Btm2 Picker Grip Close"; break;
		case  4: sMsg = "Picker Vaccum On & Stage Vacuum Off"; break;
		case  5: sMsg = "Picker Up"; break;
		case  6: sMsg = "Position Check, let inspect stage go"; break;
		case  8: sMsg = "Btm2 X Move to Btm2 & 3 position"; break;
		case  9: sMsg = "Set Load Complete (Btm2,3)"; break;
		case 11: sMsg = "Btm2 Z Focus move(AMOVE)"; break;
		case 12: sMsg = "Send Move Complete(AMOVE)[S]"; break;
		case 14: sMsg = "BTM2 Z & I/O Down"; break;
		case 15: sMsg = "X Move to Pitch or Inspection End Check[S]"; break;
		case 16: sMsg = "Buffer Tray Position X & Y Move"; break;
		case 20: sMsg = "Move to Buffer Position"; break;
		case 21: sMsg = "Picker Up or not"; break;
		case 22: sMsg = "Picker Down"; break;
		case 23: sMsg = "Info Exchange & Btm2 Picker Vac Off"; break;
		case 24: sMsg = "Picker Up"; break;
		case 25: sMsg = "Tray Check"; break;
		case 26: sMsg = "Picker Check & Unload or X & P1 & P2 Move to Load Position"; break;
		case 28: sMsg = "Position Check[S]"; break;

		}
		break;
	case 11:	// BufferTray1
		sZone = "BufferStage1";
		switch (nCase) {
		case 1: sMsg = "Ready check[S]"; break;
		case 10: sMsg = "Satge1 Y Move to Sort Picker 1 or 2 Position[S]"; break;
		case 11: sMsg = "Satge1 Y Move to Sort Picker 1 or 2 Position"; break;
		case 12: sMsg = "Move to Sort Unload Wait Position"; break;
		case 13: sMsg = "Satge1 Y Move to Sort Picker 1 or 2 Position"; break;
		case 20: sMsg = "Z Cylinder Down[S]"; break;
		case 21: sMsg = "Z Cylinder Down"; break;
		case 50: sMsg = "Y Axis Move to Module Loading Position[S]"; break;
		case 51: sMsg = "Y Axis Move to Module Loading Position"; break;
		case 52: sMsg = "Z Cylinder Up[S]"; break;
		case 60: sMsg = "Lot End Check"; break;
		}
		break;
	case 12:	// BufferTray2
		sZone = "BufferStage2";
		switch (nCase) {
		case 1: sMsg = "Ready check[S]"; break;
		case 10: sMsg = "Satge1 Y Move to Sort Picker 1 or 2 Position[S]"; break;
		case 11: sMsg = "Satge1 Y Move to Sort Picker 1 or 2 Position"; break;
		case 12: sMsg = "Move to Sort Unload Wait Position"; break;
		case 13: sMsg = "Satge1 Y Move to Sort Picker 1 or 2 Position"; break;
		case 20: sMsg = "Z Cylinder Down[S]"; break;
		case 21: sMsg = "Z Cylinder Down"; break;
		case 50: sMsg = "Y Axis Move to Module Loading Position[S]"; break;
		case 51: sMsg = "Y Axis Move to Module Loading Position"; break;
		case 52: sMsg = "Z Cylinder Up[S]"; break;
		case 60: sMsg = "Lot End Check"; break;
		}
		break;
	case 13:	// SortPicker1
		sZone = "SortPicker1";
		switch (nCase) {
			case  0: sMsg = "Check Working Buffer[S]"; break;
		case  1: sMsg = "Calculate Pick Position & Picker X and buffer Y move"; break;
		case  2: sMsg = "Picker Down"; break;
		case  3: sMsg = "Info Exchange & Picker Vac On"; break;
		case  4: sMsg = "Picker Up"; break;
		case  5: sMsg = "Let Buffer Tray go"; break;
		case  6: sMsg = "Check NG or Good[S]"; break;
		case  7: sMsg = "Wait or Move to Ng Position[S]"; break;
		case  8: sMsg = "Move to NG Tray Position"; break;
		case  9: sMsg = "Check Position"; break;
		case 10: sMsg = "Check Ng Tray Status/Move to Ng Tray/Move to Ng buffer[S]"; break;
		case 11: sMsg = "Check Position"; break;
		case 12: sMsg = "Move to NG Unload Position/Z ready up[S]"; break;
		case 13: sMsg = "Picker Down (NG)"; break;
		case 14: sMsg = "Info Exchange & Vac Off"; break;
		case 15: sMsg = "Picker Up (NG)"; break;
		case 16: sMsg = "Position Check"; break;
		case 17: sMsg = "Check NG or Good or Lot End[S]"; break;
		case 18: sMsg = "Move to Good Tray Position"; break;
		case 19: sMsg = "Check Position"; break;
		case 20: sMsg = "Check Good Tray Status[S]"; break;
		case 21: sMsg = "Check Position[S]"; break;
		case 22: sMsg = "Move to Good Unload Position[S]"; break;
		case 23: sMsg = "Picker Down"; break;
		case 24: sMsg = "Info Exchange, Vac Off"; break;
		case 25: sMsg = "Picker Up (Good) "; break;
		case 26: sMsg = "Picker Check[S]"; break;
		case 27: sMsg = "Position Check & Empty Check[S]"; break;
		case 28: sMsg = "Back to Buffer[S]"; break;
		case 40: sMsg = "Move to NG Buffer Position[S]"; break;
		case 41: sMsg = "NG Buffer Down"; break;
		case 42: sMsg = "Info Exchange, Vac off"; break;
		case 43: sMsg = "Picker Up(Ng buffer put)"; break;
		case 44: sMsg = "P Axis Move toTray Pitch[S]"; break;
		case 50: sMsg = "X Move to Ng Buffer"; break;
		case 51: sMsg = "Z Axis, NG Buffer Down"; break;
		case 52: sMsg = "Info Exchange & Vac Off"; break;
		case 53: sMsg = "Picker Up"; break;
		case 54: sMsg = "P Axis Move to Tray Pitch[S]"; break;
		}
		break;
	case 14:	// SortPicker2
		sZone = "SortPicker2";
		switch (nCase) {
		case  0: sMsg = "Check Working Buffer[S]"; break;
		case  1: sMsg = "Calculate Pick Position & Picker X and buffer Y move"; break;
		case  2: sMsg = "Picker Down"; break;
		case  3: sMsg = "Info Exchange & Picker Vac On"; break;
		case  4: sMsg = "Picker Up"; break;
		case  5: sMsg = "Let Buffer Tray go"; break;
		case  6: sMsg = "Check NG or Good[S]"; break;
		case  7: sMsg = "Wait or Move to Ng Position[S]"; break;
		case  8: sMsg = "Move to NG Tray Position"; break;
		case  9: sMsg = "Check Position"; break;
		case 10: sMsg = "Check Ng Tray Status/Move to Ng Tray/Move to Ng buffer[S]"; break;
		case 11: sMsg = "Check Position"; break;
		case 12: sMsg = "Move to NG Unload Position/Z ready up[S]"; break;
		case 13: sMsg = "Picker Down (NG)"; break;
		case 14: sMsg = "Info Exchange & Vac Off"; break;
		case 15: sMsg = "Picker Up (NG)"; break;
		case 16: sMsg = "Position Check"; break;
		case 17: sMsg = "Check NG or Good or Lot End[S]"; break;
		case 18: sMsg = "Move to Good Tray Position"; break;
		case 19: sMsg = "Check Position"; break;
		case 20: sMsg = "Check Good Tray Status[S]"; break;
		case 21: sMsg = "Check Position[S]"; break;
		case 22: sMsg = "Move to Good Unload Position[S]"; break;
		case 23: sMsg = "Picker Down"; break;
		case 24: sMsg = "Info Exchange, Vac Off"; break;
		case 25: sMsg = "Picker Up (Good) "; break;
		case 26: sMsg = "Picker Check[S]"; break;
		case 27: sMsg = "Position Check & Empty Check[S]"; break;
		case 28: sMsg = "Back to Buffer[S]"; break;
		case 40: sMsg = "Move to NG Buffer Position[S]"; break;
		case 41: sMsg = "NG Buffer Down"; break;
		case 42: sMsg = "Info Exchange, Vac off"; break;
		case 43: sMsg = "Picker Up(Ng buffer put)"; break;
		case 44: sMsg = "P Axis Move toTray Pitch[S]"; break;
		case 50: sMsg = "X Move to Ng Buffer"; break;
		case 51: sMsg = "Z Axis, NG Buffer Down"; break;
		case 52: sMsg = "Info Exchange & Vac Off"; break;
		case 53: sMsg = "Picker Up"; break;
		case 54: sMsg = "P Axis Move to Tray Pitch[S]"; break;
		

/*
		case  1: sMsg = "Move to Module Pick Position"; break;
		case  2: sMsg = "Module Loading (Down + VacuumOn + Up) In Buffer Tray"; break;
		case  3: sMsg = "Move to NG Unloading Position"; break;
		case  4: sMsg = "NG Unloading (Down + VacuumOff + Up) In NG Tray"; break;
		case  5: sMsg = "Z Axis Ready Up & Next Waiting"; break;
		case  6: sMsg = "Move to Good Unloading Position"; break;
		case  7: sMsg = "Good Unloading (Down + VacuumOff + Up) In Good Tray"; break;
		case  8: sMsg = "Z Axis Ready Up"; break;
		case  9: sMsg = "Move to Buffer Position"; break;
		case 10: sMsg = "Move to NG Buffer Position"; break;
		case 11: sMsg = "Z Axis Ready Up"; break;
		case 12: sMsg = "Module Loading (Down + VacuumOn + Up) In NG Buffer"; break;
		case 13: sMsg = "Sort picker move to NG unloading + good unloading"; break;*/
		}
		break;
	case 15:	// GoodTray1
		sZone = "GoodTray1";
		switch (nCase) {
		case  1: sMsg = "Good Tray1 Master/Slave In"; break;
		case  2: sMsg = "Z Axis Move to Moving Up Position"; break;
		case  3: sMsg = "Y Axis Move to Sort Picker1 Position"; break;
		case  4: sMsg = "Z Axis Move to Load Up Position"; break;
		case  5: sMsg = "Z Axis Move to Moving Down Position"; break;
		case  6: sMsg = "Y Axis Move to Unload Position"; break;
		case  7: sMsg = "Z Axis Move to Unload Up Position"; break;
		case  8: sMsg = "Good Tray1 Master/Slave Out"; break;
		case  9: sMsg = "Z Axis Move to Moving Down Position"; break;
		case 10: sMsg = "Y Axis Move to Load Position"; break;
		case 11: sMsg = "Z Axis Move to Moving Up Position"; break;
		}
		break;
	case 16:	// GoodTray2
		sZone = "GoodTray2";
		switch (nCase) {
		case  1: sMsg = "Good Tray2 Master/Slave In"; break;
		case  2: sMsg = "Z Axis Move to Moving Up Position"; break;
		case  3: sMsg = "Y Axis Move to Sort Picker1 Position"; break;
		case  4: sMsg = "Z Axis Move to Load Up Position"; break;
		case  5: sMsg = "Z Axis Move to Moving Down Position"; break;
		case  6: sMsg = "Y Axis Move to Unload Position"; break;
		case  7: sMsg = "Z Axis Move to Unload Up Position"; break;
		case  8: sMsg = "Good Tray2 Master/Slave Out"; break;
		case  9: sMsg = "Z Axis Move to Moving Down Position"; break;
		case 10: sMsg = "Y Axis Move to Load Position"; break;
		case 11: sMsg = "Z Axis Move to Moving Up Position"; break;
		}
		break;
	case 17:	// NgTray
		sZone = "NgTray";
		switch (nCase) {
		case 1: sMsg = "Y Axis Move to Picker2 Tray1 Position"; break;
		case 2: sMsg = "Y Axis Move to Ready Position"; break;
		}
		break;
	case 18:	// EmptyTrayX
		sZone = "EmptyTrayX";
		switch (nCase) {
		case 1: sMsg = "Cylinder Down"; break;
		case 2: sMsg = "Angle Stage Vacuum Off"; break;
		case 3: sMsg = "Master/Slave In"; break;
		case 4: sMsg = "Cylinder Up"; break;
		case 5: sMsg = "Move to Empty Port Position"; break;
		case 6: sMsg = "Cylinder Down"; break;
		case 7: sMsg = "Master/Slave Out"; break;
		case 8: sMsg = "Cylinder Up"; break;
		case 9: sMsg = "Move to Angle Stage Position"; break;
		}
		break;
	case 19:	// EmptyTrayElevator
		sZone = "EmptyTrayElevator";
		switch (nCase) {
		case 1: sMsg = "Slow Up"; break;
		case 2: sMsg = "Slow down"; break;
		case 3: sMsg = "Move to Base Down Position"; break;
		}
		break;
	case 20:	// EmptyTrayY
		sZone = "EmptyTrayY";
		switch (nCase) {
		case 1: sMsg = "Move to Empty Port Position"; break;
		case 2: sMsg = "Cylinder Down"; break;
		case 3: sMsg = "Master/Slave In"; break;
		case 4: sMsg = "Cylinder Up"; break;
		case 5: sMsg = "Move to Good Stage Position"; break;
		case 6: sMsg = "Cylinder Down"; break;
		case 7: sMsg = "Master/Slave Out"; break;
		case 8: sMsg = "Cylinder Up"; break;
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
