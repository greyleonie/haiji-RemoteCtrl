#include <QMessageBox>
#include "Config.h"

CConfig::CConfig(QWidget *parent)
{
    m_fconfig = new QFile("./config.xml");

    if(!m_fconfig->open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Warning"), tr("打开配置文件失败"), QMessageBox::Yes);
    }

    QDomDocument xml;
    QString errStr;
    int errLine, errCol;
    if(!xml.setContent(m_fconfig, false, &errStr, &errLine, &errCol))
    {
        QMessageBox::warning(this, tr("Warning"), tr("配置文件加载失败"), QMessageBox::Yes);
        m_fconfig->close();
        return;
    }
    m_fconfig->close();

    QDomElement root = xml.documentElement();
    QDomElement dbcfg = root.firstChildElement();
    if(root.nodeName() != "Configuration"
            || dbcfg.nodeName() != "DatabaseConnection")
    {
        QMessageBox::warning(this, tr("Warning"), tr("配置文件内容错误"), QMessageBox::Yes);
        return;
    }

    for(QDomNode n = dbcfg.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QString tag = n.toElement().tagName();
        QDomElement e = n.toElement();
        if(e.tagName() == "Database")
            m_sDB = e.text();
        else if(e.tagName() == "Host")
            m_sDBHost = e.text();
        else if(e.tagName() == "DatabaseName")
            m_sDBName = e.text();
        else if(e.tagName() == "UserName")
            m_sDBUsername = e.text();
        else if(e.tagName() == "Password")
            m_sDBPassword = e.text();
   }

}

CConfig::~CConfig()
{
    delete m_fconfig;
}

QString CConfig::getDB()
{
    return m_sDB;
}

QString CConfig::getDBHost()
{
    return m_sDBHost;
}

QString CConfig::getDBName()
{
    return m_sDBName;
}

QString CConfig::getDBUsername()
{
    return m_sDBUsername;
}

QString CConfig::getDBPassword()
{
    return m_sDBPassword;
}
