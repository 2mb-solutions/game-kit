/*
 * miscelaneous helper functions
 * By 2MB Solutions (https://2mb.solutions)
 * Provided under the MIT license. See license.txt for details.
 * Billy: https://stormdragon.tk
 * Michael: https://michaeltaboada.me
 */
#include <allegro5/allegro.h>
#include <dynamic_menu.h>
#include <iostream>
#include <keyboard.h>
#include <menu_helper.h>
#include <screen_reader.h>
#include <sound.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <misc.h>

ALLEGRO_DISPLAY* game_window(string gameTitle)
{
log_open("errors.log");
if(!al_init()) {
log("Could not initialize allegro.\n");
return NULL;
}
if(!al_install_audio()) {
log("Could not install audio subsystem.\n");
al_uninstall_system();
return NULL;
}
if(!al_install_keyboard()) {
log("Could not install keyboard hooks.\n");
al_uninstall_audio();
al_uninstall_system();
return NULL;
}
if(!al_init_acodec_addon()) {
log("Could not initialize codec routines.\n");
al_uninstall_audio();
al_uninstall_keyboard();
al_uninstall_system();
return NULL;
}
ALLEGRO_DISPLAY* disp = al_create_display(640,480);
if(!disp) {
log("Could not create display.\n");
	al_uninstall_audio();
al_uninstall_keyboard();
al_uninstall_system();
return NULL;
}
else {
al_set_window_title(disp, gameTitle.c_str());
return disp;
}
}

int end_game(ALLEGRO_DISPLAY* disp)
{
al_uninstall_audio();
al_uninstall_keyboard();
al_destroy_display(disp);
al_uninstall_system();
return 0;
}

string get_cpu_name()
{
string nameList[] = {"Alicia",
"April",
"Alonzo",
"Anaya",
"Andrew",
"Anthony",
"Billy",
"Burt",
"Cayden",
"Dorothy",
"Draken",
"Dave",
"Ember",
"Faye",
"George",
"Janet",
"Jenna",
"Jeniffer",
"Jeremiah",
"Kendell",
"Kris",
"Kyle",
"Labesia",
"Maggie",
"Mellisa",
"Michael",
"Rebecca",
"Scott",
"Storm",
"Terence",
"Tux",
"Tyler",
"Victoria"};
int i = rand() % (sizeof(nameList)/sizeof(nameList[0])); // generate a number within the size of the array nameList.
return nameList[i];
}

FILE* fi = NULL;
int log_open(string file) {
fi = fopen(file.c_str(), "w");
if(!fi) {
return false;
}
return true;
}

void credits(ALLEGRO_DISPLAY* disp, string gameName)
{
string credits[] = {gameName + "! brought to you by 2MB Solutions!",
"Billy Wolfe. Coder and designer. storm dragon dot tk",
"Michael Taboada. Coder and designer. Michael Taboada dot m e",
"Back to main menu"};
dynamic_menu* creditsMenu = create_menu(vector<string>(credits, credits+4), vector<string>());
creditsMenu->set_display(disp);
creditsMenu->run_extended("", "", 1, true);
delete creditsMenu;
}

void log(string message) {
if(fi) {
fprintf(fi, message.c_str());
}
}

void log_close() {
if(fi) {
fclose(fi);
fi = NULL;
}
}

