// OSiSP_PhoneDataBase.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
#include <vector>


//own files
#include "PhoneBookNode.h"
#include "FileLogic.h"

__declspec(dllexport) std::vector<PhoneBookNode*>* WINAPI GetPhoneBookList(LPTSTR fileName)
{
	return FileLogic::GetPhoneBookList(fileName);
}