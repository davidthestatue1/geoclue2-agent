#include "service.h"
#include "servicewin.h"

typedef struct
{
	GtkApplicationWindow *parent;
	GtkComboBox *combobox;
	GtkToggleButton *check_button;
} GeoClueAgentServiceWindowPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GeoClueAgentServiceWindow, geoclue_agent_service_window, GTK_TYPE_APPLICATION_WINDOW);

static void
geoclue_agent_service_window_init (GeoClueAgentServiceWindow *app)
{
}

static void
geoclue_agent_service_window_class_init (GeoClueAgentServiceWindowClass *class)
{
}

GeoClueAgentServiceWindow *
geoclue_agent_service_window_new (GeoClueAgentService *app)
{
	return g_object_new (GEOCLUE_AGENT_SERVICE_WINDOW_TYPE,
	                     "application", app,
	                     "type", GTK_WINDOW_TOPLEVEL,
	                     NULL);
}

void
geoclue_agent_service_window_open (GeoClueAgentServiceWindow 	*win,
                                   const char 			*desktop_id,
				   guint			accuracy,
				   GCallback			allow_cb,
				   GCallback			deny_cb,
				   GCallback			cancel_cb,
				   gpointer			user_data)
{
	printf("window opening\n");
	GeoClueAgentServiceWindowPrivate *priv;
	GtkWidget *cancel_button = NULL;
	GtkWidget *allow_button = NULL;
	GtkWidget *main_hbox = NULL;
	GtkWidget *content_vbox = NULL;
	GtkWidget *accuracy_hbox = NULL;
	GtkWidget *buttons_hbox = NULL;
	GtkWidget *textbox = NULL;
	GtkWidget *app_icon = NULL;
	GtkWidget *intro_label = NULL;
	GtkWidget *accuracy_label = NULL;
	GtkWidget *remember = NULL;
	GtkComboBoxText *combobox = NULL;
	GDesktopAppInfo *desktop_app_info = g_desktop_app_info_new (desktop_id);
	GKeyFile *desktop_keyfile = g_key_file_new();


	priv = geoclue_agent_service_window_get_instance_private (win);
	g_key_file_load_from_file(desktop_keyfile,
				  g_desktop_app_info_get_filename(desktop_app_info),
		  		  G_KEY_FILE_NONE,
				  NULL);
	gchar *icon_name = g_key_file_get_string(desktop_keyfile, "Desktop Entry", "Icon", NULL);
	//TODO get good size
	GdkPixbuf *icon_buf = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (),
				    icon_name,
                                    72,
                                    GTK_ICON_LOOKUP_FORCE_SIZE,
                                    NULL);

	/* Create the main window */
	gtk_container_set_border_width (GTK_CONTAINER (win), 18);
	gtk_window_set_title (GTK_WINDOW (win), _("Localisation access"));
	gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable (GTK_WINDOW(win), FALSE);
	gtk_widget_realize (GTK_WIDGET(win));

	main_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 24);
	gtk_container_add (GTK_CONTAINER (win), main_hbox);

	app_icon = gtk_image_new_from_pixbuf(icon_buf);
	gtk_box_pack_start(GTK_BOX(main_hbox), app_icon, FALSE, FALSE, 0);

	content_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
	gtk_container_add(GTK_CONTAINER(main_hbox), content_vbox);

	intro_label = gtk_label_new (g_strdup_printf(_("Application \'\'%s\'\' is trying to access to your localisation"), g_key_file_get_locale_string(desktop_keyfile, "Desktop Entry", "Name", NULL, NULL)));
	gtk_box_pack_start(GTK_BOX(content_vbox), intro_label, TRUE, TRUE, 0);

	accuracy_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_box_set_homogeneous(GTK_BOX(accuracy_hbox), FALSE);
	gtk_container_add(GTK_CONTAINER(content_vbox), accuracy_hbox);

	app_icon =gtk_image_new_from_icon_name("find-location-symbolic", GTK_ICON_SIZE_BUTTON);
	gtk_box_pack_start(GTK_BOX(accuracy_hbox), app_icon, FALSE, FALSE, 0);

	accuracy_label = gtk_label_new (_("Accuracy: "));
	gtk_box_pack_start(GTK_BOX(accuracy_hbox), accuracy_label, FALSE, FALSE, 0);

	combobox = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
	gtk_combo_box_text_append_text(combobox,
				  _("None")); //GCLUE_ACCURACY_LEVEL_NONE
	gtk_combo_box_text_append_text(combobox,
				  _("Country"));//GCLUE_ACCURACY_LEVEL_COUNTRY
	gtk_combo_box_text_append_text(combobox,
				  _("City"));//GCLUE_ACCURACY_LEVEL_CITY
	gtk_combo_box_text_append_text(combobox,
				  _("Neighborhood"));//GCLUE_ACCURACY_LEVEL_NEIGHBORHOOD
	gtk_combo_box_text_append_text(combobox,
				  _("Street"));//GCLUE_ACCURACY_LEVEL_STREET
	gtk_combo_box_text_append_text(combobox,
				  _("Exact"));//GCLUE_ACCURACY_LEVEL_EXACT$
	gtk_combo_box_set_active(GTK_COMBO_BOX(combobox), accuracy); //TODO set on previously granted accuracy ?
	gtk_box_pack_start (GTK_BOX (accuracy_hbox), GTK_WIDGET(combobox), TRUE, TRUE, 0);

	remember = gtk_check_button_new_with_label (_("Remember my choice"));
	gtk_box_pack_start (GTK_BOX (content_vbox), remember, TRUE, TRUE, 0);

	buttons_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_container_add(GTK_CONTAINER(content_vbox), buttons_hbox);
	/* Create a horizontal box with buttons */

	cancel_button = gtk_button_new_with_label (_("Refuse"));
	gtk_box_pack_start (GTK_BOX (buttons_hbox), cancel_button, TRUE, TRUE, 0);

	allow_button = gtk_button_new_with_label (_("Allow"));
	gtk_box_pack_start (GTK_BOX (buttons_hbox), allow_button, TRUE, TRUE, 0);

	gtk_widget_grab_focus(cancel_button);

	priv->combobox = GTK_COMBO_BOX(combobox);
	priv->check_button = GTK_TOGGLE_BUTTON(remember);

	g_signal_connect (cancel_button, "clicked", deny_cb, user_data);
	g_signal_connect (allow_button, "clicked", allow_cb, user_data);
	g_signal_connect (win, "delete_event", cancel_cb, user_data);
  	printf("showing widget\n");
	/* Enter the main loop */
	gtk_widget_show_all (GTK_WIDGET(win));
}
void geoclue_agent_service_window_destroy_window(GeoClueAgentServiceWindow *self) {
	gtk_widget_destroy(GTK_WIDGET(self));
	printf("destoryed\n");
}
gboolean geoclue_agent_service_window_get_remember(GeoClueAgentServiceWindow *self) {
	GeoClueAgentServiceWindowPrivate *priv;

	priv = geoclue_agent_service_window_get_instance_private (self);
	return gtk_toggle_button_get_active(priv->check_button);
}
guint geoclue_agent_service_window_combobox_get_active(GeoClueAgentServiceWindow *self) {
	GeoClueAgentServiceWindowPrivate *priv;

	priv = geoclue_agent_service_window_get_instance_private (self);
	return gtk_combo_box_get_active(priv->combobox);
}

