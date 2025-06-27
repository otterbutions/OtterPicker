#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define COLORENTRY_TEXT gtk_entry_get_text(GTK_ENTRY(color.colorEntry))

int setColor(const char* toChange);
int reverseAlg(void);
int grayscaleAlg(void);
void grayscaleHandler(void);
void colorHander(void);

void initWidget(void);
void initCSS(void);
void initSignal(void);
void win(void);
void cleanup(void);

typedef struct {
	GtkWidget *window, *grid;
} application;

typedef struct {
	GtkWidget *colorEntry, *grayButton, *reverseButton, *colorBox;
} colorPicker;

typedef enum {
	APP_CSS,
	COLOR_CSS,
	TOTAL_CSS,
} css;

application app;
colorPicker color;
GtkCssProvider *stylesheets[TOTAL_CSS];

int setColor(const char* toChange)
{
	GdkRGBA parse;

	// Checks if color is valid
	if(!gdk_rgba_parse(&parse, COLORENTRY_TEXT)) return 1;

	gtk_css_provider_load_from_data(stylesheets[COLOR_CSS],
		g_strdup_printf("#picked-color { background-color: %s }", toChange), -1, NULL);

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(stylesheets[COLOR_CSS]), GTK_STYLE_PROVIDER_PRIORITY_USER);
	gtk_entry_set_text(GTK_ENTRY(color.colorEntry), toChange);

	return 0;
}

int reverseAlg(void)
{
	unsigned int rgb = strtol((COLORENTRY_TEXT+1), NULL, 16);

	return (
		(0xFF - (rgb >> 16 & 0xFF)) << 16 | 
		(0xFF - (rgb >> 8 & 0xFF)) << 8 | 
		(0xFF - (rgb & 0xFF))
	);
}

int grayscaleAlg(void)
{
	unsigned int rgb = strtol((COLORENTRY_TEXT+1), NULL, 16);

	return (
		(rgb >> 16 & 0xFF) * 0.299 +
		(rgb >> 8 & 0xFF) * 0.587 + 
		(rgb & 0xFF) * 0.114
	);
}


void reverseHandler(void)
{
	GdkRGBA parse;
	char reverse[7];

	if(!gdk_rgba_parse(&parse, COLORENTRY_TEXT) || strlen(COLORENTRY_TEXT) != 7) return;

	sprintf(reverse, "#%x", reverseAlg());

	setColor(reverse);
}

void grayscaleHandler(void)
{
	GdkRGBA parse;
	char gray[7];
	unsigned int i;

	// Checks if color is valid
	if(!gdk_rgba_parse(&parse, COLORENTRY_TEXT) || strlen(COLORENTRY_TEXT) != 7) return;

	i = grayscaleAlg();
	sprintf(gray, "#%x%x%x", i, i, i);

	setColor(gray);
}

void colorHandler(void)
{

	if(setColor(COLORENTRY_TEXT)) return;
	gtk_widget_queue_draw(color.colorBox);
}

void initWidget(void)
{
	// Create RGB input entry 
	color.colorEntry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(color.colorEntry), "RRGGBB");

	// Create gray input entry 
	color.grayButton = gtk_button_new_with_label("Grayscale color");
	color.reverseButton = gtk_button_new_with_label("Reverse color");

	// Color box
	color.colorBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_name(color.colorBox, "picked-color");

	// Adding Everything to the grid
	gtk_grid_attach(GTK_GRID(app.grid), color.colorEntry, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(app.grid), color.colorBox, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(app.grid), color.grayButton, 0, 1, 2, 1);
	gtk_grid_attach(GTK_GRID(app.grid), color.reverseButton, 0, 2, 2, 1);

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

void initSignal()
{
	g_signal_connect(color.colorEntry, "changed", G_CALLBACK(colorHandler), NULL);
	g_signal_connect(color.grayButton, "clicked", G_CALLBACK(grayscaleHandler), NULL);
	g_signal_connect(color.reverseButton, "clicked", G_CALLBACK(reverseHandler), NULL);
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
	initSignal();

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
