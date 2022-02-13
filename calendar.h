#ifndef CALENDAR_H
#define CALENDAR_H

#include "dayrecord.h"

#include <QObject>
#include <QDate>

class Calendar : public QObject
{
    Q_OBJECT
public:
    explicit Calendar(QObject *parent = nullptr);

    // Load and save with files
    void load();
    void save() const;

    // Interfaces
    int indexOf(const QDate &date) const;
    const DayRecord &record(int index) const;

    void addDate(const QDate &date);
    void deleteDate(int index);
    bool clearRecords(const QDate &date1, const QDate &date2);

    bool addRemark(const QDate &date, const QString &remark);
    bool deleteRemark(const QDate &date, int index);
    bool switchRemark(const QDate &date, int index, int offset);

    bool addAlarm(const QDate &date, const QTime &time);
    bool deleteAlarm(const QDate &date, int index);

    const QString &memo() const;
    void setMemo(const QString &newMemo);

private:
    QVector<DayRecord> m_dayRecords;
    QString m_memo;

signals:

};

#endif // CALENDAR_H
