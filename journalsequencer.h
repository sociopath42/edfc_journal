#ifndef JOURNALSEQUENCER_H
#define JOURNALSEQUENCER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

class JournalSequencer : public QObject
{
    Q_OBJECT
public:
    explicit JournalSequencer(QObject *parent = nullptr);

private:
    QFile m_journalFile;
    QFile m_journalFileLive;
    QByteArray m_line;
    QDateTime m_eventTime;
    QJsonObject m_nextEvent;
    QJsonDocument m_jdoc;
    QString m_playerName;
public:
    QDateTime nextEventTime() { if (m_journalFile.isOpen()) return m_eventTime;
                                else                        return QDateTime(); }
    QJsonObject nextEventObject() { if (m_journalFile.isOpen()) return m_nextEvent;
                                    else                        return QJsonObject(); }
    bool isRunning() { return !!m_journalFile.bytesAvailable(); }
public slots:
    void setJournal(QString journalFile);
    void nextEvent(); // will process event at 'nextEventTime'
    void forwardToDate(const QDateTime &dateTime); // move to time right before 'dateTime'
    void forwardToEvent(const QString &event); // move to time right before 'dateTime'
    void restart();

signals:
    void newEvent(const QJsonObject &event, const QDateTime &eventDateTime);
    void playerNameChanged(const QString &playerName);
    void logThat(QString logLine);
};

#endif // JOURNALSEQUENCER_H
