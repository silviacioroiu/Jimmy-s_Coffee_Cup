#ifndef EVENIMENTE_H
#define EVENIMENTE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <iostream>

using namespace std;

// DESIGN PATTERN!!!!!!!!!!!!- AICI AM AVUT NEVOIE DE AJUTOR
class EventObserver {
public:
    //ABSTRACTIZARE
    virtual void onEventStatusChanged(const string& eventName, bool status) = 0;
    virtual ~EventObserver() = default;
};

//practic e si inheritance
class EventLogger : public EventObserver {
public:
    void onEventStatusChanged(const string& eventName, bool status) override {
        ofstream log("event_log.txt", ios::app);
        log << "Event: " << eventName << " changed status to "
            << (status ? "active" : "inactive") << "\n";
        log.close();
    }
};

// Event class
class Evenimente {
private:
    //atribute private
    bool activ;
    string oras;
    string denumire;
    int cost_de_organizare;
    static vector<shared_ptr<EventObserver>> observers;

    static void notifyObservers(const string& eventName, bool status) {
        for (const auto& observer : observers) {
            observer->onEventStatusChanged(eventName, status);
        }
    }

public:
    //constructor
    Evenimente(string denumire, string oras, int cost, bool activ = false);
    //observer class design pattern
    static void addObserver(shared_ptr<EventObserver> observer) {
        observers.push_back(observer);
    }

    static void afisareEvenimente();
    static void adaugaEveniment();
    static void activeazaEveniment(string denumire);
    static void stergeEveniment(string denumire);
    void salvareEveniment();

    // getters-ENCAPSULATION
    bool getActiv() const { return activ; }
    string getOras() const { return oras; }
    string getDenumire() const { return denumire; }
    int getCost() const { return cost_de_organizare; }
};

void gestionareEvenimente(int optiune);

#endif
