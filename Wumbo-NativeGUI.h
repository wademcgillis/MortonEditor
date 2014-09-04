#ifndef __Wumbo_NativeGUI_h__
#define __Wumbo_NativeGUI_h__
#include <windows.h>
namespace Wumbo
{
	namespace NativeGUI
	{
		extern void initialize();
		extern void handle_messages();


		extern void hide_control(int control);
		extern void show_control(int control);

		extern void enable_control(int control);
		extern void disable_control(int control);


		int controlgroup_create();
		void controlgroup_addcontrol(int controlgroup, int control);
		void controlgroup_removecontrol(int controlgroup, int control);
		bool controlgroup_getvisible(int controlgroup);
		void controlgroup_setvisible(int controlgroup, bool visible);


		extern int menu_create();
		extern HMENU menu_get_menu(int menu);
		extern int menu_append_submenu(int menu, const char *text);
		extern int submenu_append_string(int submenu, const char *text);


		extern int submenu_append_separator(int submenu);
		extern bool menu_check(int itemID);


		extern HWND get_handle(int element);
		extern void control_sendtoback(int element);


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


		extern int listbox_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		extern int listbox_addstring(int listbox, const char *text);
		extern void listbox_deletestring(int listbox, int index);
		extern int listbox_getselectedindex(int listbox);
		extern void listbox_setselectedindex(int listbox, int index);


		extern int text_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *text);

		extern int textbox_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool readonly, bool multiline, bool hscroll, bool vscroll, bool border);
		extern void textbox_settext(int parent, const char *text);
		extern void textbox_gettext(int parent, char *dest, unsigned int length);


		extern int button_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *text);
		extern bool button_check(int button);

		extern int groupbox_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *text);

		extern int updown_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

		extern int treeview_create(int parent, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
		extern int treeview_addstring(int parent, const char *text, int indentLevel, int image = -1);
		extern int treeview_getselectedindex(int parent);
		extern void treeview_setimagelist(int parent, int imagelist);

		extern int imagelist_create(int width, int height);
		extern int imagelist_addimage(int imagelist, int image);

		extern int image_createfromptr(const unsigned char *ptr, unsigned int width, unsigned int height);
		extern int image_createfromptrsubrect(const unsigned char *ptr, unsigned int width, unsigned int height, unsigned int destX, unsigned int destY, unsigned int destWidth, unsigned int destHeight);
		extern int image_delete(int image);
		/*extern int listbox_addstring(int listbox, const char *text);
		extern void listbox_deletestring(int listbox, int index);
		extern int listbox_getselectedindex(int listbox);
		extern void listbox_setselectedindex(int listbox, int index);*/
	}
}
#endif