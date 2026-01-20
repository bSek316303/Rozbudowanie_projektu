#ifndef HISTORIAEGZAMINOW_H
#define HISTORIAEGZAMINOW_H
#include <vector>
#include "pytanie.h"
#include "qdebug.h"

struct Egzamin {
    int przedmiot; // 0 ppk 1 pk
    int blok;
    int pytanie_1;
    int pytanie_2;
    int pytanie_3;
    int odrzucone_pytanie_1;
    int odrzucone_pytanie_2;
};

class historiaEgzaminow
{
private:
    std::vector<Egzamin> tab_egzaminow;
    void wypiszHistorie(){
        for(auto egz: tab_egzaminow){
            qDebug() << egz.przedmiot;
            qDebug() << egz.blok;
            qDebug() << egz.pytanie_1;
            qDebug() << egz.pytanie_2;
            qDebug() << egz.pytanie_3;
            qDebug() << egz.odrzucone_pytanie_1;
            qDebug() << egz.odrzucone_pytanie_2;
            qDebug() << "PRZERWA!\n";
        }
    }
public:
    historiaEgzaminow();
    void dodajEgzamin(QVector<Pytanie>& pytania, int blok, int odrzucone_1, int odrzucone_2, int przedmiot);
    void serializuj(const QString& path);
    std::vector<Egzamin> deserializuj(const QString& path);
};

#endif // HISTORIAEGZAMINOW_H
