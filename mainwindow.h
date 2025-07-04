#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QCloseEvent>
#include "user.h"
#include "equalizerwidget.h"
#include <QListWidgetItem>
#include <QObject>
#include <QMediaPlayer>
#include "musicplayer.h"
#include "MusicPlayer.h"
#include <QMenu>
#include <QSlider>          // برای QSlider
#include <QLabel>           // برای QLabel
#include <QString>          // برای QString
#include <QObject>          // برای QObject و سیگنال‌ها/اسلات‌ها
#include <QMouseEvent>
#include "ClickableSlider.h"
#include "simple_sync.h"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString formatTime(qint64 timeMilliSeconds);
    void updateTimeLabel(qint64 position, qint64 duration);
    void setPosition(int position);
    void showContextMenu(const QPoint &pos);
    SimpleSync *sync = new SimpleSync(this);

    bool isSender = false;
    EqualizerWidget *eq = nullptr;


    void logToFile(const QString &text) {
        QFile file("debug_log.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << QDateTime::currentDateTime().toString("hh:mm:ss") << ": " << text << "\n";
            file.close();
        }
    }

private slots:
    void onSongFinished();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_btnPlayPause_clicked();
    void handleClickRepeat();
    void handleClickShuffle();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    QString currentUsername;


private:
    Ui::MainWindow *ui;
    bool isPlaying = false;
    int stateRepeat = 0;
    int stateShuffle = 0;
    MusicPlayer *musicPlayer;

};
#endif // MAINWINDOW_H
