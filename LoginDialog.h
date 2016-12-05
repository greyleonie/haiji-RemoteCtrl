#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QtGui>
#include <QtCore>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include "UserInfo.h"
//#include "ui_LoginDialog.h"
#include "Logs.h"

namespace Ui
{
    class Dialog_login;
}

class CLoginDialog : public QDialog
{
    Q_OBJECT

public:
    CLoginDialog(QWidget *parent = 0);
    ~CLoginDialog();
    CUserInfo *getUserInfo();

public slots:
    void UserNameChanged();
    void CheckLogin();

private:
    QGroupBox   *m_LoginGroup;
    QLabel  *m_UsernameLabel;
    QLabel  *m_PasswordLabel;
    QLineEdit   *m_UsernameEdit;
    QLineEdit   *m_PasswordEdit;
    QPushButton *m_LoginButton;
    QPushButton *m_QuitButton;

    CUserInfo *m_pUserInfo;
};

#endif // LOGINDIALOG_H
