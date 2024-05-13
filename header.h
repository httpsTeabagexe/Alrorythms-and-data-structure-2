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
	string name;
	Airline* next = nullptr;
};

// ��������� ��� �������� ���������� �� ���������
struct Airport {
	string codeIATA;
	Airline* airlines = nullptr;  // ������ ������������, ������������� ���� ��������
	Airport* next = nullptr;
};

// ������� ��� �������� ������ ���� ���������
Airport* createAirport(string codeIATA);

// ������� ��� �������� ������ ���� ������������
Airline* createAirline(string name);

// ������� ��� ���������� ��������� � ����� ������
void addAirport(Airport*& head, string codeIATA);
//void addAirline(Airport* airport, string name);

// ������� ��� ���������� ������������ � ���������
void addAirline(Airport* airport, string name);

// ������� ��� �������� ��������� �� ���� IATA
void deleteAirport(Airport*& head, string codeIATA);

// ������� ��� �������� ������������ �� ���������
void deleteAirline(Airport* airport, string name);

// ������� ��� ������ ������ ���������� � ������������
void printAirports(Airport* head);

// ������� ��� �������� ������ �� �����
bool loadAirportsFromFile(Airport*& head, string filename);

// ������� ��� ���������� ������ � ����
void saveAirportsToFile(Airport* head, string filename);

// ������� ��� ������ ��������� �� ���� IATA
Airport* findAirportByCode(Airport* head, string codeIATA);

string getValidIATACode();

string getValidAirlineName();

// ������� ��� ������ ���� � ��������� ������ ������������
int menuSelection();

char getYesNoAnswer();

string getFilename();

bool check_file_exists(string filename);

bool hasWindowsSpecChar(string filename);

bool isReservedName(string name);

bool txt_check(string filename);

void add_txt(string& filename);

void setColor(int colorCode);

void resetColor();