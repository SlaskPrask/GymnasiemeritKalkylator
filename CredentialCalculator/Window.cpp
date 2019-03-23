#include "Window.h"

Window::Window()
{
	WinMain(SW_SHOWNORMAL);
}
BOOL CALLBACK Window::DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_COMMAND:

			switch (HIWORD(wParam))
			{
				case BN_CLICKED:
					switch (LOWORD(wParam))
					{
						case (IDOK):
						{
							HWND g_handle = GetDlgItem(hwnd, IDC_COMBO1);
							HWND p_handle = GetDlgItem(hwnd, IDC_COMBO2);
							HWND c_handle = GetDlgItem(hwnd, IDC_EDIT1);

							if (SendMessage(g_handle, CB_GETCURSEL, 0, 0) == 0 || SendMessage(g_handle, CB_GETCURSEL, 0, 0) == 7)
								return false;

							if (SendMessage(p_handle, CB_GETCURSEL, 0, 0) == 0)
								return false;

							char* t = new char[GetWindowTextLength(c_handle) + 1];

							GetWindowText(c_handle, t, GetWindowTextLength(c_handle) + 1);
							
							if (strlen(t) == 0)
							{
								delete[] t;
								return false;
							}

							data.NewCourse(SendMessage(g_handle, CB_GETCURSEL, 0, 0), SendMessage(p_handle, CB_GETCURSEL, 0, 0), t);
							delete[] t;
							return EndDialog(hwnd, 1);
						}
							break;
						case (IDCANCEL):
							return EndDialog(hwnd, 0);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			break;
		
		case WM_INITDIALOG:
			//std::cout << "[CC][DLG] Created Dialog successfully!" << std::endl;
			
			//init the dropdowns:
			{
				HWND g_handle = GetDlgItem(hwnd, IDC_COMBO1);
				HWND p_handle = GetDlgItem(hwnd, IDC_COMBO2);

				if (SendMessage(p_handle, CB_GETCOUNT, 0, 0) == 0)
				{
					SendMessage(p_handle, CB_ADDSTRING, 0, (LPARAM)"---");
					SendMessage(p_handle, CB_ADDSTRING, 0, (LPARAM)"50");
					SendMessage(p_handle, CB_ADDSTRING, 0, (LPARAM)"100");
					SendMessage(p_handle, CB_ADDSTRING, 0, (LPARAM)"150");
					SendMessage(p_handle, CB_ADDSTRING, 0, (LPARAM)"200");
					SendMessage(p_handle, CB_SETCURSEL, 0, (LPARAM)0);
				}

				if (SendMessage(g_handle, CB_GETCOUNT, 0, 0) == 0)
				{
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"---");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"A");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"B");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"C");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"D");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"E");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"F");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"---");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"MVG");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"VG");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"G");
					SendMessage(g_handle, CB_ADDSTRING, 0, (LPARAM)"IG");
					SendMessage(g_handle, CB_SETCURSEL, 0, (LPARAM)0);
				}
				
			}
			return 1;
		default:
			//return DefDlgProc(hwnd, msg, wParam, lParam);
			break;
	}
	return 0;
}

bool Window::InsertListViewItems(int amount)
{
	InsertListViewItems(GetDlgItem(hwnd, LST_COURSES), amount);
	return true;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
		case WM_TIMER:
			return DefWindowProc(hwnd, msg, wParam, lParam);
			break;
		case WM_COMMAND:
			switch (HIWORD(wParam))
			{
				case BN_CLICKED:
					switch (LOWORD(wParam))
					{
						case (BTN_ADDCOURSE):
							std::cout << "Button" << std::endl;
							if (DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgProc))
							{
								InsertListViewItems(GetDlgItem(hwnd, LST_COURSES), 1);
								HWND ed = GetDlgItem(hwnd, EDIT_CREDIT);

								if (SendMessage(ed, WM_SETTEXT, 0, (LPARAM)std::to_string(data.cred).substr(0, 5).c_str()))
									return true;
							}
							else
							{
								std::cout << "[CC][DLG] Pressed Cancel" << std::endl;
							}
							break;
						case (BTN_SAVE):
							OnFileSaveAs();
							break;
						case (BTN_OPEN):
							OnFileOpen();
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}


			break;
		case WM_NOTIFY:
			if (lParam)
			{
				switch (((LPNMHDR)lParam)->code)
				{
				case LVN_GETDISPINFO:
				{
					NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;

					switch (plvdi->item.iSubItem)
					{
						case 0:
							plvdi->item.pszText = (LPSTR)data.GetCourse(plvdi->item.iItem)->name.c_str();
							break;
						case 1:
							plvdi->item.pszText = (LPSTR)std::to_string(data.GetCourse(plvdi->item.iItem)->points).c_str();
							break;
						case 2:
							plvdi->item.pszText = (LPSTR)data.GetCourse(plvdi->item.iItem)->gradeName.c_str();
							break;
						default:
							break;
					}

					break;
				}
				default:
					break;
				}
			}
			break;
		case WM_CREATE:
		{
			//std::cout << "[CC-WIN] Window Created Successfully!" << std::endl;

			HWND addButton = CreateWindowEx(
				NULL, "BUTTON",
				"Lägg till Kurs",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				470, 325, 80, 25,
				hwnd,
				(HMENU)BTN_ADDCOURSE,
				(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

			if (addButton == NULL)
			{
				std::cout << "[CC][WIN] " << "Button initialization failed!" << std::endl;
				return false;
			}

			SendMessage(addButton, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(ANSI_VAR_FONT), MAKELPARAM(FALSE, 0));
	
			HWND saveButton = CreateWindowEx(
				NULL, "BUTTON",
				"Spara",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				470, 10, 50, 25,
				hwnd,
				(HMENU)BTN_SAVE,
				(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

			if (saveButton == NULL)
			{
				std::cout << "[CC][WIN] " << "Button initialization failed!" << std::endl;
				return false;
			}

			SendMessage(saveButton, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(ANSI_VAR_FONT), MAKELPARAM(FALSE, 0));

			HWND openButton = CreateWindowEx(
				NULL, "BUTTON",
				"Öppna",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				525, 10, 50, 25,
				hwnd,
				(HMENU)BTN_OPEN,
				(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

			if (openButton == NULL)
			{
				std::cout << "[CC][WIN] " << "Button initialization failed!" << std::endl;
				return false;
			}

			SendMessage(openButton, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(ANSI_VAR_FONT), MAKELPARAM(FALSE, 0));

			if (CreateListView(hwnd) == NULL)
			{
				std::cout << "[CC][WIN] " << "Listview initialization failed!" << std::endl;
					return false;
			}

			HWND creditLabel = CreateWindowEx(
				NULL, "STATIC",
				"Merit", 
				WS_CHILD | WS_VISIBLE | WS_TABSTOP ,
				497, 45, 40, 25,
				hwnd,
				NULL,
				(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

			SendMessage(creditLabel, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(ANSI_VAR_FONT), MAKELPARAM(FALSE, 0));

			HWND creditEdit = CreateWindowEx(
				WS_EX_CLIENTEDGE, "EDIT",
				"",
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_READONLY | ES_CENTER,
				497, 60, 50, 25,
				hwnd,
				(HMENU)EDIT_CREDIT,
				(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

			if (creditEdit == NULL)
			{
				std::cout << "[CC][WIN] " << "Edit initialization failed!" << std::endl;
				return false;
			}

			SendMessage(creditEdit, WM_SETFONT, (WPARAM)(HFONT)GetStockObject(ANSI_VAR_FONT), MAKELPARAM(FALSE, 0));
		}

			break;
		case WM_SETTEXT:
			std::cout << "IS TEXT" << std::endl;
			break;
		case WM_CHILDACTIVATE:
			std::cout << "AM CHILD :/" << std::endl;
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

Window::~Window()
{
}

int WINAPI Window::WinMain(int nCmdShow)
{
	//Step 1: Register the window
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;// hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		std::cout << "[FORM] Window Registration Failed!" << std::endl;
		return 0;
	}

	DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

	//Step 2: Create the window
	hwnd = CreateWindowEx(
		0,
		g_szClassName,
		"Gymnasiemeritkalkylator",
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
		NULL, NULL, NULL, NULL
	);

	if (hwnd == NULL)
	{
		std::cout << "[FORM] Window Creation Failed!" << std::endl;
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

CourseData Window::data;

LRESULT CALLBACK Window::ListViewProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (Msg)
	{
		case LVM_DELETECOLUMN:
			return true;
			break;
	case WM_KEYDOWN:
		if (wParam == 46)
		{
			std::cout << SendMessage(hwnd, LVM_GETSELECTIONMARK, 0, 0) << std::endl;


			int selIndex = SendMessage(hwnd, LVM_GETSELECTIONMARK, 0, 0);

			if (SendMessage(hwnd, LVM_DELETEITEM, selIndex, 0))
			{
				data.DeleteCourse(selIndex);

				HWND ed = GetDlgItem(GetParent(hwnd), EDIT_CREDIT);

				if (SendMessage(ed, WM_SETTEXT, 0, (LPARAM)std::to_string(data.cred).substr(0, 5).c_str()))
					return true;
			}
			else
				std::cout << SendMessage(hwnd, LVM_GETSELECTIONMARK, 0, 0) << std::endl;
			
			return TRUE;
		}

		break;
	}
	return DefSubclassProc(hwnd, Msg, wParam, lParam);
}

HWND Window::CreateListView(HWND hwndParent)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcClient;

	GetClientRect(hwndParent, &rcClient);

	HWND hWndListView = CreateWindowEx(NULL, WC_LISTVIEW, "",
		WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_VISIBLE,
		10,10, 450,
		340,
		hwndParent,
		(HMENU)LST_COURSES,
		(HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE),
		NULL); 

	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (int iCol = 0; iCol < 3; iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.cx = iCol == 0 ? 300 : 75;
		if (iCol == 0)
			lvc.pszText = (LPSTR)"Kurs";
		if (iCol == 1)
			lvc.pszText = (LPSTR)"Poäng";
		if (iCol == 2)
			lvc.pszText = (LPSTR)"Betyg";

		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
			return false;
	}

	SetWindowSubclass(hWndListView, ListViewProc, 0, 0);

		return hWndListView;
}

BOOL Window::InsertListViewItems(HWND hWndListView, int cItems)
{
	LVITEM lvI;

	lvI.pszText = LPSTR_TEXTCALLBACK;
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.iSubItem = 0;
	lvI.state = 0;

	for (int i = 0; i < cItems; i++)
	{
		lvI.iItem = i;
		lvI.iImage = i;

		if (ListView_InsertItem(hWndListView, &lvI) == -1)
			return false;
	}
	return true;
}

HRESULT Window::_WriteDataToFile(HANDLE hFile, PCWSTR pszDataIn)
{
	DWORD cbData = WideCharToMultiByte(CP_ACP, 0, pszDataIn, -1, NULL, 0, NULL, NULL);
	HRESULT hr = (cbData == 0) ? HRESULT_FROM_WIN32(GetLastError()) : S_OK;

	if (SUCCEEDED(hr))
	{
		char *pszData = new (std::nothrow) CHAR[cbData];

		hr = pszData ? S_OK : E_OUTOFMEMORY;

		if (SUCCEEDED(hr))
		{
			hr = WideCharToMultiByte(CP_ACP, 0, pszDataIn, -1, pszData, cbData, NULL, NULL)
				? S_OK : HRESULT_FROM_WIN32(GetLastError());

			if (SUCCEEDED(hr))
			{
				DWORD dwBytesWritten = 0;
				hr = WriteFile(hFile, pszData, cbData - 1, &dwBytesWritten, NULL)
					? S_OK : HRESULT_FROM_WIN32(GetLastError());
			}
			delete[] pszData;		
		}
	}
	return hr;
}

HRESULT Window::_WritePropertyToCustomFile(PCWSTR pszFileName, PCWSTR pszPropertyName, PCWSTR pszValue)
{
	HANDLE hFile = CreateFileW(pszFileName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	HRESULT hr = (hFile == INVALID_HANDLE_VALUE) ? HRESULT_FROM_WIN32(GetLastError()) : S_OK;

	if (SUCCEEDED(hr))
	{
		const WCHAR wszPropertyStartTag[] = L"[PROPERTY]";
		const WCHAR wszPropertyEndTag[] = L"[ENDPROPERTY]\r\n";
		const DWORD cchPropertyStartTag = (DWORD)wcslen(wszPropertyStartTag);
		const static DWORD cchPropertyEndTag = (DWORD)wcslen(wszPropertyEndTag);
		DWORD const cchPropertyLine = cchPropertyStartTag +
			cchPropertyEndTag + (DWORD)wcslen(pszPropertyName) + (DWORD)wcslen(pszValue) + 2;
		PWSTR pszPropertyLine = new (std::nothrow) WCHAR[cchPropertyLine];
		hr = pszPropertyLine ? S_OK : E_OUTOFMEMORY;

		if (SUCCEEDED(hr))
		{
			hr = StringCchPrintfW(pszPropertyLine,
				cchPropertyLine,
				L"%s%s=%s%s",
				wszPropertyStartTag,
				pszPropertyName,
				pszValue,
				wszPropertyEndTag);

			if (SUCCEEDED(hr))
			{
				hr = SetFilePointer(hFile, 0, NULL, FILE_END) ? S_OK : HRESULT_FROM_WIN32(GetLastError());
				
				if (SUCCEEDED(hr))
				{
					hr = _WriteDataToFile(hFile, pszPropertyLine);
				}
			}
			delete[] pszPropertyLine;
		}
		CloseHandle(hFile);
	}
	return hr;
}

HRESULT Window::_WriteDataToCustomFile(PCWSTR pszFileName)
{
	HANDLE hFile = CreateFileW(pszFileName,
	GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	HRESULT hr = (hFile == INVALID_HANDLE_VALUE) ? HRESULT_FROM_WIN32(GetLastError()) : S_OK;

	if (SUCCEEDED(hr))
	{
		char* c = data.GetAsSaveData();
		int n = MultiByteToWideChar(CP_ACP, 0, c, -1, NULL, 0);

		WCHAR* wszContent = new WCHAR[n];
		MultiByteToWideChar(CP_ACP, 0, c, -1, (LPWSTR)wszContent, n);

		hr = _WriteDataToFile(hFile, wszContent);
		CloseHandle(hFile);
		//delete[] c;
		delete[] wszContent;
	}
	return hr;

}

HRESULT Window::OnFileSaveAs()
{
	IFileSaveDialog *pfsd;
	HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfsd));

	if (SUCCEEDED(hr))
	{
		const COMDLG_FILTERSPEC rgSaveTypes[] = 
		{
			{L"MeritCalc File (*.mkf)", L"*.mkf"}
		};

		hr = pfsd->SetFileTypes(ARRAYSIZE(rgSaveTypes), rgSaveTypes);

		if (SUCCEEDED(hr))
		{
			hr = pfsd->SetFileTypeIndex(0);

			if (SUCCEEDED(hr))
			{
				hr = pfsd->SetDefaultExtension(L"mkf");

				if (SUCCEEDED(hr))
				{
					DWORD dwFlags;
					hr = pfsd->GetOptions(&dwFlags);

					if (SUCCEEDED(hr))
					{
						hr = pfsd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);

						if (SUCCEEDED(hr))
						{
							IPropertyDescriptionList *pd1;
							hr = PSGetPropertyDescriptionListFromString(L"prop:System.Keywords", IID_PPV_ARGS(&pd1));

							if (SUCCEEDED(hr))
							{
								hr = pfsd->SetCollectedProperties(pd1, TRUE);
								pd1->Release();
							}
						}
					}
				}
			}
		}

		if (SUCCEEDED(hr))
		{
			hr = pfsd->Show(NULL);

			if (SUCCEEDED(hr))
			{
				IShellItem *psiResult;
				hr = pfsd->GetResult(&psiResult);

				if (SUCCEEDED(hr))
				{
					PWSTR pszNewFileName;
					hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszNewFileName);

					if (SUCCEEDED(hr))
					{
						hr = _WriteDataToCustomFile(pszNewFileName);
						
						if (SUCCEEDED(hr))
						{
							IPropertyStore *pps;
							hr = pfsd->GetProperties(&pps);
						
							if (SUCCEEDED(hr))
							{
								DWORD cProps = 0;
								hr = pps->GetCount(&cProps);

								for (DWORD i = 0; i < cProps && SUCCEEDED(hr); i++)
								{
									PROPERTYKEY key;
									hr = pps->GetAt(i, &key);

									if (SUCCEEDED(hr))
									{
										PWSTR pszPropertyName;
										hr = PSGetNameFromPropertyKey(key, &pszPropertyName);

										if (SUCCEEDED(hr))
										{
											PROPVARIANT propvarValue;
											PropVariantInit(&propvarValue);
											hr = pps->GetValue(key, &propvarValue);

											if (SUCCEEDED(hr))
											{
												WCHAR wszValue[MAX_PATH];

												hr = PropVariantToString(propvarValue, wszValue, ARRAYSIZE(wszValue));

												if (SUCCEEDED(hr))
												{
													hr = _WritePropertyToCustomFile(pszNewFileName, pszPropertyName, wszValue);
												}
											}
											PropVariantClear(&propvarValue);
											CoTaskMemFree(pszPropertyName);
										}
									}
								}
								pps->Release();
							}		
						}
						CoTaskMemFree(pszNewFileName);
					}
					psiResult->Release();
				}
			}
		}
		pfsd->Release();
	}
	return hr;

}

HRESULT Window::OnFileOpen()
{
	//DO THE THING
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			const COMDLG_FILTERSPEC rgSaveTypes[] =
			{
				{L"MeritCalc File (*.mkf)", L"*.mkf"}
			};

			hr = pFileOpen->SetFileTypes(ARRAYSIZE(rgSaveTypes), rgSaveTypes);

			if (SUCCEEDED(hr))
			{
				hr = pFileOpen->Show(NULL);

				if (SUCCEEDED(hr))
				{
					IShellItem *pItem;
					hr = pFileOpen->GetResult(&pItem);

					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						if (SUCCEEDED(hr))
						{
							_GetDataFromFile(pszFilePath);
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}	
				}
				pFileOpen->Release();
			}
		}
		CoUninitialize();
	}

	return hr;
}

bool Window::_GetDataFromFile(PCWSTR pszFileName)
{
	char* path = 0;
	int bytesRequired = WideCharToMultiByte(CP_ACP, 0, pszFileName, -1, 0, 0, 0, 0);

	if (bytesRequired > 0)
	{
		path = new char[bytesRequired];
		int rc = WideCharToMultiByte(CP_ACP, 0, pszFileName, -1, path, bytesRequired, 0, 0);

		if (rc != 0)
		{
			path[bytesRequired - 1] = 0;
		}
	}

	std::ifstream file(path);

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		std::string buffer(size, ' ');
		file.seekg(0);
		file.read(&buffer[0], size);
		file.close();
		delete[] path;
		data.FromSaveData(buffer);
		InsertListViewItems(data.GetAmount());
		
		HWND ed = GetDlgItem(hwnd, EDIT_CREDIT);
		
		SendMessage(ed, WM_SETTEXT, 0, (LPARAM)std::to_string(data.cred).substr(0, 5).c_str());
		return 1;
	}
	delete[] path;
	return 0;
}

HWND Window::hwnd;