#ifndef CONFIG_H
#define CONFIG_H

#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QFile>
#include <QString>


class CConfig : public QWidget
{
public:
    CConfig(QWidget *parent = 0);
    ~CConfig();

    QString getDB();
    QString getDBHost();
    QString getDBName();
    QString getDBUsername();
    QString getDBPassword();

private:
    QFile *m_fconfig;
    QString m_sDB;
    QString m_sDBHost;
    QString m_sDBName;
    QString m_sDBUsername;
    QString m_sDBPassword;

};

#endif // CONFIG_H
