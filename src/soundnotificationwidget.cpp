#include "soundnotificationwidget.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include "makert.h"

// Constructor
SoundNotificationWidget::SoundNotificationWidget(QWidget *parent):
    QWidget(parent),
    _ui(new Ui::SoundNotificationWidget),
    _player(new Phonon::MediaObject(this))
  #ifdef Q_OS_SYMBIAN
  , _emptyAction(new QAction("", this)),
    _goBackAction(new QAction(tr("Go back"), this))
  #endif // Q_OS_SYMBIAN
{
    _ui->setupUi(this);

    Phonon::AudioOutput *output = MakeRT::GetInstance()->GetAudioOutput();
    Phonon::createPath(_player, output);

#ifdef Q_OS_SYMBIAN
    addAction(_emptyAction);
    addAction(_goBackAction);
    _emptyAction->setSoftKeyRole(QAction::PositiveSoftKey);
    _goBackAction->setSoftKeyRole(QAction::NegativeSoftKey);
#endif

    connect(_ui->enable, SIGNAL(clicked(bool)), this, SLOT(ActivationChange(bool)));
    connect(_ui->play, SIGNAL(clicked()), this, SLOT(PlayStop()));
    connect(_player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(StateChanged(Phonon::State)));
#ifdef Q_OS_SYMBIAN
    connect(_ui->fileName, SIGNAL(clicked()), this, SLOT(SelectFileName()));
#else
    connect(_ui->fileName, SIGNAL(textChanged(QString)), this, SLOT(FileNameChange(QString)));
    connect(_ui->select, SIGNAL(clicked()), this, SLOT(SelectFileName()));
#endif // Q_OS_SYMBIAN
}

// Destructor
SoundNotificationWidget::~SoundNotificationWidget()
{
    delete _ui;
    delete _player;
#ifdef Q_OS_SYMBIAN
    delete _emptyAction;
    delete _goBackAction;
#endif // Q_OS_SYMBIAN
}

// ActivationChange
void SoundNotificationWidget::ActivationChange(bool enabled)
{
    emit ActivationChanged(enabled);
}

// FileNameChange
void SoundNotificationWidget::FileNameChange(QString fileName)
{
    emit FileNameChanged(fileName);
}

// SelectFileName
// It emits FileNameChanged() signal on Symbian.
void SoundNotificationWidget::SelectFileName()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select audio file..."), "", "Audio files (*.wav, *.mp3, *.ogg);;All files (*.*)");
    if (!fileName.isEmpty())
    {
        _ui->fileName->setText(fileName);
#ifdef Q_OS_SYMBIAN
        emit FileNameChanged(fileName);
#endif // Q_OS_SYMBIAN
    }
}

// PlayStop
// Checks if file exists and plays it.
void SoundNotificationWidget::PlayStop()
{
    if (_player->state() == Phonon::StoppedState)
    {
        QString fileName = _ui->fileName->text();
        if (!QFile::exists(fileName))
        {
            QMessageBox::warning(this, tr("Failed!"), tr("Audio file does not exist!"));
            return;
        }
        _player->enqueue(fileName);
        _player->play();
    }
    else
        _player->stop();
}

// StateChanged
void SoundNotificationWidget::StateChanged(Phonon::State state)
{
    switch (state)
    {
    case Phonon::PlayingState:
        _ui->play->setText(tr("Stop"));
        break;
    default:
        _ui->play->setText(tr("Play"));
        break;
    }
}
