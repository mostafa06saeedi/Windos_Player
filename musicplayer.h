// MusicPlayer.h

#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QDir>
#include <QListWidgetItem>
#include <QSet>
#include <QRandomGenerator>
#include <QMediaMetaData>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QStringList>
#include "ui_mainwindow.h"
#include "equalizerwidget.h"

class MusicPlayer : public QObject
{
    Q_OBJECT

public:
    explicit MusicPlayer(QObject *parent = nullptr);

    QStringList loadMusicFromFolder(const QString &folderPath);
    QMediaPlayer *player;

    void play(const QString &filePath, qint64 positionInSeconds = 0);
    void play();
    void pause();
    void setVolume(int volumePercent);
    void selectPlaylist(QListWidget* listWidget,QLabel *listname);
    void playAtIndex(int index);
    void playNext(QListWidget *& trackList, QPushButton *& icon,bool &isPlaying );
    void playNextAuto(QListWidget *& trackList, QPushButton *& icon,bool &isPlaying );
    void playBack(QListWidget *& trackList, QPushButton *& icon,bool &isPlaying );
    void playPrevious();
    void addPlaylist(const QString &name);
    void addToPlaylist(const QString &name, const QString &filePath);
    void setCurrentPlaylist(const QString &name);
    void setCurrentIndex(QListWidgetItem *item);
    void new_Playlist(QListWidget* listWidget, QLabel *listname);
    void createNewPlaylist();
    void addSongToPlaylist();
    void loadPlaylist(const QString& playlistName, QListWidget* listWidget,  QString basePath, QLabel *listname );
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void SetRepeatListEnabled (bool val);
    void SetRepeatSongEnabled (bool val);
    void SetShuffleEnabled (bool val);
    void setUI(Ui::MainWindow *uiPtr);
    void enqueueSong(const QString &filePath);
    void saveUserPlaylists(const QString &username);
    void loadUserPlaylists(const QString &username);
    QStringList playbackQueue;

    int currentIndex = 0;
    QString currentSongaddres = "";

    void logToFile(const QString &text) {
        QFile file("debug_log.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << QDateTime::currentDateTime().toString("hh:mm:ss") << ": " << text << "\n";
            file.close();
        }
    }

signals:
    void songFinished();
    void requestHideEq();
    void requestShowEq();


private:

    Ui::MainWindow *ui = nullptr;
    QStringList musicList;
    QAudioOutput *audioOutput;

    //////////////////////////////

    QMap<QString, QStringList> playlists;    // نگهدارنده پلی‌لیست‌ها

    QString currentPlaylistName = "default";
    QSet<int> playedIndices;

    bool repeatListEnabled = false;
    bool repeatSongEnabled = false;
    bool ShuffleEnabled = false;

};

#endif // MUSICPLAYER_H
