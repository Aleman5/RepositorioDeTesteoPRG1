#include "Menu.h"

// Menu_Image proportions (600 x 400)

Menu::Menu() {

}
Menu::~Menu() {

}
bool Menu::Portada() {

	ALLEGRO_BITMAP* menuImage;
	menuImage = al_load_bitmap("Menu_Image.png");
	
	bool stop = false;
	bool start = false;

	al_install_keyboard();

	ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(menuImage, 100, 100, 0);
	al_flip_display();

	while (!stop) {
		ALLEGRO_EVENT events;
		al_wait_for_event(eventQueue, &events);

		if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (events.keyboard.keycode) {
			case ALLEGRO_KEY_ENTER:
				stop = true;
				start = true;
				break;
			case ALLEGRO_KEY_ESCAPE:
				stop = true;
				break;
			}
		}
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_bitmap(menuImage, 100, 100, 0);
		al_flip_display();
	}

	al_destroy_bitmap(menuImage);

	return start;
}