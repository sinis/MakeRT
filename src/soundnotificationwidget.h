///////////////////////////////////////////////////////////////////////////////
// Sound notification settings dialog.

#ifndef SOUNDNOTIFICATIONWIDGET_H
#define SOUNDNOTIFICATIONWIDGET_H

#include <QWidget>
#ifdef Q_OS_SYMBIAN
#include "ui_soundnotificationwidget_s60.h"
#include <QAction>
#else
#include "ui_soundnotificationwidget.h"
#endif // Q_OS_SYMBIAN
#include <MediaObject>
#include <AudioOutput>

class SoundNotificationWidget: public QWidget
{
    Q_OBJECT
public:
    SoundNotificationWidget(QWidget *parent = 0, Phonon::MediaObject *player = 0);
    ~SoundNotificationWidget();

    bool IsActive() const { return _ui->enable->isChecked(); }
    void SetActive(bool active) { _ui->enable->setChecked(active); }
    const QString &GetFileName() const { return _ui->fileName->text(); }
    void SetFileName(const QString &fileName) { _ui->fileName->setText(fileName); }
    void SetPlayer(Phonon::MediaObject *player) { _player = player; }
    void SetAudioOutput(Phonon::AudioOutput *output);

public slots:
    void SelectFileName();
    void PlayStop();
    void StateChanged(Phonon::State);

private slots:
    void ActivationChange(bool);
    void FileNameChange(QString);

signals:
    void ActivationChanged(bool);
    void FileNameChanged(QString);

private:
    Ui::SoundNotificationWidget *_ui;
#ifdef Q_OS_SYMBIAN
    QAction *_emptyAction;
    QAction *_goBackAction;
#endif // Q_OS_SYMBIAN
    Phonon::MediaObject *_player;
};

#endif // SOUNDNOTIFICATIONWIDGET_H ///////////////////////////////////////////
