#include "Console.h"
#include "Config.h"

CConsole::CConsole(QWidget *parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));

    CLogs log;
    log.set("====ϵͳ����====");

    CConfig cfg;

    m_db = QSqlDatabase::addDatabase(cfg.getDB());
    m_db.setHostName(cfg.getDBHost());
    m_db.setDatabaseName(cfg.getDBName());
    m_db.setUserName(cfg.getDBUsername());
    m_db.setPassword(cfg.getDBPassword());

    if (!m_db.open())
    {
        log.set("���ݿ�����ʧ��");
        QMessageBox::warning(this, tr("Warning"), tr("���ݿ�����ʧ��"), QMessageBox::Yes);
    }
    log.set("���ݿ����ӳɹ�");
}

CConsole::~CConsole()
{
    m_db.close();
    CLogs log;
    log.set("���ݿ����ӹر�");
}

int CConsole::exec()
{
    CLoginDialog *login = new CLoginDialog;
    login->show();
    int ret = login->exec();
    if (ret == QDialog::Rejected)
        return 0;

    if (ret == QDialog::Accepted)
    {
        CMoniterWindow *m = new CMoniterWindow(this, login->getUserInfo());
        //m->setCurrUserInfo(login->getUserInfo());
        m->show();
    }

    return 1;
}
