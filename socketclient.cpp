// socketclient.cpp
#include "socketclient.h"
#include <QDebug>
#include <QJsonDocument>
#include <qjsonobject.h>

SocketClient::SocketClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)), reconnectTimer(new QTimer(this)) {
    connect(socket, &QTcpSocket::readyRead, this, &SocketClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &SocketClient::onDisconnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &SocketClient::onErrorOccurred);

    reconnectTimer->setInterval(500); // Exemple : 5 secondes
    connect(reconnectTimer, &QTimer::timeout, this, &SocketClient::tryReconnect);
}

void SocketClient::connectToServer() {
    socket->connectToHost("localhost", 12345);
}
void SocketClient::onDisconnected() {
    qDebug() << "Déconnexion détectée, tentative de reconnexion...";
    reconnectTimer->start();
}

void SocketClient::onErrorOccurred(QTcpSocket::SocketError socketError) {
    if(socketError == QAbstractSocket::ConnectionRefusedError){
        qDebug() << "Erreur de connexion : " << socketError;
        if (!reconnectTimer->isActive()) {
            reconnectTimer->start();
        }
    }


}

void SocketClient::tryReconnect() {
    if(socket->state() == QTcpSocket::UnconnectedState) {
        qDebug() << "Tentative de reconnexion...";
        connectToServer();
    }
}

void SocketClient::onReadyRead() {
    buffer.append(socket->readAll());

    int endIndex;
    while ((endIndex = buffer.indexOf('\n')) != -1) { // Cherche un retour à la ligne
        QByteArray rawData = buffer.left(endIndex);
        buffer.remove(0, endIndex + 1); // Supprime le JSON traité du buffer

        QJsonDocument doc = QJsonDocument::fromJson(rawData);
        if (doc.isNull()) {
            qDebug() << "Données JSON invalides ou incomplètes";
            continue;
        }

        //qDebug() << "Données reçues:" << rawData;
        // JSON valide trouvé, traiter les données
        QJsonObject obj = doc.object();
        QString vehId = obj["veh_id"].toString();
        QString lon = obj["lon"].toString();
        QString lat = obj["lat"].toString();
        QString red = obj["red"].toString();
        QString green = obj["green"].toString();
        QString blue = obj["blue"].toString();
        QString pt = obj["pt"].toString();

        emit newDataReceived(vehId, lat, lon, red, green, blue, pt);
    }
}
