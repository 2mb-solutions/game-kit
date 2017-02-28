/*
dynamic_menu.cpp: a menu class supporting text, speech, and sounds!
 * By 2MB Solutions: https//2mb.solutions
 * Released under the MIT license. See license.txt for details.
 * Billy: https://stormdragon.tk/
 * Michael: https://michaeltaboada.me
 * */
#include "dynamic_menu.h"
#include <iostream>

dynamic_menu::dynamic_menu(int numberofitems)
{
select = move = NULL;
	number_of_items=numberofitems;
	s = new sound();
	font = NULL;
	font_size = 0;
	soundarray=new string[number_of_items];
	textarray = new string[number_of_items];
functions.resize(numberofitems, NULL);	
functionargs.resize(numberofitems, NULL);
function= NULL;
position = 0;
	wrap = false;
	allow_escape = false;
	display = NULL;
sr = new screen_reader();
}


dynamic_menu::~dynamic_menu(void)
{
	delete[] soundarray;
	delete[] textarray;
	delete s;
if(font) {
	al_destroy_font(font);
}
delete sr;
}

int dynamic_menu::add_item_sound(string filename, int pos, int (*funcptr)(dynamic_menu*, int, void*), void* arg) {
	if(pos > number_of_items || pos<1) {
		return -1;
	}
	else {
		soundarray[pos-1] = filename;
functions[pos-1] = funcptr;
functionargs[position-1] = arg;
	}
	return pos;
}

int dynamic_menu::add_item_text(string text, int pos, int (*funcptr)(dynamic_menu*, int, void*), void* arg) {
	if(pos > number_of_items || pos < 1) {
		return -1;
	}
	else {
		textarray[pos-1] = text;
functions[pos-1] = funcptr;
functionargs[pos-1] = arg;
	}
	return pos;
}

int dynamic_menu::get_item_count() {
	return number_of_items;
}

int dynamic_menu::get_position() {
	return position;
}

void dynamic_menu::reset(bool completely, int numberofitems) {
	delete[] textarray;
	delete[] soundarray;
	number_of_items = numberofitems;
	textarray = new string[number_of_items];
	soundarray = new string[number_of_items];
	position = 0;
	if(completely) {
		wrap=false;
		allow_escape = false;
		function = NULL;
		if(font) {
			al_destroy_font(font);
		}
		display = NULL;
	}
}

void dynamic_menu::set_callback(int (*functocall)(dynamic_menu*, string), string data) {
	function = functocall;
}

bool dynamic_menu::set_font(string f, int size) {
	if(f.compare("") == 0) {
		return false;
	}
	else {
		ALLEGRO_FONT* fo = al_load_font(f.c_str(), size, 0);
		if(!fo) {
			return false;
		}
		else {
			font = fo;
			font_size = size;
			return true;
		}
	}
	return false;
}

int dynamic_menu::run(string filename, string text) {
	return run_extended(filename, text, 0, false);
}

int dynamic_menu::run_extended(string filename, string text, int pos, bool speak) {
	if(pos<= 0 || pos > number_of_items) {
		pos = 1;
	}
	position = pos;
	ALLEGRO_COLOR selected = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR regular = al_map_rgb(128, 128, 128);
	int fontsize;
	if(font) {
		fontsize = font_size;
	}
	int center;
	int last_x = 0;
	int true_x = 0;
	if(display) {
		center = al_get_display_width(display)/2;
	}
	if(filename.compare("") != 0) {
		if(!s->load(filename)) {
			return -1;
		}
		if(!s->play()) {
			return -1;
		}
		while(s->is_playing()) {
			al_rest(0.05);
		}
	}
	if(soundarray[position-1].compare("") != 0) {
		if(!s->load(soundarray[position-1]))
			return -1;
		if(!s->play())
			return -1;
	}
	if(text.compare("") != 0) {
		sr->speak_any_interrupt(text);
		if(display) {
			if(font) {
				last_x = last_x+fontsize;
				al_draw_text(font, regular, center, last_x, ALLEGRO_ALIGN_CENTRE, text.c_str());
				true_x = last_x;
			}
		}
	}
	last_x = true_x;
	for(int i = 0; i < number_of_items; i++) {
		if(textarray[i].compare("") == 0) {
			continue;
		}
		if(!display) {
			continue;
		}
		if(!font) {
			continue;
		}
		last_x = last_x+fontsize;
		al_draw_text(font, regular, center, last_x, ALLEGRO_ALIGN_CENTRE, textarray[i].c_str());
	}
	al_flip_display();
	if(speak && textarray[position-1].compare("") != 0)
		sr->speak_any(textarray[position-1]);
	ALLEGRO_EVENT_QUEUE* eventqueue = NULL;
	eventqueue = al_create_event_queue();
	if(!eventqueue) {
		return -1;
	}
	al_register_event_source(eventqueue, al_get_keyboard_event_source());
	while(true) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(eventqueue, &ev);
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
		last_x = true_x;
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_DOWN:
			case ALLEGRO_KEY_K:
				if(position == number_of_items) {
					if(wrap) {
						position = 0;
					}
					else {
						continue;
					}
				}
				position++;
				if(move) {
					move->stop();
					move->play();
				}
				if(soundarray[position-1].compare("") != 0) {
					s->stop();
					s->load(soundarray[position-1]);
					s->play();
				}
if(textarray[position-1].compare("") != 0) {
sr->speak_any_interrupt(textarray[position-1]);
}
				for (int i = 0; i < number_of_items; i++) {
					if(textarray[i].compare("") == 0) {
						continue;
					}	
					if(!display) {
						continue;
					}
					if(!font) {
						continue;
					}
					if(i == position-1) {
						last_x = last_x+fontsize;
						al_draw_text(font, selected, center, last_x, ALLEGRO_ALIGN_CENTRE, textarray[i].c_str());
					}
					else {
						last_x = last_x+fontsize;
						al_draw_text(font, regular, center, last_x, ALLEGRO_ALIGN_CENTRE, textarray[i].c_str());
					}						
				}
				al_flip_display();
			break;
			case ALLEGRO_KEY_UP:
			case ALLEGRO_KEY_I:
			if(position == 1 || position == 0) {
					if(wrap) {
						position = number_of_items+1;
					}
					else {
						continue;
					}
				}
				position--;
				if(move) {
					move->stop();
					move->play();
				}
				if(soundarray[position-1].compare("") != 0) {
					s->stop();
					s->load(soundarray[position-1]);
					s->play();
				}
if(textarray[position-1].compare("") != 0) {
sr->speak_any_interrupt(textarray[position-1]);
}
				for (int i = 0; i < number_of_items; i++) {
					if(textarray[i].compare("") == 0) {
						continue;
					}
					if(!display) {
						continue;
					}
					if(!font) {
						continue;
					}
					if(i == position-1) {
						last_x = last_x+fontsize;
						al_draw_text(font, selected, center, last_x, ALLEGRO_ALIGN_CENTRE, textarray[i].c_str());
					}
					else {
						last_x = last_x+fontsize;
						al_draw_text(font, regular, center, last_x, ALLEGRO_ALIGN_CENTRE, textarray[i].c_str());
					}
				}
				al_flip_display();
			break;
			case ALLEGRO_KEY_ESCAPE:
				if(allow_escape) {
					s->stop();
					return 0;
				}
			break;
			case ALLEGRO_KEY_ENTER:
				if(select) {
					select->stop();
					select->play();
				}
				s->stop();
if(functions.size() >= position && functions[position-1]) {
int ret = (*(functions[position-1]))(this, position, functionargs[position-1]);
if(ret == 1) {
				return position;
}
else {
return -1;
}
}
else {
return position;
}
			break;
			}
		}
	}
	return -1;
}

void dynamic_menu::set_display(ALLEGRO_DISPLAY* d) {
	display=d;
}
