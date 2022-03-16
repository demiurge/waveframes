#pragma once
#include <QObject>
#include <QAbstractSeries>
#include <QXYSeries>
#include <QValueAxis>

/*
 * Metadane dla wykresu:
 * - name
 * - color
 * - isActive
 * Powiązana funkcja generate generująca odpowiednie dane z wykorzystaniem nazwy funkcji.
 * Możliwa refaktoryzacja w celu przeniesienia tej funkcji poza ta klasę z metadanymi.
 */

class DataObject : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool isActive READ isActive WRITE setActive NOTIFY isActiveChanged)
public:
    explicit DataObject(QString  name, QString  color, bool active);
    DataObject() = default;
    DataObject(const DataObject& other);
    ~DataObject() override = default;;

    QString name() const { return _name; }
    QString color() const { return _color; }
    bool isActive() const { return _isActive; }
    void setName(const QString& name);
    void setColor(const QString& color);
    void setActive(bool active);

    QVector<QPointF> generate(QtCharts::QValueAxis* axisX, QtCharts::QValueAxis* axisY);

private:
    QString _name;
    QString _color;
    bool _isActive;


signals:
    void nameChanged();
    void colorChanged();
    void isActiveChanged();
};


