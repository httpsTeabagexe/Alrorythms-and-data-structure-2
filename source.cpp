
#include "header.h"

// Function to create a new airport node
Airport* createAirport(string codeIATA) {
	// Allocate memory for a new Airport object on the heap
	Airport* newAirport = new Airport;

	// Assign the IATA code to the new airport
	newAirport->codeIATA = codeIATA;

	// Initialize the airlines list to nullptr (no airlines initially)
	newAirport->airlines = nullptr;

	// Initialize the next pointer to nullptr (end of the list)
	newAirport->next = nullptr;

	// Return the pointer to the newly created airport
	return newAirport;
}

// Function to create a new airline node
Airline* createAirline(string name) {
	// Allocate memory for a new Airline object on the heap
	Airline* newAirline = new Airline;

	// Assign the airline name to the new node
	newAirline->name = name;

	// Initialize the next pointer to nullptr (end of the list)
	newAirline->next = nullptr;

	// Return the pointer to the newly created airline node
	return newAirline;
}

// Function to add an airport to the end of the list
void addAirport(Airport*& head, string codeIATA) {
	// Create a new airport node using the given IATA code
	Airport* newAirport = createAirport(codeIATA);

	// Case 1: The list is empty (head is nullptr)
	if (head == nullptr) {
		// The new airport becomes the head of the list
		head = newAirport;
	}
	// Case 2: The list is not empty
	else {
		// Start from the head of the list
		Airport* current = head;

		// Traverse the list until the last node (current->next is nullptr)
		while (current->next != nullptr) {
			current = current->next;
		}

		// Add the new airport to the end of the list
		current->next = newAirport;
	}
}

// Function to add an airline to an airport
void addAirline(Airport* airport, string name) {
	// Check if the airline already exists for this airport
	Airline* current = airport->airlines;
	while (current != nullptr) {
		if (current->name == name) {
			cout << "Airline '" << name << "' already exists for this airport. "
				<< "Do you want to add it again? (y/n): ";
			if (getYesNoAnswer() != 'y') {
				return; // User chose not to add a duplicate
			}
			break; // Exit the loop, we'll add the duplicate
		}
		current = current->next;
	}

	// Create a new airline node
	Airline* newAirline = createAirline(name);

	// Add the new airline to the airport's list
	if (airport->airlines == nullptr) {
		// If the airlines list is empty, set the new airline as the head
		airport->airlines = newAirline;
	}
	else {
		// If the list is not empty, find the last node and append the new airline
		Airline* current = airport->airlines;
		while (current->next != nullptr) {
			current = current->next;
		}
		current->next = newAirline;
	}
}

// Function to delete an airline from an airport
void deleteAirlineFromAirport(Airport* head) {
	// Main loop for deleting airlines
	while (true) {
		// Print the list of airports
		printAirports(head);

		// Get the IATA code of the airport
		string codeIATA = getValidIATACode();

		// Exit the loop if the user enters "~"
		if (codeIATA == "~") break;

		// Find the airport with the specified IATA code
		Airport* airport = findAirportByCode(head, codeIATA);

		// Check if the airport exists
		if (airport != nullptr) {
			// Get the name of the airline to delete
			string airlineName = getValidAirlineName();

			// Exit the loop if the user enters "~"
			if (airlineName == "~") break;

			// Check for multiple occurrences of the airline in the airport's list
			int count = 0;
			Airline* current = airport->airlines;
			while (current != nullptr) {
				if (current->name == airlineName) {
					count++;
				}
				current = current->next;
			}

			// If multiple occurrences are found, ask the user whether to delete all or a specific one
			if (count > 1) {
				cout << "Airline '" << airlineName << "' appears multiple times. "
					<< "Do you want to delete all occurrences [Y] or a specific one [N]?\n>>";

				// Delete all occurrences if the user chooses 'y'
				if (getYesNoAnswer() == 'y') {
					current = airport->airlines;
					Airline* prev = nullptr;
					while (current != nullptr) {
						if (current->name == airlineName) {
							if (prev == nullptr) {
								airport->airlines = current->next;
							}
							else {
								prev->next = current->next;
							}
							Airline* temp = current;
							current = current->next;
							delete temp;
						}
						else {
							prev = current;
							current = current->next;
						}
					}
					// Print a success message
					setColor(2);
					cout << "All occurrences of airline '" << airlineName << "' deleted from airport '" << codeIATA << "'.\n";
					resetColor();
				}
				// Delete a specific occurrence if the user chooses 'n'
				else {
					deleteAirline(airport, airlineName);
					setColor(2);
					cout << "One occurrence of airline '" << airlineName << "' deleted from airport '" << codeIATA << "'.\n";
					resetColor();
				}
			}
			// If only one occurrence is found, delete it directly
			else {
				deleteAirline(airport, airlineName);
				// Print a success message
				setColor(2);
				cout << "Airline '" << airlineName << "' deleted from airport '" << codeIATA << "'.\n";
				resetColor();
			}
		}
		// If the airport is not found, print an error message
		else {
			setColor(12);
			cerr << "ERROR: Airport with this IATA code not found.\n";
			resetColor();
		}
	}
}

// Function to delete an airport by IATA code
void deleteAirport(Airport*& head, string codeIATA) {
	// If the airport list is empty, return
	if (head == nullptr) return;

	// Case 1: Deleting the head node
	if (head->codeIATA == codeIATA) {
		// Store the head node in a temporary variable
		Airport* temp = head;
		// Update the head pointer to the next node
		head = head->next;

		// Delete all airlines associated with the airport
		Airline* currentAirline = temp->airlines;
		while (currentAirline != nullptr) {
			Airline* tempAirline = currentAirline;
			currentAirline = currentAirline->next;
			delete tempAirline;
		}

		// Delete the airport node
		delete temp;
		// Print a success message
		setColor(2);
		cout << "Airport '" << codeIATA << "' deleted successfully.\n";
		resetColor();
		// Return from the function
		return;
	}

	// Case 2: Deleting a node other than the head
	// Start from the node after the head
	Airport* current = head->next;
	// Keep track of the previous node
	Airport* prev = head;

	// Traverse the list until the airport with the matching IATA code is found
	while (current != nullptr && current->codeIATA != codeIATA) {
		prev = current;
		current = current->next;
	}

	// If the airport is found
	if (current != nullptr) {
		// Check if the airport has any airlines associated with it
		if (current->airlines != nullptr) {
			// Ask the user for confirmation before deleting
			cout << "Airport '" << codeIATA << "' has airlines associated with it.\n"
				<< "Are you sure you want to delete it? (y/n): ";
			if (getYesNoAnswer() != 'y') {
				// If the user does not confirm, return from the function
				return;
			}
		}

		// Bypass the current node (delete the airport from the list)
		prev->next = current->next;

		// Delete all airlines associated with the airport
		Airline* currentAirline = current->airlines;
		while (currentAirline != nullptr) {
			Airline* tempAirline = currentAirline;
			currentAirline = currentAirline->next;
			delete tempAirline;
		}

		// Delete the airport node
		delete current;
		// Print a success message
		setColor(2);
		cout << "Airport '" << codeIATA << "' deleted successfully.\n";
		resetColor();
	}
	// If the airport is not found, print an error message
	else {
		setColor(12);
		cerr << "ERROR: Airport '" << codeIATA << "' not found.\n";
		resetColor();
	}
}

// Function to delete an airline from an airport by name
void deleteAirportFromList(Airport*& head, bool& dataModified) {
	//cout << "List of existing airports:" << endl;
	printAirports(head); // Show the list of airports

	if (head == nullptr) {
		return; // Go back to main menu if the list is empty
	}

	string codeIATA = getValidIATACode(); // Use the validation function
	if (codeIATA == "~") return;

	deleteAirport(head, codeIATA);
	dataModified = true;
}

// Function to delete an airline from an airport
bool deleteAirline(Airport* airport, string name) {
	// If the airport has no airlines, return false (airline not found)
	if (airport->airlines == nullptr) return false;

	// Check if the first airline in the list matches the name
	if (airport->airlines->name == name) {
		// Store the first airline node in a temporary variable
		Airline* temp = airport->airlines;
		// Update the head of the airlines list to the next airline
		airport->airlines = airport->airlines->next;
		// Delete the temporary airline node
		delete temp;
		// Return true (airline deleted)
		return true;
	}

	// Initialize pointers for traversing the list
	Airline* current = airport->airlines;
	Airline* prev = nullptr;

	// Traverse the list until the airline with the matching name is found
	while (current != nullptr && current->name != name) {
		// Update the previous pointer to the current node
		prev = current;
		// Move to the next airline in the list
		current = current->next;
	}

	// If the airline is found (current is not nullptr)
	if (current != nullptr) {
		// If a previous node exists (not deleting the first airline)
		if (prev != nullptr) {
			// Bypass the current node, removing it from the list
			prev->next = current->next;
		}
		// Delete the current airline node
		delete current;
		return true; // Airline deleted
	}

	// If the airline is not found, return false
	return false;
}

// Function to print the list of airports and airlines
void printAirports(Airport* head) {
	// If the airport list is empty, print an error message and return
	if (head == nullptr) {
		setColor(12);
		cerr << "ERROR: The list of airports is empty.\n";
		resetColor();
		return; // Nothing to print
	}

	// Print a header for the list of airports
	cout << "List of airports:" << endl;

	// Start from the head of the airport list
	Airport* currentAirport = head;

	// Iterate through each airport in the list
	while (currentAirport != nullptr) {
		// Print the IATA code of the current airport
		cout << currentAirport->codeIATA << endl;

		// Get the list of airlines for the current airport
		Airline* currentAirline = currentAirport->airlines;

		// If the airport has airlines associated with it
		if (currentAirline != nullptr) {
			// Iterate through each airline in the list
			while (currentAirline != nullptr) {
				// Set the text color to light blue
				setColor(11);
				// Print the airline name
				cout << "  " << currentAirline->name << endl;
				// Move to the next airline in the list
				currentAirline = currentAirline->next;
				// Reset the text color to the default
				resetColor();
			}
		}
		// Move to the next airport in the list
		currentAirport = currentAirport->next;
	}
}
// Function to load data from a file
bool loadAirportsFromFile(Airport*& head, string filename) {
	add_txt(filename); // Add .txt extension if needed
	ifstream inFile(filename);
	if (!inFile.is_open()) {
		setColor(12);
		cerr << "ERROR: Unable to open the file '" << filename << "' for reading." << endl;
		resetColor();
		return false;
	}
	// Check if the data structure is already populated
	if (head != nullptr) {
		cout << "The data structure is not empty. "
			<< "Do you want to [Y] - merge data from the file or [N] - save existing data to a different file?\n"
			<< "After saving data to different file, the data will be merged with data from file\n>>";
		char choice = getYesNoAnswer();

		if (choice == 'n') {
			cout << "Enter the filename to save the existing data: ";
			string saveFilename = getFilename();
			if (saveFilename == "~") return false; // Cancel loading
			saveAirportsToFile(head, saveFilename);
			head = nullptr; // Clear the existing data
		}
		// If choice merge the loading will proceed, 
		// effectively merging data from the file into the existing structure.
	}
	string line;
	while (getline(inFile, line)) {
		size_t tabPos = line.find('\t');
		if (tabPos != string::npos) {
			string airlineName = line.substr(0, tabPos);
			string codeIATA = line.substr(tabPos + 1);

			Airport* airport = findAirportByCode(head, codeIATA);
			if (airport == nullptr) {
				airport = createAirport(codeIATA); // Create a new airport
				addAirport(head, codeIATA);       // Add it to the list
			}
			addAirline(airport, airlineName); // Add the airline to the airport
		}
		else {
			setColor(12);
			cerr << "Error: Invalid data format in line: " << line << endl;
			resetColor();
		}
	}
	inFile.close();
	setColor(2);
	cout << "Data successfully loaded from file '" << filename << "'" << endl;
	resetColor();
	return true;
}

// Function to save data to a file
void saveAirportsToFile(Airport* head, string filename) {
	// Ensure the filename has a .txt extension
	add_txt(filename);

	// Check if the airport list is empty
	if (head == nullptr) {
		// If empty, display an error message and return
		setColor(12);
		cerr << "ERROR: The list of airports is empty. Nothing to save.\n";
		resetColor();
		return;
	}

	// Open the output file for writing
	ofstream outFile(filename);

	// Check if the file was opened successfully
	if (!outFile.is_open()) {
		// If not, display an error message and return
		setColor(12);
		cerr << "ERROR: Unable to open the file '" << filename << "' for writing.\n";
		resetColor();
		return;
	}

	// Start from the head of the airport list
	Airport* currentAirport = head;

	// Iterate through each airport in the list
	while (currentAirport != nullptr) {
		// Get the list of airlines for the current airport
		Airline* currentAirline = currentAirport->airlines;

		// Iterate through each airline for the current airport
		while (currentAirline != nullptr) {
			// Write the airline name and airport IATA code to the file
			outFile << currentAirline->name << "\t" << currentAirport->codeIATA << endl;
			// Move to the next airline in the list
			currentAirline = currentAirline->next;
		}
		// Move to the next airport in the list
		currentAirport = currentAirport->next;
	}

	// Close the output file
	outFile.close();

	// Display a success message to the user
	setColor(2);
	cout << "List of airports successfully saved to file '" << filename << "'\n";
	resetColor();
}

// Function to find an airport by IATA code
Airport* findAirportByCode(Airport* head, string codeIATA) {
	// Start from the head of the airport list
	Airport* current = head;

	// Iterate through the list until the end is reached
	while (current != nullptr) {
		// If the current airport's IATA code matches the search code
		if (current->codeIATA == codeIATA) {
			// Return a pointer to the found airport
			return current;
		}
		// Move to the next airport in the list
		current = current->next;
	}

	// If the IATA code is not found, return nullptr
	return nullptr;
}

// Function to display the menu and get user's choice
int menuSelection() {
	while (true) {
		cout << "1. Add airport to the end of the list\n"
			<< "2. Add an airline to an airport\n"
			<< "3. Delete an airport from the list\n"
			<< "4. Delete an airline from an airport\n"
			<< "5. Display the list of airports\n"
			<< "6. Load data from a file\n"
			<< "7. Save data to a file\n"
			<< "0. Exit\n";

		int choice = _getch() - '0'; // Directly convert char to int
		if (choice >= 0 && choice <= 7) {
			cout << choice << endl;
			return choice;
		}
		else {
			setColor(12);
			system("cls");
			cerr << "Invalid choice. Please enter a number between 0 and 7.\n";
			resetColor();
		}
	}
}

string getValidAirlineName() {
	string airlineName;

	while (true) {
		cout << "Enter the airline name (or '~' to return to menu): ";
		getline(cin, airlineName);

		if (airlineName == "~") {
			return "~"; // Signal to return to the main menu
		}

		// Airline name validation (letters, numbers, spaces, and hyphens)
		if (all_of(airlineName.begin(), airlineName.end(), [](char c) {
			return (c >= '\u0410' && c <= '\u044F') || // Cyrillic uppercase
				(c >= '\u0430' && c <= '\u045F') || // Cyrillic lowercase
				isalnum(c) || c == ' ' || c == '-'; // Allow alphanumeric, spaces, hyphens
			})) {
			return airlineName;
		}
		else {
			setColor(12);
			cerr << "ERROR: Invalid airline name. Only letters (including Cyrillic), numbers, spaces, and hyphens (-) are allowed.\n";
			resetColor();
		}
	}
}

string getValidIATACode() {
	string codeIATA;

	while (true) {
		cout << "Enter the airport's IATA code (or '~' to return to menu):\n" << ">>";
		getline(cin, codeIATA); // Read the entire line to handle spaces

		if (codeIATA == "~") {
			return "~"; // Signal to return to the main menu
		}

		// Remove spaces from the input IATA code
		codeIATA.erase(remove(codeIATA.begin(), codeIATA.end(), ' '), codeIATA.end());

		// IATA code validation (3 uppercase letters)
		if (codeIATA.length() == 3 && all_of(codeIATA.begin(), codeIATA.end(), ::isupper)) {
			return codeIATA; // Valid IATA code
		}
		else {
			setColor(12);
			cerr << "ERROR: Invalid IATA code. It must be 3 uppercase letters.\n";
			resetColor();
		}
	}
}

char getYesNoAnswer() {
	char choice;
	while (true) {
		choice = _getch();
		cout << choice << endl; // Echo the input
		choice = tolower(choice);

		if (choice == 'y' || choice == 'n') {
			return choice;  // Valid answer, return it
		}
		else {
			setColor(12);
			cerr << "Invalid choice. Please enter 'y' or 'n': ";
			resetColor();
		}
	}
}

// Function to get a filename from the user
string getFilename() {
	string filename;
	cout << "Enter name of the file (or '~' to return to menu)\n";
	while (true) {
		cout << ">>";
		getline(cin, filename);
		if (filename == "~") {
			return "~";
		}
		if (filename.empty()) {
			setColor(12);
			cerr << "ERROR: File name cannot be empty.\n";
			resetColor();
			continue;
		}
		if (hasWindowsSpecChar(filename)) {
			continue;
		}
		if (isReservedName(filename)) {
			continue;
		}
		break;
	}
	return filename;
}

// Function to check if the file exists
bool check_file_exists(string filename) {
	ifstream file(filename);
	bool exists = file.good();
	file.close();
	return exists;
}

// Function to check for prohibited characters in the filename
bool hasWindowsSpecChar(string filename) {
	// List of characters not allowed in Windows filenames
	list<char> prohibited_chars = { '<', '>', ':', '\"', '/', '\\', '|', '?', '*' };
	for (char c : filename) {
		if (find(prohibited_chars.begin(), prohibited_chars.end(), c) != prohibited_chars.end()) {
			setColor(12);
			cerr << "ERROR: Name of the file contains prohibited characters.\n";
			resetColor();
			return true;
		}
	}
	return false;
}

// Function to check if the filename is a reserved name
bool isReservedName(string name) {
	// Check if the name matches any of the reserved names
	static const string reserved_names[] = { "CON","PRN","AUX","NUL",
	"COM1","COM2","COM3","COM4","COM5","COM6","COM7","COM8","COM9","LPT1","LPT2","LPT3","LPT4","LPT5","LPT6","LPT7","LPT8","LPT9" };
	for (const auto& reserved : reserved_names) {
		if (name == reserved) {
			setColor(12);
			cerr << "ERROR: It is a reserved name.\n";
			resetColor();
			return true; // Name matches a reserved name
		}
	}
	return false; // Name is not reserved
}

bool txt_check(string filename) {
	// Check if the filename ends with ".cty" (case insensitive)
	if (filename.length() >= 4 && equal(filename.end() - 4, filename.end(), ".txt", [](char a, char b) {
		return tolower(a) == b;
		})) {
		setColor(2); cout << "File extension is valid." << endl; resetColor();
		system("pause"); system("cls");
		return true;
	}
	else { return false; }
}

void add_txt(string& filename) {
	if (!txt_check(filename)) {
		char choice;
		while (true) {
			cout << "The filename doesn't have .txt extension. Do you want to add it? (y/n): ";
			choice = _getch();
			cout << choice << endl;
			choice = tolower(choice);
			if (choice == 'y') {
				filename += ".txt"; // Add .txt extension to the filename
				return; // Return after modifying the filename
			}
			if (choice == 'n') {
				cout << "\nFile wasn't modified." << endl;
				return; // Return without modifying the filename
			}
		}
	}
}

void setColor(int colorCode) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, colorCode);
}
void resetColor() {
	setColor(7); // 7 is the default color (white on black)
}