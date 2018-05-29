#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"


int main(int argc, char **argv) {

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *player = NULL;
ALLEGRO_BITMAP *enemie = NULL;

if (!al_init()) {
	al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	return 0;
}

if (!al_init_image_addon()) {
	al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	return 0;
}

display = al_create_display(800, 600);

if (!display) {
	al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	return 0;
}

enemie = al_load_bitmap("Enemigo.png");
player = al_load_bitmap("Personaje.png");

if (!player || !enemie) {
	al_show_native_message_box(display, "Error", "Error", "Failed to load image!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	al_destroy_display(display);
	return 0;
}


al_draw_bitmap(player, 0, 0, 0);
al_draw_bitmap(enemie, 400, 10, 0);

al_flip_display();
al_rest(5.0);

al_destroy_display(display);
al_destroy_bitmap(player);
al_destroy_bitmap(enemie);

return 0;
}
