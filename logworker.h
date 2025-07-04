#ifndef LOGWORKER_H
#define LOGWORKER_H

#include <QQueue>           //Queue에 넣은 데이터 빼서 작업
#include <QMutex>           //배타적인 작업
#include <QThread>          //Thread
#include <QString>          //string
#include <QWaitCondition>
#include <QFile>            //저장용
#include <QTextStream>      //>

class LogWorker:public QThread
{
    Q_OBJECT;
public:
    LogWorker(const QString& roomName, QObject* parent=nullptr);
    void enqueueLog(const QString& logLine);
    void stop();

private:
    void run() override;
    QString m_logFileName;
    QQueue<QString> m_logQueue; //로그에 넣을 데이터 저장용 queue
    QMutex m_mutex;             //mutex 자동 초기화
    QWaitCondition m_notifier;  //
    bool m_running;
};

#endif // LOGWORKER_H
