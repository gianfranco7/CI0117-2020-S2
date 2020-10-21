#include "../view/pthreadmon_ui.h"
static void my_callback(GObject *source_object, GAsyncResult *res, gpointer user_data){}
static void start_async(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable)
{
    pthreadmon();
}

static void start_clicked()
{
    GCancellable *cancellable = g_cancellable_new();
    GTask *task = g_task_new(g_object_new(G_TYPE_OBJECT, NULL), cancellable, my_callback, NULL);
    g_task_run_in_thread(task, start_async);
    g_object_unref(task);    
}

static gboolean display_pthreadmon_data(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    if (p0_pokemon_data_list && p1_pokemon_data_list)
    {


    }
    return TRUE;
}

void build_window(GtkApplication *app)
{
   //WINDOW CONSTRUCTOR
    app_elements->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(app_elements->window), "Pthreadmon");
    gtk_window_set_default_size(GTK_WINDOW(app_elements->window), 1000, 1000);
    gtk_container_set_border_width(GTK_CONTAINER(app_elements->window), 10);
}

void build_grid()
{
    //GRID CONSTRUCTOR
    app_elements->grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(app_elements->grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(app_elements->grid), TRUE);
}

void attach_grid()
{
    ///ATTACH GRID TO WINDOW
    gtk_container_add(GTK_CONTAINER(app_elements->window), app_elements->grid);
}

void initialize_buttons()
{
    app_elements->startButton = gtk_button_new_with_label("Begin Battle");
    g_signal_connect(app_elements->startButton, "clicked", G_CALLBACK(start_clicked), NULL);
}

void initialize_buffers()
{
    app_elements->player1_pokemon1_buffer = gtk_entry_buffer_new("Pokémon 1", 9);
    app_elements->player1_pokemon1 = gtk_entry_new_with_buffer(app_elements->player1_pokemon1_buffer);
    app_elements->player1_pokemon2_buffer = gtk_entry_buffer_new("Pokémon 2", 9);
    app_elements->player1_pokemon2 = gtk_entry_new_with_buffer(app_elements->player1_pokemon2_buffer);
    app_elements->player1_pokemon3_buffer = gtk_entry_buffer_new("Pokémon 3", 9);
    app_elements->player1_pokemon3 = gtk_entry_new_with_buffer(app_elements->player1_pokemon3_buffer);
    app_elements->player2_pokemon1_buffer = gtk_entry_buffer_new("Pokémon 1", 9);
    app_elements->player2_pokemon1 = gtk_entry_new_with_buffer(app_elements->player2_pokemon1_buffer);
    app_elements->player2_pokemon2_buffer = gtk_entry_buffer_new("Pokémon 2", 9);
    app_elements->player2_pokemon2 = gtk_entry_new_with_buffer(app_elements->player2_pokemon2_buffer);
    app_elements->player2_pokemon3_buffer = gtk_entry_buffer_new("Pokémon 3", 9);
    app_elements->player2_pokemon3 = gtk_entry_new_with_buffer(app_elements->player2_pokemon3_buffer);
}

void initialize_labels()
{
    app_elements->player1_label = gtk_label_new("Player 1");
    app_elements->player1_active_label = gtk_label_new("Player 1 Active Pokémon");
    app_elements->player1_active_name = gtk_label_new("Name");
    app_elements->player1_active_hp = gtk_label_new("1500");
    app_elements->player1_active_energy = gtk_label_new("0");
    app_elements->player1_fast_attack = gtk_label_new("Fast Attack");
    app_elements->player1_charged_attack = gtk_label_new("Charged Attack");
    app_elements->player2_label = gtk_label_new("Player 2");
    app_elements->player2_active_label = gtk_label_new("Player 2 Active Pokémon");
    app_elements->player2_active_name = gtk_label_new("Name");
    app_elements->player2_active_hp = gtk_label_new("1500");
    app_elements->player2_active_energy = gtk_label_new("0");
    app_elements->player2_fast_attack = gtk_label_new("Fast Attack");
    app_elements->player2_charged_attack = gtk_label_new("Charged Attack");
}

void attach_widgets()
{
    //ATTACH TO GRID
    //Fila 1
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_label, 1,1,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_active_label, 2,1,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_active_label, 3,1,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_label, 4,1,1,1);
    //Fila 2
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_pokemon1, 1,2,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_active_name, 2,2,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_active_name, 3,2,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_pokemon1, 4,2,1,1);
    //Fila 3
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_pokemon2, 1,3,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_active_hp, 2,3,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_active_hp, 3,3,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_pokemon2, 4,3,1,1);
    //Fila 4
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_pokemon3, 1,4,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_active_energy, 2,4,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_active_energy, 3,4,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_pokemon3, 4,4,1,1);
    //Fila 5
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_fast_attack, 2,5,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_fast_attack, 3,5,1,1);
    //Fila 6
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_charged_attack, 2,6,1,1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_charged_attack, 3,6,1,1);
    //Fila 7
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->startButton, 2,7,2,1);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    build_window(app);
    build_grid();
    attach_grid();
    initialize_buttons();
    initialize_buffers();
    initialize_labels();
    attach_widgets();
    g_timeout_add(33, (GSourceFunc)display_pthreadmon_data, (gpointer)app_elements->window);
    gtk_widget_show_all(app_elements->window);
}

int main(int argc, char *argv[])
{
    app_elements = (app_widgets *)calloc(1, sizeof(app_widgets));
    GtkApplication *app = gtk_application_new("gtk.pthreadmon", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}