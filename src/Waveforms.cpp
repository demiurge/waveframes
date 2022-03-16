#include "Waveforms.h"
#include <QtCharts/QAreaSeries>
#include <QtCharts/QXYSeries>
#include <QtConcurrent>
/*
 * Konstruktor z początkowymi zakresami w osiach X i Y.
 */
Waveforms::Waveforms(QObject *parent) : QAbstractListModel(parent) {
    _axisX = std::make_unique<QtCharts::QValueAxis>();
    _axisY = std::make_unique<QtCharts::QValueAxis>();
    _axisX->setRange(-4.0, 4.0);
    _axisY->setRange(-1.0, 1.0);

    connect(_axisX.get(), &QtCharts::QValueAxis::rangeChanged, this, &Waveforms::rangeXChanged);
}

/*
 * Usunięcie obserwatorów asynchronicznych
 */
Waveforms::~Waveforms() {
    for(auto i = 0; i < watchers.count(); i++) {
        watchers.at(i)->deleteLater();
    }
    watchers.clear();
}
/*
 * Zmieniono zakres w osi X
 */
void Waveforms::rangeXChanged(qreal min, qreal max) {
    emit axisXChanged();
}
/*
 * Ile wykresów ma model
 */
int Waveforms::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return _metadata.count();
}
/*
 * Wykresy i ich metadane
 */
QVariant Waveforms::data(const QModelIndex &index, int role) const {
    if ( !index.isValid() ) return QVariant();
    const DataObject & data = _metadata.at(index.row());
    switch (static_cast<Role>(role)) {
        case Role::NameRole:
            return data.name();
        case Role::ColorRole:
            return data.color();
        case Role::ActiveRole:
            return data.isActive();
        default:
            return {};
    }
}
/*
 * Nazwa rolew ułatwiają dostanie się do danego pola w modelu
 */
QHash<int, QByteArray> Waveforms::roleNames() const {
    static QHash<int, QByteArray> mapping {
            {static_cast<int>(Role::NameRole), "name"},
            {static_cast<int>(Role::ColorRole), "color"},
            {static_cast<int>(Role::ActiveRole), "active"}
    };
    return mapping;
}
/*
 * Usuwanie wykresu, poprzez usunięcie metadanych oraz powiązanego obserwatora zmian danych wykresu
 */
void Waveforms::remove(int row) {
    if (row < 0 || row >= _metadata.count()) return;
    beginRemoveRows(QModelIndex(), row, row);
    _metadata.removeAt(row);
    watchers.at(row)->deleteLater();
    watchers.removeAt(row);
    endRemoveRows();
    emit removeSeries(row);
}

/*
 * Dodajemy nowy wykres do modelu oraz obserwatora asynchronicznych zmian danych wykresu
 */
void Waveforms::addFunction(const QString &name, const QString &color, bool active) {
    beginInsertRows(QModelIndex(), _metadata.count(), _metadata.count());
    _metadata.push_back(DataObject{name, color, active});
    watchers.push_back(new QFutureWatcher<void>());
    endInsertRows();
}

/*
 * Aktywny może być tylko jeden wykres wskazany przez wartość row.
 * Widok poinformowany o zmianach przez sygnał dataChanged.
 */
void Waveforms::setActive(int row) {
    if (row<0 || row >= _metadata.count()) return;
    for(int i =0 ; i < _metadata.count(); i++) {
        _metadata[i].setActive(i == row);
    }
    emit dataChanged(index(0), index(_metadata.count() - 1));
}

/*
 * Dla danego wykresu będą generowane dane.
 */
void Waveforms::setSeries(QtCharts::QAbstractSeries *series) {
    if (series) {
        generateData(dynamic_cast<QtCharts::QXYSeries *>(series));
    }
}

/*
 * Generowanie danych asynchronicznie - nie chcemy obciązać UI obliczeniami.
 * Przy przesuwaniu suwakiem w UI sprawdzamy, czy nadal obliczają się dane do wykresu,
 * jeśli tak to przerywamy obliczenia. Uruchamiamy aynchronicznie metodę generate i przechodzimy dalej.
 */
void Waveforms::generateData(QtCharts::QXYSeries *series) {
    for (auto i = 0; i < _metadata.count(); i++) {
        if (_metadata.at(i).name() == series->name()) {
            if (watchers.at(i)->isRunning()) {
                watchers.at(i)->cancel();
                watchers.at(i)->waitForFinished();
            }
            QFuture<void> future = QtConcurrent::run(this, &Waveforms::generate, i, _axisX.get(), _axisY.get() , series);
            watchers.at(i)->setFuture(future);
            break;
        }
    }
}

/*
 * Jest to moment, w którym zostaną wygenrowane dane dla danego wykresu i ostatecznie podmienione.
 * Wykres wsparty jest OpenGL-em do generowania go w widoku.
 */
void Waveforms::generate(int row, QtCharts::QValueAxis* axisX, QtCharts::QValueAxis* axisY, QtCharts::QXYSeries *series) {
    series->setUseOpenGL(true);
    series->replace(_metadata[row].generate(axisX, axisY));
}


