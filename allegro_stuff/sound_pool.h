//sound_pool.h
//By Michael Taboada
//Provided under the unlicense
//see UNLICENSE for details
#include <string>
#include "sound_pool_item.h"

using std::string;

#pragma once
/**
*The sound pool allows you to play sounds in a virtual space without having to manually control each one.
**/
class sound_pool
{
public:
	sound_pool(void);
	sound_pool(int number_of_items);
	~sound_pool(void);
	int play_stationary(string filename, bool looping);
	int play_stationary_extended(string filename, bool looping, double start_pan = 0, double start_volume = 0, double start_pitch = 100, bool persistent = false);
	int play_1d(string filename, double listener_x, double sound_x, bool looping);
	int play_extended_1d(string filename, double listener_x, double sound_x, double left_range, double right_range, bool looping, double start_pan = 0, double start_volume = 0, double start_pitch = 100, bool persistent = false);
	int play_2d(string filename, double listener_x, double listener_y, double listener_angle, double sound_x, double sound_y, bool looping);
	int play_extended_2d(string filename, double listener_x, double listener_y, double listener_angle, double sound_x, double sound_y, double left_range, double right_range, double backward_range, double forward_range, bool looping, float start_pan = 0.0, float start_volume = 0.0, float start_pitch = 100.0, bool persistent=false);
	int play_3d(string filename, double listener_x, double listener_y, double listener_z, double listener_angle, double sound_x, double sound_y, double sound_z, bool looping);
	int play_extended_3d(string filename, double listener_x, double listener_y, double listener_z, double listener_angle, double sound_x, double sound_y, double sound_z, double left_range, double right_range, double backward_range, double forward_range, double up_range, double down_range, bool looping, float start_pan = 0.0, float start_volume = 0.0, float start_pitch = 100.0, bool persistent = false);
	bool sound_is_active(int slot);
	bool sound_is_playing(int slot);
	bool pause_sound(int slot);
	bool resume_sound(int slot);
	void pause_all();
	void resume_all();
	void update_listener_1d(double listener_x);
	void update_listener_2d(double listener_x, double listener_y, double listener_angle);
	void update_listener_3d(double listener_x, double listener_y, double listener_z, double listener_angle);
	bool update_sound_1d(int slot, double x);
	bool update_sound_2d(int slot, double x, double y);
	bool update_sound_3d(int slot, double x, double y, double z);
	bool update_sound_start_values(int slot, float start_pan, float start_volume, float start_pitch);
	bool update_sound_range_1d(int slot, double left_range, double right_range);
	bool update_sound_range_2d(int slot, double left_range, double right_range, double backward_range, double forward_range);
	bool update_sound_range_3d(int slot, double left_range, double right_range, double backward_range, double forward_range, double up_range, double down_range);
	void destroy_all();
	bool destroy_sound(int slot);
	double max_distance;
	float pan_step;
	float volume_step;
	float behind_pitch_decrease;
	float behind_volume_decrease;
	float above_pitch;
	double last_listener_x;
	double last_listener_y;
	double last_listener_z;
	double last_listener_angle;
	int numberofitems;
	sound_pool_item* items;
	bool verify_slot(int slot);
	void clean_unused();
	int reserve_slot(string filename);
};

