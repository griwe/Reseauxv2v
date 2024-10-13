#ifndef VEHICULE_H
#define VEHICULE_H

#include <QObject>

class Vehicule : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(double latitude READ lat WRITE setLat NOTIFY latChanged)
    Q_PROPERTY(double longitude READ lon WRITE setLon NOTIFY lonChanged)

public:
    explicit Vehicule(QObject *parent = nullptr);
    explicit Vehicule(const QString &id, QObject *parent = nullptr);
    explicit Vehicule(const QString &id,double latitude, double longitude, QObject *parent = nullptr);

    Q_INVOKABLE QString id() const;
    void setId(const QString &id);

    Q_INVOKABLE double lat() const;
    Q_INVOKABLE void setLat(float latitude);

    Q_INVOKABLE double lon() const;
    Q_INVOKABLE void setLon(float longitude);

    QString toString() const;

signals:
    void idChanged();
    void latChanged();
    void lonChanged();

private:
    QString m_id;
    float m_latitude;
    float m_longitude;
};

#endif // VEHICULE_H
