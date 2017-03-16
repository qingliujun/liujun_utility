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

	//���ܣ��жϽ����Ƿ�Ϊ����Ա����(ȱʡ����IDΪ��ȡ�����Ȩ��)
	//1������ֵ����0��Ϊ�й���ԱȨ��
	//2������ֵΪ0���޹���ԱȨ��
	//3������ֵС��0��������
	int IsRunAsAdmin(DWORD dwProcessId = 0);

	//���ܣ���ͨ�û�Ȩ�޵Ľ����Թ���ԱȨ����������(ȱʡ����Ϊ��������һ���Լ�)
	BOOL UserRunAsAdmin(const HWND hWnd = NULL, const std::tstring& strPath = _T(""), const std::tstring& strParameters = _T(""), const std::tstring& strDirectory = _T(""));

	//���ܣ�����ԱȨ�޵Ľ�������ͨ�û�Ȩ����������(ȱʡ����Ϊ��������һ���Լ�)
	BOOL AdminRunAsUser(const HWND hWnd = NULL, const std::tstring& strPath = _T(""), const std::tstring& strParameters = _T(""), const std::tstring& strDirectory = _T(""));

	//����: �Խ��������Ȩ������һ���µĽ���(ȱʡ����Ϊ��������һ���Լ�)
	BOOL RunAsSelf(const HWND hWnd, const std::tstring& strPath = _T(""), const std::tstring& strParameters = _T(""), const std::tstring& strDirectory = _T(""));

	static CAuthorityChecker* GetInstance();
	~CAuthorityChecker();
private:
	bool GetFileRealPath(const std::tstring& strSrcPath, std::tstring& strDesPath);
	BOOL ShellExecExt(HWND hwnd, LPCTSTR pszVerb, LPCTSTR pszPath, LPCTSTR pszParameters = NULL, LPCTSTR pszDirectory = NULL);

	CAuthorityChecker();
	static CAuthorityChecker*	m_pInstance;
};

