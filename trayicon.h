#ifndef TRAYICON_H
#define TRAYICON_H

#include "mainwindow.h"

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
    QApplication *m_app;
    MainWindow *m_window;
    QMenu *m_menu;
    QAction *m_options;

public slots:
    void showMainWindow();
    void onActivated(ActivationReason);
    void onMessageClicked();
};

#endif // TRAYICON_H
