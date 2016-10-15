//#define ALLEGRO_STATICLINK 1
#include <dynamic_menu.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

dynamic_menu* create_menu(vector<string> items, vector<string> sounditems, vector<int(*)(dynamic_menu*, int, void*)> vec, vector<void*> vec2) {
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
ALLEGRO_FS_ENTRY *f = al_create_fs_entry((((string)(al_path_cstr(al_get_standard_path(ALLEGRO_RESOURCES_PATH), '/')))+dir).c_str());
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
if(fn[x] == '/') {
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
