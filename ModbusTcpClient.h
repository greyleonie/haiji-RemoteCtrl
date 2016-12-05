#ifndef MODBUSTCPCLIENT_H
#define MODBUSTCPCLIENT_H

#include <QtNetwork/QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QByteArray>
#include <QList>
#include <QTimer>
#include "ModbusTcpFrame.h"

class CModbusTcpClient : public QObject
{
    Q_OBJECT

public:
    CModbusTcpClient();
    ~CModbusTcpClient();

    void        ConnectToServer(QString host, quint16 port);
    void        CloseConnection();
    QString     GetHostIp();
    quint16     GetHostPort();
    bool IsConnected();

    void        SendRequest(CModbusTcpFrame &request);
    bool        IsConfirmListEmpty();
    CModbusTcpFrame *GetConfirm();
    QList<CModbusTcpFrame> ConfirmationParser(QByteArray &data);

signals:
    void ConfirmListNotEmpty(void);
    void ConnectionStateChange();

private slots:
    void ReceiveConfirmation(void);
    void Connected();
    void Disconnected();
    void Error(QAbstractSocket::SocketError);
    void AutoConnect();

private:
    QTcpSocket              *m_TcpSocket;
    QList<CModbusTcpFrame>     m_RequestList;
    QList<CModbusTcpFrame>     m_ConfirmationList;
    QByteArray              m_ReceivedData;

    bool                    m_IsConnected;
    QString m_Host;
    quint16 m_Port;

    QTimer  *m_Tick;
};

#endif // MODBUSTCPCLIENT_H
