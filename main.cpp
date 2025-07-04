#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#include "UserManager.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login loginWindow(nullptr);
    MainWindow mainWindow;






    // ساخت ویجت اینترو
    QWidget *intro = new QWidget(nullptr, Qt::FramelessWindowHint | Qt::Dialog);
    intro->setStyleSheet("background-color: #039555; color: white;");
    intro->setFixedSize(300, 200);

    // مرکز صفحه
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    intro->move(screenGeometry.center() - intro->rect().center());

    // نوشته‌ی خوش آمدید
    QVBoxLayout *layout = new QVBoxLayout(intro);
    QLabel *label = new QLabel("خوش آمدید", intro);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 32px;background-color: #039555; font-weight: normal;");
    layout->addStretch();
    layout->addWidget(label);
    layout->addStretch();

    intro->setLayout(layout);
    intro->hide();



    QObject::connect(&loginWindow, &Login::loginSuccess, [&]() {
        loginWindow.hide();        // یا delete &loginWindow اگر دیگه نمی‌خوایش

        label->setText("Hello " + User::currentname + "!");
        intro->show();

        // بعد از ۳ ثانیه ببند و برنامه اصلی رو نشون بده
        QTimer::singleShot(2000, [&]() {
            intro->close();

            mainWindow.show();

        });


    });


    loginWindow.show();

    return a.exec();
}







