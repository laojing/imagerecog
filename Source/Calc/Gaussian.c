#include "../main.h"

gsl_matrix*
GaussianMatrix ( gsl_vector *v, float sigma ) {
	gsl_vector_add_constant ( v, -1 );
	gsl_vector_scale ( v, 0.5 );
	
	int siz1 = gsl_vector_get ( v, 0 );
	int siz2 = gsl_vector_get ( v, 1 );
	gsl_matrix *x = gsl_matrix_alloc ( siz1*2+1, siz2*2+1 );
	gsl_matrix *y = gsl_matrix_alloc ( siz1*2+1, siz2*2+1 ); 

	for ( int i=-siz2; i<=siz2; i++ ) {
		for ( int j=-siz1; j<=siz1; j++ ) {
			gsl_matrix_set ( x, i+siz2, j+siz1, j );
			gsl_matrix_set ( y, i+siz2, j+siz1, i );
		}
	}

	gsl_matrix_mul_elements ( x, x );
	gsl_matrix_mul_elements ( y, y );
	gsl_matrix_add ( x, y );
	gsl_matrix_scale ( x, -1/(2*sigma*sigma) );

	float sum = 0;
	for ( int i=0; i<x->size1; i++ ) {
		for ( int j=0; j<x->size2; j++ ) {
			gsl_matrix_set ( x, i, j, exp(gsl_matrix_get ( x, i, j )) );
			sum += gsl_matrix_get ( x, i, j );
		}
	}
	if ( sum != 0 ) gsl_matrix_scale ( x, 1/sum );

	gsl_matrix_free ( y );
	return x;
}


void
Gaussian ( GtkWidget *widget, gpointer gdata ) {
	GuiData *gd = GetGuiData ();
	if ( gd->gauss == NULL ) {
		int i;
		gsl_matrix *gauss = NULL;
		gsl_vector *v = gsl_vector_alloc (2);
		gsl_vector_set ( v, 0, 5 );
		gsl_vector_set ( v, 1, 5 );

		gauss = GaussianMatrix ( v, 1.2 );

		gint width = cairo_image_surface_get_width ( gd->init );
		gint height = cairo_image_surface_get_height ( gd->init );
		unsigned char *data = cairo_image_surface_get_data ( gd->init );
		gsl_matrix *pic = gsl_matrix_alloc ( height, width );
		for ( gint row=0; row<height; row++ ) {
			for ( gint col=0; col<width; col++ ) {
				gsl_matrix_set ( pic, row, col, data[row*width+col] ); 
			}
		}
		gsl_matrix *res = Convolution ( pic, gauss );

		gd->gauss = cairo_image_surface_create ( 
					CAIRO_FORMAT_A8, width, height );
		cairo_surface_flush ( gd->gauss );
		data = cairo_image_surface_get_data ( gd->gauss );
		gint stride = cairo_image_surface_get_stride( gd->gauss );
		for ( gint row=0; row<height; row++ ) {
			for ( gint col=0; col<width; col++ ) {
				data[row*stride+col] = gsl_matrix_get ( res, row, col );
			}
		}
		cairo_surface_mark_dirty ( gd->gauss );

		gsl_vector_free ( v );
		gsl_matrix_free ( gauss );
		gsl_matrix_free ( pic );
		gsl_matrix_free ( res );
	} 
	gd->sCur = "高斯滤波后图片";
	gd->sGauss = "已高斯滤波";
	gd->cur = gd->gauss;
	gtk_widget_queue_draw ( gd->draw );
	UpdateStatusbar ();
}

