#include "hash_table.hpp"
#include "hash_table_test.hpp"
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <time.h>

using namespace std;
using namespace hash_table;

vector<wstring> first_names = {L"James", L"John", L"Robert", L"Michael", L"William", L"David", L"Richard", L"Joseph", L"Thomas", L"Charles", L"Christopher", L"Daniel", L"Matthew", L"Anthony", L"Donald", L"Mark", L"Paul", L"Steven", L"Andrew", L"Kenneth", L"Joshua", L"George", L"Kevin", L"Brian", L"Edward", L"Mary", L"Patricia", L"Jenifer", L"Linda", L"Elizabeth", L"Barbra", L"Susan", L"Jessica", L"Sarah", L"Karen", L"Margaret", L"Lisa", L"Betty", L"Dorothy", L"Sandra", L"Ashley", L"Kimberly", L"Donna", L"Emily", L"Michelle", L"Carol", L"Amanda", L"Melissa", L"Deborah"};
vector<wstring> last_names = {L"Smith", L"Johnson", L"Williams", L"Jones", L"Brown", L"Davis", L"Miller", L"Moore", L"Taylor", L"Hall", L"Allen", L"Young", L"Hernandez", L"Kind", L"Wright", L"Lopez", L"Hill", L"Scott", L"Green", L"Stewart", L"Sanchez", L"Morris", L"Rogers", L"Reed"};

//typedef for the hashing function
typedef unsigned long(*hash_function)(string, unsigned long);

//forward declares
wstring random_name();
wstring random_number();
void main_menu();
void hash_table_playground();
void testing_menu();

//custom string hashing function
unsigned long hash_string(wstring key, unsigned long size) {
	unsigned long hash = 33;

	for (int i = 0; i < key.length(); i++) {
		hash = 33 * hash + (unsigned int)key[i];
	}

	return hash % size;
}

int main() {
	//enable unicode in wcout
	_setmode(_fileno(stdout), _O_U16TEXT);

	//launch main menu
	main_menu();
}

//main menu
void main_menu() {
main_menu:

	wcout << "<> Main Menu <>\n\n";
	wcout << "[1] Hash Table Playground\n";
	wcout << "[2] Testing Menu\n";
	wcout << "[3] Exit\n\n";
	wcout << "Please enter your selection: ";

	int choice;
	cin >> choice;

	switch (choice) {
	case 1:
		system("cls");
		hash_table_playground();
		goto main_menu;
	case 2:
		system("cls");
		testing_menu();
		goto main_menu;
	case 3:
		return;
	default:
		system("cls");
		wcout << "INVALID OPTION: Please select one of the valid options below (1-3).\n\n";
		goto main_menu;
	}
}

void hash_table_playground() {
	srand(time(nullptr));

	//create the hash table, given a hashing function and starting size of 100
	HashTable<wstring, wstring> table(hash_string, 100);

	//add some values to the table
	for (int i = 0; i < 10; i++) {
		table.insert(random_name(), random_number());
	}

	//user response from cin
	wstring response = L"";

	//interface while loop
	while (response != L"5") {
		//clear screen
		system("cls");

		//print the hash table to the screen
		table.print(wcout, 20, 25);

		//main menu
		wcout << endl << "<> Hash Table Editor <>" << endl;
		wcout << " [1] Insert a new value with a key." << endl;
		wcout << " [2] Remove a value by key." << endl;
		wcout << " [3] Search a value by key." << endl;
		wcout << " [4] Add bulk random values." << endl;
		wcout << " [5] Exit program." << endl;
		wcout << endl << "Please choose from one of the above options:" << endl;
		wcout << "> "; getline(wcin, response);

		//insert a new value
		if (response == L"1") {
			system("cls");

			wstring key, value;

			//get the key
			wcout << "Please enter the key for the new entry:" << endl << "> ";
			getline(wcin, response); wcout << endl << endl; key = response;

			//get the value
			wcout << "Please enter the value to be inserted:" << endl;
			getline(wcin, response); wcout << endl << endl; value = response;

			//insert the value
			table.insert(key, value);

			wcout << "Value inserted." << endl;
			system("pause");
		}
		else if (response == L"2") {
			system("cls");

			//print table for refference
			table.print(wcout, 20, 25);

			wstring key;

			//get the key
			wcout << "Please enter the key of the entry to be removed:" << endl << "> ";
			getline(wcin, response); wcout << endl << endl; key = response;

			//remove value
			if (table.remove(key)) {
				wcout << "Entry with key '" << key << "' was removed." << endl;
			}
			else {
				wcout << "No entry with key '" << key << "' exists in the table." << endl;
			}

			system("pause");
		}
		else if (response == L"3") {
			system("cls");

			//print table for refference
			table.print(wcout, 20, 25);

			wstring key;

			//get the key
			wcout << "Please enter the key of the entry to be searched:" << endl << "> ";
			getline(wcin, response); wcout << endl << endl; key = response;

			wstring* val = table.get(key);
			//check if search was sucessful
			if (val != nullptr) {
				wcout << "Entry with key '" << key << "' has a value of '" << *val << "'." << endl;
			}
			else {
				wcout << "No entry with key '" << key << "' exists in the table." << endl;
			}

			system("pause");
		}
		else if (response == L"4") {
			system("cls");

			wstring number;

			//get the key
			wcout << "Please enter the number of random entrys you would like to add (int only):" << endl << "> ";
			getline(wcin, response); wcout << endl << endl; number = response;

			//try to add random values, output error message if input is invalid
			try {
				int x = stoi(number);

				//add x number of random values
				for (int i = 0; i < x; i++) {
					table.insert(random_name(), random_number());
				}

				wcout << "Added " << x << " new entries successfuly!" << endl;
			}
			catch (...) {
				wcout << "Value given could not be converted to an integer." << endl;
			}

			system("pause");
		}
	}
}

void testing_menu() {
testing_menu:
	wcout << "<> Testing Menu <>\n\n";
	wcout << "[1] Hash Table Functionality Test\n";
	wcout << "[2] Hash Table Performance Test\n";
	wcout << "[3] Exit\n\n";
	wcout << "Please enter which test you would like to run: ";

	int choice;
	cin >> choice;

	system("cls");

	switch (choice) {
	case 1:
		hash_table_testing::test_functionality();
		system("pause");
		system("cls");
		goto testing_menu;
	case 2:
		hash_table_testing::test_performance();
		system("pause");
		system("cls");
		goto testing_menu;
	case 3:
		return;
	default:
		wcout << "INVALID OPTION: Please select one of the valid options below (1-3).\n\n";
		goto testing_menu;
	}
}

//generates a random name from the first and last name lists
wstring random_name() {
	wstring name = L"";

	//add the first name
	name += first_names[rand() % first_names.size()] + L" ";

	//add the last name
	name += last_names[rand() % last_names.size()];

	return name;
}

//returns a random int from 0-9 as a string
wstring rand_int() {
	return to_wstring(rand() % 10);
}

//generates a random phone number
wstring random_number() {
	return (rand_int() + rand_int() + rand_int() + L"-" + rand_int() + rand_int() + rand_int() + L"-" + rand_int() + rand_int() + rand_int() + rand_int());
}