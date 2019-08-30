#-------------------------------------------------
#
# Project created by QtCreator 2019-08-10T14:31:42
#
#-------------------------------------------------

QT       += gui
QT += widgets

CONFIG += precompile_header
PRECOMPILED_HEADER = ../pEntryPoint/pch.h

TARGET = pMath
TEMPLATE = lib
CONFIG += staticlib

Debug:DESTDIR = ../Outputs/libs/Debug
Debug:OBJECTS_DIR = ../Outputs/libs/Debug/
Debug:MOC_DIR = ../Outputs/libs/Debug/
Debug:RCC_DIR = ../Outputs/libs/Debug/
Debug:UI_DIR = ../Outputs/libs/Debug/

Release:DESTDIR = ../Outputs/libs/Release
Release:OBJECTS_DIR = ../Outputs/libs/Release/
Release:MOC_DIR = ../Outputs/libs/Release/
Release:RCC_DIR = ../Outputs/libs/Release/
Release:UI_DIR = ../Outputs/libs/Release/

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Common.cpp \
    Plane3.cpp \
    main.cpp

HEADERS += \
    Common.h \
    Matrix.h \
    Matrix2.h \
    Matrix3.h \
    Matrix4.h \
    Plane3.h \
    Vector.h \
    Vector2.h \
    Vector3.h \
    Vector4.h \
    mathematics.h \
    mathematics_typedef_planes.h \
    mathematics_typedef_vector.h \
    mathematics_typedefs_matrix.h \
    other_types.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
