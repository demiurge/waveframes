#pragma once
#include <vector>
#include <QVector>
#include <QPointF>

/*
 * Interfejs dla metody create
 */
class DataInterface {
public:
    virtual QVector<QPointF> create(double beg, double end, double amplitude, double step = 0.01) = 0;
};

