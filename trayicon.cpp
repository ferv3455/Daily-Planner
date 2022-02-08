#include "trayicon.h"

TrayIcon::TrayIcon(QApplication *app, QObject *parent) :
    QSystemTrayIcon(parent),
    m_app(app),
    m_window(nullptr)
{
    // Adding an icon
    setIcon(QIcon(":/images/plus.png"));
    setVisible(true);

    // Creating the menu
    m_menu = new QMenu;
    m_options = new QAction[3];
    for (int i = 0; i < 3; i++)
    {
        m_menu->addAction(m_options + i);
    }
    setContextMenu(m_menu);

    // Setting the options
    m_options[0].setText("Show MainWindow");
    connect(&m_options[0], &QAction::triggered, this, &TrayIcon::showMainWindow);
    m_options[1].setText("Option 2");
    m_options[2].setText("Quit");
    connect(&m_options[2], &QAction::triggered, m_app, &QApplication::quit);

    // Setting Other Message
    setToolTip("Haha");

    // Connect Signals and Slots
    connect(this, &TrayIcon::activated, this, &TrayIcon::onActivated);
    connect(this, &TrayIcon::messageClicked, this, &TrayIcon::onMessageClicked);
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
    }
}

void TrayIcon::onActivated(ActivationReason reason)
{
    if (reason == Trigger)
    {
        m_app->quit();
    }
}

void TrayIcon::onMessageClicked()
{
    m_app->quit();
}
