#include <dynamic_menu.h>
#include <string>
#include <vector>
using std::string;
using std::vector;


dynamic_menu* create_menu(vector<string> items, vector<string> sounditems, vector<int(*)(dynamic_menu*, int, void*)> vec = vector<int(*)(dynamic_menu*, int, void*)>(), vector<void*> vec2 = vector<void*>());
vector<string>* get_dir_children(string dir, int dir_or_file);
string generic_menu(vector<string> extra_items, string music = "");
