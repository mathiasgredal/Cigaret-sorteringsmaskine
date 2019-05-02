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

void ArduinoHelper::ArduinoConnect()
{
    sp_return error = sp_get_port_by_name(comport,&port);

    if(error == SP_OK)
    {
        error = sp_open(port, SP_MODE_WRITE);

        if(error == SP_OK)
        {
            sp_set_baudrate(port,57600);

            sp_nonblocking_write(port, "hej", sizeof ("hej"));


        }

    }
}
