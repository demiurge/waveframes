import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

import "./Widgets" as Widgets
import "./Views" as Views

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    /*
        Funkcja wykorzystuje model barw HSL w celu przejścia koloru w odcienie szarości.
        Wykres nieaktywny otrzymuje ten kolor.
    */
    function desaturate(colorString)
    {
        var c = Qt.darker(colorString, 0.6);
        return Qt.hsla(c.hslHue, 0.0, c.hslLightness, c.a);
    }

    ColumnLayout {
        anchors.fill: parent
        /*
            Utworzenie widzetu odpowiadającego za utworzenie legendy do wykresów.
            Dodatkowy suwak pozwala określić zakres rysowanych wykresów (-100 : 100).
        */
        Widgets.Legend {
            id: legend
            Layout.preferredHeight: 40
            Layout.fillWidth: true

            /*
                Obsługa poruszanego suwaka, który zmienia zakres na osi X.
                Connections łączy źródło sygnału (legend) z funkcją obsługującą przypisanie wartości do dwóch propercji
                min i max osi X.
            */
            Connections {
                target: legend
                function onMoved(min, max) {
                    waveforms.AxisX.min = min;
                    waveforms.AxisX.max = max;
                }
            }
        }

        Views.ChartView {
            id: chartView
            /*
                Obszar widoku wykresu wypełnia prawie całą przestrzeń aplikacji. Legenda zabiera od góry 40 pikseli.
            */
            Layout.fillWidth: true
            Layout.fillHeight: true

            /*
                Gdy widok zostanie utworzony, zostaną naniesione utworzone wykresy liniowe dla funkcji sin(x) i cos(x).
                Model przechowywany w obiekcie waveforms zawiera ogólne dane na temat wykresu:
                - nazwe
                - kolor
                - czy jest aktywny
                Widok pozwala na tworzenie różnych typów wykresów, dokumentacja
                https://doc.qt.io/qt-5/qml-qtcharts-chartview.html#createSeries-method
                Metoda waveforms.setSeries pozwala na wypełnienie wykresów danymi.
                Kolor wykresu ustalany jest w zależności czy dany wykres jest aktywny.
                Qt.UserRole - odpowiada za nazwę ( ma domyślnie wartość 0x100 )
                Qt.UserRole + 1 - odpowiada za kolor
                Qt.UserRole + 2  - odpowiada za aktywowanie wykresu
            */
            Component.onCompleted : {
                for (var i=0; i < waveforms.rowCount(); i++) {
                    var index = waveforms.index(i, 0);
                    var name = waveforms.data(index, Qt.UserRole);
                    var color = waveforms.data(index, Qt.UserRole + 1);
                    var active = waveforms.data(index, Qt.UserRole + 2);

                    var series = chartView.createSeries(ChartView.SeriesTypeLine, name, waveforms.AxisX, waveforms.AxisY);
                    waveforms.setSeries(series);
                    series.color = active ? color : desaturate(color);
                }
            }


            Connections {
                target: waveforms
                /*
                    Zmiana kolorów wykresów w zależności czy dany wykres był aktywny.
                    Sygnałem tutaj jest zmiana danych w waveforms.
                */
                function onDataChanged() {
                    for (var i=0; i < waveforms.rowCount(); i++) {
                        var index = waveforms.index(i, 0);
                        var color = waveforms.data(index, Qt.UserRole + 1);
                        var active = waveforms.data(index, Qt.UserRole + 2);

                        var series = chartView.series(i);
                        if (series) {
                            series.color = active ? color : desaturate(color);
                        }
                    }
                }
                /*
                    Usunięcie wykresu z widoku. Sygnałem jest usunięcie jednego elementu z modelu waveforms.
                */
                function onRemoveSeries(row) {
                    var series = chartView.series(row);
                    if (series) {
                        chartView.removeSeries(series);
                    }
                }
                /*
                    Przesuwanie suwakiem wymusza odświeżenie w nowym zakresie wykresów.
                */
                function onAxisXChanged() {
                    for (var i=0; i < waveforms.rowCount(); i++) {
                        var index = waveforms.index(i, 0);
                        var color = waveforms.data(index, Qt.UserRole + 1);
                        var active = waveforms.data(index, Qt.UserRole + 2);

                        var series = chartView.series(i)
                        waveforms.setSeries(series);
                        series.color = active ? color : desaturate(color);
                    }
                }
            }
        }
    }
}
