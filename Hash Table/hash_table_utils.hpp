#include <iostream>
#include "hash_table.hpp"
#include "unit_testing.hpp"

namespace hash_table_utils {
	using namespace std;
	using namespace hash_table;
	using namespace unit_testing;

	vector<wstring> first_names = { L"James", L"John", L"Robert", L"Michael", L"William", L"David", L"Richard", L"Joseph", L"Thomas", L"Charles", L"Christopher", L"Daniel", L"Matthew", L"Anthony", L"Donald", L"Mark", L"Paul", L"Steven", L"Andrew", L"Kenneth", L"Joshua", L"George", L"Kevin", L"Brian", L"Edward", L"Mary", L"Patricia", L"Jenifer", L"Linda", L"Elizabeth", L"Barbra", L"Susan", L"Jessica", L"Sarah", L"Karen", L"Margaret", L"Lisa", L"Betty", L"Dorothy", L"Sandra", L"Ashley", L"Kimberly", L"Donna", L"Emily", L"Michelle", L"Carol", L"Amanda", L"Melissa", L"Deborah" };
	vector<wstring> last_names = { L"Smith", L"Johnson", L"Williams", L"Jones", L"Brown", L"Davis", L"Miller", L"Moore", L"Taylor", L"Hall", L"Allen", L"Young", L"Hernandez", L"Kind", L"Wright", L"Lopez", L"Hill", L"Scott", L"Green", L"Stewart", L"Sanchez", L"Morris", L"Rogers", L"Reed" };

	//wstring hashing function
	inline unsigned long string_hash_function(wstring key, unsigned long size) {
		unsigned long hash = 33;

		for (int i = 0; i < key.length(); i++) {
			hash = 33 * hash + (unsigned int)key[i];
		}

		return hash % size;
	}

	//generates a random name from the first and last name lists
	inline wstring random_name() {
		wstring name = L"";

		//add the first name
		name += first_names[rand() % first_names.size()] + L" ";

		//add the last name
		name += last_names[rand() % last_names.size()];

		return name;
	}

	//returns a random int from 0-9 as a string
	inline wstring rand_int_str() {
		return to_wstring(rand() % 10);
	}

	//generates a random phone number
	inline wstring random_phone_number() {
		return (rand_int_str() + rand_int_str() + rand_int_str() + L"-" + rand_int_str() + rand_int_str() + rand_int_str() + L"-" + rand_int_str() + rand_int_str() + rand_int_str() + rand_int_str());
	}

	//generate some number of random name/phone number pairs
	inline tuple<wstring, wstring>* gen_dataset(int count = 100) {
		tuple<wstring, wstring>* dataset = new tuple<wstring, wstring>[count];

		for (int i = 0; i < count; i++) {
			dataset[i] = tuple<wstring, wstring>(random_name(), random_phone_number());
		}

		return dataset;
	}

	//add a specified dataset to a specified hash table
	inline void add_items(HashTable<wstring, wstring> * table, tuple<wstring, wstring>* dataset, int count) {
		for (int i = 0; i < count; i++) {
			table->insert(get<0>(*(dataset + i)), get<1>(*(dataset + i)));
		}
	}

	//remove a specified dataset from a specified hash table
	inline void remove_items(HashTable<wstring, wstring> * table, tuple<wstring, wstring>* dataset, int count) {
		for (int i = 0; i < count; i++) {
			table->remove(get<0>(*(dataset + i)));
		}
	}
}