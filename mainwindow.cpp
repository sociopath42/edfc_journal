
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStandardPaths>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_playTimer.setInterval(ui->playInterval_dspin->value() * 1000);
    m_playTimer.setSingleShot(false);
    QObject::connect(&m_playTimer, &QTimer::timeout,
                     this, &MainWindow::on_stepNextEvent_pb_clicked);
}

QJsonObject MainWindow::on_stepNextEvent_pb_clicked()
{
    JournalSequencer *nextPlayerEventSource = nullptr;
    if (m_player_1.isRunning() &&
        (m_player_1.nextEventTime() != QDateTime()))
        nextPlayerEventSource = &m_player_1;
    if (m_player_2.isRunning() &&
        (m_player_2.nextEventTime() > nextPlayerEventSource->nextEventTime()))
        nextPlayerEventSource = &m_player_2;
    if (m_player_3.isRunning() &&
     (m_player_3.nextEventTime() > nextPlayerEventSource->nextEventTime()))
        nextPlayerEventSource = &m_player_3;
    if (m_player_4.isRunning() &&
     (m_player_4.nextEventTime() > nextPlayerEventSource->nextEventTime()))
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
            return nextEventObj;
        nextEventObj = on_stepNextEvent_pb_clicked();
    }
    return nextEventObj;
}


void MainWindow::on_syncJournals_pb_clicked()
{
 // todo later
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

void MainWindow::on_loadJournal1_pb_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Open journal file"),
                                                    QDir::homePath(),
                                                    "ED journal file (*.log)");
    m_player_1.setJournal(filePath);
}

void MainWindow::on_loadJournal4_pb_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Open journal file"),
                                                    QDir::homePath(),
                                                    "ED journal file (*.log)");
    m_player_2.setJournal(filePath);
}

void MainWindow::on_loadJournal3_pb_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Open journal file"),
                                                    QDir::homePath(),
                                                    "ED journal file (*.log)");
    m_player_3.setJournal(filePath);
}

void MainWindow::on_loadJournal2_pb_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Open journal file"),
                                                    QDir::homePath(),
                                                    "ED journal file (*.log)");
    m_player_4.setJournal(filePath);
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
