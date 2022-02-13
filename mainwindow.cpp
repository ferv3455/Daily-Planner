#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_calendar(),
    m_firstDate(QDate::currentDate().addDays(-1)),
    m_currentDay(1),
    m_schedule()
{
    // Initializing ui
    ui->setupUi(this);

    ui->clearDateEdit_1->setDate(QDate::currentDate().addDays(-1));
    ui->clearDateEdit_2->setDate(QDate(2050, 12, 31));

    for (int i = 0; i < 5; i++)
    {
        m_labels.append(new QPushButton("date", ui->mainFrame));
        m_labels[i]->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        ui->mainLayout->addWidget(m_labels[i], 0, i);

        m_alarms.append(new QListWidget(ui->mainFrame));
        m_alarms[i]->setMinimumHeight(60);
        m_alarms[i]->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        ui->mainLayout->addWidget(m_alarms[i], 1, i);

        m_days.append(new QListWidget(ui->mainFrame));
        m_days[i]->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        ui->mainLayout->addWidget(m_days[i], 2, i);
    }

    ui->mainLayout->setRowStretch(1, 1);
    ui->mainLayout->setRowStretch(2, 3);

    // Initializing calendar
    m_calendar.load();
    refreshCalendarVisual();
    refreshCalendar();
    resetRange();
    ui->memoEdit->setPlainText(m_calendar.memo());

    // Initializing schedule
    m_schedule.load();
    refreshSchedule();

    // Connecting signals and slots
    // Calendar
    connect(ui->prevButton, &QPushButton::clicked, this, [ = ]()
    {
        m_firstDate = m_firstDate.addDays(-1);
        m_currentDay = (m_currentDay >= 4 ? 4 : m_currentDay + 1);
        refreshCalendarVisual();
        refreshCalendar();
        resetRange();
    });
    connect(ui->nextButton, &QPushButton::clicked, this, [ = ]()
    {
        m_firstDate = m_firstDate.addDays(1);
        m_currentDay = (m_currentDay > 0 ? m_currentDay - 1 : 0);
        refreshCalendarVisual();
        refreshCalendar();
        resetRange();
    });

    for (int i = 0; i < 5; i++)
    {
        connect(m_labels[i], &QPushButton::clicked, this, [ = ]()
        {
            m_currentDay = i;
            refreshCalendarVisual();
            resetRange();
        });
        connect(m_alarms[i], &QListWidget::itemClicked, this, [ = ](QListWidgetItem * item)
        {
            int row = m_alarms[i]->row(item);
            m_currentDay = i;
            refreshCalendarVisual();
            resetRange();
            ui->alarmSpinBox->setValue(row);
        });
        connect(m_days[i], &QListWidget::itemClicked, this, [ = ](QListWidgetItem * item)
        {
            int row = m_days[i]->row(item);
            m_currentDay = i;
            refreshCalendarVisual();
            resetRange();
            ui->remarkSpinBox->setValue(row);
        });
    }

    connect(ui->addRemarkButton, &QPushButton::clicked, this, [ = ]()
    {
        if (!m_calendar.addRemark(m_firstDate.addDays(m_currentDay), ui->remarkEdit->text()))
            QMessageBox::warning(this, tr("Error"), tr("Addition failed! Check your input."));
        ui->remarkEdit->setText("");
        refreshCalendar();
        resetRange();
    });
    connect(ui->deleteRemarkButton, &QPushButton::clicked, this, [ = ]()
    {
        if (!m_calendar.deleteRemark(m_firstDate.addDays(m_currentDay), ui->remarkSpinBox->value()))
            QMessageBox::warning(this, tr("Error"), tr("Deletion failed! Check your input."));
        refreshCalendar();
        resetRange();
    });
    connect(ui->upRemarkButton, &QPushButton::clicked, this, [ = ]()
    {
        if (!m_calendar.switchRemark(m_firstDate.addDays(m_currentDay), ui->remarkSpinBox->value(), -1))
            QMessageBox::warning(this, tr("Error"), tr("Move failed! Check the index."));
        ui->remarkSpinBox->setValue(ui->remarkSpinBox->value() - 1);
        refreshCalendar();
    });
    connect(ui->downRemarkButton, &QPushButton::clicked, this, [ = ]()
    {
        if (!m_calendar.switchRemark(m_firstDate.addDays(m_currentDay), ui->remarkSpinBox->value(), 1))
            QMessageBox::warning(this, tr("Error"), tr("Move failed! Check the index."));
        ui->remarkSpinBox->setValue(ui->remarkSpinBox->value() + 1);
        refreshCalendar();
    });

    connect(ui->addAlarmButton, &QPushButton::clicked, this, [ = ]()
    {
        if (!m_calendar.addAlarm(m_firstDate.addDays(m_currentDay), ui->alarmTimeEdit->time()))
            QMessageBox::warning(this, tr("Error"), tr("Addition failed! Check your input."));
        ui->alarmTimeEdit->setTime(QTime(0, 0, 0));
        refreshCalendar();
        resetRange();
    });
    connect(ui->deleteAlarmButton, &QPushButton::clicked, this, [ = ]()
    {
        if (!m_calendar.deleteAlarm(m_firstDate.addDays(m_currentDay), ui->alarmSpinBox->value()))
            QMessageBox::warning(this, tr("Error"), tr("Deletion failed! Check your input."));
        refreshCalendar();
        resetRange();
    });

    connect(ui->clearButton, &QPushButton::clicked, this, [ = ]()
    {
        if (m_calendar.clearRecords(ui->clearDateEdit_1->date(), ui->clearDateEdit_2->date()))
            QMessageBox::information(this, tr("Cleared Successfully"), tr("Records out of the given range have been cleared!"));
        else
            QMessageBox::warning(this, tr("Error"), tr("Clearance failed! Check the dates."));
        refreshCalendar();
        resetRange();
    });

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
        m_calendar.setMemo(ui->memoEdit->toPlainText());
        m_calendar.save();
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

void MainWindow::refreshCalendarVisual()
{
    for (int i = 0; i < 5; i++)
    {
        // Stylesheet
        if (i == m_currentDay)
        {
            m_labels[i]->setStyleSheet("font: 10pt 'Microsoft YaHei'; background-color: khaki;");
            m_alarms[i]->setStyleSheet("font: 9pt 'Microsoft YaHei'; background-color: beige;");
            m_days[i]->setStyleSheet("font: 9pt 'Microsoft YaHei'; background-color:  beige;");
        }
        else if (m_firstDate.addDays(i).dayOfWeek() > 5)
        {
            m_labels[i]->setStyleSheet("font: 10pt 'Microsoft YaHei';");
            m_alarms[i]->setStyleSheet("font: 9pt 'Microsoft YaHei'; background-color: lavender;");
            m_days[i]->setStyleSheet("font: 9pt 'Microsoft YaHei'; background-color: lavender;");
        }
        else
        {
            m_labels[i]->setStyleSheet("font: 10pt 'Microsoft YaHei';");
            m_alarms[i]->setStyleSheet("font: 9pt 'Microsoft YaHei';");
            m_days[i]->setStyleSheet("font: 9pt 'Microsoft YaHei';");
        }

        if (m_firstDate.addDays(i) == QDate::currentDate())
        {
            m_labels[i]->setStyleSheet(m_labels[i]->styleSheet() + "font: bold 10pt 'Microsoft YaHei';");
        }
    }
}

void MainWindow::refreshCalendar()
{
    for (int i = 0; i < 5; i++)
    {
        // Content
        m_labels[i]->setText(m_firstDate.addDays(i).toString("MM-dd (ddd)"));
        if (m_firstDate.addDays(i) == QDate::currentDate())
        {
            m_labels[i]->setText(m_labels[i]->text() + tr("(Today)"));
        }
        m_alarms[i]->clear();
        m_days[i]->clear();

        int index = m_calendar.indexOf(m_firstDate.addDays(i));
        if (index >= 0)
        {
            const DayRecord &r = m_calendar.record(index);
            int j = 0;
            for (const QTime &t : r.m_alarms)
            {
                m_alarms[i]->addItem(QString("%1 %2").arg(j++, 2).arg(t.toString()));
            }
            j = 0;
            for (const QString &s : r.m_remarks)
            {
                m_days[i]->addItem(QString("%1  %2").arg(j++, 2).arg(s));
            }
        }
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

void MainWindow::resetRange()
{
    ui->alarmSpinBox->setMaximum(m_alarms[m_currentDay]->count() > 0 ? m_alarms[m_currentDay]->count() - 1 : 0);
    ui->remarkSpinBox->setMaximum(m_days[m_currentDay]->count() > 0 ? m_days[m_currentDay]->count() - 1 : 0);
}

