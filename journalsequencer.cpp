#include "journalsequencer.h"

JournalSequencer::JournalSequencer(QObject *parent)
    : QObject{parent} {}

void JournalSequencer::setJournal(QString journalFile)
{
    if (m_journalFile.isOpen())
        m_journalFile.close();
    m_journalFile.setFileName(journalFile);
    m_journalFile.open(QIODeviceBase::ReadOnly);
    QFileInfo info(journalFile);
    info.absoluteDir().mkdir(info.absoluteDir().path()
                             + QDir::separator() + "runtime");
    m_journalFileLive.setFileName(info.absoluteDir().path()
                                  + QDir::separator() + "runtime"
                                  + QDir::separator() + info.fileName());
    if (m_journalFileLive.exists())
        m_journalFileLive.remove();
    m_journalFileLive.open(QIODeviceBase::WriteOnly);
    m_line = m_journalFile.readLine();
    m_journalFileLive.write(m_line);
    QJsonParseError jerror;
    m_jdoc = QJsonDocument::fromJson(m_line, &jerror);

    m_nextEvent = m_jdoc.object();
    m_eventTime = QDateTime::fromString(m_nextEvent.value("timestamp").toString(), Qt::ISODate);
    emit newEvent(m_nextEvent, m_eventTime);
    forwardToEvent("Commander");
    m_playerName = m_nextEvent.value("Name").toString();
    emit playerNameChanged(m_playerName);
}

void JournalSequencer::nextEvent()
{
    m_journalFileLive.write(m_line);
    m_line = m_journalFile.readLine();
    m_jdoc = QJsonDocument::fromJson(m_line);
    m_nextEvent = m_jdoc.object();
    m_eventTime = QDateTime::fromString(m_nextEvent.value("timestamp").toString(), Qt::ISODate);
    emit newEvent(m_nextEvent, m_eventTime);
    emit logThat(m_playerName + " next event is " + m_line);
}

void JournalSequencer::forwardToDate(const QDateTime &dateTime)
{
    while (m_journalFile.bytesAvailable() > 0 &&
           m_eventTime < dateTime)
        nextEvent();
}

void JournalSequencer::forwardToEvent(const QString &eventName)
{
    while (m_journalFile.bytesAvailable() > 0 &&
               m_nextEvent.value("event").toString() != eventName)
        nextEvent();
}

void JournalSequencer::restart()
{
    setJournal(m_journalFile.fileName());
}
