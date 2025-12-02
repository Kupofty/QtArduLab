#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QDateTime>
#include <QRegularExpression>

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
        void sendMessage(const QString &message);

    private slots:
        void handleSerialInput(const QByteArray &line);

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
        void on_pushButton_send_pwm_clicked();
        void on_pushButton_clear_custom_text_clicked();
};

