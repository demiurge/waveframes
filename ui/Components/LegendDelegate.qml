import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./" as Components

/*
    Delegat legendy składa się z:
        - niewielkiego zaokrąglonego prostokąta z kolorem danego wykresu
        - nazwy funkcji
    Obszar obsługi myszki pokrywa w całości delegata i działa na lewy bądź prawy przycisk myszy.
    Lewy przycisk myszy wysyła sygnał - aktywuj dany wykres.
    Prawy przycisk myszy otwiera menu podręczne z możliwością wysłania sygnału usunięcia danego wykresu.
*/
Rectangle{
    width: 80
    height: 25
    radius: 5

    signal activeSeries(int index)

    Row {
        id: row
        anchors.centerIn: parent
        spacing: 5
        Rectangle {
            id: marker
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0.3
            radius: 4
            width: 15
            height: 15
            color: model.color
        }
        Text {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -1
            font.bold: model.active
            text: model.name
        }
    }
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if(mouse.button & Qt.RightButton) {
                contextMenu.popup();
            } else if (mouse.button & Qt.LeftButton){
                activeSeries(index);
            }
        }
        Components.Menu {
            id: contextMenu

            Connections {
                target: contextMenu
                function onRemoveSeries() {
                    waveforms.remove(index);
                }
            }
        }
    }
}
