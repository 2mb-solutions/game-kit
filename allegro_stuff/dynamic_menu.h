#include <string>
#include "sound.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro.h>
#include <screen_reader.h>
#include <vector>
using std::vector;

using std::string;
#pragma once
class dynamic_menu
{
public:
    dynamic_menu(unsigned int numberofitems);
    ~dynamic_menu(void);
    int add_item_sound(string filename, unsigned int pos, int (*func)(dynamic_menu *m, int posi, void*) = NULL, void* arg = NULL);
    int add_item_text(string text, unsigned int pos, int (*func)(dynamic_menu *m, int posi, void*) = NULL, void* arg = NULL);
    unsigned 	int get_item_count();
    unsigned 	int get_position();
    void reset(bool completely, int numberofitems);
    int run(string filename, string text);
    int run_extended(string filename, string text, unsigned int pos, bool speak = false);
    void set_callback(int (*functocall)(dynamic_menu*, string), string data);
    bool set_font(string f, int size);
    void set_display(ALLEGRO_DISPLAY* d);
    bool wrap;
    bool allow_escape;
    sound* select;
    sound* move;
private:
    unsigned 	int number_of_items;
    string* textarray;
    string* soundarray;
    unsigned 	int position;
    int (*function)(dynamic_menu*, string);
    sound* s;
    ALLEGRO_FONT* font;
    ALLEGRO_DISPLAY* display;
    int font_size;
    screen_reader* sr;
    vector<int(*)(dynamic_menu*, int, void*)> functions;
    vector<void*> functionargs;
};
typedef int(*f)(dynamic_menu*, int, void*);
