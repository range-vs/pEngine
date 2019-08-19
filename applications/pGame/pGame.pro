#-------------------------------------------------
#
# Project created by QtCreator 2019-08-02T04:51:09
#
#-------------------------------------------------

QT       += gui widgets opengl

TARGET = pGame
TEMPLATE = lib
CONFIG += precompile_header
PRECOMPILED_HEADER = ../../pEntryPoint/pch.h

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS DYNAMIC_LIBRARY_PGAME

Debug:DESTDIR = ../../Outputs/bins/Debug
Debug:OBJECTS_DIR = ../../Outputs/bins/Debug/
Debug:MOC_DIR = ../../Outputs/bins/Debug/
Debug:RCC_DIR = ../../Outputs/bins/Debug/
Debug:UI_DIR = ../../Outputs/bins/Debug/
Debug:LIBS += -l../../Outputs/bins/Debug/pDirect3DRender -l../../Outputs/bins/Debug/pOpenGLRender
Debug:LIBS += -l../../Outputs/libs/Debug/pUtils -l../../Outputs/libs/Debug/pApplicationGeneral -l../../Outputs/bins/Debug/pDebug

Release:DESTDIR = ../../Outputs/bins/Release
Release:OBJECTS_DIR = ../../Outputs/bins/Release/
Release:MOC_DIR = ../../Outputs/bins/Release/
Release:RCC_DIR = ../../Outputs/bins/Release/
Release:UI_DIR = ../../Outputs/bins/Release/
Release:LIBS += -l../../Outputs/bins/Release/pDirect3DRender -l../../Outputs/bins/Debug/pOpenGLRender
Release:LIBS += -l../../Outputs/libs/Release/pUtils -l../../Outputs/libs/Release/pApplicationGeneral -l../../Outputs/bins/Release/pDebug


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        pgame.cpp

HEADERS += \
        lib_define.h \
        pgame.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
