#include "DataObject.h"
#include "Generator.h"
#include "Utils.h"

DataObject::DataObject(QString name, QString color, bool active)
        : _name(std::move(name)),
          _color(std::move(color)),
          _isActive(active) {

}

DataObject::DataObject(const DataObject &other) : _name(other._name), _color(other._color), _isActive(other._isActive) {
}

void DataObject::setName(const QString &name) {
    if (this->_name != name) {
        this->_name = name;
        emit nameChanged();
    }
}

void DataObject::setColor(const QString &color) {
    if (this->_color != color) {
        this->_color = color;
        emit colorChanged();
    }
}

void DataObject::setActive(bool active) {
    if (this->_isActive != active) {
        this->_isActive = active;
        emit isActiveChanged();
    }
}


QVector<QPointF> DataObject::generate(QtCharts::QValueAxis *axisX, QtCharts::QValueAxis *axisY) {
    QVector<QPointF> result;
    if (_name == "sin") {
        result = Generator(sinus<double>).create(axisX->min(), axisX->max(), axisY->max(), 0.01);
    } else if (_name == "cos") {
        result = Generator(cosinus<double>).create(axisX->min(), axisX->max(), axisY->max(), 0.01);
    }
    return result;
}

