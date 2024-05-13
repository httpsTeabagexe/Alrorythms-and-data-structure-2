#include "header.h"

// Function to create a new airport node
Airport* createAirport(string codeIATA) {
    Airport* newAirport = new Airport;
    newAirport->codeIATA = codeIATA;
    newAirport->airlines = nullptr;
    newAirport->next = nullptr;
    return newAirport;
}

// Function to create a new airline node
Airline* createAirline(string name) {
    Airline* newAirline = new Airline;
    newAirline->name = name;
    newAirline->next = nullptr;
    return newAirline;
}

// Function to add an airport to the end of the list
void addAirport(Airport*& head, string codeIATA) {
    Airport* newAirport = createAirport(codeIATA);
    if (head == nullptr) {
        head = newAirport;
    }
    else {
        Airport* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newAirport;
    }
}

// Function to add an airline to an airport
void addAirline(Airport* airport, string name) {
    Airline* newAirline = createAirline(name);
    if (airport->airlines == nullptr) {
        airport->airlines = newAirline;
    }
    else {
        Airline* current = airport->airlines;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newAirline;
    }
}

// Function to delete an airport by IATA code

//void deleteAirport(Airport*& head, string codeIATA) {
//    if (head == nullptr) return;
//
//    if (head->codeIATA == codeIATA) {
//        Airport* temp = head;
//        head = head->next;
//        // Delete all airlines associated with this airport
//        Airline* currentAirline = temp->airlines;
//        while (currentAirline != nullptr) {
//            Airline* tempAirline = currentAirline;
//            currentAirline = currentAirline->next;
//            delete tempAirline;
//        }
//        delete temp;
//        return;
//    }
//
//    Airport* prev = nullptr;
//    Airport* current = head;
//    while (current != nullptr && current->codeIATA != codeIATA) {
//        prev = current;
//        current = current->next;
//    }
//
//    if (current != nullptr) {
//        prev->next = current->next;
//        // Delete all airlines associated with this airport
//        Airline* currentAirline = current->airlines;
//        while (currentAirline != nullptr) {
//            Airline* tempAirline = currentAirline;
//            currentAirline = currentAirline->next;
//            delete tempAirline;
//        }
//        delete current;
//    }
//}

void deleteAirport(Airport*& head, std::string codeIATA) {
    if (head == nullptr) return;

    if (head->codeIATA == codeIATA) {
        Airport* temp = head;
        head = head->next;

        // Delete all airlines associated with this airport
        Airline* currentAirline = temp->airlines;
        while (currentAirline != nullptr) {
            Airline* tempAirline = currentAirline;
            currentAirline = currentAirline->next;
            delete tempAirline;
        }
        delete temp;
        return;
    }

    Airport* current = head;
    Airport* prev = nullptr;  // Initialize prev here

    while (current != nullptr && current->codeIATA != codeIATA) {
        prev = current;        // Update prev after moving to the next node
        current = current->next;
    }

    if (current != nullptr) {
        if (prev != nullptr) {  // Check if prev is valid
            prev->next = current->next;
        }

        // Delete all airlines associated with this airport
        Airline* currentAirline = current->airlines;
        while (currentAirline != nullptr) {
            Airline* tempAirline = currentAirline;
            currentAirline = currentAirline->next;
            delete tempAirline;
        }
        delete current;
    }
}

//// Function to delete an airline from an airport
//void deleteAirline(Airport* airport, string name) {
//    if (airport->airlines == nullptr) return;
//
//    if (airport->airlines->name == name) {
//        Airline* temp = airport->airlines;
//        airport->airlines = airport->airlines->next;
//        delete temp;
//        return;
//    }
//
//    Airline* prev = nullptr;
//    Airline* current = airport->airlines;
//    while (current != nullptr && current->name != name) {
//        prev = current;
//        current = current->next;
//    }
//
//    if (current != nullptr) {
//        prev->next = current->next;
//        delete current;
//    }
//}

void deleteAirline(Airport* airport, string name) {
    if (airport->airlines == nullptr) return;

    if (airport->airlines->name == name) {
        Airline* temp = airport->airlines;
        airport->airlines = airport->airlines->next;
        delete temp;
        return;
    }

    Airline* current = airport->airlines;
    Airline* prev = nullptr; // Initialize prev here

    while (current != nullptr && current->name != name) {
        prev = current;     // Update prev only after you've moved to the next node
        current = current->next;
    }

    if (current != nullptr) {
        if (prev != nullptr) { // Check if prev is valid
            prev->next = current->next;
        }
        delete current;
    }
}

// Function to print the list of airports and airlines
void printAirports(Airport* head) {
    if (head == nullptr) {
        cout << "The list of airports is empty." << endl;
        return;
    }

    cout << "List of airports:" << endl;
    Airport* currentAirport = head;
    while (currentAirport != nullptr) {
        cout << "IATA Code: " << currentAirport->codeIATA << endl;
        Airline* currentAirline = currentAirport->airlines;
        if (currentAirline != nullptr) {
            cout << "  Airlines:" << endl;
            while (currentAirline != nullptr) {
                cout << "    " << currentAirline->name << endl;
                currentAirline = currentAirline->next;
            }
        }
        currentAirport = currentAirport->next;
    }
}

// Function to load data from a file
bool loadAirportsFromFile(Airport*& head, string filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        return false;
    }

    string line;
    while (getline(inFile, line)) {
        size_t tabPos = line.find('\t');
        if (tabPos != string::npos) {
            string airlineName = line.substr(0, tabPos);
            string codeIATA = line.substr(tabPos + 1);

            Airport* airport = findAirportByCode(head, codeIATA);
            if (airport == nullptr) {
                airport = createAirport(codeIATA);
                addAirport(head, codeIATA);
            }
            addAirline(airport, airlineName);
        }
        else {
            cerr << "ERROR: Invalid data format in line: " << line << endl;
        }
    }
    inFile.close();
    return true;
}

// Function to save data to a file
void saveAirportsToFile(Airport* head, string filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "ERROR: Unable to open the file for writing." << endl;
        return;
    }

    Airport* currentAirport = head;
    while (currentAirport != nullptr) {
        Airline* currentAirline = currentAirport->airlines;
        while (currentAirline != nullptr) {
            outFile << currentAirline->name << "\t" << currentAirport->codeIATA << endl;
            currentAirline = currentAirline->next;
        }
        currentAirport = currentAirport->next;
    }
    outFile.close();
    cout << "List of airports successfully saved to file " << filename << endl;
}

// Function to find an airport by IATA code
Airport* findAirportByCode(Airport* head, string codeIATA) {
    Airport* current = head;
    while (current != nullptr) {
        if (current->codeIATA == codeIATA) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Function to display the menu and get user's choice
int menuSelection() {
    while (true) {
        cout << "\nMenu:\n"
            << "1. Add airport to the end of the list\n"
            << "2. Add an airline to an airport\n"
            << "3. Delete an airport from the list\n"
            << "4. Delete an airline from an airport\n"
            << "5. Display the list of airports\n"
            << "6. Load data from a file\n"
            << "7. Save data to a file\n"
            << "0. Exit\n";

        int choice = _getch() - '0'; // Directly convert char to int
        if (choice >= 0 && choice <= 7) {
            return choice;
        }
        else {
            cerr << "\nInvalid choice. Please enter a number between 0 and 7.\n";
        }
    }
}

int main() {
    Airport* head = nullptr;
    string filename = "airports.txt";

    while (true) {
        int choice = menuSelection();

        switch (choice) {
        case 1: {
            string codeIATA;
            cout << "Enter the airport's IATA code: ";
            cin >> codeIATA;
            addAirport(head, codeIATA);
            break;
        }
        case 2: {
            string codeIATA, airlineName;
            cout << "Enter the airport's IATA code: ";
            cin >> codeIATA;
            Airport* airport = findAirportByCode(head, codeIATA);
            if (airport != nullptr) {
                cout << "Enter the airline name: ";
                cin >> airlineName;
                addAirline(airport, airlineName);
            }
            else {
                cerr << "Airport with this IATA code not found.\n";
            }
            break;
        }
        case 3: {
            string codeIATA;
            cout << "Enter the airport's IATA code to delete: ";
            cin >> codeIATA;
            deleteAirport(head, codeIATA);
            break;
        }
        case 4: {
            string codeIATA, airlineName;
            cout << "Enter the airport's IATA code: ";
            cin >> codeIATA;
            Airport* airport = findAirportByCode(head, codeIATA);
            if (airport != nullptr) {
                cout << "Enter the airline name to delete: ";
                cin >> airlineName;
                deleteAirline(airport, airlineName);
            }
            else {
                cout << "Airport with this IATA code not found.\n";
            }
            break;
        }
        case 5:
            printAirports(head);
            break;
        case 6:
            if (loadAirportsFromFile(head, filename)) {
                cout << "Data successfully loaded from file " << filename << endl;
            }
            else {
                cerr << "ERROR: Unable to open the file for reading." << endl;
            }
            break;
        case 7:
            saveAirportsToFile(head, filename);
            break;
        case 0:
            // Memory release before exiting
            while (head != nullptr) {
                Airport* tempAirport = head;
                head = head->next;
                Airline* currentAirline = tempAirport->airlines;
                while (currentAirline != nullptr) {
                    Airline* tempAirline = currentAirline;
                    currentAirline = currentAirline->next;
                    delete tempAirline;
                }
                delete tempAirport;
            }
            return 0;
        }
    }
}