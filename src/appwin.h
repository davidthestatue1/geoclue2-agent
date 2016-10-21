#include "app.h"
#include <gtk/gtk.h>


#define GEOCLUE_AGENT_APP_WINDOW_TYPE (geoclue_agent_app_window_get_type ())
G_DECLARE_FINAL_TYPE (GeoClueAgentAppWindow, geoclue_agent_app_window, GEOCLUE_AGENT, APP_WINDOW, GtkApplicationWindow)

struct _GeoClueAgentAppWindow {
	GtkApplicationWindow parent;
};

GeoClueAgentAppWindow *geoclue_agent_app_window_new (GeoClueAgentApp *app);
void geoclue_agent_app_window_about_dialog_open (GeoClueAgentApp *app);
void geoclue_agent_app_window_destroy_window(GeoClueAgentAppWindow *self);
