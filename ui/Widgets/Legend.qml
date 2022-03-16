import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../Components" as Components

/*
    Dwa suwaki do określenia zakresu pomiędzy wartościami (-100 a 100). Przy usuwaniu obiektu waveforms,
    propercje (min i max) przestają być widoczne.
    Przesuwanie suwakiem wywołuje sygnał moved z zakresem zmian.
    Legenda tworzona jest odpowiedniej ilości elementów w modelu waveforms. Za utworzenie odpowiada delegat,
    który da sygnał elementu aktywnego i przekaze do modelu odpowiednią informacje.
*/

Item {
    signal moved(int min, int max)
    RowLayout {
        anchors.fill: parent
        RangeSlider {
            Layout.fillHeight: true
            Layout.fillWidth: true
            from: -100
            to: 100

            first.value: waveforms == null ? 0 : waveforms.AxisX.min
            second.value: waveforms == null ? 0 : waveforms.AxisX.max

            first.onMoved: {
                moved(first.value, second.value);
            }
            second.onMoved: {
                moved(first.value, second.value);
            }

        }
        Rectangle{
            color: "lightgray"
            Layout.fillHeight: true
            Layout.preferredWidth: 300

            Row {
                id: legendRow

                anchors.centerIn: parent
                Text {
                    text: "Legenda:"
                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
                    font.pointSize: 15
                }
                spacing: 5
                Repeater {
                    id: legendRepeater
                    model: waveforms
                    delegate: Components.LegendDelegate {
                        id: legendDelegate
                        Connections {
                            target: legendDelegate
                            function onActiveSeries() {
                                waveforms.setActive(index);
                            }
                        }
                    }
                }
            }
        }
    }
}
