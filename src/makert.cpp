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

    LoadSettings();

#ifdef Q_OS_SYMBIAN
    addAction(_menuAction);
    addAction(_hideAction);
    _menu->addAction(_textNotificationAction);
    _menu->addAction(_soundNotificationAction);
    _menu->addAction(_timerSettingsAction);
    _menu->addAction(_vibrationsAction);
    _menu->addAction(_aboutQtAction);
    _menu->addAction(_quitAction);
    _vibrationsAction->setCheckable(true);
    _menuAction->setMenu(_menu);
    _menuAction->setSoftKeyRole(QAction::PositiveSoftKey);
    _hideAction->setSoftKeyRole(QAction::NegativeSoftKey);

    connect(_textNotificationAction, SIGNAL(triggered()), _textNotificationWidget, SLOT(show()));
    connect(_soundNotificationAction, SIGNAL(triggered()), _soundNotificationWidget, SLOT(show()));
    connect(_timerSettingsAction, SIGNAL(triggered()), _timerSettingsWidget, SLOT(show()));
    connect(_vibrationsAction, SIGNAL(triggered(bool)), this, SLOT(VibrationsEnabled(bool)));
    connect(_aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(_hideAction, SIGNAL(triggered()), this, SLOT(hide()));
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
    connect(_timerSettingsWidget, SIGNAL(ModeChanged(Mode)), this, SLOT(TimerModeChanged(TimerSettingsWidget::Mode)));
    connect(_timerSettingsWidget, SIGNAL(FixedIntervalChanged(int)), this, SLOT(FixedIntervalChanged(int)));
    connect(_timerSettingsWidget, SIGNAL(RandomIntervalChanged(int,int)), this, SLOT(RandomIntervalChanged(int,int)));

    connect(_timer, SIGNAL(timeout()), this, SLOT(Alarm()));


    Phonon::createPath(_player, _audioOutput);
    SetTimer();
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
    delete _vibrationsAction;
    delete _aboutQtAction;
    delete _quitAction;
    delete _menu;
    delete _menuAction;
    delete _hideAction;
#else
    delete _trayIcon;
#endif // Q_OS_SYMBIAN
    _instance = 0;
}
