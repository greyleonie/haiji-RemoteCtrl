#ifndef MODBUSTCPPACKET_H
#define MODBUSTCPPACKET_H

#include <QByteArray>
#include "ModbusFrame.h"

class CModbusTcpPacket
{
public:
    CModbusTcpPacket();
    CModbusTcpPacket(QByteArray &data);
    CModbusTcpPacket(CModbusFrame &frame);
    ~CModbusTcpPacket();

    unsigned short getTransactionId();
    unsigned short getProtocolId();
    unsigned short getLength();
    QByteArray getModbusFrame();

    void setTransactionId(unsigned short transId);
    void setProtocolId(unsigned short protocolId);
    void setLength(unsigned short length);
    void setLength();
    void setModbusFrame(QByteArray &frame);

    QByteArray toQByteArray();
    int length();

private:
    static unsigned short m_StaticTransId;

    unsigned short  m_TransactionId;
    unsigned short  m_ProtocolId;
    unsigned short  m_Length;
    QByteArray      m_PacketHead;
    QByteArray      m_ModbusFrame;
};

#endif // MODBUSTCPPACKET_H
