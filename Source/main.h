#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

GtkWidget* CreateMenuBar ();
GtkWidget* CreateToolBar ();
GtkWidget* CreateStatusBar ();
void UpdateStatusbar ();

gboolean DrawWindow ( GtkWidget *widget, cairo_t *cr, gpointer gdata );

typedef struct _GuiData GuiData;
struct _GuiData
{
	gchar *root;
	gboolean redraw;
	gint x;
	gint y;
	gint stride;
	gint width;
	gint height;
	gint step;
	gfloat zoom;
	gfloat scale;
	gfloat startx;
	gfloat starty;
	gfloat zstartx;
	gfloat zstarty;
	GtkWidget *window;
	GtkWidget *draw;
	GtkWidget *status;
	gint context_id;
	cairo_surface_t *cur; 
	cairo_surface_t *init; 
	cairo_surface_t *gauss; 
	cairo_surface_t *raw; 
	cairo_surface_t *fine; 
	gchar *sInit;
	gchar *sGauss;
	gchar *sRaw;
	gchar *sFine;
	gchar *sCur;
	GSList *rawlist;
};

GuiData*
GetGuiData ();

void LoadPicture ( GtkWidget *widget, gpointer data );
void LoadDefaultPicture ();

void About ( GtkWidget *widget, gpointer gdata );
void CloseWindow ( GtkWidget *widget, gpointer data );

void DispVector ( gsl_vector *v );
void DispMatrix ( gsl_matrix *m );
float MatrixSum ( gsl_matrix *x );
gsl_matrix* Convolution ( gsl_matrix *A, gsl_matrix *B );
void DestroyVectorList ( gpointer v );

void Gaussian ( GtkWidget *widget, gpointer gdata );
void RawBound01 ( GtkWidget *widget, gpointer gdata );
void RawBound02 ( GtkWidget *widget, gpointer gdata );
void FineBound01 ( GtkWidget *widget, gpointer gdata );
void FineBound02 ( GtkWidget *widget, gpointer gdata );

gboolean
MouseMove ( 
		GtkWidget *widget,
		GdkEventMotion *event,
		gpointer gdata );

gboolean 
MousePress (
		GtkWidget *widget,
		GdkEventButton *event,
		gpointer gdata );

gboolean 
MouseRelease (
		GtkWidget *widget,
		GdkEventButton *event,
		gpointer gdata );

gboolean
KeyPress ( GtkWidget *widget,
		GdkEventKey *event,
		GuiData *gdata );


#endif