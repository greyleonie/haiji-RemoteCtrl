#ifndef MODBUSTCPFRAME_H
#define MODBUSTCPFRAME_H

#include <QByteArray>
#include <QDataStream>
#include <QMap>
#include <QString>
#include <memory>

//Functions List

//01 (0x01) Read Coils
//02 (0x02) Read Discrete Inputs
//03 (0x03) Read Holding Registers
//04 (0x04) Read Input Registers
//05 (0x05) Write Single Coil
//06 (0x06) Write Single Register
//15 (0x0F) Write Multiple Coils
//16 (0x10) Write Multiple registers
//20 (0x14) Read File Record										(To Do)
//21 (0x15) Write File Record										(To Do)
//22 (0x16) Mask Write Register										(To Do)
//23 (0x17) Read/Write Multiple registers							(To test)
//24 (0x18) Read FIFO Queue											(To Do)
//43 (0x2B) Encapsulated Interface Transport						(To Do)


class CModbusTcpFrame
{
public:
    enum FunctionCode
    {
        ReadCoils						= 1,
        ReadDiscreteInputs				= 2,
        ReadHoldingRegisters			= 3,
        ReadInputRegisters				= 4,
        WriteSingleCoil					= 5,
        WriteSingleRegister				= 6,
        WriteMultipleCoils				= 15,
        WriteMultipleRegisters			= 16,
        ReadFileRecord					= 20,
        WriteFileRecord					= 21,
        MaskWriteRegister				= 22,
        ReadWriteMultipleRegisters		= 23,
        ReadFIFOQueue					= 24,
        EncapsulatedInterfaceTransport	= 43,
    };

    enum ExceptionCode
    {
        IllegalFunction						= 1,
        IllegalDataAddress					= 2,
        IllegalDataValue					= 3,
        SlaveDeviceFailure					= 4,
        Acknowledge							= 5,
        SlaveDeviceBusy						= 6,
        MemoryParityError					= 7,
        GatewayPathUnavailable				= 10,
        GatewayTargetDeviceFailedToRespond	= 11
    };

    CModbusTcpFrame();
    CModbusTcpFrame(const QByteArray &data);
    CModbusTcpFrame(unsigned char unitId, unsigned char functionCode);
    CModbusTcpFrame(unsigned char unitId, unsigned char functionCode, unsigned short startAddr, unsigned short  regCntOrData);
    CModbusTcpFrame(unsigned char unitId, unsigned char functionCode, unsigned short startAddr, unsigned short  registerCnt, QByteArray &data);
    CModbusTcpFrame(const CModbusTcpFrame &copy);

    ~CModbusTcpFrame();

    void setTransactionId(unsigned short transId);
    void setProtocolId(unsigned short protocolId);
    void setMBAPLength(unsigned short length);
    void setUnitId(unsigned char unitId);
    void setFunctionCode(unsigned char functionCode);
    void setStartAddress(unsigned short startAddr);
    void setRegCntOrData(unsigned short regCntOrData);
    //void setSingleData(unsigned short singalData);
    void setDataLength(unsigned char dataLength);
    void setData(QByteArray &data);

    unsigned short getTransactionId();
    unsigned short getProtocolId();
    unsigned short getMBAPLength();
    unsigned char getUnitId();
    unsigned char getFunctionCode();
    unsigned short getStartAddress();
    unsigned short getRegCntOrData();
    //unsigned short getSingleData();
    unsigned char getDataLength();
    QByteArray getMBAPHead();
    QByteArray getData();

    QByteArray toQByteArray();
    int length();
    bool isMatch(const CModbusTcpFrame &frame);

private:
    static QMap<int, QString> initFunctionMap();
    static QMap<int, QString> m_FunctionMap;

    static unsigned short m_StaticTransId;

    unsigned short  m_TransactionId;
    unsigned short  m_ProtocolId;
    unsigned short  m_MBAPLength;
    unsigned char   m_UnitId;
    unsigned char   m_FunctionCode;
    unsigned short  m_StartAddress;
    unsigned short  m_CountOrData;
    unsigned short  m_RegisterQuantity;
    unsigned short  m_SingleData;
    unsigned char   m_DataLength;
    QByteArray      m_Data;

    QByteArray      m_Frame;
};

using namespace std;
typedef auto_ptr<CModbusTcpFrame> CModbusTcpFramePtr;

#endif // MODBUSTCPFRAME_H
