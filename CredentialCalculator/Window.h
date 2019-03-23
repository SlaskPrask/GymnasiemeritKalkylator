#pragma once
#include <Windows.h>
#include <iostream>
#include "resource.h"
#include "CourseData.h"
#include <string.h>
#include <CommCtrl.h>
#include <ShObjIdl.h>
#include <propvarutil.h>
#include <strsafe.h>
#include <fstream>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//button ids 100s
#define BTN_ADDCOURSE 100
#define BTN_OPEN 101
#define BTN_SAVE 102
#define BTN_NEW 103

//dialog ids 200s
#define DLG_ADDCOURSE 200

//id 300-399 is reserved for courses

//id listviews
#define LST_COURSES 400

//id edit
#define EDIT_CREDIT 500

class Window
{
private:
	const char g_szClassName[7] = "Window";
	WNDCLASSEX wc;

	MSG Msg;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ListViewProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	int WINAPI WinMain(int nCmdShow);
	int _nCmdShow;

	static CourseData data; 
	static HWND CreateListView(HWND hwndParent);
	static BOOL InsertListViewItems(HWND hWndListView, int cItems);
	static HRESULT OnFileSaveAs();
	static HRESULT _WriteDataToCustomFile(PCWSTR pszFileName);
	static HRESULT _WritePropertyToCustomFile(PCWSTR pszFileName, PCWSTR pszPropertyName, PCWSTR pszValue);
	static HRESULT _WriteDataToFile(HANDLE hFile, PCWSTR pszDataIn);
	static HRESULT OnFileOpen();
	static bool _GetDataFromFile(PCWSTR pszFileName);
	static bool InsertListViewItems(int amount);
	static HWND hwnd;

public:


	Window();
	~Window();
};