# include <stdlib.h>
# include <cairo.h>
# include <gtk/gtk.h>

int slides[] = { 0, 0, 0, 0 };

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
    if (pos < 25) {
        i = pos*4 + x;
        j = y;
    }
    else if (pos < 50) {
        i = x + 100;
        j = y + (pos - 25)*4;
    }
    else if (pos < 75) {
        i = x + 100 - (pos - 50)*4;
        j = y + 100;
    }
    else {
        i = x;
        j = y + 100 - (pos - 75)*4;
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

  draw_grid_part(cr, 30, 30);
  draw_grid_part(cr, 130, 30);
  draw_grid_part(cr, 80, 130);
  draw_grid_part(cr, 180, 130);
  draw_grid_part(cr, 230, 30);

  draw_trem(cr, 100, 0, 0, 30, 30, slides[0]);
  draw_trem(cr, 100, 100, 0, 130, 30, slides[1]);
  draw_trem(cr, 0, 100, 0, 80, 130, slides[2]);
  draw_trem(cr, 0, 0, 100, 180, 130, slides[3]);
  draw_trem(cr, 100, 0, 100, 230, 30, slides[4]);

  cairo_destroy(cr);

  return FALSE;
}

typedef struct {
  int a;
  GtkWidget* b;
} pair;

GtkWidget* scale()
{
  return gtk_hscale_new_with_range(0, 100, 1);
}

gboolean changed (GtkRange     *range,
                                                        GtkScrollType scroll,
                                                        gdouble       value,
                                                        pair*      user_data)
{
    printf("risos %d:  %g\n", user_data->a, gtk_range_get_value(range));

    slides[user_data->a] = gtk_range_get_value(range);

    gtk_widget_queue_draw_area(user_data->b, 0, 0, 360, 260);

    return FALSE;
}

pair a0, a1, a2, a3, a4;

int main( int   argc,
          char *argv[] )
{

    gtk_init (&argc, &argv);
    
    GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *box = gtk_hbox_new (FALSE, 3);
    GtkWidget *vbox = gtk_vbox_new(TRUE, 3);
    GtkWidget *darea = gtk_drawing_area_new();

    GtkWidget *scale1 = scale();
    GtkWidget *scale2 = scale();
    GtkWidget *scale3 = scale();
    GtkWidget *scale4 = scale();
    GtkWidget *scale5 = scale();

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

    a0.a = 0;
    a1.a = 1;
    a2.a = 2;
    a3.a = 3;
    a4.a = 4;
    a0.b = a1.b = a2.b = a3.b = a4.b = darea;

    g_signal_connect(scale1, "change-value", G_CALLBACK (changed), &a0);
    g_signal_connect(scale2, "change-value", G_CALLBACK (changed), &a1);
    g_signal_connect(scale3, "change-value", G_CALLBACK (changed), &a2);
    g_signal_connect(scale4, "change-value", G_CALLBACK (changed), &a3);
    g_signal_connect(scale5, "change-value", G_CALLBACK (changed), &a4);



    gtk_container_add (GTK_CONTAINER (window), box);

    g_signal_connect (window, "destroy",
		      G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect(darea, "expose-event",
      G_CALLBACK (on_expose_event), NULL);

    
    gtk_widget_show_all (window);
    
    gtk_main ();
    
    return 0;
}
