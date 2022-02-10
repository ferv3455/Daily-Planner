#ifndef TRAYICON_H
#define TRAYICON_H

#include "mainwindow.h"
#include "settings.h"
#include "scheduletimer.h"

#include <QSystemTrayIcon>
#include <QApplication>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    TrayIcon(QApplication *app, QObject *parent = nullptr);
    ~TrayIcon();

private:
    // Qt window items
    QApplication *m_app;
    MainWindow *m_window;

    // Data
    Settings m_settings;
    ScheduleTimer m_timer;

    // Context Menu
    QMenu *m_menu;
    QAction *m_options;

signals:
    void programQuitted();

public slots:
    // Basic functions
    void showMainWindow();
    void fullscreen();
    void changeShutdown();
    void showAbout();
    void quit();

    // Basic interactions
    void onActivated(ActivationReason);
};

#endif // TRAYICON_H
