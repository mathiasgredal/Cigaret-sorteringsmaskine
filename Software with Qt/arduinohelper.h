#ifndef ARDUINOHELPER_H
#define ARDUINOHELPER_H

#include <QtCore>
#include <libserialport.h>


class ArduinoHelper
{
public:
    ArduinoHelper();
    QVector<QString> ListCOMPorts();
    void ArduinoConnect();
private:
    struct sp_port *port;
    int baudrate;
    const char* comport;
};

#endif // ARDUINOHELPER_H
