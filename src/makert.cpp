#include "makert.h"
#include "ui_makert.h"

// Allocate MakeRT static data.
MakeRT *MakeRT::_instance = 0;

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
