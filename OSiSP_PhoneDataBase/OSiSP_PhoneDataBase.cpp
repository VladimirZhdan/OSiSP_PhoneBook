// OSiSP_PhoneDataBase.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
#include <vector>


//own files
#include "PhoneBookNode.h"
#include "FileLogic.h"

std::vector<PhoneBookNode*>* WINAPI LoadPhoneBookList(LPTSTR fileName)
{
	return FileLogic::GetPhoneBookList(fileName);
}