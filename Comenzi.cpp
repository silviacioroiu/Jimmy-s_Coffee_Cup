#include "Comenzi.h"
#include "Produse.h"

// constructor
template <typename Container>
Comanda<Container>::Comanda(string nume_client, string locatie)
        : nume_client(nume_client), locatie(locatie), total(0), fidelitate(false) {}

// MI A FOST FOARTEEE GREU SA MI DAU SEAMA CUM SA FAC AICI
template <typename Container>
void Comanda<Container>::adaugaProdus(const string& produs, int cantitate, vector<Produs*>& produse, const string& locatie) {
    auto it = find_if(produse.begin(), produse.end(),
                      [&produs, &locatie](Produs* p) {
                          return p->getNumeProdus() == produs && p->getLocatie() == locatie;
                      });

    if (it != produse.end()) {
        if ((*it)->getCantitate() < cantitate) {
            cout << "Insufficient stock for the product '" << produs << "' in location '" << locatie << "'!\n";
            return;
        }

        int cost = (*it)->getCostVanzare();  // getter pt costu devanzare
        produse_cantitate[produs] += cantitate;  // add quantity to the hash table
        total += cantitate * cost;  // update tot

        // dupa ce se plaseaza comanda se scade din cantitate respectiva
        (*it)->setCantitate((*it)->getCantitate() - cantitate);

        cout << "Product added: " << produs << ", Quantity: " << cantitate
             << ", Unit price: " << cost << " RON\n";
    } else {
        cout << "Product '" << produs << "' does not exist in the stock of the location '" << locatie << "'!\n";
    }
}

// calculat total daca fidel
template <typename Container>
void Comanda<Container>::calculeazaTotal() {
    if (fidelitate) {
        int discount = total * 0.1;  // 10% discount
        total -= discount;
        cout << "Discount applied: -" << discount << " RON\n";
    }
}

// verificare fidelitate
template <typename Container>
void Comanda<Container>::verificaFidelitate(const string& file) {
    ifstream inFile(file);
    if (!inFile.is_open()) {
        cout << "Error opening the file: " << file << endl;
        return;
    }

    string line;
    int count = 0;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string client;
        getline(ss, client, ',');

        if (client == nume_client) {
            count++;
        }
    }

    fidelitate = (count >= 2);  // AICI!! un client devinde fidel daca viziteaza cafeneaua de mai mult de 2 ori!!!!
    inFile.close();
}

// display
template <typename Container>
void Comanda<Container>::display() const {
    cout << "Client name:: " << nume_client << "\n";
    cout << "Location: " << locatie << "\n";
    cout << "Products ordered:\n";
    for (const auto& p : produse_cantitate) {
        cout << "  " << p.first << " x" << p.second << "\n";
    }
    cout << "Total: " << total << " RON\n";
    if (fidelitate) {
        cout << "Loyalty: Yes (Discount applied - 10% off)\n";
    } else {
        cout << "Loyalty: No\n";
    }
}

// convert to CSV format
template <typename Container>
string Comanda<Container>::toCSV() const {
    stringstream ss;
    ss << nume_client << "," << locatie << "," << total << "," << fidelitate << ",";
    for (const auto& p : produse_cantitate) {
        ss << p.first << ":" << p.second << ";";
    }
    return ss.str();
}

// read order from CSV
template <typename Container>
Comanda<Container>* Comanda<Container>::fromCSV(const string& line) {
    stringstream ss(line);
    string client, locatie, total_str, fidelitate_str, produse_str;

    getline(ss, client, ',');
    getline(ss, locatie, ',');
    getline(ss, total_str, ',');
    getline(ss, fidelitate_str, ',');

    Comanda<Container>* comanda = new Comanda<Container>(client, locatie);
    comanda->total = stoi(total_str);
    comanda->fidelitate = (fidelitate_str == "1");

    getline(ss, produse_str, ',');
    stringstream produse_stream(produse_str);
    string produs_cant;

    while (getline(produse_stream, produs_cant, ';')) {
        string produs;
        int cantitate;
        size_t pos = produs_cant.find(':');
        if (pos != string::npos) {
            produs = produs_cant.substr(0, pos);
            cantitate = stoi(produs_cant.substr(pos + 1));
            comanda->produse_cantitate[produs] = cantitate;
        }
    }

    return comanda;
}

// functia importanta care este apelata din main si se foloseste de toate din file ul asta
void gestionareComenzi(int k) {
    const string COMENZI_FILE = "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\comenzi.csv";
    const string PRODUSE_FILE = "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\produse.csv";

    // using template with unordered_map
    vector<Comanda<unordered_map<string, int>>*> comenzi;
    vector<Produs*> produse;

    // Read products from produse.csv
    ifstream produseFile(PRODUSE_FILE);
    if (produseFile.is_open()) {
        string line;
        while (getline(produseFile, line)) {
            Produs* p = Produs::fromCSV(line);
            if (p) {
                produse.push_back(p);
            }
        }
        produseFile.close();
    }

    ifstream inFile(COMENZI_FILE);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            auto comanda = Comanda<unordered_map<string, int>>::fromCSV(line);
            if (comanda) {
                comenzi.push_back(comanda);
            }
        }
        inFile.close();
    }

    switch (k) {
        case 1: {  // list
            cout << "\n=== ORDERS LIST ===\n";
            if (comenzi.empty()) {
                cout << "There are no orders.\n";
            } else {
                for (const auto& comanda : comenzi) {
                    comanda->display();
                    cout << "-------------------\n";
                }
            }
            break;
        }
        case 2: {  // adauga comanda
            string client, locatie;
            cout << "Client name: ";
            cin.ignore();
            getline(cin, client);

            cout << "Location: ";
            getline(cin, locatie);

            auto comanda = new Comanda<unordered_map<string, int>>(client, locatie);

            comanda->verificaFidelitate(COMENZI_FILE);

            while (true) {
                string produs;
                int cantitate;
                cout << "Product: ";
                getline(cin, produs);

                cout << "Quantity: ";
                cin >> cantitate;

                // adauga produs la comanda
                comanda->adaugaProdus(produs, cantitate, produse, locatie);

                cout << "Add another product? (1 = Yes, 0 = No): ";
                int opt;
                cin >> opt;
                cin.ignore();
                if (opt == 0) break;
            }

            comanda->calculeazaTotal();

            ofstream outFile(COMENZI_FILE, ios::app);
            outFile << comanda->toCSV() << "\n";
            outFile.close();

            ofstream produseOut(PRODUSE_FILE);
            for (const auto& p : produse) {
                produseOut << p->toCSV() << endl;
            }
            produseOut.close();

            cout << "Order successfully added!\n";
            delete comanda;
            break;
        }
    }
    for (auto& comanda : comenzi) {
        delete comanda;
    }
    for (auto& produs : produse) {
        delete produs;
    }
}
