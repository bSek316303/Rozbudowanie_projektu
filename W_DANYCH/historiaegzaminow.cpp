#include "historiaegzaminow.h"
#include "W_PREZENTACJI/mainwindow.h"
#include "qdatetime.h"
#include <QVector>

historiaEgzaminow::historiaEgzaminow() {}
void historiaEgzaminow::dodajEgzamin(QVector<Pytanie> &pytania, int blok, int odrzucone_1, int odrzucone_2, int przedmiot){
    Egzamin temp;
    temp.blok = blok;
    temp.przedmiot = przedmiot;

    QVector<int> numery;
    for (const auto& p : pytania)
        numery.push_back(p.getNumer());

    if (odrzucone_1 != -1)
        numery.erase(std::remove(numery.begin(), numery.end(), odrzucone_1), numery.end());

    if (odrzucone_2 != -1)
        numery.erase(std::remove(numery.begin(), numery.end(), odrzucone_2), numery.end());

    if (numery.size() != 3)
        return;

    temp.pytanie_1 = numery[0];
    temp.pytanie_2 = numery[1];
    temp.pytanie_3 = numery[2];

    temp.odrzucone_pytanie_1 = odrzucone_1;
    temp.odrzucone_pytanie_2 = odrzucone_2;

    tab_egzaminow.push_back(temp);
    //wypiszHistorie();
}

void historiaEgzaminow::serializuj(const QString& path) {
    QDateTime teraz = QDateTime::currentDateTime();
    QString nazwaPliku = "/egzamin_" + teraz.toString("yyyyMMdd_HHmmss") + ".bin";
    QFile file(path + nazwaPliku);
    if (!file.open(QIODevice::WriteOnly)) return;

    // Obliczamy rozmiar bufora (tylko egzaminów)
    size_t rozmiarBufora = tab_egzaminow.size() * sizeof(Egzamin);
    std::vector<std::byte> bufSer(rozmiarBufora);

    std::byte* buf = bufSer.data();

    // Kopiujemy egzamin po egzaminie do bufora
    for (const auto& egz : tab_egzaminow) {
        std::memcpy(buf, &egz, sizeof(Egzamin));
        buf += sizeof(Egzamin);
    }

    // Zapis do pliku
    file.write(reinterpret_cast<const char*>(bufSer.data()), bufSer.size());
    file.close();
}

std::vector<Egzamin> historiaEgzaminow::deserializuj(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return {};

    // Odczyt całego pliku do bufora
    QByteArray dane = file.readAll();
    file.close();

    // Obliczamy liczbę egzaminów w pliku
    size_t liczbaEgzaminow = dane.size() / sizeof(Egzamin);

    // Czyścimy aktualną historię
    tab_egzaminow.clear();
    tab_egzaminow.reserve(liczbaEgzaminow);

    // Deserializacja egzaminów
    const std::byte* buf = reinterpret_cast<const std::byte*>(dane.constData());
    for (size_t i = 0; i < liczbaEgzaminow; ++i) {
        Egzamin egz;
        std::memcpy(&egz, buf, sizeof(Egzamin));
        tab_egzaminow.push_back(egz);
        buf += sizeof(Egzamin);
    }
    return tab_egzaminow;
}


