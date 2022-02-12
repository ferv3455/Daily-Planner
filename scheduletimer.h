#ifndef SCHEDULETIMER_H
#define SCHEDULETIMER_H

#include "schedule.h"
#include "settings.h"

#include <QObject>
#include <QTimer>

class ScheduleTimer : public QObject
{
    Q_OBJECT
public:
    enum state { REST, TASK, OVER };

public:
    ScheduleTimer(Settings *settings, QObject *parent = nullptr);
    ~ScheduleTimer();

    // Load from file
    void loadSchedule();

    // Interfaces
    QString endTime();

private:
    // Settings
    Settings *m_settings;

    // Schedule related
    Schedule m_schedule;
    QTime m_endTime;
    int m_currentTask;

    // Calendar alarm related


    // Time ticking
    QTimer m_timer;

signals:
    void stateChanged(state);

public slots:
    void updateTick();

};

#endif // SCHEDULETIMER_H
