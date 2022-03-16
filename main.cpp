#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "src/Waveforms.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /*
     * Silnik generujący interfejs użytkownika z plików QML.
     */
    QQmlApplicationEngine engine;

    /*
     *  Główny obiekt aplikacji waveforms zawierający na wstępie dane ogólne dotyczące dwóch wykresów:
     *  nazwa wykresu, kolor zdefiniowany za pomocą nazwanego koloru css, oraz czy na wstępie jest aktywny.
    */
    Waveforms waveforms;
    waveforms.addFunction("sin", "darkturquoise", true);
    waveforms.addFunction("cos", "deeppink", false);

    /*
     *  Obiekt waveforms będzie widoczny w plikach QML pod tą samą nazwą
     */
    engine.rootContext()->setContextProperty("waveforms", &waveforms);

    /*
     * Załadowanie pliku main.qml jako startowego.
     */
    const QUrl url("qrc:/ui/main.qml");
    engine.load(url);

    /*
     * Program wykonuje się w pętli zdarzeń - do zamknięcia go.
     */
    return QApplication::exec();
}
