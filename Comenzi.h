#ifndef COMENZI_H
#define COMENZI_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>  // for fixed and setprecision
#include "Produse.h"

using namespace std;

template <typename Container = unordered_map<string, int>>//TEMPLATES!!!!!!!!!!!!!
class Comanda {
private:
    //ENCAPSULATION:
    //atribute private cu metode publice
    string nume_client;
    string locatie;
    int total;
    bool fidelitate;
    Container produse_cantitate;  // Template container for product quantities

public:
    // constructor
    Comanda(string nume_client, string locatie);

    void adaugaProdus(const string& produs, int cantitate, vector<Produs*>& produse, const string& locatie);

    void calculeazaTotal();

    void verificaFidelitate(const string& file);

    void display() const;

    // convertire csv
    string toCSV() const;

    // Static method to read orders from CSV
    static Comanda<Container>* fromCSV(const string& line);
};

// Function to manage orders
void gestionareComenzi(int k);

#endif // COMENZI_H
