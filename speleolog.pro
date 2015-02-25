#-------------------------------------------------
#
# Project created by QtCreator 2015-02-25T15:28:37
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = speleolog
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
QMAKE_CXXFLAGS += -Wall -Wextra -Winit-self #-Wconversion
QMAKE_CXXFLAGS += -std=c++11
