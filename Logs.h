#ifndef LOGS_H
#define LOGS_H

#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>

class CLogs
{
    public:
        CLogs(QString fileName = "./logs.txt");
        ~CLogs();
        void set(QString log);
    private:
        QFile *m_LogFile;
};

#endif // LOGS_H
