#!/bin/bash

includes="-I/usr/include/speech-dispatcher -Igame-kit -Igame-kit/allegro_stuff -Igame-kit/screen-reader -Igame-kit/allegro_stuff/ game-kit/allegro_stuff/sound.cpp game-kit/screen-reader/screen_reader.cpp game-kit/menu_helper.cpp "
for i in *.cpp ; do
includes="${includes}$i "
done
unset appName
if [ $# -eq 1 ]; then
appName="$1"
fi
g++ $includes
if [ -n "$appName" ]; then
mv a.out "$appName"
fi
exit 0
