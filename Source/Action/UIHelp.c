#include "../main.h"

void 
CloseWindow ( GtkWidget *widget, gpointer data ) {
	gtk_main_quit();
}

void
About ( GtkWidget *widget, gpointer gdata ) {
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_window_set_transient_for ( GTK_WINDOW(dialog), GTK_WINDOW(GetGuiData()->window ));
	gtk_about_dialog_set_program_name ( GTK_ABOUT_DIALOG(dialog), "图像识别"); 
	gtk_about_dialog_set_version ( GTK_ABOUT_DIALOG(dialog), "1.0");
	gtk_about_dialog_set_copyright ( GTK_ABOUT_DIALOG(dialog), _("(c) 沈阳工业大学"));
	gtk_about_dialog_set_comments ( GTK_ABOUT_DIALOG(dialog), "图像识别"); 
	gtk_about_dialog_set_website ( GTK_ABOUT_DIALOG(dialog), "http://www.sut.edu.cn");
	gtk_about_dialog_set_website_label ( GTK_ABOUT_DIALOG(dialog), "http://www.sut.edu.cn");
	gtk_about_dialog_set_logo ( GTK_ABOUT_DIALOG(dialog), gdk_pixbuf_new_from_resource ( "/about.png", NULL ) );
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

char buff[300];
void
UpdateStatusbar () {
	GuiData *gd = GetGuiData();
	gtk_statusbar_pop ( GTK_STATUSBAR(gd->status), gd->context_id );

	if ( gd->x < 0 ) gd->x = 0;
	if ( gd->x > gd->width ) gd->x = gd->width;
	if ( gd->y < 0 ) gd->y = 0;
	if ( gd->y > gd->height ) gd->y = gd->height;

	guchar *idata = cairo_image_surface_get_data ( gd->cur );
	g_snprintf ( buff, 300, "x:%04d y:%04d v:%03d s:%03d z:%3.2f 当前图片：%s  %s  %s  %s  %s", 
			gd->x, gd->y, 
			idata[gd->y*gd->width+gd->x], gd->step, gd->zoom,
			gd->sCur, gd->sInit, 
			gd->sGauss, gd->sRaw, gd->sFine );
	gtk_statusbar_push ( GTK_STATUSBAR(gd->status), gd->context_id, buff );
	gtk_widget_queue_draw ( gd->draw );
}
