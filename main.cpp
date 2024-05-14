#include "header.h"
#include <consoleapi2.h>
#include <clocale>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
int main() {
	////todo дублирование в авиакомпаниях
	////показывать данные при вводе в аэропорт
	////работа с дубликатами авиакомпаний
	////удаление нескольких авиакомпаний за один раз
	////запретить запись пустой структуры в файл
	////добавить возможность удалить все дубликаты в одном аэропорте
	////loadAirportsFromFile после мерджа данных модификатор "модифайд" поменять на тру и спрашивать юзера схоранить ли данные
	////сообщение когда ничего не удалено в удалении авиалиний
	////удаление пока не будет введен спецсимвол ~ в 3 и 4 пункте меню

	//спрашивать пользователя о сохранении данных в уже существующий файл, и 
	//сортировка внутри выходных файлов???
	//поиск по имени авиалинии в каком аэропорту она содержится
	//сортировка аэропортов

	//!!ФИКС не загружаются данные в структуру из файла, а именно авиакомпании


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
			while (true) {
				cout << "List of existing airports:" << endl;
				printAirports(head); // Show the list of airports

				if (head == nullptr) {
					continue; // Go back to main menu if the list is empty
				}

				string codeIATA = getValidIATACode();
				if (codeIATA == "~") break; // Return to main menu

				deleteAirport(head, codeIATA);
				dataModified = true;
			}
			break;
		}
		case 4: {
			deleteAirlineFromAirport(head);
			dataModified = true;
			break;
		}
		case 5: // Print airports
			system("cls");
			printAirports(head);
			system("pause");
			break; // dataModified doesn't need to be changed here

		case 6: { // Load from file
			system("cls");
			cout << "List of .txt files in current folder" << endl;
			setColor(2);
			system("dir /b *.txt");
			resetColor();
			filename = getFilename();
			if (filename == "~") continue;
			if (loadAirportsFromFile(head, filename)) {
				// Check if merging happened (head was not nullptr before loading)
				if (head != nullptr) {
					dataModified = true; // Set dataModified to true only after merging
				}
			}
			system("pause");
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