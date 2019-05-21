#ifndef ARDUINOHELPER_H
#define ARDUINOHELPER_H

#include <QtCore>
#include <libserialport.h>


class ArduinoHelper
{
public:
    ArduinoHelper();
    QVector<QString> ListCOMPorts();
    enum ConnectionErrors {PORT_NOT_FOUND, UNABLE_TO_OPEN_PORT, SUCCES};
    enum ConnectionErrors ArduinoConnect(int baudrate, QString COMPort);
    void SendString(QString data);
private:
    struct sp_port *port;
    int baudrate;
    const char* comport;
};

#endif // ARDUINOHELPER_H
