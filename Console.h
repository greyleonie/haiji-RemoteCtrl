#ifndef CONSOLE_H
#define CONSOLE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QMessageBox>
#include <QTextCodec>
#include "LoginDialog.h"
#include "MonitorWindow.h"
#include "Logs.h"

class CConsole : public QWidget
{

public:
    explicit CConsole(QWidget *parent = 0);
    ~CConsole();
    int exec();

private:
    QSqlDatabase m_db;
};

#endif // CONSOLE_H
