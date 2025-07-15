#include "logworker.h"
#include <QDateTime>

LogWorker::LogWorker(const QString &roomName, QObject *parent)
    : QThread(parent)
    , m_logFileName(QDateTime::currentDateTime().toString("MMdd_HHMMss_")+roomName + ".log")
    , m_running(true)
{}
//mutex 가지고 있어야지만 logworker의 동작 변경 가능.
void LogWorker::enqueueLog(const QString &logLine)
{
    QMutexLocker locker(&m_mutex);
    m_logQueue.enqueue(logLine);
    m_notifier.wakeOne(); //한줄 들어왓으니까 하나 깨움.
}

void LogWorker::stop()
{
    QMutexLocker locker(&m_mutex);
    m_running = false;
    m_notifier.wakeAll(); //모두 깨움
}

void LogWorker::run()
{
    while (true) {
        QString logLine;

        QMutexLocker locker(&m_mutex);
        if (m_logQueue.isEmpty()) {
            if (!m_running)
                break; //log stop에서 깨워졌으면 종료
            m_notifier.wait(&m_mutex);
            continue;
        }
        //한줄 뽑아온거 쓰기
        logLine = m_logQueue.dequeue();
        //파일
        QFile file(m_logFileName);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            //일단 \n도 껴서 넣었는데 안들어가도 될수도 있음 아직모름
            out << logLine << "\n";
        }
    }
}
