#include "makert.h"
#ifdef Q_OS_WIN
#include <windows.h>
#elif defined Q_OS_LINUX
#include <QFile>
#include <QDir>
#endif // Q_OS_WIN
#ifdef Q_OS_SYMBIAN
#include <QActionGroup>
#endif // Q_OS_SYMBIAN

MakeRT *MakeRT::_instance = 0;

// Constructor
MakeRT::MakeRT(QWidget *parent):
    QWidget(parent),
    _ui(new Ui::MakeRT),
    _audioOutput(new Phonon::AudioOutput(Phonon::MusicCategory, this)),
    _player(new Phonon::MediaObject(this)),
    _timer(new QTimer(this)),
    _settings(new QSettings(this)),
    _textNotificationWidget(new TextNotificationWidget(this)),
    _soundNotificationWidget(new SoundNotificationWidget(this, _player)),
    _timerSettingsWidget(new TimerSettingsWidget(this)),
    _messageBox(new QMessageBox(QMessageBox::Information, tr("Make reality test!"), "", QMessageBox::Ok, this))
  #ifndef Q_OS_SYMBIAN
  , _trayIcon(new QSystemTrayIcon(QIcon(":/icon.png"), this))
  #else
  , _textNotificationAction(new QAction(tr("Text notifications"), this)),
    _soundNotificationAction(new QAction(tr("Sound notification"), this)),
    _timerSettingsAction(new QAction(tr("Timer settings"), this)),
    _vibrationsMenu(new QMenu(tr("Vibrations"), this)),
    _vibrationsOnAction(new QAction(tr("On"), this)),
    _vibrationsOffAction(new QAction(tr("Off"), this)),
    _aboutQtAction(new QAction(tr("About Qt4"), this)),
    _quitAction(new QAction(tr("Quit"), this)),
    _menu(new QMenu(this)),
    _menuAction(new QAction(tr("Menu"), this)),
    _hideAction(new QAction(tr("Hide"), this))
  #endif // Q_OS_SYMBIAN
{
    if (!_instance)
        _instance = this;
    else return;

    _ui->setupUi(this);

#ifdef Q_OS_SYMBIAN
    _vibrator = CHWRMVibra::NewL();

    _vibrationsOnAction->setCheckable(true);
    //_vibrationsOnAction->setChecked(true);
    _vibrationsOffAction->setCheckable(true);
    QActionGroup *group = new QActionGroup(this);
    group->addAction(_vibrationsOnAction);
    group->addAction(_vibrationsOffAction);
    _vibrationsMenu->addActions(group->actions());

    addAction(_menuAction);
    addAction(_hideAction);
    _menu->addAction(_textNotificationAction);
    _menu->addAction(_soundNotificationAction);
    _menu->addAction(_timerSettingsAction);
    _menu->addMenu(_vibrationsMenu);
    _menu->addAction(_aboutQtAction);
    _menu->addAction(_quitAction);
    _menuAction->setMenu(_menu);
    _menuAction->setSoftKeyRole(QAction::PositiveSoftKey);
    _hideAction->setSoftKeyRole(QAction::NegativeSoftKey);

    connect(_textNotificationAction, SIGNAL(triggered()), _textNotificationWidget, SLOT(showMaximized()));
    connect(_soundNotificationAction, SIGNAL(triggered()), _soundNotificationWidget, SLOT(showMaximized()));
    connect(_timerSettingsAction, SIGNAL(triggered()), _timerSettingsWidget, SLOT(showMaximized()));
    connect(_vibrationsOnAction, SIGNAL(triggered()), this, SLOT(VibrationsEnabled()));
    connect(_vibrationsOffAction, SIGNAL(triggered()), this, SLOT(VibrationsEnabled()));
    connect(_aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(_hideAction, SIGNAL(triggered()), this, SLOT(lower()));

    /*_textNotificationWidget->hide();
    _textNotificationWidget->setWindowModality(Qt::WindowModal);
    _soundNotificationWidget->hide();
    _soundNotificationWidget->setWindowModality(Qt::WindowModal);
    _timerSettingsWidget->hide();
    _timerSettingsWidget->setWindowModality(Qt::WindowModal);*/
#else
    _ui->tabWidget->addTab(_textNotificationWidget, tr("Text notifications settings"));
    _ui->tabWidget->addTab(_soundNotificationWidget, tr("Sound notification settigns"));
    _ui->tabWidget->addTab(_timerSettingsWidget, tr("Timer settings"));

    connect(_ui->quit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(TrayIconClicked()));
    connect(_ui->startup, SIGNAL(toggled(bool)), this, SLOT(RunAtStartupEnabled(bool)));
    connect(_ui->tray, SIGNAL(toggled(bool)), this, SLOT(RunInTrayEnabled(bool)));

    _trayIcon->show();
#endif // Q_OS_SYMBIAN

    connect(_textNotificationWidget, SIGNAL(ActivationChanged(bool)), this, SLOT(TextMessageActivationChanged(bool)));
    connect(_textNotificationWidget, SIGNAL(TextMessageChanged(QString)), this, SLOT(TextMessageChanged(QString)));
    connect(_textNotificationWidget, SIGNAL(MessageListChanged(QStringList&)), this, SLOT(MessageListChanged(QStringList&)));
    connect(_textNotificationWidget, SIGNAL(ModeChanged(TextNotificationWidget::Mode)), this, SLOT(TextMessageModeChanged(TextNotificationWidget::Mode)));
    connect(_soundNotificationWidget, SIGNAL(ActivationChanged(bool)), this, SLOT(SoundNotificationActivationChanged(bool)));
    connect(_soundNotificationWidget, SIGNAL(FileNameChanged(QString)), this, SLOT(AudioFileNameChanged(QString)));
    connect(_timerSettingsWidget, SIGNAL(ModeChanged(TimerSettingsWidget::Mode)), this, SLOT(TimerModeChanged(TimerSettingsWidget::Mode)));
    connect(_timerSettingsWidget, SIGNAL(FixedIntervalChanged(int)), this, SLOT(FixedIntervalChanged(int)));
    connect(_timerSettingsWidget, SIGNAL(RandomIntervalChanged(int,int)), this, SLOT(RandomIntervalChanged(int,int)));
    connect(_timerSettingsWidget, SIGNAL(ModeChanged(TimerSettingsWidget::Mode)), this, SLOT(SetTimer()));
    connect(_timerSettingsWidget, SIGNAL(FixedIntervalChanged(int)), this, SLOT(SetTimer()));
    connect(_timerSettingsWidget, SIGNAL(RandomIntervalChanged(int,int)), this, SLOT(SetTimer()));
    connect(_audioOutput, SIGNAL(volumeChanged(qreal)), this, SLOT(VolumeChanged(qreal)));

    connect(_timer, SIGNAL(timeout()), this, SLOT(Alarm()));

    Phonon::createPath(_player, _audioOutput);
    SetTimer();
    _soundNotificationWidget->SetAudioOutput(_audioOutput);

#ifndef Q_OS_SYMBIAN
    _trayIcon->show();
#endif // Q_OS_SYMBIAN

    LoadSettings();
    this->adjustSize();
}

// Destructor
MakeRT::~MakeRT()
{
    delete _ui;
    delete _player;
    delete _audioOutput;
    delete _timer;
    delete _textNotificationWidget;
    delete _soundNotificationWidget;
    delete _timerSettingsWidget;
#ifdef Q_OS_SYMBIAN
    delete _textNotificationAction;
    delete _soundNotificationAction;
    delete _timerSettingsAction;
    delete _vibrationsMenu;
    delete _vibrationsOffAction;
    delete _vibrationsOnAction;
    delete _aboutQtAction;
    delete _quitAction;
    delete _menu;
    delete _menuAction;
    delete _hideAction;
    delete _vibrator;
#else
    delete _trayIcon;
#endif // Q_OS_SYMBIAN
    _instance = 0;
}

// LoadSettings
void MakeRT::LoadSettings()
{
    _textNotificationWidget->SetActive(_settings->value(TEXTNOTIFICATIONENABLED, true).toBool());
    _textNotificationWidget->SetMode((TextNotificationWidget::Mode)_settings->value(TEXTNOTIFICATIONMODE, (int)TextNotificationWidget::RandomMessageMode).toInt());
    _textNotificationWidget->SetTextMessage(_settings->value(TEXTMESSAGE, tr("Are you dreaming?")).toString());
    _textNotificationWidget->SetMessageList(_settings->value(MESSAGELIST, TextNotificationWidget::_defaultMessageList).toStringList());
    _soundNotificationWidget->SetActive(_settings->value(SOUNDNOTIFICATIONENABLED, false).toBool());
    _soundNotificationWidget->SetFileName(_settings->value(AUDIOFILENAME, "").toString());
    _timerSettingsWidget->SetTimerMode((TimerSettingsWidget::Mode)_settings->value(TIMERMODE, (int)TimerSettingsWidget::RandomInterval).toInt());
    _timerSettingsWidget->SetFixedInterval(_settings->value(FIXEDINTERVAL, 15).toInt());
    int from = _settings->value(RANDOMINTERVALFROM, 10).toInt();
    int to = _settings->value(RANDOMINTERVALTO, 30).toInt();
    _timerSettingsWidget->SetRandomInterval(from, to);
#ifdef Q_OS_SYMBIAN
    bool vibrations = _settings->value(VIBRATIONSENABLED, true).toBool();
    if (vibrations)
    {
        //_vibrationsOnAction->activate(QAction::Trigger);
        _vibrationsOnAction->setChecked(true);
    }
    else
    {
        //_vibrationsOffAction->activate(QAction::Trigger);
        _vibrationsOffAction->setChecked(true);
    }
#else
    _ui->startup->setChecked(_settings->value(RUNATSTARTUP, false).toBool());
    _ui->tray->setChecked(_settings->value(RUNINTRAY, false).toBool());
#endif // Q_OS_SYMBIAN
    _audioOutput->setVolume(_settings->value(VOLUME, 100.0).toReal());
}

// SetTimer
void MakeRT::SetTimer()
{
    int interval;
    if (_timerSettingsWidget->GetTimerMode() == TimerSettingsWidget::FixedInterval)
    {
        interval = _timerSettingsWidget->GetFixedInterval();
    }
    else
    {
        int from, to;
        from = _timerSettingsWidget->GetRandomIntervalFrom();
        to = _timerSettingsWidget->GetRandomIntervalTo();
        interval = qrand() % (to - from) + from;
    }
    _timer->start(interval*60000);
}

// Alarm
void MakeRT::Alarm()
{
    if (_soundNotificationWidget->IsActive())
    {
        _player->enqueue(_soundNotificationWidget->GetFileName());
        _player->play();
    }

#ifdef Q_OS_SYMBIAN
    if (_vibrationsOnAction->isChecked())
    {
        _vibrator->StartVibraL(4000);
    }
#endif // Q_OS_SYMBIAN

    if (_textNotificationWidget->IsActive())
    {
        QString message;
        if (_textNotificationWidget->GetMode() == TextNotificationWidget::SingleMessageMode)
        {
            message = _textNotificationWidget->GetTextMessage();
        }
        else
        {
            int tmp = _textNotificationWidget->GetMessageList().count();
            message = _textNotificationWidget->GetMessageList()[qrand() % tmp];
        }
        _messageBox->setText(message);

#ifdef Q_OS_SYMBIAN
        this->raise();
        _messageBox->raise();
#endif // Q_OS_SYMBIAN

        _messageBox->exec();

#ifdef Q_OS_SYMBIAN
        this->lower();
#endif // Q_OS_SYMBIAN
    }

    if (_soundNotificationWidget->IsActive())
        _player->clear();

#ifdef Q_OS_SYMBIAN
    _vibrator->StopVibraL();
#endif // Q_OS_SYMBIAN

    SetTimer();
}

// TextMessageActivationChanged
void MakeRT::TextMessageActivationChanged(bool activation)
{
    _settings->setValue(TEXTNOTIFICATIONENABLED, activation);
}

// TextMessageChanged
void MakeRT::TextMessageChanged(QString message)
{
    _settings->setValue(TEXTMESSAGE, message);
}

// MessageListChanged
void MakeRT::MessageListChanged(QStringList &messageList)
{
    _settings->setValue(MESSAGELIST, messageList);
}

// TextMessageModeChanged
void MakeRT::TextMessageModeChanged(TextNotificationWidget::Mode mode)
{
    _settings->setValue(TEXTNOTIFICATIONMODE, (int)mode);
}

// SoundNotificationActivationChanged
void MakeRT::SoundNotificationActivationChanged(bool activation)
{
    _settings->setValue(SOUNDNOTIFICATIONENABLED, activation);
}

// AudioFileNameChanged
void MakeRT::AudioFileNameChanged(QString fileName)
{
    _settings->setValue(AUDIOFILENAME, fileName);
}

// TimerModeChanged
void MakeRT::TimerModeChanged(TimerSettingsWidget::Mode mode)
{
    _settings->setValue(TIMERMODE, (int)mode);
}

// FixedIntervalChanged
void MakeRT::FixedIntervalChanged(int interval)
{
    _settings->setValue(FIXEDINTERVAL, interval);
}

// RandomIntervalChanged
void MakeRT::RandomIntervalChanged(int from, int to)
{
    _settings->setValue(RANDOMINTERVALFROM, from);
    _settings->setValue(RANDOMINTERVALTO, to);
}

// VibrationsEnabled
#ifdef Q_OS_SYMBIAN
void MakeRT::VibrationsEnabled()
{
    bool vibrations = _vibrationsOnAction->isChecked();
    _settings->setValue(VIBRATIONSENABLED, vibrations);
}
#else

// RunAtStartupEnabled
void MakeRT::RunAtStartupEnabled(bool startup)
{
    _settings->setValue(RUNATSTARTUP, startup);

#ifdef Q_OS_WIN
    HKEY key;
    RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &key);
    if (startup)
    {
        QString appFileName = qApp->applicationFilePath();
        if (RegSetValueExA(key, "MakeRT", 0, REG_SZ, (BYTE*)appFileName.toStdString().c_str(), appFileName.length()) != ERROR_SUCCESS)
        {
            QMessageBox::critical(this, tr("Failed!"), tr("Cannot set autostart in registry."));
            RunAtStartupEnabled(false);
        }
    }
    else
    {
        RegDeleteValueA(key, "MakeRT");
    }
    RegCloseKey(key);
#elif defined Q_OS_LINUX
    QString dir = QDir::homePath() + "/.config/autostart/";
    QString fileName = "MakeRT.desktop";
    QString data;
    if (startup)
    {
        QDir tmp(dir);
        if (!tmp.exists())
            tmp.mkpath(dir);
        QFile file(dir + fileName);
        if (!file.open(QFile::WriteOnly))
        {
            QMessageBox::critical(this, tr("Failed!"), tr("Cannot create autostart file!"));
            RunAtStartupEnabled(false);
            return;
        }

        data = "[Desktop Entry]\n";
        data += "Type=Application\n";
        data += "Name=MakeRT\n";
        data += "Exec=" + qApp->applicationFilePath() + "\n";
        file.write(data.toStdString().c_str());
        file.close();
    }
    else
        QFile::remove(dir + fileName);
#endif // Q_OS_WIN
}

// RunInTrayEnabled
void MakeRT::RunInTrayEnabled(bool tray)
{
    _settings->setValue(RUNINTRAY, tray);
}

// TrayIconClicked
void MakeRT::TrayIconClicked()
{
    if (this->isVisible())
        this->hide();
    else this->show();
}

#endif // Q_OS_SYMBIAN

// VolumeChanged
void MakeRT::VolumeChanged(qreal volume)
{
    _settings->setValue(VOLUME, volume);
}
