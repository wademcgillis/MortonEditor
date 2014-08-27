#ifndef __Wumbo_NativeGUI_h__
#define __Wumbo_NativeGUI_h__
#include <windows.h>
namespace Wumbo
{
	namespace NativeGUI
	{
		extern void initialize();
		extern void handle_messages();

		extern int menu_create();
		extern HMENU menu_get_menu(int menu);
		extern int menu_append_submenu(int menu, const char *text);
		extern int submenu_append_string(int submenu, const char *text);

		extern int submenu_append_separator(int submenu);
		extern bool menu_check(int itemID);

		extern HWND get_handle(int element);

		extern unsigned int window_create(int parent, unsigned int width, unsigned int height, const char *text);
		extern void window_set_menu(int window, int menu);
		extern bool window_isopen(int window);

		extern int tabcontroller_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		extern int tabcontroller_addtab(int parent, unsigned int index, const char *text);
		extern bool tab_check(int tab);

		extern int combobox_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		extern int combobox_addstring(int combobox, const char *text);
		extern void combobox_deletestring(int combobox, int index);
		extern int combobox_getselectedindex(int combobox);
		extern void combobox_setselectedindex(int combobox, int index);

		extern int textbox_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool readonly, bool multiline, bool hscroll, bool vscroll);
		extern void textbox_settext(int parent, const char *text);
		extern void textbox_gettext(int parent, char *dest, unsigned int length);

		extern int button_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *text);
		extern bool button_check(int button);
	}
}
#endif