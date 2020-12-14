#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpsend= new QUdpSocket(this);
    udprecev= new QUdpSocket(this);
    udprecev->bind(QHostAddress::LocalHost,4444);
    connect(udprecev, SIGNAL(readyRead()), this, SLOT(readDataFrom1234()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::readDataFrom1234(){

    QByteArray dataToRece;
    while (udprecev->hasPendingDatagrams())
    {
        dataToRece.resize(udprecev->pendingDatagramSize());
        udprecev->readDatagram(dataToRece.data(),dataToRece.size(),&senderip,&senderportno);
        memcpy((char*)&recvm_Data,dataToRece.data(),sizeof(recvm_Data));

        qDebug()<<"readData : "<<sizeof (recvm_Data);
        if(recvm_Data.msg_Id==0x542A){
            ui->listWidget->insertItem(0,"Msg Id:"+QString::number(recvm_Data.msg_Id,16));
            ui->listWidget->insertItem(0,"Source Id:"+QString::number(recvm_Data.source_Id));
            ui->listWidget->insertItem(0,"Destination Id:"+QString::number(recvm_Data.destination_Id));
            ui->listWidget->insertItem(0,"X:"+QString::number(recvm_Data.x));
            ui->listWidget->insertItem(0,"Y:"+QString::number(recvm_Data.y));
            ui->listWidget->insertItem(0,"Z:"+QString::number(recvm_Data.z));
            ui->listWidget->insertItem(0,"CheckSum:"+QString::number(recvm_Data.s_checksum));
            ui->listWidget->insertItem(0,tr("Size Of Structure: %1").arg(sizeof(recvm_Data)));

            qDebug()<<"Message From : "<<senderip.toString();
            qDebug()<<"Message port : "<<senderportno;
        }else {
            qDebug()<<"ERROR : ";
        }
    }
    //---------------------------PORT-5555------------------------------------------
    sendm_Data.msg_Id = 0xAAA1;
    sendm_Data.x=recvm_Data.x;
    sendm_Data.y=recvm_Data.y;
    sendm_Data.z=recvm_Data.z;
    sendm_Data.source_Id=recvm_Data.source_Id;
    sendm_Data.destination_Id=recvm_Data.destination_Id;
    sendm_Data.s_checksum=recvm_Data.s_checksum;

    QByteArray dataToSend;
    //    dataToSend = QByteArray::fromRawData((char*)&m_Data,sizeof(m_Data));
    dataToSend = QByteArray::fromRawData(reinterpret_cast<char*>(&sendm_Data),sizeof(sendm_Data));
    qint64 sizeSent =  udpsend->writeDatagram(dataToSend,QHostAddress::LocalHost,5555);
    qDebug()<<"sizeSent is : "<<sizeSent;

}

