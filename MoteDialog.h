#ifndef MOTEDIALOG_H
#define MOTEDIALOG_H

/*
#include <QWidget>
#include <QDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QList>
#include <QLabel>
#include <QLayout>
//*/
#include <QtCore>
#include <QtGui>
#include <QtSql/QSqlQuery>
//#include "ui_MoteDialog.h"
#include "Mote.h"
#include "MoteConfig.h"
#include "UserInfo.h"
#include "Logs.h"
#include "OperLogs.h"
/*
namespace Ui
{
    class MoteDialog;
}
//*/

class CMoteDialog : public QDialog
{
    Q_OBJECT

public:
    CMoteDialog(QWidget *parent, QList<CMote *> &moteList, CUserInfo *userInfo);
    ~CMoteDialog();

signals:
    void UpdMote();
    void CloseApp();

private slots:
    void New(void);
    void Delete(void);
    void Save(void);
    void EnSave(void);
    void UpdTabName(QString name);
    void Close();

private:
    QPushButton *m_NewButton;
    QPushButton *m_DeleteButton;
    QPushButton *m_SaveButton;
    QPushButton *m_CloseButton;

//    Ui::MoteDialog *ui_dialog;
    CMoteConfig *ui_mote;
    QTabWidget  *m_MoteTab;
    QList<QWidget> m_TabList;
    QList<CMote *> *m_MoteList;
    CUserInfo *m_CurrUserInfo;
    bool m_IsResetApp;


};

#endif // MOTESDIALOG_H
