#include "app.h"
#include "appwin.h"

static const gchar *geoclue_agent_app_window_credits_authors[] = {
	"koko-ng <koko.fr.mu\@gmail.com>",
	NULL
};

typedef struct
{
	GtkApplicationWindow *parent;
} GeoClueAgentAppWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GeoClueAgentAppWindow, geoclue_agent_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void
geoclue_agent_app_window_init (GeoClueAgentAppWindow *app)
{
}

static void
geoclue_agent_app_window_class_init (GeoClueAgentAppWindowClass *class)
{
}

GeoClueAgentAppWindow *
geoclue_agent_app_window_new (GeoClueAgentApp *app)
{
	return g_object_new (GEOCLUE_AGENT_APP_WINDOW_TYPE,
	                     "application", app,
	                     "type", GTK_WINDOW_TOPLEVEL,
	                     NULL);
}
void geoclue_agent_app_window_about_dialog_open(GeoClueAgentApp *app) {
	gtk_show_about_dialog (g_list_nth_data (gtk_application_get_windows (GTK_APPLICATION (app)), 0),
                       "program-name", _("GeoClue Agent"),
                       "title", _("About GeoClue Agent"),
                       "authors", geoclue_agent_app_window_credits_authors,
                       NULL);
}
void geoclue_agent_app_window_destroy_window(GeoClueAgentAppWindow *self) {
	gtk_widget_destroy(GTK_WIDGET(self));
}
