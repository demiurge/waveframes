#pragma once

#include "DataInterface.h"

/*
 * Generator wie z jaką funkcją ma doczynienia. Jest to klasa fabryczna z metodą create.
 * Metoda ta tworzy wektor z punktami (x,y) obliczonymi w zakresie beg - end i amplitudą oraz krokiem obliczeń.
 */
class Generator : public DataInterface
{
public:
    explicit Generator(std::function<double(double, double)>  fp);
    QVector<QPointF> create(double beg, double end, double amplitude, double step = 0.01) override;
private:
    std::function<double(double,double)> _fp;
};




