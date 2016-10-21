#include "app.h"
#include "appwin.h"
typedef struct
{
	GList 		*accuracy_levels;
	GSettings 	*gsettings;
	GCancellable 	*cancellable;
	GVariantDict 	*remembered_apps;

	GtkApplication 	 parent;
} GeoClueAgentAppPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GeoClueAgentApp, geoclue_agent_app, GTK_TYPE_APPLICATION);

static void
geoclue_agent_app_display_about(GSimpleAction *action,
				GVariant      *parameter,
				gpointer       user_data) {
	geoclue_agent_app_window_about_dialog_open((GeoClueAgentApp*) user_data);
}

const GActionEntry app_actions[] = {
	{ "about", geoclue_agent_app_display_about }
};

static void
geoclue_agent_app_init (GeoClueAgentApp *app)
{
	GeoClueAgentAppPrivate *priv;

	priv = geoclue_agent_app_get_instance_private (app);

	priv->accuracy_levels=g_list_append (priv->accuracy_levels, GINT_TO_POINTER(GCLUE_ACCURACY_LEVEL_NONE));
	priv->accuracy_levels=g_list_append (priv->accuracy_levels, GINT_TO_POINTER(GCLUE_ACCURACY_LEVEL_COUNTRY));
	priv->accuracy_levels=g_list_append (priv->accuracy_levels, GINT_TO_POINTER(GCLUE_ACCURACY_LEVEL_CITY));
	priv->accuracy_levels=g_list_append (priv->accuracy_levels, GINT_TO_POINTER(GCLUE_ACCURACY_LEVEL_NEIGHBORHOOD));
	priv->accuracy_levels=g_list_append (priv->accuracy_levels, GINT_TO_POINTER(GCLUE_ACCURACY_LEVEL_STREET));
	priv->accuracy_levels=g_list_append (priv->accuracy_levels, GINT_TO_POINTER(GCLUE_ACCURACY_LEVEL_EXACT));

	priv->cancellable = g_cancellable_new ();

	priv->gsettings = g_settings_new ("io.github.koko-ng.gclue-agent");

}

static void
geoclue_agent_app_class_init (GeoClueAgentAppClass *class)
{/*
	GObjectClass *gobject_class;
	GApplicationClass *application_class;

	gobject_class = G_OBJECT_CLASS (class);
	application_class = (GApplicationClass*) class;*/

}
void
geoclue_agent_app_post_register (GApplication *app) {
	GMenu *menu;

	// for the actions, we use the name "about"
	g_action_map_add_action_entries (G_ACTION_MAP (app), app_actions, G_N_ELEMENTS (app_actions), app);

	// from the menu, we refer to the action as "app.about" since we installed it on the application
	menu = g_menu_new ();
	g_menu_append (menu, "About", "app.about");

	gtk_application_set_app_menu (GTK_APPLICATION (app),
	                              G_MENU_MODEL (menu));
	g_object_unref (menu);
}
void
geoclue_agent_load_remebered_apps (GeoClueAgentAppPrivate *priv) {
	priv->remembered_apps = g_variant_dict_new(g_settings_get_value(priv->gsettings, "remembered-apps"));
}
GeoClueAgentApp*
geoclue_agent_app_new (void) {
	return g_object_new (GEOCLUE_AGENT_APP_TYPE,
		             NULL);
}

guint
geoclue_agent_app_get_accuracy_level_uint(GeoClueAgentApp *self, gint accuracy) {
	GeoClueAgentAppPrivate *priv;

	priv = geoclue_agent_app_get_instance_private (self);
	return GPOINTER_TO_UINT (g_list_nth_data (priv->accuracy_levels, accuracy));
}
guint
geoclue_agent_app_get_accuracy_level_index(GeoClueAgentApp *self, gint accuracy) {
	GeoClueAgentAppPrivate *priv;

	priv = geoclue_agent_app_get_instance_private (self);
	return g_list_index (priv->accuracy_levels, GINT_TO_POINTER(accuracy));
}
GVariantDict*
geoclue_agent_app_get_remembered_apps(GeoClueAgentApp *self) {
	GeoClueAgentAppPrivate *priv;

	priv = geoclue_agent_app_get_instance_private (self);
	if(priv->remembered_apps==NULL)
		geoclue_agent_load_remebered_apps (priv);
	return priv->remembered_apps;
}
void
geoclue_agent_app_remember_app(GeoClueAgentApp *self, const gchar *desktop_id, GVariant *accuracy_level) {
	GeoClueAgentAppPrivate *priv;

	priv = geoclue_agent_app_get_instance_private (self);
	g_variant_dict_insert_value(geoclue_agent_app_get_remembered_apps(self),
						desktop_id,
						accuracy_level);
	g_settings_set_value(priv->gsettings, "remembered-apps", g_variant_dict_end(priv->remembered_apps));
	priv->remembered_apps=NULL;
}
GVariant*
geoclue_agent_app_remembered_accuracy(GeoClueAgentApp *self, const gchar *desktop_id) {

	return g_variant_dict_lookup_value (geoclue_agent_app_get_remembered_apps (self), desktop_id, G_VARIANT_TYPE_UINT32);
}
GCancellable*
geoclue_agent_app_get_cancellable(GeoClueAgentApp *self) {
	GeoClueAgentAppPrivate *priv;

	priv = geoclue_agent_app_get_instance_private (self);
	return priv->cancellable;
}
