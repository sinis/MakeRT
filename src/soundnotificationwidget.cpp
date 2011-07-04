#include "src/soundnotificationwidget.h"
#include "ui_soundnotificationwidget.h"

SoundNotificationWidget::SoundNotificationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoundNotificationWidget)
{
    ui->setupUi(this);
}

SoundNotificationWidget::~SoundNotificationWidget()
{
    delete ui;
}
