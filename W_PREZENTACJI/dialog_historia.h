#ifndef DIALOG_HISTORIA_H
#define DIALOG_HISTORIA_H
#include <QDialog>
#include "W_USLUG/program.h"

namespace Ui {
class dialog_historia;
}

class dialog_historia : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_historia(std::vector<Egzamin> tab_egz, QWidget *parent = nullptr);
    ~dialog_historia();
    void wczytaj_pytanie();

private slots:
    void on_pushButton_nastepny_clicked();

    void on_pushButton_poprzedni_clicked();

private:
    Ui::dialog_historia *ui;
    std::vector<Egzamin> tab_egzaminow;
    std::vector<Egzamin>::iterator iterator;
    std::array<QVector<Pytanie>, 2> pytania_ppk;
    std::array<QVector<Pytanie>, 2> pytania_pk;
};

#endif // DIALOG_HISTORIA_H
