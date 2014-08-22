#include <windows.h>
#include <cstdio>
#include <CommCtrl.h>
#include <map>

//' 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
						version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df'\
						language='*'\"")
#pragma comment(lib,"comctl32.lib")

WNDCLASSEX wc;


template<class T, class U>
class bimap
{
private:
	T Tzero;
	U Uzero;
	std::map<T,U> map__key_value;
	std::map<U,T> map__value_key;
public:
	template<class T, class U> void set_Tzero(T key)
	{
		Tzero = key;
	}
	template<class T, class U> void set_Uzero(U value)
	{
		Uzero = value;
	}
	template<class T, class U> void set(T key, U value)
	{
		if (!key_exists<T,U>(key))
			map__key_value.insert(std::map<T,U>::value_type(key,value));
		/*else
		{
			std::map<T,U>::iterator it;
			it = map__key_value.find(key);
			it->second = value;
		}
*/
		if (!value_exists<T,U>(value))
			map__value_key.insert(std::map<U,T>::value_type(value,key));
/*		else
		{
			std::map<U,T>::iterator it;
			it = map__value_key.find(value);
			it->second = value;
		}*/
	}
	template<class T, class U> void drop_key(T key);
	template<class T, class U> void drop_value(U value);
	template<class T, class U> bool key_exists(T key)
	{
		std::map<T,U>::iterator it;
		it = map__key_value.find(key);
		return (it != map__key_value.end());
	}
	template<class T, class U> bool value_exists(U value)
	{
		std::map<U,T>::iterator it;
		it = map__value_key.find(value);
		return (it != map__value_key.end());
	}
	template<class T, class U> T key_from_value(U value)
	{
		std::map<U,T>::iterator it;
		it = map__value_key.find(value);
		if (it != map__value_key.end())
			return it->second;
		else
			return Tzero;
	}
	template<class T, class U> U value_from_key(T key)
	{
		std::map<T,U>::iterator it;
		it = map__key_value.find(key);
		if (it != map__key_value.end())
			return it->second;
		else
			return Uzero;
	}
};

typedef bimap<unsigned int, HWND> HWNDBIMAP;




namespace gui
{
	const unsigned int ELEMENTS = 128;
	HWND hwnds[ELEMENTS];
	HMENU menus[ELEMENTS];
	bool btnClicked[ELEMENTS];
	unsigned int HWND_ID = 0;
	unsigned int MENU_ID = 0;

	unsigned int menu_create()
	{
		MENU_ID++;
		menus[MENU_ID] = CreateMenu();
		return MENU_ID;
	}

	HMENU menu_get_hmenu(unsigned int menu)
	{
		return menus[menu];
	}

	unsigned int menu_append_submenu(unsigned int menu, const char *text)
	{
		MENU_ID++;
		menus[MENU_ID] = CreatePopupMenu();
		AppendMenu(menus[menu], MF_STRING | MF_POPUP, (UINT)menus[MENU_ID], text);
		return MENU_ID;
	}

	unsigned int submenu_append_string(unsigned int submenu, const char *text)
	{
		MENU_ID++;
		AppendMenu(menus[submenu], MF_STRING, MENU_ID, text);
		return MENU_ID;
	}

	unsigned int submenu_append_separator(unsigned int submenu)
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

	HWND get_handle(unsigned int element)
	{
		return hwnds[element];
	}

	unsigned int window_create(unsigned int parent, unsigned int width, unsigned int height, const char *text)
	{
		HWND_ID++;
		hwnds[HWND_ID] = CreateWindowExA(WS_EX_CLIENTEDGE,
        "WUMBOGUI_WIN32_WINDOW",
		text,
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, (HINSTANCE)GetWindowLong(hwnds[parent], GWL_HINSTANCE), NULL);
		return HWND_ID;
	}

	void window_set_menu(unsigned int window, unsigned int menu)
	{
		SetMenu(hwnds[window],menus[menu]);
	}

	unsigned int tabcontroller_create(unsigned int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		HWND_ID++;
		hwnds[HWND_ID] = CreateWindowExW(
								0, WC_TABCONTROLW,   // predefined class 
                                NULL,         // no window title 
                                WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 
                                x,y,width,height,   // set size in WM_SIZE message 
                                hwnds[parent],         // parent window 
                                (HMENU)HWND_ID,   // edit control ID 
                                (HINSTANCE) GetWindowLong(hwnds[parent], GWL_HINSTANCE), 
                                NULL);        // pointer not needed 
		return HWND_ID;
	}

	unsigned int tabcontroller_addtab(unsigned int parent, unsigned int index, const char *text)
	{
		HWND_ID++;
		TCITEM tcitem;
		tcitem.mask = TCIF_TEXT;
		tcitem.pszText =(char*)text;
		SendMessage(hwnds[parent],(UINT) TCM_INSERTITEMA,(WPARAM) HWND_ID,(LPARAM) &tcitem);
		return HWND_ID;
	}

	unsigned int combobox_create(unsigned int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
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
		return HWND_ID;
	}

	int combobox_addstring(unsigned int combobox, const char *text)
	{
		//HWND_ID++;
		SendMessage(hwnds[combobox],(UINT) CB_ADDSTRING,(WPARAM) 0,(LPARAM) text);
		return SendMessage(hwnds[combobox],(UINT) CB_GETCOUNT,(WPARAM) 0,(LPARAM) 0)-1;
	}

	void combobox_deletestring(unsigned int combobox, int index)
	{
		SendMessage(hwnds[combobox],(UINT) CB_DELETESTRING,(WPARAM) index,(LPARAM) 0);
	}

	int combobox_getselectedindex(unsigned int combobox)
	{
		return SendMessage(hwnds[combobox], CB_GETCURSEL, 0, 0);
	}

	unsigned int textbox_create(unsigned int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool readonly, bool multiline, bool hscroll, bool vscroll)
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

	void textbox_settext(unsigned int parent, const char *text)
	{
		SendMessage(hwnds[parent], WM_SETTEXT, 0, (LPARAM) text);
	}

	unsigned int button_create(unsigned int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *text)
	{
		MENU_ID++;
		hwnds[MENU_ID] = CreateWindowExA(0,WC_BUTTONA,text,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
			x,y,width,height,hwnds[parent],(HMENU)HWND_ID,(HINSTANCE)GetWindowLong(hwnds[parent], GWL_HINSTANCE),NULL);
		return MENU_ID;
	}

	bool button_check(unsigned int button)
	{
		return btnClicked[button];
	}
}



unsigned int my_window;
unsigned int MYBUTTON;
unsigned int COMBO;
unsigned int TEXTBOX;
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
		CloseWindow(gui::get_handle(my_window));
		ShowWindow(gui::get_handle(my_window),SW_HIDE);
        PostQuitMessage(0);
        return 0;
		break;
	case WM_COMMAND:
		//lo = LOWORD(lParam);
		//hi = HIWORD(lParam);
		//printf("--lParam %i\n",lParam);
		//printf("\t LOW = %i\n",lo);
		//printf("\tHIGH = %i\n",hi);
		lo = LOWORD(wParam);
		hi = HIWORD(wParam);
		//printf("--wParam--\n");
		//printf("\t LOW = %i\n",lo);
		//printf("\tHIGH = %i\n",hi);
		//printf("----------\n");
		//printf("\tbtn = %i\n",MYBUTTON);
		if (hi == BN_CLICKED)
		{
			printf("LOWORD(wParam) = %i\n",LOWORD(wParam));
			gui::btnClicked[LOWORD(wParam)] = true;
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
	HWNDBIMAP bob;
	bob.set(5,0);
	printf("%i\n",bob.value_from_key<unsigned int, HWND>(5));
	Sleep(10000);
	return 0;





	InitCommonControlsEx(NULL);
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(gui::get_handle(my_window), GWL_HINSTANCE);

	
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
	ShowWindow(gui::get_handle(my_window),SW_NORMAL);
	UpdateWindow(gui::get_handle(my_window));

	unsigned int men = gui::menu_create();
	unsigned int sub = gui::menu_append_submenu(men,"&File");
		gui::submenu_append_string(sub,"&New");
		gui::submenu_append_string(sub,"&Open");
		gui::submenu_append_string(sub,"&Save");
		gui::submenu_append_string(sub,"Save &As");
		gui::submenu_append_separator(sub);
		gui::submenu_append_string(sub,"&Exit");
	gui::window_set_menu(my_window, men);
	
	unsigned int tabbo = gui::tabcontroller_create(my_window, 300,32,300,200);
	gui::tabcontroller_addtab(tabbo,0,"mooses");
	gui::tabcontroller_addtab(tabbo,0,"lemons");
	gui::textbox_create(tabbo,0,32,120,32,false,false,false,false);

	MYBUTTON = gui::button_create(my_window,10,10,32,32,"OK");//CreateTextBox(my_window,10,10,100,100,true,true,true);
	gui::textbox_create(my_window,10,120,160,160,false,true,true,true);//
	COMBO = gui::combobox_create(my_window,10,320,100,128);//
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
		while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			// If a message was waiting in the message queue, process it
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		//printf("PEEEEEEEEEEEEEEP\n");
		farts = gui::combobox_getselectedindex(COMBO);
		//printf("farts = %i\n",farts);
		char t[32];
		memset(t,0,32);
		if (farts >= 0)
			sprintf(t,"%i",farts);
		else
			sprintf(t,"<no selection>",farts);
		gui::textbox_settext(TEXTBOX,t);
		if (gui::button_check(MYBUTTON))
			printf("DA BUTTON WAS CLICKEED\n");
		Sleep(10);
		for(unsigned int i = 0;i < gui::ELEMENTS; i++)
			gui::btnClicked[i] = false;
		//if (SendMessage(MYBUTTON,BM_GETSTATE,0,0) & BN_CLICKED)
		//	printf("FUCKER\n");
	}
}