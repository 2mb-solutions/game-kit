#include <sound.h>
#include <string>
using std::string;



int power_bar(string soundstring, int p=0, int min=50, int max=200, float wait = 0.01);
int direction_bar(string soundstring, int p=-101, int min=-50, int max=50, float wait = 0.01);
string get_cpu_name();
void fade(sound* s, int start=0, int end=-50, float time=2.5);
bool play_sound_wait(string file);
bool play_sound_wait(sound* so);
