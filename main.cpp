
//Silvia Cioroiu 321 AC ---- Jimmy's Coffee Cup <3
/*
 Cuprins concepte folosite:(aici sunt doar exemple , sunt mai multe unde am si  comentat)

Clase și Obiecte : le-am folosit la orice pas exemple:class Employee, Comanda , Evenimente , Produs , Rapoarte
Constructori și Destructori : le-am folosit la orice pas exemple: Employee(linia 25 - Angajati.h),~Produs() = default(linia 51-Produse.h)
Încapsulare: exemple :class Produs unde am folosit gettere si settere , saupt clasa Comanda unde am atribute private care sunt accesate prin metode publice
Moștenire: exemple : class Barista Waiter si Manager din class Employee
Polimorfism : exemple :virtual void display() const(linia 36 - Angajati.h)
Abstractizare : exemple: string toCSV() const(linia 39 - Produse.h)
Templates : linia 15 Comenzi.h-template <typename Container = unordered_map<string, int>>
Exceptions : main menu , si in evenimente.cpp-linia 156
Design Patterns: Observer Pattern pentru Evenimente.h linia 14
*/

#include <iostream>
#include "Angajati.h"
#include <thread>

#include "Produse.h"
#include "Comenzi.h"
#include "Evenimente.h"
#include "Rapoarte.h"

using namespace std;

void eepy(int x) {
    this_thread::sleep_for(chrono::seconds(x));
}

void dotdotdot() {
    eepy(1);
    cout << ".";
    eepy(1);
    cout << ".";
    eepy(1);
    cout << "." << endl;
    eepy(1);
}

void clearConsole() {
    for (int i = 0; i < 100; ++i) {
        cout << endl;
    }
}
//DRY- functii luate de la tema cu rezervarea biletelor de tren

// Functii pentru gestionarea fiecarei optiuni de meniu
void main_menu() {
    //Exceptii
    while (true) {
        try {
            cout << "\n=========== MAIN MENU ===========\n";
            cout << "1. Personnel management\n";
            cout << "2. Stock management\n";
            cout << "3. Customer orders\n";
            cout << "4. Events\n";
            cout << "5. Raports\n";
            cout << "6. Exit\n";
            cout << "=======================================\n";
            cout << "Select an option:" << endl;

            int optiune;
            cin >> optiune;

            if (cin.fail()) {
                throw runtime_error("Invalid Input! Choose a number.");
            }

            switch (optiune) {
                case 1:
                    cout << "1. Personnel list\n";
                    cout << "2. Hire\n";
                    cout << "3. Fire\n";
                    cout << "4. Change Schedule\n";
                    int k;
                    cin >> k;
                    gestionarePersonal(k);
                    break;
                case 2:
                    cout << "1. Stock List\n";
                    cout << "2. Add New Product\n";
                    cout << "3. Remove Product\n";
                    cout << "4. Order More\n";
                    int k2;
                    cin >> k2;
                    gestionareStocuri(k2);
                    break;
                case 3:
                    cout << "1. Orders List\n";
                    cout << "2. Add Order\n";
                    int k3;
                    cin >> k3;
                    gestionareComenzi(k3);
                    break;
                case 4:
                    cout << "1. Events List\n";
                    cout << "2. Add New Event\n";
                    cout << "3. Activate / Deactivate Event\n";
                    cout << "4. Delete Event\n";
                    int k4;
                    cin >> k4;
                    gestionareEvenimente(k4);
                    break;
                case 5: {
                    generateRapoarteCSV();
                    string oras;
                    cout << "Enter the city for which to generate the profit report (Bucuresti/Cluj-Napoca/Timisoara/Iasi/Brasov): ";
                    cin.ignore();
                    getline(cin, oras);

                    try {
                        if (!Rapoarte::isValidLocation(oras)) {
                            throw invalid_argument("Invalid city entered.");
                        }

                        shared_ptr<ProfitCalculationStrategy> strategy = make_shared<DefaultProfitStrategy>();
                        Rapoarte calculator(strategy);

                        calculator.generateDetailedReport(oras); // Generate and display report

                    } catch (const exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                    break;
                }
                case 6:
                    cout << "Byee!\n";
                    return;
                default:
                    throw invalid_argument("Invalid option! Choose a number between 1 and 6.");
            }
        } catch (const runtime_error &e) {
            cin.clear();

            cout << "Error: " << e.what() << "\n";
        } catch (const invalid_argument &e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

void intro() {
    eepy(1);
    cout << "Wellcome to your growing buisness , Jimmy's Coffee Cup <3" << endl;
    cout << "Let's manage it";
    eepy(2);
    dotdotdot();
    clearConsole();
}

int main() {
    intro();
    main_menu();
    return 0;
}
