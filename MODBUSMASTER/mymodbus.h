#ifndef MYMODBUS_H
#define MYMODBUS_H

#include <QObject>
#include <QModbusRtuSerialClient>

class MyModbus : public QObject
{
    Q_OBJECT

public:
    explicit MyModbus(QObject *parent = nullptr);
    ~MyModbus();

signals:
    void signal_data(QString s);

public slots:
    void slot_readReg(quint8 id, quint16 addr, quint8 len);
    void slot_writeReg(quint8 id, quint16 addr, QVector<quint16> buff);

private:
    // 创建Modbus RTU主站
    QModbusRtuSerialClient *modbusClient;

private slots:
    void replyFinished();

};

#endif // MYMODBUS_H
