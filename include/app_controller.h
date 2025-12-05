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
        QString actualTimeStamp();
        void sendSerial(const QString &message);
        void sendMessage(QString type, int pin, QString messageCommand);

    private slots:
        void handleSerialInput(const QByteArray &line);
        void handleSerialError(const QString errorMessage);

        void on_pushButton_refresh_available_ports_list_clicked();
        void on_pushButton_connect_serial_input_clicked();
        void on_pushButton_disconnect_serial_input_clicked();
        void on_pushButton_clear_input_text_clicked();
        void on_pushButton_clear_output_text_clicked();
        void on_pushButton_freeze_input_text_clicked();
        void on_pushButton_digital_low_clicked();
        void on_pushButton_digital_high_clicked();
        void on_pushButton_send_custom_text_clicked();
        void on_pushButton_clear_custom_text_clicked();
        void on_lineEdit_custom_text_returnPressed();
        void on_spinBox_servomotor_angle_editingFinished();
        void on_horizontalSlider_servomotor_angle_sliderReleased();
        void on_horizontalSlider_servomotor_angle_sliderMoved(int position);
        void on_pushButton_servomotor_0_clicked();
        void on_pushButton_servomotor_90_clicked();
        void on_pushButton_servomotor_180_clicked();
        void on_pushButton_speaker_on_clicked();
        void on_pushButton_speaker_off_clicked();
        void on_horizontalSlider_analog_pwm_sliderReleased();
        void on_spinBox_analog_pwm_editingFinished();
        void on_pushButton_analog_0_clicked();
        void on_pushButton_analog_128_clicked();
        void on_pushButton_analog_255_clicked();
        void on_horizontalSlider_analog_pwm_sliderMoved(int position);
};

