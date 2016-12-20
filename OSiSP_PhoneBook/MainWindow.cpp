#include "stdafx.h"
#include "MainWindow.h"


MainWindow::MainWindow() : Window(MainWndProc, _T("MAINWINDOW"), _T("���������� ����������"), WS_OVERLAPPEDWINDOW, 1024, 768, nullptr)
{
	hMenu = LoadMenu(WindowManager::GetHInstance(), MAKEINTRESOURCE(IDC_OSISP_PHONEBOOK));
	SetMenu(hWnd, hMenu);
	Init();
	needRefresh = false;
}


MainWindow::~MainWindow()
{
	delete(listViewPhonebook);
	delete(btnRefresh);
	delete(cBoxSearchCriterion);
	delete(editSearchString);
	delete(btnSearch);
}

void MainWindow::Show()
{
	Window::Show();
}

void MainWindow::Hide()
{
	Window::Hide();
}

void MainWindow::Init()
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
 	listViewPhonebook = new PhonebookListView(0, 50, clientRect.right, clientRect.bottom - 50, hWnd, ID_LISTVIEW_PHONEBOOK, WindowManager::GetHInstance(), clientRect);

	btnRefresh = new Button(clientRect.right - 100, clientRect.top + 10, 100, 30, hWnd, ID_BTN_REFRESH, WindowManager::GetHInstance(), _T("��������"));
	btnRefresh->SetEnabled(true);

	cBoxSearchCriterion = new ComboBox(clientRect.left + 10, clientRect.top + 10, 100, 30, hWnd, ID_COMBOBOX_SEARCH, WindowManager::GetHInstance(), 3);
	cBoxSearchCriterion->AddString(_T("�������"));
	cBoxSearchCriterion->AddString(_T("�������"));
	cBoxSearchCriterion->AddString(_T("�����"));

	editSearchString = new Edit(clientRect.left + 150, clientRect.top + 10, 100, 30, hWnd, ID_EDIT_SEARCH, WindowManager::GetHInstance());

	btnSearch = new Button(clientRect.left + 350, clientRect.top + 10, 100, 30, hWnd, ID_BTN_SEARCH, WindowManager::GetHInstance(), _T("�����"));
	btnSearch->SetEnabled(false);
}

void MainWindow::RefrechListView()
{			
	listViewPhonebook->Refresh();
}

void MainWindow::UdpateWindow()
{
	tstring emptyStr(_T(""));
	bool btnSearchEnabled = false;
	if ((cBoxSearchCriterion->GetSelectedItem() != emptyStr) && (editSearchString->GetText().length() != 0))
	{
		btnSearchEnabled = true;
	}
	btnSearch->SetEnabled(btnSearchEnabled);
}

void MainWindow::SearchPhoneBookList()
{
	tstring searchType = cBoxSearchCriterion->GetSelectedItem();
	tstring searchKey = editSearchString->GetText();
	if (searchType == tstring(_T("�������")))
	{		
		listViewPhonebook->SearchAndRefresh(SEARCH_TYPE::SURNAME, (LPTSTR)searchKey.c_str());
	}
	if (searchType == tstring(_T("�������")))
	{
		listViewPhonebook->SearchAndRefresh(SEARCH_TYPE::TELEPHONE, (LPTSTR)searchKey.c_str());
	}
	if (searchType == tstring(_T("�����")))
	{
		listViewPhonebook->SearchAndRefresh(SEARCH_TYPE::STREET, (LPTSTR)searchKey.c_str());
	}
}

void MainWindow::EditSelectedListViewRow()
{
	listViewPhonebook->EditSelectedRow();
}

void MainWindow::EndEditSelectedListViewRow()
{
	//SetNewValue
	listViewPhonebook->EndEditSelectedRow();
}

static MainWindow *mainWindow = (MainWindow*)((WindowManager::GetInstance())->GetWindow(WINDOW_TYPE::MAIN));

LRESULT CALLBACK MainWindow::MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 1024;
		lpMMI->ptMinTrackSize.y = 768;
	}
	break;		
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:		
		switch (wmId)
		{
		case ID_LISTVIEW_PHONEBOOK:
		{
			if (wmEvent == LVN_ENDLABELEDIT)
			{
				int i = 0;
			}		
		}
		break;
		case ID_BTN_REFRESH:
		{
			if (wmEvent == BN_CLICKED)
			{
				mainWindow->RefrechListView();
				SendMessage(hWnd, WM_SIZE, NULL, NULL);
			}
		}
		break;
		case ID_BTN_SEARCH:
		{
			if (wmEvent == BN_CLICKED)
			{
				mainWindow->SearchPhoneBookList();
				SendMessage(hWnd, WM_SIZE, NULL, NULL);
			}
		}
		break;
		case ID_EDIT_SEARCH:
		{
			if (wmEvent == EN_UPDATE)
			{
				mainWindow->UdpateWindow();
			}				
		}			
		break;
		case ID_COMBOBOX_SEARCH:
		{						
			if (wmEvent == CBN_SELCHANGE)
			{
				mainWindow->UdpateWindow();
			}
		}
		break;
		case IDM_ABOUT:
			DialogManager::GetInstance()->ShowDialog(DIALOG_TYPE::ABOUT, hWnd);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_SIZE:
	{
		RECT clientRect;
		GetWindowRect(hWnd, &clientRect);
		if (mainWindow != nullptr)
		{
			mainWindow->listViewPhonebook->ChangeSize(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: �������� ���� ����� ��� ����������, ������������ HDC...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_DBLCLK:
		{			
			// ������� ����� ���� �������������� ������			
		}
		break;		
		break;		
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}