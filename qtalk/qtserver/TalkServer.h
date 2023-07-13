#ifndef TALKSERVER_H
#define TALKSERVER_H
#include<QObject>
#include<QTcpServer>
#include<QDebug>
#include<QTcpSocket>
class TalkServer:public QObject{
    Q_OBJECT

public:
    TalkServer(QObject *parent = nullptr):QObject(parent)
    {

    }
    ~TalkServer()
    {

    }
    void init()
    {
        m_tcpServer.listen(QHostAddress("192.168.2.222"),1234);
        connect(&m_tcpServer,&QTcpServer::newConnection,this,&TalkServer::dealNewCon);
        qDebug()<<"server running";

    }
    void dealNewCon()
    {

        auto *socket  = m_tcpServer.nextPendingConnection();
        qDebug()<<"my con"<<socket->peerAddress().toString();
        TcpInfo info{socket};
        m_ltcp<<info;
        connect(socket,&QTcpSocket::readyRead,this,&TalkServer::dealRead);
        connect(socket,&QTcpSocket::disconnected,this,[=]()
                {
             QTcpSocket *soctet = static_cast<QTcpSocket*>(sender());
            qDebug()<<"socket close"<<socket->peerAddress().toString();
            for(int i =0 ;i<m_ltcp.size();i++)
            {
                if(m_ltcp[i].socket ==soctet)
                {
                    m_ltcp.erase(m_ltcp.begin()+i);
                    break;
                }
            }
        });

    }

    void dealRead()
    {
        QTcpSocket *soctet = static_cast<QTcpSocket*>(sender());
        auto data = soctet->readAll();
        for(int i =0 ;i<m_ltcp.size();i++)
        {
            if(m_ltcp[i].socket ==soctet)
            {
                m_ltcp[i].arrDate.append(data);
                dealPack(i);
                break;
            }
        }

//            if(m_ltcp[i] ==soctet)
//            {
//                //0x02
//                continue;
//            }
//            //0x03
//        }

    }

    void dealPack(int i)
    {
        auto info = &m_ltcp[i];
        if(info->arrDate.size()>=4&&info->arrDate[0]==0x66)
        {
            qint16 len = (info->arrDate[2]<<8&0xFF00)| static_cast<qint16>(info->arrDate[1]);
            if(info->arrDate.size()>=len)
            {
                //取出完整的数据包
                QByteArray arr = info->arrDate.mid(0,len);
                dealRec(i,arr);
                info->arrDate = info->arrDate.mid(len);
            }
        }
    }

    void dealRec(int i,QByteArray arr)
    {
         auto info = m_ltcp[i];


    char cmd =arr[3];
    QByteArray arrDate = arr.mid(4);
    do
    {
         if(cmd == 0x01)
          {
        //心跳包反馈，计时
        break;
         }

        if(cmd == 0x03)
         {

        auto  strIP =  info.socket->peerAddress().toString();
        qDebug()<<strIP<<":"<<QString(arrDate);
            //发送给当前连接所有的用户客服端数据包
        arrDate.insert(0,strIP.toUtf8()+":");

         QByteArray arrsendBuffer;//除发送者外的 0x03
        arrsendBuffer.append(0x66);
         quint16 l = arrDate.size()+4;
        arrsendBuffer.append(static_cast<char>(l&0x00FF));
         arrsendBuffer.append(static_cast<char>(l>>8&0x00FF));
        arrsendBuffer.append(0x03);
         arrsendBuffer.append(arrDate);
        for(int i= 0;i<m_ltcp.size();i++)
         {
             if(m_ltcp[i].socket==info.socket)
            {
                arrsendBuffer[3] = 0x02;
                 m_ltcp[i].socket->write(arrsendBuffer);
                continue;
            }
             arrsendBuffer[3] = 0x03;
             m_ltcp[i].socket->write(arrsendBuffer);
         }

             break;
         }

    }while(false);

    }
private:
    QTcpServer  m_tcpServer;
    struct TcpInfo
    {
        QTcpSocket  *socket;
        QByteArray arrDate;
    };
    QList<TcpInfo> m_ltcp;
};
#endif // TalkServer_H

