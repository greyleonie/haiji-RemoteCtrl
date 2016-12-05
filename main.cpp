#include <QtGui/QApplication>

#include "console.h"
#include "ModbusTcpClient.h"

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
/*
    char rawdata[] = {0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x00, 0x00, 0x00, 0x0A};
    QByteArray data(rawdata, sizeof(rawdata));
    CModbusFrame frame(data);

    CModbusTcpClient client;
    client.ConnectToServer("127.0.0.1", 502);
//    QString hostip = client.GetHostIp();
//    quint16 hostport = client.GetHostPort();
    client.SendRequest(frame);
    client.CloseConnection();
//*/
    CConsole *console = new CConsole;
    if (!console->exec())
        return 0;

    return a.exec();
}
