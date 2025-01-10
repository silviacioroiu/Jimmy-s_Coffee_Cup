#include "Angajati.h"
#include <algorithm>
//ENCAPSULARE
string Employee::getName() const {
    return name;
}

string Employee::getSurname() const {
    return surname;
}

string Employee::getRole() const {
    return role;
}

int Employee::getStartShift() const {
    return start_shift;
}

int Employee::getEndShift() const {
    return end_shift;
}

double Employee::getHourlyPay() const {
    return hourly_pay;
}
//constructor
Employee::Employee(string name, string surname, string role, string location, int start, int end, double pay)
        : name(name), surname(surname), role(role), location(location), start_shift(start), end_shift(end),
          hourly_pay(pay) {}//instantiaza un obiect de clasa Employee

string Employee::getLocation() const {
    return location;
}

bool Employee::isValidLocation(const string &loc) {
    vector<string> validLocations = {"Bucuresti", "Cluj-Napoca", "Timisoara", "Iasi", "Brasov"};
    return find(validLocations.begin(), validLocations.end(), loc) != validLocations.end();
}

void Employee::display() const {
    cout << "Name: " << name << " " << surname << ", Role: " << role << ", Location: " << location
         << ", Shift: " << start_shift << ":00 - " << end_shift
         << ":00, Pay: " << hourly_pay << " RON/hour" << endl;
}
//ABSTRACTIZARE
//cele doua operatii toCSV si fromCSV sunt abstractizate in clasa de baza
string Employee::toCSV() const {
    return name + "," + surname + "," + role + "," + location + "," +
           to_string(start_shift) + "," + to_string(end_shift) + "," +
           to_string(hourly_pay);
}

Employee *Employee::fromCSV(const string &line) {
    stringstream ss(line);
    string name, surname, role, location;
    int start, end;
    double pay;

    getline(ss, name, ',');
    getline(ss, surname, ',');
    getline(ss, role, ',');
    getline(ss, location, ',');

    string temp;
    getline(ss, temp, ',');
    start = stoi(temp);
    getline(ss, temp, ',');
    end = stoi(temp);
    getline(ss, temp, ',');
    pay = stod(temp);

    if (role == "Barista")
        return new Barista(name, surname, location, start, end, pay);
    else if (role == "Waiter")
        return new Waiter(name, surname, location, start, end, pay);
    else if (role == "Manager")
        return new Manager(name, surname, location, start, end, pay);

    return nullptr;
}
//INHERITANCE
void Barista::display() const {
    cout << "[Barista] ";
    Employee::display();
}

void Waiter::display() const {
    cout << "[Waiter] ";
    Employee::display();
}

void Manager::display() const {
    cout << "[Manager] ";
    Employee::display();
}

// Update constructors for derived classes
Barista::Barista(string name, string surname, string location, int start, int end, double pay)
        : Employee(name, surname, "Barista", location, start, end, pay) {}

Waiter::Waiter(string name, string surname, string location, int start, int end, double pay)
        : Employee(name, surname, "Waiter", location, start, end, pay) {}

Manager::Manager(string name, string surname, string location, int start, int end, double pay)
        : Employee(name, surname, "Manager", location, start, end, pay) {}

//functia "main"/importanta pentru gestionarea personalului care primeste ca argument optiunea de la main
void gestionarePersonal(int k) {
    const string CSV_FILE = "C:\\Users\\cioro\\Desktop\\POO\\JimmysCup\\angajati.csv";

    switch (k) {
        case 1: {  // lista cu toti angajatii
            ifstream file(CSV_FILE);
            if (!file.is_open()) {
                cout << "Could not open file: " << CSV_FILE << endl;
                return;
            }

            cout << "\n====== Employee List ======\n";
            string line;
            int count = 0;
            while (getline(file, line)) {
                stringstream ss(line);
                string name, surname, role, location, temp;
                int start, end;
                double pay;

                getline(ss, name, ',');
                getline(ss, surname, ',');
                getline(ss, role, ',');
                getline(ss, location, ',');
                getline(ss, temp, ',');
                start = stoi(temp);
                getline(ss, temp, ',');
                end = stoi(temp);
                getline(ss, temp, ',');
                pay = stod(temp);

                cout << ++count << ". " << name << " " << surname << endl;
                cout << "   Role: " << role << endl;
                cout << "   Location: " << location << endl;
                cout << "   Shift: " << start << ":00 - " << end << ":00" << endl;
                cout << "   Pay: " << pay << " RON/hour" << endl;
                cout << "-------------------------\n";
            }

            if (count == 0) {
                cout << "No employees found.\n";
            }
            file.close();
            break;
        }
        case 2: {  // angajeaza
            string name, surname, role, location;
            int start, end;
            double pay;

            cout << "Enter employee details:\n";
            cout << "Name: ";
            cin >> name;
            cout << "Surname: ";
            cin >> surname;
            cout << "Role (Barista/Waiter/Manager): ";
            cin >> role;
            do {
                cout << "Location (Bucuresti/Cluj-Napoca/Timisoara/Iasi/Brasov): ";
                cin >> location;
            } while (!Employee::isValidLocation(location));
            cout << "Start shift (hour): ";
            cin >> start;
            cout << "End shift (hour): ";
            cin >> end;
            cout << "Hourly pay: ";
            cin >> pay;

            Employee *emp = nullptr;
            if (role == "Barista")
                emp = new Barista(name, surname, location, start, end, pay);
            else if (role == "Waiter")
                emp = new Waiter(name, surname, location, start, end, pay);
            else if (role == "Manager")
                emp = new Manager(name, surname, location, start, end, pay);

            if (emp) {
                ofstream file(CSV_FILE, ios::app);
                file << emp->toCSV() << endl;
                file.close();
                delete emp;
                cout << "Employee added successfully!\n";
            }
            break;
        }
        case 3: {  // concediaza
            string name, surname;
            cout << "Enter employee name to fire: ";
            cin >> name;
            cout << "Enter employee surname: ";
            cin >> surname;

            vector<string> employees;
            ifstream inFile(CSV_FILE);
            string line;
            bool found = false;

            while (getline(inFile, line)) {
                Employee *emp = Employee::fromCSV(line);
                if (emp) {
                    if (emp->getName() != name || emp->getSurname() != surname) {
                        employees.push_back(line);
                    } else {
                        found = true;
                    }
                    delete emp;
                }
            }
            inFile.close();

            if (found) {
                ofstream outFile(CSV_FILE);
                for (const string &emp: employees) {
                    outFile << emp << endl;
                }
                outFile.close();
                cout << "Employee fired successfully!\n";//naspa pentru el
            } else {
                cout << "Employee not found!\n";
            }
            break;
        }
        case 4: {  // schimba programul
            string name, surname;
            cout << "Enter employee name: "; cin >> name;
            cout << "Enter employee surname: "; cin >> surname;

            vector<string> employees;
            ifstream inFile(CSV_FILE);
            string line;
            bool found = false;


            while (getline(inFile, line)) {
                stringstream ss(line);
                string curr_name, curr_surname, role, location, temp;
                int start, end;
                double pay;


                getline(ss, curr_name, ',');
                getline(ss, curr_surname, ',');
                getline(ss, role, ',');
                getline(ss, location, ',');
                getline(ss, temp, ',');
                start = stoi(temp);
                getline(ss, temp, ',');
                end = stoi(temp);
                getline(ss, temp, ',');
                pay = stod(temp);

                if (curr_name == name && curr_surname == surname) {
                    found = true;
                    int new_start, new_end;
                    cout << "Current schedule: " << start << ":00 - " << end << ":00\n";
                    cout << "Enter new start hour (0-23): "; cin >> new_start;
                    cout << "Enter new end hour (0-23): "; cin >> new_end;

                    // verificare orelor
                    if (new_start < 0 || new_start > 23 || new_end < 0 || new_end > 23 || new_start >= new_end) {
                        cout << "Invalid hours! Schedule unchanged.\n";
                        employees.push_back(line);
                        continue;
                    }
                    stringstream new_line;
                    new_line << curr_name << "," << curr_surname << "," << role << ","
                             << location << "," << new_start << "," << new_end << "," << pay;
                    employees.push_back(new_line.str());
                    cout << "Schedule updated successfully!\n";
                } else {
                    employees.push_back(line);
                }
            }
            inFile.close();

            if (!found) {
                cout << "Employee not found!\n";
                break;
            }

            // rescrie employees
            ofstream outFile(CSV_FILE);
            for (const string& emp : employees) {
                outFile << emp << endl;
            }
            outFile.close();
            break;
        }
    }
}