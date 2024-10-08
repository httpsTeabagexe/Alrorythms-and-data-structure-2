
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
			setColor(12);
			cout << "Airline '" << name << "' already exists for this airport. "
				<< "Do you want to add it again? (y/n): ";
			resetColor();
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
	while (true) {
		printAirports(head);

		string codeIATA = getValidIATACode();
		if (codeIATA == "~") break;

		Airport* airport = findAirportByCode(head, codeIATA);
		if (airport != nullptr) {
			string airlineName = getValidAirlineName();
			if (airlineName == "~") break;

			// Find and display duplicate airlines with their indices
			vector<int> duplicateIndices;
			int index = 1;
			Airline* current = airport->airlines;
			while (current != nullptr) {
				if (current->name == airlineName) {
					duplicateIndices.push_back(index);
				}
				current = current->next;
				index++;
			}

			if (duplicateIndices.size() > 1) {
				cout << "Airline '" << airlineName << "' has multiple entries:\n";
				for (size_t i = 0; i < duplicateIndices.size(); ++i) {
					cout << i + 1 << ". Entry " << duplicateIndices[i] << endl;
				}

				cout << "Enter the index of the entry to delete (or 0 to delete the first occurrence): ";
				int entryToDelete;
				cin >> entryToDelete;
				system("pause");
				if (entryToDelete == 0) {
					// Delete one occurrence
					deleteAirline(airport, airlineName);
					setColor(2);
					cout << "One occurrence of airline '" << airlineName << "' deleted from airport '" << codeIATA << "'.\n";
					resetColor();
				}
				else if (entryToDelete > 0 && entryToDelete <= static_cast<int>(duplicateIndices.size())) {
					// Delete by index
					int targetIndex = duplicateIndices[entryToDelete - 1];
					deleteAirlineByIndex(airport, airlineName, targetIndex);
					setColor(2);
					cout << "Entry " << targetIndex << " of airline '" << airlineName << "' deleted from airport '" << codeIATA << "'.\n";
					resetColor();
				}
				else {
					setColor(12);
					cerr << "ERROR: Invalid index.\n";
					resetColor();
				}
			}
			else {
				// If only one occurrence is found, delete it directly
				if (deleteAirline(airport, airlineName)) {
					setColor(2);
					cout << "Airline '" << airlineName << "' deleted from airport '" << codeIATA << "'.\n";
					resetColor();
				}
				else {
					// Airline not found, but we don't print a message in this case 
				}
			}

		}
		else {
			setColor(12);
			cerr << "ERROR: Airport with this IATA code not found.\n";
			resetColor();
		}
	}
}

// Function to delete an airline from an airport by index
void deleteAirlineByIndex(Airport* airport, string& name, int index) {
	if (airport->airlines == nullptr) return;

	Airline* current = airport->airlines;
	Airline* prev = nullptr;
	int currentIndex = 1;

	while (current != nullptr) {
		if (current->name == name && currentIndex == index) {
			if (prev == nullptr) {
				airport->airlines = current->next;
			}
			else {
				prev->next = current->next;
			}
			delete current;
			return;
		}
		prev = current;
		current = current->next;
		currentIndex++;
	}
}
// Function to delete an airport by IATA code
void deleteAirport(Airport*& head, string codeIATA) {
	// If the airport list is empty, return
	if (head == nullptr) return;

	// Case 1: Deleting the head node
	if (head->codeIATA == codeIATA) {
		// Check if the airport has any airlines associated with it
		if (head->airlines != nullptr) {
			setColor(12);
			cout << "Airport '" << codeIATA << "' has airlines associated with it.\n"
				<< "Deleting the airport will also delete all its airlines. " // Clarified message
				<< "Are you sure you want to delete it? (y/n): ";
			resetColor();
			if (getYesNoAnswer() != 'y') {
				return; // User chose not to delete
			}
		}

		Airport* temp = head;
		head = head->next;

		// Delete all airlines associated with the airport
		Airline* currentAirline = temp->airlines;
		while (currentAirline != nullptr) {
			Airline* tempAirline = currentAirline;
			currentAirline = currentAirline->next;
			delete tempAirline;
		}

		delete temp;
		setColor(2);
		cout << "Airport '" << codeIATA << "' deleted successfully.\n";
		resetColor();
		return;
	}

	// Case 2: Deleting a node other than the head
	Airport* current = head->next;
	Airport* prev = head;

	while (current != nullptr && current->codeIATA != codeIATA) {
		prev = current;
		current = current->next;
	}

	// If the airport is found
	if (current != nullptr) {
		// Check if the airport has any airlines associated with it
		if (current->airlines != nullptr) {
			cout << "Airport '" << codeIATA << "' has airlines associated with it.\n"
				<< "Deleting the airport will also delete all its airlines. " // Clarified message
				<< "Are you sure you want to delete it? (y/n): ";
			if (getYesNoAnswer() != 'y') {
				return; // User chose not to delete
			}
		}

		prev->next = current->next;

		// Delete all airlines associated with the airport
		Airline* currentAirline = current->airlines;
		while (currentAirline != nullptr) {
			Airline* tempAirline = currentAirline;
			currentAirline = currentAirline->next;
			delete tempAirline;
		}
		delete current;
		setColor(2);
		cout << "Airport '" << codeIATA << "' deleted successfully.\n";
		resetColor();
		// --- Check if Deleting the Last Airport ---
		if (prev->next == nullptr) { // Check if prev->next is null (last node)
			prev->next = nullptr;   // Set prev->next to nullptr (end of list)
			if (prev == head) {       // If prev is the head, the list is now empty
				head = nullptr;
			}
		}
	}
	else { // Airport not found 
		setColor(12);
		cerr << "ERROR: Airport '" << codeIATA << "' not found.\n";
		resetColor();
	}
}
// Function to delete an airline from an airport by name
void deleteAirportFromList(Airport*& head, bool& dataModified) {
	printAirports(head); // Show the list of airports

	if (head == nullptr) {
		return; // Go back to main menu if the list is empty
	}

	string codeIATA = getValidIATACode(); // Use the validation function
	if (codeIATA == "~") return;

	deleteAirport(head, codeIATA);
	dataModified = true;

	// Check if the list is empty after deletion
	if (head == nullptr) {
		return; // Return to the main menu if the list is now empty
	}
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

void searchAirlineInAirports(Airport* head, string airlineName) {
	if (head == nullptr) {
		setColor(12);
		cout << "The list of airports and airlines is empty.\n";
		resetColor();
		return;
	}

	bool found = false;
	Airport* currentAirport = head;

	cout << "Searching for airline '" << airlineName << "'...\n";
	cout << "----------------------------------------\n";

	while (currentAirport != nullptr) {
		Airline* currentAirline = currentAirport->airlines;
		int count = 0;

		while (currentAirline != nullptr) {
			if (currentAirline->name == airlineName) {
				count++;
			}
			currentAirline = currentAirline->next;
		}

		if (count > 0) {
			cout << "Airport: " << currentAirport->codeIATA << "\n";
			cout << "  Airline '" << airlineName << "' found " << count << " time(s).\n";
			cout << "----------------------------------------\n";
			found = true;
		}

		currentAirport = currentAirport->next;
	}

	if (!found) {
		setColor(12);
		cout << "Airline '" << airlineName << "' not found in any airport.\n";
		resetColor();
	}
}




// Function to sort airports alphabetically by IATA code
void sortAirports(Airport*& head) {
	if (head == nullptr) return;

	bool swapped;
	Airport* ptr1;
	Airport* lptr = nullptr;

	do {
		swapped = false;
		ptr1 = head;

		while (ptr1->next != lptr) {
			if (ptr1->codeIATA > ptr1->next->codeIATA) {
				// Swap the data
				swap(ptr1->codeIATA, ptr1->next->codeIATA);
				swap(ptr1->airlines, ptr1->next->airlines);
				swapped = true;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
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
			<< "5. Display the list of airports (sorted alphabetically)\n"
			<< "6. Load data from a file\n"
			<< "7. Save data to a file\n"
			<< "8. Search for airline\n"
			<< "0. Exit\n";

		int choice = _getch() - '0'; // Directly convert char to int
		if (choice >= 0 && choice <= 7) {
			cout << choice << endl;
		}
			return choice;
		//else {
		//	setColor(12);
		//	system("cls");
		//	cerr << "Invalid choice. Please enter a number between 0 and 7.\n";
		//	resetColor();
		//}
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