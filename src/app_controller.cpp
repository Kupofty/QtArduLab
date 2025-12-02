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

    //Try to connect
    QString result;
    if(serial_device->openSerialDevice())
    {
        result =  "Connected to " + serial_device->getPortName();
        updateGuiAfterSerialConnection(true);
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
void AppController::on_pushButton_clear_output_text_clicked()
{
    ui->plainTextEdit_outputs->clear();
}

void AppController::on_pushButton_send_custom_text_clicked()
{
    QString message = ui->lineEdit_custom_text->text();
    if(!message.isEmpty())
        sendMessage(message);
}

void AppController::on_pushButton_clear_custom_text_clicked()
{
    ui->lineEdit_custom_text->clear();
}

void AppController::on_pushButton_led_low_clicked()
{
    QString message = "LOW";
    sendMessage(message);
}

void AppController::on_pushButton_led_high_clicked()
{
    QString message = "HIGH";
    sendMessage(message);
}

void AppController::on_pushButton_send_pwm_clicked()
{
    QString message = QString::number(ui->dial_pwm->value());
    sendMessage(message);
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
    ui->plainTextEdit_inputs->appendPlainText(prependTimestamp(text));
}

void AppController::on_pushButton_clear_input_text_clicked()
{
    ui->plainTextEdit_inputs->clear();
}



/////////////////////////
/// General Functions ///
/////////////////////////
QString AppController::prependTimestamp(const QString &msg)
{
    return "[" + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "] " + msg;
}

void AppController::sendMessage(const QString &message)
{
    if (!serial_device->isSerialOpen())
        return;

    // Send message to serial device
    serial_device->sendData(message);

    // Append timestamped message to output
    ui->plainTextEdit_outputs->appendPlainText(prependTimestamp(message));
}





