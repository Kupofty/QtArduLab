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

        //Com
        bool openSerialDevice();
        void closeSerialDevice();

        //Send
        void sendData(const QString &data);
        void sendData(const QByteArray &data);

        //Get
        QString getPortName();
        QString getErrorString();
        bool isSerialOpen();

    signals:
        void newLineReceived(const QByteArray &line);
        void deviceDisconnected(const QString errorMessage);

    private slots:
        void readData();
        void handleSerialError(QSerialPort::SerialPortError error);

    private:
        QSerialPort serial;
        QByteArray buffer;

};
