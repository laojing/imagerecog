#include "../main.h"

GtkWidget*
CreateMenu ( gchar *name, GCallback func ) {
	GtkWidget *item = gtk_menu_item_new_with_label ( name );
	gtk_widget_show ( item );
	g_signal_connect ( item, "activate", func, NULL );
	return item;
}

GtkToolItem*
CreateTool ( gchar *name, GCallback func ) {
	GtkToolItem *tool = gtk_tool_button_new ( NULL, NULL );
	gtk_tool_button_set_icon_name ( GTK_TOOL_BUTTON(tool), name );
	g_signal_connect ( tool, "clicked", func, NULL );
	gtk_widget_show ( GTK_WIDGET(tool) );
	return tool;
}

void 
FileNew ( GtkWidget *widget, gpointer data ) {
	gtk_main_quit();
}


GtkWidget *status;
gint context_id;
void 
PushStatus ( GtkWidget *widget, gpointer data ) {
	static int count = 1;
	char buff[20];
	g_snprintf ( buff, 20, "Item %d", count++ );
	gtk_statusbar_push ( GTK_STATUSBAR(status), context_id, buff );
	return;
}
void 
PopStatus ( GtkWidget *widget, gpointer data ) {
	gtk_statusbar_pop ( GTK_STATUSBAR(status), context_id );
}

GtkWidget*
CreateMenuBar () {
	GtkWidget *item = NULL;
	GtkWidget *submenu = NULL;
	GtkWidget *menu = NULL;

	GtkWidget *menubar = gtk_menu_bar_new ();

	// Project
	menu = gtk_menu_item_new_with_label ( "工程" );
	submenu = gtk_menu_new ();

	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu( "新建", G_CALLBACK(FileNew)) );
	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu(_("打开"), G_CALLBACK(FileNew)) );
	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu(_("保存"), G_CALLBACK(FileNew)) );
	GtkWidget *sep = gtk_separator_menu_item_new();
	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), sep );
	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu(_("关闭"), G_CALLBACK(FileNew)) );

	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(menu), submenu );
	gtk_menu_shell_append ( GTK_MENU_SHELL(menubar), menu );

	// Gaussian
	menu = gtk_menu_item_new_with_mnemonic ( "滤波" );
	submenu = gtk_menu_new ();
	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(menu), submenu );

	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu("高斯", G_CALLBACK(Gaussian)) );

	gtk_menu_shell_append ( GTK_MENU_SHELL(menubar), menu );

	// Row Edge
	menu = gtk_menu_item_new_with_mnemonic ( "粗边界" );
	submenu = gtk_menu_new ();
	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(menu), submenu );

	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu("方法01", G_CALLBACK(RawBound01)) );
	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			gtk_separator_menu_item_new() );
	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu("方法02", G_CALLBACK(RawBound02)) );

	gtk_menu_shell_append ( GTK_MENU_SHELL(menubar), menu );

	// Good Edge
	menu = gtk_menu_item_new_with_mnemonic ( "精边界" );
	submenu = gtk_menu_new ();
	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(menu), submenu );

	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu("方法01", G_CALLBACK(FineBound02)) );
	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			gtk_separator_menu_item_new() );
	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu("方法02", G_CALLBACK(FineBound02)) );

	gtk_menu_shell_append ( GTK_MENU_SHELL(menubar), menu );


	// Do All
	menu = gtk_menu_item_new_with_mnemonic ( "工具" );
	submenu = gtk_menu_new ();
	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(menu), submenu );

	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu("全部", G_CALLBACK(About)) );

	gtk_menu_shell_append ( GTK_MENU_SHELL(menubar), menu );


	// Help
	menu = gtk_menu_item_new_with_mnemonic ( "帮助" );
	submenu = gtk_menu_new ();
	gtk_menu_item_set_submenu ( GTK_MENU_ITEM(menu), submenu );

	gtk_menu_shell_append ( GTK_MENU_SHELL(submenu), 
			CreateMenu("关于", G_CALLBACK(About)) );

	gtk_menu_shell_append ( GTK_MENU_SHELL(menubar), menu );


	
	gtk_widget_set_hexpand ( menubar, TRUE );
	gtk_widget_show ( menubar );
	return menubar;
}

GtkWidget*
CreateToolBar () {
	GtkWidget *toolbar = gtk_toolbar_new ();
	gint index = 0;
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			CreateTool("camera-photo", G_CALLBACK(LoadPicture)), index++ );
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			gtk_separator_tool_item_new (), index++ );
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			CreateTool("applications-multimedia", G_CALLBACK(Gaussian)), index++ );
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			gtk_separator_tool_item_new (), index++ );

	// Raw Edge
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			CreateTool("applications-utilities", G_CALLBACK(RawBound01)), index++ );
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			CreateTool("preferences-system", G_CALLBACK(RawBound02)), index++ );
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			gtk_separator_tool_item_new (), index++ );

	// Good Edge
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			CreateTool("system-run", G_CALLBACK(FineBound01)), index++ );
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			CreateTool("applications-system", G_CALLBACK(FineBound02)), index++ );
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			gtk_separator_tool_item_new (), index++ );

	// Do all
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			CreateTool("media-skip-backward-rtl", G_CALLBACK(PopStatus)), index++ );
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			gtk_separator_tool_item_new (), index++ );

	// Aoubt
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			CreateTool("help-about", G_CALLBACK(About)), index++ );
	gtk_toolbar_insert ( GTK_TOOLBAR(toolbar),
			CreateTool("application-exit", G_CALLBACK(CloseWindow)), index++ );

	gtk_widget_show ( toolbar );
	return toolbar;
}

GtkWidget*
CreateStatusBar () {
//	GtkWidget *img = gtk_image_new_from_icon_name ( 
//			"gtk-about", GTK_ICON_SIZE_DND );
	GuiData* gdata = GetGuiData ();
	gdata->status = gtk_statusbar_new ();
	gdata->context_id = gtk_statusbar_get_context_id (
			GTK_STATUSBAR(gdata->status), "Statusbar Example" );
	return gdata->status;
}

