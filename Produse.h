#ifndef PRODUSE_H
#define PRODUSE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class Produs {
private:
    string nume_produs;
    int cod;
    int cost_de_fabricare;
    int cost_vanzare;
    int cantitate;
    string locatie;

public:
    Produs(string nume_produs, int cod, int cost_fabricare, int cost_vanzare, int cantitate, string locatie);//constructor
    //ENCAPSULATION
    // getters
    string getNumeProdus() const;
    int getCod() const;
    int getCostFabricare() const;
    int getCostVanzare() const;
    int getCantitate() const;
    string getLocatie() const;

    // setters
    void setCantitate(int cantitate);

    void display() const;

    //ABSTRACTIZARE- ascunde implementarea
    string toCSV() const;
    // Metoda toCSV transforma obiectul curent de tip Produs intr-un sir de caractere
    // formatat in stil CSV (valori separate prin virgula).
    // Acest sir este utilizat pentru a salva produsul intr-un fisier CSV.

    static Produs* fromCSV(const string& line);
    // Metoda fromCSV primeste o linie dintr-un fisier CSV, o desparte in componente
    // folosind separatorul virgula si creeaza un obiect Produs corespunzator.
    // Returneaza un pointer la obiectul Produs creat.

    static bool isValidLocation(const string& loc);

    ~Produs() = default;//destructor
};

void gestionareStocuri(int k);

#endif
