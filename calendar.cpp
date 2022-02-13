#include "calendar.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Calendar::Calendar(QObject *parent) :
    QObject(parent),
    m_dayRecords(),
    m_memo()
{

}

void Calendar::load()
{
    // Trying to open file settings.json
    QFile file("./save/calendar.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        // Couldn't open file: use default settings
        qDebug() << "Couldn't load save file.";
        return;
    }

    // Reading the file into a json object
    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject json = loadDoc.object();

    // Initializing settings from json object
    if (json.contains("records") && json["records"].isArray())
    {
        QJsonArray records = json["records"].toArray();
        m_dayRecords.clear();
        m_dayRecords.resize(records.size());
        for (int i = 0; i < records.size(); i++)
        {
            QJsonObject obj = records[i].toObject();
            m_dayRecords[i].load(obj);
        }
    }

    if (json.contains("memo") && json["memo"].isString())
    {
        m_memo = json["memo"].toString();
    }
}

void Calendar::save() const
{
    // Trying to open file settings.json
    QFile file("./save/calendar.json");
    if (!file.open(QIODevice::WriteOnly))
    {
        // Couldn't open file: skip
        qDebug() << "Couldn't open save file.";
        return;
    }

    // Contructing a json object from current settings
    QJsonObject json;
    QJsonArray records;
    for (const DayRecord &d : m_dayRecords)
    {
        QJsonObject obj;
        d.save(obj);
        records.append(obj);
    }
    json["records"] = records;
    json["memo"] = m_memo;

    // Saving the object into the file
    QJsonDocument saveDoc(json);
    file.write(saveDoc.toJson());
}

int Calendar::indexOf(const QDate &date) const
{
    int size = m_dayRecords.size();
    for (int i = 0; i < size; i++)
    {
        if (m_dayRecords[i].m_date >= date)
        {
            if (m_dayRecords[i].m_date == date)
            {
                return i;
            }
            else
            {
                return -1;
            }
        }
    }
    return -1;
}

const DayRecord &Calendar::record(int index) const
{
    return m_dayRecords[index];
}

void Calendar::addDate(const QDate &date)
{
    int size = m_dayRecords.size();
    for (int i = 0; i < size; i++)
    {
        if (m_dayRecords[i].m_date >= date)
        {
            if (m_dayRecords[i].m_date == date)
            {
                return;
            }
            else
            {
                m_dayRecords.insert(i, DayRecord(date));
                return;
            }
        }
    }

    m_dayRecords.append(DayRecord(date));
}

void Calendar::deleteDate(int index)
{
    m_dayRecords.remove(index);
}

bool Calendar::clearRecords(const QDate &date1, const QDate &date2)
{
    if (date1 > date2)
        return false;

    int size = m_dayRecords.size();
    int id1 = 0, id2 = 0;
    for (id1 = 0; id1 < size; id1++)
    {
        if (m_dayRecords[id1].m_date >= date1)
        {
            break;
        }
    }
    for (id2 = id1; id2 < size; id2++)
    {
        if (m_dayRecords[id2].m_date > date2)
        {
            break;
        }
    }

    m_dayRecords.remove(id2, size - id2);
    m_dayRecords.remove(0, id1);
    return true;
}

bool Calendar::addRemark(const QDate &date, const QString &remark)
{
    int id = indexOf(date);
    if (id < 0)
    {
        addDate(date);
        id = indexOf(date);
    }

    m_dayRecords[id].m_remarks.append(remark);
    return true;
}

bool Calendar::deleteRemark(const QDate &date, int index)
{
    int id = indexOf(date);
    if (id < 0)
    {
        return false;
    }

    if (index < 0 || index >= m_dayRecords[id].m_remarks.size())
    {
        return false;
    }

    m_dayRecords[id].m_remarks.remove(index);
    return true;
}

bool Calendar::switchRemark(const QDate &date, int index, int offset)
{
    int id = indexOf(date);
    if (id < 0)
    {
        return false;
    }

    if (index < 0 || index >= m_dayRecords[id].m_remarks.size() ||
            index  + offset < 0 || index + offset >= m_dayRecords[id].m_remarks.size())
    {
        return false;
    }

    QString tmp = m_dayRecords[id].m_remarks[index];
    m_dayRecords[id].m_remarks[index] = m_dayRecords[id].m_remarks[index + offset];
    m_dayRecords[id].m_remarks[index + offset] = tmp;
    return true;
}

bool Calendar::addAlarm(const QDate &date, const QTime &time)
{
    int id = indexOf(date);
    if (id < 0)
    {
        addDate(date);
        id = indexOf(date);
    }

    QVector<QTime> &alarms = m_dayRecords[id].m_alarms;
    int index = std::lower_bound(alarms.begin(), alarms.end(), time) - alarms.begin();
    if (index < alarms.size() && alarms[index] == time)
    {
        return false;
    }
    alarms.insert(index, time);
    return true;
}

bool Calendar::deleteAlarm(const QDate &date, int index)
{
    int id = indexOf(date);
    if (id < 0)
    {
        return false;
    }

    if (index < 0 || index >= m_dayRecords[id].m_alarms.size())
    {
        return false;
    }

    m_dayRecords[id].m_alarms.remove(index);
    return true;
}

const QString &Calendar::memo() const
{
    return m_memo;
}

void Calendar::setMemo(const QString &newMemo)
{
    m_memo = newMemo;
}
