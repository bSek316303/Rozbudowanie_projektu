#include "W_PREZENTACJI/dialog_historia.h"
#include "W_DANYCH/importerBlokowy.h"
#include "ui_dialog_historia.h"

dialog_historia::dialog_historia(const std::vector<Egzamin> tab_egz, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dialog_historia)
{
    ui->setupUi(this);
    qDebug() << "13333";
    tab_egzaminow = tab_egz;
    iterator = tab_egzaminow.begin();
    ImporterBlokowy importer(this, "PytaniaNEW");
    importer.setPrzedmiot("PPK");
    importer.wczytajDane();
    pytania_ppk[0] = importer.getBlok(0);
     qDebug() << "pytania ppk";
    for(auto x : pytania_ppk[0]){
        qDebug() << x.getNumer();
    }
    //pytania_ppk[1] = importer.getBlok(1);
    importer.setPrzedmiot("PK");
    importer.wczytajDane();
    pytania_pk[0] = importer.getBlok(0);
     qDebug() << "pytania pk 0";
    for(auto x : pytania_pk[0]){
        qDebug() << x.getNumer();
    }
    pytania_pk[1] = importer.getBlok(1);
    qDebug() << "pytania pk 1";
    for(auto x : pytania_pk[1]){
        qDebug() << x.getNumer();
    }
    qDebug() << "1";
    wczytaj_pytanie();
}

dialog_historia::~dialog_historia()
{
    delete ui;
}

void dialog_historia::on_pushButton_nastepny_clicked()
{
    if (iterator + 1 != tab_egzaminow.end()) {
        iterator += 1;
    }
    wczytaj_pytanie();
}

void dialog_historia::on_pushButton_poprzedni_clicked()
{
    if (iterator != tab_egzaminow.begin()) {
        iterator -= 1;
    }
    wczytaj_pytanie();
}
void dialog_historia::wczytaj_pytanie()
{
    QString przedmiot = (iterator->przedmiot == 0) ? "PPK" : "PK";
    QVector<Pytanie> tab_pytan;
    QVector<Pytanie> tab_pytan_odrzuconych;
    ui->label_blok->setText("BLOK: " + QString::number(iterator->blok));
    if (przedmiot == "PPK") {
        tab_pytan.push_back(pytania_ppk[iterator->blok][iterator->pytanie_1]);
        tab_pytan.push_back(pytania_ppk[iterator->blok][iterator->pytanie_2]);
        tab_pytan.push_back(pytania_ppk[iterator->blok][iterator->pytanie_3]);
        if(iterator->odrzucone_pytanie_1 == -1){
            ui->textBrowser_usuniete->setText("Brak dodatkowych wpisów!");
        } else if (iterator->odrzucone_pytanie_2 == -1){
            tab_pytan_odrzuconych.push_back(pytania_ppk[iterator->blok][iterator->odrzucone_pytanie_1]);
        } else {
            tab_pytan_odrzuconych.push_back(pytania_ppk[iterator->blok][iterator->odrzucone_pytanie_1]);
            tab_pytan_odrzuconych.push_back(pytania_ppk[iterator->blok][iterator->odrzucone_pytanie_2]);
        }
        ui->label_przedmiot->setText(przedmiot);
    } else if (przedmiot == "PK") {
        tab_pytan.push_back(pytania_pk[iterator->blok][iterator->pytanie_1]);
        tab_pytan.push_back(pytania_pk[iterator->blok][iterator->pytanie_2]);
        tab_pytan.push_back(pytania_pk[iterator->blok][iterator->pytanie_3]);
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
        return;
    QString tresc = "";
    for (int pyt = 0; pyt < tab_pytan.size(); pyt++)
        tresc += tab_pytan[pyt].getTresc();
    ui->textBrowser_aktualne->setText(tresc);
    QString tresc_usunietych = "";
    for (int pyt = 0; pyt < tab_pytan_odrzuconych.size(); pyt++)
        tresc_usunietych += tab_pytan_odrzuconych[pyt].getTresc();
    ui->textBrowser_usuniete->setText(tresc_usunietych);
}


