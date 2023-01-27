#ifndef LOCKER_H
#define LOCKER_H

#include <QObject>
#include <QTimer>
#include <windef.h>

struct Window
{
    DWORD tid;
    DWORD pid;
    QString module;
    QString title;
};

class Locker : public QObject
{
    Q_OBJECT
public:
    static BOOL CALLBACK addToWindows(HWND hwnd, LPARAM lparam);
    static BOOL CALLBACK checkWindow(HWND hwnd, LPARAM lparam);

    explicit Locker(QObject *parent = nullptr);
    bool idle();

    void updateWindows();
    void checkWindows();

    void lock(int minutes);
    void end();

private:
    QVector<Window> m_windows;
    QTimer m_timer;
    QTimer m_checkTimer;
    bool m_idle;

signals:
    void timeout(bool);
    void warning();
};

#endif // LOCKER_H
