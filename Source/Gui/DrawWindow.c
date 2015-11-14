#include "../main.h"
gint fuck = 0;
void
DrawPic ( GuiData *gdata, cairo_t *cr, 
		cairo_surface_t *cur,
		gint width, gint height,
		gint imgwidth, gint imgheight ) { 
	if ( gdata->y > gdata->height ) gdata->y = gdata->height;
	gfloat x = gdata->x - gdata->zstartx;
	gfloat y = gdata->y - gdata->zstarty;

	gdata->startx = 0;
	gdata->starty = 0;
	gdata->scale = (float)width/imgwidth;
	gdata->starty = ( height/gdata->scale - imgheight ) / 2;
	if ( gdata->scale > (float)height/imgheight ) {
		gdata->scale = (float)height/imgheight;
		gdata->startx = ( width/gdata->scale - imgwidth ) / 2;
		gdata->starty = 0;
	}
	cairo_scale  (cr, gdata->scale, gdata->scale ); 
	cairo_set_source_surface (cr, cur, gdata->startx, gdata->starty );
	cairo_paint (cr);

	gint span = 20/gdata->scale;
	cairo_set_source_rgb ( cr, 0.6, 0.6, 0.6 );
	static const double dashed[] = {4.0, 2.0};
	cairo_set_dash ( cr, dashed, 2, 0 );
	cairo_set_line_width ( cr, 5 );
	cairo_move_to ( cr, gdata->startx + x - span, gdata->starty + y );
	cairo_line_to ( cr, gdata->startx + x + span, gdata->starty + y );
	cairo_move_to ( cr, gdata->startx + x, gdata->starty + y - span );
	cairo_line_to ( cr, gdata->startx + x, gdata->starty + y + span );
	cairo_stroke ( cr );

	cairo_set_source_rgb ( cr, 0, 0.6, 0.6 );
	cairo_arc ( cr, (gdata->startx + x), (gdata->starty + y), 6, 0, 2*M_PI );
	cairo_fill ( cr );
}
		
gboolean
DrawWindow ( GtkWidget *widget, cairo_t *cr, gpointer pdata ) {
	GuiData* gdata = GetGuiData ();
	gint width = gtk_widget_get_allocated_width (widget);
	gint height = gtk_widget_get_allocated_height (widget);

	if ( gdata->zoom < 1 ) {
		gint imgwidth = gdata->width * gdata->zoom;
		if ( imgwidth <= 10 ) imgwidth = 10;
		gint imgheight = gdata->height * gdata->zoom;
		if ( imgheight <= 10 ) imgheight = 10;
		cairo_surface_t *cur = cairo_image_surface_create ( 
					CAIRO_FORMAT_A8, imgwidth, imgheight );

		gint rowstride = cairo_image_surface_get_stride(cur);
		cairo_surface_flush ( cur );
		guchar *idata = cairo_image_surface_get_data ( gdata->cur );
		guchar *iidata = cairo_image_surface_get_data ( cur );
		// 放大左上角
		if ( gdata->y - imgheight/2 <= 0 
				&& gdata->x - imgwidth/2 <= 0 ) {
//			printf ( "Left Up%d\n", gdata->zstartx ); fflush ( stdout );
			gdata->zstartx = 0;
			gdata->zstarty = 0;
			for ( gint row=0; row<imgheight; row++ ) {
				for ( gint col=0; col<imgwidth; col++ ) {
					iidata[row*rowstride+col] = idata[row*gdata->stride+col];
				}
			}
		// 放大左下角
		} else if ( gdata->y + imgheight/2 >= gdata->height
				&& gdata->x - imgwidth/2 <= 0 ) {
//			printf ( "Left Down\n" ); fflush ( stdout );
			gdata->zstartx = 0;
			gdata->zstarty = gdata->height-imgheight;
			for ( gint row=0; row<imgheight; row++ ) {
				for ( gint col=0; col<imgwidth; col++ ) {
					iidata[row*rowstride+col] = idata[(row+gdata->height-imgheight)*gdata->stride+col];
				}
			}
		// 放大右上角
		} else if ( gdata->y - imgheight/2 <= 0
				&& gdata->x + imgwidth/2 >= gdata->width ) {
//			printf ( "Right Up\n" ); fflush ( stdout );
			gdata->zstartx = gdata->width-imgwidth;
			gdata->zstarty = 0;
			for ( gint row=0; row<imgheight; row++ ) {
				for ( gint col=0; col<imgwidth; col++ ) {
					iidata[row*rowstride+col] = idata[row*gdata->stride+col+gdata->width-imgwidth];
				}
			}
		// 放大右下角
		} else if ( gdata->y + imgheight/2 >= gdata->height
				&& gdata->x + imgwidth/2 >= gdata->width ) {
//			printf ( "Right Down\n" ); fflush ( stdout );
			gdata->zstartx = gdata->width-imgwidth;
			gdata->zstarty = gdata->height-imgheight;
			for ( gint row=0; row<imgheight; row++ ) {
				for ( gint col=0; col<imgwidth; col++ ) {
					iidata[row*rowstride+col] = idata[(row+gdata->height-imgheight)*gdata->stride+col+gdata->width-imgwidth];
				}
			}
		// 放大左角
		} else if ( gdata->x - imgwidth/2 <= 0 ) {
//			printf ( "Left\n" ); fflush ( stdout );
			gdata->zstartx = 0;
			gdata->zstarty = gdata->y - imgheight/2;
			for ( gint row=0; row<imgheight; row++ ) {
				for ( gint col=0; col<imgwidth; col++ ) {
					iidata[row*rowstride+col] = idata[(row+gdata->y-imgheight/2)*gdata->stride+col];
				}
			}
		} else if ( gdata->x + imgwidth/2 >= gdata->width ) {
//			printf ( "Right\n" ); fflush ( stdout );
			gdata->zstartx = gdata->width-imgwidth;
			gdata->zstarty = gdata->y - imgheight/2;
			for ( gint row=0; row<imgheight; row++ ) {
				for ( gint col=0; col<imgwidth; col++ ) {
					iidata[row*rowstride+col] = idata[(row+gdata->y-imgheight/2)*gdata->stride+col+gdata->width-imgwidth];
				}
			}
		} else if ( gdata->y - imgheight/2 <= 0 ) {
//			printf ( "Top\n" ); fflush ( stdout );
			gdata->zstartx = gdata->x-imgwidth/2;
			gdata->zstarty = 0;
			for ( gint row=0; row<imgheight; row++ ) {
				for ( gint col=0; col<imgwidth; col++ ) {
					iidata[row*rowstride+col] = idata[row*gdata->stride+col+gdata->x-imgwidth/2];
				}
			}
		} else if ( gdata->y + imgheight/2 >= gdata->height ) {
//			printf ( "Bottom\n" ); fflush ( stdout );
			gdata->zstartx = gdata->x-imgwidth/2;
			gdata->zstarty = gdata->height - imgheight;
			for ( gint row=0; row<imgheight; row++ ) {
				for ( gint col=0; col<imgwidth; col++ ) {
					iidata[row*rowstride+col] = idata[(row+gdata->height-imgheight)*gdata->stride+col+gdata->x-imgwidth/2];
				}
			}
		} else {
//			printf ( "Center\n" ); fflush ( stdout );
			gdata->zstartx = gdata->x-imgwidth/2;
			gdata->zstarty = gdata->y-imgheight/2;
			for ( gint row=0; row<imgheight; row++ ) {
				for ( gint col=0; col<imgwidth; col++ ) {
					iidata[row*rowstride+col] = idata[(row+gdata->y-imgheight/2)*gdata->stride+col+gdata->x-imgwidth/2];
				}
			}
		}

		cairo_surface_mark_dirty ( cur );
		DrawPic ( gdata, cr, cur, width, height, imgwidth, imgheight );
		cairo_surface_destroy ( cur );
	} else {
		gdata->zstartx = 0;
		gdata->zstarty = 0;
		DrawPic ( gdata, cr, gdata->cur, width, height, gdata->width, gdata->height );
	}
}
