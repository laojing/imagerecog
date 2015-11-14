#include "../main.h"
#include <gdk/gdkkeysyms.h>

gboolean
MouseMove ( 
		GtkWidget *widget,
		GdkEventMotion *event,
		gpointer gdata ) {
	gint x, y;
	GdkModifierType state;
	GuiData *gd = (GuiData*)gdata;
	gdk_window_get_device_position ( event->window,
			event->device, &x, &y, &state );

	gd->x = x/gd->scale-gd->startx+gd->zstartx;
	gd->y = y/gd->scale-gd->starty+gd->zstarty;
	UpdateStatusbar();
	return TRUE;
}

gboolean
KeyPress ( GtkWidget *widget,
		GdkEventKey *event,
		GuiData *gd ) {
	if ( event->keyval == GDK_KEY_Up ) {
		gd->y -= gd->step;
	} else if ( event->keyval == GDK_KEY_Down ) {
		gd->y += gd->step;
	} else if ( event->keyval == GDK_KEY_Left ) {
		gd->x -= gd->step;
	} else if ( event->keyval == GDK_KEY_Right ) {
		gd->x += gd->step;
	} else if ( event->keyval == GDK_KEY_bracketleft ) {
		if ( --gd->step <= 1 ) gd->step = 1;
	} else if ( event->keyval == GDK_KEY_bracketright ) {
		gd->step++;
	} else if ( event->keyval == GDK_KEY_minus ) {
		gd->zoom -= gd->step/100.0;
		if ( gd->zoom < 0 ) gd->zoom = 0;
	} else if ( event->keyval == GDK_KEY_equal ) {
		gd->zoom += gd->step/100.0;
		if ( gd->zoom > 10 ) gd->zoom = 10;
	} else if ( event->keyval == GDK_KEY_z ) {
	}
	UpdateStatusbar();
	return TRUE;
}

gboolean 
MousePress (
		GtkWidget *widget,
		GdkEventButton *event,
		gpointer gdata ) {
	return TRUE;
}

gboolean 
MouseRelease (
		GtkWidget *widget,
		GdkEventButton *event,
		gpointer gdata ) {
	return TRUE;
}
