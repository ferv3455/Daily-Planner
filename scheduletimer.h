#ifndef SCHEDULETIMER_H
#define SCHEDULETIMER_H

#include <QObject>
#include <QTimer>

class ScheduleTimer : public QObject
{
    Q_OBJECT
public:
    explicit ScheduleTimer(QObject *parent = nullptr);
    ~ScheduleTimer();

    // Load from file

private:
    QTimer m_timer;

signals:

public slots:
    void updateTick();

};

#endif // SCHEDULETIMER_H
