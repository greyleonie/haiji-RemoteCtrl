#include "Logs.h"

CLogs::CLogs(QString fileName)
{
    m_LogFile = new QFile(fileName);
    if(!m_LogFile->open(QIODevice::ReadWrite | QIODevice::Text))
        return;
}

CLogs::~CLogs()
{
    if(m_LogFile != NULL)
    {
        m_LogFile->close();
        delete m_LogFile;
    }
}

void CLogs::set(QString log)
{
    QTextStream stream(m_LogFile);
    stream.seek(m_LogFile->size());
    stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << log << endl;
}
