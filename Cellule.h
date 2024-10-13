#ifndef CELLULE_H
#define CELLULE_H

#include "Vehicule.h"
#include <QObject>
#include <QtPositioning/QGeoCoordinate>
#include <QColor>

class Cellule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate coordinate READ coordinate WRITE setCoordinate NOTIFY coordinateChanged)
    Q_PROPERTY(double x READ x WRITE setX NOTIFY cartesianCoordinateChangedX)
    Q_PROPERTY(double y READ y WRITE setY NOTIFY cartesianCoordinateChangedY)
    Q_PROPERTY(QGeoCoordinate center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(double centerX READ centerX WRITE setCenterX NOTIFY cartesianCenterCoordinateChangedX)
    Q_PROPERTY(double centerY READ centerY WRITE setCenterY NOTIFY cartesianCenterCoordinateChangedY)
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(QColor strokeColor READ strokeColor WRITE setStrokeColor NOTIFY strokeColorChanged)
    Q_PROPERTY(QList<QObject*> vehicules READ vehicules NOTIFY vehiculesChanged)

public:
    static const double HAUTEUR_CELLULE;
    static const double LARGEUR_CELLULE;
    static const double HAUTEUR_CARTESIENNE_CELLULE;
    static const double LARGEUR_CARTESIENNE_CELLULE;

    explicit Cellule(QObject *parent = nullptr);
    QGeoCoordinate coordinate() const;
    double x() const;
    double y() const;
    QGeoCoordinate center() const;
    double centerX() const;
    double centerY() const;
    QColor fillColor() const;
    QColor strokeColor() const;
    QList<QObject*> vehicules() const;

    Q_INVOKABLE void clearVehicules();
    Q_INVOKABLE int getNombreDeVehicules() const ;

    Q_INVOKABLE QGeoCoordinate getCoordinate() const;
    Q_INVOKABLE double getX() const;
    Q_INVOKABLE double getY() const;

    Q_INVOKABLE QGeoCoordinate getCenter() const;
    Q_INVOKABLE double getCartesianCenterX() const;
    Q_INVOKABLE double getCartesianCenterY() const;

    QColor getFillColor() const;
    QColor getStrokeColor() const;

    QGeoCoordinate calculateCenter() const;
    double calculateCartesianCenterX() const;
    double calculateCartesianCenterY() const;

    Q_INVOKABLE QString toString() const;
    Q_INVOKABLE QString colorToString(const QColor &color) const;

public slots:
    void setCoordinate(const QGeoCoordinate &coordinate);
    void setX(const double &coordinate);
    void setY(const double &coordinate);
    void setCenter(const QGeoCoordinate &coordinate);
    void setCenterX(const double &coordinate);
    void setCenterY(const double &coordinate);
    void setFillColor(const QColor &color);
    void setStrokeColor(const QColor &color);
    void setVehicules(const QList<QObject*> &vehicules);
    void addVehicule(Vehicule *vehicule);
    void removeVehiculeById(const QString &vehiculeId);

signals:
    void coordinateChanged();
    void cartesianCoordinateChangedX();
    void cartesianCoordinateChangedY();
    void centerChanged();
    void cartesianCenterCoordinateChangedX();
    void cartesianCenterCoordinateChangedY();
    void fillColorChanged();
    void strokeColorChanged();
    void vehiculesChanged();

private:
    QGeoCoordinate m_coordinate;
    double m_x;
    double m_y;
    QGeoCoordinate m_center;
    double m_centerx;
    double m_centery;
    QColor m_fillColor;
    QColor m_strokeColor;
    QList<QObject*> m_vehicules;
};

#endif // CELLULE_H
