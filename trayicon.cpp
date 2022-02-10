#include "trayicon.h"

#include <QMessageBox>

TrayIcon::TrayIcon(QApplication *app, QObject *parent) :
    QSystemTrayIcon(parent),
    m_app(app),
    m_window(nullptr),
    m_settings(this),
    m_timer(this)
{
    // Adding an icon
    setIcon(QIcon(":/images/plus.png"));
    setVisible(true);

    // Initializing settings from file
    m_settings.load();

    // Initializing schedule timer


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
    m_options[2].setIcon(m_settings.shutdown() ? QIcon(":/images/on.png") : QIcon(":/images/off.png"));
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
    connect(this, &TrayIcon::activated, this, &TrayIcon::onActivated);
    connect(this, &TrayIcon::messageClicked, this, &TrayIcon::showMainWindow);
}

TrayIcon::~TrayIcon()
{
    delete m_menu;
    delete[] m_options;

    if (m_window != nullptr)
    {
        delete m_window;
    }
}

void TrayIcon::showMainWindow()
{
    if (m_window == nullptr)
    {
        m_window = new MainWindow(m_app);
        connect(m_window, &MainWindow::closed, this, [ = ]()
        {
            delete m_window;
            m_window = nullptr;
        });
        m_window->show();
        m_window->activateWindow();
        m_window->setFocus();
    }
}

void TrayIcon::fullscreen()
{
    // TODO: REDEFINED
    static int i = 0;
    i = 1 - i;
    setIcon(i ? QIcon(":/images/negative.png") : QIcon(":/images/plus.png"));
}

void TrayIcon::changeShutdown()
{
    m_settings.setShutdown(!m_settings.shutdown());
    m_options[2].setIcon(m_settings.shutdown() ? QIcon(":/images/on.png") : QIcon(":/images/off.png"));
}

void TrayIcon::showAbout()
{
    QMessageBox *msgBox = new QMessageBox;
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(tr("About Daily Planner"));
    msgBox->setWindowIcon(QIcon(":/images/planner.png"));
    msgBox->setIcon(QMessageBox::Information);
    msgBox->setText(tr("<strong>Daily Planner - Arrange your life</strong>"));
    msgBox->setInformativeText(tr("Version 0.0 (Original Version)<br/>Developed by <a href='https://github.com/ferv3455'>ferv3455</a>"));
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setModal(false);
    msgBox->open();
}

void TrayIcon::quit()
{
    // Pre-exit savings
    m_settings.save();

    // Quitting application
    emit programQuitted();
}

void TrayIcon::onActivated(ActivationReason reason)
{
    if (reason == Trigger)
    {
        // Left button
        showMainWindow();
    }
}

