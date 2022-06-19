#include <iostream>
#include "hash_table.hpp"
#include "hash_table_utils.hpp"
#include "unit_testing.hpp"

namespace hash_table_testing {
	using namespace std;
	using namespace hash_table;
	using namespace unit_testing;
	using namespace hash_table_utils;

	//random value generator seed (for consistantcy between tests)
	const int RAND_SEED = 1231548125718923;

	inline void test_functionality() {
		UnitTest test = UnitTest(L"Hash Table Functionality Test");

		//initalize random value seed
		srand(RAND_SEED);

		//create instance of a Hash Table
		auto hash_table = new HashTable<wstring, wstring>(string_hash_function);

		test.assert<bool>(L"Removing a value when empty.", [&hash_table]() {
			hash_table->remove(L"does not exist");
			return true;
		}, true);

		test.assert<bool>(L"Adding duplicate values.", [&hash_table]() {
			hash_table->insert(L"duplicate", L"duplicate");
			hash_table->insert(L"duplicate", L"duplicate");
			return true;
		}, true);

		delete hash_table;
		hash_table = new HashTable<wstring, wstring>(string_hash_function);

		test.assert<bool>(L"Removing a value which does exist.", [&hash_table]() {
			hash_table->insert(L"test", L"test");
			return hash_table->remove(L"test");
		}, true);

		test.assert<bool>(L"Finding an existing value.", [&hash_table]() {
			hash_table->insert(L"test", L"test");
			return *hash_table->get(L"test") == L"test";
		}, true);

		delete hash_table;
		hash_table = new HashTable<wstring, wstring>(string_hash_function);
		auto dataset = gen_dataset(100);
		add_items(hash_table, dataset, 100);
		//delete dataset;

		test.assert<bool>(L"Print 100 items", [&hash_table]() {
			hash_table->print(wcout);
			return true;
		}, true);

		//print results
		test.log_results();
	}

	inline void test_performance() {
		UnitTest test = UnitTest(L"Hash Table Performance Test");

		//initalize random value seed
		srand(RAND_SEED);

		//create instance of a Hash Table
		HashTable<wstring, wstring>* hash_table;

		//sizes to test
		vector<int> sizes = { 100, 1000, 10000, 100000 };

		for (auto size : sizes) {
			//create table
			hash_table = new HashTable<wstring, wstring>(string_hash_function, size / 10);

			//dataset used for adding/removing
			auto item_count = 100;
			auto dataset = gen_dataset(size);

			//performance testing for size
			test.assert<bool>(L"Add " + to_wstring(size) + L" items.", [&hash_table, &dataset, &size]() {
				add_items(hash_table, dataset, size);
				return true;
			}, true);

			test.assert<bool>(L"Remove " + to_wstring(size) + L" items.", [&hash_table, &dataset, &size]() {
				remove_items(hash_table, dataset, size);
				return true;
			}, true);

			//cleanup
			delete hash_table;
		}

		//print results
		test.log_results();
	}
}