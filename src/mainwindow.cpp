#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentUsername = User::currentname;


    this->setWindowTitle("Music Player");
    ui->h_slider->setCursor(Qt::PointingHandCursor);
    ui->PlayButton->setIcon(QIcon("C:/Users/tech system/Downloads/Vector.svg"));
    ui->PlayButton->setIconSize(QSize(60,60));
    ui->PlayButton->setFixedSize(80,80);  // دکمه کوچک فقط برای آیکون
    ui->PlayButton->setFlat(true);
    ui->PlayButton->setCursor(Qt::PointingHandCursor);
    ui->nextButton->setIcon(QIcon("C:/Users/tech system/Downloads/Group.svg"));
    ui->nextButton->setIconSize(QSize(25,25));
    ui->nextButton->setFixedSize(40,40);  // دکمه کوچک فقط برای آیکون
    ui->nextButton->setFlat(true);
    ui->nextButton->setCursor(Qt::PointingHandCursor);
    ui->cover_buttom->setStyleSheet(R"(
    QPushButton {
        border: none;
        background-color: #337180;
        border-radius: 0px;
        padding: 0px;
    }
    )");

    ui->backButton->setIcon(QIcon("C:/Users/tech system/Downloads/Group (2).svg"));
    ui->backButton->setIconSize(QSize(25,25));
    ui->backButton->setFixedSize(40,40);  // دکمه کوچک فقط برای آیکون
    ui->backButton->setFlat(true);
    ui->backButton->setCursor(Qt::PointingHandCursor);
    ui->Vol_Button->setIcon(QIcon("C:/Users/tech system/Downloads/Group Copy.svg"));
    ui->Vol_Button->setIconSize(QSize(25,25));
    ui->Vol_Button->setFixedSize(30,30);  // دکمه کوچک فقط برای آیکون
    ui->Vol_Button->setFlat(true);
    ui->Vol_Button->setCursor(Qt::PointingHandCursor);
    ui->Vol_Button->setDisabled(true);
    ui->AddPlayList_Button->setIcon(QIcon("C:/Users/tech system/Downloads/Library.svg"));
    ui->AddPlayList_Button->setIconSize(QSize(25,25));
    ui->AddPlayList_Button->setFixedSize(30,30);  // دکمه کوچک فقط برای آیکون
    ui->AddPlayList_Button->setFlat(true);
    ui->AddPlayList_Button->setCursor(Qt::PointingHandCursor);
    ui->Repeat_Button->setCursor(Qt::PointingHandCursor);
    ui->Shuffle_Button->setCursor(Qt::PointingHandCursor);
    ui->Eco_Button->setCursor(Qt::PointingHandCursor);
    ui->Eco_Button->hide();
    ui->PlayButton->setEnabled(false);
    ui->nextButton->setEnabled(false);
    ui->backButton->setEnabled(false);
    ui->Shuffle_Button->setEnabled(false);
    ui->Repeat_Button->setEnabled(false);
    ui->Eco_Button->setEnabled(false);
    ui->h_slider->setAttribute(Qt::WA_TransparentForMouseEvents);

    /////////////////////////////////////////////////////////////////////////////////////////////////

    musicPlayer = new MusicPlayer(this);

    musicPlayer->loadUserPlaylists(currentUsername);

    QStringList loadedList = musicPlayer->loadMusicFromFolder("D:/UserTrack");
    musicPlayer->loadPlaylist("default", ui->trackList,"D:/UserTrack", ui->PLaylist_name);


    musicPlayer->setUI(ui);


    //////////////////////////////
    ///
    ///
    ///

    ui->Vol_Slider->setRange(0, 100);
    ui->Vol_Slider->setValue(50);
    musicPlayer->setVolume(50);

    connect(ui->trackList, &QListWidget::itemClicked ,
            this, &MainWindow::on_listWidget_itemDoubleClicked);

    connect(ui->PlayButton, &QPushButton::clicked,
            this, &MainWindow::on_btnPlayPause_clicked);

    connect(ui->Vol_Slider, &QSlider::valueChanged, this, [this](int value){
        musicPlayer->setVolume(value);
    });

    ui->h_slider->installEventFilter(this);

    ///
    ///
    ///


    connect(musicPlayer->player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);

    connect(musicPlayer->player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    connect(ui->h_slider, &QSlider::sliderMoved, this, &MainWindow::setPosition);

    ////////////////////////////////////////////////////////////////////
    connect(ui->nextButton, &QPushButton::clicked, this, [this]() {
        musicPlayer->playNext(ui->trackList, ui->PlayButton, isPlaying);
    });

    connect(ui->backButton, &QPushButton::clicked, this, [this]() {
        musicPlayer->playBack(ui->trackList, ui->PlayButton,isPlaying);
    });

    connect(ui->AddPlayList_Button, &QPushButton::clicked, this, [this]() {
        musicPlayer->new_Playlist(ui->trackList, ui->PLaylist_name);
    });

    ////////////////////////////
    /// \brief connect
    ///

    connect(musicPlayer, &MusicPlayer::songFinished,
            this, &MainWindow::onSongFinished);

    connect(ui->Repeat_Button, &QPushButton::clicked, this, &MainWindow::handleClickRepeat);

    connect(ui->Shuffle_Button, &QPushButton::clicked, this, &MainWindow::handleClickShuffle);

    ui->trackList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->trackList, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);


    connect(sync, &SimpleSync::playRequested, this, [&](const QString &msg) {
        logToFile("WTF");
        logToFile(msg);

        QStringList parts = msg.split("::");

        if (parts.size() < 1) {
            qDebug() << "Invalid playRequested message format:" << msg;
            logToFile("Invalid playRequested message format:");

            return;
        }

        QString path = parts[0];
        qint64 position = 0;
        if (parts.size() >= 2) {
            position = parts[1].toLongLong();
        }

        qDebug() << "This Song is playing:" << path << "from position (seconds):" << position;
        logToFile("This Song is playing: " + path + " from position: " + QString::number(position));

        ui->PlayButton->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));
        isPlaying = true;

        musicPlayer->play(path, position);
    });



    connect(sync, &SimpleSync::stopRequested, this, [&](){
        qDebug() << " pause song :";
        logToFile("pause song");

        ui->PlayButton->setIcon(QIcon("C:/Users/tech system/Downloads/Vector.svg"));
        isPlaying = false;

        musicPlayer->pause();
    });


    connect(ui->Eco_Button_2, &QPushButton::clicked,[=](){


        if(sync->startServer(12345)) {
            isSender = true;
            qDebug() << "Started as sender";
        }

    });


    connect(ui->Eco_Button_3, &QPushButton::clicked,[=](){


        sync->connectToServer("127.0.0.1", 12345);
        isSender = false;
        qDebug() << "Started as receiver";


    });


    eq = new EqualizerWidget(this);
    eq->setFixedSize(600, 300); // یا هر اندازه‌ای که خواستی
    ui->horizontalLayout_2->addWidget(eq); // یا: ui->verticalLayout->addWidget(eq);
    eq->hide();


    connect(musicPlayer, &MusicPlayer::requestHideEq, eq, &EqualizerWidget::hide);
    connect(musicPlayer, &MusicPlayer::requestShowEq, eq, &EqualizerWidget::show);


}



void MainWindow::showContextMenu(const QPoint &pos) {
    QListWidgetItem *item = ui->trackList->itemAt(pos);
    if (!item) return;

    QMenu contextMenu;
    contextMenu.setStyleSheet(
        "QMenu {"
        "  background-color: #d6e4e9;"
        "  border: 1px solid gray;"
        "  padding: 0px;"               // فضای داخلی کل منو
        "}"
        "QMenu::item {"
        "  padding: 4px 8px;"           // فاصله اطراف متن آیتم‌ها (بالا/پایین، چپ/راست)"
        "  margin: 0px;"                // فاصله بین آیتم‌ها
        "}"
        "QMenu::item:selected {"
        "  background-color: #e0e0e0;"  // انتخاب آیتم
        "}"
        );
    QAction *addToQueue = contextMenu.addAction("افزودن به صف پخش");
    QAction *addToPlaylist = contextMenu.addAction("افزودن به پلی‌لیست");

    QAction *selectedAction = contextMenu.exec(ui->trackList->mapToGlobal(pos));

    if (selectedAction == addToQueue) {

        QString fullPath = item->data(Qt::UserRole).toString();
        musicPlayer->enqueueSong(fullPath);

        if(musicPlayer->currentIndex == -1){
            ui->trackList->clearSelection();
        }
        else{
        ui->trackList->setCurrentRow(musicPlayer->currentIndex);
        }

        QMessageBox::information(this, "افزوده شد", "آهنگ به صف پخش اضافه شد.");
    }
    else if (selectedAction == addToPlaylist) {

        QString fullPath = item->data(Qt::UserRole).toString();
        QString tmp = musicPlayer->currentSongaddres;


        musicPlayer->currentSongaddres = fullPath;

        musicPlayer->addSongToPlaylist();

        musicPlayer->currentSongaddres = tmp;

        if(musicPlayer->currentIndex == -1){
            ui->trackList->clearSelection();
        }
        else{
            ui->trackList->setCurrentRow(musicPlayer->currentIndex);
        }
    }
}

void MainWindow::handleClickRepeat() {

    stateRepeat = (stateRepeat + 1) % 3;

    switch (stateRepeat) {
    case 0:

        ui->Repeat_Button->setText("Repeat:   None");

        musicPlayer->SetRepeatListEnabled(false);
        musicPlayer->SetRepeatSongEnabled(false);

        break;
    case 1:
        ui->Repeat_Button->setText("Repeat:  Song");

        musicPlayer->SetRepeatListEnabled(false);
        musicPlayer->SetRepeatSongEnabled(true);

        break;
    case 2:
        ui->Repeat_Button->setText("Repeat:  Playlist");

        musicPlayer->SetRepeatListEnabled(true);
        musicPlayer->SetRepeatSongEnabled(false);

        break;
    }

}

void MainWindow::handleClickShuffle() {

    stateShuffle = (stateShuffle + 1) % 2;

    switch (stateShuffle) {
    case 0:

        ui->Shuffle_Button->setText("Shuffle: off");

        musicPlayer->SetShuffleEnabled(false);

        break;
    case 1:
        ui->Shuffle_Button->setText("Shuffle: On");

        musicPlayer->SetShuffleEnabled(true);

        break;
    }
}

void MainWindow::onSongFinished()
{
    // اینجا وقتی آهنگ تموم شد هر کاری میخوای انجام بده
    qDebug() << "Song finished!";

    musicPlayer->playNextAuto(ui->trackList, ui->PlayButton, isPlaying);
    // مثلا بگو آهنگ بعدی را پلی کن
    // یا دکمه ها را آپدیت کن
}

MainWindow::~MainWindow()
{
    delete ui;}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    eq->show();
    musicPlayer->setCurrentIndex(item);

    QString filePath = item->data(Qt::UserRole).toString();
    isPlaying = true;

    if(isSender) {


        sync->sendPlayCommand(filePath, 0);
    }


    musicPlayer->play(filePath);
    ui->PlayButton->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));
    musicPlayer->play(filePath);

}

void MainWindow::on_btnPlayPause_clicked()
{
    if (isPlaying) {

        eq->hide();

        if(isSender) {

            sync->sendStopCommand();
        }
        musicPlayer->pause();
        ui->PlayButton->setIcon(QIcon("C:/Users/tech system/Downloads/Vector.svg"));
        isPlaying = false;
    } else {

        eq->show();
            if(isSender) {

                qint64 positionNow;

                positionNow = musicPlayer->player->position();
                sync->sendPlayCommand(musicPlayer->currentSongaddres, positionNow);
            }

            qint64 positionNow;

            positionNow = musicPlayer->player->position();

        musicPlayer->play(musicPlayer->currentSongaddres ,positionNow);
          //  musicPlayer->play(); دیلی در استپ دادن را حذف میکنه

        ui->PlayButton->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));
        isPlaying = true;
    }
}

void MainWindow::durationChanged(qint64 duration)
{
    ui->h_slider->setMaximum(duration);

    // نمایش زمان کل در لیبل
    updateTimeLabel(ui->h_slider->value(), duration);
}

void MainWindow::positionChanged(qint64 position)
{
    if (!ui->h_slider->isSliderDown()) // فقط وقتی کاربر اسلایدر رو نمی‌کشه
        ui->h_slider->setValue(position);

    updateTimeLabel(position, ui->h_slider->maximum());
}

void MainWindow::setPosition(int position)
{
    musicPlayer->player->setPosition(position);
}

QString MainWindow::formatTime(qint64 timeMilliSeconds)
{
    int seconds = (timeMilliSeconds / 1000) % 60;
    int minutes = (timeMilliSeconds / 60000) % 60;
    int hours = (timeMilliSeconds / 3600000);

    if (hours > 0)
        return QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    else
        return QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
}

void MainWindow::updateTimeLabel(qint64 position, qint64 duration)
{
    QString posStr = formatTime(position);
    QString durStr = formatTime(duration);
    ui->label_time->setText(posStr + " / " + durStr);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->h_slider && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QStyleOptionSlider opt;
        opt.initFrom(ui->h_slider);
        QRect groove = ui->h_slider->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, ui->h_slider);
        QRect handle = ui->h_slider->style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, ui->h_slider);
        int sliderMin = groove.x();
        int sliderMax = groove.right() - handle.width() + 1;
        double pos = mouseEvent->position().x();
        int newVal = QStyle::sliderValueFromPosition(ui->h_slider->minimum(), ui->h_slider->maximum(), pos - sliderMin, sliderMax - sliderMin);
        ui->h_slider->setValue(newVal);
        musicPlayer->player->setPosition(newVal); // <-- پخش‌کننده رو به اون نقطه ببر
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // اینجا مثلاً فایل ذخیره کن یا هر کاری
    musicPlayer->saveUserPlaylists(currentUsername);

   // musicPlayer->savePlaylist();  // مثلاً ذخیره پلی‌لیست

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Exit",
                                  "آیا می‌خواهید خارج شوید؟",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // مثلاً ذخیره‌سازی قبل از خروج
       // musicPlayer->savePlaylist();  // اگر چنین تابعی داری
        event->accept(); // اجازه بستن
    } else {
        event->ignore(); // جلو بستن رو بگیر
    }
}



