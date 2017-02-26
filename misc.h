#include <allegro5/allegro.h>
#include <string>
using std::string;
#include <stdio.h>
#include <sstream>
using std::stringstream;
#include <sound.h>
#include <screen_reader.h>
#include <keyboard.h>

ALLEGRO_DISPLAY* game_window(string);
int end_game(ALLEGRO_DISPLAY* DISP);
string get_cpu_name();
int log_open(string file);
void credits(ALLEGRO_DISPLAY* disp, string gameName);
void credits();
void instructions();
void log(string message);
void log_close();
