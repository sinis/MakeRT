#include "editordialog.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QFile>
#include <QMessageBox>

// Constructor
EditorDialog::EditorDialog(QWidget *parent):
    QDialog(parent),
    _ui(new Ui::EditorDialog)
  #ifdef Q_OS_SYMBIAN
  , _addNewAction(new QAction(tr("Add new"), this)),
    _editAction(new QAction(tr("Edit"), this)),
    _removeAction(new QAction(tr("Remove"), this)),
    _loadAction(new QAction(tr("Load"), this)),
    _saveAction(new QAction(tr("Save"), this)),
    _menu(new QMenu(this)),
    _menuAction(new QAction(tr("Menu"), this)),
    _cancelAction(new QAction(tr("Cancel"), this))
  #endif // Q_OS_SYMBIAN
{
    _ui->setupUi(this);

    connect(_ui->messageList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(Edit()));
    connect(_ui->messageList, SIGNAL(currentRowChanged(int)), this, SLOT(SelectionChanged(int)));
#ifdef Q_OS_SYMBIAN
    _menu->addAction(_addNewAction);
    _menu->addAction(_editAction);
    _menu->addAction(_removeAction);
    _menu->addAction(_loadAction);
    _menu->addAction(_saveAction);

    addAction(_menuAction);
    addAction(_editAction);
    addAction(_cancelAction);

    _menuAction->setMenu(_menu);
    _menuAction->setSoftKeyRole(QAction::PositiveSoftKey);
    _editAction->setSoftKeyRole(QAction::SelectSoftKey);
    _cancelAction->setSoftKeyRole(QAction::NegativeSoftKey);

    connect(_addNewAction, SIGNAL(triggered()), this, SLOT(AddNew()));
    connect(_editAction, SIGNAL(triggered()), this, SLOT(Edit()));
    connect(_removeAction, SIGNAL(triggered()), this, SLOT(Remove()));
    connect(_loadAction, SIGNAL(triggered()), this, SLOT(Load()));
    connect(_saveAction, SIGNAL(triggered()), this, SLOT(accept()));
    connect(_cancelAction, SIGNAL(triggered()), this, SLOT(reject()));
    connect(_ui->messageList, SIGNAL(entered(QModelIndex)), this, SLOT(Edit()));
#else
    connect(_ui->addNew, SIGNAL(clicked()), this, SLOT(AddNew()));
    connect(_ui->edit, SIGNAL(clicked()), this, SLOT(Edit()));
    connect(_ui->remove, SIGNAL(clicked()), this, SLOT(Edit()));
    connect(_ui->load, SIGNAL(clicked()), this, SLOT(Load()));
#endif // Q_OS_SYMBIAN
    SelectionChanged(_ui->messageList->currentRow());
}

// Destructor
EditorDialog::~EditorDialog()
{
    delete _ui;
#ifdef Q_OS_SYMBIAN
    delete _addNewAction;
    delete _editAction;
    delete _removeAction;
    delete _loadAction;
    delete _saveAction;
    delete _menu;
    delete _menuAction;
    delete _cancelAction;
#endif // Q_OS_SYMBIAN
}

// GetMessageList
QStringList EditorDialog::GetMessageList()
{
    QStringList list;
    for (int i = 0; i < _ui->messageList->count(); i++)
    {
        list.append(_ui->messageList->item(i)->text());
    }
    return list;
}

// SetMessageList
void EditorDialog::SetMessageList(const QStringList &list)
{
    _ui->messageList->clear();
    _ui->messageList->addItems(list);
}

// AddNew
void EditorDialog::AddNew()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("New notification"), tr("Message:"), QLineEdit::Normal, QString(), &ok);
    if (ok)
    {
        _ui->messageList->addItem(text);
    }
}

// Edit
void EditorDialog::Edit()
{
    bool ok;
    //int current = _ui->messageList->currentRow();
    QString text = _ui->messageList->currentItem()->text();
    text = QInputDialog::getText(this, tr("Edit notification"), tr("Message:"), QLineEdit::Normal, text, &ok);
    if (ok)
    {
        _ui->messageList->currentItem()->setText(text);
    }
}

// Remove
void EditorDialog::Remove()
{
    delete _ui->messageList->takeItem(_ui->messageList->currentRow());
}

// SelectionChanged
void EditorDialog::SelectionChanged(int row)
{
    if (row == -1)
    {
#ifdef Q_OS_SYMBIAN
        _editAction->setDisabled(true);
        _removeAction->setDisabled(true);
#else
        _ui->edit->setDisabled(true);
        _ui->remove->setDisabled(true);
#endif // Q_OS_SYMBIAN
    }
    else
    {
#ifdef Q_OS_SYMBIAN
        _editAction->setEnabled(true);
        _removeAction->setEnabled(true);
#else
        _ui->edit->setEnabled(true);
        _ui->remove->setEnabled(true);
#endif // Q_OS_SYMBIAN
    }
}

// keyPressEvent
void EditorDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        Remove();
        break;
    default:
        this->keyPressEvent(event);
    }
}

// Load
void EditorDialog::Load()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select file with notifications..."), QDir::homePath(), tr("Text files (*.txt);;All files (*.*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::critical(this, tr("Failed!"), tr("Cannot open selected file!"));
        return;
    }

    QByteArray line;
    while (!(line = file.readLine()).isEmpty())
    {
        _ui->messageList->addItem(line);
    }
}
