#pragma once
#include <conio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include <list>
#include <algorithm>
using namespace std;

// ��������� ��� �������� ���������� �� ������������
struct Airline {
    string name;                  // �������� ������������
    Airline* next = nullptr;      // ��������� �� ��������� ������������ � ������
};

// ��������� ��� �������� ���������� �� ���������
struct Airport {
    string codeIATA;             // ��� IATA ���������
    Airline* airlines = nullptr; // ��������� �� ������ ������������, ������������� ���� ��������
    Airport* next = nullptr;      // ��������� �� ��������� �������� � ������
};

// ������� ��� �������� ������ ���� ���������
Airport* createAirport(string codeIATA);

// ������� ��� �������� ������ ���� ������������
Airline* createAirline(string name);

// ������� ��� ���������� ��������� � ����� ������
void addAirport(Airport*& head, string codeIATA);

// ������� ��� ���������� ������������ � ���������
void addAirline(Airport* airport, string name);

// ������� ��� �������� ������������ �� ���������
void deleteAirlineFromAirport(Airport* head);

// ������� ��� �������� ��������� �� ������
void deleteAirportFromList(Airport*& head, bool& dataModified);

// ������� ��� �������� ��������� �� ���� IATA
void deleteAirport(Airport*& head, string codeIATA);

// ������� ��� �������� ������������ �� ���������
bool deleteAirline(Airport* airport, string name);

// ������� ��� ������ ������ ���������� � ������������
void printAirports(Airport* head);

// ������� ��� �������� ������ �� �����
bool loadAirportsFromFile(Airport*& head, string filename);

// ������� ��� ���������� ������ � ����
void saveAirportsToFile(Airport* head, string filename);

// ������� ��� ������ ��������� �� ���� IATA
Airport* findAirportByCode(Airport* head, string codeIATA);

// ������� ��� ��������� �� ������������ ����������� ���� IATA (3 ��������� �����)
string getValidIATACode();

// ������� ��� ��������� �� ������������ ����������� �������� ������������ 
// (��������� �����, �����, ������� � ������)
string getValidAirlineName();

// ������� ��� ������ ���� � ��������� ������ ������������
int menuSelection();

// ������� ��� ��������� �� ������������ ������ "y" ��� "n" (��/���)
char getYesNoAnswer();

// ������� ��� ��������� �� ������������ ����� ����� � ��������� ������������
string getFilename();

// ������� ��� �������� ������������� �����
bool check_file_exists(string filename);

// ������� ��� ��������, �������� �� ��� ����� ������������ ������� ��� Windows
bool hasWindowsSpecChar(string filename);

// ������� ��� ��������, �������� �� ��� ����� ����������������� ������ � Windows
bool isReservedName(string name);

// ������� ��� ��������, ����� �� ��� ����� ���������� ".txt"
bool txt_check(string filename);

// ������� ��� ���������� ���������� ".txt" � ����� �����, ���� ��� ���
void add_txt(string& filename);

// ������� ��� ��������� ����� ������ � �������
void setColor(int colorCode);

// ������� ��� ������ ����� ������ � ������� �� �������� �� ���������
void resetColor();