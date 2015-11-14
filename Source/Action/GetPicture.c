#include "../main.h"

void 
LoadBmp ( gchar *name ) {
	GuiData* gdata = GetGuiData ();
	GError *error = NULL;
//	gchar *dir = g_strdup_printf ( "%s\\dist\\%s", g_get_current_dir(), "init.bmp" );
//	gchar *dir = g_strdup_printf ( "%s/dist/%s", g_get_current_dir(), "new-000.bmp" );
//	gchar *dir = g_strdup_printf ( "%s/dist/%s", g_get_current_dir(), "init.bmp" );
	gchar *dir = g_strdup_printf ( "%s%s", gdata->root, name );
	GdkPixbuf *pic = gdk_pixbuf_new_from_file ( (const char*)dir, &error );
	g_free ( dir );

	guchar *pixels = gdk_pixbuf_get_pixels ( pic );
	gdata->stride = gdk_pixbuf_get_rowstride(pic);
	gint channel = gdk_pixbuf_get_n_channels(pic);

	gdata->width = gdk_pixbuf_get_width(pic);
	gdata->height = gdk_pixbuf_get_height(pic);
	if ( gdata->init ) cairo_surface_destroy ( gdata->init );
	gdata->init = cairo_image_surface_create ( 
				CAIRO_FORMAT_A8, gdata->width, gdata->height );

	cairo_surface_flush ( gdata->init );
	guchar *idata = cairo_image_surface_get_data ( gdata->init );
	for ( gint row=0; row<gdata->height; row++ ) {
		for ( gint col=0; col<gdata->width; col++ ) {
			idata[row*gdata->width+col] = 255 - (pixels + row*gdata->stride + col*channel)[0];
		}
	}
	gdata->stride /= channel;
	cairo_surface_mark_dirty ( gdata->init );
	gdata->cur = gdata->init;
	gdata->sCur = "初始图片";
	gdata->sInit = "已加载";
	gdata->sGauss = "";
	gdata->sRaw = "";
	gdata->sFine = "";
	if ( gdata->rawlist ) {
		g_slist_free_full ( gdata->rawlist, 
				(GDestroyNotify)DestroyVectorList );
		gdata->rawlist = NULL;
	}
	if ( gdata->gauss ) { 
		cairo_surface_destroy ( gdata->gauss );
		gdata->gauss = NULL;
	}
	if ( gdata->raw ) { 
		cairo_surface_destroy ( gdata->raw );
		gdata->raw = NULL;
	}
	if ( gdata->fine ) { 
		cairo_surface_destroy ( gdata->fine );
		gdata->fine = NULL;
	}
	UpdateStatusbar ();
}

void 
LoadPicture ( GtkWidget *widget, gpointer data ) {

	/*
	gchar *cmd = g_strdup_printf ( "capimage.exe \"%s\\%s\" %d",
			g_get_current_dir(), "test", 4 );
	system ( cmd );
	g_free ( cmd );
	*/

	LoadBmp ( "90-10.bmp" );
	//LoadBmp ( "new-000.bmp" );
	gtk_widget_queue_draw ( GetGuiData()->draw );
}

void 
LoadDefaultPicture () {
//	LoadBmp ( "init.png" );
	//LoadBmp ( "new-000.bmp" );
	LoadBmp ( "90-26.bmp" );
}
