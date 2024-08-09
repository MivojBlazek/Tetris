# 
# Tetris
#
# File: Tetris.pro
#
# Description: File generated from qmake and correctly setup for my project. It contains c++ version, source files and headers.
# 
# Author: Michal Blažek
#

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OBJECTS_DIR = ./obj

SOURCES += \
    block.cpp \
    holdmenu.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    scene.cpp \
    shape.cpp

HEADERS += \
    block.h \
    config.h \
    holdmenu.h \
    mainwindow.h \
    menu.h \
    scene.h \
    shape.h

FORMS += \
    holdmenu.ui \
    mainwindow.ui \
    menu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
