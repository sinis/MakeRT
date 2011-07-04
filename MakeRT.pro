#-------------------------------------------------
#
# Project created by QtCreator 2011-07-03T22:55:07
#
#-------------------------------------------------

QT       += core gui phonon

TARGET = MakeRT
TEMPLATE = app


SOURCES += src/main.cpp\
        src/makert.cpp \
    src/textnotificationwidget.cpp \
    src/editordialog.cpp \
    src/soundnotificationwidget.cpp \
    src/timersettingswidget.cpp

HEADERS  += src/makert.h \
    src/textnotificationwidget.h \
    src/editordialog.h \
    src/soundnotificationwidget.h \
    src/timersettingswidget.h

FORMS    += ui/makert.ui \
    ui/textnotificationwidget.ui \
    ui/editordialog.ui \
    ui/soundnotificationwidget.ui \
    ui/timersettingswidget.ui

symbian: {
    LIBS += -lhwrmvibraclient
}