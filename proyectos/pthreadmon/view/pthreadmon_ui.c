#include "../model/pthreadmon_ui.h"
#include "../audio_handler/audio_handler.c"
#include <string.h>
static void my_callback(GObject *source_object, GAsyncResult *res, gpointer user_data) {}
static void start_async(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable)
{
    pthread_t audio_thread;
    pthread_create(&audio_thread, NULL, play_theme, NULL);
    pthreadmon();
}


void get_user_input()
{
    p0_pokemon_data_list[0].id =
        (int)strtoul(gtk_entry_buffer_get_text(app_elements->player1_pokemon1_buffer), NULL, 10);
    p0_pokemon_data_list[1].id =
        (int)strtoul(gtk_entry_buffer_get_text(app_elements->player1_pokemon2_buffer), NULL, 10);
    p0_pokemon_data_list[2].id =
        (int)strtoul(gtk_entry_buffer_get_text(app_elements->player1_pokemon3_buffer), NULL, 10);
    p1_pokemon_data_list[0].id =
        (int)strtoul(gtk_entry_buffer_get_text(app_elements->player2_pokemon1_buffer), NULL, 10);
    p1_pokemon_data_list[1].id =
        (int)strtoul(gtk_entry_buffer_get_text(app_elements->player2_pokemon2_buffer), NULL, 10);
    p1_pokemon_data_list[2].id =
        (int)strtoul(gtk_entry_buffer_get_text(app_elements->player2_pokemon3_buffer), NULL, 10);
}

/*
int get_player1_pokemon(pokemon_data_t *pokemon_data_list)
{
	signed int validation_array[AMOUNT_OF_POKEMON] = {-1, -1, -1};

	int scanned_id1 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player1_pokemon1_buffer), NULL, 10);
	while (validate_pokemon(scanned_id1, validation_array) == 1)
	{
		app_elements->player1_pokemon1_buffer = gtk_entry_buffer_new("Repeated ID", 11);
        scanned_id1 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player1_pokemon1_buffer), NULL, 10);
	}
    validation_array[0] = scanned_id1;
	pokemon_data_list[0].id = scanned_id1;

    int scanned_id2 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player1_pokemon2_buffer), NULL, 10);
    while (validate_pokemon(scanned_id2, validation_array) == 1)
	{
		app_elements->player1_pokemon2_buffer = gtk_entry_buffer_new("Repeated ID", 11);
        scanned_id2 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player1_pokemon2_buffer), NULL, 10);
	}
	validation_array[1] = scanned_id2;
	pokemon_data_list[1].id = scanned_id2;

    int scanned_id3 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player1_pokemon3_buffer), NULL, 10);
    while (validate_pokemon(scanned_id3, validation_array) == 1)
	{
		app_elements->player1_pokemon3_buffer = gtk_entry_buffer_new("Repeated ID", 11);
        scanned_id3 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player1_pokemon3_buffer), NULL, 10);
	}
    validation_array[2] = scanned_id3;
	pokemon_data_list[2].id = scanned_id3;
    return 1;
}

int get_player2_pokemon(pokemon_data_t *pokemon_data_list)
{
	signed int validation_array[AMOUNT_OF_POKEMON] = {-1, -1, -1};

	int scanned_id1 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player2_pokemon1_buffer), NULL, 10);
	while (validate_pokemon(scanned_id1, validation_array) == 1)
	{
		app_elements->player2_pokemon1_buffer = gtk_entry_buffer_new("Repeated ID", 11);
        scanned_id1 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player2_pokemon1_buffer), NULL, 10);
	}
    validation_array[0] = scanned_id1;
	pokemon_data_list[0].id = scanned_id1;

    int scanned_id2 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player2_pokemon2_buffer), NULL, 10);
    while (validate_pokemon(scanned_id2, validation_array) == 1)
	{
		app_elements->player2_pokemon2_buffer = gtk_entry_buffer_new("Repeated ID", 11);
        scanned_id2 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player2_pokemon2_buffer), NULL, 10);
	}
	validation_array[1] = scanned_id2;
	pokemon_data_list[1].id = scanned_id2;

    int scanned_id3 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player2_pokemon3_buffer), NULL, 10);
    while (validate_pokemon(scanned_id3, validation_array) == 1)
	{
		app_elements->player2_pokemon3_buffer = gtk_entry_buffer_new("Repeated ID", 11);
        scanned_id3 = (int)strtoul(gtk_entry_buffer_get_text(app_elements->player2_pokemon3_buffer), NULL, 10);
	}
    validation_array[2] = scanned_id3;
	pokemon_data_list[2].id = scanned_id3;
    return 1;
}

void get_user_input()
{
    while(get_player1_pokemon(p0_pokemon_data_list) !=1 && get_player2_pokemon(p1_pokemon_data_list)!=1)
    {

    }
}
*/
void set_image(GtkWidget *image, int id)
{
    char pokemon_name[15] = "";
    char *file_extension = ".png";
    char image_path[30] = "sprites/";
    sprintf(pokemon_name, "%s", get_pokemon_species_name(id));
    strcat(pokemon_name, file_extension);
    strcat(image_path, pokemon_name);
    gtk_image_set_from_file(GTK_IMAGE(image), image_path);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image),
                              gdk_pixbuf_scale_simple(gtk_image_get_pixbuf(GTK_IMAGE(image)), 56, 42, GDK_INTERP_NEAREST));
}

static void start_clicked()
{
    GCancellable *cancellable = g_cancellable_new();
    GTask *task = g_task_new(g_object_new(G_TYPE_OBJECT, NULL), cancellable, my_callback, NULL);
    get_user_input();
    g_task_run_in_thread(task, start_async);
    g_object_unref(task);
}

int get_activity_level(pokemon_data_t *pokemon_data_list)
{
    for (int i = 0; i < AMOUNT_OF_POKEMON; ++i)
    {
        if (pokemon_data_list[i].active == 1)
        {
            return 0;
        }
    }
    return 1;
}

static gboolean display_pthreadmon_data(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
    if (p0_pokemon_data_list && p1_pokemon_data_list)
    {
        char battle_time[20];
        sprintf(battle_time, "%3.0lf", get_battle_time());
        gtk_label_set_text(GTK_LABEL(app_elements->battle_time_label), battle_time);
        if (p0_pokemon_data_list[0].active)
        {
            char hp[4];
            sprintf(hp, "%.0lf", p0_pokemon_data_list[0].hp);
            char energy[4];
            sprintf(energy, "%.0lf", p0_pokemon_data_list[0].power);
            gtk_label_set_text(GTK_LABEL(app_elements->player1_active_name),
                               (char *)get_pokemon_species_name(p0_pokemon_data_list[0].id));
            gtk_label_set_text(GTK_LABEL(app_elements->player1_active_hp), hp);
            gtk_label_set_text(GTK_LABEL(app_elements->player1_active_energy), energy);
            gtk_label_set_text(GTK_LABEL(app_elements->player1_fast_attack),
                               (char *)get_move_name(get_pokemon_fast_move_id(p0_pokemon_data_list[0].id)));
            gtk_label_set_text(GTK_LABEL(app_elements->player1_charged_attack),
                               (char *)get_move_name(get_pokemon_charged_move_id(p0_pokemon_data_list[0].id)));
            char pokemon_time[100];
            sprintf(pokemon_time, "%3.0lf", walltime_elapsed(&p0_pokemon_data_list[0].time_lived));
            gtk_label_set_text(GTK_LABEL(app_elements->player1_pokemon1_time), pokemon_time);
            set_image(app_elements->player1_active_image, p0_pokemon_data_list[0].id);
        }
        if (p0_pokemon_data_list[1].active)
        {
            char hp[4];
            sprintf(hp, "%.0lf", p0_pokemon_data_list[1].hp);
            char energy[4];
            sprintf(energy, "%.0lf", p0_pokemon_data_list[1].power);
            gtk_label_set_text(GTK_LABEL(app_elements->player1_active_name),
                               (char *)get_pokemon_species_name(p0_pokemon_data_list[1].id));
            gtk_label_set_text(GTK_LABEL(app_elements->player1_active_hp), hp);
            gtk_label_set_text(GTK_LABEL(app_elements->player1_active_energy), energy);
            gtk_label_set_text(GTK_LABEL(app_elements->player1_fast_attack),
                               (char *)get_move_name(get_pokemon_fast_move_id(p0_pokemon_data_list[1].id)));
            gtk_label_set_text(GTK_LABEL(app_elements->player1_charged_attack),
                               (char *)get_move_name(get_pokemon_charged_move_id(p0_pokemon_data_list[1].id)));
            char pokemon_time[100];
            sprintf(pokemon_time, "%3.0lf", walltime_elapsed(&p0_pokemon_data_list[1].time_lived));
            gtk_label_set_text(GTK_LABEL(app_elements->player1_pokemon2_time), pokemon_time);
            set_image(app_elements->player1_active_image, p0_pokemon_data_list[1].id);
        }
        if (p0_pokemon_data_list[2].active)
        {
            char hp[4];
            sprintf(hp, "%.0lf", p0_pokemon_data_list[2].hp);
            char energy[4];
            sprintf(energy, "%.0lf", p0_pokemon_data_list[2].power);
            gtk_label_set_text(GTK_LABEL(app_elements->player1_active_name),
                               (char *)get_pokemon_species_name(p0_pokemon_data_list[2].id));
            gtk_label_set_text(GTK_LABEL(app_elements->player1_active_hp), hp);
            gtk_label_set_text(GTK_LABEL(app_elements->player1_active_energy), energy);
            gtk_label_set_text(GTK_LABEL(app_elements->player1_fast_attack),
                               (char *)get_move_name(get_pokemon_fast_move_id(p0_pokemon_data_list[2].id)));
            gtk_label_set_text(GTK_LABEL(app_elements->player1_charged_attack),
                               (char *)get_move_name(get_pokemon_charged_move_id(p0_pokemon_data_list[2].id)));
            char pokemon_time[100];
            sprintf(pokemon_time, "%3.0lf", walltime_elapsed(&p0_pokemon_data_list[2].time_lived));
            gtk_label_set_text(GTK_LABEL(app_elements->player1_pokemon3_time), pokemon_time);
            set_image(app_elements->player1_active_image, p0_pokemon_data_list[2].id);
        }
        if (p1_pokemon_data_list[0].active)
        {
            char hp[4];
            sprintf(hp, "%.0lf", p1_pokemon_data_list[0].hp);
            char energy[4];
            sprintf(energy, "%.0lf", p1_pokemon_data_list[0].power);
            gtk_label_set_text(GTK_LABEL(app_elements->player2_active_name),
                               (char *)get_pokemon_species_name(p1_pokemon_data_list[0].id));
            gtk_label_set_text(GTK_LABEL(app_elements->player2_active_hp), hp);
            gtk_label_set_text(GTK_LABEL(app_elements->player2_active_energy), energy);
            gtk_label_set_text(GTK_LABEL(app_elements->player2_fast_attack),
                               (char *)get_move_name(get_pokemon_fast_move_id(p1_pokemon_data_list[0].id)));
            gtk_label_set_text(GTK_LABEL(app_elements->player2_charged_attack),
                               (char *)get_move_name(get_pokemon_charged_move_id(p1_pokemon_data_list[0].id)));
            char pokemon_time[100];
            sprintf(pokemon_time, "%3.0lf", walltime_elapsed(&p1_pokemon_data_list[0].time_lived));
            gtk_label_set_text(GTK_LABEL(app_elements->player2_pokemon1_time), pokemon_time);
            set_image(app_elements->player2_active_image, p1_pokemon_data_list[0].id);
        }
        if (p1_pokemon_data_list[1].active)
        {
            char hp[4];
            sprintf(hp, "%.0lf", p1_pokemon_data_list[1].hp);
            char energy[4];
            sprintf(energy, "%.0lf", p1_pokemon_data_list[1].power);
            gtk_label_set_text(GTK_LABEL(app_elements->player2_active_name),
                               (char *)get_pokemon_species_name(p1_pokemon_data_list[1].id));
            gtk_label_set_text(GTK_LABEL(app_elements->player2_active_hp), hp);
            gtk_label_set_text(GTK_LABEL(app_elements->player2_active_energy), energy);
            gtk_label_set_text(GTK_LABEL(app_elements->player2_fast_attack),
                               (char *)get_move_name(get_pokemon_fast_move_id(p1_pokemon_data_list[1].id)));
            gtk_label_set_text(GTK_LABEL(app_elements->player2_charged_attack),
                               (char *)get_move_name(get_pokemon_charged_move_id(p1_pokemon_data_list[1].id)));
            char pokemon_time[100];
            sprintf(pokemon_time, "%3.0lf", walltime_elapsed(&p1_pokemon_data_list[1].time_lived));
            gtk_label_set_text(GTK_LABEL(app_elements->player2_pokemon2_time), pokemon_time);
            set_image(app_elements->player2_active_image, p1_pokemon_data_list[1].id);
        }
        if (p1_pokemon_data_list[2].active)
        {
            char hp[4];
            sprintf(hp, "%.0lf", p1_pokemon_data_list[2].hp);
            char energy[4];
            sprintf(energy, "%.0lf", p1_pokemon_data_list[2].power);
            gtk_label_set_text(GTK_LABEL(app_elements->player2_active_name),
                               (char *)get_pokemon_species_name(p1_pokemon_data_list[2].id));
            gtk_label_set_text(GTK_LABEL(app_elements->player2_active_hp), hp);
            gtk_label_set_text(GTK_LABEL(app_elements->player2_active_energy), energy);
            gtk_label_set_text(GTK_LABEL(app_elements->player2_fast_attack),
                               (char *)get_move_name(get_pokemon_fast_move_id(p1_pokemon_data_list[2].id)));
            gtk_label_set_text(GTK_LABEL(app_elements->player2_charged_attack),
                               (char *)get_move_name(get_pokemon_charged_move_id(p1_pokemon_data_list[2].id)));
            char pokemon_time[100];
            sprintf(pokemon_time, "%3.0lf", walltime_elapsed(&p1_pokemon_data_list[2].time_lived));
            gtk_label_set_text(GTK_LABEL(app_elements->player2_pokemon3_time), pokemon_time);
            set_image(app_elements->player2_active_image, p1_pokemon_data_list[2].id);
        }
        if (winner_winner_chicken_dinner() == 1)
        {
            char *winner = "Winner Player 1";
            gtk_label_set_text(GTK_LABEL(app_elements->winner_label), winner);
        }
        if (winner_winner_chicken_dinner() == 2)
        {
            char *winner = "Winner Player 2";
            gtk_label_set_text(GTK_LABEL(app_elements->winner_label), winner);
        }
    }
    return TRUE;
}

void build_window(GtkApplication *app)
{
    app_elements->window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(app_elements->window), "Pokémon Battle Simulator");
    gtk_window_set_default_size(GTK_WINDOW(app_elements->window), 1000, 1000);
    gtk_container_set_border_width(GTK_CONTAINER(app_elements->window), 10);
}

void build_grid()
{
    app_elements->grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(app_elements->grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(app_elements->grid), TRUE);
}

void attach_grid()
{
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

    app_elements->winner_label = gtk_label_new("Winner");
    app_elements->battle_time_label = gtk_label_new("Battle time");

    app_elements->player1_pokemon1_time = gtk_label_new("Pokémon 1 Time lived");
    app_elements->player1_pokemon2_time = gtk_label_new("Pokémon 2 Time lived");
    app_elements->player1_pokemon3_time = gtk_label_new("Pokémon 3 Time lived");
    app_elements->player2_pokemon1_time = gtk_label_new("Pokémon 1 Time lived");
    app_elements->player2_pokemon2_time = gtk_label_new("Pokémon 2 Time lived");
    app_elements->player2_pokemon3_time = gtk_label_new("Pokémon 3 Time lived");
}

void initialize_images()
{
    app_elements->player1_active_image = gtk_image_new();
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_active_image, 2, 7, 1, 2);
    app_elements->player2_active_image = gtk_image_new();
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_active_image, 3, 7, 1, 2);
}

void attach_widgets()
{
    //Fila 1
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_active_label, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_active_label, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_label, 4, 1, 1, 1);
    //Fila 2
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_pokemon1, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_active_name, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_active_name, 3, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_pokemon1, 4, 2, 1, 1);
    //Fila 3
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_pokemon2, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_active_hp, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_active_hp, 3, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_pokemon2, 4, 3, 1, 1);
    //Fila 4
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_pokemon3, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_active_energy, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_active_energy, 3, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_pokemon3, 4, 4, 1, 1);
    //Fila 5
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_fast_attack, 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_fast_attack, 3, 5, 1, 1);
    //Fila 6
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_pokemon1_time, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_charged_attack, 2, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_charged_attack, 3, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_pokemon1_time, 4, 6, 1, 1);
    //Fila 7
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_pokemon2_time, 1, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->winner_label, 2, 7, 2, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_pokemon2_time, 4, 7, 1, 1);
    //Fila 8
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player1_pokemon3_time, 1, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->battle_time_label, 2, 8, 2, 1);
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->player2_pokemon3_time, 4, 8, 1, 1);
    //Fila 9
    gtk_grid_attach(GTK_GRID(app_elements->grid), app_elements->startButton, 2, 9, 2, 1);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    build_window(app);
    build_grid();
    attach_grid();
    initialize_buttons();
    initialize_buffers();
    initialize_labels();
    initialize_images();
    attach_widgets();
    g_timeout_add(33, (GSourceFunc)display_pthreadmon_data, (gpointer)app_elements->window);
    gtk_widget_show_all(app_elements->window);
}

int main(int argc, char *argv[])
{
    allocate_dynamic_memory();
    initialize_synchronization_structures();
    initialize_data();
    app_elements = (app_widgets *)calloc(1, sizeof(app_widgets));
    GtkApplication *app = gtk_application_new("gtk.pthreadmon", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    free(app_elements);
    return status;
}