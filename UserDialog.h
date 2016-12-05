#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QListWidget>
#include <QList>
#include "UserInfo.h"
#include "ui_UserDialog.h"
#include "Logs.h"
#include "OperLogs.h"

namespace Ui
{
    class Dialog_user;
}

class CUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CUserDialog(QWidget *parent = 0);
    ~CUserDialog();

public slots:
    void AddUser();
    void DelUser();
    void Save();
    void SelectItem(QListWidgetItem *item);
    void EditUsername();
    void EditPassword();
    void SelectRole(int);
    void ConfirmPassword();

private:
    Ui::Dialog_user *ui;
    QList<CUserInfo> m_liUserList;
    int m_iMaxUserid;
};



#endif // USERDIALOG_H
