#include "ModbusTcpClient.h"

CModbusTcpClient::CModbusTcpClient()
{
    m_Tick = new QTimer(this);
    m_Tick->setInterval(1000);
    m_Tick->setSingleShot(false);
    connect(m_Tick, SIGNAL(timeout()), this, SLOT(AutoConnect()));
    m_Tick->start();

    m_IsConnected = false;
    m_TcpSocket = new QTcpSocket(this);
    connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(ReceiveConfirmation()));
    connect(m_TcpSocket, SIGNAL(connected()), this, SLOT(Connected()));
    connect(m_TcpSocket, SIGNAL(disconnected()), this, SLOT(Disconnected()));
    connect(m_TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Error(QAbstractSocket::SocketError)));
}

CModbusTcpClient::~CModbusTcpClient()
{
    delete m_TcpSocket;
}

void CModbusTcpClient::ConnectToServer(QString host, quint16 port)
{
    m_Host = host;
    m_Port = port;
    if(!m_IsConnected)
        m_TcpSocket->connectToHost(host, port);
}

void CModbusTcpClient::CloseConnection()
{
    m_TcpSocket->close();
}

bool CModbusTcpClient::IsConnected()
{
    return m_IsConnected;
}

void CModbusTcpClient::SendRequest(CModbusTcpFrame &request)
{
    if(!m_IsConnected)
        return;
    m_RequestList.append(request);
    m_TcpSocket->write(request.toQByteArray(), request.length());
}

void CModbusTcpClient::ReceiveConfirmation(void)
{
    QByteArray data = m_TcpSocket->readAll();
    m_ConfirmationList = ConfirmationParser(data);
    if(!m_ConfirmationList.isEmpty())
        emit(ConfirmListNotEmpty());
}

QString CModbusTcpClient::GetHostIp()
{
    QHostAddress hostAddr = m_TcpSocket->localAddress();
    return hostAddr.toString();
}

quint16 CModbusTcpClient::GetHostPort()
{
    return m_TcpSocket->localPort();
}

bool CModbusTcpClient::IsConfirmListEmpty()
{
    return m_ConfirmationList.isEmpty();
}

CModbusTcpFrame *CModbusTcpClient::GetConfirm()
{
    CModbusTcpFrame request = m_RequestList.first();
    m_RequestList.removeFirst();
    CModbusTcpFrame *confirm = new CModbusTcpFrame(m_ConfirmationList.first());
    m_ConfirmationList.removeFirst();
    if(confirm->isMatch(request))
        return confirm;
    else
        return NULL;
}

QList<CModbusTcpFrame> CModbusTcpClient::ConfirmationParser(QByteArray &data)
{
    QList<CModbusTcpFrame> confirmList;
    while(data.size())
    {
        CModbusTcpFrame confirm(data);
        if(confirm.length() == 0)
            break;
        confirmList.append(confirm);
        data.remove(0, confirm.length());
    }

    return confirmList;
}

void CModbusTcpClient::Connected()
{
    m_IsConnected = true;
    emit ConnectionStateChange();
}

void CModbusTcpClient::Disconnected()
{
    m_IsConnected = false;
    emit ConnectionStateChange();
}

void CModbusTcpClient::Error(QAbstractSocket::SocketError err)
{
    QDebug dbg = qDebug();
    dbg << "error:" << err << "\n";
}

void CModbusTcpClient::AutoConnect()
{
    if(!m_IsConnected)
        m_TcpSocket->connectToHost(m_Host, m_Port);
}
