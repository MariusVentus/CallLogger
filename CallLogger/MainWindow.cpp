//#include "MainWindow.h"
//
//MainWindow::MainWindow(HINSTANCE hInst)
//{
//	m_Wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
//	m_Wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	m_Wc.hInstance = hInst;
//	m_Wc.lpszClassName = L"myWindowClass";
//	m_Wc.lpfnWndProc = WindowProcedure;
//
//	RegisterClassW(&m_Wc);
//
//	CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
//
//}
//
//void MainWindow::AddMenus(HWND hWnd)
//{
//	hMenu = CreateMenu();
//	HMENU hFileMenu = CreateMenu();
//	HMENU hSubMenu = CreateMenu();
//
//	AppendMenu(hSubMenu, MF_STRING, NULL, "Change Title");
//
//
//	AppendMenu(hFileMenu, MF_STRING, NULL, "New");
//	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Open SubMenu");
//	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
//	AppendMenu(hFileMenu, MF_STRING, NULL, "Exit");
//
//
//
//	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
//	AppendMenu(hMenu, MF_STRING, NULL, "Help");
//
//
//	SetMenu(hWnd, hMenu);
//}
//
//LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
//{
//	switch (msg) {
//	case WM_COMMAND:
//		MessageBeep(MB_OK);
//		break;
//	case WM_CREATE:
//		MainWindow::AddMenus(hWnd);
//		//AddControls(hWnd);
//		break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProcW(hWnd, msg, wp, lp);
//		break;
//	}
//}
