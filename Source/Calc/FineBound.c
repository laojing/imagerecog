#include "../main.h"

#define NEWRADIUS 4000
#define REALRADIUS 3179.5
#define TOOTHROOT 2953.7

gfloat a1;
gfloat a0;
gfloat radius;
gfloat centerx;
gfloat centery;

gboolean
FineBound01Thread ( gpointer gdata ) {
	GuiData *gd = GetGuiData ();

	gint offset = NEWRADIUS - gd->width/2;
	gint len = g_slist_length ( gd->rawlist );
	if ( gd->rawlist == NULL ) return TRUE;
	if ( gd->fine != NULL ) return TRUE;

	gsl_vector *x = gsl_vector_alloc ( len );
	gsl_vector *y = gsl_vector_alloc ( len );

	for ( int i=0; i<len; i++ ) {
		gsl_vector *v = g_slist_nth_data ( gd->rawlist, i );
		gsl_vector_set ( v, 1, gsl_vector_get(v,1) + offset );
		gsl_vector_set ( x, i, gsl_vector_get(v,1) );
		gsl_vector_set ( y, i, gsl_vector_get(v,0) );
	}

	gsl_vector *res = PolyFit ( x, y, 1 );
	a0 = gsl_vector_get ( res, 0 );
	a1 = gsl_vector_get ( res, 1 );
	gsl_vector_free ( res );

	res = CircleFit ( x, y );
	radius = gsl_vector_get ( res, 0 );
	centerx = gsl_vector_get ( res, 1 );
	centery = gsl_vector_get ( res, 2 );
	gsl_vector_free ( res );

	gint tol = 10;
	while ( abs(radius - REALRADIUS) >=5 ) {
		// 计算垂足
		gfloat footy = (a1*a1*centery + a1*centerx + a0)/(a1*a1 + 1);
		gfloat footx = (footy - a0)/a1;

		gfloat length = sqrt(pow(centerx - footx, 2) + pow(centery - footy, 2));
		gfloat ang = asin((centerx - footx)/length);
		length += (REALRADIUS - radius)*2/3;

		gfloat newcenterx = footx + (sin(ang) * length);
		gfloat newcentery = footy + (cos(ang) * length);

		gsl_vector *lens = gsl_vector_alloc ( len );
		for ( int i=0; i<len; i++ ) {
			gsl_vector_set ( lens, i, sqrt(pow(newcenterx - gsl_vector_get(x,i), 2) + pow(newcentery - gsl_vector_get(y,i), 2)) );
		}
		double max = gsl_vector_max ( lens );
		gint maxcount = 0;
		for ( int i=0; i<len; i++ ) {
			if ( gsl_vector_get ( lens, i ) > max - tol ) maxcount++;
		}
		gsl_vector *newx = gsl_vector_alloc ( maxcount );
		gsl_vector *newy = gsl_vector_alloc ( maxcount );
		maxcount = 0;
		for ( int i=0; i<len; i++ ) {
			if ( gsl_vector_get ( lens, i ) > max - tol ) {
				gsl_vector_set ( newx, maxcount, gsl_vector_get ( x, i ) );
				gsl_vector_set ( newy, maxcount++, gsl_vector_get ( y, i ) );
			}
		}

		res = PolyFit ( newx, newy, 1 );
		a0 = gsl_vector_get ( res, 0 );
		a1 = gsl_vector_get ( res, 1 );
		gsl_vector_free ( res );

		res = CircleFit ( newx, newy );
		radius = gsl_vector_get ( res, 0 );
		centerx = gsl_vector_get ( res, 1 );
		centery = gsl_vector_get ( res, 2 );
		gsl_vector_free ( res );

		gsl_vector_free ( lens );
		gsl_vector_free ( newx );
		gsl_vector_free ( newy );

	}

	// 寻找齿顶
	gsl_vector *lens = gsl_vector_alloc ( len );
	for ( int i=0; i<len; i++ ) {
		gsl_vector_set ( lens, i, sqrt(pow(centerx - gsl_vector_get(x,i), 2) + pow(centery - gsl_vector_get(y,i), 2)) );
	}

	gint maxcount = 0;
	for ( int i=0; i<len; i++ ) {
		if ( abs(gsl_vector_get ( lens, i ) - radius) < 2 ) maxcount++;
	}

	gd->toothtipx = gsl_vector_alloc ( maxcount );
	gd->toothtipy = gsl_vector_alloc ( maxcount );
	maxcount = 0;
	for ( int i=0; i<len; i++ ) {
		if ( abs(gsl_vector_get ( lens, i ) - radius) < 2 ) {
			gsl_vector_set ( gd->toothtipx, maxcount, gsl_vector_get ( x, i ) );
			gsl_vector_set ( gd->toothtipy, maxcount++, gsl_vector_get ( y, i ) );
			gsl_vector_set ( x, i, 0 );
		}
	}

	gsl_vector *newx = gsl_vector_alloc ( maxcount );
	gsl_vector *newy = gsl_vector_alloc ( maxcount );
	gsl_vector_memcpy ( newx, gd->toothtipx );
	gsl_vector_memcpy ( newy, gd->toothtipy );

	res = PolyFit ( newx, newy, 1 );
	a0 = gsl_vector_get ( res, 0 );
	a1 = gsl_vector_get ( res, 1 );
	gsl_vector_free ( res );

	res = CircleFit ( newx, newy );
	radius = gsl_vector_get ( res, 0 );
	centerx = gsl_vector_get ( res, 1 );
	centery = gsl_vector_get ( res, 2 );
	gsl_vector_free ( res );

	gsl_vector_free ( newx );
	gsl_vector_free ( newy );

	// 寻找齿根
	maxcount = 0;
	for ( int i=0; i<len; i++ ) {
		if ( gsl_vector_get ( lens, i ) < TOOTHROOT ) maxcount++;
	}
	gd->toothrootx = gsl_vector_alloc ( maxcount );
	gd->toothrooty = gsl_vector_alloc ( maxcount );
	maxcount = 0;
	for ( int i=0; i<len; i++ ) {
		if ( gsl_vector_get ( lens, i ) < TOOTHROOT ) {
			gsl_vector_set ( gd->toothrootx, maxcount, gsl_vector_get ( x, i ) );
			gsl_vector_set ( gd->toothrooty, maxcount++, gsl_vector_get ( y, i ) );
			gsl_vector_set ( x, i, 0 );
		}
	}

	// 寻找齿廓
	maxcount = 0;
	for ( int i=0; i<len; i++ ) {
		if ( gsl_vector_get ( x, i ) > 1 ) maxcount++;
	}
	gd->toothprofilex = gsl_vector_alloc ( maxcount );
	gd->toothprofiley = gsl_vector_alloc ( maxcount );
	maxcount = 0;
	for ( int i=0; i<len; i++ ) {
		if ( gsl_vector_get ( x, i ) > 1 ) {
			gsl_vector_set ( gd->toothprofilex, maxcount, gsl_vector_get ( x, i ) );
			gsl_vector_set ( gd->toothprofiley, maxcount++, gsl_vector_get ( y, i ) );
		}
	}
	gsl_vector_free ( lens );

	maxcount = 0;
	for ( int i=0; i<gd->toothprofilex->size-1; i++ ) {
		if ( gsl_vector_get ( gd->toothprofilex, i+1 ) -  gsl_vector_get ( gd->toothprofilex, i ) < 10 ) continue;
		else maxcount++;
	}
	gsl_vector *profiles = gsl_vector_alloc ( 2 + maxcount );
	gsl_vector_set ( profiles, 0, -1.0 );
	gsl_vector_set ( profiles, maxcount+1, gd->toothprofilex->size-1 );
	maxcount = 1;
	for ( int i=0; i<gd->toothprofilex->size-1; i++ ) {
		if ( gsl_vector_get ( gd->toothprofilex, i+1 ) -  gsl_vector_get ( gd->toothprofilex, i ) < 10 ) continue;
		else gsl_vector_set ( profiles, maxcount++, i );
	}

	for ( int i=0; i<profiles->size-1; i++ ) {
		gint start = gsl_vector_get ( profiles, i ) + 1;
		gint end = gsl_vector_get ( profiles, i+1 );
		gsl_vector *profilex = gsl_vector_alloc ( end - start + 1 );
		gsl_vector *profiley = gsl_vector_alloc ( end - start + 1 );

		for ( int j=0; j<=end-start; j++ ) {
			gsl_vector_set ( profilex, j, gsl_vector_get ( gd->toothprofilex, start+j ) );
			gsl_vector_set ( profiley, j, gsl_vector_get ( gd->toothprofiley, start+j ) );
		}

		gsl_vector_free ( profilex );
		gsl_vector_free ( profiley );
	}




	for ( int i=0; i<profiles->size; i++ ) {
		printf ( "%f\n", gsl_vector_get(profiles,i) );
	}

	printf ( "maxcount = %d\n", maxcount );
	printf ( "a0 = %f, a1 = %f\n", a0, a1 );
	printf ( "radius = %f, centerx = %f, centery = %f\n", radius, centerx, centery );
	fflush ( stdout );

	gsl_vector_free ( profiles );




	// 生成精边界后图片
	gd->width = 2*NEWRADIUS;
	gd->height = 2*NEWRADIUS;
	gd->fine = cairo_image_surface_create ( 
				CAIRO_FORMAT_A8, gd->width, gd->height );

	gint rowstride = cairo_image_surface_get_stride(gd->fine);
	cairo_surface_flush ( gd->fine );
	guchar *idata = cairo_image_surface_get_data ( gd->fine );

//	for ( int i=0; i<gd->toothtipx->size; i++ ) {
//		idata[(int)gsl_vector_get(gd->toothtipy,i)*rowstride+(int)gsl_vector_get(gd->toothtipx,i)] = 255;
//	}
//	for ( int i=0; i<gd->toothrootx->size; i++ ) {
//		idata[(int)gsl_vector_get(gd->toothrooty,i)*rowstride+(int)gsl_vector_get(gd->toothrootx,i)] = 255;
//	}
	for ( int i=0; i<gd->toothprofilex->size; i++ ) {
		idata[(int)gsl_vector_get(gd->toothprofiley,i)*rowstride+(int)gsl_vector_get(gd->toothprofilex,i)] = 255;
	}

	// 圆心的十字
	for ( int i=-NEWRADIUS/20; i<NEWRADIUS/20; i++ ) {
		idata[(int)(centery)*rowstride+(int)(centerx+i)] = 255;
		idata[(int)(centery+i)*rowstride+(int)(centerx)] = 255;
	}

	// 拟合出的圆
	for ( gint i=0; i<3600; i++ ) {
		double ang = i*0.1*M_PI/180;
		gint tempx = (int)(centerx - radius*sin(ang));
		gint tempy = (int)(centery - radius*cos(ang));
		idata[tempy*rowstride+tempx] = 255;
		idata[tempy*rowstride+tempx+1] = 255;
		idata[tempy*rowstride+tempx-1] = 255;
	}

	cairo_surface_mark_dirty ( gd->fine );

	gd->sCur = "精边界后图片";
	gd->sFine = "已精边界";
	gd->cur = gd->fine;
	UpdateStatusbar ();

	return TRUE;
}

void
FineBound01 ( GtkWidget *widget, gpointer gdata ) {
	GThread *gth = g_thread_new ( "finebound01", (GThreadFunc)FineBound01Thread, NULL );
	g_thread_unref ( gth );
}

void
FineBound02 ( GtkWidget *widget, gpointer gdata ) {
	GuiData *gd = GetGuiData ();
}
