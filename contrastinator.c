#include <cairo.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>

static void (* dlev)(cairo_surface_t*) = NULL;

static void init(void) {
	dlev = dlsym(RTLD_NEXT,
			/* libevdocument3.so: */ "ev_document_misc_invert_surface");
	if (dlev == NULL) {
		fputs(dlerror(), stderr);
		fputc('\n', stderr);
	}
}

void ev_document_misc_invert_surface(cairo_surface_t* const surface) {
	if (dlev == NULL)
		init();

	/* dlev(surface); */ {
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
