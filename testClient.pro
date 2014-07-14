#-------------------------------------------------
#
# Project created by QtCreator 2014-07-11T16:19:27
#
#-------------------------------------------------

QT       += core

QT       -= gui

QT 	    += network

TARGET = testClient
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    client.cpp \
    basedata.cpp \
    basedatamaker.cpp \
    emsdata.cpp \
    libsdata.cpp \
    metadata.cpp \
    metadatamaker.cpp \
    metadatareader.cpp \
    xrfdata.cpp

HEADERS += \
    client.h \
    basedata.h \
    basedatamaker.h \
    emsdata.h \
    libsdata.h \
    metadata.h \
    metadatamaker.h \
    metadatareader.h \
    xrfdata.h
