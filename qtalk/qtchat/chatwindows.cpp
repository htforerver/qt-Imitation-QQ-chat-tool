#include "chatwindows.h"
#include "ui_chatwindows.h"
#include<QMouseEvent>
#include<QMovie>
#include <QLabel>
#include<QString>
#include<QMessageBox>
#include<QDesktopServices>
#include <QString>
#include <QCoreApplication>
#include <QObject>
#include <QSystemTrayIcon>
#include<QMenu>
#include<QDialog>
#include<QTcpSocket>
#include"dialog.h"

chatwindows::chatwindows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatwindows)
{
    setWindowFlags(  Qt::Window | Qt::FramelessWindowHint);

    ui->setupUi(this);
    auto fcon  =[&](){
        auto ip  = m_socket.localAddress().toString();
        ui->textBrowser->append(ip+"连接成功");
    };
    connect(&m_socket,&QAbstractSocket::connected,this,fcon);


    auto fdcon  =[&](){
        auto ip  = m_socket.localAddress().toString();
        ui->textBrowser->append(ip+"连接失败");
    };
    connect(&m_socket,&QAbstractSocket::disconnected,this,fdcon);
    connect(&m_socket,&QAbstractSocket::readyRead,this,&chatwindows::dealClientRead);


    //信号槽，实现最小化的功能
    connect(ui->btnmin1,&QPushButton::clicked,this,&chatwindows::on_btnmin_clicked);
    //信号槽，实现关闭窗口的功能
    connect(ui->btnclose1,&QPushButton::clicked,this,&chatwindows::close);
    connect(ui->btnclose2,&QPushButton::clicked,this,&chatwindows::close);
//    connect(ui->btntar,&QPushButton::clicked,[=](){

//        Dialog dlg;
//        dlg.exec();
//    });
//


   //关闭按钮样式
    ui->btnclose2->setStyleSheet("QPushButton{image: url(:/imgs/close.png);}"
                                 "QPushButton:hover{background-color: rgb(255, 84, 57);}"
                                 "QPushButton{border:none;}");
    ui->btnmin1->setStyleSheet(  "QPushButton{image: url(:/imgs/min.png);}"
                                 "QPushButton:hover{ background-color:rgba(190, 190 , 190,200);}"
                                 "QPushButton{border:none;}");
    ui->btnmax->setStyleSheet(   "QPushButton{image: url(:/imgs/max.png);}"
                                 "QPushButton:hover{ background-color:rgba(190, 190 , 190,200);}"
                                 "QPushButton{border:none;}");
    ui->btntar->setStyleSheet(   "QPushButton{image: url(:/imgs/pull.png);}"
                                 "QPushButton:hover{ background-color:rgba(190, 190 , 190,200);}"
                                 "QPushButton{border:none;}");
    ui->btnclose1->setStyleSheet( "QPushButton{ background-color:rgb(190, 190 , 190);}"
                                 "QPushButton:hover{ background-color:rgba(190, 190 , 190,200);}");

    ui->btnsend->setStyleSheet( "QPushButton{ background-color: rgb(205,192,178);}"
                                "QPushButton:hover{ background-color:rgba(190, 190 , 190,200);}");

}

chatwindows::~chatwindows()
{
    delete ui;
}

void chatwindows::tcpconnect()
{
    auto local_ip = m_dlg.getLoaclAddressIP();
    auto server_ip = m_dlg.getServerIP();
    auto server_port = m_dlg.getSererPort();
    m_socket.close();
    m_socket.bind(QHostAddress(local_ip));
    m_socket.connectToHost(server_ip,server_port);

}

void chatwindows::dealClientRead()
{

    m_buffer.push_back( m_socket.readAll());
    if(m_buffer.size()>=4&&m_buffer[0]==0x66)
    {
        qint16 len = (m_buffer[2]<<8&0xFF00)| static_cast<qint16>(m_buffer[1]);
        if(m_buffer.size()>=len)
        {
            //取出完整的数据包
            QByteArray arr = m_buffer.mid(0,len);
            dealpack(arr);
            m_buffer = m_buffer.mid(len);
        }
    }

}

void chatwindows::dealpack(QByteArray arr)
{
        //拿到了一包数据
    char cmd =arr[3];
    QByteArray arrDate = arr.mid(4);
    do
    {
         if(cmd == 0x01)
          {
        //心跳包反馈，计时
        break;
         }
auto strtime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
         if(cmd == 0x02)
         {

        //我发送的数据包

        ui->textBrowser->append("<font color = 'green'>"+strtime+"我发送:"+arrDate);
            break;
         }
        if(cmd == 0x03)
         {
        //我接受的数据包
             ui->textBrowser->append("<font color = 'blue'>"+strtime+"我接收:"+arrDate);
             break;
         }

    }while(false);


}
void chatwindows::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        dragPosition = event-> globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void chatwindows::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging && (event->buttons() & Qt::LeftButton))
    {

        move(event-> globalPosition().toPoint() - dragPosition);

        event->accept();
    }
}

void chatwindows::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = false;
        event->accept();
    }
}
//实现最小化功能
void chatwindows::on_btnmin_clicked()
{
    //实现最小化的函数
    this->showMinimized();
}

void chatwindows::on_btntar_clicked()
{
     int ret = m_dlg.exec();
    if(ret){
        tcpconnect();
    qDebug()<<"本地IP地址   "<<m_dlg.getLoaclAddressIP();
    qDebug()<<"服务器IP地址 "<<m_dlg.getServerIP();
    qDebug()<<"服务器端口号 "<<m_dlg.getSererPort();
    }
}

void chatwindows::on_btnsend_clicked()
{
    auto str  = ui->textEdit->toPlainText();
    ui->textEdit->clear();

    QByteArray arr = str.toUtf8();
    quint16 l = arr.size()+4;
    arr.insert(0,0x66);
    arr.insert(1,static_cast<char>(l&0x00FF));
    arr.insert(2,static_cast<char>(l>>8&0x00FF));
    arr.insert(3,0x03);
    m_socket.write(arr);

}

