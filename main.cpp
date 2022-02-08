#include "mainwindow.h"
#include "trayicon.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);     // closing controlled by trayicon

    // Load Translator
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "Daily-Planner_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    // Create window
//    MainWindow w(&a);
//    w.show();

    // Create Icon
    TrayIcon c(&a);

    return a.exec();
}
