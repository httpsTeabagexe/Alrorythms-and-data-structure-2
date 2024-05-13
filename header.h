#pragma once
#include <conio.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// Структура для хранения информации об авиакомпании
struct Airline {
	string name;
	Airline* next = nullptr;
};

// Структура для хранения информации об аэропорте
struct Airport {
	string codeIATA;
	Airline* airlines = nullptr;  // Список авиакомпаний, обслуживающих этот аэропорт
	Airport* next = nullptr;
};

// Функция для создания нового узла аэропорта
Airport* createAirport(string codeIATA);

// Функция для создания нового узла авиакомпании
Airline* createAirline(string name);

// Функция для добавления аэропорта в конец списка
void addAirport(Airport*& head, string codeIATA);

// Функция для добавления авиакомпании к аэропорту
void addAirline(Airport* airport, string name);

// Функция для удаления аэропорта по коду IATA
void deleteAirport(Airport*& head, string codeIATA);

// Функция для удаления авиакомпании из аэропорта
void deleteAirline(Airport* airport, string name);

// Функция для вывода списка аэропортов и авиакомпаний
void printAirports(Airport* head);

// Функция для загрузки данных из файла
bool loadAirportsFromFile(Airport*& head, string filename);

// Функция для сохранения данных в файл
void saveAirportsToFile(Airport* head, string filename);

// Функция для поиска аэропорта по коду IATA
Airport* findAirportByCode(Airport* head, string codeIATA);

// Функция для вывода меню и получения выбора пользователя
int menuSelection();