#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define COLORENTRY_TEXT gtk_entry_get_text(GTK_ENTRY(color.colorEntry))

void colorHander(void);
void initWidget(void);
void initCSS(void);
void win(void);
void cleanup(void);

typedef struct {
	GtkWidget *window, *grid;
} application;

typedef struct {
	GtkWidget *colorEntry, *colorBox;
} colorPicker;

typedef enum {
	APP_CSS,
	COLOR_CSS,
	TOTAL_CSS,
} css;

application app;
colorPicker color;
GtkCssProvider *stylesheets[TOTAL_CSS];

void colorHandler(void)
{
	GdkRGBA parse;

	// Checks if color is valid
	if(!gdk_rgba_parse(&parse, COLORENTRY_TEXT)) return;

	gtk_css_provider_load_from_data(stylesheets[COLOR_CSS],
		g_strdup_printf("#picked-color { background-color: \\%s }", COLORENTRY_TEXT), -1, NULL);

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(stylesheets[COLOR_CSS]), GTK_STYLE_PROVIDER_PRIORITY_USER);

	gtk_widget_queue_draw(color.colorBox);
}

void initWidget(void)
{
	// Create RGB input entry 
	color.colorEntry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(color.colorEntry), "RRGGBB");

	// Color box
	color.colorBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_name(color.colorBox, "picked-color");

	// Adding Everything to the grid
	gtk_grid_attach(GTK_GRID(app.grid), color.colorEntry, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(app.grid), color.colorBox, 1, 0, 1, 1);

	gtk_container_add(GTK_CONTAINER(app.window), app.grid);
}

void initCSS(void)
{
	// Initializes all stylesheets first
	int i;
	for(i = 0; i < TOTAL_CSS; i++)
	{
		stylesheets[i] = gtk_css_provider_new();
	}

	gtk_css_provider_load_from_data(stylesheets[APP_CSS],
		 "entry {"
		 "	margin: 0.4rem;"
		 "}"
		 "#picked-color {"
		 "	border: 1px solid;"
		 "	border-color: black white white black;"
		 "	padding: 1rem;"
		 "	margin: 0.4rem;"
		 "}", -1, NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(stylesheets[APP_CSS]), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void cleanup(void)
{
	int i;
	for (i = 0; i < TOTAL_CSS; i++) g_object_unref(stylesheets[i]);
}

void win(void)
{
	gtk_init(0, 0);
	app.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(app.window), "OtterPicker");
	gtk_window_set_resizable(GTK_WINDOW(app.window), 0);

	app.grid = gtk_grid_new();
	g_signal_connect(G_OBJECT(app.window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	initCSS();
	initWidget();

	g_signal_connect(color.colorEntry, "changed", G_CALLBACK(colorHandler), NULL);

	gtk_widget_show_all(app.window);
	gtk_main();

	// On exit
	cleanup();
}

int main(void)
{
	win();

	return 0;
}
