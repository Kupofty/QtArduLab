#include "app_controller.h"
#include "ui_interface.h"

#include <QStandardItemModel>

/////////////
/// Class ///
/////////////
AppController::AppController(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AppController),
    serial_device(new SerialCom)
{
    //Setup UI
    ui->setupUi(this);
    ui->tabWidget->setCurrentWidget(ui->tab_connection);
    //this->showMaximized();

    //Setup Connects
    connect(serial_device, &SerialCom::newLineReceived, this, &AppController::handleSerialInput);
    connect(serial_device, &SerialCom::serialError, this, &AppController::handleSerialError);

    //Init UI
    ui->verticalFrame_serial_advanced_config->hide();
    listAvailablePorts(ui->comboBox_serial_input_port_list);
}

AppController::~AppController()
{
    delete ui;
    delete serial_device;
}



/////////////////////////
/// Serial Connection ///
/////////////////////////

// Connection
void AppController::updateSerialSettings()
{
    //Update serial settings
    serial_device->setPortName(ui->comboBox_serial_input_port_list->currentText());
    serial_device->setBaudRate((ui->comboBox_serial_input_port_baudrate->currentText()).toInt());

    //Update IO Method
    int index_io_method = ui->comboBox_io_method->currentIndex();
    if (index_io_method == 0){
        serial_device->setOpenMode(QIODevice::ReadWrite);
        serial_is_read_only = false;
    }
    else if (index_io_method == 1){
        serial_device->setOpenMode(QIODevice::ReadOnly);
        serial_is_read_only = true;
    }
    else if (index_io_method == 2){
        serial_device->setOpenMode(QIODevice::WriteOnly);
        serial_is_read_only = false;
    }

    //Update Data Bits
    int index_data_bits = ui->comboBox_data_bits->currentIndex();
    if (index_data_bits == 0)
        serial_device->setDataBits(QSerialPort::Data5);
    else if (index_data_bits == 1)
        serial_device->setDataBits(QSerialPort::Data6);
    else if (index_data_bits == 2)
        serial_device->setDataBits(QSerialPort::Data7);
    else if (index_data_bits == 3)
        serial_device->setDataBits(QSerialPort::Data8);

    //Update Parity
    int index_parity = ui->comboBox_parity->currentIndex();
    if (index_parity == 0)
        serial_device->setParity(QSerialPort::NoParity);
    else if (index_parity == 1)
        serial_device->setParity(QSerialPort::EvenParity);
    else if (index_parity == 2)
        serial_device->setParity(QSerialPort::OddParity);
    else if (index_parity == 3)
        serial_device->setParity(QSerialPort::SpaceParity);
    else if (index_parity == 4)
        serial_device->setParity(QSerialPort::MarkParity);

    //Update Stop Bits
    int index_stop_bits = ui->comboBox_stop_bit->currentIndex();
    if (index_stop_bits == 0)
        serial_device->setStopBits(QSerialPort::OneStop);
    else if (index_stop_bits == 1)
        serial_device->setStopBits(QSerialPort::OneAndHalfStop);
    else if (index_stop_bits == 2)
        serial_device->setStopBits(QSerialPort::TwoStop);

    //Update Flow Control
    int index_flow_control = ui->comboBox_flow_control->currentIndex();
    if (index_flow_control == 0)
        serial_device->setFlowControl(QSerialPort::NoFlowControl);
    else if (index_flow_control == 1)
        serial_device->setFlowControl(QSerialPort::HardwareControl);
    else if (index_flow_control == 2)
        serial_device->setFlowControl(QSerialPort::SoftwareControl);
}

void AppController::serialConnect()
{
    QString result;
    if(serial_device->openSerialDevice())
    {
        result =  "Connected to " + serial_device->getPortName();
        updateGuiAfterSerialConnection(true);
        ui->verticalFrame_commands->setEnabled(!serial_is_read_only);
    }
    else
        result =  "Failed to open " + serial_device->getPortName() + " : " + serial_device->getErrorString();

    //Display connection status
    ui->plainTextEdit_connection_status->setPlainText(result);
}

void AppController::closeInputSerial()
{
    if(serial_device->isSerialOpen())
    {
        serial_device->closeSerialDevice();
        ui->plainTextEdit_connection_status->setPlainText(serial_device->getPortName()+" closed");
    }
    else
        ui->plainTextEdit_connection_status->setPlainText("Connection not opened");
}

void AppController::updateGuiAfterSerialConnection(bool connectState)
{
    ui->horizontalFrame_serial_input_connection->setEnabled(!connectState);
    ui->pushButton_connect_serial_input->setEnabled(!connectState);
    ui->pushButton_disconnect_serial_input->setEnabled(connectState);
    ui->verticalFrame_commands->setEnabled(connectState);
}

void AppController::on_pushButton_connect_serial_input_clicked()
{
    //Send GUI settings to serial device
    updateSerialSettings();

    //Try to connect
    serialConnect();
}

void AppController::on_pushButton_disconnect_serial_input_clicked()
{
    closeInputSerial();
    updateGuiAfterSerialConnection(false);
}

void AppController::on_checkBox_serial_advanced_config_stateChanged(int checked)
{
    if(checked)
    {
        ui->verticalFrame_serial_advanced_config->show();
        ui->horizontalFrame_serial_show_advanced_config->hide();
    }
}


// COM ports
void AppController::listAvailablePorts(QComboBox *comboBox)
{
    comboBox->clear();

    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports)
    {
        QString tooltip = QString(
                              "Port: %1\n"
                              "Description: %2\n"
                              "Manufacturer: %3\n"
                              "Serial Number: %4\n"
                              "VID: %5\n"
                              "PID: %6")
                              .arg(
                                  port.portName(),
                                  port.description(),
                                  port.manufacturer(),
                                  port.serialNumber(),
                                  QString("%1").arg(port.vendorIdentifier(), 4, 16, QLatin1Char('0')),
                                  QString("%1").arg(port.productIdentifier(), 4, 16, QLatin1Char('0'))
                                  );


        comboBox->addItem(port.portName());

        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(comboBox->model());

        QStandardItem *item = model->item(comboBox->count() - 1);
        item->setToolTip(tooltip);
    }
}



void AppController::on_pushButton_refresh_available_ports_list_clicked()
{
    listAvailablePorts(ui->comboBox_serial_input_port_list);
}



/////////////////
/// Send data ///
/////////////////

//Send
void AppController::sendMessage(QString command, int pin, QString value)
{
    QString space = " ";
    QString pinStr = QString::number(pin);

    QString message = command + space + pinStr + space + value;
    sendSerial(message);
}

void AppController::sendSerial(const QString &message)
{
    if (!serial_device->isSerialOpen())
        return;

    // Send message to serial device
    serial_device->sendData(message);

    // Append timestamped message to output
    ui->plainTextEdit_commands->appendPlainText("--------------------------------");
    ui->plainTextEdit_commands->appendPlainText(actualTimeStamp());
    ui->plainTextEdit_commands->appendPlainText("Command : " + message);
}


//Clear textEdit
void AppController::on_pushButton_clear_output_text_clicked()
{
    ui->plainTextEdit_commands->clear();
}


//Custom Text
void AppController::on_pushButton_send_custom_text_clicked()
{
    QString message = ui->lineEdit_custom_text->text();
    if(!message.isEmpty())
        sendSerial(message);

    ui->plainTextEdit_commands->moveCursor(QTextCursor::End);
}

void AppController::on_lineEdit_custom_text_returnPressed()
{
    ui->pushButton_send_custom_text->click();
}

void AppController::on_pushButton_clear_custom_text_clicked()
{
    ui->lineEdit_custom_text->clear();
}


//LED
void AppController::sendDigitalCmd(QString state)
{
    int pin = ui->spinBox_digital_pin->value();
    sendMessage("DIGITAL", pin, state);
}

void AppController::on_pushButton_digital_low_clicked()
{
    sendDigitalCmd("LOW");
}

void AppController::on_pushButton_digital_high_clicked()
{
    sendDigitalCmd("HIGH");
}


//Servomotor
void AppController::sendServoCmd(int angle)
{
    ui->horizontalSlider_servomotor_angle->setValue(angle);
    ui->spinBox_servomotor_angle->setValue(angle);

    int pin = ui->spinBox_servomotor_pin->value();
    QString angleStr = QString::number(angle);
    sendMessage("SERVO", pin, angleStr);
}

void AppController::on_spinBox_servomotor_angle_editingFinished()
{
    int angle = ui->spinBox_servomotor_angle->value();
    sendServoCmd(angle);
}

void AppController::on_horizontalSlider_servomotor_angle_sliderReleased()
{
    int angle = ui->horizontalSlider_servomotor_angle->value();
    sendServoCmd(angle);
}

void AppController::on_horizontalSlider_servomotor_angle_sliderMoved(int position)
{
    ui->spinBox_servomotor_angle->setValue(position);
}

void AppController::on_pushButton_servomotor_0_clicked()
{
    sendServoCmd(0);
}

void AppController::on_pushButton_servomotor_90_clicked()
{
    sendServoCmd(90);
}

void AppController::on_pushButton_servomotor_180_clicked()
{
    sendServoCmd(180);
}


//Speaker / Passive Buzzer
void AppController::sendToneCmd(int frequency)
{
    QString frequencyStr = QString::number(frequency);
    int pin = ui->spinBox_speaker_pin->value();
    sendMessage("TONE", pin, frequencyStr);
}

void AppController::on_pushButton_speaker_on_clicked()
{
    int frequency = ui->spinBox_speaker_frequency->value();
    sendToneCmd(frequency);
}

void AppController::on_pushButton_speaker_off_clicked()
{
    sendToneCmd(-1);
}


//Analog
void AppController::sendAnalogCmd(int pwm)
{
    ui->horizontalSlider_analog_pwm->setValue(pwm);
    ui->spinBox_analog_pwm->setValue(pwm);

    int pin = ui->spinBox_analog_pin->value();
    QString pwmStr = QString::number(pwm);
    sendMessage("ANALOG", pin, pwmStr);
}

void AppController::on_spinBox_analog_pwm_editingFinished()
{
    int pwm = ui->spinBox_analog_pwm->value();
    sendAnalogCmd(pwm);
}

void AppController::on_horizontalSlider_analog_pwm_sliderReleased()
{
    int pwm = ui->horizontalSlider_analog_pwm->value();
    sendAnalogCmd(pwm);
}

void AppController::on_horizontalSlider_analog_pwm_sliderMoved(int position)
{
    ui->spinBox_analog_pwm->setValue(position);
}

void AppController::on_pushButton_analog_0_clicked()
{
    sendAnalogCmd(0);
}

void AppController::on_pushButton_analog_128_clicked()
{
    sendAnalogCmd(128);
}

void AppController::on_pushButton_analog_255_clicked()
{
    sendAnalogCmd(255);
}



////////////////////
/// Receive data ///
////////////////////
void AppController::handleSerialInput(const QByteArray &line)
{
    // Don't display sentences if freeze button is checked
    if (ui->pushButton_freeze_input_text->isChecked())
        return;

    QString text = QString::fromUtf8(line).trimmed();

    // Reponse to CMD
    if (text.startsWith("[CMD]")) {
        int endBracket = text.indexOf(']');
        QString payload = text.mid(endBracket + 1).trimmed();
        ui->plainTextEdit_commands->appendPlainText("Response : " + payload);
    }

    //Display sensors & logs
    else
    {
        QString timeStampedText = actualTimeStamp() + text;
        ui->plainTextEdit_inputs->appendPlainText(timeStampedText);
    }

}

void AppController::handleSerialError(const QString errorMessage)
{
    ui->tabWidget->setCurrentWidget(ui->tab_connection);
    ui->pushButton_disconnect_serial_input->click();
    ui->pushButton_refresh_available_ports_list->click();

    QMessageBox::warning(this, "Serial Error", errorMessage);
}

void AppController::on_pushButton_clear_input_text_clicked()
{
    ui->plainTextEdit_inputs->clear();
}

void AppController::on_pushButton_freeze_input_text_clicked()
{
    ui->plainTextEdit_inputs->moveCursor(QTextCursor::End);
}



/////////////////////////
/// General Functions ///
/////////////////////////
QString AppController::actualTimeStamp()
{
    return "[" + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "] ";
}






