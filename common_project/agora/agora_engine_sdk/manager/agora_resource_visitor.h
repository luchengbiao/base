#pragma once
#include <wtypes.h>

class AGResourceVisitor
{
public:
	AGResourceVisitor(void);
	~AGResourceVisitor(void);

	static BOOL PASCAL SaveResourceToFile(LPCTSTR lpResourceType, WORD wResourceID, LPCTSTR lpFilePath);

	// ��ΪWINDOWS��·����Ŀ¼�ָ������׼��ʽ��һ�£���ת���ᷢ������
	static LPCSTR PASCAL TransWinPathA(LPCSTR lpWinPath, LPSTR lpStandardPath, SIZE_T cchSize);
	static LPCWSTR PASCAL TransWinPathW(LPCWSTR lpWinPath, LPWSTR lpStandardPath, SIZE_T cchSize);
};