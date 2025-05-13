// MESInterface.cpp : 구현 파일
//
#include "stdafx.h"
#include "MESInterface.h"
#include <vector>
#include "io.h"

#include "CMI8000.h"
#include "CMI8000Dlg.h"
#include "SequenceMain.h"
#include "Common.h"
#include "WorkDlg.h"
#include "LogFile.h"

#define RESULT_FOLDER	"D:\\MES\\VALIDATION\\"
#define MAINLOG_FOLDER	"D:\\MES\\LOG\\"
#define STATUS_FOLDER	"D:\\MES\\STATUS\\"
#define APD_FOLDER		"D:\\MES\\DATA\\"
#define MES_FOLDER_APD	"D:\\MES\\APD\\"
#define MES_FOLDER_RMS	"D:\\MES\\Recipe\\"
#define MES_FOLDER_APD_RESULT "D:\\EVMS\\TP\\MES\\VALIDATION\\"

#define EQUIP_TYPE		"S"	//Single:S, Dual:D"
#define APD_COUNT		100	//전송Max수량
#define APD_MAX_COUNT	500	//전송Max수량
#define TIME_OUT		40	//40*500=20000(20초)

CMESInterface	 g_objMES;

CCriticalSection g_csMesLog;
CCriticalSection g_csMesUse;
CCriticalSection g_csMesApd;

CMESInterface::CMESInterface(void)
{
	m_pThreadMES = NULL;
	m_bThreadMES = FALSE;
}

CMESInterface::~CMESInterface(void)
{
}

void CMESInterface::Initialize(BOOL bMESUse)
{
	MakeFolder(RESULT_FOLDER);
	MakeFolder(MAINLOG_FOLDER);
	MakeFolder(STATUS_FOLDER);
	MakeFolder(APD_FOLDER);
	MakeFolder(MES_FOLDER_APD);
	MakeFolder(MES_FOLDER_RMS);
	MakeFolder(MES_FOLDER_APD_RESULT);

	m_nTCount = m_nSNo = 0;
	m_nMESSequence = 0;
// 	m_bMesErr = FALSE;
	m_bMesStart = FALSE;

	Set_MESUse(bMESUse);		//On-Off Line Set
	Set_Status(3);				//Idle Set
	Clear_Result();				// Validateion 파일 삭제
}

/////////////////////////////////////////////////////////////////////////////
// Thread Function 
UINT CMESInterface::Thread_MES(LPVOID lpVoid)
{
	CString strLog;

	strLog.Format("[MESInterface] Thread_MES. Start.");
	g_objLogFile.Save_MesAgentLog(strLog);

	g_objMES.m_nReadCnt = 0;
	while (g_objMES.m_bThreadMES) {
		Sleep(500);
		g_objMES.Read_Result();

		g_objMES.m_nReadCnt++;
		if (g_objMES.m_nReadCnt > TIME_OUT) break;
		if (!g_objMES.m_bMesStart) break;
	}

	if (!g_objMES.m_bMesStart) {
		g_objMES.m_bThreadMES = FALSE; g_objMES.m_pThreadMES = NULL;
		strLog.Format("[MESInterface] Thread_MES. bMesStart = FALSE.");
		g_objLogFile.Save_MesAgentLog(strLog);
		return 0;
	}

	if (g_objMES.m_nReadCnt > TIME_OUT) {	//TimeOver
		g_objMES.m_nMESSequence = 0;
		g_objMES.m_bThreadMES = FALSE; g_objMES.m_pThreadMES = NULL;
		if (gData.bMesFirstLot) gData.bMesFirstLot = FALSE;
		strLog.Format("[MESInterface] Thread_MES. Timeout. ErrNo:991, OperLotId(%s), OperCmCnt(%d)", g_objMES.m_sOperLotID, g_objMES.m_nOperCount);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(991);
		return 0;
	}

	if (g_objMES.m_sMESResult == "1") {		//Lot취소
		g_objMES.m_nMESSequence = 0;
		g_objMES.m_bThreadMES = FALSE; g_objMES.m_pThreadMES = NULL;
		strLog.Format("[MESInterface] Thread_MES. Cancel. ErrNo:992, OperLotId(%s), OperCmCnt(%d)", g_objMES.m_sOperLotID, g_objMES.m_nOperCount);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(992);
		return 0;
	}

	if (g_objMES.m_sMESLotID != g_objMES.m_sOperLotID) {	// Lot ID 불일치
		g_dlgWork.Enable_UserInput(gData.nMesPortNo, TRUE);
		g_objMES.m_nMESSequence = 4;
		g_objMES.m_bThreadMES = FALSE; g_objMES.m_pThreadMES = NULL;
// 		g_objMES.m_bMesErr = TRUE;
		if (gData.bMesFirstLot) gData.bMesFirstLot = FALSE;
		strLog.Format("[MESInterface] Thread_MES. LotID Fail. ErrNo:993, OperLotId(%s), OperCmCnt(%d)", g_objMES.m_sOperLotID, g_objMES.m_nOperCount);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(993);
		return 0;
	}

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (g_objMES.m_nMESCount != g_objMES.m_nOperCount && !pEquipData->bUse2ndAVI) {	// Lot Count 불일치
		g_dlgWork.Enable_UserInput(gData.nMesPortNo, TRUE);
		g_objMES.m_nMESSequence = 4;
		g_objMES.m_bThreadMES = FALSE; g_objMES.m_pThreadMES = NULL;
// 		g_objMES.m_bMesErr = TRUE;
		if (gData.bMesFirstLot) gData.bMesFirstLot = FALSE;
		strLog.Format("[MESInterface] Thread_MES. CmCount Fail. ErrNo:994, OperLotId(%s), OperCmCnt(%d)", g_objMES.m_sOperLotID, g_objMES.m_nOperCount);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(994);
		return 0;
	}

	strLog.Format("[MESInterface] Thread_MES. OK.");
	g_objLogFile.Save_MesAgentLog(strLog);

	g_objMES.Set_LotStart();

	g_objMES.m_bThreadMES = FALSE;
	g_objMES.m_pThreadMES = NULL;

	g_objMES.Clear_Result();	//수신 Folder All File 삭제

	return 0;
}
/////////////////////////////////////////////////////////////////////////////

void CMESInterface::Create_Folder(CString sPath)
{
	if (sPath == _T("")) return;
	if (sPath.Right(1) == _T("\\")) sPath = sPath.Left(sPath.GetLength() - 1);
	if (GetFileAttributes(sPath) != -1) return;

	int nFound = sPath.ReverseFind('\\');
	Create_Folder(sPath.Left(nFound));

	CreateDirectory(sPath, NULL);
}

void CMESInterface::MakeFolder(CString sPath)	//"D:\\Run\\Log\\LotData\\YYYY\\MM\\DD\\";
{
	CString sTemp;
	int nIdx = 0;

	while(true) {
		nIdx = sPath.Find("\\");
		sTemp += sPath.Mid(0, nIdx) + "\\";
		CreateDirectory(sTemp,FALSE);
		sPath = sPath.Mid(nIdx+1, sPath.GetLength());
		if(nIdx<0) break;
	}
}

void CMESInterface::Clear_Result()
{
	CString strLog;
	strLog.Format("[MESInterface] Clear_Result. (LotID:%s)", m_sMESLotID);
	g_objLogFile.Save_MesAgentLog(strLog);

	m_sMESResult = m_sMESLotID = m_sReasonCode = m_sReasonText = "";
	m_nMESCount = 0;
	m_nReadCnt = 0;

	CString strPath, strFile;
	strPath.Format("%s*.*", RESULT_FOLDER);

	CFileFind finder;
	BOOL bRes = finder.FindFile(strPath);

	while (bRes) {
		bRes = finder.FindNextFile();
		if (finder.IsDirectory()) continue;
		strFile = finder.GetFilePath();
		DeleteFile(strFile);
	}
}

void CMESInterface::Read_Result()
{
	CString strPath, strFile, strData, sDataA, sResult[10], strLog;
	strPath.Format("%s*.*", RESULT_FOLDER);

	CFileFind finder;
	BOOL bRes = finder.FindFile(strPath);

	while (bRes) {
		bRes = finder.FindNextFile();
		if (finder.IsDirectory()) continue;
		strFile = finder.GetFilePath();
	}

	if (strFile.GetLength() < 4) return;

	CFile file;
	if (!file.Open(strFile, CFile::modeRead)) return;

	int nSize = (int)file.GetLength();
	char *pBuff = new char[nSize + 1];
	pBuff[nSize] = '\0';

	file.Read(pBuff, nSize);
	strData.Format("%s", pBuff);
	strData.Replace("\r\n", ",");

	file.Close();
	delete pBuff;

	char chSepA = '=', chSepB = ',';
	for (int i = 0; i < 8; i++) {
		AfxExtractSubString(sDataA, strData, i, chSepA);
		if (i > 0) AfxExtractSubString(sResult[i-1], sDataA, 0, chSepB);
	}

	m_sMESResult = sResult[1];
	m_sReasonCode = sResult[2];
//	m_sReasonText = sResult[3];
	m_sReasonText.Empty();
	m_sReasonText.Insert(0, UTF8ToANSI(sResult[3]));
	m_sReasonText.Replace("\r\n", ".");
	m_sMESLotID = sResult[4];
	m_nMESCount = atoi(sResult[6]);

	m_bThreadMES = FALSE;

	strLog.Format("[MESInterface] Read_Result. (LotID:%s, CmCnt:%d)", m_sMESLotID, m_nMESCount);
	g_objLogFile.Save_MesAgentLog(strLog);
}

void CMESInterface::Clear_APDResult()
{
	CString strLog;
	strLog.Format("[MESInterface] Clear_APDResult.");
	g_objLogFile.Save_MesAgentLog(strLog);

	CString strPath, strFile;
	strPath.Format("%s*.*", MES_FOLDER_APD_RESULT);

	CFileFind finder;
	BOOL bRes = finder.FindFile(strPath);

	while (bRes) {
		bRes = finder.FindNextFile();
		if (finder.IsDirectory()) continue;
		strFile = finder.GetFilePath();
		DeleteFile(strFile);
	}
}

BOOL CMESInterface::Read_APDResult(CString sLotID)
{
	CString strPath, strFile, strData, sDataA, sResult[47], strLog;
	CString strLotID, strItem, strTemp;
	strPath.Format("%s*.*", MES_FOLDER_APD);//strPath.Format("%s*.*", MES_FOLDER_APD);

	CFileFind finder;
	BOOL bRes = finder.FindFile(strPath);

	while (bRes) {
		bRes = finder.FindNextFile();
		if (finder.IsDirectory()) continue;
		strFile = finder.GetFilePath();
		if (strFile.GetLength() > 8 && strFile.Find(sLotID) != -1 ) break;
	}

	//파일명: LotID_YYYYMMDD.txt
	if (strFile.Find(sLotID) == -1) return FALSE;

	CFile file;
	if (!file.Open(strFile, CFile::modeRead)) return FALSE;

	int nSize = (int)file.GetLength();
	char *pBuff = new char[nSize + 1];
	pBuff[nSize] = '\0';

	file.Read(pBuff, nSize);
	strData.Format("%s", pBuff);
	strData.Replace("\r\n", ",");

	file.Close();
	delete pBuff;

	char chSepA = ',', chSepB = '=';
	strItem = "";
	gData.sAPDNgItem = "";
	gData.bAPDResultErr = FALSE;
	for (int i = 0; i < 4; i++) {
		AfxExtractSubString(sDataA, strData, i, chSepA);
		if (i == 3) AfxExtractSubString(strLotID, sDataA, 1, chSepB);
		
	}

	gData.sAPDNgItem.Format("#==> LotID:%s.", strLotID);
	strLog.Format("[MESInterface] Read_APDResult. (LotID:%s)", strLotID);
	g_objLogFile.Save_MesAgentLog(strLog);

	return TRUE;
}

char* CMESInterface::UTF8ToANSI(const char *pszCode)
{
	int nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, NULL, NULL);
	BSTR bstrWide = SysAllocStringLen(NULL, nLength);

	MultiByteToWideChar(CP_UTF8, 0, pszCode, lstrlen(pszCode) + 1, bstrWide, nLength);

	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	char *pszAnsi = new char[nLength];

	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);

	return pszAnsi;
}

void CMESInterface::Write_APD()
{
	if (m_nTCount < 1) return;
	m_nSNo++;

	CString strPathAPD, strFileAPD, strSaveAPD;
	strPathAPD.Format("%s%s%s%s", APD_FOLDER, m_sDate[0], m_sDate[1], m_sDate[2]);
	strFileAPD.Format("%s\\%s_%03d.dat", strPathAPD, m_sLotID, m_nSNo);
	Create_Folder(strPathAPD);

	CFile fileAPD;
	if (fileAPD.Open(strFileAPD, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			for(int i = 0; i < m_nTCount; i++) {
				strSaveAPD.Format("[%s],%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d\r\n", m_sTime[i], EQUIP_TYPE, m_sLotID,
					m_sBarID[i], m_sJudge[i], m_sNGCode[i], m_sNGText[i], m_nLoadTray[i], m_nLoadPocket[i], m_nOKTray[i], m_nOKPocket[i], m_nNGTray[i], m_nNGPocket[i]);
				fileAPD.Write(strSaveAPD, strSaveAPD.GetLength());
			}
			fileAPD.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

	CString strPath, strFile, strSave;
	strPath.Format("%s%s%s%s", MAINLOG_FOLDER, m_sDate[0], m_sDate[1], m_sDate[2]);
	strFile.Format("%s\\%s%s%s.txt", strPath, m_sDate[0], m_sDate[1], m_sDate[2]);
	Create_Folder(strPath);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave.Format("[%s],APD,PATH=%s\r\n",m_sTime[0], strFileAPD);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	m_nTCount = 0;
}

///////////////////////////////////////////////////////////////////////////////

void CMESInterface::Set_MESUse(BOOL bMESUse)
{
	g_csMesUse.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, strLog;
	strPath.Format("%s%04d%02d%02d", STATUS_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\ValidationSet_%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	m_bMESUse = bMESUse;
	int nStatus = (m_bMESUse ? 1 : 0);
	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],CONTROL_STATE=%d\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, nStatus);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

	strLog.Format("[MESInterface] Set_MESUse. (Use:%d)", (int)bMESUse);
	g_objLogFile.Save_MesAgentLog(strLog);
	g_csMesUse.Unlock();
}

void CMESInterface::Set_Status(int nState)	//1:Run, 2:Stop, 3:Idle
{
	if (!m_bMESUse) return;

	g_csMesLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, strLog;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CString strState = (nState == 1 ? "RUN" : (nState == 2 ? "STOP" : "IDLE"));
	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],StateChange,UNITID=1000,WorkMode=%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, strState);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

	strLog.Format("[MESInterface] Set_Status. (Status:%d)", nState);
	g_objLogFile.Save_MesAgentLog(strLog);
	g_csMesLog.Unlock();
}

void CMESInterface::Set_Alarm(int nType, int nAlarmID, CString sText )	//1:발생, 2:해제
{
	if (!m_bMESUse) return;

	g_csMesLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, strSave2, strLog;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CString strState = (nType == 1 ? "AlarmSet" : "AlarmReset");
	CString strState2 = (nType == 1 ? "STOP" : "IDLE");
	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave2.Format("[%04d/%02d/%02d %02d:%02d:%02d],%s,UNITID=1000,ALARMID=%d,ALARMTYPE=H,ALARMTEXT=%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, strState, nAlarmID, sText);
			strSave.Format("%s[%04d/%02d/%02d %02d:%02d:%02d],StateChange,UNITID=1000,WorkMode=%s\r\n",
				strSave2, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, strState2);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

	strLog.Format("[MESInterface] Set_Alarm. (Type:%d, AlarmID:%d, Text:%s)", nType, nAlarmID, sText);
	g_objLogFile.Save_MesAgentLog(strLog);
	g_csMesLog.Unlock();
}

void CMESInterface::Set_JobReady(CString sLotID, int nCount, CString sOperID, int nPortNo)
{
// 	m_nMESSequence = 0;
// 	m_bMesErr = FALSE;

	Clear_Result();

	m_sOperLotID = sLotID;
	m_nOperCount = nCount;
	m_sOperID = sOperID;
	gData.nMesPortNo = nPortNo;

// 	Set_Status(1);	// Run Set
// 
	if (!m_pThreadMES) {
		m_bThreadMES = TRUE;
		m_pThreadMES = AfxBeginThread(Thread_MES, NULL);
	}

	g_csMesLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, strLog;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CString strState = "JobReady";
	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],%s,UNITID=1000,TYPE=%s,LOTID=%s,COUNT=%d,USERID=%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, strState, EQUIP_TYPE, sLotID, nCount, sOperID);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	m_nMESSequence = 1;	// JobReady

	strLog.Format("[MESInterface] Set_JobReady. (LotID:%s, CmCnt:%d, PortNo:%d)", sLotID, nCount, nPortNo);
	g_objLogFile.Save_MesAgentLog(strLog);
	g_csMesLog.Unlock();
}

void CMESInterface::Set_LotStart()
{
	g_csMesLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, strLog;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CString strState = "LotStart";
	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],%s,UNITID=1000,TYPE=%s,LOTID=%s,COUNT=%d,USERID=%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, strState, EQUIP_TYPE, m_sMESLotID, m_nMESCount, m_sOperID);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	m_nMESSequence = 2;	// LotStart

	strLog.Format("[MESInterface] Set_LotStart. (LotID:%s, CmCnt:%d)", m_sMESLotID, m_nMESCount);
	g_objLogFile.Save_MesAgentLog(strLog);
	g_csMesLog.Unlock();
}

void CMESInterface::Set_LotCancel(CString sLotID, int nCount, CString sOperID)
{
	m_sOperLotID = sLotID;
	m_nOperCount = nCount;
	m_sOperID = sOperID;

	g_csMesLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, strLog;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CString strState = "LotCancel";
	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],%s,UNITID=1000,TYPE=%s,LOTID=%s,COUNT=%d,USERID=%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, strState, EQUIP_TYPE, sLotID, nCount, sOperID);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

	Clear_Result();	//수신 Folder All File 삭제

	strLog.Format("[MESInterface] Set_LotCancel. (LotID:%s, CmCnt:%d)", sLotID, nCount);
	g_objLogFile.Save_MesAgentLog(strLog);
	g_csMesLog.Unlock();
}

void CMESInterface::Set_LotEnd(CString sLotID, int nCount, CString sOperID, int nOKCount, int nNGCount, int nPNo)
{
 	if (!m_bMESUse) return;

	g_csMesLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	// 2019.8.1 Tack Time 이후에 APD 로그가 남아서 Tack Time 로그 위치를 바꿔주었다.
	DWORD dwTime = gLot.dwLotEnd[nPNo] - gLot.dwLotStart[nPNo];
	double dMESTack = double(dwTime/1000);
	CString sEndTime;
	sEndTime.Format("%04d/%02d/%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	Set_TackTime(dMESTack, sLotID, nCount, gLot.sMESStartTime[nPNo], sEndTime);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CString strPath, strFile, strSave;
	CString strTemp, strLog;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CString strState = "LotEnd";
	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],%s,UNITID=1000,TYPE=%s,LOTID=%s,GoodCount=%d,NGCount=%d,USERID=%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, strState, EQUIP_TYPE, sLotID, nOKCount, nNGCount, sOperID);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			// 다른 이유로 파일을 못썼을때
			switch (pEx->m_cause) {

			case CFileException::genericException:
				strTemp = _T("An unspecified error occurred.");
				break;
			case CFileException::fileNotFound:
				strTemp = _T("The file could not be located.");
				break;
			case CFileException::badPath:
				strTemp = _T("All or part of the path is invalid.");
				break;
			case CFileException::tooManyOpenFiles:
				strTemp = _T("The permitted number of open files was exceeded.");
				break;
			case CFileException::accessDenied:
				strTemp = _T("The file could not be accessed.");
				break;
			case CFileException::invalidFile:
				strTemp = _T("There was an attempt to use an invalid file handle.");
				break;
			case CFileException::removeCurrentDir:
				strTemp = _T("The current working directory cannot be removed.");
				break;
			case CFileException::directoryFull:
				strTemp = _T("There are no more directory entries.");
				break;
			case CFileException::badSeek:
				strTemp = _T("There was an error trying to set the file pointer.");
				break;
			case CFileException::hardIO:
				strTemp = _T("There was a hardware error.");
				break;
			case CFileException::sharingViolation:
				strTemp = _T("SHARE.EXE was not loaded, or a shared region was locked.");
				break;
			case CFileException::lockViolation:
				strTemp = _T("There was an attempt to lock a region that was already locked.");
				break;
			case CFileException::diskFull:
				strTemp = _T("The disk is full.");
				break;
			case CFileException::endOfFile:
				strTemp = _T("The end of file was reached.");
				break;
			default:
				strTemp = _T("EOCS - Can't Find Error Description.");
				break;
			}

			strLog.Format("[MES Debug],FileException,%s,%d", strTemp, pEx->m_cause);
			g_objLogFile.Save_MesAgentLog(strLog);
			pEx->Delete();
		}
	} else {
		// 파일을 못열었을때
		strLog.Format("[MES Debug],File Open Fail.");
		g_objLogFile.Save_MesAgentLog(strLog);
	}

	strLog.Format("[MESInterface] Set_LotEnd. (LotID:%s, CmCnt:%d)", sLotID, nCount);
	g_objLogFile.Save_MesAgentLog(strLog);
	g_csMesLog.Unlock();

	Clear_Result();	//수신 Folder All File 삭제

	Set_Status(3);	// Idle Set
}

void CMESInterface::Set_TackTime(double dTack, CString sLotID, int nCount, CString sStartTime, CString sEndTime)	//sStartTime="2017/12/04 14:00:10:
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],TackTime,UNITID=1000,TackTime=%0.3lf,LotID=%s,Count=%d,StartTime=%s,EndTime=%s\r\n",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, dTack, sLotID, nCount, sStartTime, sEndTime);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CMESInterface::Set_IDLEReasonReport(CString sEventTime, CString sOperID, CString sStartTime, CString sEndTime, CString sReasonCode)	//sStartTime="2017/12/04 14:00:10:
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave;
	strPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			strSave.Format("[%s],IDLEReasonReport,UNITID=1000,TIME=%s,USERID=%s,STARTTIME=%s,ENDTIME=%s,REASONCODE=%s\r\n",
				sEventTime, sEventTime, sOperID, sStartTime, sEndTime, sReasonCode);
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CMESInterface::Set_Result(CString sLotID, CString sBarID, CString sJudge, CString sNGCode, CString NGText, int nLTray, int nLPno, int nUTray, int nUPno,int nNGTray, int nNGPno)
{
	if (!m_bMESUse) return;
					
	SYSTEMTIME time;
	GetLocalTime(&time);

	int nBarErr = 0;
	m_sLotID = sLotID;
	if (m_nTCount == 0) {
		m_sDate[0].Format("%04d", time.wYear);
		m_sDate[1].Format("%02d", time.wMonth);
		m_sDate[2].Format("%02d", time.wDay);
	}

	m_sTime[m_nTCount].Format("%04d/%02d/%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	if (sBarID.GetLength() < 12) {
		nBarErr = 1;
		m_sBarID[m_nTCount].Format("%04d%02d%02d%02d%02d%02d%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	} else {
		m_sBarID[m_nTCount] = sBarID;
	}

	m_sJudge[m_nTCount] = sJudge;

	if (sJudge == "N1") {
		if (sNGCode.GetLength() < 1) sNGCode = "4";
		if (NGText.GetLength() < 1) NGText = "NON";
		nNGTray = gLot.nNGT+1;
		nNGPno = gLot.nNGC+1;
		nUTray = nUPno = 0;
		gLot.nNGC++;
		if (gLot.nNGC >= gData.nTrayX * gData.nTrayY) { gLot.nNGT++; gLot.nNGC = 0; }

	} else if (sJudge == "N2") {
		if (sNGCode.GetLength() < 1) sNGCode = "5";
		if (NGText.GetLength() < 1) NGText = "NON";
		nNGTray = gLot.nNGT+1;
		nNGPno = gLot.nNGC+1;
		nUTray = nUPno = 0;
		gLot.nNGC++;
		if (gLot.nNGC >= gData.nTrayX * gData.nTrayY) { gLot.nNGT++; gLot.nNGC = 0; }

	} else if (sJudge == "N3") {
		if (sNGCode.GetLength() < 1) sNGCode = "6";
		if (NGText.GetLength() < 1) NGText = "NON";
		nNGTray = gLot.nNGT+1;
		nNGPno = gLot.nNGC+1;
		nUTray = nUPno = 0;
		gLot.nNGC++;
		if (gLot.nNGC >= gData.nTrayX * gData.nTrayY) { gLot.nNGT++; gLot.nNGC = 0; }

	} else if (sJudge == "N4") {
		if (sNGCode.GetLength() < 1) sNGCode = "8";
		if (NGText.GetLength() < 1) NGText = "NON";
		nNGTray = gLot.nNGT+1;
		nNGPno = gLot.nNGC+1;
		nUTray = nUPno = 0;
		gLot.nNGC++;
		if (gLot.nNGC >= gData.nTrayX * gData.nTrayY) { gLot.nNGT++; gLot.nNGC = 0; }

	} else {
		nUTray = gLot.nGDT+1;
		nUPno = gLot.nG1DC+1;
		nNGTray = nNGPno = 0;
		gLot.nG1DC++;
		if (gLot.nG1DC >= gData.nTrayX * gData.nTrayY) { gLot.nGDT++; gLot.nG1DC = 0; }
	}

	m_sNGCode[m_nTCount] = sNGCode;
	m_sNGText[m_nTCount] = NGText;
	m_nLoadTray[m_nTCount] = nLTray;
	m_nLoadPocket[m_nTCount] = nLPno;
	m_nOKTray[m_nTCount] = nUTray;
	m_nOKPocket[m_nTCount] = nUPno;
	m_nNGTray[m_nTCount] = nNGTray;
	m_nNGPocket[m_nTCount] = nNGPno;
	m_nTCount++;

	if (m_nTCount >= APD_COUNT) Write_APD();

	if (nBarErr == 1) Sleep(5);

	CString strLog;
	strLog.Format("[MESInterface] Set_Result. (LotID:%s, BarID:%s, Judge:%s)", sLotID, sBarID, sJudge);
	g_objLogFile.Save_MesAgentLog(strLog);
}

void CMESInterface::LotEnd_WriteAPD()
{
	if (!m_bMESUse) return;

	g_csMesLog.Lock();

	Write_APD();
	m_nSNo = 0;

	g_csMesLog.Unlock();
}

void CMESInterface::Save_AviApdData(CString sLotID, int nPortNo, CString sOperID)
{
	g_csMesApd.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave, strData;
	strPath.Format("%s", MES_FOLDER_APD);
	strFile.Format("%s\\%s_%04d%02d%02d.txt", strPath, sLotID, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(szPcName, &dwNameSize);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			//[2022/12/16 11:52:39],Station=KCMAPST001,Operator=101004,LotNum=GSY847AI4N30,
			//CM1PAPSHT1=255.00,CM1PAPSHT2=255.00,CM1PAPSHT3=255.00,CM1PAPSHT4=255.00,CM1PAPSHT5=255.00,CM1PAPSHT6=255.00,CM1PAPSHT7=0.800
			strSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],Station=%s,Operator=%s,LotNum=%s",
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, szPcName, sOperID, sLotID);

			int nGvCnt = 0;
			for (int j = 0; j < 5; j++) {
				for (int i=0; i < gData.nAviApdCnt[j]; i++) {
					strData.Format(",CM1P6AVI%02d=%s", nGvCnt, gData.sVisionGV[j][i]);
					strSave += strData;
					nGvCnt++;
				}
			}
			strData.Format(",CM1P6AVI%02d=%0.3lf\r\n", nGvCnt, gData.dAssyLoadCellAvg);
			strSave += strData;

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

	CString strMesPath, strMesFile, strMesSave;
	strMesPath.Format("%s%04d%02d%02d", MAINLOG_FOLDER, time.wYear, time.wMonth, time.wDay);
	strMesFile.Format("%s\\%04d%02d%02d.txt", strMesPath, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strMesPath);

	CFile fileMes;
	if (fileMes.Open(strMesFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			fileMes.SeekToEnd();

			strMesSave.Format("[%04d/%02d/%02d %02d:%02d:%02d],APD,PATH=%s\r\n", 
				time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, strFile);
			fileMes.Write(strMesSave, strMesSave.GetLength());

			fileMes.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

	gData.bAviApdReceive[5] = TRUE;	// Lot End 가능.

	g_csMesApd.Unlock();
}

void CMESInterface::Save_AviRmsData(CString sKey, CString sValue)
{
	CString strFile = (CString)MES_FOLDER_RMS + "Current_Recipe.txt";

	CFileFind finder;
	if (!finder.FindFile(strFile)) {
		CopyFile(gData.sEnvPath + "\\Current_Recipe_Org.txt", strFile, FALSE);
	}

	CIniFileCS INI(strFile);
	if (!INI.Check_File()) { AfxMessageBox("Current_Recipe.txt File Not Found!!!"); return; }

	INI.Set_String("Current_Recipe", "Recipe_Name_Handler", gData.sRecipe);	// Model
	INI.Set_String("Current_Recipe", sKey, sValue);	// Change Data
}
