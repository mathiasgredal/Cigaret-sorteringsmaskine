#include "arduinohelper.h"

ArduinoHelper::ArduinoHelper()
{

}

QVector<QString> ArduinoHelper::ListCOMPorts()
{
    int i;
    struct sp_port **arrayPorts;
    QVector<QString> ports;



    sp_return error = sp_list_ports(&arrayPorts);

    if (error == SP_OK) {
      for (i = 0; arrayPorts[i]; i++) {
          ports.push_back(QString(sp_get_port_name(arrayPorts[i])));
      }
      sp_free_port_list(arrayPorts);
    } else {
      qInfo() << "No serial devices detected";
    }

    return ports;
}

enum ArduinoHelper::ConnectionErrors ArduinoHelper::ArduinoConnect(int baudrate, QString COMPort)
{
    sp_return error = sp_get_port_by_name(COMPort.toStdString().c_str(),&port);

    if(error == SP_OK)
    {
        error = sp_open(port, SP_MODE_WRITE);

        if(error == SP_OK)
        {
            sp_set_baudrate(port,baudrate);

            sp_nonblocking_write(port, "e", sizeof ("e"));

            return ArduinoHelper::ConnectionErrors::SUCCES;
        }
        else {
            return ArduinoHelper::ConnectionErrors::UNABLE_TO_OPEN_PORT;
        }

    }
    else {
        return ArduinoHelper::ConnectionErrors::PORT_NOT_FOUND;
    }
}

void ArduinoHelper::SendString(QString data){
    sp_nonblocking_write(port, data.toStdString().c_str(), sizeof (data.toStdString().c_str()));
}
