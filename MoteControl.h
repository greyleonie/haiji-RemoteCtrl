#ifndef MOTECONTROL_H
#define MOTECONTROL_H

#include "Mote.h"
#include "UserInfo.h"
#include "Logs.h"
#include "OperLogs.h"
#include "ui_MoteControl.h"

namespace Ui
{
    class MoteControl;
}

class CMoteControl : public QWidget
{
    Q_OBJECT
public:
    CMoteControl(QWidget *parent, CMote *mote, CUserInfo *userInfo);
    ~CMoteControl();

private slots:
    void UpdateMoteState();
    void UpdateGateOpenCfgVal();
    void GateOpen();
    void GateStop();
    void GateClose();
    void GateConfig();
    void Reset();

private:
    Ui::MoteControl *ui;
    CMote   *m_Mote;
    CUserInfo *m_CurrUserInfo;

};

#endif // MOTECONTROL_H
