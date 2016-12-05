#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QtSql/QSqlQuery>
#include "UserDialog.h"
#include "MoteDialog.h"
#include "UserInfo.h"
#include "Mote.h"
#include "MoteControl.h"
#include "ui_monitor.h"
#include "MoteTableDialog.h"
#include "Logs.h"

namespace Ui
{
    class Window_monitor;
}

class CMoniterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMoniterWindow(QWidget *parent, CUserInfo *userInfo);
    ~CMoniterWindow();
    void setCurrUserInfo(CUserInfo *currUserInfo);

public slots:
    void DoUser();
    void DoMote();
    void RefreshWindow();
    void ShowLog();
    void ShowState();

private:
    Ui::Window_monitor *ui;
    CUserInfo *m_pCurrUserInfo;
    QList<CMote *> m_MoteList;

};

#endif // MONITORWINDOW_H
