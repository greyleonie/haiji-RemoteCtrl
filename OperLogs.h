#ifndef OPERLOGS_H
#define OPERLOGS_H

#include <QString>
#include <QtSql>
#include <QDateTime>
#include "UserInfo.h"

class COperLogs
{
public:
    COperLogs(CUserInfo *userInfo);
    ~COperLogs();
    void set(QString oper);
private:
    CUserInfo *m_CurrUserInfo;
};

#endif // OPERLOGS_H
