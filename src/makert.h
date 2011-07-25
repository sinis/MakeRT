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
#include <QMessageBox>
#include "textnotificationwidget.h"
#include "soundnotificationwidget.h"
#include "timersettingswidget.h"

// Keys
#define TEXTNOTIFICATIONENABLED "TextNotificationEnabled"
#define TEXTNOTIFICATIONMODE "TextNotificationMode"
#define TEXTMESSAGE "TextMessage"
#define MESSAGELIST "MessageList"
#define SOUNDNOTIFICATIONENABLED "SoundNotificationEnabled"
#define AUDIOFILENAME "AudioFile"
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
#define VOLUME "Volume"

class MakeRT: public QWidget
{
    Q_OBJECT
public:
    MakeRT(QWidget *parent = 0);
    ~MakeRT();

    Phonon::AudioOutput *GetAudioOutput() { return _audioOutput; }
    Phonon::MediaObject *GetPlayer() { return _player; }
#ifndef Q_OS_SYMBIAN
    bool IsRunInTrayEnabled() const { return _ui->tray->isChecked(); }
#endif // Q_OS_SYMBIAN

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
    void VibrationsEnabled();
#else
    void RunAtStartupEnabled(bool);
    void RunInTrayEnabled(bool);
    void TrayIconClicked();
#endif // Q_OS_SYMBIAN

    void VolumeChanged(qreal volume);

private:
    static MakeRT *_instance;
    Ui::MakeRT *_ui;
    Phonon::AudioOutput *_audioOutput;
    Phonon::MediaObject *_player;
    QTimer *_timer;
    QSettings *_settings;
    TextNotificationWidget *_textNotificationWidget;
    SoundNotificationWidget *_soundNotificationWidget;
    TimerSettingsWidget *_timerSettingsWidget;
    QMessageBox *_messageBox;
#ifndef Q_OS_SYMBIAN
    QSystemTrayIcon *_trayIcon;
#else
    QAction *_textNotificationAction;
    QAction *_soundNotificationAction;
    QAction *_timerSettingsAction;
    QMenu *_vibrationsMenu;
    QAction *_vibrationsOnAction;
    QAction *_vibrationsOffAction;
    QAction *_aboutQtAction;
    QAction *_quitAction;
    QMenu *_menu;
    QAction *_menuAction;
    QAction *_hideAction;
    CHWRMVibra *_vibrator;
#endif // Q_OS_SYMBIAN
};

#endif // MAKERT_H ////////////////////////////////////////////////////////////
