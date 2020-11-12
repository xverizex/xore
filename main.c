#include <gtk/gtk.h>
#include <string.h>
#include "list.h"
#include "config.h"

const char *app_name = "com.xverizex.xore";
GtkApplication *app;
GNotification *notify;

struct widgets {
	GtkWidget *window_main;
	GtkWidget *box_main;
	GtkWidget *frame_tools;
	GtkWidget *box_tools;
	GtkWidget *image_t_source;
	GtkWidget *button_source;
	GtkWidget *image_t_data_item;
	GtkWidget *button_data_item;
	GtkWidget *drawing_area;
	GtkWidget *header_bar;
	GtkWidget *window_source_main;
	GtkWidget *box_source_main;
	GtkWidget *frame_source_source_main;
	GtkWidget *box_source_source_main;
	GtkWidget *label_source_source_main;
	GtkWidget *combo_box_source_source_main;
	GtkWidget *frame_mysql_source_main;
	GtkWidget *box_mysql_source_main;
	GtkWidget *box_login_mysql_source_main;
	GtkWidget *label_login_mysql_source_main;
	GtkWidget *entry_login_mysql_source_main;
	GtkWidget *box_password_mysql_source_main;
	GtkWidget *label_password_mysql_source_main;
	GtkWidget *entry_password_mysql_source_main;
	GtkWidget *box_host_mysql_source_main;
	GtkWidget *label_host_mysql_source_main;
	GtkWidget *entry_host_mysql_source_main;
	GtkWidget *box_port_mysql_source_main;
	GtkWidget *label_port_mysql_source_main;
	GtkWidget *entry_port_mysql_source_main;
	GtkWidget *box_db_mysql_source_main;
	GtkWidget *label_db_mysql_source_main;
	GtkWidget *entry_db_mysql_source_main;
	GtkWidget *box_buttons_end_source_main;
	GtkWidget *button_save_source_main;
	GtkWidget *window_data_item_main;
	GtkWidget *box_data_item_main;
	GtkWidget *frame_data_item_main;
	GtkWidget *box_item_data_item_main;
	GtkWidget *label_item_data_item_main;
	GtkWidget *entry_item_data_item_main;
	GtkWidget *box_buttons_end_data_item_main;
	GtkWidget *button_save_data_item_main;

	GtkWidget *STUBS;
} w;

struct list_xore *object;

struct list_xore *c_source_xore;

static struct list_xore *get_objects ( ) {
	if ( object == NULL ) object = calloc ( 1, sizeof ( struct list_xore ) );
	return object;
}

static void show_settings_source ( struct list_xore *l ) {
	c_source_xore = l;
	gtk_widget_show_all ( w.window_source_main );

	const char *item = gtk_combo_box_get_active_id ( ( GtkComboBox * ) w.combo_box_source_source_main );
	int id = atoi ( item );

	switch ( c_source_xore->data_type ) {
		case DATA_MYSQL:
			gtk_combo_box_set_active ( ( GtkComboBox * ) w.combo_box_source_source_main, 1 );
			if ( c_source_xore->data ) {
				struct source_mysql *m = ( struct source_mysql * ) c_source_xore->data;
				gtk_entry_set_text ( ( GtkEntry * ) w.entry_login_mysql_source_main, m->login );
				gtk_entry_set_text ( ( GtkEntry * ) w.entry_password_mysql_source_main, m->password );
				gtk_entry_set_text ( ( GtkEntry * ) w.entry_host_mysql_source_main, m->host );
				gtk_entry_set_text ( ( GtkEntry * ) w.entry_port_mysql_source_main, m->port );
				gtk_entry_set_text ( ( GtkEntry * ) w.entry_db_mysql_source_main, m->db );
			}
			gtk_widget_show ( w.frame_mysql_source_main );
			break;
		default:
			gtk_combo_box_set_active ( ( GtkComboBox * ) w.combo_box_source_source_main, 0 );
			gtk_widget_hide ( w.frame_mysql_source_main );
			break;
	}
}

static void show_settings_data_item ( struct list_xore *l ) {
	c_source_xore = l;

	switch ( c_source_xore->data_type ) {
		case DATA_ITEM:
			if ( c_source_xore->data ) {
				struct data_item *m = ( struct data_item * ) c_source_xore->data;
				if ( m->item ) {
					gtk_entry_set_text ( ( GtkEntry * ) w.entry_item_data_item_main, m->item );
				} else {
					gtk_entry_set_text ( ( GtkEntry * ) w.entry_item_data_item_main, "" );
				}
			} else {
				gtk_entry_set_text ( ( GtkEntry * ) w.entry_item_data_item_main, "" );
			}
			break;
	}

	gtk_widget_show_all ( w.window_data_item_main );
}

static void item_settings_row_activate_cb ( GtkMenuItem *item, gpointer data ) {
	struct list_xore *l = ( struct list_xore * ) data;
	switch ( l->type ) {
		case TYPE_IS_SOURCE: 
			show_settings_source ( l );
			break;
		case TYPE_IS_DATA_ITEM:
			show_settings_data_item ( l );
			break;

	}
}

static void show_menu_popup_source ( struct list_xore *l ) {
	c_source_xore = l;
	GtkWidget *gtk_menu = gtk_menu_new ( );
	GtkWidget *item_settings = gtk_menu_item_new_with_label ( "Настроить" );
	g_signal_connect ( item_settings, "activate", G_CALLBACK ( item_settings_row_activate_cb ), l );
	gtk_menu_shell_append ( ( GtkMenuShell * ) gtk_menu, item_settings );
	gtk_widget_show ( item_settings );

	gtk_menu_popup_at_pointer ( ( GtkMenu * ) gtk_menu, NULL );
}

static void show_menu_popup_data_item ( struct list_xore *l ) {
	c_source_xore = l;
	GtkWidget *gtk_menu = gtk_menu_new ( );
	GtkWidget *item_settings = gtk_menu_item_new_with_label ( "Настроить" );
	g_signal_connect ( item_settings, "activate", G_CALLBACK ( item_settings_row_activate_cb ), l );
	gtk_menu_shell_append ( ( GtkMenuShell * ) gtk_menu, item_settings );
	gtk_widget_show ( item_settings );

	gtk_menu_popup_at_pointer ( ( GtkMenu * ) gtk_menu, NULL );
}


GtkTargetEntry target = { "application/x-rootwindow-drop", 0, 0 };

const char *styles = "window#light { background-color: #ececec; } treeview#light { background-color: #ffffff; color: #000000; } treeview#light:selected  { background: #8c8c8c; color: #ffffff; } treeview#light header button { background: #ffffff; color: #000000; } entry#light { background-color: #ffffff; } label#light { color: #000000; } frame#light { border-radius: 8px; background-color: #dcdcdc; } textview#light.view text { background-color: #ffffff; } frame#light border { border-radius: 8px; } button#light.text-button { background: #ffffff; } notebook#light tab { background-color: #ececec; color: #ffffff; }";

static void set_theme ( const char *name ) {
	struct widgets **p = ( struct widgets ** ) &w;
	for ( int i = 0; p[i] != NULL; i++ ) {
		gtk_widget_set_name ( ( GtkWidget * ) p[i], name );
	}
}

int drag = 0;
int drag_x = 0;
int drag_y = 0;
int drag_image = 0;

static gboolean image_drag_motion_cb ( GtkWidget *widget,
		GdkDragContext *context,
		int x,
		int y,
		unsigned int time ) {
	const char *name = gtk_widget_get_name ( widget );

	return FALSE;
}

static gboolean drawing_area_drag_motion_cb ( GtkWidget *widget,
		GdkDragContext *context,
		int x,
		int y,
		unsigned int time ) {
	const char *name = gtk_widget_get_name ( widget );
	drag_x = x;
	drag_y = y;
	gtk_widget_queue_draw ( w.drawing_area );
	return FALSE;
}

static gboolean drawing_area_drag_drop_cb ( GtkWidget *widget,
		GdkDragContext *context,
		int x,
		int y,
		unsigned int time,
		gpointer data ) {
	const char *name = gtk_widget_get_name ( widget );
	struct list_xore *l;
	l = get_objects();
	while ( l->next ) l = l->next;
	switch ( drag_image ) {
		case TYPE_IS_SOURCE:
			l->type = TYPE_IS_SOURCE;
			l->file = IMAGE_XORE_SOURCE;
			l->open_menu = show_menu_popup_source;
			break;
		case TYPE_IS_DATA_ITEM:
			l->type = TYPE_IS_DATA_ITEM;
			l->file = IMAGE_XORE_DATA_ITEM;
			l->open_menu = show_menu_popup_data_item;
			break;
	}

	l->x = drag_x;
	l->y = drag_y;
	l->width = 64;
	l->height = 64;
	l->next = calloc ( 1, sizeof ( struct list_xore ) );
	drag = 0;

	return TRUE;
}

static void drawing_area_drag_data_received_cb ( GtkWidget *widget,
		GdkDragContext *context,
		int x,
		int y,
		GtkSelectionData *selection_data,
		unsigned int target_type,
		unsigned int time,
		gpointer data ) {
	const char *name = gtk_widget_get_name ( widget );
}

static void image_drag_begin_cb ( GtkWidget *widget, GdkDragContext *context, gpointer data ) {
	const char *name = gtk_widget_get_name ( widget );
	GtkWidget *image = gtk_button_get_image ( ( GtkButton * ) widget );
	if ( image == w.image_t_source ) {
		drag_image = TYPE_IS_SOURCE;
	}
	if( image == w.image_t_data_item ) {
		drag_image = TYPE_IS_DATA_ITEM;
	}

	drag = 1;
}


static void create_tools ( ) {
	w.image_t_source = gtk_image_new_from_file ( IMAGE_XORE_SOURCE );
	w.image_t_data_item = gtk_image_new_from_file ( IMAGE_XORE_DATA_ITEM );

	w.button_source = gtk_button_new ( );
	gtk_button_set_image ( ( GtkButton * ) w.button_source, w.image_t_source );

	w.button_data_item = gtk_button_new ( );
	gtk_button_set_image ( ( GtkButton * ) w.button_data_item, w.image_t_data_item );

	gtk_drag_source_set ( w.button_source, GDK_BUTTON1_MASK, &target, 1, GDK_ACTION_COPY );
	gtk_drag_source_set ( w.button_data_item, GDK_BUTTON1_MASK, &target, 1, GDK_ACTION_COPY );

	g_signal_connect ( w.button_source, "drag-motion", G_CALLBACK ( image_drag_motion_cb ), NULL );
	g_signal_connect ( w.button_source, "drag-begin", G_CALLBACK ( image_drag_begin_cb ), NULL );

	g_signal_connect ( w.button_data_item, "drag-motion", G_CALLBACK ( image_drag_motion_cb ), NULL );
	g_signal_connect ( w.button_data_item, "drag-begin", G_CALLBACK ( image_drag_begin_cb ), NULL );

	gtk_box_pack_start ( ( GtkBox * ) w.box_tools, w.button_source, FALSE, FALSE, 0 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_tools, w.button_data_item, FALSE, FALSE, 0 );
}

struct list_xore *l_current;

static gboolean drawing_area_motion_event_cb ( GtkWidget *widget, GdkEvent *event, gpointer data ) {

	if ( l_current ) {
		GdkEventMotion *ev = ( GdkEventMotion * ) event;
		int old_x = l_current->x;
		int old_y = l_current->y;
		int new_x = ev->x - l_current->sl_x;
		int new_y = ev->y - l_current->sl_y;
		struct list_xore *l = get_objects();
		int found = 0;
		int inc = 0;
		while ( l->next ) {
			if ( l == l_current ) {
				l = l->next;
				continue;
			}

			if ( new_x + l_current->width < l->x || new_x > l->x + l->width || new_y + l_current->height < l->y || new_y > l->y + l->height ) {
				found = 1;
			} else {
				found = 0;
				inc++;
				break;
			}

			l = l->next;
			inc++;
		}

		if ( !inc ) {
			l_current->x = new_x;
			l_current->y = new_y;
			gtk_widget_queue_draw ( w.drawing_area );
		}
		if ( found ) {
			l_current->x = new_x;
			l_current->y = new_y;
			gtk_widget_queue_draw ( w.drawing_area );
		}
	}

	return FALSE;
}

static gboolean drawing_area_button_release_event_cb ( GtkWidget *widget, GdkEvent *event, gpointer data ) {
	l_current = NULL;
}

static gboolean drawing_area_draw_cb ( GtkWidget *widget, cairo_t *cr, gpointer data ) {
	cairo_set_source_rgb ( cr,
			0.8,
			0.8,
			0.8
			);
	cairo_paint ( cr );

	if ( drag ) {
		int width, height;
		cairo_surface_t *surface;
		switch ( drag_image ) {
			case TYPE_IS_SOURCE: 
	       			surface = cairo_image_surface_create_from_png ( IMAGE_XORE_SOURCE );
				width = height = 64;
				break;
			case TYPE_IS_DATA_ITEM:
	       			surface = cairo_image_surface_create_from_png ( IMAGE_XORE_DATA_ITEM );
				width = height = 64;
				break;
		}

		cairo_set_source_surface ( cr, surface, drag_x, drag_y );
		cairo_paint ( cr );
		cairo_surface_destroy ( surface );
	}

	struct list_xore *xl = get_objects();

	while ( xl->next ) {
		if ( xl->file ) {
			cairo_surface_t *surface = cairo_image_surface_create_from_png ( xl->file );
			cairo_set_source_surface ( cr, surface, xl->x, xl->y );
			cairo_paint ( cr );

		}
		xl = xl->next;
	}

	return FALSE;
}


static gboolean drawing_area_button_press_event_cb ( GtkWidget *widget, GdkEvent *event, gpointer data ) {

	GdkEventButton *btn = ( GdkEventButton * ) event;

	struct list_xore *l = get_objects();
	l_current = NULL;

	while ( l->next ) {
		if ( l->file ) {
			if ( btn->x >= l->x && btn->x <= ( l->x + l->width ) ) {
				if ( btn->y >= l->y && btn->y <= ( l->y + l->height ) ) {
					if ( btn->button == 1 ) {
						l_current = l;
						l->sl_x = btn->x - l->x;
						l->sl_y = btn->y - l->y;
						break;
					}
					if ( btn->button == 3 ) {
						if ( l->open_menu == NULL ) {
							switch ( l->type ) {
								case TYPE_IS_SOURCE:
									l->open_menu = show_menu_popup_source;
									break;
								case TYPE_IS_DATA_ITEM:
									l->open_menu = show_menu_popup_data_item;
									break;
							}
						}
						l->open_menu ( l );
						break;
					}
				}
			}
		}
		l = l->next;
	}

	return FALSE;
}

static gboolean window_source_main_delete_event_cb ( GtkWidget *widget, gpointer data ) {
	gtk_widget_hide ( w.window_source_main );

	return TRUE;
}

static void combo_box_source_source_main_changed_cb ( GtkComboBox *widget, gpointer data ) {
	const char *item = gtk_combo_box_get_active_id ( widget );
	int id = atoi ( item );

	switch ( id ) {
		case 0: 
			gtk_widget_hide ( w.frame_mysql_source_main );
			break;
		case 1:
			gtk_entry_set_text ( ( GtkEntry * ) w.entry_login_mysql_source_main, "" );
			gtk_entry_set_text ( ( GtkEntry * ) w.entry_password_mysql_source_main, "" );
			gtk_entry_set_text ( ( GtkEntry * ) w.entry_host_mysql_source_main, "" );
			gtk_entry_set_text ( ( GtkEntry * ) w.entry_port_mysql_source_main, "" );
			gtk_entry_set_text ( ( GtkEntry * ) w.entry_db_mysql_source_main, "" );
			gtk_widget_show ( w.frame_mysql_source_main );
			break;
	}
}

static void check_and_save_mysql_source ( ) {
	const char *login = gtk_entry_get_text ( ( GtkEntry * ) w.entry_login_mysql_source_main );
	const char *password = gtk_entry_get_text ( ( GtkEntry * ) w.entry_password_mysql_source_main );
	const char *host = gtk_entry_get_text ( ( GtkEntry * ) w.entry_host_mysql_source_main );
	const char *port = gtk_entry_get_text ( ( GtkEntry * ) w.entry_port_mysql_source_main );
	const char *db = gtk_entry_get_text ( ( GtkEntry * ) w.entry_db_mysql_source_main );
	if ( strlen ( login ) == 0 || strlen ( password ) == 0 || strlen ( host ) == 0 || strlen ( port ) == 0 || strlen ( db ) == 0 ) {
		g_notification_set_body ( notify, "Должны быть указаны все поля" );
		g_application_send_notification ( ( GApplication * ) app, NULL, notify );
		return;
	}
#if 0
	int len_login = strlen ( login );
	int len_password = strlen ( password );
	int len_host = strlen ( host );
	int len_port = strlen ( port );
#endif

	struct source_mysql *sm = calloc ( 1, sizeof ( struct source_mysql ) );
	if ( !sm ) {
		g_notification_set_body ( notify, "Не удалось выделить память для mysql" );
		g_application_send_notification ( ( GApplication * ) app, NULL, notify );
		return;
	}

	sm->login = g_strdup ( login );
	sm->password = g_strdup ( password );
	sm->host = g_strdup ( host );
	sm->port = g_strdup ( port );
	sm->db = g_strdup ( db );

	c_source_xore->data = sm;
	c_source_xore->data_type = DATA_MYSQL;
}

static void button_save_source_main_clicked_cb ( GtkButton *button, gpointer data ) {
	const char *item = gtk_combo_box_get_active_id ( ( GtkComboBox * ) w.combo_box_source_source_main );
	int id = atoi ( item );

	switch ( id ) {
		case 0: 
			break;
		case 1:
			check_and_save_mysql_source ( );
			break;
	}
}

static void create_source_settings ( void ) {
	w.window_source_main = gtk_application_window_new ( app );
	gtk_window_set_default_size ( ( GtkWindow * ) w.window_source_main, 400, 600 );
	g_signal_connect ( w.window_source_main, "delete-event", G_CALLBACK ( window_source_main_delete_event_cb ), NULL );

	w.box_source_main = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 0 );
	w.frame_source_source_main = g_object_new ( GTK_TYPE_FRAME, "shadow-type", GTK_SHADOW_NONE, NULL );
	gtk_widget_set_margin_top ( w.frame_source_source_main, 16 );
	gtk_widget_set_margin_start ( w.frame_source_source_main, 32 );
	gtk_widget_set_margin_end ( w.frame_source_source_main, 32 );

	w.box_source_source_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	gtk_widget_set_margin_top ( w.box_source_source_main, 14 );
	gtk_widget_set_margin_bottom ( w.box_source_source_main, 14 );
	gtk_widget_set_margin_start ( w.box_source_source_main, 14 );
	gtk_widget_set_margin_end ( w.box_source_source_main, 14 );

	w.label_source_source_main = gtk_label_new ( "Источник" );

	w.combo_box_source_source_main = gtk_combo_box_text_new ( );
	gtk_combo_box_text_append ( ( GtkComboBoxText * ) w.combo_box_source_source_main, "0", "Ничего" );
	gtk_combo_box_text_append ( ( GtkComboBoxText * ) w.combo_box_source_source_main, "1", "Mysql" );
	gtk_combo_box_set_active ( ( GtkComboBox * ) w.combo_box_source_source_main, 0 );
	g_signal_connect ( w.combo_box_source_source_main, "changed", G_CALLBACK ( combo_box_source_source_main_changed_cb ), NULL );

	w.frame_mysql_source_main = g_object_new ( GTK_TYPE_FRAME, "shadow-type", GTK_SHADOW_NONE, NULL );
	gtk_widget_set_margin_top ( w.frame_mysql_source_main, 16 );
	gtk_widget_set_margin_start ( w.frame_mysql_source_main, 32 );
	gtk_widget_set_margin_end ( w.frame_mysql_source_main, 32 );
	w.box_mysql_source_main = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 0 );
	w.box_login_mysql_source_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	w.label_login_mysql_source_main = gtk_label_new ( "Логин" );
	w.entry_login_mysql_source_main = gtk_entry_new ( );
	gtk_entry_set_alignment ( ( GtkEntry * ) w.entry_login_mysql_source_main, 1 );
	gtk_entry_set_width_chars ( ( GtkEntry * ) w.entry_login_mysql_source_main, 30 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_login_mysql_source_main, w.label_login_mysql_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_end ( ( GtkBox * ) w.box_login_mysql_source_main, w.entry_login_mysql_source_main, FALSE, FALSE, 0 );
	gtk_widget_set_margin_top ( w.box_login_mysql_source_main, 14 );
	gtk_widget_set_margin_bottom ( w.box_login_mysql_source_main, 14 );
	gtk_widget_set_margin_start ( w.box_login_mysql_source_main, 14 );
	gtk_widget_set_margin_end ( w.box_login_mysql_source_main, 14 );
	gtk_container_add ( ( GtkContainer * ) w.frame_mysql_source_main, w.box_mysql_source_main );

	w.box_password_mysql_source_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	w.label_password_mysql_source_main = gtk_label_new ( "Пароль" );
	w.entry_password_mysql_source_main = gtk_entry_new ( );
	gtk_entry_set_alignment ( ( GtkEntry * ) w.entry_password_mysql_source_main, 1 );
	gtk_entry_set_width_chars ( ( GtkEntry * ) w.entry_password_mysql_source_main, 30 );
	gtk_entry_set_invisible_char ( ( GtkEntry * ) w.entry_password_mysql_source_main, '*' );
	gtk_entry_set_visibility ( ( GtkEntry * ) w.entry_password_mysql_source_main, FALSE );
	gtk_box_pack_start ( ( GtkBox * ) w.box_password_mysql_source_main, w.label_password_mysql_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_end ( ( GtkBox * ) w.box_password_mysql_source_main, w.entry_password_mysql_source_main, FALSE, FALSE, 0 );
	gtk_widget_set_margin_top ( w.box_password_mysql_source_main, 14 );
	gtk_widget_set_margin_bottom ( w.box_password_mysql_source_main, 14 );
	gtk_widget_set_margin_start ( w.box_password_mysql_source_main, 14 );
	gtk_widget_set_margin_end ( w.box_password_mysql_source_main, 14 );

	w.box_host_mysql_source_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	w.label_host_mysql_source_main = gtk_label_new ( "Хост" );
	w.entry_host_mysql_source_main = gtk_entry_new ( );
	gtk_entry_set_alignment ( ( GtkEntry * ) w.entry_host_mysql_source_main, 1 );
	gtk_entry_set_width_chars ( ( GtkEntry * ) w.entry_host_mysql_source_main, 30 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_host_mysql_source_main, w.label_host_mysql_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_end ( ( GtkBox * ) w.box_host_mysql_source_main, w.entry_host_mysql_source_main, FALSE, FALSE, 0 );
	gtk_widget_set_margin_top ( w.box_host_mysql_source_main, 14 );
	gtk_widget_set_margin_bottom ( w.box_host_mysql_source_main, 14 );
	gtk_widget_set_margin_start ( w.box_host_mysql_source_main, 14 );
	gtk_widget_set_margin_end ( w.box_host_mysql_source_main, 14 );

	w.box_port_mysql_source_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	w.label_port_mysql_source_main = gtk_label_new ( "Порт" );
	w.entry_port_mysql_source_main = gtk_entry_new ( );
	gtk_entry_set_alignment ( ( GtkEntry * ) w.entry_port_mysql_source_main, 1 );
	gtk_entry_set_width_chars ( ( GtkEntry * ) w.entry_port_mysql_source_main, 30 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_port_mysql_source_main, w.label_port_mysql_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_end ( ( GtkBox * ) w.box_port_mysql_source_main, w.entry_port_mysql_source_main, FALSE, FALSE, 0 );
	gtk_widget_set_margin_top ( w.box_port_mysql_source_main, 14 );
	gtk_widget_set_margin_bottom ( w.box_port_mysql_source_main, 14 );
	gtk_widget_set_margin_start ( w.box_port_mysql_source_main, 14 );
	gtk_widget_set_margin_end ( w.box_port_mysql_source_main, 14 );

	w.box_db_mysql_source_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	w.label_db_mysql_source_main = gtk_label_new ( "База данных" );
	w.entry_db_mysql_source_main = gtk_entry_new ( );
	gtk_entry_set_alignment ( ( GtkEntry * ) w.entry_db_mysql_source_main, 1 );
	gtk_entry_set_width_chars ( ( GtkEntry * ) w.entry_db_mysql_source_main, 30 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_db_mysql_source_main, w.label_db_mysql_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_end ( ( GtkBox * ) w.box_db_mysql_source_main, w.entry_db_mysql_source_main, FALSE, FALSE, 0 );
	gtk_widget_set_margin_top ( w.box_db_mysql_source_main, 14 );
	gtk_widget_set_margin_bottom ( w.box_db_mysql_source_main, 14 );
	gtk_widget_set_margin_start ( w.box_db_mysql_source_main, 14 );
	gtk_widget_set_margin_end ( w.box_db_mysql_source_main, 14 );

	w.button_save_source_main = gtk_button_new_with_label ( "Сохранить" );
	g_signal_connect ( w.button_save_source_main, "clicked", G_CALLBACK ( button_save_source_main_clicked_cb ), NULL );
	gtk_widget_set_margin_top ( w.button_save_source_main, 8 );
	gtk_widget_set_margin_bottom ( w.button_save_source_main, 8 );
	gtk_widget_set_margin_start ( w.button_save_source_main, 8 );
	gtk_widget_set_margin_end ( w.button_save_source_main, 8 );

	w.box_buttons_end_source_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	gtk_box_pack_end ( ( GtkBox * ) w.box_buttons_end_source_main, w.button_save_source_main, FALSE, FALSE, 0 );

	gtk_box_pack_start ( ( GtkBox * ) w.box_mysql_source_main, w.box_login_mysql_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_mysql_source_main, w.box_password_mysql_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_mysql_source_main, w.box_host_mysql_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_mysql_source_main, w.box_port_mysql_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_mysql_source_main, w.box_db_mysql_source_main, FALSE, FALSE, 0 );

	gtk_box_pack_end ( ( GtkBox * ) w.box_source_main, w.box_buttons_end_source_main, FALSE, FALSE, 0 );

	gtk_box_pack_start ( ( GtkBox * ) w.box_source_source_main, w.label_source_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_end ( ( GtkBox * ) w.box_source_source_main, w.combo_box_source_source_main, FALSE, FALSE, 0 );

	gtk_container_add ( ( GtkContainer * ) w.frame_source_source_main, w.box_source_source_main );
	
	gtk_box_pack_start ( ( GtkBox * ) w.box_source_main, w.frame_source_source_main, FALSE, FALSE, 0 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_source_main, w.frame_mysql_source_main, FALSE, FALSE, 0 );

	gtk_container_add ( ( GtkContainer * ) w.window_source_main, w.box_source_main );

}
static gboolean window_data_item_main_delete_event_cb ( GtkWidget *widget, gpointer data ) {
	gtk_widget_hide ( w.window_data_item_main );
	return TRUE;
}
static void button_save_data_item_main_clicked_cb ( GtkButton *button, gpointer data ) {
	const char *item = gtk_entry_get_text ( ( GtkEntry * ) w.entry_item_data_item_main );
	if ( strlen ( item ) == 0 ) {
		g_notification_set_body ( notify, "Должны быть указаны все поля" );
		g_application_send_notification ( ( GApplication * ) app, NULL, notify );
		return;
	}

	struct data_item *di = calloc ( 1, sizeof ( struct data_item ) );
	if ( !di ) {
		g_notification_set_body ( notify, "Не удалось выделить память для data_item" );
		g_application_send_notification ( ( GApplication * ) app, NULL, notify );
		return;
	}

	di->item = g_strdup ( item );
	c_source_xore->data = di;
	c_source_xore->data_type = DATA_ITEM;
}

static void create_data_item_settings ( void ) {
	w.window_data_item_main = gtk_application_window_new ( app );
	gtk_window_set_default_size ( ( GtkWindow * ) w.window_data_item_main, 400, 200 );
	g_signal_connect ( w.window_data_item_main, "delete-event", G_CALLBACK ( window_data_item_main_delete_event_cb ), NULL );

	w.box_data_item_main = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 0 );
	gtk_container_add ( ( GtkContainer * ) w.window_data_item_main, w.box_data_item_main );
	w.frame_data_item_main = g_object_new ( GTK_TYPE_FRAME, "shadow-type", GTK_SHADOW_NONE, NULL );
	gtk_widget_set_margin_top ( w.frame_data_item_main, 16 );
	gtk_widget_set_margin_start ( w.frame_data_item_main, 32 );
	gtk_widget_set_margin_end ( w.frame_data_item_main, 32 );

	w.box_item_data_item_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	gtk_widget_set_margin_top ( w.box_item_data_item_main, 14 );
	gtk_widget_set_margin_bottom ( w.box_item_data_item_main, 14 );
	gtk_widget_set_margin_start ( w.box_item_data_item_main, 14 );
	gtk_widget_set_margin_end ( w.box_item_data_item_main, 14 );

	w.label_item_data_item_main = gtk_label_new ( "Порция данных" );
	w.entry_item_data_item_main = gtk_entry_new ( );
	gtk_entry_set_alignment ( ( GtkEntry * ) w.entry_item_data_item_main, 1 );
	gtk_entry_set_width_chars ( ( GtkEntry * ) w.entry_item_data_item_main, 30 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_item_data_item_main, w.label_item_data_item_main, FALSE, FALSE, 0 );
	gtk_box_pack_end ( ( GtkBox * ) w.box_item_data_item_main, w.entry_item_data_item_main, FALSE, FALSE, 0 );
	gtk_widget_set_margin_top ( w.box_item_data_item_main, 14 );
	gtk_widget_set_margin_bottom ( w.box_item_data_item_main, 14 );
	gtk_widget_set_margin_start ( w.box_item_data_item_main, 14 );
	gtk_widget_set_margin_end ( w.box_item_data_item_main, 14 );
	gtk_container_add ( ( GtkContainer * ) w.frame_data_item_main, w.box_item_data_item_main );

	w.box_buttons_end_data_item_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );
	w.button_save_data_item_main = gtk_button_new_with_label ( "Сохранить" );
	gtk_widget_set_margin_top ( w.button_save_data_item_main, 14 );
	gtk_widget_set_margin_bottom ( w.button_save_data_item_main, 14 );
	gtk_widget_set_margin_start ( w.button_save_data_item_main, 14 );
	gtk_widget_set_margin_end ( w.button_save_data_item_main, 14 );
	g_signal_connect ( w.button_save_data_item_main, "clicked", G_CALLBACK ( button_save_data_item_main_clicked_cb ), NULL );
	gtk_box_pack_end ( ( GtkBox * ) w.box_buttons_end_data_item_main, w.button_save_data_item_main, FALSE, FALSE, 0 );

	gtk_box_pack_start ( ( GtkBox * ) w.box_data_item_main, w.frame_data_item_main, FALSE, FALSE, 0 );
	gtk_box_pack_end ( ( GtkBox * ) w.box_data_item_main, w.box_buttons_end_data_item_main, FALSE, FALSE, 0 );
}

static gboolean window_main_delete_event_cb ( GtkWidget *widget, gpointer data ) {
	exit ( EXIT_SUCCESS );
	return FALSE;
}

char *current_project;

static void create_group ( FILE *fp, struct list_xore *l ) {
	switch ( l->type ) {
		case TYPE_IS_SOURCE:
			fprintf ( fp, "source {\n" );
			fprintf ( fp, "\tx: %d\n", l->x );
			fprintf ( fp, "\ty: %d\n", l->y );
			fprintf ( fp, "\twidth: %d\n", l->width );
			fprintf ( fp, "\theight: %d\n", l->height );
			fprintf ( fp, "\tdata_type: %d\n", l->data_type );
			switch ( l->data_type ) {
				case DATA_MYSQL: {
							 if ( l->data ) {
							 	struct source_mysql *m = ( struct source_mysql * ) l->data;
							 	fprintf ( fp, "\tdata_id: %d\n", m->id );
							 }
						 }
						 break;
			}
			fprintf ( fp, "}\n\n" );
			break;
		case TYPE_IS_DATA_ITEM:
			fprintf ( fp, "data_item {\n" );
			fprintf ( fp, "\tx: %d\n", l->x );
			fprintf ( fp, "\ty: %d\n", l->y );
			fprintf ( fp, "\twidth: %d\n", l->width );
			fprintf ( fp, "\theight: %d\n", l->height );
			fprintf ( fp, "\tdata_type: %d\n", l->data_type );
			switch ( l->data_type ) {
				case DATA_ITEM: {
							 if ( l->data ) {
							 	struct data_item *m = ( struct data_item * ) l->data;
							 	fprintf ( fp, "\tdata_id: %d\n", m->id );
							 }
						 }
						 break;
			}
			fprintf ( fp, "}\n\n" );
			break;
	}
}

int id_data_storage;

static void create_data ( FILE *fp, struct list_xore *l ) {
	switch ( l->data_type ) {
		case DATA_MYSQL: {
					 if ( l->data ) {
					 	struct source_mysql *m = ( struct source_mysql * ) l->data;
						m->id = id_data_storage++;
						fprintf ( fp, "%d {\n", m->id );
						fprintf ( fp, "\tid: %d\n", DATA_MYSQL );
						if ( m->login ) fprintf ( fp, "\tlogin: %s\n", m->login );
						if ( m->password ) fprintf ( fp, "\tpassword: %s\n", m->password );
						if ( m->host ) fprintf ( fp, "\thost: %s\n", m->host );
						if ( m->port ) fprintf ( fp, "\tport: %s\n", m->port );
						if ( m->db ) fprintf ( fp, "\tdb: %s\n", m->db );
						fprintf ( fp, "}\n\n" );
					 }
				 }
				 break;
		case DATA_ITEM: {
					 if ( l->data ) {
					 	struct data_item *m = ( struct data_item * ) l->data;
						m->id = id_data_storage++;
						fprintf ( fp, "%d {\n", m->id );
						fprintf ( fp, "\tid: %d\n", DATA_ITEM );
						if ( m->item ) fprintf ( fp, "\titem: %s\n", m->item );
						fprintf ( fp, "}\n\n" );
					 }
				}
				break;
	}
}

static void action_activate_select_save_project ( GSimpleAction *simple, GVariant *parameter, gpointer data ) {
	struct list_xore *l = get_objects();
	FILE *fp = fopen ( current_project, "w" );

	while ( l->next ) {
		switch ( l->type ) {
			case TYPE_IS_SOURCE: {
						     create_data ( fp, l );
						     create_group ( fp, l );
					     }
					     break;
			case TYPE_IS_DATA_ITEM: {
						     create_data ( fp, l );
						     create_group ( fp, l );
						}
						break;
		}

		l = l->next;
	}

	fclose ( fp );
	id_data_storage = 0;
}

static void free_data_mysql ( void *data ) {
	if ( data ) {
		struct source_mysql *m = ( struct source_mysql * ) data;
		if ( m->login ) free ( m->login );
		if ( m->password ) free ( m->password );
		if ( m->host ) free ( m->host );
		if ( m->port ) free ( m->port );
		free ( m );
	}
}

static void clear_list_xore ( void ) {
	struct list_xore *l = get_objects();
	struct list_xore *ld;

	while ( l->next ) {
		ld = l;

		if ( l->data ) {
			switch ( l->data_type ) {
				case DATA_MYSQL: {
							 free_data_mysql ( l->data );
							 l->data = NULL;
						 }
					break;
			}
		}
		l->file = NULL;

		l = l->next;
		ld->next = NULL;
		free ( ld );
	}
	object = NULL;
	gtk_widget_queue_draw ( w.drawing_area );
}

static void action_activate_select_save_project_as ( GSimpleAction *simple, GVariant *parameter, gpointer data ) {
}

static void save_new_project ( const char *file ) {
	current_project = NULL;
	char *xore_end = g_strrstr ( file, ".xore" );

	if ( xore_end ) {
		if ( xore_end[5] == 0 ) {
			current_project = g_strdup ( file );
		} else {
			current_project = g_strdup_printf ( "%s.xore", file );
		}
	} else {
		current_project = g_strdup_printf ( "%s.xore", file );
	}

	FILE *fp = fopen ( current_project, "w" );
	fclose ( fp );

	clear_list_xore ( );
}

static void action_activate_select_open_project ( GSimpleAction *simple, GVariant *parameter, gpointer data ) {
	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	GtkFileFilter *filter;
	GtkFileFilter *filter_all;
	int res;

	filter = gtk_file_filter_new ( );
	gtk_file_filter_add_pattern ( filter, "*.xore" );
	gtk_file_filter_set_name ( filter, "xore files" );

	filter_all = gtk_file_filter_new ( );
	gtk_file_filter_add_pattern ( filter_all, "*" );
	gtk_file_filter_set_name ( filter_all, "all files" );

	dialog = gtk_file_chooser_dialog_new ( "Открыть проект",
			( GtkWindow * ) w.window_main,
			action,
			"Отмена",
			GTK_RESPONSE_CANCEL,
			"Сохранить",
			GTK_RESPONSE_ACCEPT,
			NULL
			);

	chooser = GTK_FILE_CHOOSER ( dialog );

	gtk_file_chooser_add_filter ( chooser, filter );
	gtk_file_chooser_add_filter ( chooser, filter_all );

	res = gtk_dialog_run ( GTK_DIALOG ( dialog ) );
	if ( res == GTK_RESPONSE_ACCEPT ) {
		char *file;
		file = gtk_file_chooser_get_filename ( chooser );
		current_project = g_strdup ( file );
		get_objects ( );
		read_config ( file );
		g_free ( file );
		gtk_widget_queue_draw ( w.drawing_area );
	}

	gtk_widget_destroy ( dialog );
}
static void action_activate_select_new_project ( GSimpleAction *simple, GVariant *parameter, gpointer data ) {
	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
	GtkFileFilter *filter;
	GtkFileFilter *filter_all;
	int res;

	filter = gtk_file_filter_new ( );
	gtk_file_filter_add_pattern ( filter, "*.xore" );
	gtk_file_filter_set_name ( filter, "xore files" );

	filter_all = gtk_file_filter_new ( );
	gtk_file_filter_add_pattern ( filter_all, "*" );
	gtk_file_filter_set_name ( filter_all, "all files" );

	dialog = gtk_file_chooser_dialog_new ( "Создать проект",
			( GtkWindow * ) w.window_main,
			action,
			"Отмена",
			GTK_RESPONSE_CANCEL,
			"Сохранить",
			GTK_RESPONSE_ACCEPT,
			NULL
			);

	chooser = GTK_FILE_CHOOSER ( dialog );

	gtk_file_chooser_set_do_overwrite_confirmation ( chooser, TRUE );
	gtk_file_chooser_add_filter ( chooser, filter );
	gtk_file_chooser_add_filter ( chooser, filter_all );

	res = gtk_dialog_run ( GTK_DIALOG ( dialog ) );
	if ( res == GTK_RESPONSE_ACCEPT ) {
		char *file;
		file = gtk_file_chooser_get_filename ( chooser );
		save_new_project ( file );
		g_free ( file );
	}

	gtk_widget_destroy ( dialog );
}

static void create_actions ( ) {
	const GActionEntry entries[] = {
		{ "select_save_project", action_activate_select_save_project },
		{ "select_save_project_as", action_activate_select_save_project_as },
		{ "select_new_project", action_activate_select_new_project },
		{ "select_open_project", action_activate_select_open_project }
	};

	g_action_map_add_action_entries ( G_ACTION_MAP (app), entries, G_N_ELEMENTS (entries), NULL );
}

static void app_activate_cb ( GtkApplication *app, gpointer data ) {
	w.window_main = gtk_application_window_new ( app );
	gtk_window_maximize ( ( GtkWindow * ) w.window_main );
	g_signal_connect ( w.window_main, "delete-event", G_CALLBACK ( window_main_delete_event_cb ), NULL );

	notify = g_notification_new ( "xore" );

	create_actions ( );

	GMenu *menu_app = g_menu_new ( );
	g_menu_append ( menu_app, "Сохранить проект", "app.select_save_project" );
	g_menu_append ( menu_app, "Сохранить проект как...", "app.select_save_project_as" );
	g_menu_append ( menu_app, "Создать новый проект", "app.select_new_project" );
	g_menu_append ( menu_app, "Открыть проект", "app.select_open_project" );

	w.header_bar = gtk_header_bar_new ( );
	gtk_header_bar_set_show_close_button ( ( GtkHeaderBar * ) w.header_bar, TRUE );
	gtk_header_bar_set_decoration_layout ( ( GtkHeaderBar * ) w.header_bar, ":menu,minimize,maximize,close" );

	gtk_window_set_titlebar ( ( GtkWindow * ) w.window_main, w.header_bar );
	gtk_application_set_app_menu ( app, ( GMenuModel * ) menu_app );

	create_source_settings ( );
	create_data_item_settings ( );

	w.box_main = gtk_box_new ( GTK_ORIENTATION_HORIZONTAL, 0 );

	GtkCssProvider *provider = gtk_css_provider_new ( );
	GtkStyleContext *context = gtk_style_context_new ( );
	GdkScreen *screen = gtk_style_context_get_screen ( context );
	gtk_css_provider_load_from_data ( provider, styles, -1, NULL );
	gtk_style_context_add_provider_for_screen ( screen, ( GtkStyleProvider * ) provider, GTK_STYLE_PROVIDER_PRIORITY_USER );

	w.box_tools = gtk_box_new ( GTK_ORIENTATION_VERTICAL, 0 );
	create_tools ( );
	w.frame_tools = g_object_new ( GTK_TYPE_FRAME, "shadow-type", GTK_SHADOW_NONE, NULL );
	gtk_container_add ( ( GtkContainer * ) w.frame_tools, w.box_tools );

	w.drawing_area = gtk_drawing_area_new ( );
	gtk_widget_add_events ( w.drawing_area, GDK_BUTTON1_MOTION_MASK | GDK_BUTTON_PRESS_MASK );
	g_signal_connect ( w.drawing_area, "draw", G_CALLBACK ( drawing_area_draw_cb ), NULL );
	g_signal_connect ( w.drawing_area, "drag-drop", G_CALLBACK ( drawing_area_drag_drop_cb ), NULL );
	g_signal_connect ( w.drawing_area, "drag-motion", G_CALLBACK ( drawing_area_drag_motion_cb ), NULL );
	g_signal_connect ( w.drawing_area, "drag-data-received", G_CALLBACK ( drawing_area_drag_data_received_cb ), NULL );
	g_signal_connect ( w.drawing_area, "button-press-event", G_CALLBACK ( drawing_area_button_press_event_cb ), NULL );
	g_signal_connect ( w.drawing_area, "button-release-event", G_CALLBACK ( drawing_area_button_release_event_cb ), NULL );
	g_signal_connect ( w.drawing_area, "motion-notify-event", G_CALLBACK ( drawing_area_motion_event_cb ), NULL );
	gtk_drag_dest_set ( w.drawing_area,
			GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT,
			&target,
			1,
			GDK_ACTION_COPY
			);

	gtk_box_pack_start ( ( GtkBox * ) w.box_main, w.frame_tools, FALSE, FALSE, 0 );
	gtk_box_pack_start ( ( GtkBox * ) w.box_main, w.drawing_area, TRUE, TRUE, 0 );

	gtk_container_add ( ( GtkContainer * ) w.window_main, w.box_main );
	
	set_theme ( "light" );

	gtk_widget_show_all ( w.window_main );
}

int main ( int argc, char **argv ) {
	app = gtk_application_new ( app_name, G_APPLICATION_FLAGS_NONE );
	g_application_register ( ( GApplication * ) app, NULL, NULL );
	g_signal_connect ( app, "activate", G_CALLBACK ( app_activate_cb ), NULL );
	return g_application_run ( ( GApplication * ) app, argc, argv );
}
