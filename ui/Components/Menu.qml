import QtQuick.Controls 2.15
import QtQuick 2.15
/*
    Menu podręczne z jednym wyborem - usunięcia funkcji o danej nazwie. Wybór powoduje wysłanie sygnału usunięcia danego wykresu.
    Dodatkowo menu podręczne jest animowane - rozszerza się w krótkim czasie.
    Model zawiera informacje o wybranym elemencie, np. jego nazwę, czy jego położenie w modelu(index).
*/
Menu {

    signal removeSeries(int index)

    enter: Transition {
        ParallelAnimation {
            NumberAnimation { property: "width"; from: 0.0; to: contextMenu.implicitWidth; duration: 150; easing.type: Easing.InCubic  }
            NumberAnimation { property: "height"; from: 0.0; to: contextMenu.implicitHeight; duration: 150;easing.type: Easing.InCubic  }
        }
    }
    MenuItem {
        text: "Usuń funkcje <b>" + model.name + "</b>"
        onTriggered: {
            removeSeries(index);
        }
    }
}