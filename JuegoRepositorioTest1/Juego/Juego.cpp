#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

struct Object{
	ALLEGRO_BITMAP *image;
	int x;
	int y;
	int movSpeed;
};

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

Object player;
player.x = 10;
player.y = 10;
player.movSpeed = 4;

Object enemy;
enemy.x = 400;
enemy.y = 0;
enemy.movSpeed = 5;

const float fps = 60.0;

bool done = false;
bool draw = true;

int displayWidth = 800;
int displayHeight = 600;

bool enemieGoingUp = false;

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

player.image = al_load_bitmap("Personaje.png");
enemy.image = al_load_bitmap("Enemigo.png");

if (!player.image || !enemy.image) {
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
			player.y += player.movSpeed;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_UP)){
			player.y -= player.movSpeed;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_RIGHT)) {
			player.x += player.movSpeed;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_LEFT)) {
			player.x -= player.movSpeed;
		}


		if (Collision(player.x, player.y, enemy.x, enemy.y, al_get_bitmap_width(player.image), al_get_bitmap_height(player.image), al_get_bitmap_width(enemy.image), al_get_bitmap_height(enemy.image))){
			done = true;
		}


		if (!enemieGoingUp){
			enemy.y += enemy.movSpeed;
		}
		if (enemieGoingUp){
			enemy.y -= enemy.movSpeed;
		}
		if (enemy.y + al_get_bitmap_height(enemy.image) >= displayHeight){
			enemieGoingUp = true;
		}
		else if (enemy.y <= 0){
			enemieGoingUp = false;
		}

		draw = true;
	}

	if(draw){
		draw = false;
		al_draw_bitmap(player.image, player.x, player.y, 0);
		al_draw_bitmap(enemy.image, enemy.x, enemy.y, 0);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
}

al_destroy_display(display);
al_destroy_bitmap(player.image);
al_destroy_bitmap(enemy.image);
al_destroy_timer(timer);

return 0;
}
