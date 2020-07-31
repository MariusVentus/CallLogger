#include <windows.h>
#include <locale>
#include <codecvt>
#include "SettingsHandler.h"
#include "TimeClock.h"
#include "StringParser.h"


const char g_szClassName[] = "CallLoggerMainWindow";
const char g_MainWindowTitle[] = "Call Logger v0.1.31";
SettingsHandler g_Settings;
TimeClock g_Timeclock;
StringParser g_Crafter(g_Settings, g_Timeclock);

#define ID_FILE_EXIT 9001
#define ID_HELP 9002
#define ID_TRAIN_AI 9003
#define ID_PARSE 9004
#define ID_UNDO 9005
#define ID_ABOUT 9006
#define ID_SETTINGSMENU_GENERAL 9007
#define ID_OPENLOG 9008
#define ID_CLEARLOG 9009
#define ID_STAMPLOG 9010
#define ID_SHIFTDATEF 9011
#define ID_SHIFTDATEB 9012
#define ID_RESETDATE 9013
#define IDC_MAIN_EDIT 101

HWND hMainWindow, hLogo, hDate, hCSVDisplay, hID, hNote, hLastLine;
HBITMAP hLogoImage;
RECT g_MainWin;

void RegisterSettingsWindow(HINSTANCE hInst);
void OpenSettingsWindow(HWND hWnd);
LRESULT CALLBACK SetWinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
//MENU
		HMENU hMenu, hFileMenu, hSettingsMenu;
		hMenu = CreateMenu();
		//File Menu
		hFileMenu = CreatePopupMenu();
		AppendMenu(hFileMenu, MF_STRING, ID_OPENLOG, "Open Current Log");
		AppendMenu(hFileMenu, MF_STRING, ID_CLEARLOG, "Clear Current Log");
		AppendMenu(hFileMenu, MF_STRING, ID_STAMPLOG, "Stamp Current Log");
		AppendMenu(hFileMenu, MF_STRING, ID_TRAIN_AI, "Learn my Notes");
		AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
		AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, "Exit");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hFileMenu, "File");
		//Settings Menu
		hSettingsMenu = CreateMenu();
		AppendMenu(hSettingsMenu, MF_STRING, ID_SETTINGSMENU_GENERAL, "General");
		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSettingsMenu, "Settings");

		//Remaining Main Menu Items
		AppendMenu(hMenu, MF_STRING, ID_ABOUT, "About");
		AppendMenu(hMenu, MF_STRING, ID_HELP, "Help");

		SetMenu(hwnd, hMenu);

//MAIN SCREEN 
		//Logo and Title --------------------------------------------------
		hLogoImage = (HBITMAP)LoadImage(NULL, "Resources\\CL Banner.bmp", IMAGE_BITMAP, 480, 50, LR_LOADFROMFILE);
		if (hLogoImage == NULL) { MessageBox(hwnd, "Could not load Logo!", "Error", MB_OK | MB_ICONEXCLAMATION); }
		hLogo = CreateWindowEx(NULL, "static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 0, 0, 480, 50, hwnd, NULL, NULL, NULL);
		if (hLogo == NULL) { MessageBox(hwnd, "Logo window creation failed!", "Error", MB_OK | MB_ICONEXCLAMATION); }
		SendMessage(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);

		//Current CSV --------------------------------------------------
		hCSVDisplay = CreateWindowEx(WS_EX_CLIENTEDGE, "Static", g_Crafter.GetCSVNameNoPath().c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | SS_CENTER,
			120, 60, 240, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
		
		//Date --------------------------------------------------
		hDate = CreateWindowEx(WS_EX_CLIENTEDGE, "Static", g_Timeclock.GetDate().c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | SS_CENTER ,
			150, 95, 180, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
		
		//Date Buttons --------------------------------------------------
		CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "<Shift", WS_CHILD | WS_VISIBLE | SS_CENTER,
			150, 120, 60, 25, hwnd, (HMENU)ID_SHIFTDATEB, GetModuleHandle(NULL), NULL);
		CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Reset", WS_CHILD | WS_VISIBLE | SS_CENTER,
			210, 120, 60, 25, hwnd, (HMENU)ID_RESETDATE, GetModuleHandle(NULL), NULL);
		CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Shift>", WS_CHILD | WS_VISIBLE | SS_CENTER,
			270, 120, 60, 25, hwnd, (HMENU)ID_SHIFTDATEF, GetModuleHandle(NULL), NULL);
		
		//SR --------------------------------------------------
		CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", " SR: ", WS_CHILD | WS_VISIBLE | SS_CENTER,
			15, 155, 40, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
		hID = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", " SR#123456789", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
			55, 155, 410, 25, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

		//Notes --------------------------------------------------
		CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", " Raw Notes (Per Call):", WS_CHILD | WS_VISIBLE,
			15, 185, 450, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
		hNote = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "@X Time called Y Number",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
			15, 210, 450, 200, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		
		//Last Added Line --------------------------------------------------
		CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", " Last Added Line:", WS_CHILD | WS_VISIBLE,
			15, 410, 450, 25, hwnd, NULL, GetModuleHandle(NULL), NULL);
		hLastLine = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Date, SR, Time, Phone Number",
			WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
			15, 435, 450, 25, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		
		//Buttons --------------------------------------------------
		CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Parse n Add", WS_CHILD | WS_VISIBLE,
			25, 470, 200, 50, hwnd, (HMENU)ID_PARSE, GetModuleHandle(NULL), NULL);
		CreateWindowEx(WS_EX_CLIENTEDGE, "Button", "Remove CSV Last Line", WS_CHILD | WS_VISIBLE,
			255, 470, 200, 50, hwnd, (HMENU)ID_UNDO, GetModuleHandle(NULL), NULL);	
		
//End Creation
		break;
//COMMANDS
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_OPENLOG:
			g_Crafter.CheckCSV();
			ShellExecute(hwnd, "open", g_Crafter.GetCSVName().c_str(), NULL, NULL, SW_SHOW);
			break;
		case ID_CLEARLOG:
			if (MessageBox(hwnd, "Are you sure you want to clear the current log?\nThis will erase EVERYTHING.", "Clear Log?", MB_OKCANCEL | MB_ICONERROR) == IDOK) {
				g_Crafter.ClearCurrentLog();
			}
			break;
		case ID_STAMPLOG:
			if (MessageBox(hwnd, "Are you sure you want to stamp the current log?\nThis is normally done right before submission.", "Stamp Log?", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK) {
				g_Crafter.StampCurrentLog();
			}
			break;
		case ID_TRAIN_AI:
			MessageBox(hwnd, "Apologies, at this time, note parsing is hard coded. Possible AI-related features where the logger can learn a particular note-taking style are not yet available.", "I'll get around to it eventually.", MB_OK | MB_ICONINFORMATION);
			break;
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		case ID_SETTINGSMENU_GENERAL:
			OpenSettingsWindow(hwnd);
			break;
		case ID_ABOUT:
			MessageBox(hwnd, "This was originally made in an hour while eating breakfast. \nOver time, it has ballooned into a project made over many hours, breakfasts, some lunches, a few dinners, and even squeezed into a few breaks.\n\nIt's far from perfect, but I am happy with the progress, and it's certainly helped me learn the Windows API.\nI hope it helps!\n\n-Marius Ventus", "About", MB_OK | MB_ICONINFORMATION);
			break;
		case ID_HELP:
			MessageBox(hwnd, "There is no help, only Zuul. \nOr ping me on Teams. \nOr the instructions in the root folder. \n\nOr the Readme on Github: https://github.com/MariusVentus/CallLogger/blob/master/README.md", "Halp", MB_OK | MB_ICONINFORMATION);
			break;
		case ID_UNDO:
			g_Crafter.RemoveLastLine();
			break;
		case ID_SHIFTDATEB:
			g_Timeclock.SetTimeShiftedX(-1);
			g_Crafter.SetCSVfromDate();
			SetWindowText(hDate, g_Timeclock.GetDate().c_str());
			SetWindowText(hCSVDisplay, g_Crafter.GetCSVNameNoPath().c_str());
			break;
		case ID_SHIFTDATEF:
			g_Timeclock.SetTimeShiftedX(1);
			g_Crafter.SetCSVfromDate();
			SetWindowText(hDate, g_Timeclock.GetDate().c_str());
			SetWindowText(hCSVDisplay, g_Crafter.GetCSVNameNoPath().c_str());
			break;
		case ID_RESETDATE:
			g_Timeclock.RefreshTime();
			g_Crafter.SetCSVfromDate();
			SetWindowText(hDate, g_Timeclock.GetDate().c_str());
			SetWindowText(hCSVDisplay, g_Crafter.GetCSVNameNoPath().c_str());
			break;
		case ID_PARSE:
			char srNum[100] = "";
			char rawNote[1000] = "";
			std::string stringSR;
			std::string stringNote;
			GetWindowText(hID, srNum, 100);
			GetWindowText(hNote, rawNote, 1000);
			stringSR = srNum;
			stringNote = rawNote;
			SetWindowText(hLastLine, g_Crafter.OutputToCSV(stringSR, stringNote).c_str());
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

// -------------------------------------------------- MAIN FUNCTION ----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = { 0 };
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(27, 32, 45));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = (HICON)LoadImage(hInstance, "Resources\\CL Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	wc.hIcon = (HICON)LoadImage(hInstance, "Resources\\CL Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);


	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	RegisterSettingsWindow(hInstance);

	hMainWindow = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, g_MainWindowTitle, WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 600, NULL, NULL, hInstance, NULL);

	if (hMainWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

void RegisterSettingsWindow(HINSTANCE hInst) {
	WNDCLASSEX setWin = { 0 };

	setWin.cbSize = sizeof(WNDCLASSEX);
	setWin.style = 0;
	setWin.lpfnWndProc = SetWinProc;
	setWin.cbClsExtra = 0;
	setWin.cbWndExtra = 0;
	setWin.hInstance = hInst;
	setWin.hCursor = LoadCursor(NULL, IDC_ARROW);
	setWin.hbrBackground = (HBRUSH)COLOR_WINDOW;
	setWin.lpszMenuName = NULL;
	setWin.lpszClassName = "mySettingsWindow";
	setWin.hIconSm = (HICON)LoadImage(hInst, "Resources\\CL Logo.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);


	RegisterClassEx(&setWin);
}

void OpenSettingsWindow(HWND hWnd) {

	GetWindowRect(hMainWindow, &g_MainWin);

	HWND hSetWindow = CreateWindowEx(WS_EX_CLIENTEDGE, "mySettingsWindow", "Settings", WS_VISIBLE | WS_OVERLAPPEDWINDOW, g_MainWin.left, g_MainWin.top, 300, 600, hWnd, NULL, NULL, NULL);
	
	if (hSetWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
	}

	CreateWindowEx(NULL, "STATIC", "Weekly Auto-Splitting", WS_CHILD | WS_VISIBLE, 20, 10, 200, 25, hSetWindow, NULL, GetModuleHandle(NULL), NULL);
	CreateWindowEx(NULL, "button", "Enabled", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 20, 35, 80, 40, hSetWindow, (HMENU)10, NULL, NULL);
	CreateWindowEx(NULL, "button", "Monthly Split", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 120, 35, 120, 40, hSetWindow, (HMENU)11, NULL, NULL);

	if (g_Settings.GetAutoSplit()) {
		CheckDlgButton(hSetWindow, 10, BST_CHECKED);
	}

	if (g_Settings.GetMonthSplit()) {
		CheckDlgButton(hSetWindow, 11, BST_CHECKED);
	}

	CreateWindowEx(NULL, "STATIC", "My Workdays", WS_CHILD | WS_VISIBLE, 20, 80, 200, 25, hSetWindow, NULL, GetModuleHandle(NULL), NULL);
	CreateWindowEx(NULL, "button", "Sun", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 110, 80, 40, hSetWindow, (HMENU)3, NULL, NULL);
	CreateWindowEx(NULL, "button", "Mon", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 120, 110, 80, 40, hSetWindow, (HMENU)4, NULL, NULL);
	CreateWindowEx(NULL, "button", "Tue", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 155, 80, 40, hSetWindow, (HMENU)5, NULL, NULL);
	CreateWindowEx(NULL, "button", "Wed", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 120, 155, 80, 40, hSetWindow, (HMENU)6, NULL, NULL);
	CreateWindowEx(NULL, "button", "Thur", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 200, 80, 40, hSetWindow, (HMENU)7, NULL, NULL);
	CreateWindowEx(NULL, "button", "Fri", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 120, 200, 80, 40, hSetWindow, (HMENU)8, NULL, NULL);
	CreateWindowEx(NULL, "button", "Sat", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 245, 80, 40, hSetWindow, (HMENU)9, NULL, NULL);

	for (unsigned i = 0; i < 7; i++) {
		if (g_Settings.GetWorkday(i)) {
			CheckDlgButton(hSetWindow, i + 3, BST_CHECKED);
		}
	}


	CreateWindowEx(NULL, "STATIC", "Advanced: Column Options", WS_CHILD | WS_VISIBLE, 20, 300, 200, 25, hSetWindow, NULL, GetModuleHandle(NULL), NULL);
	CreateWindowEx(NULL, "button", "Disable Phone Numbers", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 315, 200, 40, hSetWindow, (HMENU)12, NULL, NULL);
	CreateWindowEx(NULL, "button", "Call Type", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 345, 80, 40, hSetWindow, (HMENU)13, NULL, NULL);
	CreateWindowEx(NULL, "button", "Connected", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 120, 345, 90, 40, hSetWindow, (HMENU)14, NULL, NULL);
	CreateWindowEx(NULL, "button", "Temp", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 375, 80, 40, hSetWindow, (HMENU)15, NULL, NULL);

	CreateWindowEx(NULL, "STATIC", "Advanced: Outcome Column", WS_CHILD | WS_VISIBLE, 20, 410, 200, 25, hSetWindow, NULL, GetModuleHandle(NULL), NULL);

	CreateWindowEx(NULL, "button", "Call Type", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 425, 80, 40, hSetWindow, (HMENU)16, NULL, NULL);
	CreateWindowEx(NULL, "button", "Connected", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 120, 425, 90, 40, hSetWindow, (HMENU)17, NULL, NULL);
	CreateWindowEx(NULL, "button", "Temp", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 30, 455, 80, 40, hSetWindow, (HMENU)18, NULL, NULL);

	if (g_Settings.GetPN()) {
		CheckDlgButton(hSetWindow, 12, BST_CHECKED);
	}
	if (g_Settings.GetCType()) {
		CheckDlgButton(hSetWindow, 13, BST_CHECKED);
	}
	if (g_Settings.GetCConnect()) {
		CheckDlgButton(hSetWindow, 14, BST_CHECKED);
	}
	if (g_Settings.GetCTemp()) {
		CheckDlgButton(hSetWindow, 15, BST_CHECKED);
	}
	if (g_Settings.GetOutcomeType()) {
		CheckDlgButton(hSetWindow, 16, BST_CHECKED);
	}
	if (g_Settings.GetOutcomeConnect()) {
		CheckDlgButton(hSetWindow, 17, BST_CHECKED);
	}
	if (g_Settings.GetOutcomeTemp()) {
		CheckDlgButton(hSetWindow, 18, BST_CHECKED);
	}

	CreateWindowEx(WS_EX_CLIENTEDGE, "button", "Ok", WS_VISIBLE | WS_CHILD, 25, 500, 100, 40, hSetWindow, (HMENU)IDOK, NULL, NULL);
	CreateWindowEx(WS_EX_CLIENTEDGE, "button", "Cancel", WS_VISIBLE | WS_CHILD, 150, 500, 100, 40, hSetWindow, (HMENU)IDCANCEL, NULL, NULL);

	//Disable the main window, turning a Modless dialogue box into a modal dialogue
	EnableWindow(hWnd, false);
}


LRESULT CALLBACK SetWinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		switch (wp)
		{
		case IDOK:
			g_Settings.SaveSettingsToFile();
			g_Crafter.SetCSVfromDate();
			SetWindowText(hCSVDisplay, g_Crafter.GetCSVNameNoPath().c_str());
			EnableWindow(hMainWindow, true);
			DestroyWindow(hWnd);
			break;
		case IDCANCEL:
			g_Settings.ResetSettings();
			EnableWindow(hMainWindow, true);
			DestroyWindow(hWnd);
			break;
		case 10:
			g_Settings.SetAutoSplit(!g_Settings.GetAutoSplit()); 
			break;
		case 11:
			g_Settings.SetMonthSplit(!g_Settings.GetMonthSplit());
			break;
		//Weekdays
		case 3:
			g_Settings.SetWorkday(0, !g_Settings.GetWorkday(0)); //Sun
			break;
		case 4:
			g_Settings.SetWorkday(1, !g_Settings.GetWorkday(1)); //Mon
			break;
		case 5:
			g_Settings.SetWorkday(2, !g_Settings.GetWorkday(2)); //Tue
			break;
		case 6:
			g_Settings.SetWorkday(3, !g_Settings.GetWorkday(3)); // Wed
			break;
		case 7:
			g_Settings.SetWorkday(4, !g_Settings.GetWorkday(4)); //Thur
			break;
		case 8:
			g_Settings.SetWorkday(5, !g_Settings.GetWorkday(5)); //Fri
			break;
		case 9:
			g_Settings.SetWorkday(6, !g_Settings.GetWorkday(6)); //Sat
			break;
		//Added Column Options
		case 12:
			g_Settings.SetPN(!g_Settings.GetPN());
			break;
		case 13:
			g_Settings.SetCType(!g_Settings.GetCType());
			break;
		case 14:
			g_Settings.SetCConnect(!g_Settings.GetCConnect());
			break;
		case 15:
			g_Settings.SetCTemp(!g_Settings.GetCTemp());
			break;
		case 16:
			g_Settings.SetOutcomeType(!g_Settings.GetOutcomeType());
			break;
		case 17:
			g_Settings.SetOutcomeConnect(!g_Settings.GetOutcomeConnect());
			break;
		case 18:
			g_Settings.SetOutcomeTemp(!g_Settings.GetOutcomeTemp());
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		EnableWindow(hMainWindow, true);
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}
