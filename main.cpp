#include <cstdio>
#include "Wumbo-NativeGUI.h"

int my_window;
int MYBUTTON;
int COMBO;
int TEXTBOX;
int farts;

#include "stb_image.h"



#include "C:\\LD29\\SOURCE\\MortonLevelData.h"

int img_bird, img_sign, img_gravestone, img_scribble, img_tree, img_fence,
	img_rock, img_dirt, img_spike, img_light, img_brick, img_brickwindow,
	img_stairsup, img_stairsdown, img_hole, img_door, img_selector[2];

MortonLevelData mortondata;
MortonTile *selectedTile;
int selectedTileX, selectedTileY;


HDC offscreenDC;
HBITMAP offscreenBitmap;

void refresh_tiles();

void paintfunc()
{
	Wumbo::NativeGUI::paint_DC(offscreenDC,200,0,480,480);//
}

void refresh_tiles()
{
	Wumbo::NativeGUI::paint_setDC(offscreenDC);
	for(unsigned int i=0;i<400;i+=2)
		Wumbo::NativeGUI::paint_rectangle(24*(i%20 + (i/20)%2),24*(i/20),24,24,0x00DDDDDD);
	for(unsigned int i=1;i<400;i+=2)
		Wumbo::NativeGUI::paint_rectangle(24*(i%20 - (i/20)%2),24*(i/20),24,24,0x00BBBBBB);

	static int xx = 200;
	static int yy = 0;
	for(unsigned int i=0;i<15;i++)
		for(unsigned int j=0;j<15;j++)
		{
			MortonTile *tile = mortondata.get(i,j);
			unsigned short type = tile->type;
			switch(type)
			{
			case TILE_TREE:
				Wumbo::NativeGUI::paint_image(img_tree,24*i,24*j);
				break;
			case TILE_FENCE:
				Wumbo::NativeGUI::paint_image(img_fence,24*i,24*j);
				break;
			case TILE_HOLE:
				Wumbo::NativeGUI::paint_image(img_hole,24*i,24*j,tile->width,tile->height);
				break;
			case TILE_ROCK:
				Wumbo::NativeGUI::paint_image(img_rock,24*i,24*j);
				break;
			case TILE_SPIKE:
				Wumbo::NativeGUI::paint_image(img_spike,24*i,24*j);
				break;
			case TILE_DIRT:
				Wumbo::NativeGUI::paint_image(img_dirt,24*i,24*j);
				break;
			case TILE_BRICK:
				Wumbo::NativeGUI::paint_image(img_brick,24*i,24*j);
				break;
			case TILE_BRICKWINDOW:
				Wumbo::NativeGUI::paint_image(img_brickwindow,24*i,24*j);
				break;
			case TILE_TALKER:
				if (strcmp(tile->strings[0].c_str(),"bird") == 0)
					Wumbo::NativeGUI::paint_image(img_bird,24*i,24*j);
				break;
			}
		}
	static int tick = 0;
	tick++;
	if (selectedTile != NULL)
		Wumbo::NativeGUI::paint_image(img_selector[(tick/10) % 2],24*selectedTileX,24*selectedTileY);
	Wumbo::NativeGUI::paint_resetDC();
	RECT rect;
	rect.left = 200;
	rect.top = 0;
	rect.right = 680;
	rect.bottom = 480;
	InvalidateRect(Wumbo::NativeGUI::get_handle(my_window),&rect,FALSE);
}


int main()
{
	mortondata.loadClassic("C:\\LD29\\SOURCE\\Rooms\\world.oel");
	Wumbo::NativeGUI::initialize();
	Wumbo::NativeGUI::set_paint_function(paintfunc);

	my_window = Wumbo::NativeGUI::window_create(NULL,880,480,"ZOOBERF");








	offscreenDC = CreateCompatibleDC(GetDC(Wumbo::NativeGUI::get_handle(my_window)));
	offscreenBitmap = CreateCompatibleBitmap(GetDC(Wumbo::NativeGUI::get_handle(my_window)), 480, 480);
	SelectObject(offscreenDC, offscreenBitmap);
	//offscreenDC_src = CreateCompatibleDC(offscreenDC_dest);








	printf("my_window handle = %i\n",Wumbo::NativeGUI::get_handle(my_window));

	/*int OBJECTSELECT_WINDOW = Wumbo::NativeGUI::window_create(my_window,240,360,"Add Object");
	int OBJECTSELECT_CANCEL = Wumbo::NativeGUI::button_create(OBJECTSELECT_WINDOW,5+110+5+5,330,110,24,"Cancel");
	Wumbo::NativeGUI::hide_control(OBJECTSELECT_WINDOW);*/

	int imagelist = Wumbo::NativeGUI::imagelist_create(24,24);
	int w,h;
	const unsigned char *ptr = stbi_load("strip.png",&w,&h,NULL,false);
	img_selector[0] = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,			0,0,24,24);
	img_selector[1] = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,			24,0,24,24);
	img_bird = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,			48,0,24,24);
	img_sign = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,			72,0,24,24);
	img_gravestone = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,	96,0,24,24);
	img_scribble = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,		120,0,24,24);
	img_tree = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,			144,0,24,24);
	img_fence = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,		168,0,24,24);
	img_rock = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,			192,0,24,24);
	img_dirt = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,			216,0,24,24);
	img_spike = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,		240,0,24,24);
	img_light = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,		264,0,24,24);
	img_brick = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,		288,0,24,24);
	img_brickwindow = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,	312,0,24,24);
	img_stairsup = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,		336,0,24,24);
	img_stairsdown = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,	360,0,24,24);
	img_hole = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,			384,0,24,24);
	img_door = Wumbo::NativeGUI::image_createfromptrsubrect(ptr,480,24,			408,0,24,24);
	//int imgls_
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

	int tab = Wumbo::NativeGUI::tabcontroller_create(my_window,0,0,200,480);
	int OBJECTS_TAB = Wumbo::NativeGUI::tabcontroller_addtab(tab,0,"Objects");
	int OBJECTS_GROUP = Wumbo::NativeGUI::controlgroup_create();
		int OBJECTS_TREE = Wumbo::NativeGUI::treeview_create(tab,8,30,180,440);
		Wumbo::NativeGUI::treeview_setimagelist(OBJECTS_TREE,imagelist);
		printf("tree = %i\n",Wumbo::NativeGUI::get_handle(OBJECTS_TREE));
		Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Talkers",1);
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Bird",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_bird));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Sign",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_sign));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Gravestone",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_gravestone));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Scribble",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_scribble));
		Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Decorations",1);
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Tree",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_tree));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Fence",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_fence));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Rock",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_rock));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Dirt",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_dirt));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Spike",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_spike));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Light",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_light));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Brick Wall",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_brick));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Brick Window",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_brickwindow));
		Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Transports",1);
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Door",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_door));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Stairs",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_stairsdown));
			Wumbo::NativeGUI::treeview_addstring(OBJECTS_TREE,"Hole",2,Wumbo::NativeGUI::imagelist_addimage(imagelist,img_hole));
		Wumbo::NativeGUI::controlgroup_addcontrol(OBJECTS_GROUP,OBJECTS_TREE);
		Wumbo::NativeGUI::controlgroup_setvisible(OBJECTS_GROUP,true);
	int ROOMINFO_TAB = Wumbo::NativeGUI::tabcontroller_addtab(tab,1,"Room Info");
	int ROOMINFO_GROUP = Wumbo::NativeGUI::controlgroup_create();
		int ROOMINFO_DESCLABEL = Wumbo::NativeGUI::text_create(tab,8,30,160,24,"Room Description:");
		int ROOMINFO_DESCEDIT = Wumbo::NativeGUI::textbox_create(tab,8,48,160,24,false,false,false,false,true);

		int ROOMINFO_WIDTHLABEL = Wumbo::NativeGUI::text_create(tab,8,80,90,24,"Room Width:");
		int ROOMINFO_WIDTHEDIT = Wumbo::NativeGUI::textbox_create(tab,8,98,60,24,true,false,false,false,true);
		int ROOMINFO_WIDTHUPDOWN = Wumbo::NativeGUI::updown_create(tab,68,98,24,24);

		int ROOMINFO_HEIGHTLABEL = Wumbo::NativeGUI::text_create(tab,8,130,90,24,"Room Height:");
		int ROOMINFO_HEIGHTEDIT = Wumbo::NativeGUI::textbox_create(tab,8,148,60,24,true,false,false,false,true);
		int ROOMINFO_HEIGHTUPDOWN = Wumbo::NativeGUI::updown_create(tab,68,148,24,24);

		Wumbo::NativeGUI::controlgroup_addcontrol(ROOMINFO_GROUP,ROOMINFO_DESCLABEL);
		Wumbo::NativeGUI::controlgroup_addcontrol(ROOMINFO_GROUP,ROOMINFO_DESCEDIT);
		Wumbo::NativeGUI::controlgroup_addcontrol(ROOMINFO_GROUP,ROOMINFO_WIDTHLABEL);
		Wumbo::NativeGUI::controlgroup_addcontrol(ROOMINFO_GROUP,ROOMINFO_WIDTHEDIT);
		Wumbo::NativeGUI::controlgroup_addcontrol(ROOMINFO_GROUP,ROOMINFO_WIDTHUPDOWN);
		Wumbo::NativeGUI::controlgroup_addcontrol(ROOMINFO_GROUP,ROOMINFO_HEIGHTLABEL);
		Wumbo::NativeGUI::controlgroup_addcontrol(ROOMINFO_GROUP,ROOMINFO_HEIGHTEDIT);
		Wumbo::NativeGUI::controlgroup_addcontrol(ROOMINFO_GROUP,ROOMINFO_HEIGHTUPDOWN);
		Wumbo::NativeGUI::controlgroup_setvisible(ROOMINFO_GROUP,false);


/*	int listbox = Wumbo::NativeGUI::listbox_create(tab,8,30,184,432);
	for(int i=0;i<100;i++)
		Wumbo::NativeGUI::listbox_addstring(listbox,"bob");
	Wumbo::NativeGUI::listbox_addstring(listbox,"suzy");
	Wumbo::NativeGUI::listbox_addstring(listbox,"grampa");*/

	

	int GX = 880-200+8;
	int GY = 8;
	Wumbo::NativeGUI::groupbox_create(my_window,GX,GY,200-12,480-16,"Instance Info");

	Wumbo::NativeGUI::text_create(my_window,GX+16,GY+24,34,20,"Model:");
	int COMBO_TYPE = Wumbo::NativeGUI::combobox_create(my_window,GX+16+40,GY+22,120,32);
	Wumbo::NativeGUI::combobox_addstring(COMBO_TYPE,"Bird");
	Wumbo::NativeGUI::combobox_addstring(COMBO_TYPE,"Sign");
	Wumbo::NativeGUI::combobox_addstring(COMBO_TYPE,"Gravestone");
	Wumbo::NativeGUI::combobox_addstring(COMBO_TYPE,"Scribble");

	Wumbo::NativeGUI::text_create(my_window,GX+16,GY+24+32,34,20,"Voice:");
	int COMBO_VOICE = Wumbo::NativeGUI::combobox_create(my_window,GX+16+40,GY+22+32,120,32);
	Wumbo::NativeGUI::combobox_addstring(COMBO_VOICE,"Default");
	Wumbo::NativeGUI::combobox_addstring(COMBO_VOICE,"Tweet");
	Wumbo::NativeGUI::combobox_addstring(COMBO_VOICE,"Scream");

	Wumbo::NativeGUI::text_create(my_window,GX+16,GY+24+32+32,64,24,"Width:");
	Wumbo::NativeGUI::textbox_create(my_window,GX+16+40,GY+24+32+32-4,64,24,true,false,false,false,true);
	Wumbo::NativeGUI::updown_create(my_window,GX+16+40+64,GY+24+32+32-4,32,24);

	Wumbo::NativeGUI::text_create(my_window,GX+16,GY+24+32+32+32,64,24,"Height:");
	Wumbo::NativeGUI::textbox_create(my_window,GX+16+40,GY+24+32+32+32-4,64,24,true,false,false,false,true);
	Wumbo::NativeGUI::updown_create(my_window,GX+16+40+64,GY+24+32+32+32-4,32,24);
	//Wumbo::NativeGUI::combobox_addstring(COMBO_TYPE,"Bird");

	//Wumbo::NativeGUI::combobox_addstring*/

	
	
	while(Wumbo::NativeGUI::window_isopen(my_window))
	{
		Wumbo::NativeGUI::handle_messages();
		/*if (Wumbo::NativeGUI::button_check(OBJECTS_ADDBUTTON))
		{
			Wumbo::NativeGUI::show_control(OBJECTSELECT_WINDOW);
			Wumbo::NativeGUI::disable_control(my_window);
		}
		if (Wumbo::NativeGUI::button_check(OBJECTSELECT_CANCEL))
		{
			Wumbo::NativeGUI::enable_control(my_window);
			Wumbo::NativeGUI::hide_control(OBJECTSELECT_WINDOW);
		}*/
		if (Wumbo::NativeGUI::tab_check(OBJECTS_TAB))
		{
			Wumbo::NativeGUI::controlgroup_setvisible(OBJECTS_GROUP,true);
			Wumbo::NativeGUI::controlgroup_setvisible(ROOMINFO_GROUP,false);
		}
		if (Wumbo::NativeGUI::tab_check(ROOMINFO_TAB))
		{
			Wumbo::NativeGUI::controlgroup_setvisible(OBJECTS_GROUP,false);
			Wumbo::NativeGUI::controlgroup_setvisible(ROOMINFO_GROUP,true);
		}
		if (Wumbo::NativeGUI::mouse_left_pressed(200 + 0, 0 + 0, 480, 480))
		{
			selectedTileX = (Wumbo::NativeGUI::mouse_x()-200)/24;
			selectedTileY = (Wumbo::NativeGUI::mouse_y())/24;
			selectedTile = mortondata.get(selectedTileX,selectedTileY);
		}
		refresh_tiles();
		Sleep(33);
	}
	return 0;

	/*int tabbo = Wumbo::NativeGUI::tabcontroller_create(my_window, 300,32,300,200);
	int TAB_MOOSE = Wumbo::NativeGUI::tabcontroller_addtab(tabbo,0,"mooses");
	int TAB_LEMON = Wumbo::NativeGUI::tabcontroller_addtab(tabbo,0,"lemons");


	int moosegroup = Wumbo::NativeGUI::controlgroup_create();
	int MOOSEBUTTON, MOOSEBOX;
		Wumbo::NativeGUI::controlgroup_addcontrol(moosegroup,(MOOSEBOX = Wumbo::NativeGUI::textbox_create(tabbo,32,0,120,32,false,false,false,false,false)));
		Wumbo::NativeGUI::controlgroup_addcontrol(moosegroup,(MOOSEBUTTON = Wumbo::NativeGUI::button_create(tabbo,200,64,64,32,"Moose!")));

	int lemongroup = Wumbo::NativeGUI::controlgroup_create();
	int LEMONBUTTON, LEMONLIST;
		Wumbo::NativeGUI::controlgroup_addcontrol(lemongroup,(LEMONLIST = Wumbo::NativeGUI::listbox_create(tabbo,32,32,120,80)));
			Wumbo::NativeGUI::listbox_addstring(LEMONLIST,"do");
			Wumbo::NativeGUI::listbox_addstring(LEMONLIST,"you");
			Wumbo::NativeGUI::listbox_addstring(LEMONLIST,"like");
			Wumbo::NativeGUI::listbox_addstring(LEMONLIST,"lemons");
			Wumbo::NativeGUI::listbox_addstring(LEMONLIST,"?");
		Wumbo::NativeGUI::controlgroup_addcontrol(lemongroup,(LEMONBUTTON = Wumbo::NativeGUI::button_create(tabbo,200,64,64,32,"Lemon!")));

	Wumbo::NativeGUI::controlgroup_setvisible(moosegroup,true);
	Wumbo::NativeGUI::controlgroup_setvisible(lemongroup,false);

	MYBUTTON = Wumbo::NativeGUI::button_create(my_window,10,10,32,32,"OK");

	int BIG_TEXT_BOX = Wumbo::NativeGUI::textbox_create(my_window,10,120,160,160,false,true,true,true,false);//

	COMBO = Wumbo::NativeGUI::combobox_create(my_window,10,320,100,128);//
		Wumbo::NativeGUI::combobox_addstring(COMBO,"lemons");
		Wumbo::NativeGUI::combobox_addstring(COMBO,"chickens");
		Wumbo::NativeGUI::combobox_addstring(COMBO,"mushrooms");
		Wumbo::NativeGUI::combobox_addstring(COMBO,"diamond");
		Wumbo::NativeGUI::combobox_deletestring(COMBO,2);
	TEXTBOX = Wumbo::NativeGUI::textbox_create(my_window,0,400,120,32,true,false,false,false,false);//
	int TEXTBOX2 = Wumbo::NativeGUI::textbox_create(my_window,130,400,120,32,true,false,false,false,false);//

	while(Wumbo::NativeGUI::window_isopen(my_window))
	{
		Wumbo::NativeGUI::handle_messages();
		char t[32];


		farts = Wumbo::NativeGUI::combobox_getselectedindex(COMBO);
		memset(t,0,32);
		if (farts >= 0)
			sprintf(t,"%i",farts);
		else
			sprintf(t,"<no selection>",farts);
		Wumbo::NativeGUI::textbox_settext(TEXTBOX,t);


		/*farts = Wumbo::NativeGUI::listbox_getselectedindex(listbox);
		memset(t,0,32);
		if (farts >= 0)
			sprintf(t,"%i",farts);
		else
			sprintf(t,"<no selection>",farts);
		Wumbo::NativeGUI::textbox_settext(TEXTBOX2,t);



		if (Wumbo::NativeGUI::menu_check(MENU_NEW))
			printf("new!\n");
		if (Wumbo::NativeGUI::button_check(MYBUTTON))
		{
			char bobo[128];
			Wumbo::NativeGUI::textbox_gettext(BIG_TEXT_BOX,bobo,128);
			MessageBox(Wumbo::NativeGUI::get_handle(my_window),bobo,"THE BOX CONTAINS:",MB_OK | MB_ICONQUESTION);
		}
		if (Wumbo::NativeGUI::tab_check(TAB_MOOSE))
		{
			Wumbo::NativeGUI::controlgroup_setvisible(moosegroup,true);
			Wumbo::NativeGUI::controlgroup_setvisible(lemongroup,false);
		}
		if (Wumbo::NativeGUI::tab_check(TAB_LEMON))
		{
			Wumbo::NativeGUI::controlgroup_setvisible(moosegroup,false);
			Wumbo::NativeGUI::controlgroup_setvisible(lemongroup,true);
		}

		if (Wumbo::NativeGUI::button_check(MOOSEBUTTON))
			MessageBox(Wumbo::NativeGUI::get_handle(my_window),"You clicked the moose button","MESSAGE",MB_OK | MB_ICONEXCLAMATION);
		if (Wumbo::NativeGUI::button_check(LEMONBUTTON))
			MessageBox(Wumbo::NativeGUI::get_handle(my_window),"You clicked the lemon button","MESSAGE",MB_OK | MB_ICONEXCLAMATION);
		Sleep(10);
	}
	return 0;*/
}