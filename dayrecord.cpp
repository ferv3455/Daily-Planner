#include "dayrecord.h"

#include <QJsonArray>

DayRecord::DayRecord(const QDate &date) :
    m_date(date),
    m_alarms(),
    m_remarks()
{

}

void DayRecord::load(const QJsonObject &json)
{
    if (json.contains("date") && json["date"].isString())
    {
        m_date = QDate::fromString(json["date"].toString());
    }

    if (json.contains("alarms") && json["alarms"].isArray())
    {
        QJsonArray alarms = json["alarms"].toArray();
        m_alarms.clear();
        m_alarms.reserve(alarms.size());
        for (int i = 0; i < alarms.size(); i++)
        {
            m_alarms.append(QTime::fromString(alarms[i].toString()));
        }
    }

    if (json.contains("remarks") && json["remarks"].isArray())
    {
        QJsonArray remarks = json["remarks"].toArray();
        m_remarks.clear();
        m_remarks.reserve(remarks.size());
        for (int i = 0; i < remarks.size(); i++)
        {
            m_remarks.append(remarks[i].toString());
        }
    }
}

void DayRecord::save(QJsonObject &json) const
{
    json["date"] = m_date.toString();

    QJsonArray alarmsArray;
    for (const QTime &t : m_alarms)
    {
        alarmsArray.append(t.toString());
    }
    json["alarms"] = alarmsArray;

    QJsonArray remarksArray;
    for (const QString &s : m_remarks)
    {
        remarksArray.append(s);
    }
    json["remarks"] = remarksArray;
}
