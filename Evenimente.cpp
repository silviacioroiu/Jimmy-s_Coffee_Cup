#include "Evenimente.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

// initialize the static observers vector
vector<shared_ptr<EventObserver>> Evenimente::observers;

// constructor
Evenimente::Evenimente(string denumire, string oras, int cost, bool activ)
        : denumire(denumire), oras(oras), cost_de_organizare(cost), activ(activ) {}

void Evenimente::salvareEveniment() {
    ofstream file("C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\evenimente.csv", ios::app);
    if (!file.is_open()) {
        throw runtime_error("Failed to open evenimente.csv");
    }
    file << denumire << "," << oras << "," << cost_de_organizare << "," << activ << "\n";
    file.close();
    notifyObservers(denumire, activ);
}


void Evenimente::afisareEvenimente() {
    ifstream file("C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\evenimente.csv");
    if (!file.is_open()) {
        throw runtime_error("Failed to open evenimente.csv");
    }

    string line;
    cout << "\nLista evenimente:\n";
    cout << "Denumire | Oras | Cost | Status\n";
    cout << "-----------------------------\n";

    while (getline(file, line)) {
        stringstream ss(line);
        string denumire, oras, cost, activ;
        getline(ss, denumire, ',');
        getline(ss, oras, ',');
        getline(ss, cost, ',');
        getline(ss, activ, ',');
        cout << denumire << " | " << oras << " | " << cost << " | "
             << (activ == "1" ? "Activ" : "Inactiv") << endl;
    }

    file.close();
}

// Adauga
void Evenimente::adaugaEveniment() {
    string denumire, oras;
    int cost;

    cout << "Denumire eveniment: ";
    cin.ignore();
    getline(cin, denumire);

    cout << "Oras: ";
    getline(cin, oras);

    cout << "Cost de organizare: ";
    cin >> cost;

    Evenimente event(denumire, oras, cost);
    event.salvareEveniment();
}

// Activate/ deactivate  event
void Evenimente::activeazaEveniment(string denumire) {
    ifstream inFile("C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\evenimente.csv");
    if (!inFile.is_open()) {
        throw runtime_error("Failed to open evenimente.csv");
    }

    ofstream outFile("C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\temp.csv");
    if (!outFile.is_open()) {
        throw runtime_error("Failed to create temp.csv");
    }

    string line;
    bool found = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string curr_denumire, oras, cost, activ;
        getline(ss, curr_denumire, ',');
        getline(ss, oras, ',');
        getline(ss, cost, ',');
        getline(ss, activ, ',');

        if (curr_denumire == denumire) {
            // Toggle the status
            string newStatus = (activ == "1") ? "0" : "1";
            outFile << curr_denumire << "," << oras << "," << cost << "," << newStatus << "\n";
            found = true;
            notifyObservers(denumire, newStatus == "1");
        } else {
            outFile << line << "\n";
        }
    }

    inFile.close();
    outFile.close();
    //EXceptie
    if (!found) {
        throw invalid_argument("Evenimentul nu exista!");
    }

    remove("C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\evenimente.csv");
    rename("C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\temp.csv", "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\evenimente.csv");
}


// Delete event
void Evenimente::stergeEveniment(string denumire) {
    ifstream inFile("C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\evenimente.csv");
    if (!inFile.is_open()) {
        throw runtime_error("Failed to open evenimente.csv");
    }

    ofstream outFile("C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\temp.csv");
    if (!outFile.is_open()) {
        throw runtime_error("Failed to create temp.csv");
    }

    string line;
    bool found = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string curr_denumire;
        getline(ss, curr_denumire, ',');

        if (curr_denumire != denumire) {
            outFile << line << "\n";
        } else {
            found = true;
            notifyObservers(denumire, false);
        }
    }

    inFile.close();
    outFile.close();

    if (!found) {
        throw invalid_argument("Evenimentul nu exista!");
    }

    remove("evenimente.csv");
    rename("temp.csv", "evenimente.csv");
}

// functia care e apelate din main si are ca parametru decizia din main pt gestionarea eventului
void gestionareEvenimente(int optiune) {
    //EXCEPTIE
    try {
        switch (optiune) {
            case 1:
                Evenimente::afisareEvenimente();
                break;
            case 2:
                Evenimente::adaugaEveniment();
                break;
            case 3: {
                string denumire;
                cout << "Denumire eveniment de activat: ";
                cin.ignore();
                getline(cin, denumire);
                Evenimente::activeazaEveniment(denumire);
                break;
            }
            case 4: {
                string denumire;
                cout << "Denumire eveniment de sters: ";
                cin.ignore();
                getline(cin, denumire);
                Evenimente::stergeEveniment(denumire);
                break;
            }
            default:
                throw invalid_argument("Optiune invalida!");
        }
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}
