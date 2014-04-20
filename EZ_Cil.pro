TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += ez_cil.cpp \
    turtle.cpp \
    dib_Fonts.c \
    dib.c \
    draw.c

HEADERS += \
    dib_Fonts.h \
    dib_types.h \
    dib.h \
    draw.h \
    turtle.h

