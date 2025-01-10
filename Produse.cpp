#include "Produse.h"
//constructor
Produs::Produs(string nume_produs, int cod, int cost_fabricare, int cost_vanzare, int cantitate, string locatie)
        : nume_produs(nume_produs), cod(cod), cost_de_fabricare(cost_fabricare),
          cost_vanzare(cost_vanzare), cantitate(cantitate), locatie(locatie) {}
//ENCAPSULATION
string Produs::getNumeProdus() const { return nume_produs; }
int Produs::getCod() const { return cod; }
int Produs::getCostFabricare() const { return cost_de_fabricare; }
int Produs::getCostVanzare() const { return cost_vanzare; }
int Produs::getCantitate() const { return cantitate; }
string Produs::getLocatie() const { return locatie; }

bool Produs::isValidLocation(const string& loc) {
    vector<string> validLocations = {"Bucuresti", "Cluj-Napoca", "Timisoara", "Iasi", "Brasov"};
    return find(validLocations.begin(), validLocations.end(), loc) != validLocations.end();
}

void Produs::setCantitate(int cantitate) {
    this->cantitate = cantitate;
}

void Produs::display() const {
    cout << "Name: " << nume_produs
         << "\nCode: " << cod
         << "\nLocation: " << locatie
         << "\nManufacturing cost: " << cost_de_fabricare << " RON"
         << "\nSale price: " << cost_vanzare << " RON"
         << "\nQuantity: " << cantitate << " buc\n";
}
//ABSTRACTIZARE
string Produs::toCSV() const {
    return nume_produs + "," + to_string(cod) + "," +
           to_string(cost_de_fabricare) + "," + to_string(cost_vanzare) +
           "," + to_string(cantitate) + "," + locatie;
}

Produs* Produs::fromCSV(const string& line) {
    stringstream ss(line);
    string nume, temp, loc;
    int cod = 0, cost_fab = 0, cost_vanz = 0, cant = 0;
    //exceptii
    try {
        getline(ss, nume, ',');
        getline(ss, temp, ',');
        cod = stoi(temp);  // Convert cod to integer
        getline(ss, temp, ',');
        cost_fab = stoi(temp);  // Convert cost_fabricare to integer
        getline(ss, temp, ',');
        cost_vanz = stoi(temp);  // Convert cost_vanzare to integer
        getline(ss, temp, ',');
        cant = stoi(temp);  // Convert cantitate to integer
        getline(ss, loc, ',');  // Location (string)

        return new Produs(nume, cod, cost_fab, cost_vanz, cant, loc);
    } catch (const exception& e) {
        cout << "Error parsing CSV line: " << line << "\n";
        cout << "Exception: " << e.what() << "\n";
        return nullptr;
    }
}

void gestionareStocuri(int k) {
    const string CSV_FILE = "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\produse.csv";
    vector<Produs*> produse;

    // Read products from CSV
    ifstream inFile(CSV_FILE);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            Produs* p = Produs::fromCSV(line);
            if (p) {
                produse.push_back(p);
            }
        }
        inFile.close();
    }

    switch (k) {
        case 1: {  // List by city
            string oras;
            cout << "\nCity (Bucuresti/Cluj-Napoca/Timisoara/Iasi/Brasov): ";
            cin >> oras;

            // validare locatie
            if (!Produs::isValidLocation(oras)) {
                cout << "Invalid City. Enter a valid city.\n";
                break;
            }

            cout << "\n=== PRODUCT STOCK IN " << oras << " ===\n";
            bool found = false;

            for (const auto& p : produse) {
                if (p->getLocatie() == oras) {
                    p->display();
                    cout << "-------------------\n";
                    found = true;
                }
            }

            if (!found) {
                cout << "There are no products in the chosen city.\n";
            }

            break;
        }
        case 2: {  // adauga produs
            string nume, locatie;
            int cod, cost_fab, cost_vanz, cant;

            cout << "Add new product:\n";
            cout << "Name: ";
            cin.ignore();
            getline(cin, nume);
            cout << "Code: "; cin >> cod;
            cout << "Manufacturing cost: "; cin >> cost_fab;
            cout << "Sale price: "; cin >> cost_vanz;
            cout << "Quantity: "; cin >> cant;

            do {
                cout << "Location (Bucuresti/Cluj-Napoca/Timisoara/Iasi/Brasov): ";
                cin >> locatie;
            } while (!Produs::isValidLocation(locatie));

            produse.push_back(new Produs(nume, cod, cost_fab, cost_vanz, cant, locatie));
            cout << "Product added!\n";
            break;
        }
        case 3: {  //Sterge produs din lista
            int cod;
            cout << "The code for the product that you want to be deleted: "; cin >> cod;

            auto it = remove_if(produse.begin(), produse.end(),
                                [cod](Produs* p) { return p->getCod() == cod; });

            if (it != produse.end()) {
                for (auto iter = it; iter != produse.end(); ++iter) {
                    delete *iter;  // Free
                }
                produse.erase(it, produse.end());
                cout << "Product deleted!\n";
            } else {
                cout << "Product not found!\n";
            }
            break;
        }
        case 4: {  //gen comanda mai multe
            string nume_produs, oras;
            int cant;

            cout << "Name product: ";
            cin.ignore();
            getline(cin, nume_produs);

            cout << "City (Bucuresti/Cluj-Napoca/Timisoara/Iasi/Brasov): ";
            cin >> oras;

            cout << "Quantity you want to order: ";
            cin >> cant;

            auto it = find_if(produse.begin(), produse.end(),
                              [&nume_produs, &oras](Produs* p) {
                                  return p->getNumeProdus() == nume_produs && p->getLocatie() == oras;
                              });

            if (it != produse.end()) {
                (*it)->setCantitate((*it)->getCantitate() + cant);
                cout << "The quantity has been successfully updated! The new quantity is: " << (*it)->getCantitate() << " buc.\n";
            } else {
                cout << "The product was not found in the specified city.\n";
            }
            break;
        }
    }


    ofstream outFile(CSV_FILE);
    for (const auto& p : produse) {
        outFile << p->toCSV() << endl;
    }
    outFile.close();

    // Free
    for (auto& p : produse) {
        delete p;
    }
}
