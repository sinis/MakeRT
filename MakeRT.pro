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

FORMS    += ui/textnotificationwidget.ui \
    ui/timersettingswidget.ui

symbian: {
    LIBS += -lhwrmvibraclient
    FORMS += ui/soundnotificationwidget_s60.ui \
             ui/editordialog_s60.ui \
             ui/makert_s60.ui
    ICON = res/icon_s60.svg
}

!symbian: {
    FORMS += ui/soundnotificationwidget.ui \
             ui/editordialog.ui \
             ui/makert.ui
}

win32:RC_FILE = res/icon.rc

RESOURCES += \
    res/res.qrc
