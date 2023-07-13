#include "dialog.h"
#include "ui_dialog.h"
#include<QDebug>
#include <QNetworkInterface>



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->close,&QPushButton::clicked,this,&Dialog::close);
    auto l = QNetworkInterface::allAddresses();
    for(auto it:l)
    {
        if(QAbstractSocket::IPv4Protocol == it.protocol())
            ui->cb_ips->addItem(it.toString());

    }
}

Dialog::~Dialog()
{
    delete ui;

}

void Dialog::showEvent(QShowEvent *event)
{
    ui->cb_ips->clear();

    auto l = QNetworkInterface::allAddresses();
    for(auto it:l)
  {
        if(QAbstractSocket::IPv4Protocol == it.protocol())
        ui->cb_ips->addItem(it.toString());

    }
}

QString Dialog::getLoaclAddressIP()
{
    return ui->cb_ips->currentText();
}

QString Dialog::getServerIP()
{
    return ui->le_server->text();
}

int Dialog::getSererPort()
{
    return ui->le_port->text().toInt();
}

void Dialog::on_close_clicked()
{
    this->done(0);
}


void Dialog::on_queding_clicked()
{
    this->done(1);
}

