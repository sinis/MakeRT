#include "textnotificationwidget.h"

QStringList TextNotificationWidget::_defaultMessageList;

// Constructor
TextNotificationWidget::TextNotificationWidget(QWidget *parent):
    #ifdef Q_OS_SYMBIAN
    QDialog(parent),
    #else
    QWidget(parent),
    #endif // Q_OS_SYMBIAN
    _ui(new Ui::TextNotificationWidget),
    _editor(new EditorDialog(this))
  #ifdef Q_OS_SYMBIAN
  , _emptyAction(new QAction("", this)),
    _goBackAction(new QAction(tr("Go back"), this))
  #endif // Q_OS_SYMBIAN
{
    _ui->setupUi(this);

    connect(_ui->enable, SIGNAL(toggled(bool)), this, SLOT(ActivationChange(bool)));
    connect(_ui->singleNotificationMode, SIGNAL(toggled(bool)), this, SLOT(ModeChange()));
    connect(_ui->textNotification, SIGNAL(textEdited(QString)), this, SLOT(TextMessageChange(QString)));
    connect(_ui->showEditor, SIGNAL(clicked()), this, SLOT(ShowEditor()));
    connect(_editor, SIGNAL(accepted()), this, SLOT(MessageListChange()));
#ifdef Q_OS_SYMBIAN
    addAction(_emptyAction);
    addAction(_goBackAction);
    _emptyAction->setSoftKeyRole(QAction::PositiveSoftKey);
    _goBackAction->setSoftKeyRole(QAction::NegativeSoftKey);
    connect(_goBackAction, SIGNAL(triggered()), this, SLOT(hide()));
#endif // Q_OS_SYMBIAN

    _editor->setModal(true);
    _defaultMessageList.clear();
    _defaultMessageList.append(tr("Are you dreaming?"));
    _defaultMessageList.append(tr("How did you get here?"));
    _defaultMessageList.append(tr("So... what's the time?"));
    _defaultMessageList.append(tr("Isn't there anything strange?"));
    _defaultMessageList.append(tr("Reality check time!"));
    _defaultMessageList.append(tr("I'm afraid we're in dream."));
    _defaultMessageList.append(tr("Can you fly?"));
}

// Destructor
TextNotificationWidget::~TextNotificationWidget()
{
    delete _ui;
    delete _editor;
#ifdef Q_OS_SYMBIAN
    delete _emptyAction;
    delete _goBackAction;
#endif // Q_OS_SYMBIAN
}

// SetMode
void TextNotificationWidget::SetMode(Mode mode)
{
    switch (mode)
    {
    case SingleMessageMode:
        _ui->singleNotificationMode->setChecked(true);
        break;
    case RandomMessageMode:
        _ui->randomNotification->setChecked(true);
        break;
    }
}

// GetMode
TextNotificationWidget::Mode TextNotificationWidget::GetMode()
{
    if (_ui->singleNotificationMode->isChecked())
        return SingleMessageMode;
    else return RandomMessageMode;
}

// ActivationChange
void TextNotificationWidget::ActivationChange(bool activation)
{
    emit ActivationChanged(activation);
}

// TextMessageChange
void TextNotificationWidget::TextMessageChange(QString message)
{
    emit TextMessageChanged(message);
}

// MessageListChange
void TextNotificationWidget::MessageListChange()
{
    _messageList = _editor->GetMessageList();
    emit MessageListChanged(_messageList);
}

// ModeChange
void TextNotificationWidget::ModeChange()
{
    emit ModeChanged(GetMode());
}

// ShowEditor
void TextNotificationWidget::ShowEditor()
{
    _editor->SetMessageList(_messageList);
#ifndef Q_OS_SYMBIAN
    _editor->show();
#else
    _editor->showMaximized();
#endif // Q_OS_SYMBIAN
}
