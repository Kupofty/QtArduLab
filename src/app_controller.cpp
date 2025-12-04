#include "app_controller.h"
#include "ui_interface.h"

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
void AppController::on_pushButton_connect_serial_input_clicked()
{
    //Update serial settings
    serial_device->setPortName(ui->comboBox_serial_input_port_list->currentText());
    serial_device->setBaudRate((ui->comboBox_serial_input_port_baudrate->currentText()).toInt());

    int index = ui->comboBox_io_method->currentIndex();
    if (index == 0)
        serial_device->setOpenMode(QIODevice::ReadWrite);
    else if (index == 1)
        serial_device->setOpenMode(QIODevice::ReadOnly);
    else if (index == 2)
        serial_device->setOpenMode(QIODevice::WriteOnly);

    //Try to connect
    QString result;
    if(serial_device->openSerialDevice())
    {
        result =  "Connected to " + serial_device->getPortName();
        updateGuiAfterSerialConnection(true);
        ui->verticalFrame_commands->setEnabled(index != 1);
    }
    else
        result =  "Failed to open " + serial_device->getPortName() + " : " + serial_device->getErrorString();

    //Display connection status
    ui->plainTextEdit_connection_status->setPlainText(result);

}

void AppController::on_pushButton_disconnect_serial_input_clicked()
{
    closeInputSerial();
    updateGuiAfterSerialConnection(false);
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


//COM ports
void AppController::listAvailablePorts(QComboBox *comboBox)
{
    comboBox->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports)
    {
        comboBox->addItem(port.portName());
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
void AppController::on_pushButton_led_low_clicked()
{
    int pin = ui->spinBox_led_pin->value();
    sendMessage("LED", pin, "LOW");
}

void AppController::on_pushButton_led_high_clicked()
{
    int pin = ui->spinBox_led_pin->value();
    sendMessage("LED", pin, "HIGH");
}


//Servomotor
void AppController::on_spinBox_servomotor_angle_editingFinished()
{
    int pin = ui->spinBox_servomotor_pin->value();
    int angle = ui->spinBox_servomotor_angle->value();
    QString angleStr = QString::number(angle);
    sendMessage("SERVO", pin, angleStr);

    ui->horizontalSlider_servomotor_angle->setValue(angle);
}

void AppController::on_horizontalSlider_servomotor_angle_sliderReleased()
{
    int pin = ui->spinBox_servomotor_pin->value();
    int angle = ui->horizontalSlider_servomotor_angle->value();
    QString angleStr = QString::number(angle);
    sendMessage("SERVO", pin, angleStr);
}

void AppController::on_horizontalSlider_servomotor_angle_sliderMoved(int position)
{
    ui->spinBox_servomotor_angle->setValue(position);
}

void AppController::on_pushButton_servomotor_0_clicked()
{
    int pin = ui->spinBox_servomotor_pin->value();
    int angle = 0;
    QString angleStr = QString::number(angle);
    sendMessage("SERVO", pin, angleStr);

    ui->horizontalSlider_servomotor_angle->setValue(angle);
    ui->spinBox_servomotor_angle->setValue(angle);
}


void AppController::on_pushButton_servomotor_90_clicked()
{
    int pin = ui->spinBox_servomotor_pin->value();
    int angle = 90;
    QString angleStr = QString::number(angle);
    sendMessage("SERVO", pin, angleStr);

    ui->horizontalSlider_servomotor_angle->setValue(angle);
    ui->spinBox_servomotor_angle->setValue(angle);
}


void AppController::on_pushButton_servomotor_180_clicked()
{
    int pin = ui->spinBox_servomotor_pin->value();
    int angle = 180;
    QString angleStr = QString::number(angle);
    sendMessage("SERVO", pin, angleStr);

    ui->horizontalSlider_servomotor_angle->setValue(angle);
    ui->spinBox_servomotor_angle->setValue(angle);
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






