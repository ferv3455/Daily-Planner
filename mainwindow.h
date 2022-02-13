#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "schedule.h"
#include "calendar.h"

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

    void refreshCalendarVisual();
    void refreshCalendar();
    void refreshSchedule();

    void resetRange();

private:
    // UI-related
    Ui::MainWindow *ui;
    QVector<QPushButton *> m_labels;
    QVector<QListWidget *> m_alarms;
    QVector<QListWidget *> m_days;

    // Data
    Calendar m_calendar;
    QDate m_firstDate;
    int m_currentDay;
    Schedule m_schedule;

signals:
    void closed();

};
#endif // MAINWINDOW_H
