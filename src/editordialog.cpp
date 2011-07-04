#include "src/editordialog.h"
#include "ui_editordialog.h"

EditorDialog::EditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditorDialog)
{
    ui->setupUi(this);
}

EditorDialog::~EditorDialog()
{
    delete ui;
}
