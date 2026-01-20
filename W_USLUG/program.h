#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include "W_DANYCH\urn.h"
#include "W_DANYCH\importer.h"
#include "W_DANYCH\historiaegzaminow.h"


class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(QObject *parent = nullptr);
    void setPrzedmiot(QString nazwa);
    void wczytajPytania();
    int getLiczbaBlokow();
    void losujPytania(int blok, int num);
    QString getAktPrzedmiot();
    QStringList getListaPrzedmiotow();
    void odznaczPytania(QVector<int>,int);
    void ponownieWypiszWylosowane(int blok);
    void zapiszEgzamin(int blok, int odrzucone_1, int odrzucone_2);
    void serializuj(const QString& sciezka = "");
    std::vector<Egzamin> deserializuj(const QString& sciezka = "");

signals:
    void wypisz(QVector<Pytanie>&,int);

private:
    static const int MAX_LICZ_BLOKOW = 2;
    static const int MIN_LICZ_PYT = 3;
    Importer * m_BazaPytan;
    std::array<Urn,MAX_LICZ_BLOKOW> m_urnyDoLosowania;
    std::array<QVector<Pytanie>,MAX_LICZ_BLOKOW> m_WylosPyt;
    historiaEgzaminow historia;

};

#endif // PROGRAM_H
