#ifndef STATEVIEWDIALOG_H
#define STATEVIEWDIALOG_H

#include <QtGui>
#include <QtCore>
#include <QtSql/QSqlQuery>
#include <QSqlTableModel>
#include "UserInfo.h"

class CStateViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CStateViewDialog(QWidget *parent, CUserInfo *userInfo);
    ~CStateViewDialog();

private slots:
    void Query();
    void Print();

private:
    QGroupBox *m_FunctionGroup;
    QLabel *m_MoteNameLabel;
    QLineEdit *m_MoteNameEdit;
    QLabel *m_StartTimeLabel;
    QDateTimeEdit *m_StartTimeEdit;
    QLabel *m_EndTimeLabel;
    QDateTimeEdit *m_EndTimeEdit;
    QPushButton *m_QueryButton;
    QPushButton *m_PrintButton;
    QGroupBox *m_ViewGroup;
    QTableView *m_StateLogsView;
    QSqlTableModel *m_StateLogsModel;
    QPrinter *m_Printer;

};

#endif // STATEVIEWDIALOG_H
