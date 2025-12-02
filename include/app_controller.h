#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QDateTime>
#include <QRegularExpression>
#include <QMessageBox>

#include "serial_com.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class AppController;
}
QT_END_NAMESPACE



class AppController : public QMainWindow
{
    Q_OBJECT

    public:
        AppController(QWidget *parent = nullptr);
        ~AppController();

    private:
        Ui::AppController *ui;
        SerialCom *serial_device;

    private:
        void closeInputSerial();
        void listAvailablePorts(QComboBox *comboBox);
        void updateGuiAfterSerialConnection(bool connectState);
        QString prependTimestamp(const QString &msg);
        void sendSerial(const QString &message);
        void sendMessage(QString type, int pin, QString messageCommand);

    private slots:
        void handleSerialInput(const QByteArray &line);
        void handleSerialDisconnection(const QString errorMessage);

        //connection page
        void on_pushButton_refresh_available_ports_list_clicked();
        void on_pushButton_connect_serial_input_clicked();
        void on_pushButton_disconnect_serial_input_clicked();
        // logs
        void on_pushButton_clear_input_text_clicked();
        void on_pushButton_clear_output_text_clicked();
        //commands
        void on_pushButton_led_low_clicked();
        void on_pushButton_led_high_clicked();
        void on_pushButton_send_custom_text_clicked();
        void on_pushButton_clear_custom_text_clicked();
        void on_lineEdit_custom_text_returnPressed();
        void on_pushButton_freeze_input_text_clicked();
};

