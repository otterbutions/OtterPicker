NAME=					otterpicker
SRC=					otterpicker.c

CC=						cc
CC_COMMAND=		$(CC) -o $(NAME) $(SRC) -I/usr/local/include/gtk-3.0 -I/usr/local/include/pango-1.0 -I/usr/local/include -I/usr/local/include/cairo -I/usr/local/include/gdk-pixbuf-2.0 -I/usr/local/include/at-spi2-atk/2.0 -I/usr/local/include/at-spi-2.0 -I/usr/local/include/atk-1.0 -I/usr/local/libdata/pkgconfig/../../include/dbus-1.0 -I/usr/local/libdata/pkgconfig/../../lib/dbus-1.0/include -I/usr/local/include/libepoll-shim -I/usr/local/include/fribidi -D_THREAD_SAFE -I/usr/local/include/pixman-1 -I/usr/local/include/harfbuzz -I/usr/local/include/freetype2 -I/usr/local/include/libpng16 -I/usr/local/include/gio-unix-2.0 -pthread -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -L/usr/local/lib -lgtk-3 -lgdk-3 -lz -lpangocairo-1.0 -lcairo-gobject -lgdk_pixbuf-2.0 -latk-1.0 -lpango-1.0 -lcairo -lharfbuzz -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lintl

clean:
	rm -f $(NAME)

build:
	$(CC_COMMAND)

run:
	$(CC_COMMAND)
	./$(NAME)
	rm -f $(NAME)
