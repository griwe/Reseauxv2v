#include "Cellule.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "socketclient.h"
#include "Vehicule.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SocketClient client;
    client.connectToServer();

    // Découpage de la carte
    // Point de départ Lat Lng --> Point en haut à gauche --> 47.7623, 7.2978
    // Largeur --> 7.30662 - 7.29807 = 0,00855 = 200 pixels --> 75 pixels = 0.00320625
    // Hauteur --> 47.76219 - 47.75646 = 0,00573 = 200 pixels --> 75 pixels = 0.00214875
    // Création de la liste de Cellules pour le maillage
    QList<QObject*> cellules;
    // Ligne impairs
    for (int k = 0; k < 10; ++k) {
        for (int i = 0; i < 26; ++i) {
            Cellule *cellule = new Cellule();
            cellule->setX(0 + i * 75);
            cellule->setY(0 - 75/2 + k * 75 * 2 * 0.75);
            cellule->setCoordinate(QGeoCoordinate(47.7623 + 0.00214875/2  - (k * 0.00320625), 7.2978 + i * 0.00320625));
            cellules.append(cellule);
        }
    }

    // Ligne pairs
    for (int k = 0; k < 9; ++k) {
        for (int i = 0; i < 27; ++i) {
            Cellule *cellule = new Cellule();
            cellule->setX((0 - 75/2) + i * 75);
            cellule->setY(0 + (75 * 0.25) + k * 75 * 2 * 0.75);
            cellule->setCoordinate(QGeoCoordinate(47.7623 - 0.00214875 * 0.25 - (k * 0.00320625), 7.2978 - 0.00320625/2 + i * 0.00320625));
            cellules.append(cellule);
        }
    }


    QQmlApplicationEngine engine;

    qmlRegisterType<Vehicule>("com.monapplication", 1, 0, "Vehicule");
    // declaration du client au qml
    engine.rootContext()->setContextProperty("socketClient", &client);

    // On passe la liste des cellules au QML
    engine.rootContext()->setContextProperty("cellulesModel", QVariant::fromValue(cellules));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
