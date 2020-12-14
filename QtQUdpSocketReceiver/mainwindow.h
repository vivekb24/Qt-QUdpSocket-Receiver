#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QUdpSocket>
#include <QListWidget>
#include <QDebug>
#include <QFile>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readDataFrom1234();
private:
    Ui::MainWindow *ui;
    QUdpSocket *udpsend;
    QUdpSocket *udprecev;
    QByteArray BufferData;
    QHostAddress senderip;
    quint16 senderportno;

    struct Proj_Data{
        int source_Id;
        int destination_Id;
        int msg_Id;
        int x;
        int y;
        int z;
        int s_checksum;
    }__attribute__((packed));
    Proj_Data recvm_Data,sendm_Data;
};

#endif // MAINWINDOW_H
