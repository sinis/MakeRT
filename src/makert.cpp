#include "makert.h"

MakeRT *MakeRT::_instance = 0;

// Constructor
MakeRT::MakeRT(QWidget *parent):
    QWidget(parent),
    _ui(new Ui::MakeRT),
    _textNotificationWidget(new TextNotificationWidget(this)),
    _soundNotificationWidget(new SoundNotificationWidget(this)),
    _timerSettingsWidget(new TimerSettingsWidget(this)),
    _audioOutput(new Phonon::AudioOutput(Phonon::MusicCategory, this)),
    _player(new Phonon::MediaObject(this)),
    _timer(new QTimer(this))
  #ifndef Q_OS_SYMBIAN
  , _trayIcon(new QSystemTrayIcon(":/icon.png", this))
  #else
  , _textNotificationAction(new QAction(tr("Text notifications"), this)),
    _soundNotificationAction(new QAction(tr("Sound notification"), this)),
    _timerSettingsAction(new QAction(tr("Timer settings"), this)),
    _vibrationsAction(new QAction(tr("Vibrations"), this)),
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
    // checkable _vibrationsAction
#else
    _ui->tabWidget->addTab(_textNotificationWidget, tr("Text notifications settings"));
    _ui->tabWidget->addTab(_soundNotificationWidget, tr("Sound notification settigns"));
    _ui->tabWidget->addTab(_timerSettingsWidget, tr("Timer settings"));

    connect(_ui->quit, SIGNAL(clicked()), qApp, SLOT(quit()));
#endif // Q_OS_SYMBIAN

    Phonon::createPath(_player, _output);
    LoadSettings();
    SetTimer();
}
