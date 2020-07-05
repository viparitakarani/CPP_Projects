# C/C++ Yee Light Cpntroller GUI Project using QT5


##Install QT package for C++ in mac

brew install qt

go to project file

##run qmake -project to create the .pro file
/usr/local/Cellar/qt/5.15.0/bin/qmake -project

Edit .pro file 
QT += network
QT += core gui  
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = YeeLightController
INCLUDEPATH += .

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += header/*.h
SOURCES += main.cpp src/*.cpp
DESTDIR += build/release

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u

##run below command to create Makefile 
usr/local/Cellar/qt/5.15.0/bin/qmake 
or 
/usr/local/Cellar/qt/5.15.0/bin/qmake -makefile

##run below command to build the .app in folder $$DESTDIR
make

##run the apps
open -a YeeLightController build/release
or
debug apps
./build/release/YeeLightController.app/Contents/MacOS/YeeLightController






Project File


## /build
the place where we put the build file file object files .obj and the excutable file 
## /src
the source code for the project *.c *.cpp *.hpp
## /inlucdes
the include files *.h
## Makefile
the  build configuration file
## build.sh
the file to build project from source code files.
## run.sh
the file which's used to run the main executable file in build folder
