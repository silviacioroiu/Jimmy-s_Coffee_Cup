#include "Rapoarte.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iomanip>

using namespace std;

// ok deci aici e un pic complicat pt am crezut ca citirea din comenzi.csv era facuta gresit si
//nu mi lua produsele comandate dar! cand programul citea linia din comenzi citea si statutul de fidelitate al clientului
// eu am incercat sa fac un meniu ca sa imi ia de acoloprodusul si costul lui dar am observat problema descrisa abia dupa o ora
// :P
class Menu {
private:
    unordered_map<string, double> manufacturingCosts;

public:
    explicit Menu(const string& menuFilePath) {
        ifstream menuFile(menuFilePath);
        if (!menuFile.is_open()) {
            cerr << "Error: Could not open " << menuFilePath << endl;
            return;
        }

        string line;
        getline(menuFile, line); // Skip header line
        while (getline(menuFile, line)) {
            stringstream ss(line);
            string productName, costFabricareStr, costVanzareStr;

            getline(ss, productName, ',');
            getline(ss, costFabricareStr, ',');
            getline(ss, costVanzareStr, ',');

            productName.erase(0, productName.find_first_not_of(" \t\n\r"));
            productName.erase(productName.find_last_not_of(" \t\n\r") + 1);

            manufacturingCosts[productName] = stod(costFabricareStr);
        }
        menuFile.close();
    }

    double getManufacturingCost(const string& productName) const {
        auto it = manufacturingCosts.find(productName);
        return (it != manufacturingCosts.end()) ? it->second : 0.0;
    }
};

// Implementation of DefaultProfitStrategy
// calcularea profitului este realizata astfel:
//fiecare oras are profitul proipiu in functie de evenimente,produse vandute si angajati
//profit = total_comenzi - costul_de_cumparare(cat plateste cafeneaua sa fac de ex un latte- lapte ,cafea, sirop, energie etc)- salariu_angajati - event_cost
//costul de cumparare = costul_produsului_comandat * cantitae_comandata
double DefaultProfitStrategy::calculateProfit(const string& city, double& goodsSold, double& goodsExpenses,
                                              double& employeeExpenses, double& eventExpenses) {
    const string menuPath = "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\menu.csv";
    const string comenziPath = "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\comenzi.csv";
    const string angajatiPath = "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\angajati.csv";
    const string evenimentePath = "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\evenimente.csv";

    goodsSold = 0;
    goodsExpenses = 0;
    employeeExpenses = 0;
    eventExpenses = 0;

    Menu menu(menuPath);

    // Process comenzi.csv for goods sold and goods expenses
    ifstream comenziFile(comenziPath);
    if (!comenziFile.is_open()) {
        cerr << "Error: Could not open " << comenziPath << endl;
        return 0.0;
    }

    string line;
    while (getline(comenziFile, line)) {
        stringstream ss(line);
        string clientName, cityName, totalStr, loyaltyStr, productsStr;

        getline(ss, clientName, ',');
        getline(ss, cityName, ',');
        getline(ss, totalStr, ',');
        getline(ss, loyaltyStr, ','); // Skip loyalty status //!!!!! aici aici doamne
        getline(ss, productsStr, ',');

        if (cityName == city) {
            goodsSold += stod(totalStr);

            stringstream productsStream(productsStr);
            string productPair;

            while (getline(productsStream, productPair, ';')) {
                size_t pos = productPair.find(':');
                if (pos != string::npos) {
                    string productName = productPair.substr(0, pos);
                    int quantity = stoi(productPair.substr(pos + 1));

                    productName.erase(0, productName.find_first_not_of(" \t\n\r"));
                    productName.erase(productName.find_last_not_of(" \t\n\r") + 1);

                    double costFabricare = menu.getManufacturingCost(productName);
                    if (costFabricare > 0.0) {
                        goodsExpenses += costFabricare * quantity;
                    } else {
                        cerr << "Warning: Product '" << productName << "' not found in menu.csv!" << endl;
                    }
                } else {
                    cerr << "Malformed product pair: '" << productPair << "' in comenzi.csv!" << endl;
                }
            }
        }
    }
    comenziFile.close();

    ifstream angajatiFile(angajatiPath);
    if (!angajatiFile.is_open()) {
        cerr << "Error: Could not open " << angajatiPath << endl;
        return 0.0;
    }

    while (getline(angajatiFile, line)) {
        stringstream ss(line);
        string nume, prenume, role, locatie, oraStartStr, oraSfarsitStr, salariuPeOraStr;

        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, role, ',');
        getline(ss, locatie, ',');
        getline(ss, oraStartStr, ',');
        getline(ss, oraSfarsitStr, ',');
        getline(ss, salariuPeOraStr, ',');

        if (locatie == city) {
            int oraStart = stoi(oraStartStr);
            int oraSfarsit = stoi(oraSfarsitStr);
            double salariuPeOra = stod(salariuPeOraStr);

            employeeExpenses += (oraSfarsit - oraStart) * salariuPeOra;
        }
    }
    angajatiFile.close();

    ifstream evenimenteFile(evenimentePath);
    if (!evenimenteFile.is_open()) {
        cerr << "Error: Could not open " << evenimentePath << endl;
        return 0.0;
    }

    while (getline(evenimenteFile, line)) {
        stringstream ss(line);
        string denumire, locatie, costStr, activStr;

        getline(ss, denumire, ',');
        getline(ss, locatie, ',');
        getline(ss, costStr, ',');
        getline(ss, activStr, ',');

        if (locatie == city && activStr == "1") {
            eventExpenses += stod(costStr);
        }
    }
    evenimenteFile.close();

    return goodsSold - goodsExpenses - employeeExpenses - eventExpenses;
}

// Generate rapoarte.csv
void generateRapoarteCSV() {
    const vector<string> locations = {"Bucuresti", "Cluj-Napoca", "Timisoara", "Iasi", "Brasov"};
    const string outputPath = "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\rapoarte.csv";

    ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open " << outputPath << " for writing." << endl;
        return;
    }

    // Write header
    outFile << "Location,Goods Sold,Goods Expenses,Employee Expenses,Events Expenses,Profit\n";

    shared_ptr<ProfitCalculationStrategy> strategy = make_shared<DefaultProfitStrategy>();
    Rapoarte calculator(strategy);

    for (const auto& city : locations) {
        double goodsSold = 0.0, goodsExpenses = 0.0, employeeExpenses = 0.0, eventExpenses = 0.0;
        double profit = strategy->calculateProfit(city, goodsSold, goodsExpenses, employeeExpenses, eventExpenses);

        outFile << city << "," << fixed << setprecision(2)
                << goodsSold << "," << goodsExpenses << "," << employeeExpenses << ","
                << eventExpenses << "," << profit << "\n";
    }

    outFile.close();
    //cout << "generat " << outputPath << endl; o verificare
}
