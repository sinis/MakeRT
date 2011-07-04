///////////////////////////////////////////////////////////////////////////////
// Project: MakeRT
// Author: Sinis (m.siniss@gmail.com)
// Started: 3.07.2011
// Description: MakeRT is simple application that helps newbie lucid dreamers
//              to get a habit of making reality checks.
///////////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include "makert.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("MakeRT");
    app.setApplicationVersion("0.4");
    app.setQuitOnLastWindowClosed(false);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QTranslator translator;
    QString locale = QLocale::system().name();
    translator.load(QString(":/makert_") + locale);
    app.installTranslator(&translator);

    MakeRT window;

    return app.exec();
}