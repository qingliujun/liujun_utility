#pragma once
#include <string>

#ifdef UNICODE
#define tstring wstring
#define _T(x)      L ## x
#else
#defin tstring string
#define _T(x)		x
#endif

class CAuthorityChecker
{
public:
	BOOL IsVistaOrLater();

	//功能：判断进程是否为管理员运行(缺省进程ID为获取自身的权限)
	//1、返回值大于0，为有管理员权限
	//2、返回值为0，无管理员权限
	//3、返回值小于0，检测出错
	int IsRunAsAdmin(DWORD dwProcessId = 0);

	//功能：普通用户权限的进程以管理员权限启动进程(缺省参数为，启动另一个自己)
	BOOL UserRunAsAdmin(const HWND hWnd = NULL, const std::tstring& strPath = _T(""), const std::tstring& strParameters = _T(""), const std::tstring& strDirectory = _T(""));

	//功能：管理员权限的进程以普通用户权限启动进程(缺省参数为，启动另一个自己)
	BOOL AdminRunAsUser(const HWND hWnd = NULL, const std::tstring& strPath = _T(""), const std::tstring& strParameters = _T(""), const std::tstring& strDirectory = _T(""));

	//功能: 以进程自身的权限启动一个新的进程(缺省参数为，启动另一个自己)
	BOOL RunAsSelf(const HWND hWnd, const std::tstring& strPath = _T(""), const std::tstring& strParameters = _T(""), const std::tstring& strDirectory = _T(""));

	static CAuthorityChecker* GetInstance();
	~CAuthorityChecker();
private:
	bool GetFileRealPath(const std::tstring& strSrcPath, std::tstring& strDesPath);
	BOOL ShellExecExt(HWND hwnd, LPCTSTR pszVerb, LPCTSTR pszPath, LPCTSTR pszParameters = NULL, LPCTSTR pszDirectory = NULL);

	CAuthorityChecker();
	static CAuthorityChecker*	m_pInstance;
};

