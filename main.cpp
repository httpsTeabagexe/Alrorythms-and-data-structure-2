#include "header.h"
int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Airport* head = nullptr;
    string filename;
    bool dataModified = false;

    while (true) {
        int choice = menuSelection();

        switch (choice) {
        case 1: { // Add airport
            while (true) {
                string codeIATA = getValidIATACode();
                if (codeIATA == "~") break; // Return to main menu

                // Check for duplicate IATA code
                if (findAirportByCode(head, codeIATA) != nullptr) {
                    setColor(12);
                    cerr << "ERROR: An airport with IATA code '" << codeIATA << "' already exists.\n";
                    resetColor();
                    continue; // Ask for another IATA code
                }

                addAirport(head, codeIATA);
                dataModified = true;
            }
            break;
        }

        case 2: { // Add airline
            string codeIATA;
            cout << "Enter the airport's IATA code: ";
            cin >> codeIATA;
            Airport* airport = findAirportByCode(head, codeIATA);

            if (airport != nullptr) {
                while (true) { // Loop for entering airlines
                    string airlineName = getValidAirlineName();
                    if (airlineName == "~") break; // Exit to main menu

                    addAirline(airport, airlineName);
                    dataModified = true;
                }
            }
            else {
                setColor(12);
                cerr << "ERROR: Airport with this IATA code not found.\n";
                resetColor();
            }
            break;
        }
        case 3: { // Delete airport
            cout << "List of existing airports:" << endl;
            printAirports(head); // Show the list of airports

            if (head == nullptr) {
                continue; // Go back to main menu if the list is empty
            }

            string codeIATA;
            codeIATA = getValidIATACode(); // Use the validation function 
            if (codeIATA == "~") continue;

            deleteAirport(head, codeIATA);
            dataModified = true;
            break;
        }
        case 4: { // Delete airline
            cout << "List of existing airports:" << endl;
            printAirports(head); // Show the list of airports

            string codeIATA, airlineName;
            codeIATA = getValidIATACode(); // Use the validation function 
            if (codeIATA == "~") continue;
            Airport* airport = findAirportByCode(head, codeIATA);
            if (airport != nullptr) {
                cout << "Enter the airline name to delete: ";
                cin >> airlineName;
                deleteAirline(airport, airlineName);
                dataModified = true;
            }
            else {
                setColor(12);
                cerr << "ERROR: Airport with this IATA code not found.\n";
                resetColor();
            }
            break;
        }
        case 5: // Print airports
            printAirports(head);
            break; // dataModified doesn't need to be changed here

        case 6: { // Load from file
            cout << "List of .txt files in current folder" << endl;
            setColor(2);
            system("dir /b *.txt");
            resetColor();
            filename = getFilename();
            if (filename == "~") continue;
            if (loadAirportsFromFile(head, filename)) {
                dataModified = false;
            }
            break;
        }
        case 7: { // Save to file
            cout << "List of .txt files in current folder" << endl;
            setColor(2);
            system("dir /b *.txt");
            resetColor();
            filename = getFilename();
            if (filename == "~") continue;
            saveAirportsToFile(head, filename);
            dataModified = false;
            break;
        }
        case 0: { // Exit
            if (dataModified) {
                cout << "Data has been modified. Do you want to save changes to a file? (y/n): ";
                if (getYesNoAnswer() == 'y') {
                    cout << "List of .txt files in current folder" << endl;
                    setColor(2);
                    system("dir /b *.txt");
                    resetColor();
                    filename = getFilename();
                    if (filename == "~") continue;
                    saveAirportsToFile(head, filename);
                }
            }

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
        default: { // Handle any invalid choice
            setColor(12);
            cerr << "ERROR: Invalid menu choice.\n";
            resetColor();
            break;
        }
        }
    }
}
