#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QString>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    virtual void showEvent(QShowEvent *event);
    QString getLoaclAddressIP();
    QString getServerIP();
    int getSererPort();

private slots:
    void on_close_clicked();

    void on_queding_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
