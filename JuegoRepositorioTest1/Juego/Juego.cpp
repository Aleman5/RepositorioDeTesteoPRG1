#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "Menu.h"

// Put this here to use them in the functions <------ By Abecasis Alejandro
const int displayWidth = 800;
const int displayHeight = 600;

// Made Structs to make the code easier to read. <------ By Abecasis Alejandro
// PD: I know that "Player" and "Bullet" are exactly the same, but I prefer like this because its easier to know who is who.
struct Player{
	ALLEGRO_BITMAP *image;
	int x;
	int y;
	int movSpeed;
};
struct Bullet {
	ALLEGRO_BITMAP *image;
	int x;
	int y;
	int movSpeed;
};
struct Enemy {
	ALLEGRO_BITMAP *image;
	int x;
	int y;
	int movSpeed;
	bool enemieGoingUp;
};

// Made Collision easier to fill the parameters. <------ By Abecasis Alejandro
bool Collision(Player &player, Enemy &enemy) {
	if (player.x + al_get_bitmap_width(player.image)  < enemy.x || player.x > enemy.x + al_get_bitmap_width(enemy.image)  ||
		player.y + al_get_bitmap_height(player.image) < enemy.y || player.y > enemy.y + al_get_bitmap_height(enemy.image)) {
		return false;
	}
	else{
		return true;
	}
}

// Made this function to move all the enemies. <------ By Abecasis Alejandro
void CheckMovement(Enemy &enemy) {
	if (!enemy.enemieGoingUp) {
		enemy.y += enemy.movSpeed;
	}
	else {
		enemy.y -= enemy.movSpeed;
	}
	if (enemy.y + al_get_bitmap_height(enemy.image) >= displayHeight) {
		enemy.enemieGoingUp = true;
	}
	else if (enemy.y <= 0) {
		enemy.enemieGoingUp = false;
	}
}

int main(int argc, char **argv) {

ALLEGRO_DISPLAY *display = NULL;

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

const float fps = 60.0;

bool done = false;
bool draw = true;

Menu* menu = new Menu();
if (!menu->Portada())
	done = true;
delete menu;

// Elements creation
Player player;
player.x = 10;
player.y = 10;
player.movSpeed = 4;

Enemy enemy1;
enemy1.x = 200;
enemy1.y = 20;
enemy1.movSpeed = 2;
enemy1.enemieGoingUp = false;

Enemy enemy2;
enemy2.x = 350;
enemy2.y = 300;
enemy2.movSpeed = 4;
enemy2.enemieGoingUp = false;

Enemy enemy3;
enemy3.x = 500;
enemy3.y = 250;
enemy3.movSpeed = 8;
enemy3.enemieGoingUp = true;

Enemy enemy4;
enemy4.x = 650;
enemy4.y = 0;
enemy4.movSpeed = 9;
enemy4.enemieGoingUp = false;



// Images load
player.image = al_load_bitmap("Personaje.png");
enemy1.image = al_load_bitmap("Enemigo.png");
enemy2.image = al_load_bitmap("Enemigo.png");
enemy3.image = al_load_bitmap("Enemigo.png");
enemy4.image = al_load_bitmap("Enemigo.png");

// Images check
if (!player.image || !enemy1.image || !enemy2.image || !enemy3.image || !enemy4.image) {
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

		// Checks player's movement's input
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


		// Checks Collision Player - Enemies
		if (Collision(player, enemy1)){
			done = true;
		}
		else if (Collision(player, enemy2)) {
			done = true;
		}
		else if (Collision(player, enemy3)) {
			done = true;
		}
		else if (Collision(player, enemy4)) {
			done = true;
		}

		CheckMovement(enemy1);
		CheckMovement(enemy2);
		CheckMovement(enemy3);
		CheckMovement(enemy4);

		draw = true;
	}

	if(draw){
		draw = false;
		al_draw_bitmap(player.image, player.x, player.y, 0);
		al_draw_bitmap(enemy1.image, enemy1.x, enemy1.y, 0);
		al_draw_bitmap(enemy2.image, enemy2.x, enemy2.y, 0);
		al_draw_bitmap(enemy3.image, enemy3.x, enemy3.y, 0);
		al_draw_bitmap(enemy4.image, enemy4.x, enemy4.y, 0);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
}

al_destroy_display(display);
al_destroy_bitmap(player.image);
al_destroy_bitmap(enemy1.image);
al_destroy_bitmap(enemy2.image);
al_destroy_bitmap(enemy3.image);
al_destroy_bitmap(enemy4.image);
al_destroy_timer(timer);

return 0;
}
