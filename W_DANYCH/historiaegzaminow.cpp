#include "historiaegzaminow.h"
#include "W_PREZENTACJI/mainwindow.h"
#include "qdatetime.h"
#include <QVector>

historiaEgzaminow::historiaEgzaminow() {}

// Funkcja dodająca nowy egzamin do historii
// pytania      – wektor wylosowanych pytań (3–5)
// blok         – numer bloku egzaminacyjnego
// odrzucone_1  – numer pierwszego odrzuconego pytania lub -1
// odrzucone_2  – numer drugiego odrzuconego pytania lub -1
// przedmiot    – identyfikator przedmiotu (np. 0 – PPK, 1 – PK)
void historiaEgzaminow::dodajEgzamin(QVector<Pytanie> &pytania, int blok, int odrzucone_1, int odrzucone_2, int przedmiot){
    // Tymczasowa struktura przechowująca dane jednego egzaminu
    Egzamin temp;
    temp.blok = blok;
    temp.przedmiot = przedmiot;


    // Pobranie numerów wszystkich wylosowanych pytań
    QVector<int> numery;
    for (const auto& p : pytania)
        numery.push_back(p.getNumer());


    // Usunięcie pierwszego odrzuconego pytania (jeśli istnieje)
    if (odrzucone_1 != -1)
        numery.erase(std::remove(numery.begin(), numery.end(), odrzucone_1), numery.end());

    if (odrzucone_2 != -1)
        numery.erase(std::remove(numery.begin(), numery.end(), odrzucone_2), numery.end());


    // Po odrzuceniu pytań muszą pozostać dokładnie 3 pytania
    // W przeciwnym wypadku egzamin nie jest zapisywany
    if (numery.size() != 3)
        return;



    // Po odrzuceniu pytań muszą pozostać dokładnie 3 pytania
    // W przeciwnym wypadku egzamin nie jest zapisywany
    temp.pytanie_1 = numery[0];
    temp.pytanie_2 = numery[1];
    temp.pytanie_3 = numery[2];


    // Zapis numerów odrzuconych pytań (-1 jeśli nie było odrzuceń)
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


