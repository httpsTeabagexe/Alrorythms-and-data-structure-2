#pragma once
#include <string>
#include <algorithm>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <windows.h>
#include <list>
#include <algorithm>
#include <vector>
using namespace std;

// Структура для хранения информации об авиакомпании
struct Airline {
    string name;                  // Название авиакомпании
    Airline* next = nullptr;      // Указатель на следующую авиакомпанию в списке
};

// Структура для хранения информации об аэропорте
struct Airport {
    string codeIATA;             // Код IATA аэропорта
    Airline* airlines = nullptr; // Указатель на список авиакомпаний, обслуживающих этот аэропорт
    Airport* next = nullptr;      // Указатель на следующий аэропорт в списке
};

// Функция для создания нового узла аэропорта
Airport* createAirport(string codeIATA);

// Функция для создания нового узла авиакомпании
Airline* createAirline(string name);

// Функция для добавления аэропорта в конец списка
void addAirport(Airport*& head, string codeIATA);

// Функция для добавления авиакомпании к аэропорту
void addAirline(Airport* airport, string name);

// Функция для удаления авиакомпании из аэропорта
void deleteAirlineFromAirport(Airport* head);

// Функция для удаления авиакомпании по индексу
void deleteAirlineByIndex(Airport* airport, string& name, int index);

// Функция для удаления аэропорта из списка
void deleteAirportFromList(Airport*& head, bool& dataModified);

// Функция для удаления аэропорта по коду IATA
void deleteAirport(Airport*& head, string codeIATA);

// Функция для удаления авиакомпании из аэропорта
bool deleteAirline(Airport* airport, string name);

// Функция для вывода списка аэропортов и авиакомпаний
void printAirports(Airport* head);

// Функция для загрузки данных из файла
bool loadAirportsFromFile(Airport*& head, string filename);

// Функция для поиска авиакомпании в аэропортах
void searchAirlineInAirports(Airport* head, string airlineName);

// Функция для сортировки списка аэропортов по коду IATA
void sortAirports(Airport*& head);

// Функция для сохранения данных в файл
void saveAirportsToFile(Airport* head, string filename);

// Функция для поиска аэропорта по коду IATA
Airport* findAirportByCode(Airport* head, string codeIATA);

// Функция для получения от пользователя корректного кода IATA (3 заглавные буквы)
string getValidIATACode();

// Функция для получения от пользователя корректного названия авиакомпании 
// (допустимы буквы, цифры, пробелы и дефисы)
string getValidAirlineName();

// Функция для вывода меню и получения выбора пользователя
int menuSelection();

// Функция для получения от пользователя ответа "y" или "n" (да/нет)
char getYesNoAnswer();

// Функция для получения от пользователя имени файла с проверкой корректности
string getFilename();

// Функция для проверки существования файла
bool check_file_exists(string filename);

// Функция для проверки, содержит ли имя файла недопустимые символы для Windows
bool hasWindowsSpecChar(string filename);

// Функция для проверки, является ли имя файла зарезервированным именем в Windows
bool isReservedName(string name);

// Функция для проверки, имеет ли имя файла расширение ".txt"
bool txt_check(string filename);

// Функция для добавления расширения ".txt" к имени файла, если его нет
void add_txt(string& filename);

// Функция для установки цвета текста в консоли
void setColor(int colorCode);

// Функция для сброса цвета текста в консоли до значения по умолчанию
void resetColor();