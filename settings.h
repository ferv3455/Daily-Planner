#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings(QObject *parent = nullptr, bool shutdown = true);
    ~Settings();

    // Load and save with files
    void load();
    void save() const;

    // Getters and setters
    bool shutdown() const;
    void setShutdown(bool newShutdown);

private:
    bool m_shutdown;

signals:

};

#endif // SETTINGS_H
