#include <unistd.h>
#include "../main.h"

// 忽略掉四个边的像素点数
int ignore = 20;
gfloat weight = 10000;
gfloat huidu = 100;
//gfloat weight = 0;
//gfloat huidu = 0;

gfloat
GetRadius ( gsl_vector *v, size_t i, size_t j ) {
//	return sqrt ( pow ( gsl_vector_get ( v, 0 ) - i, 2 ) + 
//		   pow ( gsl_vector_get ( v, 1 ) - j, 2 ) );
	return sqrt ( i*i + j*j );
}

gfloat 
GetHuiduMatrix ( gsl_matrix *pic, gsl_matrix *raw, gint x, gint y )  {
	gfloat tidu = gsl_matrix_get ( raw, x, y );
	gfloat hui = gsl_matrix_get ( pic, x, y );

	if ( hui <= 1 || hui >= 254 ) tidu = -1;
	return tidu;
}

gboolean 
FindSinglePoint ( gsl_matrix *pic, gsl_matrix* raw, gint width, gint height, gsl_vector **v, gsl_vector **v1, GuiData *gd ) {
	printf ( "V:x:%f y:%f\n", gsl_vector_get(*v,0), gsl_vector_get(*v,1) ); fflush ( stdout );
	printf ( "V1:x:%f y:%f v:%f\n", gsl_vector_get(*v1,0), gsl_vector_get(*v1,1), gsl_vector_get(*v1,2) ); fflush ( stdout );

	gsl_matrix_view view = gsl_matrix_submatrix ( raw, 
			gsl_vector_get ( *v1, 0 ) - 1,
			gsl_vector_get ( *v1, 1 ) - 1, 3, 3 );
	gsl_matrix_view view2 = gsl_matrix_submatrix ( pic, 
			gsl_vector_get ( *v1, 0 ) - 1,
			gsl_vector_get ( *v1, 1 ) - 1, 3, 3 );
	gint len = g_slist_length ( gd->rawlist );
	if ( len >= 2 ) {
		gsl_vector *v = g_slist_nth_data ( gd->rawlist, len - 2 );
		for ( int i=0; i<3; i++ ) {
			for ( int j=0; j<3; j++ ) {
				gsl_matrix_set ( (gsl_matrix*)&view, i, j, 
						gsl_matrix_get ( (gsl_matrix*)&view, i, j )
						+ GetRadius ( v, i-1, j-1 ) * weight
						+ gsl_matrix_get ( (gsl_matrix*)&view2, i, j ) * huidu );
			}
		}
	}

	gint dx = gsl_vector_get ( *v1, 0 ) - gsl_vector_get ( *v, 0 );
	gint dy = gsl_vector_get ( *v1, 1 ) - gsl_vector_get ( *v, 1 );

	gint x = 0, y = 0;
	// 沿y正方向 111111111111
	if ( dx == 0 && dy == 1 ) {
		printf ( "11111111111\n" );
		if ( GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) - 1, gsl_vector_get ( *v1, 1 ) + 1 )
			> GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) + 1, gsl_vector_get ( *v1, 1 ) + 1 ) ) {
			x = gsl_vector_get ( *v1, 0 ) - 1;
			y = gsl_vector_get ( *v1, 1 ) + 1;
		} else {
			x = gsl_vector_get ( *v1, 0 ) + 1;
			y = gsl_vector_get ( *v1, 1 ) + 1;
		}
		if ( GetHuiduMatrix ( pic, raw, x, y )
			< GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ), gsl_vector_get ( *v1, 1 ) + 1 ) ) {
			x = gsl_vector_get ( *v1, 0 );
			y = gsl_vector_get ( *v1, 1 ) + 1;
		}
	// 沿y负方向 222222222222222
	} else if ( dx == 0 && dy == -1 ) {
		printf ( "2222222222222\n" );
		if ( GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) - 1, gsl_vector_get ( *v1, 1 ) - 1 )
			> GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) + 1, gsl_vector_get ( *v1, 1 ) - 1 ) ) {
			x = gsl_vector_get ( *v1, 0 ) - 1;
			y = gsl_vector_get ( *v1, 1 ) - 1;
		} else {
			x = gsl_vector_get ( *v1, 0 ) + 1;
			y = gsl_vector_get ( *v1, 1 ) - 1;
		}
		if ( GetHuiduMatrix ( pic, raw, x, y )
			< GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ), gsl_vector_get ( *v1, 1 ) - 1 ) ) {
			x = gsl_vector_get ( *v1, 0 );
			y = gsl_vector_get ( *v1, 1 ) - 1;
		}
	// 沿x正方向 333333333333333
	} else if ( dx == 1 && dy == 0 ) {
		printf ( "333333333333333\n" );
		if ( GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) + 1, gsl_vector_get ( *v1, 1 ) - 1 )
			> GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) + 1, gsl_vector_get ( *v1, 1 ) + 1 ) ) {
			x = gsl_vector_get ( *v1, 0 ) + 1;
			y = gsl_vector_get ( *v1, 1 ) - 1;
		} else {
			x = gsl_vector_get ( *v1, 0 ) + 1;
			y = gsl_vector_get ( *v1, 1 ) + 1;
		}
		if ( GetHuiduMatrix ( pic, raw, x, y )
			< GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) + 1, gsl_vector_get ( *v1, 1 ) ) ) {
			x = gsl_vector_get ( *v1, 0 ) + 1;
			y = gsl_vector_get ( *v1, 1 );
		}
	// 沿x负方向 4444444444444444
	} else if ( dx == -1 && dy == 0 ) {
		printf ( "444444444444444\n" );
		if ( GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) - 1, gsl_vector_get ( *v1, 1 ) - 1 )
			> GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) - 1, gsl_vector_get ( *v1, 1 ) + 1 ) ) {
			x = gsl_vector_get ( *v1, 0 ) - 1;
			y = gsl_vector_get ( *v1, 1 ) - 1;
		} else {
			x = gsl_vector_get ( *v1, 0 ) - 1;
			y = gsl_vector_get ( *v1, 1 ) + 1;
		}
		if ( GetHuiduMatrix ( pic, raw, x, y )
			< GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) - 1, gsl_vector_get ( *v1, 1 ) ) ) {
			x = gsl_vector_get ( *v1, 0 ) - 1;
			y = gsl_vector_get ( *v1, 1 );
		}
	// 沿xy正方向 55555555555555555
	} else if ( dx == 1 && dy == 1 ) {
		printf ( "5555555555555\n" );
		if ( GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) + 1, gsl_vector_get ( *v1, 1 ) + 1 )
			> GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) + 1, gsl_vector_get ( *v1, 1 ) ) ) {
			x = gsl_vector_get ( *v1, 0 ) + 1;
			y = gsl_vector_get ( *v1, 1 ) + 1;
		} else {
			x = gsl_vector_get ( *v1, 0 ) + 1;
			y = gsl_vector_get ( *v1, 1 );
		}
		if ( GetHuiduMatrix ( pic, raw, x, y )
			< GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ), gsl_vector_get ( *v1, 1 ) + 1 ) ) {
			x = gsl_vector_get ( *v1, 0 );
			y = gsl_vector_get ( *v1, 1 ) + 1;
		}
	// 沿xy负方向 6666666666666666
	} else if ( dx == -1 && dy == -1 ) {
		printf ( "66666666666666666\n" );
		if ( GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) - 1, gsl_vector_get ( *v1, 1 ) - 1 )
			> GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) - 1, gsl_vector_get ( *v1, 1 ) ) ) {
			x = gsl_vector_get ( *v1, 0 ) - 1;
			y = gsl_vector_get ( *v1, 1 ) - 1;
		} else {
			x = gsl_vector_get ( *v1, 0 ) - 1;
			y = gsl_vector_get ( *v1, 1 );
		}
		if ( GetHuiduMatrix ( pic, raw, x, y )
			< GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ), gsl_vector_get ( *v1, 1 ) - 1 ) ) {
			x = gsl_vector_get ( *v1, 0 );
			y = gsl_vector_get ( *v1, 1 ) - 1;
		}
	// 沿x正y负方向 77777777777777777
	} else if ( dx == 1 && dy == -1 ) {
		printf ( "77777777777777777\n" );
		if ( GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) + 1, gsl_vector_get ( *v1, 1 ) - 1 )
			> GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) + 1, gsl_vector_get ( *v1, 1 ) ) ) {
			x = gsl_vector_get ( *v1, 0 ) + 1;
			y = gsl_vector_get ( *v1, 1 ) - 1;
		} else {
			x = gsl_vector_get ( *v1, 0 ) + 1;
			y = gsl_vector_get ( *v1, 1 );
		}
		if ( GetHuiduMatrix ( pic, raw, x, y )
			< GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ), gsl_vector_get ( *v1, 1 ) - 1 ) ) {
			x = gsl_vector_get ( *v1, 0 );
			y = gsl_vector_get ( *v1, 1 ) - 1;
		}
	// 沿y正x负方向 88888888888888888888
	} else if ( dx == -1 && dy == 1 ) {
		printf ( "88888888888888888\n" );
		if ( GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) - 1, gsl_vector_get ( *v1, 1 ) + 1 )
			> GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ) - 1, gsl_vector_get ( *v1, 1 ) ) ) {
			x = gsl_vector_get ( *v1, 0 ) - 1;
			y = gsl_vector_get ( *v1, 1 ) + 1;
		} else {
			x = gsl_vector_get ( *v1, 0 ) - 1;
			y = gsl_vector_get ( *v1, 1 );
		}
		if ( GetHuiduMatrix ( pic, raw, x, y )
			< GetHuiduMatrix ( pic, raw, gsl_vector_get ( *v1, 0 ), gsl_vector_get ( *v1, 1 ) + 1 ) ) {
			x = gsl_vector_get ( *v1, 0 );
			y = gsl_vector_get ( *v1, 1 ) + 1;
		}
	}

	if ( x < ignore || x > height - ignore
			|| y < ignore || y > width - ignore
			|| gsl_matrix_get ( raw, x, y ) < 1000 ) 
		return FALSE;

	//gsl_matrix_view view = gsl_matrix_submatrix ( raw, x-1, y-1, 3, 3 );
	//DispMatrix ( (gsl_matrix*)&view );

	*v = *v1;

	*v1 = gsl_vector_alloc (3);
	gsl_vector_set ( *v1, 0, x );
	gsl_vector_set ( *v1, 1, y );
	gsl_vector_set ( *v1, 2, gsl_matrix_get ( raw, x, y ) );
	gd->rawlist = g_slist_append ( gd->rawlist, *v1 );

	printf ( "x:%d y:%d\n", x, y ); fflush ( stdout );

	gd->x = y;
	gd->y = x;
	/*
	gd->redraw = TRUE;
	while ( gd->redraw ) usleep ( 2 );
	*/

	return TRUE;
}


// 寻找下一个粗边界点
gboolean 
FindNextPoint ( gsl_matrix *pic, gsl_matrix* raw, size_t *iright, size_t *jright, GuiData *gd ) {
	size_t imax, jmax;
	size_t imax2, jmax2;
	gsl_matrix_view view = gsl_matrix_submatrix ( raw, *iright-1, *jright-1, 3, 3 );

	/*
	for ( int i=0; i<3; i++ ) {
		for ( int j=0; j<3; j++ ) {
			if ( gsl_matrix_get ( (gsl_matrix*)&view, i, j ) == 0 ) {
				gsl_matrix_set ( (gsl_matrix*)&view, i, j,  500 );
			}
		}
	}
	gsl_matrix_min_index ( (gsl_matrix*)&view, &imax, &jmax );
	*/

	gsl_matrix_view view2 = gsl_matrix_submatrix ( pic, *iright-1, *jright-1, 3, 3 );

	gsl_matrix_max_index ( (gsl_matrix*)&view, &imax, &jmax );

	gint len = g_slist_length ( gd->rawlist );

	gfloat vmax = gsl_matrix_get ( (gsl_matrix*)&view, imax, jmax );
	//if ( vmax < 3000 ) return FALSE;

	if ( len > 2 ) {
		gsl_vector *v = g_slist_nth_data ( gd->rawlist, len - 2 );
		for ( int i=0; i<3; i++ ) {
			for ( int j=0; j<3; j++ ) {
				gsl_matrix_set ( (gsl_matrix*)&view, i, j, 
						gsl_matrix_get ( (gsl_matrix*)&view, i, j )
						+ GetRadius ( v, i-1, j-1 ) * weight
						+ gsl_matrix_get ( (gsl_matrix*)&view2, i, j ) * huidu );
			}
		}
		while ( abs(gsl_vector_get ( v, 0 ) - (*iright + imax - 1) ) + abs(gsl_vector_get ( v, 1 ) - (*jright + jmax - 1)) <= 2 ) {
			gsl_matrix_set ( (gsl_matrix*)&view, imax, jmax, 0 );
			gsl_matrix_max_index ( (gsl_matrix*)&view, &imax, &jmax );
		}

		//printf ( "%d,%d,%f:%f\n", *iright  );

		/*
		gsl_vector *v = g_slist_nth_data ( gd->rawlist, len - 2 );
		for ( int i=0; i<3; i++ ) {
			for ( int j=0; j<3; j++ ) {
				gsl_matrix_set ( (gsl_matrix*)&view, i, j, 
						gsl_matrix_get ( (gsl_matrix*)&view, i, j )
						+ GetRadius ( v, i-1, j-1 ) * weight
						+ gsl_matrix_get ( (gsl_matrix*)&view2, i, j ) * huidu );
			}
		}
		gsl_matrix_max_index ( (gsl_matrix*)&view, &imax, &jmax );
		*/
		
		/*
		gsl_matrix_set ( (gsl_matrix*)&view, imax, jmax, 0 );
		gsl_matrix_max_index ( (gsl_matrix*)&view, &imax2, &jmax2 );
		gfloat vmax2 = gsl_matrix_get ( (gsl_matrix*)&view, imax2, jmax2 );

		if ( GetRadius ( v, imax, jmax ) * weight + vmax < 
			 GetRadius ( v, imax2, jmax2 ) * weight + vmax2  ) {
			printf ( "%d,%d,%f:%f:%f:%f\n", *iright, *jright, 
					GetRadius ( v, imax, jmax ),
					GetRadius ( v, imax2, jmax2 ),
					vmax, vmax2 );
			imax = imax2; jmax = jmax2;
		}
		*/
	}

	*iright += imax - 1; *jright += jmax - 1;

	/*
	printf ( "%d,%d,%f\n", *iright, *jright, gsl_matrix_get ( raw, *iright, *jright ) );
	DispMatrix ( (gsl_matrix*)&view );
	*/

	gsl_vector *v = gsl_vector_alloc (3);
	gsl_vector_set ( v, 0, *iright );
	gsl_vector_set ( v, 1, *jright );
	gsl_vector_set ( v, 2, gsl_matrix_get ( raw, *iright, *jright ) );
	gd->rawlist = g_slist_append ( gd->rawlist, v );

	//gsl_matrix_set_all ( (gsl_matrix*)&view, 500 );
	gsl_matrix_set_all ( (gsl_matrix*)&view, 0 );

	gd->redraw = TRUE;
	gd->x = *jright;
	gd->y = *iright;
	while ( gd->redraw ) usleep ( 2 );

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
		/*
		gint width = cairo_image_surface_get_width ( gd->init );
		gint height = cairo_image_surface_get_height ( gd->init );
		gsl_matrix *pic = gsl_matrix_alloc ( height, width );
		unsigned char *data = cairo_image_surface_get_data ( gd->init );
		*/
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
		for ( gint row=0; row<height; row++ ) {
			for ( gint col=0; col<width; col++ ) {
				if ( row < ignore || col < ignore 
						|| row > height - ignore 
						|| col > width - ignore ) {
					gsl_matrix_set ( raw, row, col, 0 );
				} else {
					/*
					if ( gsl_matrix_get ( pic, row, col ) > 175 ) {
						gsl_matrix_set ( raw, row, col, 0 );
					} else if ( gsl_matrix_get ( pic, row, col ) < 50 ) {
						gsl_matrix_set ( raw, row, col, 0 );
					} else {
						gsl_matrix_set ( raw, row, col, 
								pow ( gsl_matrix_get ( gx, row, col ), 2 )
								+ pow ( gsl_matrix_get ( gy, row, col ), 2 ) );
					}
							*/
					gsl_matrix_set ( raw, row, col, 
							pow ( gsl_matrix_get ( gx, row, col ), 2 )
							+ pow ( gsl_matrix_get ( gy, row, col ), 2 ) );
				}
			}
		}

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

		/*
		gsl_matrix_view view = gsl_matrix_submatrix ( raw, 1780, 322, 10, 10 );
		DispMatrix ( (gsl_matrix*)&view );
		*/
		
		while ( FindSinglePoint( pic, raw, width, height, &v, &v1, gd ) ) {
			//printf ( "==x:%f y:%f\n", gsl_vector_get(v1,0), gsl_vector_get(v1,1) ); fflush ( stdout );
		}

		gint mx = 0, my = 0;
		gint length = g_slist_length ( gd->rawlist );
		for ( int i=0; i<length; i++ ) {
			gsl_vector *v = g_slist_nth_data ( gd->rawlist, i );
			if ( gsl_vector_get(v,1) > my ) {
				my = gsl_vector_get(v,1);
			}
		}

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



		/*
		// 定位粗边界初始点
		size_t imax, jmax;
		gsl_matrix_max_index ( raw, &imax, &jmax );
		gsl_vector *v = gsl_vector_alloc (3);
		gsl_vector_set ( v, 0, imax );
		gsl_vector_set ( v, 1, jmax );
		gsl_vector_set ( v, 2, gsl_matrix_get ( raw, imax, jmax ) );
		gd->rawlist = g_slist_append ( gd->rawlist, v );

		// 定位两边开始点
		gsl_matrix_set ( raw, imax, jmax, 0 );
		gsl_matrix_view view = gsl_matrix_submatrix ( raw, imax-1, jmax-1, 3, 3 );

		size_t ileft, jleft;
		gsl_matrix_max_index ( (gsl_matrix*)&view, &ileft, &jleft );
		ileft += imax - 1; jleft += jmax - 1;


		gsl_matrix_set ( raw, ileft, jleft, 0 );
		size_t iright, jright;
		gsl_matrix_max_index ( (gsl_matrix*)&view, &iright, &jright );
		iright += imax - 1; jright += jmax - 1;

		gboolean res = TRUE;
		while ( iright >= ignore && iright <= height - ignore
				&& jright >= ignore && jright <= width - ignore
				&& res ) {
			res = FindNextPoint ( pic, raw, &iright, &jright, gd );
		}

		res = TRUE;
		while ( ileft >= ignore && ileft <= height - ignore
				&& jleft >= ignore && jleft <= width - ignore
				&& res ) {
			res = FindNextPoint ( pic, raw, &ileft, &jleft, gd );
		}


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
		*/
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
