#include "ModbusTcpPacket.h"

unsigned short CModbusTcpPacket::m_StaticTransId = 0;

CModbusTcpPacket::CModbusTcpPacket()
{
    m_PacketHead.clear();
    m_ModbusFrame.clear();
}

CModbusTcpPacket::CModbusTcpPacket(QByteArray &data)
{
    m_PacketHead.clear();
    m_ModbusFrame.clear();

    if(data.size() < 6 || data.size() < data.mid(4, 2).toHex().toUShort(0, 16) + 6 )
        return;

    m_TransactionId = data.mid(0, 2).toHex().toUShort(0, 16);
    m_ProtocolId = data.mid(2, 2).toHex().toUShort(0, 16);
    m_Length = data.mid(4, 2).toHex().toUShort(0, 16);

    m_PacketHead = data.mid(0, 6);
    m_ModbusFrame = data.mid(6, m_Length);
}

CModbusTcpPacket::CModbusTcpPacket(CModbusFrame &frame)
{
    m_TransactionId = m_StaticTransId++;
    m_ProtocolId = 0;
    m_Length = frame.length();

    QDataStream out(&m_PacketHead, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << (qint16)m_TransactionId
        << (qint16)m_ProtocolId
        << (qint16)m_Length;

    m_ModbusFrame = frame.toQByteArray();
}

CModbusTcpPacket::~CModbusTcpPacket()
{
    ;
}

unsigned short CModbusTcpPacket::getTransactionId()
{
    return m_TransactionId;
}
unsigned short CModbusTcpPacket::getProtocolId()
{
    return m_ProtocolId;
}

unsigned short CModbusTcpPacket::getLength()
{
    return m_Length;
}

QByteArray CModbusTcpPacket::getModbusFrame()
{
    return m_ModbusFrame;
}

void CModbusTcpPacket::setTransactionId(unsigned short transId)
{
    m_TransactionId = transId;

    QDataStream out(&m_PacketHead, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << (qint16)m_TransactionId;
}

void CModbusTcpPacket::setProtocolId(unsigned short protocolId)
{
    m_ProtocolId = protocolId;

    QDataStream out(&m_PacketHead, QIODevice::WriteOnly);
    out.device()->seek(2);
    out << (qint16)m_ProtocolId;
}

void CModbusTcpPacket::setLength(unsigned short length)
{
    m_Length = length;

    QDataStream out(&m_PacketHead, QIODevice::WriteOnly);
    out.device()->seek(4);
    out << (qint16)m_Length;
}

void CModbusTcpPacket::setModbusFrame(QByteArray &frame)
{
    m_ModbusFrame = frame;
}

QByteArray CModbusTcpPacket::toQByteArray()
{
    QByteArray data = m_PacketHead;
    data += m_ModbusFrame;
    return data;
}

int CModbusTcpPacket::length()
{
    return (m_PacketHead.size() + m_ModbusFrame.size());
}
