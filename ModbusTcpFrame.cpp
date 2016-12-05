#include "ModbusTcpFrame.h"

QMap<int, QString> CModbusTcpFrame::initFunctionMap()
{
    QMap<int, QString> map;
    map[CModbusTcpFrame::ReadCoils] = "Read Coils";
    map[CModbusTcpFrame::ReadDiscreteInputs] = "Read Discrete Inputs";
    map[CModbusTcpFrame::ReadHoldingRegisters] = "Read Holding Registers";
    map[CModbusTcpFrame::ReadInputRegisters] = "Read Input Registers";
    map[CModbusTcpFrame::WriteSingleCoil] = "Write Single Coi";
    map[CModbusTcpFrame::WriteSingleRegister] = "Write Single Register";
    map[CModbusTcpFrame::WriteMultipleCoils] = "Write Multiple Coils";
    map[CModbusTcpFrame::WriteMultipleRegisters] = "Write Multiple registers";
    map[CModbusTcpFrame::ReadFileRecord] = "Read File Record";
    map[CModbusTcpFrame::WriteFileRecord] = "Write File Record";
    map[CModbusTcpFrame::MaskWriteRegister] = "Mask Write Register";
    map[CModbusTcpFrame::ReadWriteMultipleRegisters] = "Read/Write Multiple registers";
    map[CModbusTcpFrame::ReadFIFOQueue] = "Read FIFO Queue";
    map[CModbusTcpFrame::EncapsulatedInterfaceTransport] = "Encapsulated Interface Transport";

    return map;
}

QMap<int, QString> CModbusTcpFrame::m_FunctionMap = initFunctionMap();
unsigned short CModbusTcpFrame::m_StaticTransId = 0;
//*/
CModbusTcpFrame::CModbusTcpFrame()
{
    ;
}

CModbusTcpFrame::CModbusTcpFrame(const QByteArray &data)
{
    if(data.size() < 6 || data.size() < data.mid(4, 2).toHex().toUShort(0, 16) + 6 )
        return;

    m_TransactionId = data.mid(0, 2).toHex().toUShort(0, 16);
    m_ProtocolId = data.mid(2, 2).toHex().toUShort(0, 16);
    m_MBAPLength = data.mid(4, 2).toHex().toUShort(0, 16);
    m_UnitId = data[6];

    m_Frame = data.mid(0, m_MBAPLength + 6);
    m_FunctionCode = data[7];
    if((m_FunctionCode & 0x80) == 0x00) //FunctionCode
    {
        switch(m_FunctionCode)
        {
            case ReadCoils:
            case ReadDiscreteInputs:
            case ReadHoldingRegisters:
            case ReadInputRegisters:
            case ReadWriteMultipleRegisters:
                m_StartAddress = 0xFFFF;
                m_CountOrData = 0;
                m_DataLength = data[8];
                m_Data = data.mid(9, m_DataLength);
                break;

            case WriteSingleCoil:
            case WriteSingleRegister:
                m_StartAddress = data.mid(8, 2).toHex().toUShort(0, 16);
                m_CountOrData = data.mid(10, 2).toHex().toUShort(0, 16);
                m_DataLength = 0;
                break;

            case WriteMultipleCoils:
            case WriteMultipleRegisters:
                m_StartAddress = data.mid(8, 2).toHex().toUShort(0, 16);
                m_CountOrData = data.mid(10, 2).toHex().toUShort(0, 16);
                m_DataLength = 0;
                break;

            case ReadFileRecord:
            case WriteFileRecord:
            case MaskWriteRegister:
            case ReadFIFOQueue:
            case EncapsulatedInterfaceTransport:
            default:
                break;
        }//end switch
    }//end if
    else //ExceptionCode
    {
        unsigned char excode = (m_FunctionCode & 0x7F);
        switch(excode)
        {
            case IllegalFunction:
                break;
            case IllegalDataAddress:
                break;
            case IllegalDataValue:
                break;
            case SlaveDeviceFailure:
                break;
            case Acknowledge:
                break;
            case SlaveDeviceBusy:
                break;
            case MemoryParityError:
                break;
            case GatewayPathUnavailable:
                break;
            case GatewayTargetDeviceFailedToRespond:
                break;
            default:
                break;
        }//end switch
    }//end else



}

CModbusTcpFrame::CModbusTcpFrame(unsigned char unitId, unsigned char functionCode)
{
    m_TransactionId = m_StaticTransId++;
    m_ProtocolId = 0;
    m_MBAPLength = 2;
    m_UnitId = unitId;
    m_FunctionCode = functionCode;
    m_StartAddress = 0xFFFF;
    m_CountOrData = 0;
    m_DataLength = 0;

    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << (qint16)m_TransactionId
        << (qint16)m_ProtocolId
        << (qint16)m_MBAPLength
        << (qint8)m_UnitId
        << (qint8)m_FunctionCode;
}

CModbusTcpFrame::CModbusTcpFrame(unsigned char unitId, unsigned char functionCode, unsigned short startAddr, unsigned short regCntOrData)
{
    m_TransactionId = m_StaticTransId++;
    m_ProtocolId = 0;
    m_MBAPLength = 6;
    m_UnitId = unitId;
    m_FunctionCode = functionCode;
    m_StartAddress = startAddr;
    m_CountOrData = regCntOrData;
    m_DataLength = 0;

    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << (qint16)m_TransactionId
        << (qint16)m_ProtocolId
        << (qint16)m_MBAPLength
        << (qint8)m_UnitId
        << (qint8)m_FunctionCode
        << (qint16)m_StartAddress
        << (qint16)m_CountOrData;
}

CModbusTcpFrame::CModbusTcpFrame(unsigned char unitId, unsigned char functionCode, unsigned short startAddr, unsigned short registerCnt, QByteArray &data)
{
    m_TransactionId = m_StaticTransId++;
    m_ProtocolId = 0;
    m_MBAPLength = data.size() + 7;
    m_UnitId = unitId;
    m_FunctionCode = functionCode;
    m_StartAddress = startAddr;
    m_CountOrData = registerCnt;
    m_DataLength = data.size();
    m_Data = data;

    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << (qint16)m_TransactionId
        << (qint16)m_ProtocolId
        << (qint16)m_MBAPLength
        << (qint8)m_UnitId
        << (qint8)m_FunctionCode
        << (qint16)m_StartAddress
        << (qint16)m_CountOrData
        << (qint8)m_DataLength
        << m_Data;
}

CModbusTcpFrame::CModbusTcpFrame(const CModbusTcpFrame &copy)
{
    m_TransactionId = copy.m_TransactionId;
    m_ProtocolId = copy.m_ProtocolId;
    m_MBAPLength = copy.m_MBAPLength;
    m_UnitId = copy.m_UnitId;
    m_FunctionCode = copy.m_FunctionCode;
    m_StartAddress = copy.m_StartAddress;
    m_CountOrData = copy.m_CountOrData;
    m_DataLength = copy.m_DataLength;
    m_Data = copy.m_Data;
    m_Frame = copy.m_Frame;
}

CModbusTcpFrame::~CModbusTcpFrame()
{
    ;
}

void CModbusTcpFrame::setTransactionId(unsigned short transId)
{
    m_TransactionId = transId;
    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << (qint16)transId;
}

void CModbusTcpFrame::setProtocolId(unsigned short protocolId)
{
    m_ProtocolId = protocolId;
    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(2);
    out << (qint16)protocolId;
}

void CModbusTcpFrame::setMBAPLength(unsigned short length)
{
    m_MBAPLength = length;
    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(4);
    out << (qint16)length;
}

void CModbusTcpFrame::setUnitId(unsigned char unitId)
{
    m_UnitId = unitId;
    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(6);
    out << (qint8)unitId;
}

void CModbusTcpFrame::setFunctionCode(unsigned char functionCode)
{
    m_FunctionCode = functionCode;
    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(7);
    out << (qint8)functionCode;
}

void CModbusTcpFrame::setStartAddress(unsigned short startAddr)
{
    m_StartAddress = startAddr;
    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(8);
    out << (qint16)startAddr;
}

void CModbusTcpFrame::setRegCntOrData(unsigned short regCntOrData)
{
    m_CountOrData = regCntOrData;
    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(10);
    out << (qint16)regCntOrData;
}

void CModbusTcpFrame::setDataLength(unsigned char dataLength)
{
    m_DataLength = dataLength;
    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(12);
    out << (qint8)dataLength;
}

void CModbusTcpFrame::setData(QByteArray &data)
{
    m_Data = data;
    QDataStream out(&m_Frame, QIODevice::WriteOnly);
    out.device()->seek(13);
    out << data;
}

unsigned short CModbusTcpFrame::getTransactionId()
{
    return m_TransactionId;
}

unsigned short CModbusTcpFrame::getProtocolId()
{
    return m_ProtocolId;
}

unsigned short CModbusTcpFrame::getMBAPLength()
{
    return m_MBAPLength;
}

unsigned char CModbusTcpFrame::getUnitId()
{
    return m_UnitId;
}

unsigned char CModbusTcpFrame::getFunctionCode()
{
    return m_FunctionCode;
}

unsigned short CModbusTcpFrame::getStartAddress()
{
    return m_StartAddress;
}

unsigned short CModbusTcpFrame::getRegCntOrData()
{
    return m_CountOrData;
}

unsigned char CModbusTcpFrame::getDataLength()
{
    return m_DataLength;
}

QByteArray CModbusTcpFrame::getData()
{
    return m_Data;
}

QByteArray CModbusTcpFrame::getMBAPHead()
{
    return m_Frame.mid(0, 7);
}

int CModbusTcpFrame::length()
{
    return m_Frame.size();
}

QByteArray CModbusTcpFrame::toQByteArray()
{
    return m_Frame;
}

bool CModbusTcpFrame::isMatch(const CModbusTcpFrame &frame)
{
    if(m_TransactionId != frame.m_TransactionId)
        return false;
//    if(m_MBAPLength != frame.m_MBAPLength)
//        return false;
    if(m_UnitId != frame.m_UnitId)
        return false;
    if(m_FunctionCode != frame.m_FunctionCode)
        return false;

    switch(m_FunctionCode)
    {
        case ReadCoils:
        case ReadDiscreteInputs:
        case ReadHoldingRegisters:
        case ReadInputRegisters:
            break;

        case WriteSingleCoil:
        case WriteSingleRegister:
        case WriteMultipleCoils:
        case WriteMultipleRegisters:
            if(m_StartAddress != frame.m_StartAddress)
                return false;
            if(m_CountOrData != frame.m_CountOrData)
                return false;
            break;

        case ReadWriteMultipleRegisters:
            break;

        case ReadFileRecord:
        case WriteFileRecord:
        case MaskWriteRegister:
        case ReadFIFOQueue:
        case EncapsulatedInterfaceTransport:
        default:
            break;
    }//end switch

    return true;
}




