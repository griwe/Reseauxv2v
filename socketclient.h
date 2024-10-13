// socketclient.h
#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>


class SocketClient : public QObject {
    Q_OBJECT

public:
    explicit SocketClient(QObject *parent = nullptr);
    void connectToServer();
    void tryReconnect();


signals:
    void newDataReceived(const QString &vehId, const QString &lon, const QString &lat, const QString &red, const QString &green, const QString &blue, const QString &pt);

private slots:
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred(QTcpSocket::SocketError socketError);



private:
    QTcpSocket *socket;
    QByteArray buffer;
    QTimer *reconnectTimer; // Timer pour gérer les reconnexions

};

#endif // SOCKETCLIENT_H
