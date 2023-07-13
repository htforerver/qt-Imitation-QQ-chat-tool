#ifndef CHATWINDOWS_H
#define CHATWINDOWS_H

#include <QWidget>
#include"dialog.h"
#include<QTcpSocket>

namespace Ui {
class chatwindows;
}

class chatwindows : public QWidget
{
    Q_OBJECT

public:
    explicit chatwindows(QWidget *parent = nullptr);
    ~chatwindows();
    void tcpconnect();
    void dealClientRead();
    void dealpack(QByteArray arr);
private:
    Ui::chatwindows *ui;
    void on_btnmin_clicked();
    bool isDragging;
    QPoint dragPosition;

    Dialog m_dlg;
    QTcpSocket m_socket;
    QByteArray m_buffer;
    int m_ipackLen;


protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
private slots:
     void on_btntar_clicked();

    void on_btnsend_clicked();
};

#endif // CHATWINDOWS_H
