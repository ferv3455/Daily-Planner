#include "scheduletimer.h"

#include <calendar.h>
#include <QDebug>

ScheduleTimer::ScheduleTimer(Settings *settings, QObject *parent) :
    QObject(parent),
    m_settings(settings),
    m_schedule(this),
    m_endTime(0, 0, 0),
    m_currentTask(0),
    m_alarms(),
    m_currentAlarm(-1),
    m_timer(this)
{
    // Initialize settings

    // Set timer
    connect(&m_timer, &QTimer::timeout, this, &ScheduleTimer::updateTick);
    m_timer.start(1000);
}

ScheduleTimer::~ScheduleTimer()
{

}

void ScheduleTimer::loadSchedule()
{
    Calendar calendar;
    calendar.load();
    int id = calendar.indexOf(QDate::currentDate());
    if (id >= 0)
    {
        m_alarms = calendar.record(id).m_alarms;
        m_currentAlarm = std::lower_bound(m_alarms.begin(), m_alarms.end(),
                                          QTime::currentTime()) - m_alarms.begin();
        if (m_currentAlarm >= m_alarms.size())
        {
            m_currentAlarm = -1;
            m_alarms.clear();
        }
    }

    m_endTime = QTime(0, 0, 0);
    m_currentTask = 0;
    m_schedule.load();
}

QString ScheduleTimer::endTime()
{
    return m_endTime.toString();
}

void ScheduleTimer::updateTick()
{
    QTime now = QTime::currentTime();
    if (m_currentAlarm >= 0 && now >= m_alarms[m_currentAlarm])
    {
        // Alarm clock
        emit alarmClock(now);
        m_currentAlarm++;
        if (m_currentAlarm >= m_alarms.size())
        {
            m_currentAlarm = -1;
            m_alarms.clear();
        }
    }

    if (now >= m_endTime)
    {
        // Current task ends
        const QVector<QTime> &beginTime = m_schedule.beginTime();
        const QVector<QTime> &endTime = m_schedule.endTime();

        // Finding the next task
        bool midDay = true;
        while (true)
        {
            if (m_currentTask >= m_schedule.size())
            {
                // End of the day: no tasks remaining
                midDay = false;
                break;
            }
            if (now <= endTime[m_currentTask])
            {
                // A task that hasn't ended (it may even haven't begun)
                break;
            }
            m_currentTask++;
        }

        if (midDay)
        {
            // In the middle of the day
            state s = (now >= beginTime[m_currentTask] ? TASK : REST);
            if (s == TASK)
            {
                m_endTime = endTime[m_currentTask];
            }
            else
            {
                m_endTime = beginTime[m_currentTask];
            }
            emit stateChanged(s);
        }
        else
        {
            // End of the day
            m_endTime = QTime(23, 59, 59, 999);
            emit stateChanged(OVER);
            if (m_settings->shutdown())
            {
                system("shutdown /s /t 300");
            }
        }
    }
}
