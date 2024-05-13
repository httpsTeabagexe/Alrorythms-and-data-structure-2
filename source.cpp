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
void deleteAirport(Airport*& head, string codeIATA) {
    if (head == nullptr) return;

    // Case 1: Deleting the head node
    if (head->codeIATA == codeIATA) {
        Airport* temp = head;
        head = head->next; // Update head to the next node

        // Delete all airlines associated with this airport
        Airline* currentAirline = temp->airlines;
        while (currentAirline != nullptr) {
            Airline* tempAirline = currentAirline;
            currentAirline = currentAirline->next;
            delete tempAirline;
        }
        delete temp; // Delete the airport
        return;
    }

    // Case 2: Deleting a node other than the head
    Airport* current = head->next; // Start from the node after head
    Airport* prev = head;         // Keep track of the previous node

    while (current != nullptr && current->codeIATA != codeIATA) {
        prev = current;
        current = current->next;
    }

    if (current != nullptr) { // Found the airport to delete
		// Check if the airport has any airlines associated with it
        if (current->airlines != nullptr) {
            cout << "Airport '" << codeIATA << "' has airlines associated with it.\n" << "Are you sure you want to delete it? (y/n): ";
            if (getYesNoAnswer() != 'y') {
                return; // User chose not to delete
            }
        }

        // Proceed with deletion
        prev->next = current->next; // Bypass the current node

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

// Function to delete an airline from an airport
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
        setColor(12);
        cerr << "ERROR: The list of airports is empty.\n";
        resetColor();
        return; // Nothing to print
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
    add_txt(filename); // Add .txt extension if needed
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        setColor(12);
        cerr << "ERROR: Unable to open the file '" << filename << "' for reading." << endl;
        resetColor();
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
    add_txt(filename); // Add .txt extension if needed
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        setColor(12);
        cerr << "ERROR: Unable to open the file '" << filename << "' for writing." << endl;
        resetColor();
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
    setColor(2);
    cout << "List of airports successfully saved to file '" << filename << "'" << endl;
    resetColor();
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
			setColor(12);
            cerr << "\nInvalid choice. Please enter a number between 0 and 7.\n";
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
        if (codeIATA.length() == 3 &&
            all_of(codeIATA.begin(), codeIATA.end(), ::isupper)) {
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