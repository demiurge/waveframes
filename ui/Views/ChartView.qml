import QtCharts 2.15

    /*
        Legenda domyślna została wyłączona - ze względu na wymagania projektowe, które miały umożliwić
        pogrubienie nazwy i usunięcie z menu kontekstowego danego wykresu
        Animacja wyłączona ze względu na render w OpenGL-u. Przyśpiesza rysowanie.
    */
ChartView {
    title: "Przebiegi"
    theme: ChartView.ChartThemeLight


    legend.visible: false
    antialiasing: true
    //animationOptions: ChartView.SeriesAnimations
}


