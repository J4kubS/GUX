/*
 * File:   main.c
 * Author: Jakub Šoustar
 *
 * Created on 15. 12. 2015, 21:44
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#define BUTTON_BASE_HEIGHT 45
#define BUTTON_BASE_WIDTH 45
#define EXTENDED_BUTTONS 32
#define BASIC_BUTTONS 20
#define MODE_EXTENDED 1
#define MODE_BASIC 0
#define SPACING 3

typedef struct _CalcButton {
	GtkWidget *button;
	gchar *label;
	gchar *value;
	guint length;
	gint key;
	guint left;
	guint top;
	guint width;
	guint height;
} CalcButton;

#define SQUARE_ROOT -1
#define CUBE_ROOT -2
#define SQUARE -3
#define CUBE -4
#define PI -5
#define EULER -6
#define SIN -7
#define COS -8
#define TAN -9
#define LOG -10
#define LN -11
#define FACTORIAL -12

CalcButton extended_buttons[] = {
	{ .label = "0", .length = 1, .value = "0", .key = GDK_KEY_0, .left = 0, .top = 5, .width = 2, .height = 1, .button = NULL },
	{ .label = "1", .length = 1, .value = "1", .key = GDK_KEY_1, .left = 0, .top = 4, .width = 1, .height = 1, .button = NULL },
	{ .label = "2", .length = 1, .value = "2", .key = GDK_KEY_2, .left = 1, .top = 4, .width = 1, .height = 1, .button = NULL },
	{ .label = "3", .length = 1, .value = "3", .key = GDK_KEY_3, .left = 2, .top = 4, .width = 1, .height = 1, .button = NULL },
	{ .label = "4", .length = 1, .value = "4", .key = GDK_KEY_4, .left = 0, .top = 3, .width = 1, .height = 1, .button = NULL },
	{ .label = "5", .length = 1, .value = "5", .key = GDK_KEY_5, .left = 1, .top = 3, .width = 1, .height = 1, .button = NULL },
	{ .label = "6", .length = 1, .value = "6", .key = GDK_KEY_6, .left = 2, .top = 3, .width = 1, .height = 1, .button = NULL },
	{ .label = "7", .length = 1, .value = "7", .key = GDK_KEY_7, .left = 0, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = "8", .length = 1, .value = "8", .key = GDK_KEY_8, .left = 1, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = "9", .length = 1, .value = "9", .key = GDK_KEY_9, .left = 2, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = ".", .length = 1, .value = ".", .key = GDK_KEY_period, .left = 2, .top = 5, .width = 1, .height = 1, .button = NULL },
	{ .label = "+", .length = 1, .value = "+", .key = GDK_KEY_plus, .left = 3, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = "−", .length = 1, .value = "-", .key = GDK_KEY_minus, .left = 3, .top = 3, .width = 1, .height = 1, .button = NULL },
	{ .label = "×", .length = 1, .value = "*", .key = GDK_KEY_asterisk, .left = 3, .top = 4, .width = 1, .height = 1, .button = NULL },
	{ .label = "÷", .length = 1, .value = "/", .key = GDK_KEY_slash, .left = 3, .top = 5, .width = 1, .height = 1, .button = NULL },
	{ .label = "x²", .length = 2, .value = "sq", .key = SQUARE, .left = 4, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = "x³", .length = 2, .value = "cu", .key = CUBE, .left = 5, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = "√", .length = 1, .value = "sqrt", .key = SQUARE_ROOT, .left = 4, .top = 3, .width = 1, .height = 1, .button = NULL },
	{ .label = "∛", .length = 1, .value = "curt", .key = CUBE_ROOT, .left = 5, .top = 3, .width = 1, .height = 1, .button = NULL },
	{ .label = "=", .length = 1, .value = "", .key = GDK_KEY_equal, .left = 4, .top = 4, .width = 2, .height = 2, .button = NULL },
	{ .label = "(", .length = 1, .value = "", .key = GDK_KEY_parenleft, .left = 0, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = ")", .length = 1, .value = "", .key = GDK_KEY_parenright, .left = 1, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "π", .length = 1, .value = "", .key = PI, .left = 2, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "ℇ", .length = 1, .value = "", .key = EULER, .left = 3, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "↩", .length = 1, .value = "", .key = GDK_KEY_BackSpace, .left = 4, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "C", .length = 1, .value = "", .key = GDK_KEY_C, .left = 5, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "sin", .length = 3, .value = "sin", .key = SIN, .left = 0, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "cos", .length = 3, .value = "cos", .key = COS, .left = 1, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "tan", .length = 3, .value = "tan", .key = TAN, .left = 2, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "log", .length = 3, .value = "log", .key = LOG, .left = 3, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "ln", .length = 2, .value = "ln", .key = LN, .left = 4, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "!", .length = 1, .value = "!", .key = FACTORIAL, .left = 5, .top = 1, .width = 1, .height = 1, .button = NULL }
};

CalcButton basic_buttons[] = {
	{ .label = "0", .length = 1, .value = "0", .key = GDK_KEY_0, .left = 0, .top = 3, .width = 2, .height = 1, .button = NULL },
	{ .label = "1", .length = 1, .value = "1", .key = GDK_KEY_1, .left = 0, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = "2", .length = 1, .value = "2", .key = GDK_KEY_2, .left = 1, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = "3", .length = 1, .value = "3", .key = GDK_KEY_3, .left = 2, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = "4", .length = 1, .value = "4", .key = GDK_KEY_4, .left = 0, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "5", .length = 1, .value = "5", .key = GDK_KEY_5, .left = 1, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "6", .length = 1, .value = "6", .key = GDK_KEY_6, .left = 2, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "7", .length = 1, .value = "7", .key = GDK_KEY_7, .left = 0, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "8", .length = 1, .value = "8", .key = GDK_KEY_8, .left = 1, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "9", .length = 1, .value = "9", .key = GDK_KEY_9, .left = 2, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = ".", .length = 1, .value = ".", .key = GDK_KEY_period, .left = 2, .top = 3, .width = 1, .height = 1, .button = NULL },
	{ .label = "+", .length = 1, .value = "+", .key = GDK_KEY_plus, .left = 3, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "−", .length = 1, .value = "-", .key = GDK_KEY_minus, .left = 3, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "×", .length = 1, .value = "*", .key = GDK_KEY_asterisk, .left = 3, .top = 2, .width = 1, .height = 1, .button = NULL },
	{ .label = "÷", .length = 1, .value = "/", .key = GDK_KEY_slash, .left = 3, .top = 3, .width = 1, .height = 1, .button = NULL },
	{ .label = "↩", .length = 1, .value = "", .key = GDK_KEY_BackSpace, .left = 4, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "C", .length = 1, .value = "", .key = GDK_KEY_C, .left = 5, .top = 0, .width = 1, .height = 1, .button = NULL },
	{ .label = "(", .length = 1, .value = "(", .key = GDK_KEY_parenleft, .left = 4, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = ")", .length = 1, .value = ")", .key = GDK_KEY_parenright, .left = 5, .top = 1, .width = 1, .height = 1, .button = NULL },
	{ .label = "=", .length = 1, .value = "", .key = GDK_KEY_equal, .left = 4, .top = 2, .width = 2, .height = 2, .button = NULL }
};

GtkWidget *window;
GtkWidget *label;
GtkWidget *table;
GtkWidget *text;
guint mode = MODE_BASIC;

guint get_key(guint key) {
	switch (key) {
		case GDK_KEY_KP_0:
			key = GDK_KEY_0;
			break;
		case GDK_KEY_KP_1:
			key = GDK_KEY_1;
			break;
		case GDK_KEY_KP_2:
			key = GDK_KEY_2;
			break;
		case GDK_KEY_KP_3:
			key = GDK_KEY_3;
			break;
		case GDK_KEY_KP_4:
			key = GDK_KEY_4;
			break;
		case GDK_KEY_KP_5:
			key = GDK_KEY_5;
			break;
		case GDK_KEY_KP_6:
			key = GDK_KEY_6;
			break;
		case GDK_KEY_KP_7:
			key = GDK_KEY_7;
			break;
		case GDK_KEY_KP_8:
			key = GDK_KEY_8;
			break;
		case GDK_KEY_KP_9:
			key = GDK_KEY_9;
			break;
		case GDK_KEY_KP_Decimal:
			key = GDK_KEY_period;
			break;
		case GDK_KEY_KP_Add:
			key = GDK_KEY_plus;
			break;
		case GDK_KEY_KP_Subtract:
			key = GDK_KEY_minus;
			break;
		case GDK_KEY_KP_Multiply:
			key = GDK_KEY_asterisk;
			break;
		case GDK_KEY_KP_Divide:
			key = GDK_KEY_slash;
			break;
		case GDK_KEY_KP_Enter:
		case GDK_KEY_Return:
			key = GDK_KEY_equal;
			break;
	}

	return key;
}

CalcButton *get_button(guint key) {
	CalcButton *buttons;
	int count, i;
	key = get_key(key);

	if (mode == MODE_BASIC) {
		buttons = basic_buttons;
		count = BASIC_BUTTONS;
	} else {
		buttons = extended_buttons;
		count = EXTENDED_BUTTONS;
	}

	for (i = 0; i < count; i++) {
		CalcButton *button = &buttons[i];

		if (button->key == key) {
			return button;
		}
	}

	return NULL;
}

void clear(GtkWidget *widget, gpointer data) {
	gtk_label_set_text(GTK_LABEL(label), "");
	gtk_entry_set_text(GTK_ENTRY(text), "");
}

void about(GtkWidget *widget, gpointer data) {
	static const gchar * authors[] = { "Jakub Šoustar <xsoust02@stud.fit.vutbr.cz>", NULL };

	gtk_show_about_dialog(
			GTK_WINDOW(window),
			"program-name", "Calculator",
			"comments", "A possible GUI for calculator application. Created as project for GUX course.",
			"copyright", "Copyright 2015 © Jakub Šoustar",
			"authors", &authors,
			"version", "0.1",
			NULL);
}

void quit(GtkWidget *widget, gpointer data) {
	gtk_main_quit();
}

void button_pressed(GtkWidget *widget, gpointer data) {
	GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(text));
	guint key = get_key(*((guint *) data));

	if (key == GDK_KEY_equal) {
		gtk_label_set_text(GTK_LABEL(label), gtk_entry_get_text(GTK_ENTRY(text)));
		gtk_entry_set_text(GTK_ENTRY(text), "");
	} else if (key == GDK_KEY_C) {
		clear(widget, NULL);
	} else if (key == GDK_KEY_BackSpace) {
		GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(text));
		guint cursor;

		g_object_get(text, "cursor-position", &cursor, NULL);

		if (cursor > 0) {
			gtk_entry_buffer_delete_text(GTK_ENTRY_BUFFER(buffer), cursor - 1, 1);
		}
	} else {
		CalcButton* button = get_button(key);
		guint cursor;

		g_object_get(text, "cursor-position", &cursor, NULL);
		gtk_entry_buffer_insert_text(
				GTK_ENTRY_BUFFER(buffer),
				cursor,
				button->label,
				button->length);
		g_signal_emit_by_name(
				text,
				"move-cursor",
				GTK_MOVEMENT_LOGICAL_POSITIONS,
				button->length,
				FALSE,
				NULL);
	}
}

gboolean key_pressed(GtkWidget *widget, GdkEventKey *event, gpointer data) {
	CalcButton *button = get_button(event->keyval);

	if (button != NULL) {
		button_pressed(widget, &event->keyval);
		return FALSE;
	}

	switch (event->keyval) {
		case GDK_KEY_rightarrow:
		case GDK_KEY_leftarrow:
		case GDK_KEY_downarrow:
		case GDK_KEY_uparrow:
		case GDK_KEY_KP_Right:
		case GDK_KEY_KP_Left:
		case GDK_KEY_KP_Down:
		case GDK_KEY_KP_Up:
			return TRUE;
		default:
			return FALSE;
	}
}

void switch_buttons() {
	GList *children, *iterator;
	CalcButton *buttons = NULL;
	guint count;

	children = gtk_container_get_children(GTK_CONTAINER(table));
	iterator = children;

	while (iterator != NULL) {
		gtk_widget_hide(GTK_WIDGET(iterator->data));
		iterator = g_list_next(iterator);
	}

	g_list_free(children);

	if (mode == MODE_BASIC) {
		buttons = basic_buttons;
		count = BASIC_BUTTONS;
	} else {
		buttons = extended_buttons;
		count = EXTENDED_BUTTONS;
	}

	if (buttons != NULL) {
		CalcButton *calc_button;
		GtkWidget *button;
		int i = 0;

		for (i = 0; i < count; i++) {
			calc_button = &buttons[i];
			button = calc_button->button;

			if (button == NULL) {
				button = gtk_button_new_with_label(calc_button->label);
				calc_button->button = button;

				gtk_widget_modify_font(GTK_WIDGET(button), pango_font_description_from_string("Monospace 10"));
				gtk_button_set_focus_on_click(GTK_BUTTON(button), FALSE);
				gtk_widget_set_size_request(
						GTK_WIDGET(button),
						BUTTON_BASE_WIDTH * calc_button->width,
						BUTTON_BASE_HEIGHT * calc_button->height);
				gtk_table_attach(
						GTK_TABLE(table),
						button,
						calc_button->left,
						calc_button->left + calc_button->width,
						calc_button->top,
						calc_button->top + calc_button->height,
						GTK_EXPAND | GTK_FILL,
						GTK_EXPAND | GTK_FILL,
						0,
						0);

				g_signal_connect(button, "clicked", G_CALLBACK(button_pressed), &calc_button->key);
			}

			gtk_widget_show(GTK_WIDGET(button));
		}
	}
}

void set_extended_mode(GtkWidget *widget, gpointer data) {
	mode = MODE_EXTENDED;
	switch_buttons();
}

void set_basic_mode(GtkWidget *widget, gpointer data) {
	mode = MODE_BASIC;
	switch_buttons();
}

void initialize() {
	GtkAccelGroup *accel_group;
	GSList *group = NULL;
	GtkWidget *content_box;
	GtkWidget *file_menu;
	GtkWidget *mode_menu;
	GtkWidget *help_menu;
	GtkWidget *main_box;
	GtkWidget *menu_bar;
	GtkWidget *item;

	// WINDOW
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Calculator");
	g_signal_connect(window, "key_press_event", G_CALLBACK(key_pressed), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(quit), NULL);

	// ACCELERATORS
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	// MENU BAR
	menu_bar = gtk_menu_bar_new();
	file_menu = gtk_menu_new();
	mode_menu = gtk_menu_new();
	help_menu = gtk_menu_new();

	// FILE MENU
	item = gtk_menu_item_new_with_mnemonic("_File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), file_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item);

	item = gtk_menu_item_new_with_mnemonic("_Clear");
	gtk_widget_add_accelerator(item, "activate", accel_group, GDK_KEY_c, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(item, "activate", G_CALLBACK(clear), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), item);

	item = gtk_menu_item_new_with_mnemonic("_Quit");
	gtk_widget_add_accelerator(item, "activate", accel_group, GDK_KEY_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(item, "activate", G_CALLBACK(quit), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), item);

	// MODE MENU
	item = gtk_menu_item_new_with_mnemonic("_Mode");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), mode_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item);

	item = gtk_radio_menu_item_new_with_mnemonic(group, "_Basic");
	gtk_widget_add_accelerator(item, "activate", accel_group, GDK_KEY_b, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(item, "activate", G_CALLBACK(set_basic_mode), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(mode_menu), item);

	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(item));
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), TRUE);

	item = gtk_radio_menu_item_new_with_mnemonic(group, "_Extended");
	gtk_widget_add_accelerator(item, "activate", accel_group, GDK_KEY_e, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	g_signal_connect(item, "activate", G_CALLBACK(set_extended_mode), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(mode_menu), item);

	// HELP MENU
	item = gtk_menu_item_new_with_mnemonic("_Help");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), help_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), item);

	item = gtk_menu_item_new_with_mnemonic("_About");
	g_signal_connect(item, "activate", G_CALLBACK(about), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), item);

	// TEXT
	text = gtk_entry_new();
	gtk_widget_modify_font(GTK_WIDGET(text), pango_font_description_from_string("Monospace Bold 20"));
	gtk_editable_set_editable(GTK_EDITABLE(text), FALSE);

	// LABEL
	label = gtk_label_new(NULL);
	gtk_widget_modify_font(GTK_WIDGET(label), pango_font_description_from_string("Monospace Bold 10"));
	gtk_label_set_ellipsize(GTK_LABEL(label), PANGO_ELLIPSIZE_END);
	g_object_set(label, "xpad", SPACING, NULL);
	g_object_set(label, "xalign", 0, NULL);

	// TABLE
	table = gtk_table_new(1, 1, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(table), 1);
	gtk_table_set_row_spacings(GTK_TABLE(table), 1);

	switch_buttons();

	// CONTENT BOX
	content_box = gtk_vbox_new(FALSE, SPACING);
	gtk_container_set_border_width(GTK_CONTAINER(content_box), SPACING);
	gtk_container_add(GTK_CONTAINER(content_box), text);
	gtk_container_add(GTK_CONTAINER(content_box), label);
	gtk_container_add(GTK_CONTAINER(content_box), table);
	gtk_container_child_set(GTK_CONTAINER(content_box), GTK_WIDGET(text), "expand", FALSE, NULL);
	gtk_container_child_set(GTK_CONTAINER(content_box), GTK_WIDGET(label), "expand", FALSE, NULL);
	gtk_container_child_set(GTK_CONTAINER(content_box), GTK_WIDGET(table), "expand", TRUE, NULL);
	gtk_container_child_set(GTK_CONTAINER(content_box), GTK_WIDGET(table), "fill", TRUE, NULL);

	// MAIN BOX
	main_box = gtk_vbox_new(FALSE, SPACING);
	gtk_container_add(GTK_CONTAINER(main_box), menu_bar);
	gtk_container_add(GTK_CONTAINER(main_box), content_box);
	gtk_container_child_set(GTK_CONTAINER(main_box), GTK_WIDGET(menu_bar), "expand", FALSE, NULL);

	gtk_container_add(GTK_CONTAINER(window), main_box);
	gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);
	initialize();
	gtk_main();

	return 0;
}
