#pragma once
#include <string>

#ifdef UNICODE
#define tstring wstring
#else
#defin tstring string
#endif

class CAuthorityChecker
{
public:
	//���ܣ��жϽ����Ƿ�Ϊ����Ա����(ȱʡ����IDΪ��ȡ�����Ȩ��)
	//1������ֵ����0��Ϊ�й���ԱȨ��
	//2������ֵΪ0���޹���ԱȨ��
	//3������ֵС��0��������
	int IsRunAsAdmin(DWORD dwProcessId = 0);

	//���ܣ���ͨ�û�Ȩ�޵Ľ����Թ���ԱȨ����������(ȱʡ����Ϊ��������һ���Լ�)
	bool UserRunAsAdmin(const std::tstring& strPath = _T(""));

	//���ܣ�����ԱȨ�޵Ľ�������ͨ�û�Ȩ����������(ȱʡ����Ϊ��������һ���Լ�)
	bool AdminRunAsUser(const std::tstring& strPath = _T(""));

	//����: �Խ��������Ȩ������һ���µĽ���(ȱʡ����Ϊ��������һ���Լ�)
	bool RunAsSelf(const std::tstring& strPath = _T(""));

	static CAuthorityChecker* GetInstance();
	~CAuthorityChecker();
private:
	bool GetFileRealPath(const std::tstring& strSrcPath, std::tstring& strDesPath);
	DWORD GetProcessIdFromProcessName(const std::tstring& strName = _T(""));
	CAuthorityChecker();

	static CAuthorityChecker*	m_pInstance;
};

