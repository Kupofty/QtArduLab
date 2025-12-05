#pragma once

#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextStream>
#include <QDebug>

class SerialCom : public QObject
{
    Q_OBJECT

    public:
        SerialCom();
        ~SerialCom();

        //Set
        void setPortName(const QString &portName);
        void setBaudRate(qint32 baudRate);
        void setDataBits(QSerialPort::DataBits dataBits);
        void setParity(QSerialPort::Parity parity);
        void setStopBits(QSerialPort::StopBits stopBits);
        void setFlowControl(QSerialPort::FlowControl flowControl);
        void setOpenMode(QIODevice::OpenMode mode);

        //Com
        bool openSerialDevice();
        void closeSerialDevice();

        //Send
        void sendData(const QString &data);
        void sendData(const QByteArray &data);

        //Get
        QString getErrorString();
        bool isSerialOpen();
        QString getPortName();
        qint32 getBaudRate();
        QSerialPort::DataBits getDataBits();
        QSerialPort::Parity getParity();
        QSerialPort::StopBits getStopBits();
        QSerialPort::FlowControl getFlowControl();
        QIODevice::OpenMode getOpenMode();

    signals:
        void newLineReceived(const QByteArray &line);
        void serialError(const QString errorMessage);

    private slots:
        void readData();
        void handleSerialError(QSerialPort::SerialPortError error);

    private:
        QSerialPort serial;
        QByteArray buffer;
        QIODevice::OpenMode ioMode = QIODevice::ReadWrite;
};
