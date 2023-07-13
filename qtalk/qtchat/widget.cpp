
#include "widget.h"
#include "./ui_widget.h"
#include"mainwindow.h"
#include"chatwindows.h"
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

QString user,password;


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    //去掉边框并实现在任务栏窗口隐藏功能
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    ui->setupUi(this);
    //添加动画效果
    QMovie *movie = new QMovie(this);
    movie->setFileName(":/imgs/R.gif");
    movie->start();
    ui->label->setMovie(movie);

    //信号槽，实现最小化的功能
    connect(ui->btnmin,&QPushButton::clicked,this,&Widget::on_btnmin_clicked);
    //信号槽，实现关闭窗口的功能
    connect(ui->btnclose,&QPushButton::clicked,this,&Widget::close);
    //跳转到注册功能页面   待实现
    connect(ui->btnreg,&QPushButton::clicked,[=](){
        const QUrl regUrl(QLatin1String("http://www.baidu.com"));
        QDesktopServices::openUrl(regUrl);
    });
    //跳转到找回密码功能页面   待实现
    connect(ui->btnpassword,&QPushButton::clicked,[=](){
        const QUrl regUrl(QLatin1String("http://www.baidu.com"));
        QDesktopServices::openUrl(regUrl);
    });


    //初始化系统托盘
   QIcon icon = QIcon(":/imgs/loginlogo.png");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("a trayicon example");
    trayIcon->show(); //必须调用，否则托盘图标不显示

    //创建菜单项动作(以下动作只对windows有效)
    minimizeAction = new QAction("最小化~", this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide())); //绑定信号槽

    maximizeAction = new QAction("最大化~", this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction("还原~", this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction("退出~", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit())); //关闭应用，qApp对应的是程序全局唯一指针

    //创建托盘菜单(必须先创建动作，后添加菜单项，还可以加入菜单项图标美化)
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);


    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));




    //账号提示和账号logo
    ui->username->lineEdit()->setPlaceholderText("账号");

    QAction *action1 = new QAction();
    action1->setIcon(QIcon(":/imgs/loginlogo.png"));
    ui->username->lineEdit()->addAction(action1,QLineEdit::LeadingPosition);
    //密码logo
    QAction *action2 = new QAction();
    action2->setIcon(QIcon(":/imgs/password.png"));
    ui->password->addAction(action2,QLineEdit::LeadingPosition);


    //头像设置
    ui->touxiang->setMask(QRegion(ui->touxiang->rect(),QRegion::RegionType::Ellipse));

    //信号槽函数实现主页面跳转功能,loginskip为实现的函数，实现关闭该登录页面，跳转到主页面
    connect(ui->btnlogin,&QPushButton::clicked,this,&Widget::loginskip);

}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_btnmin_clicked()
{
    //实现最小化的函数
    this->showMinimized();
}

void Widget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        dragPosition = event-> globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void Widget::mouseMoveEvent(QMouseEvent* event)
{
    if (isDragging && (event->buttons() & Qt::LeftButton))
    {

        move(event-> globalPosition().toPoint() - dragPosition);

        event->accept();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = false;
        event->accept();
    }
}

void Widget::loginskip()
{

    user = ui->username->currentText();
    password = ui->password->text();
    if(user=="root"&&password=="123456")
    {
        QMessageBox::information(this,"tip","登录成功");
        this->close();
        chatwindows * pic = new chatwindows();
        pic->show();
    }
    else
    {
        QMessageBox::information(this,"tip","登录失败");
    }
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal(); // 显示窗口
        break;
    default:
        break;
    }
}








