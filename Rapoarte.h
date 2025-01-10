#ifndef RAPOARTE_H
#define RAPOARTE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

// ProfitCalculationStrategy este o clasa de baza abstracta care defineste un sablon (metoda calculateProfit)
// pentru calculul profitului. Clasele care o mostenesc trebuie sa implementeze aceasta metoda,
// ceea ce permite adaptarea logica de calcul in functie de nevoi.

// DefaultProfitStrategy mosteneste ProfitCalculationStrategy si ofera o implementare concreta a metodei calculateProfit.
// Astfel, folosim **mostenirea** pentru a reutiliza structura de baza si adaugam logica specifica.
// Acest mecanism permite utilizarea **polimorfismului**, adica putem apela aceeasi metoda pe obiecte diferite.


// idk aparent (si aici am avut nevoie de ajutor )Strategy Design Pattern
class ProfitCalculationStrategy {
public:
    virtual double calculateProfit(const string& oras, double& goodsSold, double& goodsExpenses,
                                   double& employeeExpenses, double& eventExpenses) = 0;
    virtual ~ProfitCalculationStrategy() = default;
};

//INHERITANCE
class DefaultProfitStrategy : public ProfitCalculationStrategy {
public:
    double calculateProfit(const string& oras, double& goodsSold, double& goodsExpenses,
                           double& employeeExpenses, double& eventExpenses) override;
};


class Rapoarte {
private:
    shared_ptr<ProfitCalculationStrategy> strategy;

public:
    //constructor
    explicit Rapoarte(shared_ptr<ProfitCalculationStrategy> strategy)
            : strategy(move(strategy)) {}

    void generateDetailedReport(const string& oras) {
        double goodsSold = 0.0, goodsExpenses = 0.0, employeeExpenses = 0.0, eventExpenses = 0.0;
        double profit = strategy->calculateProfit(oras, goodsSold, goodsExpenses, employeeExpenses, eventExpenses);

        // Display the detailed report
        cout << fixed << setprecision(2);
        cout << "\n====== DETAILED REPORT FOR " << oras << " ======\n";
        cout << "Goods Sold: " << goodsSold << " RON\n";
        cout << "Goods Expenses: " << goodsExpenses << " RON\n";
        cout << "Employee Expenses: " << employeeExpenses << " RON\n";
        cout << "Events Expenses: " << eventExpenses << " RON\n";
        cout << "----------------------------------\n";
        cout << "Profit: " << profit << " RON\n";
        cout << "==================================\n";
    }

    static bool isValidLocation(const string& loc) {
        vector<string> validLocations = {"Bucuresti", "Cluj-Napoca", "Timisoara", "Iasi", "Brasov"};
        return find(validLocations.begin(), validLocations.end(), loc) != validLocations.end();
    }
};
void generateRapoarteCSV();
#endif
