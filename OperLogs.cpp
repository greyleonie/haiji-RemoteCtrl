#include "OperLogs.h"

COperLogs::COperLogs(CUserInfo *userInfo)
{
    m_CurrUserInfo = userInfo;
}


COperLogs::~COperLogs()
{
    ;
}

void COperLogs::set(QString oper)
{
    int logid;
    QSqlQuery dbQuery;
    dbQuery.exec("SELECT MAX(log_id) FROM t_operlogs;");
    if(dbQuery.next())
        logid = dbQuery.value(0).toInt() + 1;
    else
        logid = 1;

    dbQuery.exec("INSERT INTO t_operlogs VALUES(" +
            QString::number(logid, 10) + ", " +
            QString::number(m_CurrUserInfo->getUserid(), 10) + ", '" +
            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "', '" +
            oper + "');");
}
