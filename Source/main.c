#include "main.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_sf_trig.h>
#include <gsl/gsl_multiroots.h>

static GuiData *data;
GuiData* GetGuiData () { return data; }

gboolean
CycleFunction ( gpointer gdata ) {
	if ( data->redraw ) {
		data->redraw = FALSE;
//		gtk_widget_queue_draw ( data->draw );
		UpdateStatusbar ();
	}
	return TRUE;
}

GtkStyleProvider *provider;
void ApplyCss ( GtkWidget *widget ) {
	gtk_style_context_add_provider (gtk_widget_get_style_context (widget), provider, G_MAXUINT);
	if (GTK_IS_CONTAINER (widget)) {
    	gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) ApplyCss, NULL);
  	}
}

struct rparams {
	double a;
	double b;
};
int
rosenbrock_f ( const gsl_vector *x, void *params,
		       gsl_vector *f ) {
	double a = ((struct rparams *) params)->a;
	double b = ((struct rparams *) params)->b;

	const double x0 = gsl_vector_get ( x, 0 );
	const double x1 = gsl_vector_get ( x, 1 );

	//double y0 = a * ( 0.5 - x0 );
	//double y1 = b * ( gsl_sf_sin(x1) - x0 * x0 );
	double y0 = ( 2*x0 - x1 - exp(-x0) );
	double y1 = ( -x0 + 2*x1 - exp(-x1) );

	gsl_vector_set ( f, 0, y0 );
	gsl_vector_set ( f, 1, y1 );

	return GSL_SUCCESS;
}

int 
print_state ( size_t iter, gsl_multiroot_fsolver *s ) {
	printf ( "iter = %3u x = % .5f % .5f "
			 "f(x) = % .5e % .5e\n",
			 iter,
			 gsl_vector_get ( s->x, 0 ),
			 gsl_vector_get ( s->x, 1 ),
			 gsl_vector_get ( s->f, 0 ),
			 gsl_vector_get ( s->f, 1 ) );
}
int
main ( int argc, char **argv ) {

	/*
	const gsl_multiroot_fsolver_type *T;
	gsl_multiroot_fsolver *s;

	int status;
	size_t i, iter = 0;

	const size_t n = 2;
	struct rparams p = { 1.0, 10.0 };
	gsl_multiroot_function f = { &rosenbrock_f, n, &p };

	double x_init[2] = { -10.0, -0.50 };
	gsl_vector *x = gsl_vector_alloc ( n );

	gsl_vector_set ( x, 0, x_init[0] );
	gsl_vector_set ( x, 1, x_init[1] );

	T = gsl_multiroot_fsolver_hybrids;
	s = gsl_multiroot_fsolver_alloc ( T, 2 );
	gsl_multiroot_fsolver_set ( s, &f, x );

	print_state ( iter, s );

	do {
		iter++;
		status = gsl_multiroot_fsolver_iterate ( s );

		print_state ( iter, s );

		if ( status ) break;

		status = gsl_multiroot_test_residual ( s->f, 1e-7 );
	} while ( status == GSL_CONTINUE && gsl_strerror ( status ) );

	printf ( "Status = %s\n", gsl_strerror ( status ) );

	gsl_multiroot_fsolver_free ( s );
	gsl_vector_free ( x );


	printf ( "trig sin(%f) = %f\n", M_PI/6, gsl_sf_sin(M_PI/6) );
	printf ( "trig sin(%f)", exp(1) );
	*/

	data = g_slice_new0 ( GuiData );
	data->step = 1;
	data->zoom = 1;

	gchar *root = g_strdup ( argv[0] );
	gchar *sub = g_strrstr ( root, "/" );
	data->root = g_strndup ( root, sub - root + 1 );

	gtk_init ( &argc, &argv );
	
	GError *error = NULL;
	GString *str = g_string_new ( data->root );
	g_string_append_printf ( str, "resource.gresource" );
	GResource *resource = g_resource_load ( str->str, &error );
	g_string_free ( str, TRUE );
	g_resources_register ( resource );



	GBytes *bytes;
  	bytes = g_resources_lookup_data ( "/gtk.css", 0, NULL);
	provider = GTK_STYLE_PROVIDER (gtk_css_provider_new ());
	gtk_css_provider_load_from_data (GTK_CSS_PROVIDER(provider), g_bytes_get_data ( bytes, NULL ), -1, NULL);
  	g_bytes_unref (bytes);

	data->window = gtk_window_new ( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_default_size ( GTK_WINDOW(data->window), 1100, 800 );
	gtk_window_set_position ( GTK_WINDOW(data->window), GTK_WIN_POS_CENTER );
	gtk_window_set_title ( GTK_WINDOW(data->window), "图像识别" );
	gtk_window_set_icon ( GTK_WINDOW(data->window),
			gdk_pixbuf_new_from_resource ( "/icon.png", NULL ) );

	GtkWidget *draw = gtk_drawing_area_new ();
	gtk_widget_set_hexpand ( draw, TRUE );
	gtk_widget_set_vexpand ( draw, TRUE );
	data->draw = draw;

	GtkWidget *grid = gtk_grid_new ();
	gtk_grid_attach ( GTK_GRID(grid), CreateMenuBar(), 0, 0, 1, 1 );
	gtk_grid_attach ( GTK_GRID(grid), CreateToolBar(), 0, 1, 1, 1 );
	gtk_grid_attach ( GTK_GRID(grid), draw, 0, 2, 1, 1 );
	gtk_grid_attach ( GTK_GRID(grid), CreateStatusBar(), 0, 3, 1, 1 );

	gtk_container_add ( GTK_CONTAINER (data->window), grid );

	data->sCur = "";
	data->sInit = "";
	data->sGauss = "";
	data->sRaw = "";
	data->sFine = "";
	LoadDefaultPicture ();

	gtk_widget_set_events ( draw, gtk_widget_get_events ( draw ) 
			| GDK_LEAVE_NOTIFY_MASK 
			| GDK_KEY_PRESS_MASK 
			| GDK_BUTTON_PRESS_MASK 
			| GDK_BUTTON_RELEASE_MASK
			| GDK_POINTER_MOTION_MASK 
			| GDK_POINTER_MOTION_HINT_MASK );

	g_signal_connect ( G_OBJECT(draw), "draw", G_CALLBACK(DrawWindow), NULL );
	g_signal_connect ( draw, "motion-notify-event", G_CALLBACK ( MouseMove ), data );
	g_signal_connect ( draw, "button-release-event", G_CALLBACK ( MouseRelease ), NULL );
	g_signal_connect ( draw, "button-press-event", G_CALLBACK ( MousePress ), NULL );
	g_signal_connect ( data->window, "key-press-event", G_CALLBACK ( KeyPress ), data );
	g_signal_connect ( data->window, "destroy", G_CALLBACK(CloseWindow), NULL );

    ApplyCss (data->window);
	g_timeout_add ( 10, CycleFunction, NULL );
	gtk_widget_show_all ( data->window );
	gtk_main ();  
	
	return 0;
}
