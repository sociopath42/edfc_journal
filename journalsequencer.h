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
public:
    QDateTime nextEventTime() { return m_eventTime; }
    QJsonObject nextEventObject() { return m_nextEvent; }

public slots:
    void setJournal(QString journalFile);
    void nextEvent(); // will process event at 'nextEventTime'
    void forwardToDate(const QDateTime &dateTime); // move to time right before 'dateTime'
    void restart();

signals:
    void newEvent(const QJsonObject &event, const QDateTime &eventDateTime);
};

#endif // JOURNALSEQUENCER_H
