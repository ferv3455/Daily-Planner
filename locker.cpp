#include "locker.h"

#include <winuser.h>
#include <processthreadsapi.h>
#include <psapi.h>
#include <QDebug>


BOOL Locker::addToWindows(HWND hwnd, LPARAM lparam)
{
    if (IsWindowVisible(hwnd))
    {
        TCHAR buf[1024];
        UINT len = GetWindowText(hwnd, buf, 256);
        if (len > 0)
        {
            Window w;
            w.title = QString::fromWCharArray(buf);
            w.tid = GetWindowThreadProcessId(hwnd, &w.pid);
            HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, 0, w.pid);
            GetModuleFileNameEx(handle, 0, buf, 256);
            w.module = QString::fromWCharArray(buf);

            Locker *locker = (Locker *)lparam;
            locker->m_windows.push_back(w);
        }
    }

    return TRUE;
}

WINBOOL Locker::checkWindow(HWND hwnd, LPARAM lparam)
{
    if (IsWindowVisible(hwnd))
    {
        TCHAR buf[1024];
        UINT len = GetWindowText(hwnd, buf, 256);
        if (len > 0)
        {
            DWORD pid;
            GetWindowThreadProcessId(hwnd, &pid);
            HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
            GetModuleFileNameEx(handle, 0, buf, 256);
            QString module = QString::fromWCharArray(buf);

            // Check module name
            Locker *locker = (Locker *)lparam;
            int found = 0;
            for (const Window &w : qAsConst(locker->m_windows))
            {
                if (w.module == module)
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

Locker::Locker(QObject *parent) :
    QObject(parent),
    m_windows(),
    m_timer(this),
    m_checkTimer(this)
{
    updateWindows();

    // Set timers and their signals
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &Locker::end);
    connect(&m_checkTimer, &QTimer::timeout, this, &Locker::checkWindows);
}

void Locker::updateWindows()
{
    m_windows.clear();
    EnumWindows(Locker::addToWindows, (LPARAM)this);
}

void Locker::checkWindows()
{
    bool result = EnumWindows(Locker::checkWindow, (LPARAM)this);
    if (!result)
    {
        emit warning();
    }
}

void Locker::lock(int minutes)
{
    updateWindows();
    m_timer.start(minutes * 1000 * 60);
    m_checkTimer.start(5000);
    m_idle = false;
    emit timeout(true);
}

void Locker::end()
{
    m_checkTimer.stop();
    m_idle = true;
    emit timeout(false);
}

bool Locker::idle()
{
    return m_idle;
}
