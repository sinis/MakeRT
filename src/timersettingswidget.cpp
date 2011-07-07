#include "timersettingswidget.h"
#include <QMessageBox>

// Constructor
TimerSettingsWidget::TimerSettingsWidget(QWidget *parent):
    QWidget(parent),
    _ui(new Ui::TimerSettingsWidget)
  #ifdef Q_OS_SYMBIAN
    ,_emptyAction(QAction("", this)),
    _goBackAction(QAction(tr("Go back", this)))
  #endif // Q_OS_SYMBIAN
{
    _ui->setupUi(this);

    connect(_ui->fixedInterval, SIGNAL(toggled(bool)), this, SLOT(ModeChange(TimerMode)));
    connect(_ui->interval, SIGNAL(valueChanged(int)), this, SLOT(FixedIntervalChange(int)));
    connect(_ui->randomFrom, SIGNAL(valueChanged(int)), this, SLOT(RandomIntervalChangeFrom(int)));
    connect(_ui->randomTo, SIGNAL(valueChanged(int)), this, SLOT(RandomIntervalChangeTo(int)));

#ifdef Q_OS_SYMBIAN
    this->addAction(_emptyAction);
    this->addAction(_goBackAction);
    _emptyAction->setSoftKeyRole(QAction::PositiveSoftKey);
    _goBackAction->setSoftKeyRole(QAction::NegativeSoftKey);
    connect(_goBackAction, SIGNAL(triggered()), this, SLOT(hide()));
#endif
}

// Destructor
TimerSettingsWidget::~TimerSettingsWidget()
{
    delete _ui;
#ifdef Q_OS_SYMBIAN
    delete _emptyAction;
    delete _goBackAction;
#endif
}

// GetTimerMode
// Takes mode from UI and returns it as TimerMode.
TimerMode TimerSettingsWidget::GetTimerMode()
{
    TimerMode mode;
    if (_ui->fixedInterval->isChecked())
        mode = FixedInterval;
    else mode = RandomInterval;
    return mode;
}

// SetTimerMode
// Takes mode from parameter and sets UI.
void TimerSettingsWidget::SetTimerMode(TimerMode timerMode)
{
    switch (timerMode)
    {
    case FixedInterval:
        _ui->fixedInterval->setChecked(true);
        break;
    case RandomInterval:
        _ui->randomInterval->setChecked(true);
        break;
    }
}

// SetRandomInterval
void TimerSettingsWidget::SetRandomInterval(int from, int to)
{
    _ui->randomFrom->setValue(from);
    _ui->randomTo->setValue(to);
}

// FixedIntervalChange
void TimerSettingsWidget::FixedIntervalChange(int interval)
{
    emit FixedIntervalChanged(interval);
}

// RandomIntervalChangeFrom
void TimerSettingsWidget::RandomIntervalChangeFrom(int from)
{
    if (from >= this->GetRandomIntervalTo())
    {
        QMessageBox::warning(this, tr("Warning!"), tr("Wrong interval! First value must be lower than the second."));
        return;
    }
    emit RandomIntervalChanged(from, GetRandomIntervalFrom());
}

// RandomIntervalChangeTo
void TimerSettingsWidget::RandomIntervalChangeTo(int to)
{
    if (to <= this->GetRandomIntervalFrom())
    {
        QMessageBox::warning(this, tr("Warning!"), tr("Wrong interval! First value must be lower than the second."));
        return;
    }
    emit RandomIntervalChangeTo(to);
}

// ModeChange
void TimerSettingsWidget::ModeChange(TimerMode mode)
{
    emit ModeChanged(mode);
}
