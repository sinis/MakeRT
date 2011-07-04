///////////////////////////////////////////////////////////////////////////////
// Main application class.

#ifndef MAKERT_H
#define MAKERT_H

#include <QWidget>
#ifndef Q_OS_SYMBIAN
#include "ui_makert.h"
#else
#include "ui_makert_s60.h"
#include <QAction>
#include <QMenu>
#include <hwrmvibra.h>
#endif // Q_OS_SYMBIAN
#include <AudioOutput>
#include <MediaObject>
#include <QTimer>
#include "textnotificationwidget.h"
#include "soundnotificationwidget.h"
#include "timersettingswidget.h"

class MakeRT: public QWidget
{
    Q_OBJECT
public:
    MakeRT(QWidget *parent = 0);
    ~MakeRT();

    Phonon::AudioOutput *GetAudioOutput() { return _audioOutput; }

public slots:
    void LoadSettings();
    void SaveSettings();
    void SetTimer();
    void Alarm();

private:
    Ui::MakeRT *_ui;
    TextNotificationWidget *_textNotificationWidget;
    SoundNotificationWidget *_soundNotificationWidget;
    TimerSettingsWidget *_timerSettingsWidget;
    Phonon::AudioOutput *_audioOutput;
    Phonon::MediaObject *_player;
    QTimer *_timer;
#ifdef Q_OS_SYMBIAN
    QAction *_textNotificationAction;
    QAction *_soundNotificationAction;
    QAction *_timerSettingsAction;
    QAction *_vibrationsAction;
    QAction *_aboutAction;
    QAction *_aboutQtAction;
    QMenu *_menu;
    QAction *_menuAction;
    QAction *_hideAction;
#endif // Q_OS_SYMBIAN
};

#endif // MAKERT_H ////////////////////////////////////////////////////////////
