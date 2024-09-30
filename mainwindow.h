
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QLayout>

#include "journalsequencer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    JournalSequencer m_player_1;
    JournalSequencer m_player_2;
    JournalSequencer m_player_3;
    JournalSequencer m_player_4;
    QTimer m_playTimer;

protected:
    void closeEvent(QCloseEvent  *);
private slots:
    void on_loadJournal1_pb_clicked();
    void on_loadJournal4_pb_clicked();
    void on_loadJournal3_pb_clicked();
    void on_loadJournal2_pb_clicked();
    void on_stepNextEvent_pb_clicked();
    void on_playSession_pb_clicked();
    void on_pauseSession_pb_clicked();
    void on_restartJournals_pb_clicked();
    void on_syncJournals_pb_clicked();
    void on_playInterval_dspin_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
