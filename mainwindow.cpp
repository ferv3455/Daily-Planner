#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_schedule()
{
    // Initializing ui
    ui->setupUi(this);

    // Initializing calendar

    // Initializing schedule
    m_schedule.load();
    refreshSchedule();

    // Connecting signals and slots
    // Calendar

    // Daily schedule
    connect(ui->scheduleListWidget, &QListWidget::currentRowChanged, this, [ = ](int row)
    {
        ui->indexSpinBox->setValue(row);
    });
    connect(ui->deleteTaskButton, &QPushButton::clicked, this, [ = ]()
    {
        if (!m_schedule.remove(ui->indexSpinBox->value()))
            QMessageBox::warning(this, tr("Error"), tr("Index out of range!"));
        refreshSchedule();
    });
    connect(ui->addTaskButton, &QPushButton::clicked, this, [ = ]()
    {
        if (!m_schedule.add(ui->beginTimeEdit->time(), ui->endTimeEdit->time()))
            QMessageBox::warning(this, tr("Error"), tr("Time not valid!"));
        refreshSchedule();
    });

    // Save and exit
    connect(ui->saveButton, &QPushButton::clicked, this, [ = ]()
    {
        m_schedule.save();
        QMessageBox::information(this, tr("Saved Successfully"), tr("The calendar and the daily schedule have been saved!"));
    });
    connect(ui->cancelButton, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::question(this, tr("Quit MainWindow"),
                                    tr("Are you sure necessary changes have been saved before quitting?"),
                                    QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
                                    QMessageBox::StandardButton(QMessageBox::No));
    if (ret == QMessageBox::Yes)
    {
        emit closed();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::refreshSchedule()
{
    int length = m_schedule.size();
    const QVector<QTime> &begin = m_schedule.beginTime();
    const QVector<QTime> &end = m_schedule.endTime();

    ui->scheduleListWidget->clear();
    for (int i = 0; i < length; i++)
    {
        ui->scheduleListWidget->addItem(
            QString("%1  %2 - %3").arg(i, 3).arg(begin[i].toString(), end[i].toString()));
    }

    ui->indexSpinBox->setMaximum(length - 1);
}

