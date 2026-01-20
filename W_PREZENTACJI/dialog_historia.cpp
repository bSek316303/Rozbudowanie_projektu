#include "W_PREZENTACJI/dialog_historia.h"
#include "W_DANYCH/importerBlokowy.h"
#include "ui_dialog_historia.h"

// Konstruktor klasy dialog_historia
// tab_egz – wektor egzaminów, które będą wyświetlane
// parent – rodzic QWidget (opcjonalny)
dialog_historia::dialog_historia(const std::vector<Egzamin> tab_egz, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dialog_historia)
{
    // Inicjalizacja GUI z pliku .ui
    ui->setupUi(this);

    qDebug() << "13333"; // Debug – sygnał, że konstruktor się uruchomił

    // Kopiowanie egzaminów do lokalnego wektora
    tab_egzaminow = tab_egz;

    // Ustawienie iteratora na początek historii egzaminów
    iterator = tab_egzaminow.begin();

    // Tworzymy obiekt ImporterBlokowy – będzie wczytywał pytania z plików
    ImporterBlokowy importer(this, "PytaniaNEW");

    // Wczytanie pytań dla przedmiotu PPK
    importer.setPrzedmiot("PPK");
    importer.wczytajDane();

    // Przypisanie pierwszego bloku pytań PPK do tablicy
    pytania_ppk[0] = importer.getBlok(0);

    qDebug() << "pytania ppk";
    for(auto x : pytania_ppk[0]){
        qDebug() << x.getNumer(); // Wyświetlenie numerów pytań w bloku
    }

    // Wczytanie pytań dla przedmiotu PK
    importer.setPrzedmiot("PK");
    importer.wczytajDane();

    // Pierwszy blok PK
    pytania_pk[0] = importer.getBlok(0);
    qDebug() << "pytania pk 0";
    for(auto x : pytania_pk[0]){
        qDebug() << x.getNumer();
    }

    // Drugi blok PK
    pytania_pk[1] = importer.getBlok(1);
    qDebug() << "pytania pk 1";
    for(auto x : pytania_pk[1]){
        qDebug() << x.getNumer();
    }

    qDebug() << "1";

    // Wczytanie pierwszego pytania egzaminacyjnego do GUI
    wczytaj_pytanie();
}

// Destruktor klasy dialog_historia
// Zwalnia pamięć zaalokowaną dla ui
dialog_historia::~dialog_historia()
{
    delete ui;
}

// Slot do obsługi kliknięcia przycisku "Następny egzamin"
void dialog_historia::on_pushButton_nastepny_clicked()
{
    // Sprawdzenie, czy nie jesteśmy na końcu wektora
    if (iterator + 1 != tab_egzaminow.end()) {
        iterator += 1; // Przesunięcie iteratora do przodu
    }
    wczytaj_pytanie(); // Wyświetlenie aktualnego egzaminu
}

// Slot do obsługi kliknięcia przycisku "Poprzedni egzamin"
void dialog_historia::on_pushButton_poprzedni_clicked()
{
    // Sprawdzenie, czy nie jesteśmy na początku wektora
    if (iterator != tab_egzaminow.begin()) {
        iterator -= 1; // Cofnięcie iteratora
    }
    wczytaj_pytanie(); // Wyświetlenie aktualnego egzaminu
}

// Funkcja wczytująca pytania bieżącego egzaminu do GUI
void dialog_historia::wczytaj_pytanie()
{
    // Ustalenie przedmiotu (0 -> PPK, 1 -> PK)
    QString przedmiot = (iterator->przedmiot == 0) ? "PPK" : "PK";

    QVector<Pytanie> tab_pytan;           // Pytania aktywne
    QVector<Pytanie> tab_pytan_odrzuconych; // Pytania odrzucone

    // Wyświetlenie numeru bloku w GUI
    ui->label_blok->setText("BLOK: " + QString::number(iterator->blok));

    if (przedmiot == "PPK") {
        // Wczytanie pytań egzaminacyjnych (3 główne pytania)
        tab_pytan.push_back(pytania_ppk[iterator->blok][iterator->pytanie_1]);
        tab_pytan.push_back(pytania_ppk[iterator->blok][iterator->pytanie_2]);
        tab_pytan.push_back(pytania_ppk[iterator->blok][iterator->pytanie_3]);

        // Obsługa pytań odrzuconych
        if(iterator->odrzucone_pytanie_1 == -1){
            ui->textBrowser_usuniete->setText("Brak dodatkowych wpisów!");
        } else if (iterator->odrzucone_pytanie_2 == -1){
            tab_pytan_odrzuconych.push_back(pytania_ppk[iterator->blok][iterator->odrzucone_pytanie_1]);
        } else {
            tab_pytan_odrzuconych.push_back(pytania_ppk[iterator->blok][iterator->odrzucone_pytanie_1]);
            tab_pytan_odrzuconych.push_back(pytania_ppk[iterator->blok][iterator->odrzucone_pytanie_2]);
        }

        // Ustawienie etykiety przedmiotu w GUI
        ui->label_przedmiot->setText(przedmiot);

    } else if (przedmiot == "PK") {
        // Wczytanie pytań egzaminacyjnych PK
        tab_pytan.push_back(pytania_pk[iterator->blok][iterator->pytanie_1]);
        tab_pytan.push_back(pytania_pk[iterator->blok][iterator->pytanie_2]);
        tab_pytan.push_back(pytania_pk[iterator->blok][iterator->pytanie_3]);

        // Obsługa pytań odrzuconych
        if(iterator->odrzucone_pytanie_1 == -1){
            ui->textBrowser_usuniete->setText("Brak dodatkowych wpisów!");
        } else if (iterator->odrzucone_pytanie_2 == -1){
            tab_pytan_odrzuconych.push_back(pytania_pk[iterator->blok][iterator->odrzucone_pytanie_1]);
        } else {
            tab_pytan_odrzuconych.push_back(pytania_pk[iterator->blok][iterator->odrzucone_pytanie_1]);
            tab_pytan_odrzuconych.push_back(pytania_pk[iterator->blok][iterator->odrzucone_pytanie_2]);
        }

        ui->label_przedmiot->setText(przedmiot);
    } else
        return; // Jeżeli przedmiot nie jest znany, kończymy funkcję

    // Połączenie treści pytań w jeden string do wyświetlenia
    QString tresc = "";
    for (int pyt = 0; pyt < tab_pytan.size(); pyt++)
        tresc += tab_pytan[pyt].getTresc();
    ui->textBrowser_aktualne->setText(tresc);

    // Połączenie treści pytań odrzuconych w jeden string
    QString tresc_usunietych = "";
    for (int pyt = 0; pyt < tab_pytan_odrzuconych.size(); pyt++)
        tresc_usunietych += tab_pytan_odrzuconych[pyt].getTresc();
    ui->textBrowser_usuniete->setText(tresc_usunietych);
}

