#include "serial_com.h"

/////////////
/// Class ///
/////////////
SerialCom::SerialCom()
{
    //Set serial parameters
    serial.setPortName("COM0");
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    //Connect
    connect(&serial, &QSerialPort::readyRead, this, &SerialCom::readData);
    connect(&serial, &QSerialPort::errorOccurred, this, &SerialCom::handleSerialError);
}

SerialCom::~SerialCom()
{
    closeSerialDevice();
}



/////////////////
/// Functions ///
/////////////////
void SerialCom::sendData(const QString &line)
{
    QByteArray data = line.toUtf8() + "\r\n";
    serial.write(data);
}

void SerialCom::sendData(const QByteArray &data)
{
    QByteArray sendData = data + "\r\n";
    serial.write(sendData);
}

//////////////////
/// Connection ///
//////////////////
bool SerialCom::openSerialDevice()
{
    if (serial.isOpen()){
        return false;
    }

    if (!serial.open(QIODevice::ReadWrite)) {
        return false;
    }

    return true;
}

void SerialCom::closeSerialDevice()
{
    if(isSerialOpen())
        serial.close();
}



///////////////////
/// Handle data ///
///////////////////
void SerialCom::readData()
{
    const QByteArray data = serial.readAll();
    buffer.append(data);

    while (buffer.contains('\n'))
    {
        int index = buffer.indexOf('\n');
        QByteArray line = buffer.left(index).trimmed();
        buffer.remove(0, index + 1);

        emit newLineReceived(line);
    }
}

void SerialCom::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError || error == QSerialPort::DeviceNotFoundError) {
        emit deviceDisconnected(serial.errorString());
        closeSerialDevice();
    }
}



//////////////////////
/// Set parameters ///
//////////////////////
void SerialCom::setPortName(const QString &portName)
{
    serial.setPortName(portName);
}

void SerialCom::setBaudRate(qint32 baudRate)
{
    serial.setBaudRate(baudRate);
}



////////////////
/// Get data ///
////////////////
QString SerialCom::getPortName()
{
    return serial.portName();
}

QString SerialCom::getErrorString()
{
    return serial.errorString();
}

bool SerialCom::isSerialOpen()
{
    return serial.isOpen();
}

