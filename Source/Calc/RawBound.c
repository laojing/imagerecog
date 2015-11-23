#include <unistd.h>
#include "../main.h"

// 忽略掉四个边的像素点数
int ignore = 20;
gint weightlen = 3;
gfloat weights[] = {10000, 18000, 5000};
gfloat weight = 10000;
gfloat huidu = 100;
gfloat threshold = 10000;

gfloat GetRadius ( size_t i, size_t j ) { return sqrt ( i*i + j*j ); }

void
GetNextPoint ( gsl_matrix *pic, gsl_matrix *raw, gsl_vector *v1, 
		gint dx1, gint dy1, gint dx2, gint dy2, gint dx3, gint dy3,
		gint *x, gint *y ) {

	gsl_matrix_view view = gsl_matrix_submatrix ( raw, 
			gsl_vector_get ( v1, 0 ) - 1,
			gsl_vector_get ( v1, 1 ) - 1, 3, 3 );
	gsl_matrix_view view2 = gsl_matrix_submatrix ( pic, 
			gsl_vector_get ( v1, 0 ) - 1,
			gsl_vector_get ( v1, 1 ) - 1, 3, 3 );
	for ( int i=0; i<3; i++ ) {
		for ( int j=0; j<3; j++ ) {
			if ( gsl_matrix_get((gsl_matrix*)&view2,i,j) <= 5.0 
					|| gsl_matrix_get((gsl_matrix*)&view2,i,j) >= 250 ) {
				gsl_matrix_set ( (gsl_matrix*)&view, i, j, 0 );
			} else {
			gsl_matrix_set ( (gsl_matrix*)&view, i, j, 
					gsl_matrix_get ( (gsl_matrix*)&view, i, j )
					+ GetRadius ( i-1, j-1 ) * weight
					+ gsl_matrix_get ( (gsl_matrix*)&view2, i, j ) * huidu );
			}
		}
	}
	gfloat x0 = gsl_vector_get ( v1, 0 );
	gfloat y0 = gsl_vector_get ( v1, 1 );
	gfloat f1 = gsl_matrix_get ( raw, x0+dx1, y0+dy1 ); 
	gfloat f2 = gsl_matrix_get ( raw, x0+dx2, y0+dy2 ); 
	gfloat f3 = gsl_matrix_get ( raw, x0+dx3, y0+dy3 ); 
	gfloat f = f1; *x = x0 + dx1; *y = y0 + dy1;
	if ( f2 > f ) f = f2, *x = x0 + dx2, *y = y0 + dy2;
	if ( f3 > f ) f = f3, *x = x0 + dx3, *y = y0 + dy3;
//	if ( f1 > threshold && abs(f1 - f) < threshold ) *x = x0 + dx1, *y = y0 + dy1;
}
gboolean 
FindSinglePoint ( gsl_matrix *pic, gsl_matrix* raw, gint width, gint height, gsl_vector **v, gsl_vector **v1, GuiData *gd ) {
	gint dx = gsl_vector_get ( *v1, 0 ) - gsl_vector_get ( *v, 0 );
	gint dy = gsl_vector_get ( *v1, 1 ) - gsl_vector_get ( *v, 1 );

	gint x = 0, y = 0;
	// 沿y正方向 111111111111
	if ( dx == 0 && dy == 1 ) {
		GetNextPoint ( pic, raw, *v1, 0, 1, 1, 1, -1, 1, &x, &y );
	// 沿y负方向 222222222222222
	} else if ( dx == 0 && dy == -1 ) {
		GetNextPoint ( pic, raw, *v1, 0, -1, 1, -1, -1, -1, &x, &y );
	// 沿x正方向 333333333333333
	} else if ( dx == 1 && dy == 0 ) {
		GetNextPoint ( pic, raw, *v1, 1, 0, 1, -1, 1, 1, &x, &y );
	// 沿x负方向 4444444444444444
	} else if ( dx == -1 && dy == 0 ) {
		GetNextPoint ( pic, raw, *v1, -1, 0, -1, -1, -1, 1, &x, &y );
	// 沿xy正方向 55555555555555555
	} else if ( dx == 1 && dy == 1 ) {
		GetNextPoint ( pic, raw, *v1, 1, 1, 1, 0, 0, 1, &x, &y );
	// 沿xy负方向 6666666666666666
	} else if ( dx == -1 && dy == -1 ) {
		GetNextPoint ( pic, raw, *v1, -1, -1, -1, 0, 0, -1, &x, &y );
	// 沿x正y负方向 77777777777777777
	} else if ( dx == 1 && dy == -1 ) {
		GetNextPoint ( pic, raw, *v1, 1, -1, 1, 0, 0, -1, &x, &y );
	// 沿y正x负方向 88888888888888888888
	} else if ( dx == -1 && dy == 1 ) {
		GetNextPoint ( pic, raw, *v1, -1, 1, -1, 0, 0, 1, &x, &y );
	}

	if ( x < ignore || x > height - ignore
			|| y < ignore || y > width - ignore
			|| gsl_matrix_get ( raw, x, y ) < 1000 ) 
		return FALSE;

	*v = *v1;

	*v1 = gsl_vector_alloc (3);
	gsl_vector_set ( *v1, 0, x );
	gsl_vector_set ( *v1, 1, y );
	gsl_vector_set ( *v1, 2, gsl_matrix_get ( raw, x, y ) );
	gd->rawlist = g_slist_append ( gd->rawlist, *v1 );

//	printf ( "w:%d x:%d y:%d\n", width-ignore, x, y ); fflush ( stdout );

	gd->x = y;
	gd->y = x;
//	gd->redraw = TRUE;
//	while ( gd->redraw ) usleep ( 2 );

	return TRUE;
}

gboolean
RawBound01Thread ( gpointer gdata ) {
	GuiData *gd = GetGuiData ();

	if ( gd->gauss == NULL ) return TRUE;
	if ( gd->raw == NULL ) {
		gint width = cairo_image_surface_get_width ( gd->gauss );
		gint height = cairo_image_surface_get_height ( gd->gauss );
		gsl_matrix *pic = gsl_matrix_alloc ( height, width );
		unsigned char *data = cairo_image_surface_get_data ( gd->gauss );
		for ( gint row=0; row<height; row++ ) {
			for ( gint col=0; col<width; col++ ) {
				gsl_matrix_set ( pic, row, col, data[row*width+col] ); 
			}
		}
		gsl_matrix *h = gsl_matrix_alloc ( 3, 3 );
		gsl_matrix_set_all ( h, 0 );
		gsl_matrix_set ( h, 0, 0, 1 );
		gsl_matrix_set ( h, 0, 1, 2 );
		gsl_matrix_set ( h, 0, 2, 1 );
		gsl_matrix_set ( h, 2, 0, -1 );
		gsl_matrix_set ( h, 2, 1, -2 );
		gsl_matrix_set ( h, 2, 2, -1 );
		gsl_matrix *gx = Convolution ( pic, h );
		gsl_matrix_set_all ( h, 0 );
		gsl_matrix_set ( h, 0, 0, 1 );
		gsl_matrix_set ( h, 1, 0, 2 );
		gsl_matrix_set ( h, 2, 0, 1 );
		gsl_matrix_set ( h, 0, 2, -1 );
		gsl_matrix_set ( h, 1, 2, -2 );
		gsl_matrix_set ( h, 2, 2, -1 );
		gsl_matrix *gy = Convolution ( pic, h );

		gsl_matrix *raw = gsl_matrix_alloc ( height, width );
		ignore -= 10;
		for ( gint row=0; row<height; row++ ) {
			for ( gint col=0; col<width; col++ ) {
				if ( row < ignore || col < ignore 
						|| row > height - ignore 
						|| col > width - ignore ) {
					gsl_matrix_set ( raw, row, col, 0 );
				} else {
					gsl_matrix_set ( raw, row, col, 
							pow ( gsl_matrix_get ( gx, row, col ), 2 )
							+ pow ( gsl_matrix_get ( gy, row, col ), 2 ) );
				}
			}
		}
		ignore += 10;

		gfloat val = 0;
		gint start1 = 0;

		/*
		// 从右到左
		for ( gint row=height/2; row<height; row++ ) {
			if ( gsl_matrix_get ( raw, row, width-ignore ) > val ) {
				val = gsl_matrix_get ( raw, row, width-ignore );
				start1 = row;
			}
		}
		gint start2 = start1 - 1;
		val = gsl_matrix_get ( raw, start1-1, width-ignore-1 );
		if ( val < gsl_matrix_get ( raw, start1, width-ignore-1 ) ) {
			val = gsl_matrix_get ( raw, start1, width-ignore-1 );
			start2++;
		}
		if ( val < gsl_matrix_get ( raw, start1+1, width-ignore-1 ) ) {
			val = gsl_matrix_get ( raw, start1+1, width-ignore-1 );
			start2++;
		}

		gsl_vector *v = gsl_vector_alloc (3);
		gsl_vector_set ( v, 0, start1 );
		gsl_vector_set ( v, 1, width-ignore );
		gsl_vector_set ( v, 2, gsl_matrix_get ( raw, start1, width-ignore ) );
		gd->rawlist = g_slist_append ( gd->rawlist, v );
		gsl_vector *v1 = gsl_vector_alloc (3);
		gsl_vector_set ( v1, 0, start2 );
		gsl_vector_set ( v1, 1, width-ignore-1 );
		gsl_vector_set ( v1, 2, gsl_matrix_get ( raw, start2, width-ignore-1 ) );
		gd->rawlist = g_slist_append ( gd->rawlist, v1 );

		*/

		// 从左到右
		for ( gint row=height/2; row<height; row++ ) {
			if ( gsl_matrix_get ( raw, row, ignore ) > val ) {
				val = gsl_matrix_get ( raw, row, ignore );
				start1 = row;
			}
		}
		gint start2 = start1 - 1;
		val = gsl_matrix_get ( raw, start1-1, ignore+1 );
		if ( val < gsl_matrix_get ( raw, start1, ignore+1 ) ) {
			val = gsl_matrix_get ( raw, start1, ignore+1 );
			start2++;
		}
		if ( val < gsl_matrix_get ( raw, start1+1, ignore+1 ) ) {
			val = gsl_matrix_get ( raw, start1+1, ignore+1 );
			start2++;
		}

		gsl_vector *v = gsl_vector_alloc (3);
		gsl_vector_set ( v, 0, start1 );
		gsl_vector_set ( v, 1, ignore );
		gsl_vector_set ( v, 2, gsl_matrix_get ( raw, start1, ignore ) );
		gd->rawlist = g_slist_append ( gd->rawlist, v );
		gsl_vector *v1 = gsl_vector_alloc (3);
		gsl_vector_set ( v1, 0, start2 );
		gsl_vector_set ( v1, 1, ignore+1 );
		gsl_vector_set ( v1, 2, gsl_matrix_get ( raw, start2, ignore+1 ) );
		gd->rawlist = g_slist_append ( gd->rawlist, v1 );

		for ( gint w=0; w<weightlen; w++ ) {
			weight = weights[w];
			while ( FindSinglePoint( pic, raw, width, height, &v, &v1, gd ) );
			gint length = g_slist_length ( gd->rawlist );
			if ( gsl_vector_get ( v1, 1 ) > width - 2*ignore ) break;
			for ( int i=length-1; i>1; i-- ) {
				gsl_vector *vv = g_slist_nth_data ( gd->rawlist, i );
				gd->rawlist = g_slist_remove ( gd->rawlist, vv );
				gsl_vector_free ( vv );
			}
			v = g_slist_nth_data ( gd->rawlist, 0 );
			v1 = g_slist_nth_data ( gd->rawlist, 1 );
		}
		/*
		printf ( "value:%f\n", gsl_matrix_get ( raw, 1660, 51 ) );
		gsl_matrix_view vview = gsl_matrix_submatrix ( raw, 1660, 45, 10, 10 );
		DispMatrix ( (gsl_matrix*)&vview );
		*/

		gd->raw = cairo_image_surface_create ( 
					CAIRO_FORMAT_A8, width, height );
		cairo_surface_flush ( gd->raw );
		data = cairo_image_surface_get_data ( gd->raw );
		
		guint len = g_slist_length ( gd->rawlist );
		for ( int i=0; i<len; i++ ) {
			gsl_vector *v = g_slist_nth_data ( gd->rawlist, i );
			data[(int)(gsl_vector_get(v,0)*width+gsl_vector_get(v,1))] = 255;
		}
		cairo_surface_mark_dirty ( gd->raw );

		gsl_matrix_free ( pic );
		gsl_matrix_free ( gx );
		gsl_matrix_free ( gy );
		gsl_matrix_free ( h );
		gsl_matrix_free ( raw );
	} 

	gd->sCur = "粗边界后图片";
	gd->sRaw = "已粗边界";
	gd->cur = gd->raw;
	UpdateStatusbar ();
	return TRUE;
}

void
RawBound01 ( GtkWidget *widget, gpointer gdata ) { 
	GThread *gth = g_thread_new ( "rawbound01", (GThreadFunc)RawBound01Thread, NULL );
	g_thread_unref ( gth );
}

void
RawBound02 ( GtkWidget *widget, gpointer gdata ) {
	GuiData *gd = GetGuiData ();
}
