/*
mouse.cpp: mouse handling and coordination class.
 * By 2MB Solutions: https//2mb.solutions
 * Released under the MIT license. See license.txt for details.
 * Billy: https://stormdragon.tk/
 * Michael: https://michaeltaboada.me
 * */
#include "mouse.h"

mouse::mouse(void)
{
    if(!al_is_mouse_installed())
        {
            al_install_mouse();
        }
    buttons_down = new bool[al_get_mouse_num_buttons()-1];
    buttons_pressed = new bool[al_get_mouse_num_buttons()-1];
    for(unsigned int i = 0; i <al_get_mouse_num_buttons()-1; i++)
        {
            buttons_down[i] = buttons_pressed[i] = false;
        }
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_mouse_event_source());
    x = y = z = w = dx = dy = dz = dw = mousex = mousey = 0;
    display = NULL;
}


mouse::~mouse(void)
{
    delete[] buttons_down;
    delete[] buttons_pressed;
    al_destroy_event_queue(queue);
}

/**
*This method is used to both update the mouse class and return events to your code.
*Note that since more than one queue can be listening for mouse events, this method is probably not necessary to be called outside of this class.
*@return whether there was an event in the queue.
**/
bool mouse::poll(ALLEGRO_EVENT& ev/**< [out] The event to be returned to your code. If there was no event in the queue this value is unspecified.**/)
{
    ALLEGRO_EVENT event;
    if(!al_get_next_event(queue, &event))
        {
            ev = event;
            return false;
        }
    else
        {
            ev = event;
            switch (event.type)
                {
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    buttons_pressed[event.mouse.button-1] = true;
                    buttons_down[event.mouse.button-1] = true;
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    buttons_pressed[event.mouse.button-1] = false;
                    buttons_down[event.mouse.button-1] = false;
                    break;
                case ALLEGRO_EVENT_MOUSE_AXES:
                    x = event.mouse.x;
                    y = event.mouse.y;
                    z = event.mouse.z;
                    w = event.mouse.w;
                    dx += event.mouse.dx;
                    dy += event.mouse.dy;
                    dz += event.mouse.dz;
                    dw += event.mouse.dw;
                    break;
                case ALLEGRO_EVENT_MOUSE_WARPED:
                    x = event.mouse.x;
                    y = event.mouse.y;
                    z = event.mouse.z;
                    w = event.mouse.w;
                    break;
                case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                    if(display)
                        {
                            al_set_mouse_xy(display, mousex, mousey);
                        }
                    break;
                }
            return true;
        }
}

/**
*Used to return the mouse to the window if it leaves it.
*Mostly useful in audio games where blind people would not know if their mouse had left the window.
**/
void mouse::catch_mouse(ALLEGRO_DISPLAY* disp/**< [in] The display to keep the mouse in.**/, int mx/**< [in] The x coordinate to bring the mouse back to.**/, int my/**< [in] The y coordinate to return the mouse to.**/)
{
    display = disp;
    mousex = mx;
    mousey = my;
}

/**
*Lets the mouse leave the window normally.
**/
void mouse::uncatch_mouse()
{
    display = NULL;
    mousex = mousey = 0;
}

/**
*Check if a button is down.
*@return if the button in question is down.
**/
bool mouse::button_down(unsigned int button/**< [in] The button to check, as returned by allegro.**/)
{
    if(button > al_get_mouse_num_buttons() || button < 1)
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
            return buttons_down[button-1];
        }
}

/**
*Check if a button is pressed.
*Note that once a button is checked as pressed, it will not show as pressed until it has been released and re-pressed.
*@return whether the button was pressed.
**/
bool mouse::button_pressed(unsigned int button/**< [in] The button to check, as returned by allegro.**/)
{
    if(button > al_get_mouse_num_buttons() || button < 1)
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
            bool val = buttons_pressed[button-1];
            buttons_pressed[button-1] = false;
            return val;
        }
}

/**
*Get the x coordinate of the mouse.
*@return the x coordinate.
**/
int mouse::get_x()
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    return x;
}

/**
*Get the y coordinate of the mouse.
*@return the y coordinate.
**/
int mouse::get_y()
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    return y;
}

/**
*Get the z coordinate of the mouse.
*@return the z coordinate.
**/
int mouse::get_z()
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    return z;
}

/**
*Get the w coordinate of the mouse.
*@return the w coordinate.
**/
int mouse::get_w()
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    return w;
}

/**
*Get the difference in the x coordinate from last time this method was called.
*@return the difference.
**/
int mouse::get_dx()
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    int val = dx;
    dx = 0;
    return val;
}

/**
*Get the difference in the y coordinate from last time this method was called.
*@return the difference.
**/
int mouse::get_dy()
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    int val = dy;
    dy = 0;
    return val;
}

/**
*Get the difference in the z coordinate from last time this method was called.
*@return the difference.
**/
int mouse::get_dz()
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    int val = dz;
    dz = 0;
    return val;
}

/**
*Get the difference in the w coordinate from last time this method was called.
*@return the difference.
**/
int mouse::get_dw()
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    int val = dw;
    dw = 0;
    return val;
}

/**
*Gets the x and y coordinates.
**/
void mouse::get_xy(int& mx/**< [out] The variable to put the x coordinate into.**/, int& my/**< [out] The variable to put the y coordinate into.**/)
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    mx= x;
    my = y;
}

/**
*Get the z and w coordinates of the mouse.
**/
void mouse::get_zw(int& mz/**< [out] The variable to put the z coordinate in.**/, int& mw/**< [out] The variable to put the w coordinate in.**/)
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    mz = z;
    mw = w;
}

/**
*Get the difference between the x, and the y coordinates since this method was called.
**/
void mouse::get_dx_dy(int& mdx/**< [out] The difference in the x coordinate.**/, int& mdy/**< [out] The difference in the y.**/)
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    int val = dx;
    dx = 0;
    mdx = val;
    val = dy;
    dy = 0;
    mdy = val;
}

/**
*Gets the difference between the z and w coordinates since this method was called last.
**/
void mouse::get_dz_dw(int& mdz/**< [out] The difference in the z coordinate.**/, int& mdw/**< [out] The difference in the w coordinate.**/)
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    int val = dz;
    dz = 0;
    mdz = val;
    val = dw;
    dw = 0;
    mdw = val;
}

/**
*Get the x, y, z, and w coordinates.
**/
void mouse::get_axes(int& mx/**< [out] The x coordinate.**/, int& my/**< [out] The y coordinate.**/, int& mz/**< [out] The z coordinate.**/, int& mw/**< [out] The w coordinate.**/)
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    mx = x;
    my = y;
    mz = z;
    mw = w;
}

/**
*Gets the difference in the x, y, z, and w coordinate since this method was called last.
**/
void mouse::get_changes(int& mdx/**< [out] The variable to put the change in the x coordinate.**/, int& mdy/**< [out] The y difference.**/, int& mdz/**< [out] The difference in the z coordinate.**/, int& mdw/**< [out] The difference in the w coordinate.**/)
{
    ALLEGRO_EVENT ev;
    while(poll(ev))
        {
            al_rest(0.005);
        }
    int val = dx;
    dx = 0;
    mdx = val;
    val = dy;
    dy = 0;
    mdy = val;
    val = dz;
    dz = 0;
    mdz = val;
    val = dw;
    dw = 0;
    mdw = val;
}
