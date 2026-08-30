#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

#include <QFile>
#include <QTextStream>
#include <QDateTime>

class SimpleSync : public QObject {
    Q_OBJECT
public:
    explicit SimpleSync(QObject *parent = nullptr);

    qint64 positionNow;

    bool startServer(quint16 port);
    void connectToServer(const QString &host, quint16 port);
    void sendPlayCommand(const QString &filePath, qint64 positionInSeconds = 0);
    void sendStopCommand();

    void logToFile(const QString &text) {
        QFile file("debug_log.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << QDateTime::currentDateTime().toString("hh:mm:ss") << ": " << text << "\n";
            file.close();
        }
    }

signals:
    void playRequested(const QString &filePath);
    void stopRequested();

private slots:
    void newConnection();
    void readData();
    void clientDisconnected();
    void onConnected();
    void onError(QAbstractSocket::SocketError error);

private:
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;
    bool isServer = false;
};
