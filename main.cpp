#include <cstdio>
#include "Wumbo-NativeGUI.h"

int my_window;
int MYBUTTON;
int COMBO;
int TEXTBOX;
int farts;

int main()
{
	Wumbo::NativeGUI::initialize();

	my_window = Wumbo::NativeGUI::window_create(NULL,800,480,"ZOOBERF");
	printf("my_window handle = %i\n",Wumbo::NativeGUI::get_handle(my_window));

	int men = Wumbo::NativeGUI::menu_create();
	int sub = Wumbo::NativeGUI::menu_append_submenu(men,"&File");
		int sub2 = Wumbo::NativeGUI::menu_append_submenu(sub,"&New");
			int MENU_NEW = Wumbo::NativeGUI::submenu_append_string(sub2,"&ZOOFUS");
		int MENU_OPEN =	Wumbo::NativeGUI::submenu_append_string(sub,"&Open");
		int MENU_SAVE = Wumbo::NativeGUI::submenu_append_string(sub,"&Save");
		int MENU_SAVEAS = Wumbo::NativeGUI::submenu_append_string(sub,"Save &As");
		Wumbo::NativeGUI::submenu_append_separator(sub);
		int MENU_EXIT = Wumbo::NativeGUI::submenu_append_string(sub,"&Exit");
	Wumbo::NativeGUI::window_set_menu(my_window, men);
	
	int tabbo = Wumbo::NativeGUI::tabcontroller_create(my_window, 300,32,300,200);
		int TAB_MOOSE = Wumbo::NativeGUI::tabcontroller_addtab(tabbo,0,"mooses");
		int TAB_LEMON = Wumbo::NativeGUI::tabcontroller_addtab(tabbo,0,"lemons");

	Wumbo::NativeGUI::textbox_create(tabbo,0,32,120,32,false,false,false,false);

	MYBUTTON = Wumbo::NativeGUI::button_create(my_window,10,10,32,32,"OK");

	int BIG_TEXT_BOX = Wumbo::NativeGUI::textbox_create(my_window,10,120,160,160,false,true,true,true);//


	COMBO = Wumbo::NativeGUI::combobox_create(my_window,10,320,100,128);//
		Wumbo::NativeGUI::combobox_addstring(COMBO,"lemons");
		Wumbo::NativeGUI::combobox_addstring(COMBO,"chickens");
		Wumbo::NativeGUI::combobox_addstring(COMBO,"mushrooms");
		Wumbo::NativeGUI::combobox_addstring(COMBO,"diamond");
		Wumbo::NativeGUI::combobox_deletestring(COMBO,2);
	TEXTBOX = Wumbo::NativeGUI::textbox_create(my_window,0,400,120,32,true,false,false,false);//

	while(Wumbo::NativeGUI::window_isopen(my_window))
	{
		Wumbo::NativeGUI::handle_messages();
		farts = Wumbo::NativeGUI::combobox_getselectedindex(COMBO);
		char t[32];
		memset(t,0,32);
		if (farts >= 0)
			sprintf(t,"%i",farts);
		else
			sprintf(t,"<no selection>",farts);
		Wumbo::NativeGUI::textbox_settext(TEXTBOX,t);
		if (Wumbo::NativeGUI::menu_check(MENU_NEW))
			printf("new!\n");
		if (Wumbo::NativeGUI::button_check(MYBUTTON))
		{
			char bobo[128];
			Wumbo::NativeGUI::textbox_gettext(BIG_TEXT_BOX,bobo,128);
			MessageBox(Wumbo::NativeGUI::get_handle(my_window),bobo,"THE BOX CONTAINS:",MB_OK | MB_ICONQUESTION);
			printf("DA BUTTON WAS CLICKEED\n");
		}
		if (Wumbo::NativeGUI::tab_check(TAB_MOOSE))
			printf("MOOSES!\n");
		if (Wumbo::NativeGUI::tab_check(TAB_LEMON))
			printf("LEMONS!\n");
		Sleep(10);
	}
	return 0;
}