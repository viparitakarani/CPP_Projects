#run qmake -project to create the .pro file
#/usr/local/Cellar/qt/5.15.0/bin/qmake -project

#Edit .pro file 
#QT += network
#QT += core gui  
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TEMPLATE = app
#TARGET = YeeLightController
#INCLUDEPATH += .

#DEFINES += QT_DEPRECATED_WARNINGS

#HEADERS += header/*.h
#SOURCES += main.cpp src/*.cpp
#DESTDIR += build/release

#OBJECTS_DIR = $$DESTDIR/.obj
#MOC_DIR = $$DESTDIR/.moc
#RCC_DIR = $$DESTDIR/.qrc
#UI_DIR = $$DESTDIR/.u

#run below command to create Makefile 
#usr/local/Cellar/qt/5.15.0/bin/qmake or 
/usr/local/Cellar/qt/5.15.0/bin/qmake -makefile

#run below command to build the .app in folder $$DESTDIR
make
