#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include <QTime>

class Schedule : public QObject
{
    Q_OBJECT
public:
    explicit Schedule(QObject *parent = nullptr);

    // Load and save with files
    void load();
    void save() const;

    // Getters
    const QVector<QTime> &beginTime();
    const QVector<QTime> &endTime();
    int size();

    // Edit the schedule
    bool add(const QTime &begin, const QTime &end);
    bool remove(int index);

private:
    QVector<QTime> m_beginTime;
    QVector<QTime> m_endTime;
    int m_count;

signals:

};

#endif // SCHEDULE_H
