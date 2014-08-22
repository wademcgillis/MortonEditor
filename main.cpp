#include <windows.h>
#include <cstdio>
#include <CommCtrl.h>
//' 
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
						version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df'\
						language='*'\"")
#pragma comment(lib,"comctl32.lib")

WNDCLASSEX wc;



namespace gui
{
	HWND hwnds[128];
	HMENU menus[128];
	unsigned int HWND_ID = 0;
	unsigned int MENU_ID = 0;

	HMENU menu_create()
	{
		return CreateMenu();
	}

	HMENU menu_append_submenu(HMENU menu, const char *text)
	{
		MENU_ID++;
		HMENU submenu = CreatePopupMenu();
		AppendMenu(menu, MF_STRING | MF_POPUP, submenu, text);
		return submenu;
	}

	void submenu_append_string(HMENU submenu, const char *text)
	{
		MENU_ID++;
		AppendMenu(submenu, MF_STRING, MENU_ID, text);
	}

	void submenu_append_separator(HMENU submenu)
	{
		MENU_ID++;
		MENUITEMINFO info;
		info.cbSize = sizeof(MENUITEMINFO);
		info.fMask = MIIM_FTYPE;
		info.fType = MFT_SEPARATOR;
		info.cch = 0;
		InsertMenuItem(submenu,MENU_ID,FALSE,&info);
	}

	HWND window_create(HWND parent, unsigned int width, unsigned int height, const char *text)
	{
		return CreateWindowExA(WS_EX_CLIENTEDGE,
        "WUMBOGUI_WIN32_WINDOW",
		text,
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE), NULL);
	}

	HWND tabcontroller_create(HWND parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		return CreateWindowExW(
								0, WC_TABCONTROLW,   // predefined class 
                                NULL,         // no window title 
                                WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 
                                x,y,width,height,   // set size in WM_SIZE message 
                                parent,         // parent window 
                                (HMENU)-1,   // edit control ID 
                                (HINSTANCE) GetWindowLong(parent, GWL_HINSTANCE), 
                                NULL);        // pointer not needed 
	}

	void tabcontroller_addtab(HWND parent, unsigned int index, const char *text)
	{
		TCITEM tcitem;
		tcitem.mask = TCIF_TEXT;
		tcitem.pszText = (char*)text;
		SendMessage(parent,(UINT) TCM_INSERTITEMA,(WPARAM) -1,(LPARAM) &tcitem);
	}

	HWND combobox_create(HWND parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		return CreateWindowExW(
								0, WC_COMBOBOXW,   // predefined class 
                                NULL,         // no window title 
								CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL, 
                                x,y,width,height,   // set size in WM_SIZE message 
                                parent,         // parent window 
                                (HMENU)-1,   // edit control ID 
                                (HINSTANCE) GetWindowLong(parent, GWL_HINSTANCE), 
                                NULL);        // pointer not needed 
	}

	int combobox_addstring(HWND combobox, const char *text)
	{
		//HWND_ID++;
		SendMessage(combobox,(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) text);
		return SendMessage(combobox,(UINT) CB_GETCOUNT,(WPARAM) 0,(LPARAM) 0)-1;
	}

	void combobox_deletestring(HWND combobox, int index)
	{
		SendMessage(combobox,(UINT) CB_DELETESTRING,(WPARAM) index,(LPARAM) 0);
	}

	int combobox_getselectedindex(HWND combobox)
	{
		return SendMessage(combobox, CB_GETCURSEL, 0, 0);
	}

	HWND textbox_create(HWND parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool readonly, bool multiline, bool hscroll, bool vscroll)
	{
		DWORD flags = 0;
		if (readonly)
			flags = flags | ES_READONLY;
		if (multiline)
			flags = flags | ES_MULTILINE;
		if (hscroll)
			flags = flags | WS_HSCROLL | ES_AUTOHSCROLL;
		if (vscroll)
			flags = flags | WS_VSCROLL | ES_AUTOVSCROLL;
		return CreateWindowExW(
								0, WC_EDITW,   // predefined class 
                                NULL,         // no window title 
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | flags, 
                                x,y,width,height,   // set size in WM_SIZE message 
                                parent,         // parent window 
                                (HMENU)-1,   // edit control ID 
                                (HINSTANCE) GetWindowLong(parent, GWL_HINSTANCE), 
                                NULL);        // pointer not needed 
	}

	void textbox_settext(HWND parent, const char *text)
	{
		SendMessage(parent, WM_SETTEXT, 0, (LPARAM) text);
	}

	HWND button_create(HWND parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *text)
	{
		return CreateWindowExA(0,WC_BUTTONA,text,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
			x,y,width,height,parent,(HMENU)-1,(HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),NULL);
	}
}



HWND my_window;
HWND MYBUTTON;
HWND COMBO;
HWND TEXTBOX;
int farts;
LRESULT CALLBACK WndProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	 PAINTSTRUCT ps; 
    HDC hdc; 
	WORD lo;
	WORD hi;
    switch (Message)
    {
        // Quit when we close the main window
	case WM_CREATE:

		break;
    case WM_CLOSE:
		CloseWindow(my_window);
		ShowWindow(my_window,SW_HIDE);
        PostQuitMessage(0);
        return 0;
		break;
	case WM_COMMAND:
		lo = LOWORD(lParam);
		hi = HIWORD(lParam);
		printf("--lParam %i\n",lParam);
		printf("\t LOW = %i\n",lo);
		printf("\tHIGH = %i\n",hi);
		lo = LOWORD(wParam);
		hi = HIWORD(wParam);
		printf("--wParam--\n",wParam);
		printf("\t LOW = %i\n",lo);
		printf("\tHIGH = %i\n",hi);
		printf("----------\n");
		printf("\tbtn = %i\n",MYBUTTON);
		if (hi == BN_CLICKED)
		{
			if ((HWND)lParam == MYBUTTON)
				printf("it is my button!\n");
			printf("CLICKED!\n");
		}
		break;
	/*case WM_PAINT: 
		char str[32];
		memset(str,0,32);
		if (farts >= 0)
			sprintf(str,"%i",farts);
		else
			sprintf(str,"<no selection>",farts);
		hdc = BeginPaint(my_window, &ps); 
		TextOut(hdc, 0, 0, str, 15); 
		EndPaint(my_window, &ps); 
		return 0L; 
	case BN_CLICKED:
		break;*/
    }
    
    return DefWindowProc(Handle, Message, wParam, lParam);
}





int main()
{
	InitCommonControlsEx(NULL);
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(my_window, GWL_HINSTANCE);

	
	 wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
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

	my_window = gui::window_create(NULL,800,480,"ZOOBERF");
	ShowWindow(my_window,SW_NORMAL);
	UpdateWindow(my_window);

	HMENU men = gui::menu_create();
	HMENU sub = gui::menu_append_submenu(men,"&File");
		gui::submenu_append_string(sub,"&New");
		gui::submenu_append_string(sub,"&Open");
		gui::submenu_append_string(sub,"&Save");
		gui::submenu_append_string(sub,"Save &As");
		gui::submenu_append_separator(sub);
		gui::submenu_append_string(sub,"&Exit");
	SetMenu(my_window,men);
	
	HWND tabbo = gui::tabcontroller_create(my_window, 300,32,300,200);
	gui::tabcontroller_addtab(tabbo,0,"mooses");
	gui::tabcontroller_addtab(tabbo,0,"lemons");
	gui::textbox_create(tabbo,0,32,120,32,false,false,false,false);

	MYBUTTON = gui::button_create(my_window,10,10,32,32,"OK");//CreateTextBox(my_window,10,10,100,100,true,true,true);
	gui::textbox_create(my_window,10,120,100,64,false,false,false,false);//
	COMBO = gui::combobox_create(my_window,10,240,100,128);//
	gui::combobox_addstring(COMBO,"lemons");
	gui::combobox_addstring(COMBO,"chickens");
	gui::combobox_addstring(COMBO,"mushrooms");
	gui::combobox_addstring(COMBO,"diamond");
	gui::combobox_deletestring(COMBO,2);
	TEXTBOX = gui::textbox_create(my_window,0,400,120,32,true,false,false,false);//
	/*CreateWindowExW(
                                0, L"EDIT",   // predefined class 
                                NULL,         // no window title 
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_HSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | ES_AUTOVSCROLL | ES_LEFT, 
                               50,50,100,100,   // set size in WM_SIZE message 
                                my_window,         // parent window 
								(HMENU)100,   // edit control ID 
                                (HINSTANCE) GetWindowLong(my_window, GWL_HINSTANCE), 
                                NULL);        // pointer not needed 
		*/


		//CreateWindowW(L"BUTTON",L"OK",WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE,10,10,100,100,my_window,NULL,(HINSTANCE)GetWindowLong(my_window, GWL_HINSTANCE),NULL);
	
	MSG Message;
	Message.message = ~WM_QUIT;
	while(Message.message != WM_QUIT)
	{
		while (GetMessage(&Message, NULL, 0, 0))//, PM_REMOVE))
		{
			// If a message was waiting in the message queue, process it
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		farts = gui::combobox_getselectedindex(COMBO);
		char t[32];
		memset(t,0,32);
		if (farts >= 0)
			sprintf(t,"%i",farts);
		else
			sprintf(t,"<no selection>",farts);
		gui::textbox_settext(TEXTBOX,t);
		Sleep(10);
		//if (SendMessage(MYBUTTON,BM_GETSTATE,0,0) & BN_CLICKED)
		//	printf("FUCKER\n");
	}
}