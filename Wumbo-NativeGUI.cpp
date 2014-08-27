#include <windows.h>
#include <cstdio>
#include <CommCtrl.h>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
						version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df'\
						language='*'\"")
#pragma comment(lib,"comctl32.lib")

namespace Wumbo
{
	namespace NativeGUI
	{
		const unsigned int ELEMENTS = 128;
		const unsigned int MAX_ELEMENTS_TO_BE_UPDATED_PER_TICK = 32;

		HWND hwnds[ELEMENTS];
		HMENU menus[ELEMENTS];
		int comboboxes_activeindex[ELEMENTS];
		int windowhandle_isopen[ELEMENTS];



		HWND buttons_clicked[MAX_ELEMENTS_TO_BE_UPDATED_PER_TICK];
		unsigned int button_click_count;
		int menus_clicked[MAX_ELEMENTS_TO_BE_UPDATED_PER_TICK];
		unsigned int menu_click_count;
		HWND tabs_clicked[MAX_ELEMENTS_TO_BE_UPDATED_PER_TICK];
		unsigned int tab_click_count;

		

		int HWND_ID = 0;
		int MENU_ID = 0;
		WNDCLASSEX wc;

		LRESULT CALLBACK handle_win32_event(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) // WndProc
		{
			WORD wHI = HIWORD(wParam);
			WORD wLO = LOWORD(wParam);
			WORD lHI = HIWORD(lParam);
			WORD lLO = LOWORD(lParam);
			switch (Message)
			{
			case WM_CREATE:
				break;
			case WM_CLOSE:
				DestroyWindow(Handle);
				for(unsigned int i=0;i<=HWND_ID;i++)
				{
					if (hwnds[i] == Handle)
					{
						windowhandle_isopen[i] = false;
						i = HWND_ID + 1;
					}
				}
				//PostQuitMessage(0);
				return 0;
				break;
			case WM_COMMAND:
				if (wHI == 0)
				{
					menus_clicked[Wumbo::NativeGUI::menu_click_count++] = wLO;
				}
				if (wHI == BN_CLICKED)
				{
					buttons_clicked[button_click_count++] = (HWND)lParam;
				}
				if (wHI == CBN_SELCHANGE)
				{
					for(unsigned int i=0;i<=HWND_ID;i++)
					{
						if (hwnds[i] == (HWND)lParam)
							comboboxes_activeindex[i] = (int)SendMessage((HWND)lParam,CB_GETCURSEL,0,0);
					}
				}
				break;
			case WM_NOTIFY:
				if (((NMHDR*)lParam)->code == TCN_SELCHANGE)
				{
					tabs_clicked[tab_click_count++] = (HWND)SendMessage(((LPNMHDR)lParam)->hwndFrom,TCM_GETCURSEL,0,0);
				}
				break;
			}
			return DefWindowProc(Handle, Message, wParam, lParam);
		}

		void initialize()
		{
			InitCommonControlsEx(NULL);
			HINSTANCE hInstance = (HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE);

			wc.cbSize        = sizeof(WNDCLASSEX);
			wc.style         = 0;
			wc.lpfnWndProc   = handle_win32_event;
			wc.cbClsExtra    = 0;
			wc.cbWndExtra    = 0;
			wc.hInstance     = hInstance;
			wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
			wc.lpszMenuName  = NULL;
			wc.lpszClassName = "WUMBOGUI_WIN32_WINDOW";
			wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

			if(!RegisterClassEx(&wc))
				MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		}

		void handle_messages()
		{
			Wumbo::NativeGUI::button_click_count = 0;
			Wumbo::NativeGUI::menu_click_count = 0;
			Wumbo::NativeGUI::tab_click_count = 0;

			MSG Message;
			Message.message = ~WM_QUIT;
			while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
			{
				// If a message was waiting in the message queue, process it
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
		}

		int menu_create()
		{
			MENU_ID++;
			menus[MENU_ID] = CreateMenu();
			return MENU_ID;
		}

		HMENU menu_get_menu(int menu)
		{
			return menus[menu];
		}

		int menu_append_submenu(int menu, const char *text)
		{
			MENU_ID++;
			menus[MENU_ID] = CreatePopupMenu();
			AppendMenu(menus[menu], MF_STRING | MF_POPUP, (UINT)menus[MENU_ID], text);
			return MENU_ID;
		}

		int submenu_append_string(int submenu, const char *text)
		{
			MENU_ID++;
			AppendMenu(menus[submenu], MF_STRING, MENU_ID, text);
			return MENU_ID;
		}

		int submenu_append_separator(int submenu)
		{
			MENU_ID++;
			MENUITEMINFO info;
			info.cbSize = sizeof(MENUITEMINFO);
			info.fMask = MIIM_FTYPE;
			info.fType = MFT_SEPARATOR;
			info.cch = 0;
			InsertMenuItem(menus[submenu],MENU_ID,FALSE,&info);
			return MENU_ID;
		}

		bool menu_check(int itemID)
		{
			for(unsigned int i=0;i<menu_click_count;i++)
				if (menus_clicked[i] == itemID)
					return true;
			return false;
		}

		HWND get_handle(int element)
		{
			return hwnds[element];
		}

		unsigned int window_create(int parent, unsigned int width, unsigned int height, const char *text)
		{
			HWND_ID++;
			hwnds[HWND_ID] = CreateWindowExA(WS_EX_CLIENTEDGE,
					"WUMBOGUI_WIN32_WINDOW",
					text,
					WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
					CW_USEDEFAULT, CW_USEDEFAULT, width, height,
					NULL, NULL, (HINSTANCE)GetWindowLong(hwnds[parent], GWL_HINSTANCE), NULL);
			ShowWindow(hwnds[HWND_ID],SW_NORMAL);
			UpdateWindow(hwnds[HWND_ID]);
			windowhandle_isopen[HWND_ID] = true;
			return HWND_ID;
		}

		void window_set_menu(int window, int menu)
		{
			SetMenu(hwnds[window],menus[menu]);
		}

		bool window_isopen(int window)
		{
			printf("windowhandle_isopen[%i] = %i\n",window,windowhandle_isopen[window]);
			return windowhandle_isopen[window];
		}

		int tabcontroller_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		{
			HWND_ID++;
			//GUIMAP.
			hwnds[HWND_ID] = CreateWindowExW(
					0, WC_TABCONTROLW,   // predefined class 
					NULL,         // no window title 
					WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 
					x,y,width,height,   // set size in WM_SIZE message 
					hwnds[parent],         // parent window 
					(HMENU)HWND_ID,   // edit control ID 
					(HINSTANCE) GetWindowLong(hwnds[parent], GWL_HINSTANCE), 
					NULL);
			return HWND_ID;
		}

		int tabcontroller_addtab(int parent, unsigned int index, const char *text)
		{
			HWND_ID++;
			TCITEM tcitem;
			tcitem.mask = TCIF_TEXT;
			tcitem.pszText =(char*)text;
			tcitem.lParam = HWND_ID;
			hwnds[HWND_ID] = (HWND)SendMessage(hwnds[parent],(UINT) TCM_INSERTITEMA,(WPARAM) HWND_ID,(LPARAM) &tcitem);
			return HWND_ID;
		}

		bool tab_check(int tab)
		{
			for(unsigned int i=0;i<tab_click_count;i++)
				if (tabs_clicked[i] == hwnds[tab])
					return true;
			return false;
		}

		int combobox_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		{
			HWND_ID++;
			hwnds[HWND_ID] = CreateWindowExW(
									0, WC_COMBOBOXW,   // predefined class 
									NULL,         // no window title 
									CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL, 
									x,y,width,height,   // set size in WM_SIZE message 
									hwnds[parent],         // parent window 
									(HMENU)HWND_ID,   // edit control ID 
									(HINSTANCE) GetWindowLong(hwnds[parent], GWL_HINSTANCE), 
									NULL);        // pointer not needed 
			comboboxes_activeindex[HWND_ID] = -1;
			return HWND_ID;
		}

		int combobox_addstring(int combobox, const char *text)
		{
			//HWND_ID++;
			SendMessage(hwnds[combobox],(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) text);
			return SendMessage(hwnds[combobox],(UINT) CB_GETCOUNT,(WPARAM) 0,(LPARAM) 0)-1;
		}

		void combobox_deletestring(int combobox, int index)
		{
			SendMessage(hwnds[combobox],(UINT) CB_DELETESTRING,(WPARAM) index,(LPARAM) 0);
		}

		int combobox_getselectedindex(int combobox)
		{
			return comboboxes_activeindex[combobox];
		}

		void combobox_setselectedindex(int combobox, int index)
		{
			comboboxes_activeindex[combobox] = index;
			SendMessage(hwnds[combobox],CB_SETCURSEL,index,0);
		}

		int textbox_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool readonly, bool multiline, bool hscroll, bool vscroll)
		{
			HWND_ID++;
			DWORD flags = 0;
			if (readonly)
				flags = flags | ES_READONLY;
			if (multiline)
				flags = flags | ES_MULTILINE;
			if (hscroll)
				flags = flags | WS_HSCROLL | ES_AUTOHSCROLL;
			if (vscroll)
				flags = flags | WS_VSCROLL | ES_AUTOVSCROLL;
			hwnds[HWND_ID] = CreateWindowExW(
									0, WC_EDITW,   // predefined class 
									NULL,         // no window title 
									WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | flags, 
									x,y,width,height,   // set size in WM_SIZE message 
									hwnds[parent],         // parent window 
									(HMENU)HWND_ID,   // edit control ID 
									(HINSTANCE) GetWindowLong(hwnds[parent], GWL_HINSTANCE), 
									NULL);        // pointer not needed 
			return HWND_ID;
		}

		void textbox_settext(int parent, const char *text)
		{
			SendMessage(hwnds[parent], WM_SETTEXT, 0, (LPARAM) text);
		}

		void textbox_gettext(int parent, char *dest, unsigned int length)
		{
			if (length == 0)
				return;
			SendMessage(hwnds[parent], WM_GETTEXT, (WPARAM)length, (LPARAM)dest);
		}

		int button_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *text)
		{
			MENU_ID++;
			HWND_ID++;
			hwnds[HWND_ID] = CreateWindowExA(0,WC_BUTTONA,text,
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
				x,y,width,height,hwnds[parent],(HMENU)HWND_ID,(HINSTANCE)GetWindowLong(hwnds[parent], GWL_HINSTANCE),NULL);
			return HWND_ID;
		}

		bool button_check(int button)
		{
			for(unsigned int i=0;i<button_click_count;i++)
				if (buttons_clicked[i] == hwnds[button])
					return true;
			return false;
			//return btnClicked[button];
		}
	}
}