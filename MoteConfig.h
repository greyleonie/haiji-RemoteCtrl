#ifndef MOTECONFIG_H
#define MOTECONFIG_H

#include <QString>
#include <QStringList>
#include <QtGui>
#include <QtCore>
//#include "ui_MoteConfig.h"
#include "Mote.h"
#include "Logs.h"
#include "OperLogs.h"
/*
namespace Ui
{
    class MoteConfig;
}
//*/
class CMoteConfig : public QWidget
{
    Q_OBJECT

public:
    enum{
        NONE,
        NEW,
        DELETE,
        UPDATE
    };

    CMoteConfig(QWidget *parent, CMote *mote);
    ~CMoteConfig();

    void setName(QString name);
    void setType(int type);
    void setIpAddr(QString ipAddr);
    void setPort(int port);
    void setGroupNum(int group);
    void setMaxOpenVal(int open);
    void setScanInterval(int intervalInSec);
    void setMote(CMote *mote);
    void setAction(int action);

    QString getName(void);
    int getType(void);
    QString getIpAddr(void);
    int getPort(void);
    int getGroupNum(void);
    int getMaxOpenVal(void);
    int getScanInterval(void);
    CMote *getMote(void);
    int getAction(void);

signals:
    void enableSave(void);
    void updateName(QString);

private slots:
    void EnSave(void);
    void updName(void);
    void updType(void);
    void updIpAddr(void);
    void updPort(void);
    void updGroupNum(void);
    void updMaxOpenVal(void);
    void updScanInterval(void);

private:
    QLabel *m_NameLabel;
    QLineEdit   *m_NameEdit;
    QLabel  *m_GroupLabel;
    QComboBox   *m_GroupCombo;
    QLabel  *m_MaxOpenValLabel;
    QLineEdit   *m_MaxOpenValEdit;
    QLabel  *m_IpAddressLabel;
    QLineEdit   *m_IpAddressEdit;
    QLabel  *m_PortLabel;
    QLineEdit   *m_PortEdit;
    QLabel  *m_ScanIntervalLabel;
    QLineEdit   *m_ScanIntervalEdit;
    QLabel  *m_TypeLabel;
    QComboBox   *m_TypeCombo;

//    Ui::MoteConfig *ui;
    QStringList m_ScanIntervalList;
    QStringList m_GroupList;
    CMote *m_Mote;
    int m_Action;
};


#endif // MOTECONFIG_H
