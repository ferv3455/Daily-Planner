#include "scheduletimer.h"

#include <QDebug>

ScheduleTimer::ScheduleTimer(QObject *parent) :
    QObject(parent),
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

void ScheduleTimer::updateTick()
{
    qDebug() << "tick";
}
