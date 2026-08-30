#include "simple_sync.h"

SimpleSync::SimpleSync(QObject *parent) : QObject(parent) {}

bool SimpleSync::startServer(quint16 port) {
    if (server) return false;

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &SimpleSync::newConnection);

    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Failed to start server:" << server->errorString();
        logToFile("Failed to start server:");

        delete server;
        server = nullptr;
        return false;
    }
    isServer = true;
    qDebug() << "Server started on port" << port;
    logToFile("Server started on port" + port);

    return true;
}

void SimpleSync::connectToServer(const QString &host, quint16 port) {
    if (socket) return;

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &SimpleSync::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &SimpleSync::readData);
    connect(socket, &QTcpSocket::disconnected, this, &SimpleSync::clientDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &SimpleSync::onError);

    socket->connectToHost(host, port);
    isServer = false;
    qDebug() << "Connecting to" << host << port;
    logToFile("Connecting to" + host + port);

}

void SimpleSync::newConnection() {
    if (socket) {
        // فقط یک کلاینت قبول می‌کنیم
        QTcpSocket *newSock = server->nextPendingConnection();
        newSock->disconnectFromHost();
        newSock->deleteLater();
        return;
    }

    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &SimpleSync::readData);
    connect(socket, &QTcpSocket::disconnected, this, &SimpleSync::clientDisconnected);
    qDebug() << "Client connected from" << socket->peerAddress().toString();
}

void SimpleSync::readData() {
    if (!socket) return;

    QByteArray data = socket->readAll();
    QString msg = QString::fromUtf8(data).trimmed();
    qDebug() << "Received message:" << msg;

    if (msg.startsWith("PLAY::")) {
        QString path = msg.mid(6);
        emit playRequested(path);
    } else if (msg == "STOP::") {
        emit stopRequested();
    } else {
        qDebug() << "Unknown message";
    }
}

void SimpleSync::sendPlayCommand(const QString &filePath, qint64 positionInSeconds) {
    if (!isServer || !socket) return;

    QString modifiedPath = filePath;
/*
    // اگر با C شروع می‌شه تبدیلش کن به D یا برعکس
    if (filePath.startsWith("C:", Qt::CaseInsensitive)) {
        modifiedPath.replace(0, 1, "D");
    } else if (filePath.startsWith("D:", Qt::CaseInsensitive)) {
        modifiedPath.replace(0, 1, "C");
    }
*/
    QString msg;
    if (positionInSeconds >= 0) {
        msg = QString("PLAY::%1::%2").arg(modifiedPath).arg(positionInSeconds);
    } else {
        msg = "PLAY::" + modifiedPath;
    }

    socket->write(msg.toUtf8());
    socket->flush();
    qDebug() << "Sent PLAY command with path:" << modifiedPath << (positionInSeconds > 0 ? (" at position: " + QString::number(positionInSeconds)) : "");
}



void SimpleSync::sendStopCommand() {
    if (!isServer || !socket) return;

    QString msg = "STOP::";
    socket->write(msg.toUtf8());
    socket->flush();
    qDebug() << "Sent STOP command";
}

void SimpleSync::clientDisconnected() {
    qDebug() << "Client disconnected";
    socket->deleteLater();
    socket = nullptr;
}

void SimpleSync::onConnected() {
    qDebug() << "Connected to server";
    logToFile("Connected to server");

}

void SimpleSync::onError(QAbstractSocket::SocketError error) {
    Q_UNUSED(error);
    qDebug() << "Socket error:" << (socket ? socket->errorString() : "No socket");
}
