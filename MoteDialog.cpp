#include <QMessageBox>
#include <QDebug>
#include "MoteDialog.h"

CMoteDialog::CMoteDialog(QWidget *parent, QList<CMote *> &moteList, CUserInfo *userInfo) :
    QDialog(parent)
{
    m_IsResetApp = false;

    m_NewButton = new QPushButton(this);
    m_NewButton->setText(tr("�½�"));

    m_DeleteButton = new QPushButton(this);
    m_DeleteButton->setText(tr("ɾ��"));

    m_SaveButton = new QPushButton(this);
    m_SaveButton->setText(tr("����"));

    m_CloseButton = new QPushButton(this);
    m_CloseButton->setText(tr("�ر�"));

    connect(m_NewButton, SIGNAL(clicked()), this, SLOT(New()));
    connect(m_DeleteButton, SIGNAL(clicked()), this, SLOT(Delete()));
    connect(m_SaveButton, SIGNAL(clicked()), this, SLOT(Save()));
    connect(m_CloseButton, SIGNAL(clicked()), this, SLOT(Close()));

    m_MoteList = &moteList;
    m_MoteTab = new QTabWidget(this);

    for(int i = 0; i < m_MoteList->size(); ++i)
    {
        CMote *mote = m_MoteList->at(i);
        CMoteConfig *moteCfg = new CMoteConfig(this, mote);
        connect(moteCfg, SIGNAL(enableSave()), this, SLOT(EnSave()));
        connect(moteCfg, SIGNAL(updateName(QString)), this, SLOT(UpdTabName(QString)));
        m_MoteTab->addTab(moteCfg, mote->getName());
        connect(m_SaveButton, SIGNAL(clicked()), mote, SIGNAL(updateState()));
    }
    //m_tab->resize(480,145);

    m_DeleteButton->setEnabled(m_MoteList->size() != 0 ? true : false);
    m_SaveButton->setEnabled(false);

    m_CurrUserInfo = userInfo;

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(m_MoteTab, 0, 0, 1, 6);
    layout->addWidget(m_NewButton, 1, 2, 1, 1);
    layout->addWidget(m_DeleteButton, 1, 3, 1, 1);
    layout->addWidget(m_SaveButton, 1, 4, 1, 1);
    layout->addWidget(m_CloseButton, 1, 5, 1, 1);

    this->setLayout(layout);
    this->setWindowTitle(tr("�����ֵ�վ..."));

}

CMoteDialog::~CMoteDialog()
{
    ;
}

void CMoteDialog::New(void)
{
    CMote *mote = new CMote();

    m_MoteList->append(mote);
    CMoteConfig *moteCfg = new CMoteConfig(this, mote);
    connect(moteCfg, SIGNAL(enableSave()), this, SLOT(EnSave()));
    connect(moteCfg, SIGNAL(updateName(QString)), this, SLOT(UpdTabName(QString)));
    moteCfg->setAction(CMoteConfig::NEW);
    m_MoteTab->addTab(moteCfg, mote->getName());
    m_MoteTab->setCurrentWidget(moteCfg);
    m_MoteTab->resize(480, 145);

    m_DeleteButton->setEnabled(true);
}

void CMoteDialog::Delete(void)
{
    if(QMessageBox::No == QMessageBox::question(this, tr("ɾ��"), tr("��ȷ��Ҫɾ�����ֵ�վ��"), QMessageBox::Yes, QMessageBox::No))
        return;

    CMoteConfig *moteCfg = (CMoteConfig *)m_MoteTab->currentWidget();
    CMote *mote = moteCfg->getMote();
    mote->del();
    m_MoteList->removeOne(mote);
    m_MoteTab->removeTab(m_MoteTab->currentIndex());
    m_DeleteButton->setEnabled(m_MoteList->size() != 0 ? true : false);
    m_IsResetApp = true;

    QString logstr = "ɾ���ֵ�վ:" + mote->getName();
    COperLogs operlog(m_CurrUserInfo);
    operlog.set(logstr);
    CLogs log;
    log.set(logstr);

    delete mote;
}

void CMoteDialog::Save(void)
{
    m_SaveButton->setEnabled(false);

    for(int i = 0; i < m_MoteTab->count(); ++i)
    {
        CMoteConfig *moteCfg = (CMoteConfig *)m_MoteTab->widget(i);
        if(moteCfg->getAction() == CMoteConfig::NEW)
        {
            moteCfg->setAction(CMoteConfig::NONE);
            CMote *mote = moteCfg->getMote();
            mote->add();
            m_IsResetApp = true;

            QString logstr = "�½��ֵ�վ:" + mote->getName() +
                            "(IP=" + mote->getIpAddr() +
                            ", �˿�=" + QString::number(mote->getPort()) +
                            ", ����=" + QString::number(mote->getGroupNum()) +
                            ", ɨ����(��)=" + QString::number(mote->getScanInterval() / 1000) +
                            ", ��󿪶�ֵ=" + QString::number(mote->getMaxOpenVal()) +
                            ", ����=" + QString::number(mote->getType()) + ")";
            COperLogs operlog(m_CurrUserInfo);
            operlog.set(logstr);
            CLogs log;
            log.set(logstr);
        }
        else if (moteCfg->getAction() == CMoteConfig::UPDATE)
        {
            moteCfg->setAction(CMoteConfig::NONE);
            CMote *mote = moteCfg->getMote();
            mote->upd();
            m_IsResetApp = true;

            QString logstr = "�����ֵ�վ:" + mote->getName() +
                            "(IP=" + mote->getIpAddr() +
                            ", �˿�=" + QString::number(mote->getPort()) +
                            ", ����=" + QString::number(mote->getGroupNum()) +
                            ", ɨ����(��)=" + QString::number(mote->getScanInterval() / 1000) +
                            ", ��󿪶�ֵ=" + QString::number(mote->getMaxOpenVal()) +
                            ", ����=" + QString::number(mote->getType()) + ")";
            COperLogs operlog(m_CurrUserInfo);
            operlog.set(logstr);
            CLogs log;
            log.set(logstr);
        }
        else
        {
            ;
        }
    }//end for
}

void CMoteDialog::EnSave(void)
{
    m_SaveButton->setEnabled(true);
}

void CMoteDialog::UpdTabName(QString name)
{
    m_MoteTab->setTabText(m_MoteTab->currentIndex(), name);
}

void CMoteDialog::Close()
{
    if(m_IsResetApp)
    {
        m_IsResetApp = false;
        if(QMessageBox::Yes == QMessageBox::question(this, tr("��ʾ"), tr("�ײ������Ѹ��ģ����µ�¼����Ч��\n�Ƿ������˳�ϵͳ��"), QMessageBox::Yes, QMessageBox::No))
            emit CloseApp();
    }

    this->close();
}

