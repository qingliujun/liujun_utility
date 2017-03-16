#include "stdafx.h"
#include <Shellapi.h>
#include <TlHelp32.h>
#include "AuthorityChecker.h"

CAuthorityChecker*	CAuthorityChecker::m_pInstance = NULL;

int CAuthorityChecker::IsRunAsAdmin(DWORD dwProcessId)
{
	int iRet = 0;
	HANDLE hToken = NULL;
	HANDLE hProcess = NULL;
	TOKEN_ELEVATION tokenEle;
	DWORD dwRetLen = 0;

	if (dwProcessId != 0)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, dwProcessId);
		if (hProcess == NULL)
		{
			return 1;
		}
	}
	else
	{
		hProcess = GetCurrentProcess();
		if (hProcess == NULL)
		{
			return -1;
		}
	}

	if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
	{
		CloseHandle(hProcess);
		return -2;
	}
		
	if (GetTokenInformation(hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen))
	{
		if (dwRetLen == sizeof(tokenEle))
		{
			if (tokenEle.TokenIsElevated == false)
			{
				iRet = 0;
			}
			else
			{
				iRet = 2;
			}
		}
		else
		{
			iRet = -3;
		}
	}
	else
	{
		iRet = -4;
	}
	CloseHandle(hProcess);
	CloseHandle(hToken);
	return iRet;
}


bool CAuthorityChecker::UserRunAsAdmin(const std::tstring& strPath)
{
	std::tstring strReaPath;

	if (GetFileRealPath(strPath, strReaPath) == false)
	{
		return false;
	}
	ShellExecute(0, _T("runas"), strReaPath.c_str(), 0, 0, SW_SHOWNORMAL);

	return true;
}


bool CAuthorityChecker::AdminRunAsUser(const std::tstring& strPath)
{
	bool bRet;
	HANDLE hProcess;
	HANDLE hToken = NULL;
	HANDLE hNewToken = NULL;
	PSID pIntegritySid = NULL;
	TOKEN_MANDATORY_LABEL tml;
	SID_IDENTIFIER_AUTHORITY MLAuthority = SECURITY_MANDATORY_LABEL_AUTHORITY;

	STARTUPINFO Si = {0};
	PROCESS_INFORMATION Pi = {0};
	Si.cb = sizeof(Si);

	std::tstring strReaPath;
	if (GetFileRealPath(strPath, strReaPath) == false)
	{
		return false;
	}

	hProcess = GetCurrentProcess();
	if (hProcess == NULL)
	{
		return false;
	}

	bRet = OpenProcessToken(hProcess, TOKEN_DUPLICATE | TOKEN_QUERY | TOKEN_ADJUST_DEFAULT | TOKEN_ASSIGN_PRIMARY, &hToken);
	if (bRet == false)
	{
		CloseHandle(hProcess);
		return false;
	}

	bRet = DuplicateTokenEx(hToken, 0, NULL, SecurityImpersonation,TokenPrimary, &hNewToken);
	CloseHandle(hProcess);
	CloseHandle(hToken);
	if (bRet == false)
	{
		return false;
	}

	// 创建一个低权限的SID
	bRet = AllocateAndInitializeSid(&MLAuthority, 1, SECURITY_MANDATORY_MEDIUM_RID, 0, 0, 0, 0, 0, 0, 0, &pIntegritySid);
	if (bRet == false)
	{
		CloseHandle(hNewToken);
		return false;
	}
	tml.Label.Attributes = SE_GROUP_INTEGRITY;
	tml.Label.Sid = pIntegritySid;

	// 设置这个低权限SID到令牌
	bRet = SetTokenInformation(hNewToken, TokenIntegrityLevel, &tml, (sizeof(tml) + GetLengthSid(pIntegritySid)));
	if (bRet == false)
	{
		CloseHandle(hNewToken);
		FreeSid(pIntegritySid);
		return false;
	}

	// 创建一个低权限的进程
	bRet = CreateProcessAsUser(hNewToken, strReaPath.c_str(),
		NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT, NULL, NULL, &Si, &Pi);

	CloseHandle(hNewToken);
	FreeSid(pIntegritySid);
	return bRet;
}


bool CAuthorityChecker::RunAsSelf(const std::tstring& strPath)
{
	std::tstring strReaPath;
	if (GetFileRealPath(strPath, strReaPath) == false)
	{
		return false;
	}

	ShellExecute(0, _T("open"), strReaPath.c_str(), 0, 0, SW_SHOWNORMAL);
	return false;
}


CAuthorityChecker* CAuthorityChecker::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CAuthorityChecker();
	}
	return m_pInstance;
}


CAuthorityChecker::~CAuthorityChecker()
{
}


bool CAuthorityChecker::GetFileRealPath(const std::tstring& strSrcPath, std::tstring& strDesPath)
{
	int iFileExist = 0;
	TCHAR exeFullPath[MAX_PATH + 1] = { 0 };

	if (strSrcPath.length() == 0)
	{
		GetModuleFileName(NULL, exeFullPath, MAX_PATH);
		strDesPath = exeFullPath;
	}
	else
	{
#ifdef UNICODE
		iFileExist = _waccess(strSrcPath.c_str(), 0);
#else
		iFileExist = _access(strSrcPath.c_str(), 0)；
#endif
		strDesPath = strSrcPath;
	}
	return iFileExist != -1;
}


DWORD CAuthorityChecker::GetProcessIdFromProcessName(const std::tstring& strName)
{
	BOOL status;
	HANDLE snapshot;
	PROCESSENTRY32 processinfo;

	processinfo.dwSize = sizeof(processinfo);
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == NULL)
	{
		CloseHandle(snapshot);
		return FALSE;
	}

	status = Process32First(snapshot, &processinfo);
	while (status)
	{
		if(strName.compare(processinfo.szExeFile) ==0)
		{
			CloseHandle(snapshot);
			return processinfo.th32ProcessID;
		}
			
		status = Process32Next(snapshot, &processinfo);
	}

	CloseHandle(snapshot);
	return -1;
}


CAuthorityChecker::CAuthorityChecker()
{
}

