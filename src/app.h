#ifndef __APP_H
#define __APP_H
#define _GNU_SOURCE

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <libintl.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <gclue-enums.h>
#include <libintl.h>
#include <signal.h>
#include <gio/gdesktopappinfo.h>
#include <gio/gio.h>

#define _(STRING) gettext(STRING)


#define GEOCLUE_AGENT_APP_TYPE (geoclue_agent_app_get_type())
G_DECLARE_DERIVABLE_TYPE (GeoClueAgentApp, geoclue_agent_app, GEOCLUE_AGENT, APP, GtkApplication)

struct _GeoClueAgentAppClass {
	GtkApplicationClass parent_class;
};

GeoClueAgentApp * geoclue_agent_app_new (void);
guint geoclue_agent_app_get_accuracy_level_uint(GeoClueAgentApp *self, gint accuracy);
guint geoclue_agent_app_get_accuracy_level_index(GeoClueAgentApp *self, gint accuracy);
GVariantDict* geoclue_agent_app_get_remembered_apps(GeoClueAgentApp *self);
void geoclue_agent_app_remember_app(GeoClueAgentApp *self, const gchar *desktop_id, GVariant *accuracy_level);
GVariant * geoclue_agent_app_remembered_accuracy(GeoClueAgentApp *self, const gchar *desktop_id);
GCancellable * geoclue_agent_app_get_cancellable(GeoClueAgentApp *self);
void geoclue_agent_app_post_register(GApplication *application);

#endif /* __APP_H */


