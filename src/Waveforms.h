#pragma once

#include <QAbstractListModel>
#include <QValueAxis>
#include <QFutureWatcher>
#include "DataObject.h"

/*
 * Klasa przechowuje potrzebne dane do tworzenia wykresów, t.j.:
 * - zakres osi X i Y
 * - metadane wykresu (nazwa, kolor, aktywność)
 * Dodatkowa odpowiedzialność klasy służąca za aktywowanie / usunięcie / dodanie wykresu funkcji.
 * Generuje również dane do wykresów.
 * (Prawdopodobnie przy refaktoryzacji klasy, zostanie rozbita na trzy osobne klasy)
 *
 * Osie X i Y przechowują zakres minimalnej i maksymalnej wartości w danej osi.
 *
 *
 *
 */

class Waveforms : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(QtCharts::QValueAxis* AxisX READ axisX NOTIFY axisXChanged)
    Q_PROPERTY(QtCharts::QValueAxis* AxisY READ axisY NOTIFY axisYChanged)
public:
    enum class Role {
        NameRole = Qt::UserRole,
        ColorRole,
        ActiveRole
    };
    explicit Waveforms(QObject *parent = nullptr);
    ~Waveforms() override;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addFunction(const QString &name, const QString &color, bool active);

    Q_INVOKABLE void setActive(int row);
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE void setSeries(QtCharts::QAbstractSeries *series);

    auto axisX() const { return _axisX.get();}
    auto axisY() const { return _axisY.get();}


private:
    QVector<DataObject> _metadata;
    void generateData(QtCharts::QXYSeries *series);
    void generate(int row, QtCharts::QValueAxis* axisX, QtCharts::QValueAxis* axisY, QtCharts::QXYSeries *series);

    std::unique_ptr<QtCharts::QValueAxis> _axisX;
    std::unique_ptr<QtCharts::QValueAxis> _axisY;
    // wektor z obserwatorami asynchronicznych operacji uzupełniania danych
    QVector<QFutureWatcher<void> *> watchers;
private slots:
    void rangeXChanged(qreal min, qreal max);
signals:
    void removeSeries(int row);
    void axisXChanged();
    void axisYChanged();
};


