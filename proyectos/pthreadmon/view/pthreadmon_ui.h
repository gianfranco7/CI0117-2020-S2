#include <gtk/gtk.h>
#include "../controller/pthreadmon.c"
GtkWidget *testInput;
GtkEntryBuffer *testBuffer;

typedef struct
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *startButton;
    GtkWidget *player1_label;
    GtkWidget *player1_pokemon1;
    GtkWidget *player1_pokemon2;
    GtkWidget *player1_pokemon3;
    GtkWidget *player1_active_label;
    GtkWidget *player1_active_name;
    GtkWidget *player1_active_hp;
    GtkWidget *player1_active_energy;
    GtkWidget *player1_fast_attack;
    GtkWidget *player1_charged_attack;
    GtkWidget *player2_label;
    GtkWidget *player2_pokemon1;
    GtkWidget *player2_pokemon2;
    GtkWidget *player2_pokemon3;
    GtkWidget *player2_active_label;
    GtkWidget *player2_active_name;
    GtkWidget *player2_active_hp;
    GtkWidget *player2_active_energy;
    GtkWidget *player2_fast_attack;
    GtkWidget *player2_charged_attack;

    GtkEntryBuffer *player1_pokemon1_buffer;
    GtkEntryBuffer *player1_pokemon2_buffer;
    GtkEntryBuffer *player1_pokemon3_buffer;
    GtkEntryBuffer *player2_pokemon1_buffer;
    GtkEntryBuffer *player2_pokemon2_buffer;
    GtkEntryBuffer *player2_pokemon3_buffer;
} app_widgets;

typedef struct
{
    GtkWidget *player_pokemon_labels[AMOUNT_OF_POKEMON];
    GtkWidget *player_active_pokemon_name_label;
    GtkWidget *player_active_pokemon_hp_label;
    GtkWidget *player_active_pokemon_energy_label;
} player_labels;

typedef struct
{
    GtkWidget *attack_box;
} attack_info_box;

app_widgets *app_elements;