#include "mymodbus.h"
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusReply>
#include <QSerialPort>
#include <QVariant>

MyModbus::MyModbus(QObject *parent) : QObject(parent)
{
    // 创建Modbus RTU主站
    modbusClient = new QModbusRtuSerialClient();
    // 设置串口参数
    modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, "COM5");
    modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, 115200);
    modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);

}

MyModbus::~MyModbus()
{

}

void MyModbus::slot_readReg(quint8 id, quint16 addr, quint8 len)
{
    if(modbusClient->state() != QModbusRtuSerialClient::ConnectedState){
        modbusClient->connectDevice();
    }
    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, addr, len);
    QModbusReply *reply = modbusClient->sendReadRequest(unit,id);
    if(reply && !reply->isFinished()){
        connect(reply, &QModbusReply::finished, this, &MyModbus::replyFinished);
    }
}

void MyModbus::slot_writeReg(quint8 id, quint16 addr, QVector<quint16> buff)
{
    if(modbusClient->state() != QModbusRtuSerialClient::ConnectedState){
        modbusClient->connectDevice();
    }

    // QVector<quint16> data;
    // data << 5 << 24;
    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, addr, buff);
    modbusClient->sendWriteRequest(unit, id);
}

void MyModbus::replyFinished()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if(reply){
        QModbusDataUnit unit = reply->result();
        reply->deleteLater();
        qDebug()<< "寄存器类型" <<unit.registerType();
        qDebug()<< "寄存器起始地址" << unit.startAddress();
        QVector<quint16> data = unit.values();
        QString s;
        Q_FOREACH(quint16 i, data){
            s.append(QString::number(i)).append(" ");
        }
        emit signal_data(s);
    }
}
