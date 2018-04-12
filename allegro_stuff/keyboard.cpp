/*
keyboard.cpp: Easy access to the keyboard.
 * By 2MB Solutions: https//2mb.solutions
 * Released under the MIT license. See license.txt for details.
 * Billy: https://stormdragon.tk/
 * Michael: https://michaeltaboada.me
 * */
#include "keyboard.h"


keyboard::keyboard(void)
{
    if(!al_is_keyboard_installed())
        {
            al_install_keyboard();
        }
    keys_down = new bool[ALLEGRO_KEY_MAX];
    keys_pressed = new bool[ALLEGRO_KEY_MAX];
    for(int x = 0; x<ALLEGRO_KEY_MAX; x++)
        {
            keys_down[x] = keys_pressed[x] = false;
        }
    queue =  NULL;
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    chars = "";
    getting_chars = false;
}


keyboard::~keyboard(void)
{
    delete[] keys_down;
    delete[] keys_pressed;
    al_destroy_event_queue(queue);
}

/**
*Used to both update the keyboard class of key changes, and also to return any events to your code outside of the class.
*Note that since more than one queue can be listening for keyboard events, this method is probably unnecessary.
*@return Whether there was an event in the queue
**/
bool keyboard::poll(ALLEGRO_EVENT& event/**< [out] Any event that was dequeued on this call. Note that if there was no event in the queue, this value is undefined.**/)
{
    ALLEGRO_EVENT ev;
    if(al_get_next_event(queue, &ev))
        {
            switch (ev.type)
                {
                case ALLEGRO_EVENT_KEY_CHAR:
                    if(!getting_chars)
                        break;
                    if(ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                        {
                            if(chars.size() == 0)
                                {
                                    break;
                                }
                            else
                                {
                                    chars.resize(chars.size()-1);
                                }
                        }
                    if(ev.keyboard.repeat || (ev.keyboard.unichar < 32 || ev.keyboard.unichar > 126))
                        {
                            break;
                        }
                    chars += (char)(ev.keyboard.unichar);
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    keys_pressed[ev.keyboard.keycode] = keys_down[ev.keyboard.keycode] = true;
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    keys_pressed[ev.keyboard.keycode] = keys_down[ev.keyboard.keycode] = false;
                    break;
                }
            event = ev;
            return true;
        }
    else
        {
            event = ev;
            return false;
        }
}

/**
*Check if a key is down.
*@return Whether the key is down.
**/
bool keyboard::key_down(int key/**< [in] The allegro keycode for the key you want to check.**/)
{
    if(key >= ALLEGRO_KEY_MAX || key < 0)
        {
            return false;
        }
    else
        {
            ALLEGRO_EVENT ev;
            while(poll(ev))
                {
                    al_rest(0.005);
                }
            return keys_down[key];
        }
}

/**
*Check whether a key is pressed.
*Note that a key that was returned as pressed will not be returned as pressed until it is released and pressed again.
*@return whether the key was pressed.
**/
bool keyboard::key_pressed(int key/**< [in] The allegro key constant for the key you want to check.**/)
{
    if(key >= ALLEGRO_KEY_MAX || key < 0)
        {
            return false;
        }
    else
        {
            ALLEGRO_EVENT ev;
            while(poll(ev))
                {
                    al_rest(0.005);
                }
            bool val = keys_pressed[key];
            keys_pressed[key] = false;
            return val;
        }
}

string keyboard::get_chars(int* finish, int flength, int* quit, int qlength)
{
    getting_chars = true;
    chars = (string)("");
    do
        {
            al_rest(0.005);
            ALLEGRO_EVENT ev;
            if(poll(ev))
                {
                    if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
                        {
                            for(int x = 0; x < flength; x++)
                                {
                                    if(finish[x] == ev.keyboard.keycode)
                                        {
                                            getting_chars = false;
                                            return chars;
                                        }
                                }
                            for (int x =0; x < qlength; x++)
                                {
                                    if(quit[x] == ev.keyboard.keycode)
                                        {
                                            getting_chars = false;
                                            return (string)("");
                                        }
                                }
                        }
                }
        }
    while(true);
}
