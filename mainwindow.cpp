
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStandardPaths>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_settings{"sociosoft", "edfc_journal"}
{
    ui->setupUi(this);
    m_playTimer.setInterval(ui->playInterval_dspin->value() * 1000);
    m_playTimer.setSingleShot(false);
    QObject::connect(&m_playTimer, &QTimer::timeout,
                     this, &MainWindow::on_stepNextEvent_pb_clicked);

    QObject::connect(&m_player_1, &JournalSequencer::playerNameChanged,
                     this->ui->playerName1_lbl, &QLabel::setText);
    QObject::connect(&m_player_2, &JournalSequencer::playerNameChanged,
                     this->ui->playerName2_lbl, &QLabel::setText);
    QObject::connect(&m_player_3, &JournalSequencer::playerNameChanged,
                     this->ui->playerName3_lbl, &QLabel::setText);
    QObject::connect(&m_player_4, &JournalSequencer::playerNameChanged,
                     this->ui->playerName4_lbl, &QLabel::setText);

    QObject::connect(&m_player_1, &JournalSequencer::logThat,
                     this->ui->log, &QTextEdit::append);
    QObject::connect(&m_player_2, &JournalSequencer::logThat,
                     this->ui->log, &QTextEdit::append);
    QObject::connect(&m_player_3, &JournalSequencer::logThat,
                     this->ui->log, &QTextEdit::append);
    QObject::connect(&m_player_4, &JournalSequencer::logThat,
                     this->ui->log, &QTextEdit::append);

    QObject::connect(&m_player_1, &JournalSequencer::newEvent,
                     this, [this](const QJsonObject &event, const QDateTime &eventDateTime) {
                         this->ui->currentDate1_lbl->setText(event.value("timestamp").toString());
                     });
    QObject::connect(&m_player_2, &JournalSequencer::newEvent,
                     this, [this](const QJsonObject &event, const QDateTime &eventDateTime) {
                         this->ui->currentDate2_lbl->setText(event.value("timestamp").toString());
                     });
    QObject::connect(&m_player_3, &JournalSequencer::newEvent,
                     this, [this](const QJsonObject &event, const QDateTime &eventDateTime) {
                         this->ui->currentDate3_lbl->setText(event.value("timestamp").toString());
                     });
    QObject::connect(&m_player_4, &JournalSequencer::newEvent,
                     this, [this](const QJsonObject &event, const QDateTime &eventDateTime) {
                         this->ui->currentDate4_lbl->setText(event.value("timestamp").toString());
                     });




}

QJsonObject MainWindow::on_stepNextEvent_pb_clicked()
{
    JournalSequencer *nextPlayerEventSource = nullptr;
    if (m_player_1.isRunning() &&
        (m_player_1.nextEventTime() != QDateTime()))
        nextPlayerEventSource = &m_player_1;
    if (m_player_2.isRunning() &&
        (m_player_2.nextEventTime() < nextPlayerEventSource->nextEventTime()))
        nextPlayerEventSource = &m_player_2;
    if (m_player_3.isRunning() &&
        (m_player_3.nextEventTime() < nextPlayerEventSource->nextEventTime()))
        nextPlayerEventSource = &m_player_3;
    if (m_player_4.isRunning() &&
        (m_player_4.nextEventTime() < nextPlayerEventSource->nextEventTime()))
        nextPlayerEventSource = &m_player_4;

    if (nextPlayerEventSource)
    {
        nextPlayerEventSource->nextEvent();
        return nextPlayerEventSource->nextEventObject();
    }
    return QJsonObject();
}

QJsonObject MainWindow::on_stepToNext_pb_clicked()
{
    QJsonObject nextEventObj = on_stepNextEvent_pb_clicked();
    while (nextEventObj.value("event").toString() != ui->eventName_cbx->currentText())
    {
        if (nextEventObj.isEmpty())
        {
            return nextEventObj;
        }
        nextEventObj = on_stepNextEvent_pb_clicked();
    }
    return nextEventObj;
}

void MainWindow::on_syncJournals_pb_clicked()
{
    bool everyonesOk = false;
    bool p1ok = !m_player_1.isRunning();
    bool p2ok = !m_player_2.isRunning();
    bool p3ok = !m_player_3.isRunning();
    bool p4ok = !m_player_4.isRunning();
    while (!everyonesOk)
    {
        JournalSequencer *nextPlayerEventSource = nullptr;
        if (m_player_1.isRunning() &&
            (m_player_1.nextEventTime() != QDateTime()))
        {
            nextPlayerEventSource = &m_player_1;
            p1ok = true;
        }
        if (m_player_2.isRunning() &&
            (m_player_2.nextEventTime() > nextPlayerEventSource->nextEventTime()))
        {
            nextPlayerEventSource = &m_player_2;
            p2ok = true;
        }
        if (m_player_3.isRunning() &&
            (m_player_3.nextEventTime() > nextPlayerEventSource->nextEventTime()))
        {
            nextPlayerEventSource = &m_player_3;
            p3ok = true;
        }
        if (m_player_4.isRunning() &&
            (m_player_4.nextEventTime() > nextPlayerEventSource->nextEventTime()))
        {
            nextPlayerEventSource = &m_player_4;
            p4ok = true;
        }

        if (nextPlayerEventSource)
        {
            nextPlayerEventSource->nextEvent();
        }
        everyonesOk = p1ok && p2ok && p3ok && p4ok;
    }
}

void MainWindow::on_playSession_pb_clicked()
{
    m_playTimer.start();
}

void MainWindow::on_pauseSession_pb_clicked()
{
    m_playTimer.stop();
}

void MainWindow::on_restartJournals_pb_clicked()
{
    m_player_1.restart();
    m_player_2.restart();
    m_player_3.restart();
    m_player_4.restart();
}

void MainWindow::loadPlayerJournal(JournalSequencer &pj, QString settingEntry)
{
    qDebug() << "load" << settingEntry;
    QString prev_journalPath = QDir::homePath();
    if (m_settings.contains(settingEntry))
    {
        prev_journalPath = m_settings.value(settingEntry).toString();
        qDebug() << "found previous path" << prev_journalPath;
    }
    QStringList filePath = QFileDialog::getOpenFileNames(this, tr("Open journal file"),
                                                         prev_journalPath,
                                                         "ED journal file (*.log)");
    if (filePath.count())
    {
        qDebug() << "loading file" << filePath.first();
        QFileInfo info(filePath.first());
        m_settings.setValue(settingEntry, info.absolutePath());
        pj.setJournal(filePath.first());
        m_settings.sync();
    }
    else
        qDebug() << "no file";
}

void MainWindow::on_loadJournal1_pb_clicked()
{
    this->loadPlayerJournal(m_player_1, "p1_journal_path");
}

void MainWindow::on_loadJournal4_pb_clicked()
{
    this->loadPlayerJournal(m_player_2, "p2_journal_path");
}

void MainWindow::on_loadJournal3_pb_clicked()
{
    this->loadPlayerJournal(m_player_3, "p3_journal_path");
}

void MainWindow::on_loadJournal2_pb_clicked()
{
    this->loadPlayerJournal(m_player_4, "p4_journal_path");
}

void MainWindow::on_playInterval_dspin_valueChanged(double arg1)
{
    m_playTimer.setInterval(arg1 * 1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent  *)
{
    QApplication::exit(0);
};
