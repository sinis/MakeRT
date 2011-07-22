///////////////////////////////////////////////////////////////////////////////
// This widget will be responsible for text notification settings.

#ifndef TEXTNOTIFICATIONWIDGET_H
#define TEXTNOTIFICATIONWIDGET_H

#include <QWidget>
#include "ui_textnotificationwidget.h"
#ifdef Q_OS_SYMBIAN
#include <QAction>
#endif // Q_OS_SYMBIAN
#include <QStringList>
#include "editordialog.h"

class TextNotificationWidget: public QWidget
{
    Q_OBJECT
public:
    TextNotificationWidget(QWidget *parent = 0);
    ~TextNotificationWidget();

    enum Mode
    {
        SingleMessageMode,
        RandomMessageMode
    };

    bool IsActive() const { return _ui->enable->isChecked(); }
    void SetActive(bool active) { _ui->enable->setChecked(active); }
    const QString GetTextMessage() const { return _ui->textNotification->text(); }
    void SetTextMessage(const QString &message) { _ui->textNotification->setText(message); }
    const QStringList GetMessageList() const { return _messageList; }
    void SetMessageList(const QStringList &messageList) { _messageList = messageList; }
    Mode GetMode();
    void SetMode(Mode mode);

    static QStringList _defaultMessageList;

public slots:
    void ShowEditor();

private slots:
    void ActivationChange(bool);
    void TextMessageChange(QString);
    void MessageListChange();
    void ModeChange();

signals:
    void ActivationChanged(bool);
    void TextMessageChanged(QString);
    void MessageListChanged(QStringList &);
    void ModeChanged(TextNotificationWidget::Mode);

private:
    Ui::TextNotificationWidget *_ui;
    EditorDialog *_editor;
    QStringList _messageList;
#ifdef Q_OS_SYMBIAN
    QAction *_emptyAction;
    QAction *_goBackAction;
#endif // Q_OS_SYMBIAN
};

#endif // TEXTNOTIFICATIONWIDGET_H ////////////////////////////////////////////
