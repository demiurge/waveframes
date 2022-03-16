#include "Generator.h"
#include <utility>
/*
 * Konstruktor z odpowiednią funkcją
 */
Generator::Generator(std::function<double(double, double)> fp) : _fp(std::move(fp)){

}

/*
 * Metoda zwraca wartości z krokiem step. Wywołuje fukcje _fp z wartości x oraz amplitudą.
 */
QVector<QPointF> Generator::create(double beg, double end, double amplitude, double step) {
    QVector<QPointF> points;
    for(auto x = beg; x<= end; x = x + step) {
        points.push_back(QPointF(x, _fp(x, amplitude)));
    }
    return points;
}

