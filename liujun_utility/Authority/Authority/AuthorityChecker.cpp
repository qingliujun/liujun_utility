#include <comdef.h>
#include <taskschd.h>
//#include <VersionHelpers.h>
#include <Shellapi.h>
#include "AuthorityChecker.h"

# pragma comment(lib, "taskschd.lib")

#ifdef _DEBUG
#define DO( action )		\
	if( FAILED( action ) )	\
			    {			\
		assert( FALSE );	\
        return FALSE;		\
			    }
#else
#define DO( action )		\
	if( FAILED( action ) )	\
				    {			\
        return FALSE;		\
				    }
#endif

class ITaskServiceHelper
{
public:
	ITaskServiceHelper()		{ p = NULL; }
	~ITaskServiceHelper()		{ /*if (p) p->Release();*/ }

	ITaskService * p;
};

class ITaskFolderHelper
{
public:
	ITaskFolderHelper()			{ p = NULL; }
	~ITaskFolderHelper()		{ /*if (p) p->Release();*/ }

	ITaskFolder * p;
};

class ITaskDefinitionHelper
{
public:
	ITaskDefinitionHelper()		{ p = NULL; }
	~ITaskDefinitionHelper()	{ /*if (p) p->Release();*/ }

	ITaskDefinition * p;
};

class IRegistrationInfoHelper
{
public:
	IRegistrationInfoHelper()	{ p = NULL; }
	~IRegistrationInfoHelper()	{ /*if (p) p->Release();*/ }

	IRegistrationInfo * p;
};

class IPrincipalHelper
{
public:
	IPrincipalHelper()			{ p = NULL; }
	~IPrincipalHelper()			{ /*if (p) p->Release();*/ }

	IPrincipal * p;
};

class ITaskSettingsHelper
{
public:
	ITaskSettingsHelper()		{ p = NULL; }
	~ITaskSettingsHelper()		{ /*if (p) p->Release();*/ }

	ITaskSettings * p;
};

class ITriggerCollectionHelper
{
public:
	ITriggerCollectionHelper()	{ p = NULL; }
	~ITriggerCollectionHelper()	{/* if (p) p->Release();*/ }

	ITriggerCollection * p;
};

class ITriggerHelper
{
public:
	ITriggerHelper()			{ p = NULL; }
	~ITriggerHelper()			{ /*if (p) p->Release();*/ }

	ITrigger * p;
};

class IRegistrationTriggerHelper
{
public:
	IRegistrationTriggerHelper(){ p = NULL; }
	~IRegistrationTriggerHelper(){ /*if (p) p->Release();*/ }

	IRegistrationTrigger * p;
};

class IActionCollectionHelper
{
public:
	IActionCollectionHelper()	{ p = NULL; }
	~IActionCollectionHelper()	{ /*if (p) p->Release();*/ }

	IActionCollection * p;
};

class IActionHelper
{
public:
	IActionHelper()				{ p = NULL; }
	~IActionHelper()			{ /*if (p) p->Release();*/ }

	IAction * p;
};

class IExecActionHelper
{
public:
	IExecActionHelper()			{ p = NULL; }
	~IExecActionHelper()		{ /*if (p) p->Release()*/; }

	IExecAction * p;
};

class IRegisteredTaskHelper
{
public:
	IRegisteredTaskHelper()		{ p = NULL; }
	~IRegisteredTaskHelper()	{ /*if (p) p->Release();*/ }

	IRegisteredTask * p;
};


CAuthorityChecker*	CAuthorityChecker::m_pInstance = NULL;

BOOL CAuthorityChecker::IsVistaOrLater()
{
	//return IsWindowsVistaOrGreater();

	OSVERSIONINFO osver;
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (::GetVersionEx(&osver) &&
		osver.dwPlatformId == VER_PLATFORM_WIN32_NT &&
		(osver.dwMajorVersion >= 6))
		return TRUE;

	return FALSE;
}

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


BOOL CAuthorityChecker::UserRunAsAdmin(const HWND hWnd, const std::tstring& strPath, const std::tstring& strParameters, const std::tstring& strDirectory)
{
	std::tstring strReaPath;

	if (GetFileRealPath(strPath, strReaPath) == false)
	{
		return false;
	}
	return ShellExecExt(hWnd, _T("runas"), strReaPath.c_str(), strParameters.c_str(), strDirectory.c_str());
}


BOOL CAuthorityChecker::AdminRunAsUser(const HWND hWnd, const std::tstring& strPath, const std::tstring& strParameters, const std::tstring& strDirectory)
{
	HRESULT hr = 0;
	UUID guid;
	TCHAR pszTaskName[MAX_PATH ] = _T("AdminRunAsUser Task");
	ITaskServiceHelper iService;
	std::tstring strReaPath;

	if (UuidCreate(&guid) == RPC_S_OK)
	{
#ifdef UNICODE
		swprintf_s(pszTaskName, MAX_PATH - 1, _T("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}")
			, guid.Data1
			, guid.Data2 
			, guid.Data3 
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]);
#else
		sprintf_s(pszTaskName, MAX_PATH - 1, _T("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}")
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]);
#endif
	}
	

	if (GetFileRealPath(strPath, strReaPath) == false)
	{
		return false;
	}

	CoInitialize(NULL);
	DO(hr = CoCreateInstance(CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(void**)&iService.p))

	DO(iService.p->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t()))
	ITaskFolderHelper iRootFolder;
	DO(iService.p->GetFolder(_bstr_t(L"\\"), &iRootFolder.p))
	iRootFolder.p->DeleteTask(_bstr_t(pszTaskName), 0); 
	ITaskDefinitionHelper iTask;
	DO(iService.p->NewTask(0, &iTask.p))
	IRegistrationInfoHelper iRegInfo;
	DO(iTask.p->get_RegistrationInfo(&iRegInfo.p))
	DO(iRegInfo.p->put_Author(L"AdminRunAsUser"))
	IPrincipalHelper iPrincipal;
	DO(iTask.p->get_Principal(&iPrincipal.p))
	DO(iPrincipal.p->put_Id(_bstr_t(L"AdminRunAsUser_Principal")))
	DO(iPrincipal.p->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN))
	DO(iPrincipal.p->put_RunLevel(TASK_RUNLEVEL_LUA))
	ITaskSettingsHelper iSettings;
	DO(iTask.p->get_Settings(&iSettings.p))
	DO(iSettings.p->put_StartWhenAvailable(VARIANT_BOOL(true)))
	ITriggerCollectionHelper iTriggerCollection;
	DO(iTask.p->get_Triggers(&iTriggerCollection.p))
	ITriggerHelper iTrigger;
	DO(iTriggerCollection.p->Create(TASK_TRIGGER_REGISTRATION, &iTrigger.p))
	IRegistrationTriggerHelper iRegistrationTrigger;
	DO(iTrigger.p->QueryInterface(IID_IRegistrationTrigger, (void**)&iRegistrationTrigger.p))
	DO(iRegistrationTrigger.p->put_Id(_bstr_t(L"AdminRunAsUser_Trigger")))
	DO(iRegistrationTrigger.p->put_Delay(L"PT0S"))
	IActionCollectionHelper iActionCollection;
	DO(iTask.p->get_Actions(&iActionCollection.p))
	IActionHelper iAction;
	DO(iActionCollection.p->Create(TASK_ACTION_EXEC, &iAction.p))
	IExecActionHelper iExecAction;
	DO(iAction.p->QueryInterface(IID_IExecAction, (void**)&iExecAction.p))
	DO(iExecAction.p->put_Path(_bstr_t(strReaPath.c_str())))
	if (strParameters.length())
	{
		DO(iExecAction.p->put_Arguments(_bstr_t(strParameters.c_str())))
	}
	if (strDirectory.length())
	{
		DO(iExecAction.p->put_WorkingDirectory(_bstr_t(strDirectory.c_str())))
	}
	IRegisteredTaskHelper iRegisteredTask;

	DO(iRootFolder.p->RegisterTaskDefinition(_bstr_t(pszTaskName),iTask.p,TASK_CREATE_OR_UPDATE,_variant_t(),_variant_t(),TASK_LOGON_INTERACTIVE_TOKEN,	_variant_t(L""),&iRegisteredTask.p))


	if (iRootFolder.p != NULL)
	{
		iRootFolder.p->Release();
		iRootFolder.p = NULL;
	}
	if (iTask.p != NULL)
	{
		iTask.p->Release();
		iTask.p = NULL;
	}		
	if (iRegInfo.p != NULL)
	{
		iRegInfo.p->Release();
		iRegInfo.p = NULL;
	}	
	if (iPrincipal.p != NULL)
	{
		iPrincipal.p->Release();
		iPrincipal.p = NULL;
	}		
	if (iSettings.p != NULL)
	{
		iSettings.p->Release();
		iSettings.p = NULL;
	}
	if (iTriggerCollection.p != NULL)
	{
		iTriggerCollection.p->Release();
		iTriggerCollection.p = NULL;
	}
	if (iTrigger.p != NULL)
	{
		iTrigger.p->Release();
		iTrigger.p = NULL;
	}
	if (iRegistrationTrigger.p != NULL)
	{
		iRegistrationTrigger.p->Release();
		iRegistrationTrigger.p = NULL;
	}
	if (iActionCollection.p != NULL)
	{
		iActionCollection.p->Release();
		iActionCollection.p = NULL;
	}
	if (iAction.p != NULL)
	{
		iAction.p->Release();
		iAction.p = NULL;
	}
	if (iExecAction.p != NULL)
	{
		iExecAction.p->Release();
		iExecAction.p = NULL;
	}
	if (iRegisteredTask.p != NULL)
	{
		iRegisteredTask.p->Release();
		iRegisteredTask.p = NULL;
	}
	CoUninitialize();
	return TRUE;
}


BOOL CAuthorityChecker::RunAsSelf(const HWND hWnd, const std::tstring& strPath, const std::tstring& strParameters, const std::tstring& strDirectory)
{
	std::tstring strReaPath;
	if (GetFileRealPath(strPath, strReaPath) == false)
	{
		return false;
	}

	return ShellExecExt(hWnd, _T("open"), strReaPath.c_str(), strParameters.c_str(), strDirectory.c_str());
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
		iFileExist = _access(strSrcPath.c_str(), 0)£»
#endif
		strDesPath = strSrcPath;
	}
	return iFileExist != -1;
}


BOOL CAuthorityChecker::ShellExecExt(HWND hwnd,LPCTSTR pszVerb,LPCTSTR pszPath,LPCTSTR pszParameters ,LPCTSTR pszDirectory)
{
	SHELLEXECUTEINFO shex;

	memset(&shex, 0, sizeof(shex));

	shex.cbSize = sizeof(SHELLEXECUTEINFO);
	shex.fMask = 0;
	shex.hwnd = hwnd;
	shex.lpVerb = pszVerb;
	shex.lpFile = pszPath;
	shex.lpParameters = pszParameters;
	shex.lpDirectory = pszDirectory;
	shex.nShow = SW_NORMAL;

	return ::ShellExecuteEx(&shex);
}

CAuthorityChecker::CAuthorityChecker()
{
}

