#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "schedule.h"

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void closeEvent(QCloseEvent *event);
    void refreshSchedule();

private:
    // UI-related
    Ui::MainWindow *ui;

    // Data
    Schedule m_schedule;

signals:
    void closed();

};
#endif // MAINWINDOW_H
