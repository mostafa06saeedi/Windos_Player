#ifndef LOGIN_H
#define LOGIN_H

#include "mainwindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QDialog>
#include <QMessageBox>
#include "usermanager.h"
#include <QLineEdit>
#include <QDebug>
#include "encryption.h"
#include <QScreen>
#include <QGuiApplication>
#include <QThread>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QString* player_user,QWidget *parent = nullptr);
    ~Login();

    void showForgotPasswordDialog(QWidget *parent = nullptr);
    void ApdatePass(User &user1);
    void signUP();

private:
    Ui::Login *ui;

signals:
    void signupFinished();
    void loginSuccess();

};

QWidget* createInputField(const QString &iconPath, const QString &placeholderText, int iconSize, QLineEdit *&content );


#endif // LOGIN_H
