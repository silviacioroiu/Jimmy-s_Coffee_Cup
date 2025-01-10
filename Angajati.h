// angajati.h modifications:
#ifndef ANGAJATI_H
#define ANGAJATI_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Employee {
protected:
    string name;
    string surname;
    string role;
    string location;
    int start_shift;
    int end_shift;
    double hourly_pay;

public:
    //contructor care e fol sa initializeze un ob cu valori specificate
    Employee(string name, string surname, string role, string location, int start, int end, double pay);

    //getters
    string getName() const;
    string getSurname() const;
    string getRole() const;
    string getLocation() const;
    int getStartShift() const;
    int getEndShift() const;
    double getHourlyPay() const;

    virtual void display() const;//polymorphism
    // permite claselor copii sa dea override la metoda display() ai sa includa role-specific behavior
    virtual ~Employee() = default;//destructor virtual

    // CSV
    string toCSV() const;
    static Employee* fromCSV(const string& line);
    static bool isValidLocation(const string& loc);
};
//INHERITANCE:
class Barista : public Employee {
public:
    Barista(string name, string surname, string location, int start, int end, double pay);
    void display() const override;
};

class Waiter : public Employee {
public:
    Waiter(string name, string surname, string location, int start, int end, double pay);
    void display() const override;
};

class Manager : public Employee {
public:
    Manager(string name, string surname, string location, int start, int end, double pay);
    void display() const override;
};

void gestionarePersonal(int k);

#endif // ANGAJATI_H