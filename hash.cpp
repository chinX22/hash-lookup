#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "hash.h"
using namespace std;

// This function will error check the requirements the user gave, and initalize
// the hash table with those requirements
string Hash::Set_Up(size_t table_size, const string &fxn, const string &collision){
	if (Coll == 1 || Coll == 2){
		return "Hash table already set up";
	}
	(void) table_size;
	(void) fxn;
	(void) collision;
	if (table_size < 1){
		return "Bad table size";
	} else {
		Keys.resize(table_size);
		Vals.resize(table_size);
	}

	if (fxn == "XOR"){
		Fxn = 0;
	} else if (fxn == "Last7"){
		Fxn = 1;
	} else {
		return "Bad hash function";
	}

	if (collision == "Linear"){
		Coll = 1;
	} else if (collision == "Double"){
		Coll = 2;
	} else {
		return "Bad collision resolution strategy";
	}
	Nkeys = 0;
	Nprobes = 0;
	return "";
}

// This function will take a hex key and a string and attempt to add it to the
// hash table
string Hash::Add(const string &key, const string &val){

	// Error-checks for bad keys/values, and if the table is set up/full
	if (Coll != 1 && Coll != 2){
		return "Hash table not set up";
	}
	if (key.size() == 0){
		return "Empty key";
	}
	for (unsigned long int i = 0; i < key.size(); i++){
		char c = key[i];
		if (c < 48 || c > 57){
			if (c < 65 || c > 70){
				if (c < 97 || c > 102){
					return "Bad key (not all hex digits)";
				}
			}
		}
	}

	if (val.size() == 0){
		return "Empty val";
	}


	if (Nkeys == Keys.size()){
		return "Hash table full";
	}

	stringstream ss;
	int hash = 0;
	int oldHash = 0; // oldHash and newHash needed for double hashing
	int newHash = 0; 
	int passes = 0;
	int localProbes = 0; // counts the probes in each call of Add()
	do{
		// This is the implemented Last7 hash function that will take the last
		// 7 hex digits and use the decimal value of that has the hash value
		if (Fxn == 1){
			string reduced;
			if (key.size() < 7){
				reduced = key;
			} else {
				reduced = key.substr(key.size() - 7, 7);
			}
			ss.clear();
			ss.str("");
			ss << hex << reduced;
			ss >> hash;

		} else {
			// This is the XOR function that will break the key into 7 digit
			// segments and bitwise xor them and use the decimal value has
			// the hash value
			int numOfSegments = (key.size() / 7);
			if ((key.size() % 7) > 0){
				numOfSegments++;
			}
			int arr[numOfSegments];
			for (int i = 0; i < numOfSegments; i++){
				ss.clear();
				ss.str("");
				int temp = 0;
				ss << hex << key.substr((7 * i), 7);
				ss >> temp;
				arr[i] = temp;
			}
			hash = arr[0];
			if (numOfSegments > 1){
				for (int i = 1; i < numOfSegments; i++){
					hash = hash ^ arr[i];
				}
			}
			ss << hex << hash;
			ss >> hash;
		}
		hash = hash % Keys.size();

		newHash = hash;
		if(newHash == 0){
			newHash = 1;
		}

		// After it is decided that double hashing is needed, the other hash
		// function will run and that hash value gets saved in newHash

		if (Coll == 2 && passes > 0){

			// This loop will keep adding the newHash into the hash value until
			// it finds an empty spot
			do{
				hash = ( oldHash + newHash) % Keys.size();

				// Several times in Add(), there will be a few of these checks
				// to make sure the key isn't already in a possible index for
				// the key
				if (Keys[hash] == key){
					return "Key already in the table";
				}
				localProbes++;
				if (Vals[hash] == ""){
					Vals[hash] = val;
					Keys[hash] = key;
					Fxn = (Fxn == 0) ? 1 : 0;

					// Having a seperate localProbes before adding to the total
					// probes ensures we don't add probes for attempts that
					// couldn't be entered
					Nprobes += localProbes;
					Nkeys++;
					return "";
				} else {
					oldHash = hash;
					passes++;
				}
			} while (passes < ((int)Keys.size() * 10));
			return "Cannot insert key";
		}

		if (Keys[hash] == key){
			return "Key already in the table";
		}

		if (Vals[hash] != ""){
			if (Coll == 2){

				// If double hashing, it will remember the first hash value and
				// switch to the other function for the increment
				if (passes == 0){
					Fxn = (Fxn == 0) ? 1 : 0;
					passes++;
					oldHash = hash;
				}
			} else {

				// If linear probing, it will keep adding 1 to the hash index
				// and try every index of the table until it finds an empty one
				for(int i = 0; i < (int)Keys.size(); i++){
					localProbes++;
					hash++;
					if (Keys[hash] == key){
						return "Key already in the table";
					}
					if (hash == (int)Keys.size()){
						hash -= Keys.size();
					}
					if (Keys[hash] == key){
						return "Key already in the table";
					}
					if (Vals[hash] == ""){
						Vals[hash] = val;
						Keys[hash] = key;
						Nprobes += localProbes;
						Nkeys++;
						return "";
					}
				}
				return "Cannot insert key";

			}
		} else {
			Vals[hash] = val;
			Keys[hash] = key;
			Nkeys++;
			return "";
		}

	} while(true);

	(void) key;
	(void) val;
}


// This function will try to find a value after being given a key, it will look
// largely similar to the Add function since its also just finding an index
string Hash::Find(const string &key){

	// Error checking for a bad key
	for (unsigned long int i = 0; i < key.size(); i++){
		char c = key[i];
		if (c < 48 || c > 57){
			if (c < 65 || c > 70){
				if (c < 97 || c > 102){
					return "";
				}
			}
		}
	}

	stringstream ss;
	int hash = 0;
	int oldHash = 0;
	int newHash = 0;
	int passes = 0;

	do{
		// Same Last7 and XOR function as in Add();
		if (Fxn == 1){
			string reduced;
			if (key.size() < 7){
				reduced = key;
			} else {
				reduced = key.substr(key.size() - 7, 7);
			}
			ss.clear();
			ss.str("");
			ss << hex << reduced;
			ss >> hash;

		} else {
			int numOfSegments = (key.size() / 7);
			if ((key.size() % 7) > 0){
				numOfSegments++;
			}
			int arr[numOfSegments];
			for (int i = 0; i < numOfSegments; i++){
				ss.clear();
				ss.str("");
				int temp = 0;
				ss << hex << key.substr((7 * i), 7);
				ss >> temp;
				arr[i] = temp;
			}
			hash = arr[0];
			if (numOfSegments > 1){
				for (int i = 1; i < numOfSegments; i++){
					hash = hash ^ arr[i];
				}
			}
			ss << hex << hash;
			ss >> hash;
		}


		hash = hash % Keys.size();

		newHash = hash;
		if(newHash == 0){
			newHash = 1;
		}


		// In contrast to Add(), instead of placing a value and a key into the
		// table, we just return the found string or nothing if not found
		if (Coll == 2 && passes > 0){
			do{
				hash = ( oldHash + newHash) % Keys.size();
				if (Vals[hash] != "" && (Keys[hash] == key)){
					Fxn = (Fxn == 0) ? 1 : 0;
					return Vals[hash];
				} else {
					oldHash = hash;
					passes++;
				}
			} while (passes < ((int)Keys.size() * 10));
			Fxn = (Fxn == 0) ? 1 : 0;
			return "";
		}

		if (Vals[hash] != "" && (Keys[hash] != key)){
			if (Coll == 2){
				if (passes == 0){
					Fxn = (Fxn == 0) ? 1 : 0;
					passes++;
					oldHash = hash;
				} else {
					Fxn = (Fxn == 0) ? 1 : 0;
					return "";
				}
			} else {
				for(int i = 0; i < (int)Keys.size(); i++){
					if (hash == (int)Keys.size()){
						hash -= Keys.size();
					}
					if (Vals[hash] != "" && (Keys[hash] == key)){
						return Vals[hash];
					}
					hash++;
				}
				return "";

			}
		} else {
			return Vals[hash];
		}
	} while (true);
}

// This will print the table's contents, including an entry's index, key, and
// value
void Hash::Print() const{
	for (long unsigned int i = 0; i < Vals.size(); i++){
		if (Vals[i] != ""){
			cout << setw(5) << i << " " << Keys[i] << " " << Vals[i] << endl;
		}
	}
}

// This will return the number of probes that occured from collisions
size_t Hash::Total_Probes(){
	return Nprobes;
}

// boogie woogie
