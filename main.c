# include <stdlib.h>
# include <cairo.h>
# include <gtk/gtk.h>

# include <glib.h>

extern void begin_threads ();

unsigned int slides[5] = { 50, 50, 50, 50, 50 };
unsigned int pos[5] = { 0, 0, 0, 0, 0 };

void
draw_grid_part(cairo_t* cr, double x, double y)
{
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	cairo_rectangle(cr, x, y, 100, 100);
	cairo_set_line_width(cr, 14);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND); 
	cairo_stroke(cr);
}

void
draw_trem(cairo_t* cr, double r, double g, double b,
			double x, double y, double pos)
{
	double i, j;
	if (pos < 100) {
		i = pos + x;
		j = y;
	}
	else if (pos < 200) {
		i = x + 100;
		j = y + (pos - 100);
	}
	else if (pos < 300) {
		i = x + 100 - (pos - 200);
		j = y + 100;
	}
	else {
		i = x;
		j = y + 100 - (pos - 300);
	}


	cairo_set_source_rgba(cr, r, g, b, 1);
	cairo_rectangle(cr, i, j, 2, 2);
	cairo_set_line_width(cr, 14);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND); 
	cairo_stroke(cr);
}

static gboolean
on_expose_event(GtkWidget *widget,
		GdkEventExpose *event,
		gpointer data)
{
	cairo_t *cr;

	cr = gdk_cairo_create (widget->window);

	cairo_rectangle (cr, event->area.x, event->area.y, 
			event->area.width, event->area.height);
	cairo_clip (cr);

	draw_grid_part(cr, 30, 30);
	draw_grid_part(cr, 130, 30);
	draw_grid_part(cr, 80, 130);
	draw_grid_part(cr, 180, 130);
	draw_grid_part(cr, 230, 30);

	draw_trem(cr, 100, 0, 0, 30, 30, pos[0]);
	draw_trem(cr, 100, 100, 0, 130, 30, pos[1]);
	draw_trem(cr, 0, 100, 0, 80, 130, pos[2]);
	draw_trem(cr, 0, 0, 100, 180, 130, pos[3]);
	draw_trem(cr, 100, 0, 100, 230, 30, pos[4]);

	cairo_destroy(cr);

	return FALSE;
}


gboolean changed (GtkRange* range,
			GtkScrollType scroll,
			gdouble value,
			gpointer user_data)
{
	slides[(unsigned long) user_data] = gtk_range_get_value(range);
	return FALSE;
}

gboolean
gtk_redraw (gpointer w)
{
	gtk_widget_queue_draw_area((GtkWidget*)w, 0, 0, 360, 260);
}

GtkWidget*
scale(unsigned long data)
{
	GtkWidget* q = gtk_hscale_new_with_range(0, 100, 1);
	gtk_range_set_value(GTK_RANGE(q), 50);
	g_signal_connect(q, "change-value", G_CALLBACK (changed),
			(gpointer) data);
	return q;
}

int
main(int argc, char *argv[])
{
	gtk_init (&argc, &argv);
    
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkWidget *box = gtk_hbox_new (FALSE, 3);
	GtkWidget *vbox = gtk_vbox_new(TRUE, 3);
	GtkWidget *darea = gtk_drawing_area_new();

	GtkWidget *scale1 = scale(0);
	GtkWidget *scale2 = scale(1);
	GtkWidget *scale3 = scale(2);
	GtkWidget *scale4 = scale(3);
	GtkWidget *scale5 = scale(4);

	gtk_container_set_border_width (GTK_CONTAINER (window), 3);

	gtk_widget_set_size_request(vbox, 200, 260);
	gtk_widget_set_size_request(darea, 360, 260);

	gtk_box_pack_start_defaults (GTK_BOX (box), darea);
	gtk_box_pack_start (GTK_BOX (box), vbox, FALSE, FALSE, 0);

	gtk_box_pack_start_defaults (GTK_BOX (vbox), scale1);
	gtk_box_pack_start_defaults (GTK_BOX (vbox), scale2);
	gtk_box_pack_start_defaults (GTK_BOX (vbox), scale3);
	gtk_box_pack_start_defaults (GTK_BOX (vbox), scale4);
	gtk_box_pack_start_defaults (GTK_BOX (vbox), scale5);

	g_timeout_add(100, gtk_redraw, darea);

	gtk_container_add (GTK_CONTAINER (window), box);

	g_signal_connect (window, "destroy",
			G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(darea, "expose-event",
			G_CALLBACK (on_expose_event), NULL);

    
	gtk_widget_show_all (window);

	begin_threads();
    
	gtk_main ();
    
	return 0;
}
