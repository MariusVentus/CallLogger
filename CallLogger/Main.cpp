#include <windows.h>
#include <locale>
#include <codecvt>
#include "StringParser.h"


const char g_szClassName[] = "myWindowClass";
StringParser g_crafter;

#define ID_FILE_EXIT 9001
#define ID_HELP 9002
#define ID_TRAIN_AI 9003
#define ID_PARSE 9004
#define ID_UNDO 9005
#define IDC_MAIN_EDIT 101

HWND hID, hNote;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		HMENU hMenu, hSubMenu;

		hMenu = CreateMenu();
		hSubMenu = CreatePopupMenu();
		AppendMenu(hSubMenu, MF_STRING, ID_TRAIN_AI, "Learn my Notes");
		AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "Exit");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "File");

		AppendMenu(hMenu, MF_STRING, ID_HELP, "Help");

		SetMenu(hwnd, hMenu);

		CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "SR",
			WS_CHILD | WS_VISIBLE,
			15, 10, 450, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
		hID = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "SR#123456789",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
			15, 35, 450, 25, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "Raw Notes (Per Call):",
			WS_CHILD | WS_VISIBLE,
			15, 60, 450, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
		hNote = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "@X Time called Y Number",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
			15, 85, 450, 250, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

		CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Parse n Add", WS_CHILD | WS_VISIBLE,
			25, 350, 200, 50, hwnd, (HMENU)ID_PARSE, GetModuleHandle(NULL), NULL);
		CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Remove CSV Last Line", WS_CHILD | WS_VISIBLE,
			250, 350, 200, 50, hwnd, (HMENU)ID_UNDO, GetModuleHandle(NULL), NULL);

	case WM_LBUTTONDOWN:
	{
		//MessageBeep(MB_OK);
		/*char szFileName[MAX_PATH];
		HINSTANCE hInstance = GetModuleHandle(NULL);

		GetModuleFileName(hInstance, szFileName, MAX_PATH);
		MessageBox(hwnd, szFileName, "This program is:", MB_OK | MB_ICONINFORMATION);*/
	}
	break;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_TRAIN_AI:
			MessageBox(hwnd, "Apologies, at this time, note parsing is hard coded. Possible AI-related features where the logger can learn a particular note-taking style are not yet available.", "I'll get around to it eventually.", MB_OK | MB_ICONINFORMATION);
			break;
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		case ID_HELP:
			MessageBox(hwnd, "I made this in an hour while eating breakfast. There is no help, only Zuul. Or ping me on Teams.", "Halp", MB_OK | MB_ICONINFORMATION);
			break;
		case ID_UNDO:
			g_crafter.RemoveLastLine();
			break;
		case ID_PARSE:
			char srNum[100];
			char rawNote[1000];
			std::string stringSR;
			std::string stringNote;
			GetWindowText(hID, srNum, 100);
			GetWindowText(hNote, rawNote, 1000);
			stringSR = srNum;
			stringNote = rawNote;
			g_crafter.OutputToCSV(stringSR, stringNote);
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(27, 32, 45));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Call Logger v0.0.1",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}