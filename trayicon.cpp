#include "trayicon.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QtMultimedia/QMediaPlayer>

TrayIcon::TrayIcon(QObject *parent) :
    QSystemTrayIcon(parent),
    m_window(nullptr)
{
    // Adding an icon
    setIcon(QIcon(":/images/planner.png"));
    setVisible(true);

    // Initializing settings from file
    m_settings = new Settings(this);
    m_settings->load();

    // Initializing schedule timer
    m_timer = new ScheduleTimer(m_settings, this);
    m_timer->loadSchedule();

    // Initializing locker
    m_locker = new Locker(this);

    // Creating the menu
    m_menu = new QMenu;
    m_options = new QAction[5];
    m_menu->addAction(&m_options[0]);
    m_menu->addSeparator();
    m_menu->addAction(&m_options[1]);
    m_menu->addAction(&m_options[2]);
    m_menu->addSeparator();
    m_menu->addAction(&m_options[3]);
    m_menu->addSeparator();
    m_menu->addAction(&m_options[4]);
    setContextMenu(m_menu);

    // Setting the options
    m_options[0].setText(tr("Show MainWindow"));
    m_options[0].setIcon(QIcon(":/images/window.png"));
    connect(&m_options[0], &QAction::triggered, this, &TrayIcon::showMainWindow);

    m_options[1].setText(tr("Screen Lock"));
    m_options[1].setIcon(QIcon(":/images/lock.png"));
    connect(&m_options[1], &QAction::triggered, this, &TrayIcon::fullscreen);

    m_options[2].setText(tr("Auto Shutdown"));
    m_options[2].setIcon(m_settings->shutdown() ? QIcon(":/images/on.png") : QIcon(":/images/off.png"));
    connect(&m_options[2], &QAction::triggered, this, &TrayIcon::changeShutdown);

    m_options[3].setText(tr("About"));
    m_options[3].setIcon(QIcon(":/images/about.png"));
    connect(&m_options[3], &QAction::triggered, this, &TrayIcon::showAbout);

    m_options[4].setText(tr("Quit"));
    m_options[4].setIcon(QIcon(":/images/quit.png"));
    connect(&m_options[4], &QAction::triggered, this, &TrayIcon::quit);

    // Setting Other Message
    setToolTip(tr("Daily Planner"));

    // Connect Signals and Slots
    connect(m_timer, &ScheduleTimer::alarmClock, this, &TrayIcon::alarmMessage);
    connect(m_timer, &ScheduleTimer::stateChanged, this, &TrayIcon::changeState);
    connect(m_locker, &Locker::warning, this, &TrayIcon::warnLocker);
    connect(m_locker, &Locker::timeout, this, &TrayIcon::lockerState);
    connect(this, &TrayIcon::activated, this, &TrayIcon::onActivated);
}

TrayIcon::~TrayIcon()
{
    delete m_settings;
    delete m_timer;
    delete m_menu;
    delete[] m_options;

    if (m_window != nullptr)
    {
        delete m_window;
    }
    if (m_locker != nullptr)
    {
        delete m_locker;
    }
}

void TrayIcon::alarmMessage(const QTime &time)
{
    showMessage(tr("Alarm Clock"), time.toString());
}

void TrayIcon::changeState(ScheduleTimer::state s, int taskID, int num)
{
    if (s == ScheduleTimer::TASK)
    {
        showMessage(tr("Task Begins"), tr("Rest is over.\nPeriod number %1/%2.").arg(taskID).arg(num));
        setIcon(QIcon(":/images/task.png"));
        setToolTip(tr("Daily Planner") + "\n" +
                   tr("Current state: ") + tr("TASK") + "\n" +
                   tr("End time: ") + m_timer->endTime());
        QMediaPlayer *player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile("./sounds/percussion.mp3"));
        player->setVolume(80);
        player->play();
    }
    else if (s == ScheduleTimer::REST)
    {
        showMessage(tr("Task Ends"), tr("This period is over.\nHave a rest."));
        setIcon(QIcon(":/images/rest.png"));
        setToolTip(tr("Daily Planner") + "\n" +
                   tr("Current state: ") + tr("REST") + "\n" +
                   tr("End time: ") + m_timer->endTime());
        QMediaPlayer *player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile("./sounds/percussion.mp3"));
        player->setVolume(80);
        player->play();
    }
    else if (s == ScheduleTimer::OVER)
    {
        showMessage(tr("Task Ends"), tr("Today's work is over.\nHave a rest."));
        setIcon(QIcon(":/images/rest.png"));
        setToolTip(tr("Daily Planner") + "\n" +
                   tr("Current state: ") + tr("ALL TASKS OVER") + "\n");
        QMediaPlayer *player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile("./sounds/percussion.mp3"));
        player->setVolume(80);
        player->play();
    }
}

void TrayIcon::warnLocker()
{
    showMessage(tr("Locker warning"), tr("Unknown application detected. Please shut it down as soon as possible."));
}

void TrayIcon::lockerState(bool state)
{
    if (state)
    {
        showMessage(tr("Locker begins"), tr("Locker begins. Please get to work."));
    }
    else
    {
        showMessage(tr("Locker ends"), tr("Locker ends. Please have a rest."));
    }
}

void TrayIcon::showMainWindow()
{
    if (m_window == nullptr)
    {
        m_window = new MainWindow;
        connect(m_window, &MainWindow::closed, this, [ = ]()
        {
            delete m_window;
            m_window = nullptr;
            m_timer->loadSchedule();
        });
        m_window->show();
//        m_window->showMaximized();
        m_window->activateWindow();
        m_window->setFocus();
    }
}

void TrayIcon::fullscreen()
{
    // TODO: REDEFINED
    if (m_locker->idle())
    {
        m_locker->updateWindows();

        bool ok;
        int minutes = QInputDialog::getInt(nullptr, tr("Locker begins"),
                                           tr("Enter locking time (minutes):"), 40, 0, 120, 1, &ok);
        m_locker->lock(minutes);
    }
    else
    {
        QMessageBox::warning(nullptr, tr("Warning"), tr("Locker already running!"));
    }
}

void TrayIcon::changeShutdown()
{
    m_settings->setShutdown(!m_settings->shutdown());
    m_options[2].setIcon(m_settings->shutdown() ? QIcon(":/images/on.png") : QIcon(":/images/off.png"));
}

void TrayIcon::showAbout()
{
    QMessageBox *msgBox = new QMessageBox;
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(tr("About Daily Planner"));
    msgBox->setWindowIcon(QIcon(":/images/planner.png"));
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setText(tr("<strong>Daily Planner - Arrange your life</strong>"));
    msgBox->setInformativeText(tr("Version ") + "1.2 (Lite)" + tr("<br/>Developed by <a href='https://github.com/ferv3455'>ferv3455</a>"));
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setModal(false);
    msgBox->open();
}

void TrayIcon::quit()
{
    // Warning with a messagebox
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Exit Daily Planner"));
    msgBox.setWindowIcon(QIcon(":/images/planner.png"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(tr("<strong>Are you sure you want to quit?</strong>"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    int ret = msgBox.exec();
    if (ret == QMessageBox::No)
    {
        return;
    }

    // Pre-exit savings
    m_settings->save();

    // Quitting application
    emit programQuitted();
}

void TrayIcon::onActivated(ActivationReason reason)
{
    if (reason == DoubleClick)
    {
        // Left button
        showMainWindow();
    }
}

