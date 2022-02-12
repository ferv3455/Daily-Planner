#include "mainwindow.h"
#include "trayicon.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    // Initializing an application
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);     // Exit controlled by trayicon

    // Setting fonts
//    QApplication::setFont(QFont("Microsoft YaHei", 12));

    // Loading a translator
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

    // Creating a TrayIcon
    TrayIcon c;
    a.connect(&c, &TrayIcon::programQuitted, &QApplication::quit);  // Exit on quitting the program

    return a.exec();
}
