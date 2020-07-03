#make -f Makefile
g++ src/*.cpp  -o build/main `pkg-config gtkmm-3.0 --cflags --libs` -std=c++11