#ifndef LOGVIEWDIALOG_H
#define LOGVIEWDIALOG_H

#include <QtGui>
#include <QtCore>
#include <QtSql/QSqlQuery>
#include <QSqlTableModel>
#include "UserInfo.h"

class CLogViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLogViewDialog(QWidget *parent, CUserInfo *userInfo);
    ~CLogViewDialog();

private slots:
    void Query();
    void Print();

private:
    QGroupBox *m_FunctionGroup;
    QLabel *m_UserNameLabel;
    QLineEdit *m_UserNameEdit;
    QLabel *m_StartDateLabel;
    QDateEdit *m_StartDateEdit;
    QLabel *m_EndDateLabel;
    QDateEdit *m_EndDateEdit;
    QPushButton *m_QueryButton;
    QPushButton *m_PrintButton;
    QGroupBox *m_ViewGroup;
    QTableView *m_OperLogsView;
    QSqlTableModel *m_OperLogsModel;
    QPrinter *m_Printer;
};

#endif // LOGVIEWDIALOG_H
