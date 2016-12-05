#include "Console.h"
#include "Config.h"

CConsole::CConsole(QWidget *parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));

    CLogs log;
    log.set("====系统启动====");

    CConfig cfg;

    m_db = QSqlDatabase::addDatabase(cfg.getDB());
    m_db.setHostName(cfg.getDBHost());
    m_db.setDatabaseName(cfg.getDBName());
    m_db.setUserName(cfg.getDBUsername());
    m_db.setPassword(cfg.getDBPassword());

    if (!m_db.open())
    {
        log.set("数据库连接失败");
        QMessageBox::warning(this, tr("Warning"), tr("数据库连接失败"), QMessageBox::Yes);
    }
    log.set("数据库连接成功");
}

CConsole::~CConsole()
{
    m_db.close();
    CLogs log;
    log.set("数据库连接关闭");
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
