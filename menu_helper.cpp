/*
 * Menu creation and other helper functions.
 * By 2MB Solutions (https://2mb.solutions)
 * Provided under the MIT license. See license.txt for details.
 * Billy: https://stormdragon.tk
 * Michael: https://michaeltaboada.me
*/
#include <dynamic_menu.h>
#include <string>
#include <vector>
#include <misc.h>
#include <soundplayer.h>
using std::string;
using std::vector;

dynamic_menu* create_menu(vector<string> items, vector<string> sounditems, vector<int(*)(dynamic_menu*, int, void*)> vec = vector<int(*)(dynamic_menu*, int, void*)>(), vector<void*> vec2 = vector<void*>()) {
int soundnumber = sounditems.size();
int numberofitems = items.size();
dynamic_menu* menu = new dynamic_menu((numberofitems > soundnumber)?numberofitems:soundnumber);
for(int x = 1; x <= numberofitems; x++) {
if(items[x-1] != "") {
menu->add_item_text(items[x-1], x, ((vec.size() >= x)?vec[x-1]:NULL), ((vec2.size() >= x)?vec2[x-1]:NULL));
}
}
for(int y = 1; y <= soundnumber; y++) {
if(sounditems[y-1] != "") {
menu->add_item_sound(sounditems[y-1], y, ((vec.size() >= y)?vec[y-1]:NULL), ((vec2.size() >= y)?vec2[y-1]:NULL));
}
}
return menu;
}

vector<string>* get_dir_children(string dir, int dir_or_file) {
ALLEGRO_FS_ENTRY *f = al_create_fs_entry((((string)(al_path_cstr(al_get_standard_path(ALLEGRO_RESOURCES_PATH), ALLEGRO_NATIVE_PATH_SEP)))+dir).c_str());
if(!f) {
return NULL;
}
if(al_open_directory(f)) {
vector<string>* vec = new vector<string>();
ALLEGRO_FS_ENTRY *fs = al_read_directory(f);
while(fs) {
const char* fn = al_get_fs_entry_name(fs);
string fsn = "";
int x;
for(x = strlen(fn)-1; x >= 0; x--) {
if(fn[x] == '/' || fn[x] == '\\') {
break;
}
}
for(int y = x+1; y < strlen(fn); y++) {
fsn += fn[y];
}
if(dir_or_file == 0) {
vec->push_back(fsn);
}
else if(dir_or_file == 1 && (al_get_fs_entry_mode(fs) & ALLEGRO_FILEMODE_ISFILE) == ALLEGRO_FILEMODE_ISFILE) {
vec->push_back(fsn);
}
else if(dir_or_file == 2 && (al_get_fs_entry_mode(fs) & ALLEGRO_FILEMODE_ISDIR) == ALLEGRO_FILEMODE_ISDIR){
vec->push_back(fsn);
}
if(fs) {
al_destroy_fs_entry(fs);
}
fs = al_read_directory(f);
}
al_close_directory(f);
al_destroy_fs_entry(f);
return vec;
}
else {
al_destroy_fs_entry(f);
return NULL;
}
}

string generic_menu(vector<string> extra_items, string music = "") {
sound menu_music;
if(music != "") {
menu_music.load(music);
menu_music.set_loop(true);
menu_music.play();
}
	vector<string> real_items;
real_items.push_back("Play game");
real_items.push_back("View instructions");
real_items.push_back("Learn game sounds.");
for (int x = 0; x < extra_items.size(); x++) {
real_items.push_back(extra_items[x]);
}
real_items.push_back("view credits");
real_items.push_back("Exit game");
dynamic_menu* menu = create_menu(real_items, vector<string>());
int pos = -1;
do {
	pos = menu->run_extended("", "Use your arrow keys to navigate the menu, and enter to select.", 1, true);
if(pos == 1) {
if(music != "")
	fade(&menu_music);
	return "play";
}
else if(pos == extra_items.size()+4) {
credits();
}
else if(pos == 2) {
instructions();
}
else if(pos == 3) {
if(music != "")
	fade(&menu_music);
	learn_sounds();
if(music != "") {
	menu_music.set_gain(0);
menu_music.play();
}
}
else if (pos == extra_items.size()+5) {
if(music != "")
	fade(&menu_music);
	return "exit";
}
else {
if(music != "")
	fade(&menu_music);
	return extra_items[pos-4];
}
}
while (pos != -1 && pos != 0 & pos != extra_items.size()+5);
if(music != "")
menu_music.stop();
return "invalid";
}

