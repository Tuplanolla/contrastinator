#include <cairo.h>
#include <dlfcn.h>
#include <gtk/gtk.h>
#include <stddef.h>
#include <stdio.h>

static void (* invert_surface)(cairo_surface_t*) = NULL;
static void (* invert_pixbuf)(GdkPixbuf*) = NULL;

static void init_surface(void) {
	invert_surface = dlsym(RTLD_NEXT,
			/* libevdocument3.so: */ "ev_document_misc_invert_surface");
	if (invert_surface == NULL) {
		fputs(dlerror(), stderr);
		fputc('\n', stderr);
	}
}

static void init_pixbuf(void) {
	invert_pixbuf = dlsym(RTLD_NEXT,
			/* libevdocument3.so: */ "ev_document_misc_invert_pixbuf");
	if (invert_pixbuf == NULL) {
		fputs(dlerror(), stderr);
		fputc('\n', stderr);
	}
}

void ev_document_misc_invert_surface(cairo_surface_t* const surface) {
	if (invert_surface == NULL)
		init_surface();

	/* invert_surface(surface); */ {
		cairo_t* context;

		context = cairo_create(surface);

		cairo_set_source_rgb(context, 1, 1, 1);
		cairo_set_operator(context, CAIRO_OPERATOR_DIFFERENCE);
		cairo_paint(context); /* This is the inversion. */

		cairo_set_source_surface(context, surface, 0, 0);
		cairo_set_operator(context, CAIRO_OPERATOR_SCREEN);
		cairo_paint(context); /* This is the enhancement. */

		cairo_destroy(context);
	}
}

void ev_document_misc_invert_pixbuf(GdkPixbuf* const pixbuf) {
	if (invert_pixbuf == NULL)
		init_pixbuf();

	/* invert_pixbuf(pixbuf); */ {
		guint n_channels;
		guchar* data;
		guint rowstride;
		guint width, height;
		guint x, y;

		n_channels = gdk_pixbuf_get_n_channels(pixbuf);
		g_assert(gdk_pixbuf_get_colorspace(pixbuf) == GDK_COLORSPACE_RGB);
		g_assert(gdk_pixbuf_get_bits_per_sample(pixbuf) == 8);

		data = gdk_pixbuf_get_pixels(pixbuf);

		rowstride = gdk_pixbuf_get_rowstride(pixbuf);

		width = gdk_pixbuf_get_width(pixbuf);
		height = gdk_pixbuf_get_height(pixbuf);
		for (x = 0;
				x < width;
				++x) {
			for (y = 0;
					y < height;
					++y) {
				guchar* p;

				p = data + x * n_channels + y * rowstride;

				/*
				p[0] = 255 - p[0];
				p[1] = 255 - p[1];
				p[2] = 255 - p[2];
				*/ /* This is the inversion. */

				p[0] = 255 - p[0] * p[0] / 255;
				p[1] = 255 - p[1] * p[1] / 255;
				p[2] = 255 - p[2] * p[2] / 255; /* This is the enhancement. */
			}
		}
	}
}
