#include "settings.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

Settings::Settings(QObject *parent, bool shutdown) :
    QObject(parent),
    m_shutdown(shutdown)
{

}

Settings::~Settings()
{

}

void Settings::load()
{
    // Trying to open file settings.json
    QFile file("./save/settings.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        // Couldn't open file: use default settings
        qDebug() << "Couldn't open save file.";
        return;
    }

    // Reading the file into a json object
    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject json = loadDoc.object();

    // Initializing settings from json object
    m_shutdown = json["shutdown"].toBool();
}

void Settings::save() const
{
    // Trying to open file settings.json
    QFile file("./save/settings.json");
    if (!file.open(QIODevice::WriteOnly))
    {
        // Couldn't open file: skip
        qDebug() << "Couldn't open save file.";
        return;
    }

    // Contructing a json object from current settings
    QJsonObject json;
    json["shutdown"] = m_shutdown;

    // Saving the object into the file
    QJsonDocument saveDoc(json);
    file.write(saveDoc.toJson());
}

bool Settings::shutdown() const
{
    return m_shutdown;
}

void Settings::setShutdown(bool newShutdown)
{
    m_shutdown = newShutdown;
}
