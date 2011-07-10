///////////////////////////////////////////////////////////////////////////////
// Main application class.

#ifndef MAKERT_H
#define MAKERT_H

#include <QWidget>
#ifndef Q_OS_SYMBIAN
#include <QSystemTrayIcon>
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
#include <QSettings>
#include "textnotificationwidget.h"
#include "soundnotificationwidget.h"
#include "timersettingswidget.h"

// Keys
#define TEXTNOTIFICATIONENABLED "TextNotificationEnabled"
#define TEXTNOTIFICATIONMODE "TextNotificationMode"
#define TEXTMESSAGE "TextMessage"
#define MESSAGELIST "MessageList"
#define SOUNDNOTIFICATIONENABLED "SoundNotificationEnabled"
#define AUDIOFILE "AudioFile"
#define TIMERMODE "TimerMode"
#define FIXEDINTERVAL "FixedInterval"
#define RANDOMINTERVALFROM "RandomIntervalFrom"
#define RANDOMINTERVALTO "RandomIntervalTo"
#ifdef Q_OS_SYMBIAN
#define VIBRATIONSENABLED "VibrationsEnabled"
#else
#define RUNATSTARTUP "RunAtStartup"
#define RUNINTRAY "RunInTray"
#endif // Q_OS_SYMBIAN

class MakeRT: public QWidget
{
    Q_OBJECT
public:
    MakeRT(QWidget *parent = 0);
    ~MakeRT();

    Phonon::AudioOutput *GetAudioOutput() { return _audioOutput; }
    bool IsRunInTrayEnabled() const { return _ui->tray->isChecked(); }

    static MakeRT *GetInstance() { return _instance; }

public slots:
    void LoadSettings();
    //void SaveSettings();
    void SetTimer();
    void Alarm();

private slots:
    void TextMessageActivationChanged(bool);
    void TextMessageChanged(QString);
    void MessageListChanged(QStringList &);
    void TextMessageModeChanged(TextNotificationWidget::Mode);

    void SoundNotificationActivationChanged(bool);
    void AudioFileNameChanged(QString);

    void FixedIntervalChanged(int);
    void RandomIntervalChanged(int, int);
    void TimerModeChanged(TimerSettingsWidget::Mode);

#ifdef Q_OS_SYMBIAN
    void VibrationsEnabled(bool);
#else
    void RunAtStartupEnabled(bool);
    void RunInTrayEnabled(bool);
    void TrayIconClicked();
#endif // Q_OS_SYMBIAN

private:
    static MakeRT *_instance;
    Ui::MakeRT *_ui;
    TextNotificationWidget *_textNotificationWidget;
    SoundNotificationWidget *_soundNotificationWidget;
    TimerSettingsWidget *_timerSettingsWidget;
    Phonon::AudioOutput *_audioOutput;
    Phonon::MediaObject *_player;
    QTimer *_timer;
    QSettings *_settings;
#ifndef Q_OS_SYMBIAN
    QSystemTrayIcon *_trayIcon;
#else
    QAction *_textNotificationAction;
    QAction *_soundNotificationAction;
    QAction *_timerSettingsAction;
    QAction *_vibrationsAction;
    QAction *_aboutQtAction;
    QAction *_quitAction;
    QMenu *_menu;
    QAction *_menuAction;
    QAction *_hideAction;
#endif // Q_OS_SYMBIAN
};

#endif // MAKERT_H ////////////////////////////////////////////////////////////
