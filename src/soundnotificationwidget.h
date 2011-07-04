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

class SoundNotificationWidget: public QWidget
{
    Q_OBJECT
public:
    SoundNotificationWidget(QWidget *parent = 0);
    ~SoundNotificationWidget();

    bool IsActive() const { return _ui->enable->isChecked(); }
    void SetActive(bool active) { _ui->enable->setChecked(active); }
    const QString &GetFileName() const { _ui->fileName->text(); }
    void SetFileName(const QString &fileName) { _ui->fileName->setText(fileName); }

public slots:
    void SelectFileName();
    void PlayStop();
    void StateChanged();

private:
    Ui::SoundNotificationWidget *_ui;
#ifdef Q_OS_SYMBIAN
    QAction *_emptyAction;
    QAction *_goBackAction;
#endif // Q_OS_SYMBIAN
    Phonon::MediaObject *_player;
};

#endif // SOUNDNOTIFICATIONWIDGET_H ///////////////////////////////////////////
