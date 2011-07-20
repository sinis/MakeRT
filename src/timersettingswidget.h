///////////////////////////////////////////////////////////////////////////////
// This widget allow to change timing settings. Every change causes
// MakeRT::_timer reset.

#ifndef TIMERSETTINGSWIDGET_H
#define TIMERSETTINGSWIDGET_H

#include <QWidget>
#include "ui_timersettingswidget.h"
#ifdef Q_OS_SYMBIAN
#include <QAction>
#endif // Q_OS_SYMBIAN

class TimerSettingsWidget: public QWidget
{
    Q_OBJECT
public:
    TimerSettingsWidget(QWidget *parent = 0);
    ~TimerSettingsWidget();

    enum Mode
    {
        FixedInterval,
        RandomInterval
    };

    TimerSettingsWidget::Mode GetTimerMode();
    void SetTimerMode(Mode timerMode);
    int GetFixedInterval() const { return _ui->interval->value(); }
    void SetFixedInterval(int interval) { _ui->interval->setValue(interval); }
    int GetRandomIntervalFrom() const { return _ui->randomFrom->value(); }
    int GetRandomIntervalTo() const { return _ui->randomTo->value(); }
    void SetRandomInterval(int from, int to);

private slots:
    void FixedIntervalChange(int);
    void RandomIntervalChangeFrom(int);
    void RandomIntervalChangeTo(int);
    void ModeChange();

signals:
    void FixedIntervalChanged(int);
    void RandomIntervalChanged(int, int);
    void ModeChanged(TimerSettingsWidget::Mode);

private:
    Ui::TimerSettingsWidget *_ui;
#ifdef Q_OS_SYMBIAN
    QAction *_emptyAction;
    QAction *_goBackAction;
#endif // Q_OS_SYMBIAN
};

#endif // TIMERSETTINGSWIDGET_H ///////////////////////////////////////////////
