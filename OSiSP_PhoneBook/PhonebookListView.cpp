#include "stdafx.h"
#include "PhonebookListView.h"


PhonebookListView::PhonebookListView(int X, int Y, int nWidth, int nHeight, HWND hWndParent, int listViewIdentifier, HINSTANCE hInst, RECT windowRect)
{
	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;
	if (windowWidth != 0)
	{
		this->ratioX = (double)X / windowWidth;
		this->ratioNWidth = (double)nWidth / windowWidth;
	}
	else
	{
		this->ratioX = 0;
		this->ratioNWidth = 0;
	}

	if (windowHeight != 0)
	{
		this->ratioY = (double)Y / windowHeight;
		this->ratioNHeight = (double)nHeight / windowHeight;
	}
	else
	{
		this->ratioY = 0;
		this->ratioNHeight = 0;
	}
	listViewPhonebook = new ListView(X, Y, nWidth, nHeight, hWndParent, listViewIdentifier, hInst);
	phoneDataBase = new PhoneDataBase(listViewPhonebook->GetHWND(), _T("DataBase.txt"));
	InitPhoneBookList();
	InitListLiew();
}

void PhonebookListView::ChangeSize(int newWidth, int newHeight)
{
	int x = (int)(newWidth * ratioX);
	int y = (int)(newHeight * ratioY);
	int nWidht = (int)(newWidth * ratioNWidth);
	int nHeight = (int)(newHeight * ratioNHeight);
	listViewPhonebook->ChangeSize(x, y, nWidht, nHeight);
}

void PhonebookListView::Refresh(bool isSearchRefresh)
{
	if (!isSearchRefresh)
	{
		//delete phoneBookList;
		InitPhoneBookList();
	}	
	listViewPhonebook->Clear();
	InitListLiew(false);
	InvalidateRect(listViewPhonebook->GetHWND(), NULL, TRUE);
}

void PhonebookListView::SearchAndRefresh(SEARCH_TYPE searchType, LPTSTR searchKey)
{
	//delete phoneBookList;
	switch (searchType)
	{
	case SEARCH_TYPE::SURNAME:
		phoneBookList = phoneDataBase->LoadSearchPhoneBookListUsingSurname(searchKey);
		break;
	case SEARCH_TYPE::TELEPHONE:
		phoneBookList = phoneDataBase->LoadSearchPhoneBookListUsingTelephone(searchKey);
		break;
	case SEARCH_TYPE::STREET:
		phoneBookList = phoneDataBase->LoadSearchPhoneBookListUsingStreet(searchKey);
		break;
	}
	Refresh(true);
}

PhoneBookNode * PhonebookListView::GetSelectedItem()
{
	int selectedRow = listViewPhonebook->GetSelectedRow();
	if (selectedRow != -1)
	{
		return (*phoneBookList)[selectedRow];
	}
	else
	{
		return nullptr;
	}
}

void PhonebookListView::EditSelectedRow()
{
	int selectedRow = listViewPhonebook->GetSelectedRow();
	if (selectedRow != -1)
	{
		listViewPhonebook->EditLabel(selectedRow);
	}
}

void PhonebookListView::EndEditSelectedRow()
{
	listViewPhonebook->CancelSelection();
}

void PhonebookListView::InitListLiew(bool isRefresh)
{
	if (!isRefresh)
	{
		//Inserting Columns	
		listViewPhonebook->AddColumn(_T("�������"), 0.13);
		listViewPhonebook->AddColumn(_T("�������"), 0.17);
		listViewPhonebook->AddColumn(_T("���"), 0.17);
		listViewPhonebook->AddColumn(_T("��������"), 0.17);
		listViewPhonebook->AddColumn(_T("�����"), 0.15);
		listViewPhonebook->AddColumn(_T("���"), 0.05);
		listViewPhonebook->AddColumn(_T("������"), 0.08);
		listViewPhonebook->AddColumn(_T("��������"), 0.08);
	}

	if (phoneBookList != nullptr)
	{
		for (int i = 0; i < phoneBookList->size(); ++i)
		{
			int currentRow;
			listViewPhonebook->InsertNewRowWithFirstColumn((TCHAR*)(*phoneBookList)[i]->GetPhoneNumber(), currentRow);

			listViewPhonebook->SetItem(currentRow, 1, (TCHAR*)(*phoneBookList)[i]->GetSurname());
			listViewPhonebook->SetItem(currentRow, 2, (TCHAR*)(*phoneBookList)[i]->GetName());
			listViewPhonebook->SetItem(currentRow, 3, (TCHAR*)(*phoneBookList)[i]->GetPatronymic());
			listViewPhonebook->SetItem(currentRow, 4, (TCHAR*)(*phoneBookList)[i]->GetStreet());
			listViewPhonebook->SetItem(currentRow, 5, (TCHAR*)(*phoneBookList)[i]->GetHouse());
			listViewPhonebook->SetItem(currentRow, 6, (TCHAR*)(*phoneBookList)[i]->GetBuilding());
			listViewPhonebook->SetItem(currentRow, 7, (TCHAR*)(*phoneBookList)[i]->GetApartment());
		}
	}
}

void PhonebookListView::InitPhoneBookList()
{
	phoneBookList = phoneDataBase->LoadPhoneBookList();
}

PhonebookListView::~PhonebookListView()
{
	delete listViewPhonebook;
	delete phoneDataBase;
	delete phoneBookList;
}


