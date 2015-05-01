# What?

This is a quick hack to improve the readability of thin fonts in Evince.
The idea is to dynamically link custom effects over the inverted color mode.

	$ make
	$ LD_PRELOAD=./contrastinator.so evince /tmp/paper.pdf

# Why?

It was created, because accessibility is important and should not be neglected.

# Who?

It was written by Sampsa "Tuplanolla" Kiiskinen.

# When?

It was slapped together on 2014-08-01.

# How?

Its development was trivial.

	$ cd evince
	$ find . -name "*.c*" > sources.list
	$ cat sources.list | xargs grep invert
	./libview/ev-view-presentation.c:	gboolean               inverted_colors;
	./libview/ev-view-presentation.c:	if (pview->inverted_colors)
	./libview/ev-view-presentation.c:		ev_document_misc_invert_surface (job_render->surface);
	$ cat sources.list | xargs grep ev_document_misc_invert_surface
	./libview/ev-view-presentation.c:		ev_document_misc_invert_surface (job_render->surface);
	./libview/ev-pixbuf-cache.c:		ev_document_misc_invert_surface (job_info->surface);
	./libview/ev-pixbuf-cache.c:			ev_document_misc_invert_surface (job_info->surface);
	./libview/ev-pixbuf-cache.c:			ev_document_misc_invert_surface (job_info->surface);
	./libview/ev-pixbuf-cache.c:			ev_document_misc_invert_surface (job_info->surface);
	./shell/ev-sidebar-thumbnails.c:		ev_document_misc_invert_surface (surface);
	./libdocument/ev-document-misc.c:ev_document_misc_invert_surface (cairo_surface_t *surface) {
	$ grep -C 1 ev_document_misc_invert_surface ./libdocument/ev-document-misc.c
	void
	ev_document_misc_invert_surface (cairo_surface_t *surface) {
		cairo_t *cr;
	$ gdb -q --args evince /tmp/paper.pdf
	Reading symbols from /usr/bin/evince...(no debugging symbols found)...done.
	(gdb) b ev_document_misc_invert_surface
	Function "ev_document_misc_invert_surface" not defined.
	Make breakpoint pending on future shared library load? (y or [n]) y

	Breakpoint 1 (ev_document_misc_invert_surface) pending.
	(gdb) r
	Starting program: /usr/bin/evince /tmp/paper.pdf
	warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
	[Thread debugging using libthread_db enabled]
	Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
	[New Thread 0x7fffeb973700 (LWP 1341)]
	[New Thread 0x7fffeb172700 (LWP 1342)]
	[New Thread 0x7fffe3bef700 (LWP 1343)]
	[New Thread 0x7fffe33ee700 (LWP 1344)]

	Breakpoint 1, 0x00007ffff779a550 in ev_document_misc_invert_surface ()
	   from /usr/lib/libevdocument3.so.4
	(gdb) q
	A debugging session is active.

		Inferior 1 [process 1337] will be killed.

	Quit anyway? (y or n) y

# Really?

It does not work with GTK+ 3 or AppArmor.
One day it might, but I would not count on it.
