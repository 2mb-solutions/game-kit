//mouse.h
//By Michael Taboada
//Provided under the unlicense
//see UNLICENSE for details
#include <allegro5/allegro5.h>

#pragma once
class mouse
{
public:
    mouse(void);
    ~mouse(void);
    bool poll(ALLEGRO_EVENT& ev);
    bool button_down(unsigned int button);
    bool button_pressed(unsigned int button);
    void catch_mouse(ALLEGRO_DISPLAY* disp, int mx, int my);
    void uncatch_mouse();
    int get_x();
    int get_y();
    int get_z();
    int get_w();
    void get_xy(int& mx, int& my);
    void get_zw(int&mz, int& mw);
    void get_axes(int& mx, int& my, int& mz, int& mw);
    int get_dx();
    int get_dy();
    int get_dz();
    int get_dw();
    void get_dx_dy(int& mdx, int& mdy);
    void get_dz_dw(int& mdz, int& mdw);
    void get_changes(int& mdx, int& mdy, int& mdz, int& mdw);
private:
    bool* buttons_pressed;
    bool* buttons_down;
    int x;
    int y;
    int z;
    int w;
    int dx;
    int dy;
    int dz;
    int dw;
    ALLEGRO_DISPLAY* display;
    int mousex;
    int mousey;
    ALLEGRO_EVENT_QUEUE* queue;
};