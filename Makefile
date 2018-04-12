# makefile to style .h and .cpp files properly. Type make format
# generic variables
ECHO=echo

#formatting specific flags
FORMATTER = astyle
FORMAT_FLAGS = --style=gnu -Q

GK_S_FILES= allegro_stuff/sound.cpp allegro_stuff/keyboard.cpp screen-reader/screen_reader.cpp \
allegro_stuff/dynamic_menu.cpp menu_helper.cpp misc.cpp soundplayer.cpp allegro_stuff/sound_pool_item.cpp \
allegro_stuff/sound_pool.cpp allegro_stuff/mouse.cpp
GK_H_FILES= allegro_stuff/sound.h allegro_stuff/keyboard.h screen-reader/screen_reader.h \
allegro_stuff/dynamic_menu.h menu_helper.h misc.h soundplayer.h allegro_stuff/sound_pool_item.h \
allegro_stuff/sound_pool.h allegro_stuff/mouse.h include.h

all:
	@$(ECHO) "Type make format to format header and c++ files"

format:
	@$(ECHO) Formatting
	@$(FORMATTER) $(FORMAT_FLAGS) $(GK_S_FILES)
	@$(FORMATTER) $(FORMAT_FLAGS) $(GK_H_FILES)
	@find . -name "*.orig" -exec rm -f {} \;
	@$(ECHO) Done.
