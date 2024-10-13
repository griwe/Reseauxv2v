#include "Vehicule.h"

Vehicule::Vehicule(QObject *parent)
    : QObject(parent), m_id(""), m_latitude(0.0), m_longitude(0.0) {}

Vehicule::Vehicule(const QString &id, QObject *parent)
    : QObject(parent), m_id(id), m_latitude(0.0), m_longitude(0.0) {}

Vehicule::Vehicule(const QString &id, double latitude, double longitude, QObject *parent)
    : QObject(parent), m_id(id), m_latitude(latitude), m_longitude(longitude) {}

QString Vehicule::id() const {
    return m_id;
}

QString Vehicule::toString() const {
    return QStringLiteral("Vehicule(id: %1)").arg(m_id);
}

void Vehicule::setId(const QString &id) {
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}
double Vehicule::lat() const {
    return m_latitude;
}

void Vehicule::setLat(float latitude) {
    if (m_latitude != latitude) {
        m_latitude = latitude;
        emit latChanged();
    }
}

double Vehicule::lon() const {
    return m_longitude;
}

void Vehicule::setLon(float longitude) {
    if (m_longitude != longitude) {
        m_longitude = longitude;
        emit lonChanged();
    }
}
