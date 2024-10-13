#include "Cellule.h"

const double Cellule::HAUTEUR_CELLULE = 0.00214875;
const double Cellule::LARGEUR_CELLULE = 0.00320625;
const double Cellule::HAUTEUR_CARTESIENNE_CELLULE = 75;
const double Cellule::LARGEUR_CARTESIENNE_CELLULE = 75;

Cellule::Cellule(QObject *parent)
    : QObject(parent)
{
    m_strokeColor = "black";
    m_fillColor = "transparent";
}

QGeoCoordinate Cellule::coordinate() const
{
    return m_coordinate;
}



double Cellule::x() const
{
    return m_x;
}

double Cellule::y() const
{
    return m_y;
}

QGeoCoordinate Cellule::center() const
{
    return m_center;
}

double Cellule::centerX() const
{
    return m_centerx;
}

double Cellule::centerY() const
{
    return m_centery;
}

QColor Cellule::fillColor() const
{
    return m_fillColor;
}

QColor Cellule::strokeColor() const
{
    return m_strokeColor;
}

QList<QObject*> Cellule::vehicules() const
{
    return m_vehicules;
}

QGeoCoordinate Cellule::getCoordinate() const
{
    return coordinate();
}

double Cellule::getX() const
{
    return x();
}

double Cellule::getY() const
{
    return y();
}

QGeoCoordinate Cellule::getCenter() const
{
    return center();
}

void Cellule::clearVehicules(){
    m_vehicules.clear();

}

 int Cellule::getNombreDeVehicules() const {
    return m_vehicules.size();
}

double Cellule::getCartesianCenterX() const
{
    return centerX();
}

double Cellule::getCartesianCenterY() const
{
    return centerY();
}

QColor Cellule::getFillColor() const
{
    return fillColor();
}

QColor Cellule::getStrokeColor() const
{
    return strokeColor();
}

void Cellule::setCoordinate(const QGeoCoordinate &coordinate)
{
    if (m_coordinate != coordinate) {
        m_coordinate = coordinate;
        emit coordinateChanged();
    }
    this->m_center = calculateCenter();
}

void Cellule::setX(const double &coordinate)
{
    if (m_x != coordinate) {
        m_x = coordinate;
        emit cartesianCoordinateChangedX();
    }
    this->m_centerx = calculateCartesianCenterX();
}

void Cellule::setY(const double &coordinate)
{
    if (m_y != coordinate) {
        m_y = coordinate;
        emit cartesianCoordinateChangedY();
    }
    this->m_centery = calculateCartesianCenterY();
}

void Cellule::setCenter(const QGeoCoordinate &coordinate)
{
    if (m_center != coordinate) {
        m_center = coordinate;
        emit centerChanged();
    }
}

void Cellule::setCenterX(const double &coordinate)
{
    if (m_centerx != coordinate) {
        m_centerx = coordinate;
        emit cartesianCenterCoordinateChangedX();
    }
}

void Cellule::setCenterY(const double &coordinate)
{
    if (m_centery != coordinate) {
        m_centery = coordinate;
        emit cartesianCenterCoordinateChangedY();
    }
}

void Cellule::setFillColor(const QColor &color)
{
    if (m_fillColor != color) {
        m_fillColor = color;
        emit fillColorChanged();
    }
}

void Cellule::setStrokeColor(const QColor &color)
{
    if (m_strokeColor != color) {
        m_strokeColor = color;
        emit strokeColorChanged();
    }
}

void Cellule::setVehicules(const QList<QObject*> &vehicules)
{
    if (m_vehicules != vehicules) {
        m_vehicules = vehicules;
        emit vehiculesChanged();
    }
}

void Cellule::addVehicule(Vehicule *vehicule)
{
    if (vehicule) {
        if (!m_vehicules.contains(vehicule)) {
            m_vehicules.append(vehicule);
            emit vehiculesChanged();
        }
    }
}

void Cellule::removeVehiculeById(const QString &vehiculeId)
{
    for (auto it = m_vehicules.begin(); it != m_vehicules.end(); ++it) {
        Vehicule *vehicule = qobject_cast<Vehicule*>(*it);
        if (vehicule && vehicule->id() == vehiculeId) {
            it = m_vehicules.erase(it);
            delete vehicule;
            emit vehiculesChanged();
            return;
        }
    }
}


QGeoCoordinate Cellule::calculateCenter() const
{
    double centerLatitude = m_coordinate.latitude() - HAUTEUR_CELLULE / 2;
    double centerLongitude = m_coordinate.longitude() + LARGEUR_CELLULE / 2;

    return QGeoCoordinate(centerLatitude, centerLongitude);
}

double Cellule::calculateCartesianCenterX() const
{
    double centerX = m_x + LARGEUR_CARTESIENNE_CELLULE / 2;

    return centerX;
}

double Cellule::calculateCartesianCenterY() const
{
    double centerY = m_y + HAUTEUR_CARTESIENNE_CELLULE / 2;

    return centerY;
}

QString Cellule::toString() const
{
    QString vehiclesString;
    for (QObject *object : m_vehicules) {
        Vehicule *vehicle = qobject_cast<Vehicule*>(object);
        if (vehicle) {
            vehiclesString += vehicle->toString() + ", ";
        }
    }


    return QStringLiteral("Cellule at coordinate (%1, %2) (%7, %8) and center (%3, %4) (%9, %10) with colors %5 & %6. Vehicles: [%11].")
        .arg(m_coordinate.latitude())
        .arg(m_coordinate.longitude())
        .arg(m_center.latitude())
        .arg(m_center.longitude())
        .arg(colorToString(m_strokeColor))
        .arg(colorToString(m_fillColor))
        .arg(m_x)
        .arg(m_y)
        .arg(m_centerx)
        .arg(m_centery)
        .arg(vehiclesString);
}

QString Cellule::colorToString(const QColor &color) const
{
    return QStringLiteral("(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
}
