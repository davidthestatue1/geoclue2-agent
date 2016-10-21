#include "app.h"
#include "service.h"
#include "servicewin.h"
#include "../lib/geoclue-agent-interface.h"

GeoClueAgentService *app;

struct _GeoClueAgentService
{
	GeoClueAgentApp	parent;
	gboolean 	skeleton_exported;
};

G_DEFINE_TYPE(GeoClueAgentService, geoclue_agent_service, GEOCLUE_AGENT_APP_TYPE);

struct DialogData{
	const gchar			*desktop_id;
	GDBusMethodInvocation 		*authorizeAppMethodInvocation;
	GeoClueAgentServiceWindow 	*window;
	gboolean 			 authorized;
	guint 				*asked_accuracy;
	GAsyncQueue			*queue;
};
struct DialogData*
dialog_data_new() {
	struct DialogData *data;
	data = g_slice_new0(struct DialogData);
	data->authorized=FALSE;
	return data;
}

static void
geoclue_agent_service_init (GeoClueAgentService *app)
{
	app->skeleton_exported=FALSE;
}

gboolean
geoclue_agent_service_finalize_access(struct DialogData  *data,
                               GVariant          **return_value) {
	printf("42\n");
	g_dbus_method_invocation_return_value (data->authorizeAppMethodInvocation, g_variant_new_tuple(return_value ,2));
	if(data->queue)
		g_async_queue_push (data->queue, GINT_TO_POINTER(TRUE));
	g_debug (_("Application %s granted to access to location with an accuracy of %i"), data->desktop_id, g_variant_get_uint32(return_value[1]));
	g_slice_free(struct DialogData, data);
	return TRUE;
}
gboolean
geoclue_agent_service_finalize_access_window(struct DialogData *data) {
	printf("48\n");
	GVariant **return_value = g_new(GVariant *, 2);
	guint accuracy_level = geoclue_agent_app_get_accuracy_level_uint (&app->parent, geoclue_agent_service_window_combobox_get_active (data->window));
	printf("51\n");
	if(data->authorized) {
		return_value[0] = g_variant_new_boolean (TRUE);
		return_value[1] = g_variant_new_uint32 (accuracy_level);
	}
	else {
		return_value[0] = g_variant_new_boolean (FALSE);
		return_value[1] = g_variant_new_uint32 (GCLUE_ACCURACY_LEVEL_NONE);
	}
	if(geoclue_agent_service_window_get_remember (data->window)) {
		geoclue_agent_app_remember_app (&app->parent, data->desktop_id, return_value[1]);
	}
	printf("63\n");
	geoclue_agent_service_window_destroy_window (data->window);
	geoclue_agent_service_finalize_access (data, return_value);
	return TRUE;
}
static gboolean
geoclue_agent_service_refuse_access(GtkButton	*button,
                                    gpointer	 user_data) {
	struct DialogData *data = user_data;
	data->authorized=FALSE;
	return geoclue_agent_service_finalize_access_window (data);
}
static gboolean
geoclue_agent_service_refuse_access_window(GtkButton	*button,
				    GdkEvent		*event,
                                    gpointer		 user_data) {
	return geoclue_agent_service_refuse_access (button, user_data);
}
static gboolean
geoclue_agent_service_allow_access(GtkButton	*button,
                                    gpointer	 user_data) {
	struct DialogData *data = user_data;
	data->authorized=TRUE;
	printf("86\n");
	return geoclue_agent_service_finalize_access_window (data);
}
static gboolean
geoclue_agent_service_display_authorize_window(struct DialogData	*data) {
	printf("87\n");
  	data->window = geoclue_agent_service_window_new(app);
	geoclue_agent_service_window_open(data->window,
	                                  g_string_free (g_string_append (g_string_new (data->desktop_id), ".desktop"), FALSE),
	                                  geoclue_agent_app_get_accuracy_level_index (&app->parent, *data->asked_accuracy),
					  G_CALLBACK (geoclue_agent_service_allow_access),
					  G_CALLBACK (geoclue_agent_service_refuse_access),
					  G_CALLBACK (geoclue_agent_service_refuse_access_window),
					  (gpointer) data);
	//has been handeled
	printf("97\n");
	return TRUE;
}
static gboolean
geoclue_agent_service_authorize(
		GDBusConnection		*agent,
		GDBusMethodInvocation	*invocation,
		const gchar		*desktop_id,
		guint			 accuracy_level) {
	g_debug (_("Application %s asked for location access with accuracy of %i"), desktop_id, accuracy_level);

	struct DialogData *data = dialog_data_new();

	data->desktop_id = g_strdup (desktop_id);
	data->authorizeAppMethodInvocation = invocation;
	data->asked_accuracy = &accuracy_level;

	GVariant *remembered_accuracy = geoclue_agent_app_remembered_accuracy (&app->parent, data->desktop_id);

	g_free ((gpointer) desktop_id);

	if(remembered_accuracy!=NULL) {
		GVariant **return_value = g_new (GVariant *, 2);
		return_value[1] = remembered_accuracy;
		if(g_variant_get_uint32(return_value[1])>0)
			return_value[0]=g_variant_new_boolean (TRUE);
		else
			return_value[0]=g_variant_new_boolean (FALSE);
		g_variant_unref(remembered_accuracy);
		geoclue_agent_service_finalize_access (data, return_value);
	}
	else {
		//data->queue = g_async_queue_new ();
		geoclue_agent_service_display_authorize_window (data);
		//return *(gboolean*) g_async_queue_pop (data->queue);
	}
	printf("129\n");
	//will/has been handeled
	return TRUE;
}
static void geoclue_agent_service_registered(GDBusProxy		*proxy,
                                             GAsyncResult	*res,
                                             gpointer		 user_data) {
	GVariant *result;
	GError *error;
	printf("finishing\n");
	error = NULL;
	result = g_dbus_proxy_call_finish (proxy,
	                                   res,
	                                   &error);
	g_assert_no_error (error);
	if(result==NULL) {
		g_printerr (_("error while registering as agent"));
		g_printerr ("%s", error->message);
		return;
	}
	g_variant_unref (result);
	GClueAgent *skeleton = (GClueAgent *) user_data;
	g_signal_connect (skeleton,
                          "handle_authorize_app",
                           G_CALLBACK (geoclue_agent_service_authorize),
                           NULL);

	gclue_agent_set_max_accuracy_level (skeleton, GCLUE_ACCURACY_LEVEL_EXACT);
	g_debug (_("Agent registered"));
}
static void geoclue_agent_service_on_name(
		GDBusConnection *conn,
		const gchar     *name,
		const gchar     *name_owner,
		gpointer         user_data) {
	GError *error=NULL;
	GDBusConnection *connection = g_bus_get_sync (G_BUS_TYPE_SYSTEM,
						      geoclue_agent_app_get_cancellable(&app->parent),
						      &error);

	GClueAgent *skeleton=gclue_agent_skeleton_new ();

	if (!g_dbus_interface_skeleton_export (&GCLUE_AGENT_SKELETON(skeleton)->parent_instance,
                                               connection,
                                               "/org/freedesktop/GeoClue2/Agent",
                                               &error)) {
		g_printerr (_("error while exporting the agent"));
		g_printerr ("%s", error->message);
		return;
	}
	else
		app->skeleton_exported=TRUE;
	GDBusProxy *manager=g_dbus_proxy_new_for_bus_sync(
		G_BUS_TYPE_SYSTEM,
		G_DBUS_PROXY_FLAGS_NONE,
		NULL,
		"org.freedesktop.GeoClue2",
		"/org/freedesktop/GeoClue2/Manager",
		"org.freedesktop.GeoClue2.Manager",
		geoclue_agent_app_get_cancellable (&app->parent), &error);
	if(manager==NULL) {
		g_printerr (_("error while getting the manager"));
		g_printerr ("%s", error->message);
		return;
	}
	error=NULL;
	g_dbus_proxy_call(manager,
                         "AddAgent",
                         g_variant_new("(s)", "agent-test"),
                         G_DBUS_CALL_FLAGS_NONE,
                         -1,
                         geoclue_agent_app_get_cancellable(&app->parent),
			(GAsyncReadyCallback) geoclue_agent_service_registered,
                         skeleton);


}

static void
geoclue_agent_service_activate (GApplication *application)
{
	geoclue_agent_app_post_register (application);
	g_debug (_("adding agent…"));
	GCancellable *cancel=geoclue_agent_app_get_cancellable(GEOCLUE_AGENT_APP(application));
	app=GEOCLUE_AGENT_SERVICE(application);
	g_bus_watch_name(
		G_BUS_TYPE_SYSTEM,
		"org.freedesktop.GeoClue2",
		G_BUS_NAME_WATCHER_FLAGS_AUTO_START,
		geoclue_agent_service_on_name,
		NULL,
		cancel, NULL);
}
static void
geoclue_agent_service_class_init (GeoClueAgentServiceClass *class)
{
	G_APPLICATION_CLASS (class)->activate = geoclue_agent_service_activate;

}

GeoClueAgentService *
geoclue_agent_service_new (void)
{
	GeoClueAgentService *app = g_object_ref (g_object_new (GEOCLUE_AGENT_SERVICE_TYPE,
		             "application-id", "io.github.koko-ng.geoclue-agent.service",
		             "flags", G_APPLICATION_FLAGS_NONE,
	                      NULL));

	return app;
}
void
geoclue_agent_service_quit() {
	//if(app->skeleton_exported)
	//	g_dbus_interface_skeleton_unexport(&GCLUE_AGENT_SKELETON(gclue_agent_skeleton_new())->parent_instance);
	g_cancellable_cancel(geoclue_agent_app_get_cancellable(&app->parent));
	g_application_quit(G_APPLICATION(app)); //quits ignoring the hold count
}
int
main(int argc, char** argv) {
	signal(SIGINT, geoclue_agent_service_quit);
	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);
	GeoClueAgentService *appy = geoclue_agent_service_new();
	g_application_hold (G_APPLICATION(appy));
	return g_application_run (G_APPLICATION(appy), argc, argv);
}
