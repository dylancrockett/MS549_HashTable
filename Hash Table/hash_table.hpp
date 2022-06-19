#pragma once

#include <string>
#include <iostream>

namespace hash_table {
	//function used to help space out the table properly
	std::wstring rpt_chr(wchar_t c, int num) {
		std::wstring str = L"";

		for (int i = 0; i < num; i++) {
			str += c;
		}

		return str;
	}


	/// <summary>
	/// A templated Hash Table implimentation.
	/// </summary>
	/// <typeparam name="KT">The type of the entry key.</typeparam>
	/// <typeparam name="VT">The type of the entry value.</typeparam>
	/// <typeparam name="FUNCTION">The type of the hashing function.</typeparam>
	template <typename KT, typename VT>
	class HashTable
	{
	public:
		typedef unsigned long(*HASH_FUNC)(KT, unsigned long);

		/// <summary>
		/// Object representing a entry in a Hash Table.
		/// </summary>
		/// <typeparam name="KT"></typeparam>
		/// <typeparam name="VT"></typeparam>
		class HashEntry
		{
			friend HashTable;

		public:
			struct HashNode {
				friend HashTable;
				friend HashEntry;

				/// <summary>
				/// Reference to the node before this node in the list.
				/// </summary>
				HashNode* front = nullptr;

				/// <summary>
				/// Reference to the node after this node in the list.
				/// </summary>
				HashNode* back = nullptr;

				/// <summary>
				/// Key of the node.
				/// </summary>
				KT key;

				/// <summary>
				/// Value stored in the node.
				/// </summary>
				VT value;

				HashNode(KT key, VT value) {
					this->key = key;
					this->value = value;
				}

				~HashNode() {
					//delete the next node in the list if it exists
					if (this->back != nullptr) {
						delete this->back;
						this->back = nullptr;
					}
				}

			private:
				/// <summary>
				/// Copy this node, including its links to subsequent nodes.
				/// </summary>
				/// <returns>Copy of the current node.</returns>
				HashNode* copy() {
					HashNode* node = new HashNode(this->key, this->value);
					node->front = this->front == nullptr ? nullptr : this->front->copy();
					return node;
				}

				/// <summary>
				/// Returns the node in the link.
				/// </summary>
				/// <returns>Returns the last node in the link.</returns>
				HashNode* last() {
					return this->front == nullptr ? this : this->front->last();
				}
			};

		private:
			/// <summary>
			/// Find a HashNode with the specified key.
			/// </summary>
			/// <param name="key">They key of the HashNode.</param>
			/// <returns>Pointer to the found HashNode, nullptr if no matching node was found.</returns>
			HashNode* find_node(KT key) {
				HashNode* current_node = this->head;

				//traverse the linked list until we reach the end of the list
				while (current_node != nullptr) {
					//check if the current node's key is equal to the key we are looking for
					if (current_node->key == key) {
						return current_node;
					}
					else {
						current_node = current_node->back;
					}
				}

				//if no matching node was found return a nullptr
				return nullptr;
			}

			/// <summary>
			/// Return a copy of this object.
			/// </summary>
			/// <returns>Return a copy of this object.</returns>
			HashEntry* copy() {
				HashEntry* entry = new HashEntry();
				entry->head = this->head = nullptr ? nullptr : this->head->copy();
				return entry;
			}

		public:
			/// <summary>
			/// Front of the Entry's LinkedList.
			/// </summary>
			HashNode* head = nullptr;

			/// <summary>
			/// End of the Entry's LinkedList.
			/// </summary>
			HashNode* tail = nullptr;

			HashEntry() {}

			~HashEntry() {
				//delete all nodes to free memory
				delete this->head;
				this->head = nullptr;
				this->tail = nullptr;
			}

			/// <summary>
			/// Add a new HashNode to this HashEntry with a given Key and Value.
			/// </summary>
			/// <param name="key"></param>
			/// <param name="value"></param>
			void push(KT key, VT value) {
				HashNode* new_node = new HashNode(key, value);

				//case if there are no existing nodes
				if (this->head == nullptr) {
					this->head = new_node;
					this->tail = new_node;
				}
				else {
					//point this new node's back node to the previous front node
					new_node->back = this->head;
					this->head->front = new_node;

					//set the new node as the new begining node
					this->head = new_node;
				}
			}


			/// <summary>
			/// Return a pointer to the value stored by a given key.
			/// </summary>
			/// <param name="key">The key represting the value.</param>
			/// <returns>Pointer to the value.</returns>
			VT* get(KT key) {
				VT* val = nullptr;
				HashNode* node = this->find_node(key);

				if (node != nullptr) {
					val = &node->value;
				}

				return val;
			}

			/// <summary>
			/// Remove a HashNode from the HashEntry given a key.
			/// </summary>
			/// <param name="key">The ykey represting the HashNode to be removed.</param>
			/// <returns>If a HashNode with the provided key was removed.</returns>
			bool remove(KT key) {
				HashNode* node = find_node(key);

				//if the pointer passed is a nulltpr then return false since no node with the given key exists
				if (node == nullptr) {
					return false;
				}

				HashNode* node_front = node->front;
				HashNode* node_back = node->back;

				//if the node we found is the only node in the list then set both the front and end node to null and delete the node
				if (node == this->head && node == this->tail) {
					this->head = nullptr;
					this->tail = nullptr;

					//set the front and back to nullptr to prevent cascading node deletion
					node->front = nullptr;
					node->back = nullptr;

					//delete the node
					delete node;
					return true;
				}
				//if the node to be deleted is the begining node of the LinkedList
				else if (node == this->head) {
					//set the node behind the node to be deleted to the new begining node and set that node's new front node to null
					this->head = node->back;
					this->head->front = nullptr;

					//set the front and back to nullptr to prevent cascading node deletion
					node->front = nullptr;
					node->back = nullptr;

					//delete the node
					delete node;
					return true;
				}
				//if the node to be deleted is the end node of the LinkedList
				else if (node == this->tail) {
					//set the node in front the node to be deleted to the new bend node and set that node's new end node to null
					this->tail = node->front;
					this->tail->back = nullptr;

					//set the front and back to nullptr to prevent cascading node deletion
					node->front = nullptr;
					node->back = nullptr;

					//delete the node
					delete node;
					return true;
				}
				//no special conditons just remove the node
				else {
					node_front->back = node_back;
					node_back->front = node_front;

					//set the front and back to nullptr to prevent cascading node deletion
					node->front = nullptr;
					node->back = nullptr;

					//delete the node
					delete node;
					return true;
				}

				return false;
			}

			/// <summary>
			/// Return the number of nodes that this HashEntry contains.
			/// </summary>
			/// <returns># of nodes</returns>
			int size() {
				int count = 0;
				HashNode* current_node = this->head;

				//traverse the linked list until we reach the end of the LinkedList
				while (current_node != nullptr) {
					count++;
					current_node = current_node->back;
				}

				return count;
			}

			/// <summary>
			/// Return a pointer to an array of HashNode objects for this HashEntry.
			/// </summary>
			/// <returns>Pointer to array of HashNode objects.</returns>
			HashNode** get_nodes() {
				//allocate space in memeory for the array
				HashNode** nodes = new HashNode * [this->size()];

				HashNode* current_node = this->head;

				//populate the array
				for (int i = 0; i < this->size(); i++) {
					nodes[i] = current_node;
					current_node = current_node->back;
				}

				//return the array
				return nodes;
			}
		};

	private:
		//pointer to the front of the hash table
		HashEntry** table;

		//table values
		unsigned long table_size;

		//function used to hash keys
		HASH_FUNC hash_function;

		/// <summary>
		/// Dallocate all memory in this class.
		/// </summary>
		void deallocate() {
			//delete all HashEntries
			for (int i = 0; i < this->table_size; ++i) {
				delete this->table[i];
				this->table[i] = nullptr;
			}

			//delete the table
			delete[] this->table;
		}

		/// <summary>
		/// Return the table, removes responsability for this HashTable managing it's memory.
		/// </summary>
		/// <returns></returns>
		HashEntry** copy() const {
			HashEntry** tableCopy = new HashEntry * [this->table_size]();
			
			//copy all entries
			for (int i = 0; i < this->table_size; ++i) {
				tableCopy[i] = table[i] == nullptr ? nullptr : table[i]->copy();
			}

			return tableCopy;
		}

	public:
		//constructor
		HashTable(HASH_FUNC hashing_function, unsigned long size = 128) {
			//set the hash function
			this->hash_function = hashing_function;

			//set the table size
			this->table_size = size;

			//create a table
			table = new HashEntry*[this->table_size]();
		}

		~HashTable() {
			deallocate();
		}

		HashTable& operator= (const HashTable& other)
		{
			if (this == &other) return *this;

			//deallocate existing memory
			deallocate();

			//take copy table content
			this->table = other.copy();

			//copy table size
			this->table_size = other.table_size;

			return *this;
		}

		//insert a key value pair into the table
		void insert(const KT key, VT value) {
			//hash the key
			unsigned long hashed_key = this->hash_function(key, this->table_size);

			//get the existing entry at that location at the table
			HashEntry* entry = this->table[hashed_key];

			//if no entry at that index exists yet
			if (entry == nullptr) {
				//create new entry
				entry = new HashEntry;

				//add the new key and value to the entry
				entry->push(key, value);

				//add the new entry to the table
				this->table[hashed_key] = entry;
			}
			else {
				//if the entry already exists then just add the key value pair
				entry->push(key, value);
			}
		}

		//get a pointer to a value by providing a key
		VT* get(KT key) {
			//return value
			VT* val = nullptr;

			//hash the key
			unsigned long hashed_key = this->hash_function(key, this->table_size);

			//get the existing entry at that location at the table
			HashEntry* entry = this->table[hashed_key];

			//get the value from the entry of the entry exists
			if (entry != nullptr) {
				//get the value from the entry, returns nullptr if the value key pair doesnt exist in the entry
				val = entry->get(key);
			}

			//return the value
			return  val;
		}

		/// <summary>
		/// Remove a value with the specified key from the table.
		/// </summary>
		/// <param name="key">The key to be searched for.</param>
		/// <returns>If the value was found and removed.</returns>
		bool remove(KT key) {
			//hash the key
			unsigned long hashed_key = this->hash_function(key, this->table_size);

			//get the existing entry at that location at the table
			HashEntry* entry = this->table[hashed_key];

			//if no entry exists then return false
			if (entry == nullptr) {
				return false;
			}
			else {
				//if no node in the entry contains the value then return false
				return entry->remove(key);
			}
		}

		
		/// <summary>
		/// Return the total number of Key/Value pairs stored in the table.
		/// </summary>
		/// <returns>The total number of records.</returns>
		unsigned long size() {
			//counts the number of values
			unsigned long count = 0;

			//go through each entry and get it's size
			for (int i = 0; i < this->table_size; i++) {
				HashEntry* current_entry = this->table[i];

				//only count if the entry isnt nullptr
				if (current_entry != nullptr) {
					count += current_entry->size();
				}
			}

			return count;
		}

		/// <summary>
		/// Print the Hash Table.
		/// </summary>
		/// <param name="stream">Stream to print to.</param>
		/// <param name="key_max"></param>
		/// <param name="value_max"></param>
		void print(std::wostream& stream, int key_max = 20, int value_max = 15) {
			int max_index_length = 7, max_key_length = key_max, max_value_length = value_max;

			//header and column labels
			stream << L"+" << rpt_chr(L'-', max_index_length) << L"-" << rpt_chr(L'-', max_key_length) << L"-" << rpt_chr(L'-', max_value_length) << L"+\n";
			stream << L"| Hash Table Printout " << rpt_chr(L' ', (max_index_length + max_key_length + max_value_length - 21 + 2)) << L"|\n";
			stream << L"+" << rpt_chr(L'-', max_index_length) << L"+" << rpt_chr(L'-', max_key_length) << L"+" << rpt_chr(L'-', max_value_length) << L"+\n";
			stream << L"| Index " << L"| Key " << rpt_chr(L' ', max_key_length - 6) << L" | Value " << rpt_chr(L' ', max_value_length - 7) << L"|\n";

			//table data
			for (int i = 0; i < this->table_size; i++) {
				HashEntry* current_entry = this->table[i];

				bool first = true;
				if (current_entry != nullptr) {
					auto nodes = current_entry->get_nodes();
					for (int j = 0; j < current_entry->size(); j++) {
						auto current_node = nodes[j];

						//try to convert the index into a string
						std::wstring ind = std::to_wstring(this->hash_function(current_node->key, this->table_size));

						//if this is the first line then add the index and table chars
						if (first) {
							stream << L"+" << rpt_chr(L'-', max_index_length) << L"+" << rpt_chr(L'-', max_key_length) << L"+" << rpt_chr(L'-', max_value_length) << L"+\n";
							stream << L"|" << rpt_chr(L' ', max_index_length - ind.length()) << ind << L"|" << rpt_chr(L' ', max_key_length - current_node->key.length()) << current_node->key << L"|" << rpt_chr(L' ', max_value_length - current_node->value.length()) << current_node->value << L"|\n";
							first = false;
						}
						else {
							stream << L"|" << rpt_chr(L' ', max_index_length) << L"|" << rpt_chr(L' ', max_key_length - current_node->key.length()) << current_node->key << L"|" << rpt_chr(L' ', max_value_length - current_node->value.length()) << current_node->value << L"|\n";
						}
					}
				}
			}

			//close table
			stream << L"+" << rpt_chr(L'-', max_index_length) << L"+" << rpt_chr(L'-', max_key_length) << L"+" << rpt_chr(L'-', max_value_length) << L"+\n";
		}
	};
};