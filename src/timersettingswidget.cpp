#include "src/timersettingswidget.h"
#include "ui_timersettingswidget.h"

TimerSettingsWidget::TimerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimerSettingsWidget)
{
    ui->setupUi(this);
}

TimerSettingsWidget::~TimerSettingsWidget()
{
    delete ui;
}
