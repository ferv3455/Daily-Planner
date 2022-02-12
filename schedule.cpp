#include "schedule.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Schedule::Schedule(QObject *parent) :
    QObject(parent),
    m_beginTime(),
    m_endTime(),
    m_count(0)
{

}

void Schedule::load()
{
    // Trying to open file settings.json
    QFile file("./save/schedule.json");
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
    if (json.contains("count") && json["count"].isDouble())
    {
        m_count = json["count"].toInt();
    }

    if (json.contains("begin_time") && json["begin_time"].isArray())
    {
        QJsonArray beginArray = json["begin_time"].toArray();
        m_beginTime.clear();
        m_beginTime.reserve(beginArray.size());
        for (int i = 0; i < beginArray.size(); i++)
        {
            QJsonObject timeObject = beginArray[i].toObject();
            m_beginTime.append({timeObject["h"].toInt(),
                                timeObject["m"].toInt(),
                                timeObject["s"].toInt()});
        }
    }

    if (json.contains("end_time") && json["end_time"].isArray())
    {
        QJsonArray endArray = json["end_time"].toArray();
        m_endTime.clear();
        m_endTime.reserve(endArray.size());
        for (int i = 0; i < endArray.size(); i++)
        {
            QJsonObject timeObject = endArray[i].toObject();
            m_endTime.append({timeObject["h"].toInt(),
                              timeObject["m"].toInt(),
                              timeObject["s"].toInt()});
        }
    }
}

void Schedule::save() const
{
    // Trying to open file settings.json
    QFile file("./save/schedule.json");
    if (!file.open(QIODevice::WriteOnly))
    {
        // Couldn't open file: skip
        qDebug() << "Couldn't open save file.";
        return;
    }

    // Contructing a json object from current settings
    QJsonObject json;
    json["count"] = m_count;

    QJsonArray beginTime;
    for (const QTime &t : m_beginTime)
    {
        QJsonObject timeObject;
        timeObject["h"] = t.hour();
        timeObject["m"] = t.minute();
        timeObject["s"] = t.second();
        beginTime.append(timeObject);
    }
    json["begin_time"] = beginTime;

    QJsonArray endTime;
    for (const QTime &t : m_endTime)
    {
        QJsonObject timeObject;
        timeObject["h"] = t.hour();
        timeObject["m"] = t.minute();
        timeObject["s"] = t.second();
        endTime.append(timeObject);
    }
    json["end_time"] = endTime;

    // Saving the object into the file
    QJsonDocument saveDoc(json);
    file.write(saveDoc.toJson());
}

const QVector<QTime> &Schedule::beginTime()
{
    return m_beginTime;
}

const QVector<QTime> &Schedule::endTime()
{
    return m_endTime;
}

int Schedule::size()
{
    return m_count;
}

bool Schedule::add(const QTime &begin, const QTime &end)
{
    int id1 = std::upper_bound(m_beginTime.begin(), m_beginTime.end(), begin) - m_beginTime.begin();
    int id2 = std::upper_bound(m_endTime.begin(), m_endTime.end(), end) - m_endTime.begin();
    if (id1 != id2 || begin >= end || (id2 > 0 && begin <= m_endTime[id2 - 1]) ||
            (id1 < m_count && end >= m_beginTime[id1]))
    {
        return false;
    }

    m_count++;
    m_beginTime.insert(id1, begin);
    m_endTime.insert(id2, end);
    return true;
}

bool Schedule::remove(int index)
{
    if (index < 0 || index >= m_count)
    {
        return false;
    }

    m_count--;
    m_beginTime.remove(index);
    m_endTime.remove(index);
    return true;
}


