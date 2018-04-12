//keyboard.h
//By Michael Taboada
//Provided under the unlicense
//see UNLICENSE for details
#include <allegro5/allegro5.h>
#include <string>
using std::string;

static int defaultf[] = {ALLEGRO_KEY_ENTER, ALLEGRO_KEY_PAD_ENTER};
static int defaultq[] = {ALLEGRO_KEY_ESCAPE};

#pragma once
class keyboard
{
public:
    keyboard(void);
    ~keyboard(void);
    bool poll(ALLEGRO_EVENT& event);
    bool key_down(int key);
    bool key_pressed(int key);
    string get_chars(int* finish = defaultf, int flength = 2, int* quit = defaultq, int qlength = 1);
private:
    bool* keys_down;
    bool* keys_pressed;
    ALLEGRO_EVENT_QUEUE* queue;
    bool getting_chars;
    string chars;
};

