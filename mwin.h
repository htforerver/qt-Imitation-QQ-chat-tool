#ifndef MWIN_H
#define MWIN_H

#include <QWidget>
#include <QtGui>
#include<QSystemTrayIcon>

class MWin : public QWidget
{
    Q_OBJECT
public:
    explicit MWin(QWidget *parent = 0);
    void init();//初始化函数

    QSystemTrayIcon *tray;//托盘图标
    QMenu *menu;//托盘菜单
    QAction *reset;//菜单实现功能：恢复窗口
    QAction *quit;//菜单实现功能：退出程序

protected:
    void closeEvent(QCloseEvent *event);//由于要关闭窗口变为隐藏至托盘图标，所以要重写close事件

signals:

public slots:
    void TrayIconAction(QSystemTrayIcon::ActivationReason reason);//对托盘图标操作的槽：本代码实现单机图标恢复窗口功能
};

#endif // MWIN_H
