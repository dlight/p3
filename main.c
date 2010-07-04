# include <stdlib.h>
# include <cairo.h>
# include <gtk/gtk.h>

# include <glib.h>

# include <unistd.h>

double cor[5][3] = {
	{ 0xff, 0x59, 0x00 },
	{ 0xff, 0xe7, 0x00 },
	{ 0xb2, 0x7c, 0xff },
	{ 0x7c, 0xff, 0xd7 },
	{ 0xff, 0xea, 0xdf }
};

double dim[5][2] = {
	{ 30, 30 },
	{ 130, 30 },
	{ 230, 30 },
	{ 80, 130 },
	{ 180, 130 }
};

extern int holding_lock[3];

extern void begin_threads ();
extern void begin_threads_nosync ();

unsigned slides[5] = { 50, 50, 50, 50, 50 };
unsigned pos[5] = { 0, 50, 50, 350, 250 };

void
draw_grid_part(cairo_t* cr, unsigned i)
{
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_rectangle(cr, dim[i][0], dim[i][1], 100, 100);
	cairo_set_line_width(cr, 14);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND); 
	cairo_stroke(cr);
}

void draw_square(cairo_t* cr, unsigned k, double i, double j)
{
	cairo_set_source_rgb(cr, cor[k][0] / 255, cor[k][1] / 255,
				cor[k][2] / 255);
	cairo_rectangle(cr, i-6, j-6, 12, 12);
	cairo_set_line_width(cr, 12);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
	cairo_fill(cr);
	cairo_stroke(cr);
}

void
draw_trem(cairo_t* cr, unsigned k)
{
	double p = pos[k];

	double x = dim[k][0], y = dim[k][1];
	double i, j;
	if (p < 100) {
		i = p + x;
		j = y;
	}
	else if (p < 200) {
		i = x + 100;
		j = y + (p - 100);
	}
	else if (p < 300) {
		i = x + 100 - (p - 200);
		j = y + 100;
	}
	else {
		i = x;
		j = y + 100 - (p - 300);
	}


	draw_square(cr, k, i, j);
}

void draw_flag(cairo_t* cr, unsigned p)
{
	int k = holding_lock[p];

	if (k == -1)
		return;

	double i, j;

	if (p == 0)
		i = 112, j = 80;
	else if (p == 1)
		i = 212, j = 80;
	else
		i = 162, j = 180;
	
	draw_square(cr, k, i, j);	
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

	draw_grid_part(cr, 0);
	draw_grid_part(cr, 1);
	draw_grid_part(cr, 2);
	draw_grid_part(cr, 3);
	draw_grid_part(cr, 4);

	draw_trem(cr, 0);
	draw_trem(cr, 1);
	draw_trem(cr, 2);
	draw_trem(cr, 3);
	draw_trem(cr, 4);

	draw_flag(cr, 0);
	draw_flag(cr, 1);
	draw_flag(cr, 2);

	cairo_destroy(cr);

	return FALSE;
}


gboolean
changed (GtkRange* range,
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
	return TRUE;
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

void show_help(char* name)
{
	printf( "Uso: %s\n"
		"     %s -n     (sem sincronizacao)\n"
		"\n\n"
		"Este programa foi feito por Elias e Andre, para a disciplina"
		" de Pratica de\nProgramacao Concorrente (2010.1)\n",
		name, name);

	exit(EXIT_SUCCESS);
}

int parse_opt(int argc, char* argv[])
{
	int nflag = 0, hflag = 0;
	int c;
     
	opterr = 0;

	while ((c = getopt (argc, argv, "hn")) != -1)
		if (c == 'h')
			hflag = 1;
		else if (c == 'n')
			nflag = 1;

	if (hflag == 1)
		show_help(argv[0]);

	return nflag;

}

int
main(int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	int nflag = parse_opt(argc, argv);
 
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkWidget *box = gtk_hbox_new (FALSE, 3);
	GtkWidget *vbox = gtk_vbox_new(TRUE, 3);
	GtkWidget *darea = gtk_drawing_area_new();
	GtkWidget *label = gtk_label_new("Ajustar Velocidade:");

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

	gtk_box_pack_start_defaults (GTK_BOX (vbox), label);
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

	if (nflag)
		begin_threads_nosync();
	else
		begin_threads();
    
	gtk_main ();
   
	return 0;
}
