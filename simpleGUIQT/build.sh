#run qmake -project to create the .pro file
#/usr/local/Cellar/qt/5.15.0/bin/qmake -project
#edit .pro file add 
#HEADERS += include/analogclock.h
#SOURCES += src/analogclock.cpp src/main.cpp
#DESTDIR += build/release

#OBJECTS_DIR = $$DESTDIR/.obj
#MOC_DIR = $$DESTDIR/.moc
#RCC_DIR = $$DESTDIR/.qrc
#UI_DIR = $$DESTDIR/.u

/usr/local/Cellar/qt/5.15.0/bin/qmake 
make
