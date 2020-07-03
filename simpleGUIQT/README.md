# C/C++ Simple GUI Project using QT5
This Project is a simple C/C++ Project, just to practice how to design the project folders, Makefile and how to build a GUI C++ Project  in mac


Install QT package for C++ in mac

brew install qt


go to project file

run>/usr/local/Cellar/qt/5.15.0/bin/qmake -project

this will create .pro file

add in [projectfolder].pro file

HEADERS += include/*.h
SOURCES += src/*.cpp
DESTDIR += build/release

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u

run>/usr/local/Cellar/qt/5.15.0/bin/qmake
run>make





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
