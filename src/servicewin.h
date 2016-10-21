#include "service.h"
#include "appwin.h"
#include <gtk/gtk.h>


#define GEOCLUE_AGENT_SERVICE_WINDOW_TYPE (geoclue_agent_service_window_get_type ())
G_DECLARE_FINAL_TYPE (GeoClueAgentServiceWindow, geoclue_agent_service_window, GEOCLUE_AGENT, SERVICE_WINDOW, GtkApplicationWindow)//TODO subclass app window

struct _GeoClueAgentServiceWindow {
	GtkApplicationWindowClass parent_class;
};

GeoClueAgentServiceWindow *geoclue_agent_service_window_new (GeoClueAgentService *app);
void geoclue_agent_service_window_open (GeoClueAgentServiceWindow *win, const char *desktop_id, guint accuracy, GCallback allow_cb, GCallback deny_cb, GCallback cancel_cb, gpointer user_data);
void geoclue_agent_service_window_destroy_window(GeoClueAgentServiceWindow *self);
gboolean geoclue_agent_service_window_get_remember(GeoClueAgentServiceWindow *self);
guint geoclue_agent_service_window_combobox_get_active(GeoClueAgentServiceWindow *self);
