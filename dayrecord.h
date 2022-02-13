#ifndef DAYRECORD_H
#define DAYRECORD_H

#include <QJsonObject>
#include <QDate>
#include <QTime>

class DayRecord
{
public:
    DayRecord(const QDate &date = QDate(2022, 2, 13));

    // Load and save with json object
    void load(const QJsonObject &json);
    void save(QJsonObject &json) const;

public:
    QDate m_date;
    QVector<QTime> m_alarms;
    QVector<QString> m_remarks;
};

#endif // DAYRECORD_H
