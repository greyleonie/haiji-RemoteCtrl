#include "MonitorWindow.h"
#include "LogViewDialog.h"
#include "StateViewDialog.h"
#include <QDebug>

CMoniterWindow::CMoniterWindow(QWidget *parent, CUserInfo *userInfo) :
    QMainWindow(parent)
{
    ui = new Ui::Window_monitor;
    ui->setupUi(this);

    m_pCurrUserInfo = userInfo;

    connect(ui->action_quit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->action_user, SIGNAL(triggered()), this, SLOT(DoUser()));
    connect(ui->action_mote, SIGNAL(triggered()), this, SLOT(DoMote()));
    connect(ui->action_runlog, SIGNAL(triggered()), this, SLOT(ShowLog()));
    connect(ui->action_statelog, SIGNAL(triggered()), this, SLOT(ShowState()));

    switch(m_pCurrUserInfo->getRole())
    {
        case ROLE_ADMINISTRATOR:
            ui->action_mote->setEnabled(true);
            ui->action_user->setEnabled(true);
            break;
        case ROLE_ENGINEER:
            ui->action_mote->setEnabled(false);
            ui->action_user->setEnabled(false);
            break;
        default:
            ui->action_mote->setEnabled(false);
            ui->action_user->setEnabled(false);
    }
    RefreshWindow();
    this->showMaximized();
/*
    QHBoxLayout *hlayout = new QHBoxLayout;
    QSqlQuery dbQuery;
    dbQuery.setForwardOnly(true);
    dbQuery.exec("SELECT * FROM t_mote;");
    while(dbQuery.next())
    {
        //CMote(int id, QString name, QString ipAddr, int port, int group, int open, int maxOpen, int interval);
        CMote *mote = new CMote(dbQuery.value(0).toInt(),
                                dbQuery.value(1).toString(),
                                dbQuery.value(2).toString(),
                                dbQuery.value(3).toInt(),
                                dbQuery.value(4).toInt(),
                                dbQuery.value(7).toInt(),
                                dbQuery.value(6).toInt(),
                                dbQuery.value(5).toInt());
        m_MoteList.append(mote);

        CMoteControl *moteCtrl = new CMoteControl(this, mote);
        hlayout->addWidget(moteCtrl);
    }
    this->centralWidget()->setLayout(hlayout);
    this->showMaximized();
//*/
}

CMoniterWindow::~CMoniterWindow()
{
    for(int i = 0; i < m_MoteList.size(); ++i)
    {
        CMote *mote = m_MoteList.at(i);
        m_MoteList.removeAt(i);
        delete mote;
    }
    delete m_pCurrUserInfo;
    delete ui;
}

void CMoniterWindow::DoUser()
{
    CUserDialog *userDialog = new CUserDialog(this);
    userDialog->exec();
}

void CMoniterWindow::DoMote()
{
    CMoteDialog *moteDialog = new CMoteDialog(this, m_MoteList, m_pCurrUserInfo);
    //CMoteTableDialog *moteDialog = new CMoteTableDialog(this);
    connect(moteDialog, SIGNAL(UpdMote()), this, SLOT(RefreshWindow()));
    connect(moteDialog, SIGNAL(CloseApp()), this, SLOT(close()));
    moteDialog->exec();
}

void CMoniterWindow::setCurrUserInfo(CUserInfo *currUserInfo)
{
    m_pCurrUserInfo = currUserInfo;
}

void CMoniterWindow::RefreshWindow()
{
    m_MoteList.clear();

    QHBoxLayout *hlayout = new QHBoxLayout;
    QSqlQuery dbQuery;
    dbQuery.setForwardOnly(true);
    dbQuery.exec("SELECT * FROM t_mote;");
    while(dbQuery.next())
    {
        //CMote(int id,
        //      QString name,
        //      QString ipAddr,
        //      int port,
        //      int group,
        //      int open,
        //      int maxOpen,
        //      int interval
        //      int type);
        CMote *mote = new CMote(dbQuery.value(0).toInt(),
                                dbQuery.value(1).toString(),
                                dbQuery.value(2).toString(),
                                dbQuery.value(3).toInt(),
                                dbQuery.value(4).toInt(),
                                dbQuery.value(7).toInt(),
                                dbQuery.value(6).toInt(),
                                dbQuery.value(5).toInt(),
                                dbQuery.value(8).toInt());  ///2014
        m_MoteList.append(mote);

        CMoteControl *moteCtrl = new CMoteControl(this, mote, m_pCurrUserInfo);
        hlayout->addWidget(moteCtrl);
    }
    this->setStyleSheet("font-size:14px");
    this->centralWidget()->setLayout(hlayout);
    this->centralWidget()->update();
    this->update();
}

void CMoniterWindow::ShowLog()
{
    CLogViewDialog *logViewDialog = new CLogViewDialog(this, m_pCurrUserInfo);
    logViewDialog->exec();
}

void CMoniterWindow::ShowState()
{
    CStateViewDialog *stateViewDialog = new CStateViewDialog(this, m_pCurrUserInfo);
    stateViewDialog->exec();
}

