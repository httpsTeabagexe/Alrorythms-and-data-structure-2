#include "header.h"
#include <consoleapi2.h>
#include <clocale>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
int main() {
	//todo дублирование в авиакомпаниях
	//показывать данные при вводе в аэропорт
	//работа с дубликатами авиакомпаний
	//удаление нескольких авиакомпаний за один раз
	//сортировка аэропортов
	//поиск по имени авиалинии в каком аэропорту она содержится
	setlocale(LC_ALL, "RUSSIAN");
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
			cout << "List of existing airports:" << endl;
			printAirports(head);

			while (true) { // Loop for entering IATA code
				string codeIATA = getValidIATACode();
				if (codeIATA == "~") break; // Exit to main menu

				Airport* airport = findAirportByCode(head, codeIATA);

				if (airport != nullptr) { // Airport found
					while (true) { // Loop for entering airlines
						string airlineName = getValidAirlineName();
						if (airlineName == "~") break; // Exit to main menu

						addAirline(airport, airlineName);
						dataModified = true;
					}
					break; // Exit the IATA code loop
				}
				else {
					setColor(12);
					cerr << "ERROR: Airport with this IATA code not found.\n";
					resetColor();
					// The loop will continue to ask for a valid IATA code
				}
			}
			break;
		}
		case 3: { // Delete airport
			deleteAirportFromList(head, dataModified);
			break;
		}
		case 4: { // Delete airline
			deleteAirlineFromAirport(head, dataModified);
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
	SetConsoleCP(886);
	SetConsoleOutputCP(886);
}