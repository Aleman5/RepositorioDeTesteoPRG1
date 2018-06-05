#include <stdio.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include "Menu.h"

// Put this here to use them in the functions <------ By Abecasis Alejandro
const int displayWidth = 800;
const int displayHeight = 600;

enum DirectionPlayer {
	UP,
	DOWN,
	RIGHT,
	LEFT
};

// Made Structs to make the code easier to read. <------ By Abecasis Alejandro
struct Player{
	ALLEGRO_BITMAP *image;
	int x;
	int y;
	int oriX; // Original 'x' position.
	int oriY; // Original 'y' position.
	int movSpeed;
	int health;
	ALLEGRO_SAMPLE *sndPlaDamaged;
	ALLEGRO_SAMPLE_INSTANCE *sndPlaDamagedInstance;
	DirectionPlayer dir;
};
struct Bullet {
	ALLEGRO_BITMAP *image;
	int x;
	int y;
	int movSpeed;
	bool direction; // 'false' moves horizontal, 'true' moves vertical
	int dirVel;   // '1' moves right or down, '-1' moves up or left, '0' doesn´t move
	ALLEGRO_SAMPLE *sndShoot;
	ALLEGRO_SAMPLE_INSTANCE *sndShootInstance;
};
struct Enemy {
	ALLEGRO_BITMAP *image;
	int x;
	int y;
	int movSpeed;
	bool enemieGoingUp;
	bool isAlive;
};

// Made Collision easier to fill the parameters. <------ By Abecasis Alejandro
bool CollisionPlaEne(Player &player, Enemy &enemy) {
	if (player.x + al_get_bitmap_width(player.image)  < enemy.x || player.x > enemy.x + al_get_bitmap_width(enemy.image)  ||
		player.y + al_get_bitmap_height(player.image) < enemy.y || player.y > enemy.y + al_get_bitmap_height(enemy.image)) {
		return false;
	}
	else{
		player.health--;
		player.x = player.oriX;
		player.y = player.oriY;
		al_play_sample_instance(player.sndPlaDamagedInstance);
		return true;
	}
}

bool CollisionBulEne(Bullet &bullet, Enemy &enemy) {
	if (bullet.x + al_get_bitmap_width(bullet.image)  < enemy.x || bullet.x > enemy.x + al_get_bitmap_width(enemy.image) ||
		bullet.y + al_get_bitmap_height(bullet.image) < enemy.y || bullet.y > enemy.y + al_get_bitmap_height(enemy.image)) {
		return false;
	}
	else {
		enemy.isAlive = false;
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

al_install_keyboard();
al_install_audio();
al_init_acodec_addon();

al_reserve_samples(3);

const float fps = 60.0;

bool done = false;
bool draw = true;

Menu* menu = new Menu();
if (!menu->Portada())
	done = true;
delete menu;

ALLEGRO_SAMPLE *ambientMusic = al_load_sample("ambientMusic.wav");
ALLEGRO_SAMPLE_INSTANCE *ambientMusicInstance = al_create_sample_instance(ambientMusic);
al_set_sample_instance_playmode(ambientMusicInstance, ALLEGRO_PLAYMODE_LOOP);
al_attach_sample_instance_to_mixer(ambientMusicInstance, al_get_default_mixer());



// Elements creation
Player player;
player.x = player.oriX = 10;
player.y = player.oriY = 10;
player.movSpeed = 4;
player.health = 3;
player.dir = RIGHT;
player.sndPlaDamaged = al_load_sample("playerDamaged.wav");
player.sndPlaDamagedInstance = al_create_sample_instance(player.sndPlaDamaged);
al_set_sample_instance_playmode(ambientMusicInstance, ALLEGRO_PLAYMODE_ONCE);
al_attach_sample_instance_to_mixer(ambientMusicInstance, al_get_default_mixer());

Bullet bullet;
bullet.x = -8;
bullet.y = -8;
bullet.movSpeed = 8;
bullet.direction = false;
bullet.dirVel = 0;
bullet.sndShoot = al_load_sample("shoot.wav");
bullet.sndShootInstance = al_create_sample_instance(bullet.sndShoot);
al_set_sample_instance_playmode(ambientMusicInstance, ALLEGRO_PLAYMODE_ONCE);
al_attach_sample_instance_to_mixer(ambientMusicInstance, al_get_default_mixer());

Enemy enemy1;
enemy1.x = 200;
enemy1.y = 20;
enemy1.movSpeed = 2;
enemy1.enemieGoingUp = false;
enemy1.isAlive = true;

Enemy enemy2;
enemy2.x = 350;
enemy2.y = 300;
enemy2.movSpeed = 4;
enemy2.enemieGoingUp = false;
enemy2.isAlive = true;

Enemy enemy3;
enemy3.x = 500;
enemy3.y = 250;
enemy3.movSpeed = 8;
enemy3.enemieGoingUp = true;
enemy3.isAlive = true;

Enemy enemy4;
enemy4.x = 650;
enemy4.y = 0;
enemy4.movSpeed = 9;
enemy4.enemieGoingUp = false;
enemy4.isAlive = true;

// Images load
player.image = al_load_bitmap("Personaje.png");
bullet.image = al_load_bitmap("RedBullet.png");
enemy1.image = al_load_bitmap("Enemigo.png");
enemy2.image = al_load_bitmap("Enemigo.png");
enemy3.image = al_load_bitmap("Enemigo.png");
enemy4.image = al_load_bitmap("Enemigo.png");

// Images check
if (!player.image || !bullet.image || !enemy1.image || !enemy2.image || !enemy3.image || !enemy4.image) {
	al_destroy_display(display);
	return 0;
}

ALLEGRO_KEYBOARD_STATE keystate;
ALLEGRO_TIMER *timer = al_create_timer(1.0/fps);
ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
al_register_event_source(eventQueue, al_get_keyboard_event_source());
al_register_event_source(eventQueue, al_get_timer_event_source(timer));

al_play_sample_instance(ambientMusicInstance);

al_start_timer(timer);

while (!done){
	ALLEGRO_EVENT events;
	al_wait_for_event(eventQueue, &events);

	if (events.type == ALLEGRO_EVENT_KEY_UP)
		switch (events.keyboard.keycode){
		case ALLEGRO_KEY_ESCAPE:
			done = true;
			break;
		}

	if (events.type == ALLEGRO_EVENT_TIMER){

		// Checks player's movement's input
		al_get_keyboard_state(&keystate);
		if (al_key_down(&keystate, ALLEGRO_KEY_DOWN)){
			if(player.y + al_get_bitmap_height(player.image) + player.movSpeed < displayHeight)
				player.y += player.movSpeed;
			player.dir = DOWN;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_UP)){
			if(player.y - player.movSpeed > 0)
				player.y -= player.movSpeed;
			player.dir = UP;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_RIGHT)){
			if (player.x + al_get_bitmap_width(player.image) + player.movSpeed < displayWidth)
				player.x += player.movSpeed;
			player.dir = RIGHT;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_LEFT)){
			if (player.x - player.movSpeed > 0)
				player.x -= player.movSpeed;
			player.dir = LEFT;
		}
		if (al_key_down(&keystate, ALLEGRO_KEY_SPACE)){
			if (bullet.dirVel == 0){
				al_play_sample_instance(bullet.sndShootInstance);
				switch (player.dir) // Checks the actually player's moving direction
				{
				case UP:
					bullet.direction = true;
					bullet.dirVel = -1;
					break;
				case DOWN:
					bullet.direction = true;
					bullet.dirVel = 1;
					break;
				case RIGHT:
					bullet.direction = false;
					bullet.dirVel = 1;
					break;
				case LEFT:
					bullet.direction = false;
					bullet.dirVel = -1;
					break;
				}
				if (!bullet.direction){		// Horizontal Axis
					if (bullet.dirVel > 0) {	// Right
						bullet.x = player.x + al_get_bitmap_width(player.image);
						bullet.y = player.y + al_get_bitmap_height(player.image) / 2;
					}
					else {						// Left
						bullet.x = player.x;
						bullet.y = player.y + al_get_bitmap_height(player.image) / 2;
					}
				}
				else {						// Vertical Axis
					if (bullet.dirVel > 0) {	// Down
						bullet.x = player.x + al_get_bitmap_width(player.image) / 2;
						bullet.y = player.y + al_get_bitmap_height(player.image);
					}
					else {						// Up
						bullet.x = player.x + al_get_bitmap_width(player.image) / 2;
						bullet.y = player.y;
					}
				}
			}
		}

		// Checks Bullet's movement
		if (bullet.dirVel != 0){
			if (bullet.direction == false) {
				if (bullet.x > -al_get_bitmap_width(bullet.image) && bullet.x + al_get_bitmap_width(bullet.image) < displayWidth + al_get_bitmap_width(bullet.image))
					bullet.x += bullet.movSpeed * bullet.dirVel;
				else
					bullet.dirVel = 0;
			}
			else {
				if (bullet.y > -al_get_bitmap_width(bullet.image) && bullet.y + al_get_bitmap_width(bullet.image) < displayHeight + al_get_bitmap_width(bullet.image))
					bullet.y += bullet.movSpeed * bullet.dirVel;
				else
					bullet.dirVel = 0;
			}
		}

		// Checks all the functions where the enemies participates in
		if(enemy1.isAlive){
			CheckMovement(enemy1);
			CollisionPlaEne(player, enemy1);
			CollisionBulEne(bullet, enemy1);
		}
		if(enemy2.isAlive){
			CheckMovement(enemy2);
			CollisionPlaEne(player, enemy2);
			CollisionBulEne(bullet, enemy2);
		}
		if(enemy3.isAlive){
			CheckMovement(enemy3);
			CollisionPlaEne(player, enemy3);
			CollisionBulEne(bullet, enemy3);
		}
		if(enemy4.isAlive){
			CheckMovement(enemy4);
			CollisionPlaEne(player, enemy4);
			CollisionBulEne(bullet, enemy4);
		}

		// Checks while loop state
		if (player.health > 0)
			draw = true;
		else
			done = true;
	}

	if(draw){
		draw = false;
		al_draw_bitmap(player.image, player.x, player.y, 0);
		al_draw_bitmap(bullet.image, bullet.x, bullet.y, 0);
		if(enemy1.isAlive)
			al_draw_bitmap(enemy1.image, enemy1.x, enemy1.y, 0);
		if(enemy2.isAlive)
			al_draw_bitmap(enemy2.image, enemy2.x, enemy2.y, 0);
		if(enemy3.isAlive)
			al_draw_bitmap(enemy3.image, enemy3.x, enemy3.y, 0);
		if(enemy4.isAlive)
			al_draw_bitmap(enemy4.image, enemy4.x, enemy4.y, 0);
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
}

al_destroy_display(display);
al_destroy_bitmap(player.image);
al_destroy_bitmap(bullet.image);
al_destroy_bitmap(enemy1.image);
al_destroy_bitmap(enemy2.image);
al_destroy_bitmap(enemy3.image);
al_destroy_bitmap(enemy4.image);
al_destroy_sample(player.sndPlaDamaged);
al_destroy_sample(ambientMusic);
al_destroy_sample_instance(ambientMusicInstance);
al_destroy_timer(timer);

return 0;
}
