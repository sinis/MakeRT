#include "makert.h"
#include "ui_makert.h"

MakeRT::MakeRT(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MakeRT)
{
    ui->setupUi(this);
}

MakeRT::~MakeRT()
{
    delete ui;
}
