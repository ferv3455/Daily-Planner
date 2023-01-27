#ifndef TRAYICON_H
#define TRAYICON_H

#include "mainwindow.h"
#include "settings.h"
#include "scheduletimer.h"
#include "locker.h"

#include <QSystemTrayIcon>
#include <QApplication>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit TrayIcon(QObject *parent = nullptr);
    ~TrayIcon();

private:
    // Qt window items
    MainWindow *m_window;
    Locker *m_locker;

    // Data
    Settings *m_settings;
    ScheduleTimer *m_timer;

    // Context Menu
    QMenu *m_menu;
    QAction *m_options;

signals:
    void programQuitted();

public slots:
    // Inter-widget communication
    void alarmMessage(const QTime &time);
    void changeState(ScheduleTimer::state s, int taskID = 0, int num = 0);
    void warnLocker();
    void lockerState(bool state);

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
