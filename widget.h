
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMouseEvent>
#include"mainwindow.h"
#include<QSystemTrayIcon>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget

{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


private slots:
    void on_btnmin_clicked();
    void loginskip();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);



private:
    Ui::Widget *ui;
    bool isDragging;
    QPoint dragPosition;


    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    //窗口管理动作
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;






};

#endif // WIDGET_H
