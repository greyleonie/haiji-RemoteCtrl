#include "Mote.h"

CMote::CMote(int id, QString name, QString ipAddr, int port, int group, int open, int maxOpen, int interval, int type)
{
    m_MoteId = id;

    m_Name = name;
    m_IpAddr = ipAddr;
    m_Port = port;
    m_GroupNum = group;
    m_CfgOpenVal = open;
    m_MaxOpenVal = maxOpen;
    m_ScanIntervalInMS = interval;

    m_CurrSysPress = 0;
    m_CurrOpenVal = 0;
    m_CurrOilTravel = 0;
    m_CurrCfgOpenVal = 0;
    m_CurrStateSignal1 = 0;
    m_CurrStateSignal2 = 0;

    m_Type = type;
    m_CurrStateSignal3 = 0;
    m_CurrOilTemp = 0;
    m_CurrGateDownVal = 0;
    m_CurrGateDeviation = 0;
    m_CurrGateLeftTravel = 0;
    m_CurrGateRightTravel = 0;

    QString sqlStr = "SELECT gate_open_val, gate_config_open_val, system_press, oil_travel, signal1, signal2, signal3, oil_temp, gate_down_value, gate_deviation, gate_left_travel, gate_right_travel FROM t_motestatelogs WHERE state_id=(SELECT MAX(state_id) FROM t_motestatelogs WHERE mote_id="
            + QString::number(m_MoteId) + ");";
    QSqlQuery dbQuery;
    dbQuery.exec(sqlStr);
    if(dbQuery.next())
    {
        m_CurrOpenVal = dbQuery.value(0).toInt();
        m_CurrCfgOpenVal = dbQuery.value(1).toInt();
        m_CurrSysPress = dbQuery.value(2).toInt();
        m_CurrOilTravel = dbQuery.value(3).toInt();
        m_CurrStateSignal1 = dbQuery.value(4).toInt();
        m_CurrStateSignal2 = dbQuery.value(5).toInt();
        m_CurrStateSignal3 = dbQuery.value(6).toInt();
        m_CurrOilTemp = dbQuery.value(7).toInt();
        m_CurrGateDownVal = dbQuery.value(8).toInt();
        m_CurrGateDeviation = dbQuery.value(9).toInt();
        m_CurrGateLeftTravel = dbQuery.value(10).toInt();
        m_CurrGateRightTravel = dbQuery.value(11).toInt();
    }

    m_StSignal1 = m_CurrStateSignal1;     //400601
    m_StSignal2 = m_CurrStateSignal2;     //400602
    m_StSysPress = m_CurrSysPress;        //400605
    m_StGateOpenVal = m_CurrOpenVal;      //400606
    m_StOilTravel = m_CurrOilTravel;      //400607
    m_StCfgOpenVal = m_CurrCfgOpenVal;    //400608
    m_GateCfgOpenVal = 0;   //400615
    m_GateOper = 0;         //400616
    m_StSignal3 = m_CurrStateSignal3;     //400603
    m_StOilTemp = m_CurrOilTemp;        //400609
    m_StGateDownVal = m_CurrGateDownVal;    //400610
    m_StGateDeviation = m_CurrGateDeviation;  //400611
    m_StGateLeftTravel = m_CurrGateLeftTravel; //400607
    m_StGateRightTravel = m_CurrGateRightTravel;//400612

    connectToRemote();

    m_OpenGateTimer = new QTimer(this);
    m_OpenGateTimer->setInterval(2000);
    m_OpenGateTimer->setSingleShot(true);
    connect(m_OpenGateTimer, SIGNAL(timeout()), this, SLOT(openGateTimeout()));

    m_StopGateTimer = new QTimer(this);
    m_StopGateTimer->setInterval(2000);
    m_StopGateTimer->setSingleShot(true);
    connect(m_StopGateTimer, SIGNAL(timeout()), this, SLOT(stopGateTimeout()));

    m_CloseGateTimer = new QTimer(this);
    m_CloseGateTimer->setInterval(2000);
    m_CloseGateTimer->setSingleShot(true);
    connect(m_CloseGateTimer, SIGNAL(timeout()), this, SLOT(closeGateTimeout()));

    m_ResetGateTimer = new QTimer(this);
    m_ResetGateTimer->setInterval(2000);
    m_ResetGateTimer->setSingleShot(true);
    connect(m_ResetGateTimer, SIGNAL(timeout()), this, SLOT(resetGateTimeout()));

    m_ScanTimer = new QTimer(this);
    m_ScanTimer->setInterval(m_ScanIntervalInMS);
    m_ScanTimer->setSingleShot(false);
    connect(m_ScanTimer, SIGNAL(timeout()), this, SLOT(scanRemote()));
    m_ScanTimer->start();
}

CMote::~CMote()
{
    disconnectFromRemote();
}

void CMote::setMoteId(int id)
{
    m_MoteId = id;
}

int CMote::getMoteId()
{
    return m_MoteId;
}

void CMote::setType(int type)
{
    m_Type = type;
}

int CMote::getType()
{
    return m_Type;
}

void CMote::setName(QString name)
{
    m_Name = name;
}

QString CMote::getName()
{
    return m_Name;
}

void CMote::setIpAddr(QString ipAddr)
{
    m_IpAddr = ipAddr;
}

QString CMote::getIpAddr()
{
    return m_IpAddr;
}

void CMote::setPort(int port)
{
    m_Port = port;
}

int CMote::getPort()
{
    return m_Port;
}

void CMote::setGroupNum(int group)
{
    m_GroupNum = group;
}

int CMote::getGroupNum()
{
    return m_GroupNum;
}

void CMote::setCfgOpenVal(int open)
{
    m_CfgOpenVal = open;
}

int CMote::getCfgOpenVal()
{
    return m_CfgOpenVal;
}

void CMote::setMaxOpenVal(int maxOpen)
{
    m_MaxOpenVal = maxOpen;
}

int CMote::getMaxOpenVal()
{
    return m_MaxOpenVal;
}

void CMote::setScanInterval(int intervalInSec)
{
    m_ScanIntervalInMS = intervalInSec;
}

int CMote::getScanInterval()
{
    return m_ScanIntervalInMS;
}

int CMote::getCurrSysPress()
{
    return (m_StSysPress / 10);
}

int CMote::getCurrGateOpenVal()
{
    return (m_StGateOpenVal / 1000);
}

int CMote::getCurrOilTravel()
{
    return (m_StOilTravel / 1000);
}

int CMote::getCurrGateCfgOpenVal()
{
    return (m_StCfgOpenVal / 1000);
}

int CMote::getCurrOilTemp()
{
    return (m_StOilTemp / 10);
}

int CMote::getCurrGateDownVal()
{
    return m_StGateDownVal;
}

int CMote::getCurrGateDeviation()
{
    return m_StGateDeviation;
}

int CMote::getCurrGateLeftTravel()
{
    return (m_StGateLeftTravel / 1000);
}

int CMote::getCurrGateRightTravel()
{
    return (m_StGateRightTravel / 1000);
}

bool CMote::isGateAutoCtrl()
{
    return ((m_StSignal1 & MASK_SIGNAL1_GATE_AUTO_CTRL) != 0x0000);
}

bool CMote::isGateRemoteCtrl()
{
    return ((m_StSignal1 & MASK_SIGNAL1_GATE_REMOTE_CTRL) != 0x0000);
}

bool CMote::isGateAllOpen()
{
    return ((m_StSignal2 & MASK_SIGNAL2_GATE_ALL_OPEN) != 0x0000);
}

bool CMote::isGateAllClose()
{
    return ((m_StSignal2 & MASK_SIGNAL2_GATE_ALL_CLOSE) != 0x0000);
}

bool CMote::isGateDown200()
{
    return ((m_StSignal2 & MASK_SIGNAL2_GATE_DOWN_200MM) != 0x0000);
}

bool CMote::isGateDown300()
{
    return ((m_StSignal2 & MASK_SIGNAL2_GATE_DOWN_300MM) != 0x0000);
}

bool CMote::isGateUpDownTimeout()
{
    return ((m_StSignal2 & MASK_SIGNAL2_GATE_UP_DOWN_TIMEOUT) != 0x0000);
}

bool CMote::isGateOpenSlowAlarm()
{
    return ((m_StSignal2 & MASK_SIGNAL2_GATE_OPEN_SLOW_ALARM) != 0x0000);
}

bool CMote::isGateOpenSlowStop()
{
    return ((m_StSignal2 & MASK_SIGNAL2_GATE_OPEN_SLOW_STOP) != 0x0000);
}

bool CMote::isPump1AutoCtrl()
{
    return ((m_StSignal1 & MASK_SIGNAL1_PUMP1_AUTO_CTRL) != 0x0000);
}

bool CMote::isPump1Run()
{
    return ((m_StSignal1 & MASK_SIGNAL1_PUMP1_RUN) != 0x0000);
}

bool CMote::isPump1Fault()
{
    return ((m_StSignal1 & MASK_SIGNAL1_PUMP1_FAULT) != 0x0000);
}

bool CMote::isPump1Mesh()
{
    return ((m_StSignal1 & MASK_SIGANL1_PUMP1_MESH) != 0x0000);
}

bool CMote::isPump1StartFailed()
{
    return ((m_StSignal1 & MASK_SIGNAL1_PUMP1_START_FAILED) != 0x0000);
}

bool CMote::isPump2AutoCtrl()
{
    return ((m_StSignal1 & MASK_SIGNAL1_PUMP2_AUTO_CTRL) != 0x0000);
}

bool CMote::isPump2Run()
{
    return ((m_StSignal1 & MASK_SIGNAL1_PUMP2_RUN) != 0x0000);
}

bool CMote::isPump2Fault()
{
    return ((m_StSignal1 & MASK_SIGNAL1_PUMP2_FAULT) != 0x0000);
}

bool CMote::isPump2Mesh()
{
    return ((m_StSignal1 & MASK_SIGANL1_PUMP2_MESH) != 0x0000);
}

bool CMote::isPump2StartFailed()
{
    return ((m_StSignal1 & MASK_SIGNAL1_PUMP2_START_FAILED) != 0x0000);
}

bool CMote::isSysPressHigh()
{
    return ((m_StSignal1 & MASK_SIGNAL1_SYS_PRESS_HIGH) != 0x0000);
}

bool CMote::isSysPressLow()
{
    return ((m_StSignal1 & MASK_SIGNAL1_SYS_PRESS_LOW) != 0x0000);
}

bool CMote::isCavityPipeBreak()
{
    return ((m_StSignal2 & MASK_SIGNAL2_CAVITY_PIPE_BREAK) != 0x0000);
}

bool CMote::isCavityPressOver()
{
    return ((m_StSignal2 & MASK_SIGNAL2_CAVITY_PRESS_OVER) != 0x0000);
}

bool CMote::isSysPressConverterFault()
{
    return ((m_StSignal2 & MASK_SIGNAL2_SYS_PRESS_CONVERTER_FAULT) != 0x0000);
}

bool CMote::isOilLevelHigh()
{
    return ((m_StSignal2 & MASK_SIGNAL2_OIL_LEVEL_HIGH) != 0x0000);
}

bool CMote::isOilLevelLow()
{
    return ((m_StSignal2 & MASK_SIGNAL2_OIL_LEVEL_LOW) != 0x0000);
}

bool CMote::isOilTempHigh()
{
    return ((m_StSignal2 & MASK_SIGNAL2_OIL_TEMP_HIGH) != 0x0000);
}

bool CMote::isOilTempLow()
{
    return ((m_StSignal2 & MASK_SIGNAL2_OIL_TEMP_LOW) != 0x0000);
}

bool CMote::isOilFilterFault()
{
    return ((m_StSignal2 & MASK_SIGNAL2_OIL_FILTER_FAULT) != 0x0000);
}

bool CMote::isRemoteConnected()
{
    return m_ModbusTcpClient->IsConnected();
}

bool CMote::isOilTempTransmitterFault()
{
    return ((m_StSignal2 & MASK_SIGNAL2_OIL_TEMP_TRANSMITTER_FAULT) != 0x0000);
}

bool CMote::isLeftCavityPipeBreak()
{
    return ((m_StSignal2 & MASK_SIGNAL2_LEFT_CAVITY_PIPE_BREAK) != 0x0000);
}

bool CMote::isRightCavityPipeBreak()
{
    return ((m_StSignal2 & MASK_SIGNAL2_RIGHT_CAVITY_PIPE_BREAK) != 0x0000);
}

bool CMote::isNoPoleCavityPressHigh()
{
    return ((m_StSignal3 & MASK_SIGNAL3_NO_POLE_CAVITY_PRESS_HIGH) != 0x0000);
}

bool CMote::isCtrlPower1Ok()
{
    return ((m_StSignal3 & MASK_SIGNAL3_CTRL_POWER1_OK) != 0x0000);
}

bool CMote::isCtrlPower2Ok()
{
    return ((m_StSignal3 & MASK_SIGNAL3_CTRL_POWER2_OK) != 0x0000);
}

bool CMote::isElectromagneticPowerOk()
{
    return ((m_StSignal3 & MASK_SIGNAL3_ELECTROMAGNETIC_POWER_OK) != 0x0000);
}

bool CMote::isOilHeaterOn()
{
    return ((m_StSignal3 & MASK_SIGNAL3_OIL_HEATER_ON) != 0x0000);
}

bool CMote::isGateDeviationOver()
{
    return ((m_StSignal3 & MASK_SIGNAL3_GATE_DEVIATION_OVER) != 0x0000);
}

bool CMote::isGateOn()
{
    return ((m_StSignal3 & MASK_SIGNAL3_GATE_ON) != 0x0000);
}

bool CMote::isGateOff()
{
    return ((m_StSignal3 & MASK_SIGNAL3_GATE_OFF) != 0x0000);
}

bool CMote::add()
{
    QString sqlStr = "SELECT MAX(mote_id) FROM t_mote;";
    QSqlQuery dbQuery;
    dbQuery.exec(sqlStr);
    if(dbQuery.next())
        m_MoteId = dbQuery.value(0).toInt() + 1;
    else
        m_MoteId = 1;

    sqlStr = "INSERT INTO t_mote VALUES("
                + QString::number(m_MoteId) + ", '"
                + m_Name + "', '"
                + m_IpAddr + "', "
                + QString::number(m_Port) + ", "
                + QString::number(m_GroupNum) + ", "
                + QString::number(m_ScanIntervalInMS) + ", "
                + QString::number(m_MaxOpenVal) + ", "
                + QString::number(m_CfgOpenVal) + ", "
                + QString::number(m_Type) + ");";

    bool ret;
    if(QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions))
    {
        QSqlDatabase::database().transaction();
        ret = dbQuery.exec(sqlStr);
        if(ret)
            QSqlDatabase::database().commit();
        else
            QSqlDatabase::database().rollback();
    }
    else
    {
        ret = dbQuery.exec(sqlStr);
    }

    return ret;
}

bool CMote::del()
{
    QString sqlStr = "DELETE FROM t_mote WHERE mote_id="
                    + QString::number(m_MoteId) + ";";

    bool ret = false;
    QSqlQuery dbQuery;
    if(QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions))
    {
        QSqlDatabase::database().transaction();
        ret = dbQuery.exec(sqlStr);
        if(ret)
            QSqlDatabase::database().commit();
        else
            QSqlDatabase::database().rollback();
    }
    else
    {
        ret = dbQuery.exec(sqlStr);
    }

    return ret;
}

bool CMote::upd()
{
    QString sqlStr = "UPDATE t_mote SET name='" + m_Name
                    + "', ip_addr='" + m_IpAddr
                    + "', port=" + QString::number(m_Port)
                    + ", group_num=" + QString::number(m_GroupNum)
                    + ", scan_interval=" + QString::number(m_ScanIntervalInMS)
                    + ", max_open_value=" + QString::number(m_MaxOpenVal)
                    + ", cfg_open_value=" + QString::number(m_CfgOpenVal)
                    + ", type=" + QString::number(m_Type)       ///2014
                    + " WHERE mote_id=" + QString::number(m_MoteId)
                    + ";";

    bool ret = false;
    QSqlQuery dbQuery;
    if(QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions))
    {
        QSqlDatabase::database().transaction();
        ret = dbQuery.exec(sqlStr);
        if(ret)
            QSqlDatabase::database().commit();
        else
            QSqlDatabase::database().rollback();
    }
    else
    {
        ret = dbQuery.exec(sqlStr);
    }

    return ret;
}

bool CMote::log()
{
    int state_id;
    QString sqlStr = "SELECT MAX(state_id) FROM t_motestatelogs;";
    QSqlQuery dbQuery;
    dbQuery.exec(sqlStr);
    if(dbQuery.next())
        state_id = dbQuery.value(0).toInt() + 1;
    else
        state_id = 1;

    sqlStr = "INSERT INTO t_motestatelogs VALUES("
            + QString::number(state_id) + ", "
            + QString::number(m_MoteId) + ", '"
            + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "', "
            + QString::number(m_CurrOpenVal) + ", "
            + QString::number(m_CurrCfgOpenVal) + ", "
            + (this->isGateAutoCtrl() ? "1" : "0") + ", "
            + (this->isGateRemoteCtrl() ? "1" : "0") + ", "
            + (this->isGateAllOpen() ? "1" : "0") + ", "
            + (this->isGateAllClose() ? "1" : "0") + ", "
            + (this->isGateOpenSlowAlarm() ? "1" : "0") + ", "
            + (this->isGateOpenSlowStop() ? "1" : "0") + ", "
            + (this->isGateUpDownTimeout() ? "1" : "0") + ", "
            + (this->isGateDown200() ? "1" : "0") + ", "
            + (this->isGateDown300() ? "1" : "0") + ", "
            + (this->isPump1AutoCtrl() ? "1" : "0") + ", "
            + (this->isPump1Fault() ? "1" : "0") + ", "
            + (this->isPump1Mesh() ? "1" : "0") + ", "
            + (this->isPump1Run() ? "1" : "0") + ", "
            + (this->isPump1StartFailed() ? "1" : "0") + ", "
            + (this->isPump2AutoCtrl() ? "1" : "0") + ", "
            + (this->isPump2Fault() ? "1" : "0") + ", "
            + (this->isPump2Mesh() ? "1" : "0") + ", "
            + (this->isPump2Run() ? "1" : "0") + ", "
            + (this->isPump2StartFailed() ? "1" : "0") + ", "
            + QString::number(m_CurrSysPress) + ", "
            + (this->isSysPressHigh() ? "1" : "0") + ", "
            + (this->isSysPressLow() ? "1" : "0") + ", "
            + (this->isCavityPipeBreak() ? "1" : "0") + ", "
            + (this->isCavityPressOver() ? "1" : "0") + ", "
            + (this->isSysPressConverterFault() ? "1" : "0") + ", "
            + QString::number(m_CurrOilTravel) + ", "
            + (this->isOilFilterFault() ? "1" : "0") + ", "
            + (this->isOilLevelHigh() ? "1" : "0") + ", "
            + (this->isOilLevelLow() ? "1" : "0") + ", "
            + (this->isOilTempHigh() ? "1" : "0") + ", "
            + (this->isOilTempLow() ? "1" : "0") + ", "
            + QString::number(m_CurrStateSignal1) + ", "
            + QString::number(m_CurrStateSignal2) + ", "
            + (this->isLeftCavityPipeBreak() ? "1" : "0") + ", "
            + (this->isRightCavityPipeBreak() ? "1" : "0") + ", "
            + (this->isOilTempTransmitterFault() ? "1" : "0") + ", "
            + (this->isNoPoleCavityPressHigh() ? "1" : "0") + ", "
            + (this->isCtrlPower1Ok() ? "1" : "0") + ", "
            + (this->isCtrlPower2Ok() ? "1" : "0") + ", "
            + (this->isElectromagneticPowerOk() ? "1" : "0") + ", "
            + (this->isOilHeaterOn() ? "1" : "0") + ", "
            + (this->isGateDeviationOver() ? "1" : "0") + ", "
            + (this->isGateOn() ? "1" : "0") + ", "
            + (this->isGateOff() ? "1" : "0") + ", "
            + QString::number(m_StOilTemp) + ", "
            + QString::number(m_StGateDownVal) + ", "
            + QString::number(m_StGateDeviation) + ", "
            + QString::number(m_StGateLeftTravel) + ", "
            + QString::number(m_StGateRightTravel) + ", "
            + QString::number(m_StSignal3) + ");";

    bool ret;
    if(QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions))
    {
        QSqlDatabase::database().transaction();
        ret = dbQuery.exec(sqlStr);
        if(ret)
            QSqlDatabase::database().commit();
        else
            QSqlDatabase::database().rollback();
    }
    else
    {
        ret = dbQuery.exec(sqlStr);
    }

    return ret;
}

void CMote::getRemoteStateReg()
{
    m_RemoteCommandList.append(CMD_READ_STATE_REGISTER);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_SIGNAL1, 3);///2014
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::getRemoteSysPress()
{
    m_RemoteCommandList.append(CMD_READ_SYSTEM_PRESS);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_SYSTEM_PRESS, 1);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::getRemoteGateOpenVal()
{
    m_RemoteCommandList.append(CMD_READ_GATE_OPEN_VALUE);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_GATE_OPEN_VALUE, 1);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::getRemoteOilTravel()
{
    m_RemoteCommandList.append(CMD_READ_GATE_OIL_TRAVEL);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_GATE_OIL_TRAVEL, 1);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::getRemoteGateOpenCfgVal()
{
    m_RemoteCommandList.append(CMD_READ_GATE_OPEN_CONFIG_VALUE);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_GATE_OPEN_CONFIG_VALUE, 1);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::setRemoteGateOpenCfgVal()
{
    m_GateCfgOpenVal = (unsigned short)m_CfgOpenVal;
    m_RemoteCommandList.append(CMD_WRITE_GATE_OPEN_CONFIG_VALUE);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::WriteSingleRegister, REG_GATE_OPEN_CONFIG_VALUE, m_GateCfgOpenVal);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::getRemoteOilTemp()
{
    m_RemoteCommandList.append(CMD_READ_OIL_TEMP);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_OIL_TEMP, 1);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::getRemoteGateDownVal()
{
    m_RemoteCommandList.append(CMD_READ_GATE_DOWN_VALUE);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_GATE_DOWN_VALUE, 1);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::getRemoteGateDeviation()
{
    m_RemoteCommandList.append(CMD_READ_GATE_DEVIATION);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_GATE_DEVIATION, 1);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::getRemoteGateLeftTravel()
{
    m_RemoteCommandList.append(CMD_READ_GATE_LEFT_OIL_TRAVEL);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_GATE_LEFT_OIL_TRAVEL, 1);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::getRemoteGateRightTravel()
{
    m_RemoteCommandList.append(CMD_READ_GATE_RIGHT_OIL_TRAVEL);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::ReadHoldingRegisters, REG_STATE_GATE_RIGHT_OIL_TRAVEL, 1);
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::openRemoteGate()
{
    m_RemoteCommandList.append(CMD_OPEN_GATE_UP);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::WriteSingleRegister, REG_GATE_OPERATION, (m_GateOper | MASK_GATEOPER_OPEN));
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::stopRemoteGate()
{
    m_RemoteCommandList.append(CMD_STOP_GATE_UP);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::WriteSingleRegister, REG_GATE_OPERATION, (m_GateOper | MASK_GATEOPER_STOP));
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::closeRemoteGate()
{
    m_RemoteCommandList.append(CMD_CLOSE_GATE_UP);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::WriteSingleRegister, REG_GATE_OPERATION, (m_GateOper | MASK_GATEOPER_CLOSE));
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::resetRemoteGate()
{
    m_RemoteCommandList.append(CMD_RESET_SYSTEM_UP);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::WriteSingleRegister, REG_GATE_OPERATION, (m_GateOper | MASK_GATEOPER_RST));
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::connectToRemote()
{
    m_ModbusTcpClient = new CModbusTcpClient();
    m_ModbusTcpClient->ConnectToServer(m_IpAddr, m_Port);
    connect(m_ModbusTcpClient, SIGNAL(ConfirmListNotEmpty()), this, SLOT(receiveConfirm()));
    connect(m_ModbusTcpClient, SIGNAL(ConnectionStateChange()), this, SIGNAL(updateState()));
}

void CMote::disconnectFromRemote()
{
    m_ModbusTcpClient->CloseConnection();
    delete m_ModbusTcpClient;
}

void CMote::receiveConfirm()
{
    while(!m_ModbusTcpClient->IsConfirmListEmpty())
    {
        CModbusTcpFrame *confirm = m_ModbusTcpClient->GetConfirm();
        if(confirm == NULL)
            continue;

        if(m_RemoteCommandList.isEmpty())
            continue;
        int cmd = m_RemoteCommandList.first();
        m_RemoteCommandList.removeFirst();
        switch(cmd)
        {
            case CMD_READ_STATE_REGISTER:
                {
                int length = confirm->getDataLength();
                if(length != 6)
                    continue;
                QByteArray data(confirm->getData());
                m_StSignal1 = data.mid(0, 2).toHex().toUShort(0, 16);
                m_StSignal2 = data.mid(2, 2).toHex().toUShort(0, 16);
                m_StSignal3 = data.mid(4, 2).toHex().toUShort(0, 16);
                }
                break;

            case CMD_WRITE_GATE_OPEN_CONFIG_VALUE:
                break;
            case CMD_READ_SYSTEM_PRESS:
                {
                int length = confirm->getDataLength();
                if(length != 2)
                    continue;
                QByteArray data(confirm->getData());
                m_StSysPress = data.mid(0, 2).toHex().toUShort(0, 16);
                }
                break;
            case CMD_READ_GATE_OPEN_VALUE:
                {
                int length = confirm->getDataLength();
                if(length != 2)
                    continue;
                QByteArray data(confirm->getData());
                m_StGateOpenVal = data.mid(0, 2).toHex().toUShort(0, 16);
                }
                break;
            case CMD_READ_GATE_OIL_TRAVEL:
                {
                int length = confirm->getDataLength();
                if(length != 2)
                    continue;
                QByteArray data(confirm->getData());
                m_StOilTravel = data.mid(0, 2).toHex().toUShort(0, 16);
                }
                break;
            case CMD_READ_GATE_OPEN_CONFIG_VALUE:
                {
                int length = confirm->getDataLength();
                if(length != 2)
                    continue;
                QByteArray data(confirm->getData());
                m_StCfgOpenVal = data.mid(0, 2).toHex().toUShort(0, 16);
                }
                break;
            case CMD_OPEN_GATE_UP:
                m_OpenGateTimer->start();
                break;
            case CMD_STOP_GATE_UP:
                m_StopGateTimer->start();
                break;
            case CMD_CLOSE_GATE_UP:
                m_CloseGateTimer->start();
                break;
            case CMD_RESET_SYSTEM_UP:
                m_ResetGateTimer->start();
                break;
            case CMD_READ_OIL_TEMP:
                {
                int length = confirm->getDataLength();
                if(length != 2)
                    continue;
                QByteArray data(confirm->getData());
                m_StOilTemp = data.mid(0, 2).toHex().toUShort(0, 16);
                }
                break;
            case CMD_READ_GATE_DOWN_VALUE:
                {
                int length = confirm->getDataLength();
                if(length != 2)
                    continue;
                QByteArray data(confirm->getData());
                m_StGateDownVal = data.mid(0, 2).toHex().toUShort(0, 16);
                }
                break;
            case CMD_READ_GATE_DEVIATION:
                {
                int length = confirm->getDataLength();
                if(length != 2)
                    continue;
                QByteArray data(confirm->getData());
                m_StGateDeviation = data.mid(0, 2).toHex().toUShort(0, 16);
                }
                break;
            case CMD_READ_GATE_LEFT_OIL_TRAVEL:
                {
                int length = confirm->getDataLength();
                if(length != 2)
                    continue;
                QByteArray data(confirm->getData());
                m_StGateLeftTravel = data.mid(0, 2).toHex().toUShort(0, 16);
                }
                break;
            case CMD_READ_GATE_RIGHT_OIL_TRAVEL:
                {
                int length = confirm->getDataLength();
                if(length != 2)
                    continue;
                QByteArray data(confirm->getData());
                m_StGateRightTravel = data.mid(0, 2).toHex().toUShort(0, 16);
                }
                break;
            case CMD_OPEN_GATE_DOWN:
            case CMD_STOP_GATE_DOWN:
            case CMD_CLOSE_GATE_DOWN:
            case CMD_RESET_SYSTEM_DOWN:
                break;

            default:
                break;
        }//end switch
    }//end while

    bool isStateChange = false;
    if(m_CurrOpenVal != m_StGateOpenVal)
    {
        m_CurrOpenVal = m_StGateOpenVal;
        isStateChange = true;
    }
    if(m_CurrCfgOpenVal != m_StCfgOpenVal)
    {
        m_CurrCfgOpenVal = m_StCfgOpenVal;
        isStateChange = true;
    }
    if(m_CurrSysPress != m_StSysPress)
    {
        m_CurrSysPress = m_StSysPress;
        isStateChange = true;
    }
    if(m_CurrOilTravel != m_StOilTravel)
    {
        m_CurrOilTravel = m_StOilTravel;
        isStateChange = true;
    }
    if(m_CurrStateSignal1 != m_StSignal1)
    {
        m_CurrStateSignal1 = m_StSignal1;
        isStateChange = true;
    }
    if(m_CurrStateSignal2 != m_StSignal2)
    {
        m_CurrStateSignal2 = m_StSignal2;
        isStateChange = true;
    }
    if(m_CurrStateSignal3 != m_StSignal3)
    {
        m_CurrStateSignal3 = m_StSignal3;
        isStateChange = true;
    }
    if(m_CurrOilTemp != m_StOilTemp)
    {
        m_CurrOilTemp = m_StOilTemp;
        isStateChange = true;
    }
    if(m_CurrGateDownVal != m_StGateDownVal)
    {
        m_CurrGateDownVal = m_StGateDownVal;
        isStateChange = true;
    }
    if(m_CurrGateDeviation != m_StGateDeviation)
    {
        m_CurrGateDeviation = m_StGateDeviation;
        isStateChange = true;
    }
    if(m_CurrGateLeftTravel != m_StGateLeftTravel)
    {
        m_CurrGateLeftTravel = m_StGateLeftTravel;
        isStateChange = true;
    }
    if(m_CurrGateRightTravel != m_StGateRightTravel)
    {
        m_CurrGateRightTravel = m_StGateRightTravel;
        isStateChange = true;
    }

    if(isStateChange)
    {
        log();
        emit updateState();
    }
}

void CMote::openGateTimeout()
{
    m_RemoteCommandList.append(CMD_OPEN_GATE_DOWN);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::WriteSingleRegister, REG_GATE_OPERATION, (m_GateOper & (~MASK_GATEOPER_OPEN)));
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::stopGateTimeout()
{
    m_RemoteCommandList.append(CMD_STOP_GATE_DOWN);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::WriteSingleRegister, REG_GATE_OPERATION, (m_GateOper & (~MASK_GATEOPER_STOP)));
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::closeGateTimeout()
{
    m_RemoteCommandList.append(CMD_CLOSE_GATE_DOWN);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::WriteSingleRegister, REG_GATE_OPERATION, (m_GateOper & (~MASK_GATEOPER_CLOSE)));
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::resetGateTimeout()
{
    m_RemoteCommandList.append(CMD_RESET_SYSTEM_DOWN);
    CModbusTcpFrame request(REMOTE_UNIT_ID, CModbusTcpFrame::WriteSingleRegister, REG_GATE_OPERATION, (m_GateOper & (~MASK_GATEOPER_RST)));
    m_ModbusTcpClient->SendRequest(request);
}

void CMote::scanRemote()
{
    getRemoteStateReg();
    getRemoteSysPress();
    getRemoteGateOpenVal();
    getRemoteOilTravel();
    getRemoteGateOpenCfgVal();
    getRemoteOilTemp();
    getRemoteGateDownVal();
    getRemoteGateDeviation();
    getRemoteGateLeftTravel();
    getRemoteGateRightTravel();
}


