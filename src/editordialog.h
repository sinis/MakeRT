///////////////////////////////////////////////////////////////////////////////
// This editor will be used to manage notification list in random notification
// mode.

#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QDialog>
#ifndef Q_OS_SYMBIAN
#include "ui_editordialog.h"
#else
#include "ui_editordialog_s60.h"
#include <QAction>
#include <QMenu>
#endif // Q_OS_SYMBIAN
#include <QStringList>
#include <QKeyEvent>

class EditorDialog: public QDialog
{
    Q_OBJECT
public:
    EditorDialog(QWidget *parent = 0);
    ~EditorDialog();

    QStringList GetMessageList();
    void SetMessageList(const QStringList &list);

public slots:
    void AddNew();
    void Edit();
    void Remove();
    void Load();

private slots:
    void SelectionChanged(int row);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::EditorDialog *_ui;
#ifdef Q_OS_SYMBIAN
    QAction *_addNewAction;
    QAction *_editAction;
    QAction *_removeAction;
    QAction *_saveAction;
    QAction *_loadAction;
    QMenu *_menu;
    QAction *_menuAction;
    QAction *_cancelAction;
#endif // Q_OS_SYMBIAN
};

#endif // EDITORDIALOG_H //////////////////////////////////////////////////////
