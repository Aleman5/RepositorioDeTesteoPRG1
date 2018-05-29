#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"


bool Collision(int pX, int pY, int eX, int eY, int pWidth, int pHeight, int eWidth, int eHeight){
	if (pX + pWidth<eX || pX > eX + eWidth || pY + pHeight < eY || pY > eY + eHeight){
		return false;
	}
	else{
		return true;
	}
}


int main(int argc, char **argv) {

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_BITMAP *player = NULL;
ALLEGRO_BITMAP *enemie = NULL;

const float fps = 60.0;

bool done = false;
bool draw = true;

int displayWidth = 800;
int displayHeight = 600;

int playerX = 10;
int playerY = 10;
int playerMovementSpeed = 4;

bool enemieGoingUp = false;
int enemieX = 400;
int enemieY = 0;
int enemieMovementSpeed = 5;

if (!al_init()) {
	return 0;
}

if (!al_init_image_addon()) {
	return 0;
}

display = al_create_display(displayWidth, displayHeight);

if (!display) {
	return 0;
}

enemie = al_load_bitmap("Enemigo.png");
player = al_load_bitmap("Personaje.png");

if (!player || !enemie) {
	al_destroy_display(display);
	return 0;
}

al_install_keyboard();

ALLEGRO_KEYBOARD_STATE keystate;
ALLEGRO_TIMER *timer = al_create_timer(1.0/fps);
ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
al_register_event_source(eventQueue, al_get_keyboard_event_source());
al_register_event_source(eventQueue, al_get_timer_event_source(timer));

al_start_timer(timer);

while (!done){
	ALLEGRO_EVENT events;
	al_wait_for_event(eventQueue, &events);

	if (events.type == ALLEGRO_EVENT_KEY_UP){
		switch (events.keyboard.keycode){
		case ALLEGRO_KEY_ESCAPE:
			done = true;
			break;
		}
	}

	if (events.type == ALLEGRO_EVENT_TIMER){

		al_get_keyboard_state(&keystate);
		if (al_key_down(&keystate, ALLEGRO_KEY_DOWN)){
			playerY += playerMovementSpeed;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_UP)){
			playerY -= playerMovementSpeed;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_RIGHT)) {
			playerX += playerMovementSpeed;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_LEFT)) {
			playerX -= playerMovementSpeed;
		}


		if (Collision(playerX, playerY, enemieX, enemieY, al_get_bitmap_width(player), al_get_bitmap_height(player), al_get_bitmap_width(enemie), al_get_bitmap_height(enemie))){
			done = true;
		}


		if (!enemieGoingUp){
			enemieY += enemieMovementSpeed;
		}
		if (enemieGoingUp){
			enemieY -= enemieMovementSpeed;
		}
		if (enemieY + al_get_bitmap_height(enemie) >= displayHeight){
			enemieGoingUp = true;
		}
		else if (enemieY <= 0){
			enemieGoingUp = false;
		}

		draw = true;
	}

	if(draw){
		draw = false;
		al_draw_bitmap(player, playerX, playerY, 0);
		al_draw_bitmap(enemie, enemieX, enemieY, 0);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
}

al_destroy_display(display);
al_destroy_bitmap(player);
al_destroy_bitmap(enemie);
al_destroy_timer(timer);

return 0;
}
