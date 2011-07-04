#include "src/textnotificationwidget.h"
#include "ui_textnotificationwidget.h"

TextNotificationWidget::TextNotificationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextNotificationWidget)
{
    ui->setupUi(this);
}

TextNotificationWidget::~TextNotificationWidget()
{
    delete ui;
}
