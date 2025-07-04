#include "login.h"
#include "ui_login.h"

Login::Login(QString* player_user, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{

    ui->setupUi(this);


    QDir dir("D:/nima/play");
    if (!dir.exists()) {
        QDir().mkpath("D:/nima/play");
    }

    UserManager* manager = UserManager::instance("D:/nima/play/users.txt");

    this->setStyleSheet("background-color: #d6e4e9;");
    this->resize(360, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 10);

    QWidget *greenBanner = new QWidget(this);
    greenBanner->setStyleSheet("background-color: #039555;");
    greenBanner->setFixedHeight(100);
    QLabel *label = new QLabel("Login", greenBanner);
    label->setStyleSheet("color: white; font-family:'Microsoft PhagsPa'; font-size: 45px;  font-weight: normal;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *bannerLayout = new QVBoxLayout(greenBanner);
    bannerLayout->addWidget(label);
    bannerLayout->setContentsMargins(0, 0, 0, 10);

    mainLayout->addWidget(greenBanner);
    mainLayout->addSpacing(10);
    mainLayout->addStretch();

    ///////////////////////////////////////
    /// \GET name and pass namelable
    ///

    QLineEdit* T_user2, *T_pass2;

    QWidget* namelable = createInputField("C:/Users/tech system/Desktop/hello.svg", "Username" , 25, T_user2);
    mainLayout->addWidget(namelable);
    QWidget* passlable = createInputField("C:/Users/tech system/Desktop/password.svg", "Password" , 25, T_pass2);
    mainLayout->addWidget(passlable);

    ///////////////////////////////////////
    /// \brief label2
    ///

    QLabel *label2 = new QLabel;
    label2->setTextFormat(Qt::RichText);
    label2->setText(R"(If you forgot your password, <a href="reset">click here</a>.)");
    label2->setOpenExternalLinks(false);
    label2->setStyleSheet(R"(
    QLabel {
        color: #039555;
        font-family:'Microsoft PhagsPa'; font-size: 12px;  font-weight: bold;

    }
    QLabel a {
      color: #0bdc84;
      text-decoration: none;
    }
    QLabel a:hover {
        color: #0bdc84;
        text-decoration: underline;
    }
    )");
    connect(label2, &QLabel::linkActivated, [=](const QString &link) {
        if (link == "reset") {
            showForgotPasswordDialog(this);
        }
    });

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(label2);
    layout->setContentsMargins(50, 10, 50, 20);
    mainLayout->addLayout(layout);

    ///////////////////////////////////////
    /// \brief layout
    ///

    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->setContentsMargins(50, 20, 50, 0);
    layout3->setSpacing(0);

    QPushButton *button = new QPushButton("login", this);
    button->setCursor(Qt::PointingHandCursor);

    button->setFixedHeight(40);
    button->setStyleSheet(R"(
    QPushButton {
        background-color: #039555;
        color: white;
        font-family:'Microsoft PhagsPa'; font-size: 20px;  font-weight: normal;
        border: none;
        border-radius: 10px;
        padding-bottom: 4px
    }
    QPushButton:hover {
        background-color: #0bdc84;
    }
    )");
    layout3->addWidget(button);
    mainLayout->addLayout(layout3);

    ///////////////////////////////////////
    /// \brief label2
    ///

    QLabel *label3 = new QLabel;
    label3->setTextFormat(Qt::RichText);
    label3->setText(R"( You haven't Accont yet, <a href="reset">sign up now!</a>)");
    label3->setOpenExternalLinks(false);
    label3->setStyleSheet(R"(
    QLabel {
        color: #039555;
        font-family:'Microsoft PhagsPa'; font-size: 12px;  font-weight: bold;

    }
    QLabel a:hover {
        color: #0c47a1;
        text-decoration: underline;
    }
    )");
    connect(label3, &QLabel::linkActivated, [=](const QString &link) {
        if (link == "reset") {
            this->hide();
            signUP();

            connect(this, &Login::signupFinished, this, [=]() {
                this->show();
            });
        }
    });

    //////////////////////////////////////////////////////////////
    /// \brief layout2
    ///

    QLabel *label4 = new QLabel;
    label4->setText("- نام کاربری یا رمز عبور اشتباه است ");
    label4->setStyleSheet(R"(
    QLabel {
        color: red;
        font-family:'Microsoft PhagsPa'; font-size: 12px;  font-weight: bold;

    }
    )");
    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(label3);
    layout2->setContentsMargins(50, 0, 50, 20);
    mainLayout->addLayout(layout2);

    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addWidget(label4);
    layout4->setContentsMargins(40, 0, 50, 20);
    mainLayout->addLayout(layout4);
    label4->hide();

    ///////////////////////////////
    ///
    ///

    connect(button, &QPushButton::clicked, [=](){


            // manager->loadFromFile();
        string SUname = T_user2->text().toStdString();
        User* tst = manager->findUser(SUname);

        if(tst != nullptr){

            qDebug() <<"user pass" << T_pass2->text().toStdString();

            if( tst->getPasswordHash() == Encryption::hashPassword(T_pass2->text().toStdString())){

                qDebug() << "Bomb Login Successful";
                label4->hide();

                User::currentname = QString::fromStdString( tst->getUsername() );

                emit loginSuccess();

            }
            else{

                label4->show();

                QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(label4);
                label4->setGraphicsEffect(effect);
                QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity");
                anim->setDuration(400);
                anim->setStartValue(0.0);
                anim->setEndValue(1.0);
                anim->setLoopCount(2);
                anim->start(QAbstractAnimation::DeleteWhenStopped);

            }
        }
        else{

            label4->show();


            QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(label4);
            label4->setGraphicsEffect(effect);

            QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity");
            anim->setDuration(400);
            anim->setStartValue(0.0);
            anim->setEndValue(1.0);
            anim->setLoopCount(2);
            anim->start(QAbstractAnimation::DeleteWhenStopped);

        }

    });

}

Login::~Login()
{
    delete ui;
}

void Login::showForgotPasswordDialog(QWidget *parent) {

    UserManager* manager = UserManager::instance("D:/nima/play/users.txt");

    QDialog *dialog = new QDialog(parent);

    dialog->setWindowTitle("Forgot Password");
    dialog->setModal(true);

    /////////////////////////////////
    ///
    ///

    dialog->setStyleSheet("background-color: #d6e4e9;");
    //dialog->setFixedHeight(300);
    dialog->resize(300, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->setContentsMargins(0, 0, 0, 10);

    ///////////////////////////////////////////////
    /// \brief greenBanner
    ///

    QWidget *greenBanner = new QWidget(dialog);
    greenBanner->setStyleSheet("background-color: #039555;");
    greenBanner->setFixedHeight(50);
    QLabel *label = new QLabel("Forget Password", greenBanner);
    label->setStyleSheet("color: white; font-family:'Microsoft PhagsPa'; font-size: 20px;  font-weight: normal;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *bannerLayout = new QVBoxLayout(greenBanner);
    bannerLayout->addWidget(label);
    bannerLayout->setContentsMargins(0, 0, 0, 10);

    mainLayout->addWidget(greenBanner);
    //mainLayout->addSpacing(50);
    mainLayout->addStretch();


    /////////////////////////////////////////////////
    /// \brief namelable
    ///
    QLineEdit* T_user, *T_friend;

    QWidget* namelable = createInputField("C:/Users/tech system/Desktop/hello.svg", "Username" , 25, T_user);
    mainLayout->addWidget(namelable);
    QWidget* passlable = createInputField("C:/Users/tech system/Desktop/heart.svg", "Your Best friend" , 25, T_friend);
    mainLayout->addWidget(passlable);

    ///////////////////////////////////////////////////
    /// \brief layout3
    ///

    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->setContentsMargins(50, 20, 50, 10);
    layout3->setSpacing(0);

    QPushButton *button = new QPushButton("Reset", this);
    button->setCursor(Qt::PointingHandCursor);

    button->setFixedHeight(40);
    button->setStyleSheet(R"(
    QPushButton {
        background-color: #039555;
        color: white;
        font-family:'Microsoft PhagsPa'; font-size: 20px;  font-weight: normal;
        border: none;
        border-radius: 10px;
        padding-bottom: 4px
    }
    QPushButton:hover {
        background-color: #0bdc84;
    }
    )");
    layout3->addWidget(button);
    mainLayout->addLayout(layout3);

    //////////////////////////////////////////////////////
    ///
    ///

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->setContentsMargins(50, 0, 50, 0);
    layout2->setSpacing(0);

    QTextEdit *errorBox = new QTextEdit(this);
    errorBox->setReadOnly(true);
    errorBox->setStyleSheet(R"(
    QTextEdit {
        background: transparent;
        color: red;
         font-size: 12px;  font-weight: normal;
        border: none;
        font-weight: bold;
    }

    )");

    layout2->addWidget(errorBox);
    mainLayout->addLayout(layout2);
    errorBox->hide();

    //////////////////////////////////////
    ///
    ///

    connect(button,&QPushButton::clicked, [=](){

        errorBox->clear();

        string SUname = T_user->text().toStdString();
        User* tst = manager->findUser(SUname);

        if(tst != nullptr){

            if( Encryption::hashPassword(T_friend->text().toStdString()) == tst->getEncryptedSecret()){
                dialog->close();

                ApdatePass(*tst);
            }
            else{

                errorBox->show();
                errorBox->append(" - اطاعات وارد شده صحیح نمی باشد\n");  // متن جدید به انتها اضافه میش
            }
        }
        else{

            errorBox->show();
            errorBox->append(" - اطاعات وارد شده صحیح نمی باشد\n");  // متن جدید به انتها اضافه میشه
        }

    });

    dialog->exec();
}



void Login::ApdatePass(User &user1){

    UserManager* manager = UserManager::instance();

    QDialog *dialog = new QDialog();

    dialog->setWindowTitle("Set new Password");
    dialog->setModal(true);

    /////////////////////////////////
    ///
    ///

    dialog->setStyleSheet("background-color: #d6e4e9;");
    //dialog->setFixedHeight(300);
    dialog->resize(340, 200);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->setContentsMargins(0, 0, 0, 10);

    ///////////////////////////////////////////////
    /// \brief greenBanner
    ///

    QWidget *greenBanner = new QWidget(dialog);
    greenBanner->setStyleSheet("background-color: #039555;");
    greenBanner->setFixedHeight(50);
    QLabel *label = new QLabel("Set new Password", greenBanner);
    label->setStyleSheet("color: white; font-family:'Microsoft PhagsPa'; font-size: 20px;  font-weight: normal;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *bannerLayout = new QVBoxLayout(greenBanner);
    bannerLayout->addWidget(label);
    bannerLayout->setContentsMargins(0, 0, 0, 10);

    mainLayout->addWidget(greenBanner);
    //mainLayout->addSpacing(50);
    mainLayout->addStretch();

    /////////////////////////////////////////////////
    /// \brief namelable
    ///
    QLineEdit* T_p1, *T_p2;

    QWidget* namelable = createInputField("C:/Users/tech system/Desktop/password.svg", "New Password" , 25 ,T_p1);
    mainLayout->addWidget(namelable);
    QWidget* passlable = createInputField("C:/Users/tech system/Desktop/password.svg", "Confim New Password" , 25, T_p2);
    mainLayout->addWidget(passlable);

    ///////////////////////////////////////////////////
    /// \brief layout3
    ///

    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->setContentsMargins(50, 20, 50, 10);
    layout3->setSpacing(0);

    QPushButton *button = new QPushButton("Reset", this);
    button->setCursor(Qt::PointingHandCursor);

    button->setFixedHeight(40);
    button->setStyleSheet(R"(
    QPushButton {
        background-color: #039555;
        color: white;
        font-family:'Microsoft PhagsPa'; font-size: 20px;  font-weight: normal;
        border: none;
        border-radius: 10px;
        padding-bottom: 4px
    }
    QPushButton:hover {
        background-color: #0bdc84;
    }
    )");
    layout3->addWidget(button);
    mainLayout->addLayout(layout3);

    //////////////////////////////////////////////////////
    ///
    ///

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->setContentsMargins(50, 0, 50, 0);
    layout2->setSpacing(0);

    QTextEdit *errorBox = new QTextEdit(this);
    errorBox->setReadOnly(true);
    errorBox->setStyleSheet(R"(
    QTextEdit {
        background: transparent;
        color: red;
         font-size: 12px;  font-weight: normal;
        border: none;
        font-weight: bold;
    }
    )");

    layout2->addWidget(errorBox);
    mainLayout->addLayout(layout2);
    errorBox->hide();


    connect(button,&QPushButton::clicked, [=, &user1](){

        errorBox->clear();

        if(T_p1->text() == nullptr || T_p2->text() == nullptr ){

            errorBox->show();
            errorBox->append(" -  هیچ کادری نمیتواند خالی باشد\n");  // متن جدید به انتها اضافه میش

        }
        else if(!isValidPassword(T_p1->text().toStdString())){

            errorBox->show();
            errorBox->append(" - رمز عبور ضعیف می باشد\n");  // متن جدید به انتها اضافه میش

        }
        else if(T_p1->text() != T_p2->text()){


            errorBox->show();
            errorBox->append(" - اطلاعات وارد شده یکسان نمی باشد\n");  // متن جدید به انتها اضافه میش
        }
        else if(T_p1->text() == T_p2->text()){

            user1.setPasswordHash(Encryption::hashPassword(T_p1->text().toStdString()));
            qDebug() << T_p1->text().toStdString();
            qDebug() << Encryption::hashPassword(T_p1->text().toStdString());
            manager->saveToFile();
            dialog->close();
        }
    });

    ////////
    dialog->exec();









}


QWidget* createInputField(const QString &iconPath, const QString &placeholderText, int iconSize, QLineEdit *&content ) {
    QWidget *fieldWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(fieldWidget);
    layout->setContentsMargins(50,20, 50, 0);
    layout->setSpacing(0);

    // آیکون
    QLabel *iconLabel = new QLabel();
    QIcon icon(iconPath);
    QPixmap pixmap = icon.pixmap(iconSize, iconSize);
    iconLabel->setPixmap(pixmap);
    iconLabel->setFixedSize(45, 45);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("background-color: #039555; border: none;");

    // فیلد متن
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setFixedHeight(45);
    lineEdit->setPlaceholderText(placeholderText);

    if(placeholderText == "Password" || placeholderText == "Confim your password" )
        lineEdit->setEchoMode(QLineEdit::Password);

    lineEdit->setStyleSheet(
        "background-color: white;"
        "border: none;"
        "font-size: 16px;"
        "padding: 6px;"
        );

    layout->addWidget(iconLabel);
    layout->addWidget(lineEdit);

    content = lineEdit;
    return fieldWidget;
}


void Login::signUP(){

    UserManager* manager = UserManager::instance();

    QWidget *signWidget = new QWidget();
    signWidget->show();

    signWidget->setStyleSheet("background-color: #d6e4e9;");
    //this->setFixedHeight(750);
    signWidget->resize(380, 600);


    QRect screen = QGuiApplication::primaryScreen()->geometry();
    QPoint center = screen.center() - signWidget->rect().center();
    center.setY(center.y() - 80); // 100 پیکسل بالاتر از وسط

    signWidget->move(center);


    QVBoxLayout *mainLayout = new QVBoxLayout(signWidget);
    mainLayout->setContentsMargins(0, 0, 0, 10);

    QWidget *greenBanner = new QWidget(signWidget);
    greenBanner->setStyleSheet("background-color: #039555;");
    greenBanner->setFixedHeight(100);

    QLabel *label = new QLabel("Sign Up", greenBanner);
    label->setStyleSheet("color: white; font-family:'Microsoft PhagsPa'; font-size: 40px;  font-weight: normal;");
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *bannerLayout = new QVBoxLayout(greenBanner);
    bannerLayout->addWidget(label);
    bannerLayout->addSpacing(10);
    bannerLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(greenBanner);
    mainLayout->addStretch();




    // اضافه به لایوت اصلی

    QLineEdit* T_user, *T_friend, *T_name, *T_Pass, *T_confim, *T_email;

    QWidget* namelable = createInputField("C:/Users/tech system/Desktop/smile.svg", "Your first name" , 25,T_name);
    mainLayout->addWidget(namelable);
    QWidget* usernamelable = createInputField("C:/Users/tech system/Desktop/hello.svg", "Username" , 25,T_user);
    mainLayout->addWidget(usernamelable);
    QWidget* confpasswlable = createInputField("C:/Users/tech system/Desktop/password.svg", "Password" , 25,T_Pass);
    mainLayout->addWidget(confpasswlable);
    QWidget* passwlable = createInputField("C:/Users/tech system/Desktop/password.svg", "Confim your password" , 25,T_confim);
    mainLayout->addWidget(passwlable);
    QWidget* emaillable = createInputField("C:/Users/tech system/Desktop/mail.svg", "Email Address" , 25 ,T_email);
    mainLayout->addWidget(emaillable);
    QWidget* friendlable = createInputField("C:/Users/tech system/Desktop/heart.svg", "Best friend name" , 25, T_friend);
    mainLayout->addWidget(friendlable);

    ////////////////////////////////////////////
    /// \brief layout
    ///

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(50, 20, 50, 20);
    layout->setSpacing(0);

    QPushButton *button = new QPushButton("Sign Up", signWidget);
    button->setCursor(Qt::PointingHandCursor);

    button->setFixedHeight(40);
    button->setStyleSheet(R"(
    QPushButton {
        background-color: #039555;
        color: white;
        font-family:'Microsoft PhagsPa'; font-size: 20px;  font-weight: normal;
        border: none;
        border-radius: 10px;
        padding-bottom: 4px
    }
    QPushButton:hover {
        background-color: #0bdc84;
    }
)");
    layout->addWidget(button);
    mainLayout->addLayout(layout);

    //////////////////////////////////////////////
    /// lable error
    ///

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->setContentsMargins(50, 0, 50, 20);
    layout2->setSpacing(0);
    QLabel *errorBox = new QLabel(signWidget);
    errorBox->setStyleSheet(R"(
    QLabel {
        background: transparent;
        color: red;
         font-size: 12px;  font-weight: normal;
        border: none;
        font-weight: bold;
    }
    )");

    errorBox->setText(" . رمز عبور خیلی کوتاه است");
    layout2->addWidget(errorBox);
    mainLayout->addLayout(layout2);
    //errorBox->show();

    errorBox->hide();

    connect(button, &QPushButton::clicked, [=](){

        qDebug()<< T_user->text().toStdString();

        /////// Animation Setting
        QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(errorBox);
        errorBox->setGraphicsEffect(effect);
        QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity");
        anim->setDuration(400);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setLoopCount(2);

        if(T_user->text() == "" || T_friend->text() == "" || T_name->text() == "" ||
            T_Pass->text() == "" || T_confim->text() == "" || T_email->text() == ""){

            errorBox->show();
            errorBox->setText(" . هیچ کادری نباید خالی باشد");
            anim->start(QAbstractAnimation::DeleteWhenStopped);


        }
        else if( !isValidInput(T_name->text().toStdString()) ){

            errorBox->show();
            errorBox->setText(" . اسم نامعتبر است");
            anim->start(QAbstractAnimation::DeleteWhenStopped);

        }
        else if( !isValidUsername(T_user->text().toStdString()) ){

            errorBox->show();
            errorBox->setText(" . نام کاربری نامعتبر یا تکراری است");
            anim->start(QAbstractAnimation::DeleteWhenStopped);

        }
        else if( T_Pass->text() != T_confim->text() ){

            errorBox->show();
            errorBox->setText(" . رمز و تکرارش یکسان نیست");
            anim->start(QAbstractAnimation::DeleteWhenStopped);

        }
        else if( !isValidPassword(T_Pass->text().toStdString()) ){

            errorBox->show();
            errorBox->setText(" . رمز عبور ضعیف است");
            anim->start(QAbstractAnimation::DeleteWhenStopped);

        }
        else if( !isValidEmail(T_email->text().toStdString()) ){

            errorBox->show();
            errorBox->setText(" . ایمیل وارد شده معتبر نمی باشد");
            anim->start(QAbstractAnimation::DeleteWhenStopped);

        }
        else if( !isValidInput(T_friend->text().toStdString()) ){

            errorBox->show();
            errorBox->setText(" .  نام دوست تنها حروف انگلیسی و بدون فاصله مجاز است");
            anim->start(QAbstractAnimation::DeleteWhenStopped);

        }
        else {

            errorBox->show();
            errorBox->setStyleSheet(R"(
        QLabel {
        background: transparent;
        color: green;
         font-size: 12px;  font-weight: normal;
        border: none;
        font-weight: bold;
        }
        )");
            errorBox->setText(" .  ثبت نام با موفقیت انجام شد!");

            manager->adduser( User(
                T_name->text().toStdString(),
                T_user->text().toStdString(),
                Encryption::hashPassword(T_Pass->text().toStdString()),
                T_email->text().toStdString(),
                Encryption::hashPassword(T_friend->text().toStdString())
                ));

            signWidget->hide();
            emit signupFinished();

        }

    });

}
