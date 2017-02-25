// ConsoleApplication16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <direct.h>
#include <memory>
#include "targetver.h"
#include <iostream>
#include <map>
#include <thread>
#include <string>
#include <psapi.h>
#include <stdio.h>
#include <tchar.h>
STARTUPINFO startup;
PROCESS_INFORMATION procinfo;
namespace std {
#if defined _UNICODE || defined UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif
}
HWND TL_exe = 0;
HWND ME_exe = 0;
struct wnd {
	HWND hwd;
};
wnd TLexe;
bool HelloTl(0);
bool SlsTl(0);
bool gamestr(0);
std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}
LRESULT CALLBACK pWndProc(HWND hwnd,            // Handle to our main window

	UINT Msg,             // Our message that needs to be processed

	WPARAM wParam,        // Extra values of message (Mouse for example might have x and y coord values)

	LPARAM lParam)        // Extra values of message (Mouse for example might have x and y coord values)

{
	using namespace std;
	cout << "##########################################################################" << endl;
	COPYDATASTRUCT* lpMsg = (COPYDATASTRUCT*)lParam;
	cout << "message : " << std::hex <<  Msg << endl;
	char * txt;
		HWND senderHWND = (HWND)wParam;
		//Get sender infos  : 
		int length = ::GetWindowTextLength(senderHWND);
		if (0 != length)
		{
			TCHAR* buffer;
			buffer = new TCHAR[length + 1];
			memset(buffer, 0, (length + 1) * sizeof(TCHAR));
			GetWindowText(senderHWND, buffer, length + 1);
			tstring windowTitle = tstring(buffer);
			delete[] buffer;
			cout << "window name : " << windowTitle << " \nhandle : " << senderHWND << endl;
		}
		else
		{
			cout << "sender window has no title." << endl;
		}
			HWND   h;
			char processName[255];
			DWORD PID;
			char className[255];
			BOOL find = FALSE;
			GetClassName(senderHWND, className, 254);
			cout << "class name : " << className << endl;
			GetWindowThreadProcessId(senderHWND, &PID);
			HANDLE Hhwnd = OpenProcess(PROCESS_ALL_ACCESS, 1, PID);
			if (NULL == Hhwnd)
			{
				cout << "couldn't open process" << endl;
			}
			GetModuleFileNameEx((HMODULE)Hhwnd, NULL, processName, 255);
			cout << processName << endl;
		//
		switch (Msg)

		{

	case WM_COPYDATA:
		cout << "COPY DATA message" << endl;
		cout << "size : " << lpMsg->cbData << endl;
		txt = new char[lpMsg->cbData];
		memcpy(txt, lpMsg->lpData, lpMsg->cbData);
		cout << "Message received : " << txt << endl;
		if (txt[0] == 'H')
		{
			TL_exe = senderHWND;
			ME_exe = hwnd;
			txt[lpMsg->cbData] = 0x00;
			string str(txt);
			if (str == "Hello!!")
			{
				HelloTl = 1;
				return 1;
			}
		}
		else if (txt[0] == 's')
		{
			txt[lpMsg->cbData] = 0x00;
			string str(txt);
			if (str == "slsurl")
			{
				SlsTl = 1;
				return 1;
			}
		}
		else if (txt[0] == 'g')
		{
			txt[lpMsg->cbData] = 0x00;
			string gamstr(txt);
			if (gamstr == "gamestr")
			{
				gamestr = 1;
			}
			return 1;
		}
		return TRUE;
		break;

	default:                                        // Handle all other messages in a normal way
		if(TL_exe)
		if (TL_exe == senderHWND)
			cout << "TL.exe a envoyé un message non pris en charge !!" << endl;
		return DefWindowProc(hwnd, Msg, wParam, lParam);

	}
		return TRUE;

}
void sendemall()
{
	using namespace std;
	while (1)
	{
		if (HelloTl)
		{
			cout << "BOOL HELLO IS UP" << endl;
			Sleep(800);
			//we say hello!!
			COPYDATASTRUCT Hello;
			Hello.cbData = 8;
			Hello.dwData = 0x0dbadb0a;
			Hello.lpData = "Hello!!";
			if (0 == SendMessage((HWND)TL_exe, WM_COPYDATA, (WPARAM)ME_exe, (LPARAM)&Hello))
				cout << "bad message" << endl;
			else
				cout << "good message" << endl;
			HelloTl = 0;
		}
		if (SlsTl)
		{
			Sleep(800);
			COPYDATASTRUCT Hello;
			Hello.cbData = 0x34;
			Hello.dwData = 0x2;
			string mymsg = "http://sls.service.enmasse.com:8080/servers/list.en";
			Hello.lpData = "http://sls.service.enmasse.com:8080/servers/list.en";
			cout << "we send : " << mymsg << endl;
			if (0 == SendMessage((HWND)TL_exe, WM_COPYDATA, (WPARAM)ME_exe, (LPARAM)&Hello))
				cout << "bad message" << endl;
			else
				cout << "good message" << endl;
			if (0 == SendMessage((HWND)TL_exe, WM_COPYDATA, (WPARAM)ME_exe, (LPARAM)&Hello))
				cout << "bad message" << endl;
			else
				cout << "good message" << endl;
			SlsTl = 0;
		}
		if (gamestr)
		{
			Sleep(1800);
			COPYDATASTRUCT Hello;
			Hello.cbData = 0x11d;
			Hello.dwData = 0x3;
			Hello.lpData = "{\"last_connected_server_id\":4024,\"chars_per_server\":{},\"account_bits\":\"0x00000000\",\"ticket\":\"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\",\"result-message\":\"OK\",\"result-code\":200,\"user_permission\":0,\"game_account_name\":\"TERA\",\"access_level\":0,\"master_account_name\":\"XXXXXXXXXX\"}";
			if (0 == SendMessage((HWND)TL_exe, WM_COPYDATA, (WPARAM)ME_exe, (LPARAM)&Hello))
				cout << "bad message" << endl;
			else
				cout << "good message" << endl;
			gamestr = 0;
		}
		Sleep(800);
	}
}
int main(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hwind2 = 0;
	hInstance = GetModuleHandle(NULL);
	using namespace std;
	WNDCLASS WndClass = {};
	WndClass.style = CS_HREDRAW | CS_VREDRAW; // == 0x03
	WndClass.lpfnWndProc = pWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hIcon = 0;
	WndClass.hCursor = 0;
	WndClass.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = "EME.LauncherWnd";
	int style = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION;
	if (RegisterClassA(&WndClass))
	{
		cout << "class registered. Hinstance : " << hInstance <<  " style : (expect 0xcf0000) " << std::hex << style << endl;
		cout << "will create" << endl;
		//These arguments have all been reversed directly from launcher and consequently shouldn't be edited
		hwind2 = CreateWindowExA(0, "EME.LauncherWnd", "???????d???!????????", style, 0x80000000, 0x80000000, 0x80000000, 0x80000000, NULL, NULL, hInstance, NULL);
		if (hwind2 == 0)
			cout << "Couldn't create window" << endl;
		else
			cout << "created window" << endl;
	}
	cout << "Program starting.." << endl;
	if (0 == CreateProcess(NULL, "Client\\TL.exe", NULL, NULL, true, 0x80024, NULL, NULL, &startup, &procinfo))
		std::cout << "Couldn't create TL.exe" << std::endl;
	else
	{
		cout << "Process created." << endl;
		ResumeThread(procinfo.hThread);
		cout << "Thread resumed" << endl;
	}
	MSG msg;
	std::thread t(sendemall);
	while (GetMessage(&msg, hwind2, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		std::string str = GetLastErrorAsString();
		cout << str << endl;
	}
	system("pause");
	return 0;
}

