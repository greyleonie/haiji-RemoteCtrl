#include "MoteControl.h"

CMoteControl::CMoteControl(QWidget *parent, CMote *mote, CUserInfo *userInfo)
    : QWidget(parent)
{
    m_CurrUserInfo = userInfo;
    m_Mote = mote;

    ui = new Ui::MoteControl;
    ui->setupUi(this);

    ui->groupBox_Mote->setTitle(m_Mote->getName());

    connect(m_Mote, SIGNAL(updateState()), this, SLOT(UpdateMoteState()));
    connect(ui->pushButton_GateOpen, SIGNAL(clicked()), this, SLOT(GateOpen()));
    connect(ui->pushButton_GateStop, SIGNAL(clicked()), this, SLOT(GateStop()));
    connect(ui->pushButton_GateClose, SIGNAL(clicked()), this, SLOT(GateClose()));
    connect(ui->pushButton_GateConfig, SIGNAL(clicked()), this, SLOT(GateConfig()));
    connect(ui->pushButton_Reset, SIGNAL(clicked()), this, SLOT(Reset()));
    connect(ui->lineEdit_GateOpenCfgVal, SIGNAL(editingFinished()), this, SLOT(UpdateGateOpenCfgVal()));

    UpdateMoteState();
}

CMoteControl::~CMoteControl()
{
    ;
}


void CMoteControl::UpdateMoteState()
{
    ui->groupBox_Mote->setTitle(m_Mote->getName());

    ui->label_CurrGateOpenVal->setText(QString::number(m_Mote->getCurrGateOpenVal()));
    ui->label_CurrGateOpenVal->setStyleSheet("color:blue");
    ui->label_GateOpenCfgReadVal->setText(QString::number(m_Mote->getCurrGateCfgOpenVal()));
    ui->label_GateOpenCfgReadVal->setStyleSheet("color:blue");

    ui->label_GateDownVal->setText(QString::number(m_Mote->getCurrGateDownVal()));
    ui->label_GateDownVal->setStyleSheet("color:blue");

    if(m_Mote->getType() == 0)
    {
        ui->label_GateDeviationVal->setText(QString::number(m_Mote->getCurrGateDeviation()));
        ui->label_GateDeviationVal->setStyleSheet("color:blue");
    }
    else
    {
        ui->label_GateDeviation->hide();
        ui->label_GateDeviationVal->hide();
        ui->label_GateDeviationValUnit->hide();
    }

    ui->label_OilTempVal->setText(QString::number(m_Mote->getCurrOilTemp()));
    ui->label_OilTempVal->setStyleSheet("color:blue");

    ui->label_SysPressVal->setText(QString::number(m_Mote->getCurrSysPress()));
    ui->label_SysPressVal->setStyleSheet("color:blue");

    if(m_Mote->isGateAutoCtrl() && !m_Mote->isGateRemoteCtrl())
    {
        ui->label_GateAutoRemoteCtrl->setText(tr("自动控制"));
        ui->label_GateAutoRemoteCtrl->setStyleSheet("color:blue");
    }
    else if(!m_Mote->isGateAutoCtrl() && m_Mote->isGateRemoteCtrl())
    {
        ui->label_GateAutoRemoteCtrl->setText(tr("远程控制"));
        ui->label_GateAutoRemoteCtrl->setStyleSheet("color:blue");
    }
    else
    {
        //ui->label_GateAutoRemoteCtrl->hide();
        ui->label_GateAutoRemoteCtrl->setText(tr("自动/远程"));
        ui->label_GateAutoRemoteCtrl->setStyleSheet("color:grey");
    }

    if(m_Mote->isGateAllOpen() && !m_Mote->isGateAllClose())
    {
        ui->label_GateOpenClose->setText(tr("闸门全开"));
        ui->label_GateOpenClose->setStyleSheet("color:blue");
    }
    else if(!m_Mote->isGateAllOpen() && m_Mote->isGateAllClose())
    {
        ui->label_GateOpenClose->setText(tr("闸门全关"));
        ui->label_GateOpenClose->setStyleSheet("color:blue");
    }
    else
    {
        //ui->label_GateOpenClose->hide();
        ui->label_GateOpenClose->setText(tr("全开/全关"));
        ui->label_GateOpenClose->setStyleSheet("color:grey");
    }

    if(m_Mote->isGateOn() && !m_Mote->isGateOff())
    {
        ui->label_GateOnOff->setText(tr("闸门开启"));
        ui->label_GateOnOff->setStyleSheet("color:blue");
    }
    else if(!m_Mote->isGateOn() && m_Mote->isGateOff())
    {
        ui->label_GateOnOff->setText(tr("闸门关闭"));
        ui->label_GateOnOff->setStyleSheet("color:blue");
    }
    else
    {
        //ui->label_GateOnOff->hide();
        ui->label_GateOnOff->setText(tr("开启/关闭"));
        ui->label_GateOnOff->setStyleSheet("color:grey");
    }

    if(m_Mote->isGateDown300())
    {
        ui->label_GateDownAlarm->setText(tr("下滑300M"));
        ui->label_GateDownAlarm->setStyleSheet("color:red");
    }
    else if(m_Mote->isGateDown200())
    {
        ui->label_GateDownAlarm->setText(tr("下滑200M"));
        ui->label_GateDownAlarm->setStyleSheet("color:red");
    }
    else
    {
        //ui->label_GateDownAlarm->hide();
        ui->label_GateDownAlarm->setText(tr("下滑报警"));
        ui->label_GateDownAlarm->setStyleSheet("color:grey");
    }

    if(m_Mote->isGateOpenSlowAlarm())
        ui->label_GateOpenSlowAlarm->setStyleSheet("color:red");
    else
        ui->label_GateOpenSlowAlarm->setStyleSheet("color:grey");

    if(m_Mote->isGateOpenSlowStop())
        ui->label_GateOpenSlowStop->setStyleSheet("color:red");
    else
        ui->label_GateOpenSlowStop->setStyleSheet("color:grey");

    if(m_Mote->isGateUpDownTimeout())
        ui->label_GateTimeoutStop->setStyleSheet("color:red");
    else
        ui->label_GateTimeoutStop->setStyleSheet("color:grey");

    if(m_Mote->getType() == 0)//表孔
    {
        if(m_Mote->isGateDeviationOver())
            ui->label_GateDeviationOver->setStyleSheet("color:red");
        else
            ui->label_GateDeviationOver->setStyleSheet("color:grey");
    }
    else//中孔
    {
        ui->label_GateDeviationOver->hide();
    }

    if(m_Mote->getType() == 0)
    {
        ui->label_GateTravel->hide();
        ui->label_GateTravelVal->hide();
        ui->label_GateTravelValUnit->hide();

        ui->label_GateLeftTravelVal->setText(QString::number(m_Mote->getCurrGateLeftTravel()));
        ui->label_GateLeftTravelVal->setStyleSheet("color:blue");
        ui->label_GateRightTravelVal->setText(QString::number(m_Mote->getCurrGateRightTravel()));
        ui->label_GateRightTravelVal->setStyleSheet("color:blue");
    }
    else
    {
        ui->label_GateLeftTravel->hide();
        ui->label_GateLeftTravelVal->hide();
        ui->label_GateLeftTravelValUnit->hide();
        ui->label_GateRightTravel->hide();
        ui->label_GateRightTravelVal->hide();
        ui->label_GateRightTravelValUnit->hide();

        ui->label_GateTravelVal->setText(QString::number(m_Mote->getCurrOilTravel()));
        ui->label_GateTravelVal->setStyleSheet("color:blue");
    }

    if(m_Mote->isPump1AutoCtrl())
        ui->label_Pump1AutoCtrl->setStyleSheet("color:blue");
    else
        ui->label_Pump1AutoCtrl->setStyleSheet("color:grey");

    if(m_Mote->isPump1Run())
        ui->label_Pump1Run->setStyleSheet("color:blue");
    else
        ui->label_Pump1Run->setStyleSheet("color:grey");

    if(m_Mote->isPump1Fault())
        ui->label_Pump1Fault->setStyleSheet("color:red");
    else
        ui->label_Pump1Fault->setStyleSheet("color:grey");

    if(m_Mote->isPump1Mesh())
        ui->label_Pump1Mesh->setStyleSheet("color:blue");
    else
        ui->label_Pump1Mesh->setStyleSheet("color:grey");

    if(m_Mote->isPump1StartFailed())
        ui->label_Pump1StartFailed->setStyleSheet("color:red");
    else
        ui->label_Pump1StartFailed->setStyleSheet("color:grey");

    if(m_Mote->isCtrlPower1Ok())
    {
        ui->label_CtrlPower1Ok->setText(tr("电源正常"));
        ui->label_CtrlPower1Ok->setStyleSheet("color:green");
    }
    else
    {
        ui->label_CtrlPower1Ok->setText(tr("电源异常"));
        ui->label_CtrlPower1Ok->setStyleSheet("color:red");
    }

    if(m_Mote->isPump2AutoCtrl())
        ui->label_Pump2AutoCtrl->setStyleSheet("color:blue");
    else
        ui->label_Pump2AutoCtrl->setStyleSheet("color:grey");

    if(m_Mote->isPump2Run())
        ui->label_Pump2Run->setStyleSheet("color:blue");
    else
        ui->label_Pump2Run->setStyleSheet("color:grey");

    if(m_Mote->isPump2Fault())
        ui->label_Pump2Fault->setStyleSheet("color:red");
    else
        ui->label_Pump2Fault->setStyleSheet("color:grey");

    if(m_Mote->isPump2Mesh())
        ui->label_Pump2Mesh->setStyleSheet("color:blue");
    else
        ui->label_Pump2Mesh->setStyleSheet("color:grey");

    if(m_Mote->isPump2StartFailed())
        ui->label_Pump2StartFailed->setStyleSheet("color:red");
    else
        ui->label_Pump2StartFailed->setStyleSheet("color:grey");

    if(m_Mote->isCtrlPower2Ok())
    {
        ui->label_CtrlPower2Ok->setText(tr("电源正常"));
        ui->label_CtrlPower2Ok->setStyleSheet("color:green");
    }
    else
    {
        ui->label_CtrlPower2Ok->setText(tr("电源异常"));
        ui->label_CtrlPower2Ok->setStyleSheet("color:red");
    }

    if(m_Mote->isSysPressHigh())
    {
        ui->label_PressAlarm->setText("压力过高");
        ui->label_PressAlarm->setStyleSheet("color:red");
    }
    else if(m_Mote->isSysPressLow())
    {
        ui->label_PressAlarm->setText("压力过低");
        ui->label_PressAlarm->setStyleSheet("color:red");
    }
    else
    {
        ui->label_PressAlarm->setText("压力正常");
        ui->label_PressAlarm->setStyleSheet("color:grey");
    }

    if(m_Mote->getType() == 0)  //表孔
    {
        if(m_Mote->isLeftCavityPipeBreak())
            ui->label_LeftCavityPipeBreak->setStyleSheet("color:red");
        else
            ui->label_LeftCavityPipeBreak->setStyleSheet("color:grey");

        if(m_Mote->isRightCavityPipeBreak())
            ui->label_RightCavityPipeBreak->setStyleSheet("color:red");
        else
            ui->label_RightCavityPipeBreak->setStyleSheet("color:grey");

        ui->label_CavityPipeBreak->hide();
        ui->label_CavityPressOver->hide();
    }
    else   //中孔
    {
        if(m_Mote->isCavityPipeBreak())
             ui->label_CavityPipeBreak->setStyleSheet("color:red");
        else
             ui->label_CavityPipeBreak->setStyleSheet("color:grey");

        if(m_Mote->isCavityPressOver())
            ui->label_CavityPressOver->setStyleSheet("color:red");
        else
            ui->label_CavityPressOver->setStyleSheet("color:grey");

        ui->label_LeftCavityPipeBreak->hide();
        ui->label_RightCavityPipeBreak->hide();
    }

    if(m_Mote->isSysPressConverterFault())
        ui->label_PressConverterFault->setStyleSheet("color:red");
    else
        ui->label_PressConverterFault->setStyleSheet("color:grey");

    if(m_Mote->isNoPoleCavityPressHigh())
        ui->label_NoPoleCavityPressHigh->setStyleSheet("color:red");
    else
        ui->label_NoPoleCavityPressHigh->setStyleSheet("color:grey");

    if(m_Mote->isElectromagneticPowerOk())
    {
        ui->label_ElectromagneticPowerOk->setText("电磁阀电源正常");
        ui->label_ElectromagneticPowerOk->setStyleSheet("color:green");
    }
    else
    {
        ui->label_ElectromagneticPowerOk->setText("电磁阀电源异常");
        ui->label_ElectromagneticPowerOk->setStyleSheet("color:red");
    }

    if(m_Mote->isOilLevelHigh())
    {
        ui->label_OilLevel->setText(tr("油位过高"));
        ui->label_OilLevel->setStyleSheet("color:red");
    }
    else if(m_Mote->isOilLevelLow())
    {
        ui->label_OilLevel->setText(tr("油位过低"));
        ui->label_OilLevel->setStyleSheet("color:red");
    }
    else
    {
        ui->label_OilLevel->setText(tr("油位正常"));
        ui->label_OilLevel->setStyleSheet("color:green");
    }

    if(m_Mote->isOilTempHigh())
    {
        ui->label_OilTempState->setText(tr("油温过高"));
        ui->label_OilTempState->setStyleSheet("color:red");
    }
    else if(m_Mote->isOilTempLow())
    {
        ui->label_OilTempState->setText(tr("油温过低"));
        ui->label_OilTempState->setStyleSheet("color:red");
    }
    else
    {
        ui->label_OilTempState->setText(tr("油温正常"));
        ui->label_OilTempState->setStyleSheet("color:green");
    }

    if(m_Mote->isOilFilterFault())
        ui->label_OilFilterFault->setStyleSheet("color:red");
    else
        ui->label_OilFilterFault->setStyleSheet("color:grey");

    if(m_Mote->isOilHeaterOn())
        ui->label_OilHeaterOn->setStyleSheet("color:blue");
    else
        ui->label_OilHeaterOn->setStyleSheet("color:grey");

    if(m_Mote->isOilTempTransmitterFault())
        ui->label_OilTempTransmitterFault->setStyleSheet("color:red");
    else
        ui->label_OilTempTransmitterFault->setStyleSheet("color:grey");

    ui->label_IpAddr->setText(m_Mote->getIpAddr() + ":" + QString::number(m_Mote->getPort()));
    if(m_Mote->isRemoteConnected())
    {
        ui->label_ConnectStateVal->setText(tr("已连接"));
        ui->label_ConnectStateVal->setStyleSheet("color:green");
    }
    else
    {
        ui->label_ConnectStateVal->setText(tr("未连接"));
        ui->label_ConnectStateVal->setStyleSheet("color:red");
    }
}

void CMoteControl::UpdateGateOpenCfgVal()
{
    m_Mote->setCfgOpenVal(ui->lineEdit_GateOpenCfgVal->text().toInt());
}

void CMoteControl::GateOpen()
{
    m_Mote->openRemoteGate();

    QString logstr = "现地站:" + m_Mote->getName() + "闸门开启";
    COperLogs operlog(m_CurrUserInfo);
    operlog.set(logstr);
    CLogs log;
    log.set(logstr);
}

void CMoteControl::GateStop()
{
    m_Mote->stopRemoteGate();

    QString logstr = "现地站:" + m_Mote->getName() + " 闸门停止";
    COperLogs operlog(m_CurrUserInfo);
    operlog.set(logstr);
    CLogs log;
    log.set(logstr);
}

void CMoteControl::GateClose()
{
    m_Mote->closeRemoteGate();

    QString logstr = "现地站:" + m_Mote->getName() + " 闸门关闭";
    COperLogs operlog(m_CurrUserInfo);
    operlog.set(logstr);
    CLogs log;
    log.set(logstr);
}

void CMoteControl::GateConfig()
{
    m_Mote->setRemoteGateOpenCfgVal();

    QString logstr = "现地站:" + m_Mote->getName() + " 闸门预设开度:" + QString::number(m_Mote->getCfgOpenVal());
    COperLogs operlog(m_CurrUserInfo);
    operlog.set(logstr);
    CLogs log;
    log.set(logstr);
}

void CMoteControl::Reset()
{
    m_Mote->resetRemoteGate();

    QString logstr = "现地站:" + m_Mote->getName() + " 故障复位";
    COperLogs operlog(m_CurrUserInfo);
    operlog.set(logstr);
    CLogs log;
    log.set(logstr);
}
