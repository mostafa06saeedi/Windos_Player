#include"musicplayer.h"

MusicPlayer::MusicPlayer(QObject *parent) : QObject(parent)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0); // صدای کامل


    connect(player, &QMediaPlayer::mediaStatusChanged,
            this, &MusicPlayer::handleMediaStatusChanged);


}


void MusicPlayer::handleMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        emit songFinished();
    }
}


void MusicPlayer::play(const QString &filePath, qint64 positionInSeconds)
{
    logToFile("in Play This Song is input: " + filePath + " input from position: " + QString::number(positionInSeconds));

    ui->PlayButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
    ui->backButton->setEnabled(true);
    ui->Shuffle_Button->setEnabled(true);
    ui->Repeat_Button->setEnabled(true);
    ui->Eco_Button->setEnabled(true);
    ui->h_slider->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    ui->label_start->hide();

    QFileInfo info(filePath);
    QString baseName = info.completeBaseName();
    ui->label_name->setText(baseName);

    ui->cover_buttom->setIcon(QIcon("C:/Users/tech system/Downloads/music2.svg"));
    ui->cover_buttom->setIconSize(QSize(110, 110));
    ui->cover_buttom->setFixedSize(110, 110);

    // پخش اول
    player->setSource(QUrl::fromLocalFile(filePath));
    player->setPosition(positionInSeconds );

    logToFile("Play 1 after pos/ " + QString::number(positionInSeconds));
    player->play();

    currentSongaddres = filePath;

    // (گول زدن)
    QTimer::singleShot(200, [this, positionInSeconds, filePath]() {
        player->setSource(QUrl::fromLocalFile(filePath));
        player->setPosition(positionInSeconds);  // تنظیم موقعیت قبل از پخش
        logToFile("Play 2 after pos/ " + QString::number(positionInSeconds) );
        requestShowEq();
        player->play();
    });
}


void MusicPlayer::pause()
{

    emit requestHideEq();
    player->pause();

    QTimer::singleShot(100, [this]() {
        player->pause();
    });
}

void MusicPlayer::play()
{

    player->play();
}

void MusicPlayer::setVolume(int volumePercent)
{
    float volume = volumePercent / 100.0f;  // تبدیل 0-100 به 0.0-1.0
    audioOutput->setVolume(volume);
}

void MusicPlayer::playAtIndex(int index) {
    const QStringList &playlist = playlists[currentPlaylistName];

    qDebug() << "playAtIndex called with index:" << index;

    if (index >= 0 && index < playlist.size()) {
        currentIndex = index;
        play(playlist[index]);
    }
}

void MusicPlayer::setUI(Ui::MainWindow *uiPtr) {
    this->ui = uiPtr;
}

void MusicPlayer::playNext(QListWidget*& trackList, QPushButton*& icon, bool& isPlaying) {
    const QStringList& playlist = playlists[currentPlaylistName];
    int index;

    if (playlist.isEmpty()) {
        qDebug() << "لیست پخش خالیه!";
        return;
    }

    if (!playbackQueue.isEmpty()) {

        trackList->clearSelection();

        QString nextSong = playbackQueue.takeFirst();
        isPlaying = true;
        play(nextSong);
        icon->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));

        // ست‌کردن آیتم مرتبط در لیست
        for (int i = 0; i < trackList->count(); ++i) {
            QListWidgetItem *item = trackList->item(i);
            QString itemPath = item->data(Qt::UserRole).toString();
            if (itemPath == nextSong) {
                trackList->setCurrentRow(i);
                break;
            }
        }

        return;
    }
    else{

    if (ShuffleEnabled) {

        if ( currentIndex < playlist.size()) {
            playedIndices.insert(currentIndex);
        }
        if ( ( playedIndices.size() == playlist.size() ) && !repeatListEnabled ) {

            player->stop();
            emit requestHideEq();

            icon->setIcon(QIcon("C:/Users/tech system/Downloads/Vector.svg"));
            isPlaying = false;
            QMessageBox::information(
                nullptr,
                "پایان لیست",
                "همه آهنگ‌های پلی لیست پخش شده‌اند!\nدکمه تکرار را فعال کنید یا پلی‌لیست جدید انتخاب نمایید."
                );
            return;
        }
        else if ( ( playedIndices.size() == playlist.size() ) && repeatListEnabled ) {

            playedIndices.clear();

        }

        do {
            index = QRandomGenerator::global()->bounded(playlist.size());
        } while (playedIndices.contains(index));

        playedIndices.insert(index);
        currentIndex = index;

    } else {

            playedIndices.clear();

        if (currentIndex + 1 < playlist.size()) {
            currentIndex++;

        } else if (repeatListEnabled && !playlist.isEmpty()) {
            currentIndex = 0;

        } else {
            player->stop();
            emit requestHideEq();

            icon->setIcon(QIcon("C:/Users/tech system/Downloads/Vector.svg"));
            isPlaying = false;
            QMessageBox::information(
                nullptr,
                "پایان لیست",
                "به پایان پلی لیست رسیدید !"
                );
            return;
        }
    }

    qDebug() << "پخش آهنگ:" << currentIndex << playlist[currentIndex];

    icon->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));
    play(playlist[currentIndex]);
    trackList->setCurrentRow(currentIndex);
    isPlaying = true;

    }
}

void MusicPlayer::playNextAuto(QListWidget*& trackList, QPushButton*& icon, bool& isPlaying) {
    const QStringList& playlist = playlists[currentPlaylistName];
    int index;

    if (playlist.isEmpty()) {
        qDebug() << "لیست پخش خالیه!";
        return;
    }

    if (repeatSongEnabled == true){


        icon->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));
        isPlaying = true;
        play(playlist[currentIndex]);
        trackList->setCurrentRow(currentIndex);
        return;

    }
    else if (!playbackQueue.isEmpty()) {

        trackList->clearSelection();

        QString nextSong = playbackQueue.takeFirst();
        isPlaying = true;
        play(nextSong);
        icon->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));

        // ست‌کردن آیتم مرتبط در لیست
        for (int i = 0; i < trackList->count(); ++i) {
            QListWidgetItem *item = trackList->item(i);
            QString itemPath = item->data(Qt::UserRole).toString();
            if (itemPath == nextSong) {
                trackList->setCurrentRow(i);
                break;
            }
        }

        return;
    }
    else{

        if (ShuffleEnabled) {

            if ( currentIndex < playlist.size()) {
                playedIndices.insert(currentIndex);
            }
            if ( ( playedIndices.size() == playlist.size() ) && !repeatListEnabled ) {

                player->stop();
                icon->setIcon(QIcon("C:/Users/tech system/Downloads/Vector.svg"));
                isPlaying = false;
                QMessageBox::information(
                    nullptr,
                    "پایان لیست",
                    "همه آهنگ‌های پلی لیست پخش شده‌اند!\nدکمه تکرار را فعال کنید یا پلی‌لیست جدید انتخاب نمایید."
                    );
                return;
            }
            else if ( ( playedIndices.size() == playlist.size() ) && repeatListEnabled ) {

                playedIndices.clear();

            }

            do {
                index = QRandomGenerator::global()->bounded(playlist.size());
            } while (playedIndices.contains(index));

            playedIndices.insert(index);
            currentIndex = index;

        } else {

            playedIndices.clear();

            if (currentIndex + 1 < playlist.size()) {
                currentIndex++;

            } else if (repeatListEnabled && !playlist.isEmpty()) {
                currentIndex = 0;

            } else {
                player->stop();
                icon->setIcon(QIcon("C:/Users/tech system/Downloads/Vector.svg"));
                isPlaying = false;
                QMessageBox::information(
                    nullptr,
                    "پایان لیست",
                    "به پایان لیست رسیدید !"
                    );
                return;
            }
        }

        qDebug() << "پخش آهنگ:" << currentIndex << playlist[currentIndex];

        icon->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));
        play(playlist[currentIndex]);
        trackList->setCurrentRow(currentIndex);
        isPlaying = true;

    }
}

void MusicPlayer::playBack(QListWidget *& trackList, QPushButton *& icon, bool &isPlaying ) {
    const QStringList &playlist = playlists[currentPlaylistName];
    int index;


    qDebug() << "Current index before increment:" << currentIndex;

    qDebug() << "playlist.size" << playlist.size();

    if (ShuffleEnabled) {

        if ( currentIndex < playlist.size()) {
            playedIndices.insert(currentIndex);
        }
        if ( ( playedIndices.size() == playlist.size() ) && !repeatListEnabled ) {

            player->stop();
            icon->setIcon(QIcon("C:/Users/tech system/Downloads/Vector.svg"));
            isPlaying = false;
            QMessageBox::information(
                nullptr,
                "پایان لیست",
                "همه آهنگ‌های پلی لیست پخش شده‌اند!\nدکمه تکرار را فعال کنید یا پلی‌لیست جدید انتخاب نمایید."
                );
            return;
        }
        else if ( ( playedIndices.size() == playlist.size() ) && repeatListEnabled ) {

            playedIndices.clear();

        }

        do {
            index = QRandomGenerator::global()->bounded(playlist.size());
        } while (playedIndices.contains(index));

        playedIndices.insert(index);
        currentIndex = index;


        icon->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));
        isPlaying = true;
        play(playlist[currentIndex]);
        trackList->setCurrentRow(currentIndex);

    } else {


    if (currentIndex - 1 >= 0) {
        currentIndex--;
        qDebug() << "play" << playlist[currentIndex];

        isPlaying = true;
        play(playlist[currentIndex]);
        icon->setIcon(QIcon("C:/Users/tech system/Downloads/Subtract.svg"));

        trackList->setCurrentRow(currentIndex);

    } else {
        player->stop(); // یا کاری نکنه
        icon->setIcon(QIcon("C:/Users/tech system/Downloads/Vector.svg"));
        isPlaying = false;
    }

    }

}

void MusicPlayer::addPlaylist(const QString &name) {
    if (!playlists.contains(name))
        playlists[name] = QStringList();
}

void MusicPlayer::addToPlaylist(const QString &name, const QString &filePath) {
    playlists[name].append(filePath);
}

void MusicPlayer::setCurrentPlaylist(const QString &name) {
    if (playlists.contains(name)) {
        currentPlaylistName = name;
        currentIndex = 0;
    }
}

QStringList MusicPlayer::loadMusicFromFolder(const QString &folderPath) {
    QDir dir(folderPath);
    QStringList filters;
    filters << "*.mp3" << "*.wav";
    QStringList musicList = dir.entryList(filters, QDir::Files);

    // ایجاد پلی‌لیست پیش‌فرض
    playlists["default"]= QStringList();

    playlists["default"].clear();
    for (const QString &file : musicList) {
        playlists["default"].append(folderPath + "/" + file);
    }

    for (const QString &inc : playlists["default"]) {
        qDebug()<<"kkkkkkkkkkkkkissssssss"<<inc;
    }


    currentPlaylistName = "default";
    currentIndex = 0;

    qDebug() << "Playlist size after loading:" << playlists["default"].size();

    return playlists["default"];
}

void MusicPlayer::setCurrentIndex(QListWidgetItem *item) {
    int n = 0;
    QString targetText = item->text().trimmed();

    for (const QString& mi : playlists[currentPlaylistName]) {

        qDebug() << "ssssssssssssssssssssssssssssa" << mi;

        if (  mi.trimmed() == ("D:/UserTrack/" + targetText) ) {
            qDebug() << " Found match at index:" << n << "text:" << mi;
            currentIndex = n;
            return;
        }
        n++;
    }

    qWarning() << " Item not found in playlist:" << targetText;
}

void MusicPlayer::new_Playlist(QListWidget* listWidget, QLabel *listname) {
    // ساخت دیالوگ اصلی
    QDialog *dialog = new QDialog();  // this اگر MusicPlayer از QWidget ارث می‌بره

    dialog->setStyleSheet("background-color: #d6e4e9;");

    // ساخت دکمه‌ها
    QPushButton *button1 = new QPushButton("Create New Playlist");
    QPushButton *button2 = new QPushButton("Add Song to Playlist");
    QPushButton *button3 = new QPushButton("Select Playlist");

    // ساخت لایوت‌های افقی جداگانه برای هر دکمه
    QHBoxLayout *hLayout1 = new QHBoxLayout();
    QHBoxLayout *hLayout2 = new QHBoxLayout();
    QHBoxLayout *hLayout3 = new QHBoxLayout();

    hLayout1->addWidget(button1);
    hLayout2->addWidget(button2);
    hLayout3->addWidget(button3);

    // ساخت لایوت عمودی برای چیدن لایوت‌های افقی به صورت استک‌شده
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->addLayout(hLayout1);
    mainLayout->addLayout(hLayout2);
    mainLayout->addLayout(hLayout3);

    // دکمه‌ها را فلت کن و استایل بده
    button1->setFlat(true);
    button2->setFlat(true);
    button3->setFlat(true);

    QString btnStyle = "background-color: #039555; color: #ffffff;";
    button1->setStyleSheet(btnStyle);
    button2->setStyleSheet(btnStyle);
    button3->setStyleSheet(btnStyle);



    button1->setCursor(Qt::PointingHandCursor);
    button2->setCursor(Qt::PointingHandCursor);
    button3->setCursor(Qt::PointingHandCursor);

    // تنظیم عنوان و اندازه دیالوگ
    dialog->setWindowTitle("Playlist option");
    dialog->setFixedSize(250, 120);

    connect(button1, &QPushButton::clicked, [=](){

        dialog->hide();
        createNewPlaylist();

    });

    connect(button2, &QPushButton::clicked, [=](){

        dialog->hide();
        addSongToPlaylist();

    });

    connect(button3, &QPushButton::clicked, [=](){

        dialog->hide();
        selectPlaylist(listWidget,listname);

    });

    dialog->exec();
}

void MusicPlayer::createNewPlaylist() {


    QDialog *dialog = new QDialog();

    dialog->setStyleSheet("background-color: #d6e4e9;");

    // ویجت‌ها
    QLineEdit *line1 = new QLineEdit(dialog);

    QPushButton *button1 = new QPushButton("Create Playlist", dialog);
    button1->setFlat(true);
    QString btnStyle = "background-color: #039555; color: #ffffff;";
    button1->setStyleSheet(btnStyle);

    QLabel *label1 = new QLabel(" - این نام وجود دارد", dialog);
    label1->setStyleSheet("color: red; font-weight: bold;");
    label1->hide();

    // لایوت
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(line1);
    layout->addWidget(button1);
    layout->addWidget(label1);
    button1->setCursor(Qt::PointingHandCursor);

    dialog->setLayout(layout);
    dialog->setWindowTitle("New Playlist");
    dialog->setFixedSize(200, 120);


    ////////////////////////////////////////////////////////////////////////
    /// \brief ok
    ///

    connect(button1, &QPushButton::clicked, [=](){

        label1->hide();


        if (!playlists.empty()) {
            auto it = playlists.begin();
            qDebug() << "k1:" << it.key() << ", v1:" << it.value();
        } else {
            qDebug() << "map Empty";
        }

    if (line1->text() == "" || playlists.contains(line1->text())) {

        label1->show();

    }
    else{
    playlists[line1->text()] = QStringList();
        dialog->close();
    }

    });

    dialog->exec();
}

void MusicPlayer::addSongToPlaylist() {
    // بررسی اینکه پلی‌لیستی وجود دارد یا نه
    if (playlists.isEmpty()) {
        QMessageBox::warning(nullptr, "No Playlists", "Please create a playlist first.");
        return;
    }

    // بررسی اینکه آهنگی انتخاب شده یا نه
    if (currentSongaddres == "") {
        QMessageBox::warning(nullptr, "No Song Selected", "Please select a song first.");
        return;
    }

    // نمایش لیست پلی‌لیست‌ها و انتخاب یکی از آن‌ها
    bool ok;
    QString playlistName = QInputDialog::getItem(nullptr,
                                                 "Add Song to Playlist",
                                                 "Select playlist:",
                                                 playlists.keys(),
                                                 0, false, &ok);
    if (!ok || playlistName.isEmpty())
        return;

    // اضافه کردن آهنگ به پلی‌لیست (اگر هنوز توش نباشه)
    if (!playlists[playlistName].contains(currentSongaddres)) {
        playlists[playlistName].append(currentSongaddres);
        QMessageBox::information(nullptr, "Success", "Song added to playlist!");
    } else {
        QMessageBox::information(nullptr, "Already Exists", "This song is already in the playlist.");
    }
}

void MusicPlayer::selectPlaylist(QListWidget* listWidget, QLabel *listname) {


    if (playlists.isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "No playlists available! Create one first.");
        return;
    }

    bool ok;
    QString playlistName = QInputDialog::getItem(nullptr, "Select Playlist", "Select playlist:", playlists.keys(), 0, false, &ok);
    if (!ok || playlistName.isEmpty()) return;
    playedIndices.clear();

    currentPlaylistName = playlistName;
    loadPlaylist(playlistName, listWidget, "D:/UserTrack", listname);

}

void MusicPlayer::loadPlaylist(const QString& playlistName, QListWidget* listWidget, QString basePath, QLabel *listname) {

    basePath += "/";

    if (!playlists.contains(playlistName)) {
        QMessageBox::warning(nullptr, "Warning", "Playlist not found.");
        return;
    }

    listname->setText(" " + playlistName);

    listWidget->clear();  // پاک کردن لیست قبلی

    const QStringList& musicFiles = playlists[playlistName];
    for (const QString& fullPath : musicFiles) {
        QString fileName = fullPath;
        if (fullPath.startsWith(basePath)) {
            fileName = fullPath.mid(basePath.length());
        }

        QListWidgetItem* item = new QListWidgetItem(fileName);
        item->setData(Qt::UserRole, fullPath);  // ذخیره مسیر کامل
        listWidget->addItem(item);
    }

    currentIndex = -1;
}

void MusicPlayer::SetRepeatListEnabled (bool val){ repeatListEnabled = val ;}

void MusicPlayer::SetRepeatSongEnabled (bool val){ repeatSongEnabled = val; }

void MusicPlayer::SetShuffleEnabled (bool val){ ShuffleEnabled = val; }

void MusicPlayer::enqueueSong(const QString &filePath) {
    if (!playbackQueue.contains(filePath)) {
        playbackQueue.append(filePath);
        qDebug() << "Added to queue:" << filePath;
    }
}

void MusicPlayer::saveUserPlaylists(const QString &username) {
    QString fileName = "playlists_" + username + ".json";
    QFile file(QString::fromStdString(fileName.toStdString()));
    if (!file.open(QIODevice::WriteOnly)) return;

    QJsonObject root;
    for (const QString &playlistName : playlists.keys()) {
        QJsonArray songsArray;
        for (const QString &songPath : playlists[playlistName]) {
            songsArray.append(songPath);
        }
        root[playlistName] = songsArray;
    }

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();
}

void MusicPlayer::loadUserPlaylists(const QString &username) {
    QString fileName = "playlists_" + username + ".json";
    QFile file(fileName);
    if (!file.exists()) {
        qDebug() << "No saved playlists found for" << username;
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();

    playlists.clear();

    for (const QString &playlistName : root.keys()) {
        QJsonArray songsArray = root[playlistName].toArray();
        QStringList songPaths;
        for (const QJsonValue &val : songsArray) {
            songPaths.append(val.toString());
        }
        playlists[playlistName] = songPaths;
    }

    qDebug() << "Loaded playlists for" << username;
}



