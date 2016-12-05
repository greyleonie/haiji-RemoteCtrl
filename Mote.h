#ifndef MOTE_H
#define MOTE_H

#include <QString>
#include <QtSql>
#include <QTimer>
#include <memory>
//#include <QtSql/QSqlQuery>
#include "ModbusTcpClient.h"
#include "ModbusTcpFrame.h"

class CMote : public QObject
{
    Q_OBJECT

public:
    enum StateRegisterMask
    {
        MASK_GATEOPER_OPEN = 0x0001,        //I1
        MASK_GATEOPER_STOP = 0x0002,        //I2
        MASK_GATEOPER_CLOSE = 0x0004,       //I3
        MASK_GATEOPER_RST = 0x0008,         //I4

        MASK_SIGNAL1_GATE_AUTO_CTRL = 0x0001,            //I1
        MASK_SIGNAL1_GATE_REMOTE_CTRL = 0x0002,          //I2
        MASK_SIGNAL1_PUMP1_AUTO_CTRL= 0x0004,       //I3
        MASK_SIGNAL1_PUMP1_RUN = 0x0008,            //I4
        MASK_SIGNAL1_PUMP1_FAULT = 0x0010,          //I5
        MASK_SIGANL1_PUMP1_MESH = 0x0020,           //I6
        MASK_SIGNAL1_PUMP1_START_FAILED = 0x0040,   //I7
        MASK_SIGNAL1_PUMP2_AUTO_CTRL = 0x0100,      //I9
        MASK_SIGNAL1_PUMP2_RUN = 0x0200,            //I10
        MASK_SIGNAL1_PUMP2_FAULT = 0x0400,          //I11
        MASK_SIGANL1_PUMP2_MESH = 0x0800,           //I12
        MASK_SIGNAL1_PUMP2_START_FAILED = 0x1000,   //I13
        MASK_SIGNAL1_SYS_PRESS_HIGH = 0x4000,       //I15
        MASK_SIGNAL1_SYS_PRESS_LOW = 0x8000,        //I16

        MASK_SIGNAL2_CAVITY_PIPE_BREAK = 0x0001,    //I17
        MASK_SIGNAL2_CAVITY_PRESS_OVER = 0x0002,    //I18
        MASK_SIGNAL2_LEFT_CAVITY_PIPE_BREAK = 0x0001,    //I17
        MASK_SIGNAL2_RIGHT_CAVITY_PIPE_BREAK = 0x0002,    //I18
        MASK_SIGNAL2_OIL_LEVEL_HIGH = 0x0004,       //I19
        MASK_SIGNAL2_OIL_LEVEL_LOW = 0x0008,        //I20
        MASK_SIGNAL2_OIL_TEMP_HIGH = 0x0010,        //I21
        MASK_SIGNAL2_OIL_TEMP_LOW = 0x0020,         //I22
        MASK_SIGNAL2_OIL_FILTER_FAULT = 0x0040,         //I23
        MASK_SIGNAL2_GATE_DOWN_200MM = 0x0080,      //I24
        MASK_SIGNAL2_GATE_DOWN_300MM = 0x0100,      //I25
        MASK_SIGNAL2_GATE_UP_DOWN_TIMEOUT = 0x0200, //I26
        MASK_SIGNAL2_GATE_OPEN_SLOW_ALARM = 0x0400, //I27
        MASK_SIGNAL2_GATE_OPEN_SLOW_STOP = 0x0800,  //I28
        MASK_SIGNAL2_GATE_ALL_OPEN = 0x1000,        //I29
        MASK_SIGNAL2_GATE_ALL_CLOSE = 0x2000,       //I30
        MASK_SIGNAL2_SYS_PRESS_CONVERTER_FAULT = 0x4000, //I31
        MASK_SIGNAL2_OIL_TEMP_TRANSMITTER_FAULT = 0x8000, //I32

        MASK_SIGNAL3_NO_POLE_CAVITY_PRESS_HIGH = 0x0001,    //I33
        MASK_SIGNAL3_CTRL_POWER1_OK = 0x0002,               //I34
        MASK_SIGNAL3_CTRL_POWER2_OK = 0x0004,               //I35
        MASK_SIGNAL3_ELECTROMAGNETIC_POWER_OK = 0x0008,     //I36
        MASK_SIGNAL3_OIL_HEATER_ON = 0x0010,                //I37
        MASK_SIGNAL3_GATE_DEVIATION_OVER = 0x0020,          //I38
        MASK_SIGNAL3_GATE_ON = 0x0040,                      //I39
        MASK_SIGNAL3_GATE_OFF = 0x0080                      //I40

    };

    enum Registers
    {
        REG_STATE_SIGNAL1 = 601, //400601,
        REG_STATE_SIGNAL2 = 602, //400602,
        REG_STATE_SIGNAL3 = 603, //400603,
        REG_STATE_SYSTEM_PRESS = 605, //400605,
        REG_STATE_GATE_OPEN_VALUE = 606, //400606,
        REG_STATE_GATE_OIL_TRAVEL = 607, //400607,
        REG_STATE_GATE_OPEN_CONFIG_VALUE = 608, //400608,
        REG_GATE_OPEN_CONFIG_VALUE = 615, //400615,
        REG_GATE_OPERATION = 616, //400616
        REG_STATE_OIL_TEMP = 609, //400609
        REG_STATE_GATE_DOWN_VALUE = 610, //400610
        REG_STATE_GATE_DEVIATION = 611, //400611
        REG_STATE_GATE_LEFT_OIL_TRAVEL = 607, //400607
        REG_STATE_GATE_RIGHT_OIL_TRAVEL = 612 //400612
    };

    enum MoteID
    {
        REMOTE_UNIT_ID = 1
    };

    enum RemoteCommand
    {
        CMD_READ_STATE_REGISTER = 1,
        CMD_READ_SYSTEM_PRESS = 2,
        CMD_READ_GATE_OPEN_VALUE = 3,
        CMD_READ_GATE_OIL_TRAVEL = 4,
        CMD_READ_GATE_OPEN_CONFIG_VALUE = 5,
        CMD_WRITE_GATE_OPEN_CONFIG_VALUE = 6,
        CMD_OPEN_GATE_UP = 7,
        CMD_STOP_GATE_UP = 8,
        CMD_CLOSE_GATE_UP = 9,
        CMD_RESET_SYSTEM_UP = 10,
        CMD_OPEN_GATE_DOWN = 11,
        CMD_STOP_GATE_DOWN = 12,
        CMD_CLOSE_GATE_DOWN = 13,
        CMD_RESET_SYSTEM_DOWN = 14,
        CMD_READ_OIL_TEMP = 15,
        CMD_READ_GATE_DOWN_VALUE = 16,
        CMD_READ_GATE_DEVIATION = 17,
        CMD_READ_GATE_LEFT_OIL_TRAVEL = 18,
        CMD_READ_GATE_RIGHT_OIL_TRAVEL = 19
    };

    CMote(int id = 0,
          QString name = "",
          QString ipAddr = "127.0.0.1",
          int port = 502,
          int group = 0,
          int open = 0,
          int maxOpen = 32767,
          int interval = 1000,
          int type = 0);

    ~CMote();

    void setMoteId(int id);
    void setType(int type);
    void setName(QString name);
    void setIpAddr(QString ipAddr);
    void setPort(int port);
    void setGroupNum(int group);
    void setCfgOpenVal(int open);
    void setMaxOpenVal(int maxOpen);
    void setScanInterval(int intervalInSec);

    int getMoteId();
    int getType();
    QString getName();
    QString getIpAddr();
    int getPort();
    int getGroupNum();
    int getCfgOpenVal();
    int getMaxOpenVal();
    int getScanInterval();

    bool add();
    bool del();
    bool upd();
    bool log();


    void connectToRemote();
    void disconnectFromRemote();
    void openRemoteGate();
    void stopRemoteGate();
    void closeRemoteGate();
    void resetRemoteGate();
    void setRemoteGateOpenCfgVal();
    void getRemoteStateReg();
    void getRemoteSysPress();
    void getRemoteGateOpenVal();
    void getRemoteOilTravel();
    void getRemoteGateOpenCfgVal();
    void getRemoteOilTemp();
    void getRemoteGateDownVal();
    void getRemoteGateDeviation();
    void getRemoteGateLeftTravel();
    void getRemoteGateRightTravel();


    int getCurrSysPress();
    int getCurrGateOpenVal();
    int getCurrOilTravel();
    int getCurrGateCfgOpenVal();
    int getCurrOilTemp();
    int getCurrGateDownVal();
    int getCurrGateDeviation();
    int getCurrGateLeftTravel();
    int getCurrGateRightTravel();

    bool isGateAutoCtrl();
    bool isGateRemoteCtrl();
    bool isGateAllOpen();
    bool isGateAllClose();
    bool isGateDown200();
    bool isGateDown300();
    bool isGateUpDownTimeout();
    bool isGateOpenSlowAlarm();
    bool isGateOpenSlowStop();

    bool isPump1AutoCtrl();
    bool isPump1Run();
    bool isPump1Fault();
    bool isPump1Mesh();
    bool isPump1StartFailed();
    bool isPump2AutoCtrl();
    bool isPump2Run();
    bool isPump2Fault();
    bool isPump2Mesh();
    bool isPump2StartFailed();

    bool isSysPressHigh();
    bool isSysPressLow();
    bool isCavityPipeBreak();
    bool isCavityPressOver();
    bool isSysPressConverterFault();

    bool isOilLevelHigh();
    bool isOilLevelLow();
    bool isOilTempHigh();
    bool isOilTempLow();
    bool isOilFilterFault();

    bool isRemoteConnected();

    bool isOilTempTransmitterFault();
    bool isLeftCavityPipeBreak();
    bool isRightCavityPipeBreak();
    bool isNoPoleCavityPressHigh();
    bool isCtrlPower1Ok();
    bool isCtrlPower2Ok();
    bool isElectromagneticPowerOk();
    bool isOilHeaterOn();
    bool isGateDeviationOver();
    bool isGateOn();
    bool isGateOff();


signals:
    void updateState();

private slots:
    void receiveConfirm();
    void openGateTimeout();
    void stopGateTimeout();
    void closeGateTimeout();
    void resetGateTimeout();
    void scanRemote();

private:
    int     m_MoteId;
    int     m_Type;
    QString m_Name;
    QString m_IpAddr;
    int     m_Port;
    int     m_GroupNum;
    int     m_CfgOpenVal;
    int     m_MaxOpenVal;
    int     m_ScanIntervalInMS;
    int     m_CurrSysPress;
    int     m_CurrOpenVal;
    int     m_CurrOilTravel;
    int     m_CurrCfgOpenVal;
    int     m_CurrStateSignal1;
    int     m_CurrStateSignal2;
    int     m_CurrStateSignal3;
    int     m_CurrOilTemp;
    int     m_CurrGateDownVal;
    int     m_CurrGateDeviation;
    int     m_CurrGateLeftTravel;
    int     m_CurrGateRightTravel;

    unsigned short      m_StSignal1;        //400601
    unsigned short      m_StSignal2;        //400602
    unsigned short      m_StSysPress;       //400605
    unsigned short      m_StGateOpenVal;    //400606
    unsigned short      m_StOilTravel;      //400607
    unsigned short      m_StCfgOpenVal;     //400608
    unsigned short      m_GateCfgOpenVal;   //400615
    unsigned short      m_GateOper;         //400616
    unsigned short      m_StSignal3;        //400603
    unsigned short      m_StOilTemp;        //400609
    unsigned short      m_StGateDownVal;    //400610
    unsigned short      m_StGateDeviation;  //400611
    unsigned short      m_StGateLeftTravel; //400607
    unsigned short      m_StGateRightTravel;//400612

    CModbusTcpClient *m_ModbusTcpClient;
    QList<int> m_RemoteCommandList;

    QTimer  *m_OpenGateTimer;
    QTimer  *m_CloseGateTimer;
    QTimer  *m_StopGateTimer;
    QTimer  *m_ResetGateTimer;
    QTimer  *m_ScanTimer;

};

//typedef auto_ptr<CMote> CMotePtr;

#endif // MOTE_H
