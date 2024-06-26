#include "HashTable.h"
#include <string>
#include <cmath>

/*pre: none
post: the object's member variables are assigned the correct values*/
void HashTable::SetTable(int size) {
	tableSize = size;
	overflowTableSize = int(roundf(size * (OVERFLOW_PERCENT / 100)));
	overflowIndex = 0;
	numRecInTable = 0;
	try {
		hashTable = new StudentRecord[tableSize];
		overflowTable = new StudentRecord[overflowTableSize];
	}
	catch (std::bad_alloc error) {
		tableSize = 0;
		overflowTableSize = 0;
		overflowIndex = 0;
		numRecInTable = 0;
		delete[] hashTable;
		delete[] overflowTable;
		hashTable = nullptr;
		overflowTable = nullptr;
		cerr << "Creation of Hash Table failed, " << error.what();
	}
}


/*pre: id must be the correct length
post: the index is returned*/
int HashTable::CalcHashKey(int id) const{
	string keyS = "";
	string idS = std::to_string(id);
	int len = idS.length();
	string left = "", right = "";
	for (int i = len - 1; i >= DISCARD_AMOUNT_BEGINNING; i--) {
		if (i >= SPLIT_INDEX) {
			right += idS[i];
		}
		else if (i < SPLIT_INDEX) {
			left += idS[i];
		}
	}

	int boundaryFolding = stoi(left) + stoi(right);
	boundaryFolding = pow(boundaryFolding, POWER_OF);
	string bf = to_string(boundaryFolding);

	int startIndex = bf.length() / NUM_DIVISIONS - 1;
	int numDigitNeeded = to_string(tableSize).length() - 1;

	for (int i = startIndex; i < startIndex + numDigitNeeded; i++) {
		keyS += bf[i];
	}
	return stoi(keyS);
}


/*pre: none
post: all active records are returned as a formatted table*/
string HashTable::GetAllActive() const {
	string table = "";
	for (int i = 0; i < tableSize; i++) {
		if (!IsEmptySpot(hashTable[i]) && !hashTable[i].isDeleted()) {
			table += hashTable[i].ToString();
		}
	}
	for (int i = 0; i < overflowTableSize; i++) {
		if (!IsEmptySpot(overflowTable[i]) && !overflowTable[i].isDeleted()) {
			table += overflowTable[i].ToString();
		}
	}
	return table;
}

/*pre: none
post: all deleted records are returned as a formatted table*/
string HashTable::GetAllDeleted() const {
	string table = "";
	if (!IsHashTableEmpty()) {
		for (int i = 0; i < tableSize; i++) {
			//cout << hashTable[i].ToString() << hashTable[i].isDeleted() << "\n";
			if (!IsEmptySpot(hashTable[i]) && hashTable[i].isDeleted()) {
				table += hashTable[i].ToString();
			}
		}
	}
	if (!IsOverflowTableEmpty()) {
		for (int i = 0; i < overflowTableSize; i++) {
			if (!IsEmptySpot(overflowTable[i]) && overflowTable[i].isDeleted()) {
				table += overflowTable[i].ToString();
			}
		}
	}
	return table;
}

/*pre: record to be inserted must exist
post: the record gets inserted if the table has space*/
bool HashTable::Insert(const StudentRecord& rec) {
	int index = CalcHashKey(rec.getID());
	int wasInserted = false;
	if (!IsHashTableFull()) {
		if (IsEmptySpot(hashTable[index])) {
			hashTable[index] = rec;
			numRecInTable++;
			wasInserted = true;
		}
		else {
			for (int i = index + 1; i < tableSize && !wasInserted; i = (i + 1) % tableSize) {
				if (IsEmptySpot(hashTable[i])) {
					hashTable[i] = rec;
					wasInserted = true;
					numRecInTable++;
					//cout << "insertion successful\n" << rec.getID();
				}
			}
		}
	}
	else if (!IsOverflowTableFull()) {
		wasInserted = true;
		overflowTable[overflowIndex] = rec;
		overflowIndex++;
	}
	else {
		wasInserted = false;
	}
	return wasInserted;
}

/*pre: none
post: if the record to be deleted is found it is marked as deleted*/
void HashTable::DeleteRec(int id) {
	int index = CalcHashKey(id);
	bool wasDeleted = false;
	if (hashTable[index].getID() == id) {
		wasDeleted = true;
		if (hashTable[index].isDeleted() == false) {
			hashTable[index].setDeleteStatus(true);
			
			cout << "\nDeletion Successful\n\n";
		}
		else {
			cout << "Student with id # " << id << " already deleted\n\n";
		}
	}
	else { //check rest of of hash table
		for (int i = index + 1; i != index && !wasDeleted; i = (i + 1) % tableSize) {
			if (hashTable[i].getID() == id) {
				wasDeleted = true;
				if (hashTable[i].isDeleted() == false) {
					hashTable[i].setDeleteStatus(true);
					cout << "\nDeletion Successful\n\n";
				}
				else {
					cout << "Student with id # " << id << " already deleted\n\n";
				}
			}
		}
		if (!wasDeleted && !IsOverflowTableEmpty()) { //check overflow table
			for (int i = 0; i < overflowTableSize; i++) {
				if (overflowTable[i].getID() == id) {
					wasDeleted = true;
					if (overflowTable[i].isDeleted() == false) {
						overflowTable[i].setDeleteStatus(true);
						cout << "\nDeletion Successful\n\n";
					}
					else {
						cout << "Student with id # " << id << " already deleted\n\n";
					}
				}
			}
		}
	}
	if (!wasDeleted) {
		cout << "Student with id # " << id << " was not found.\n\n";
	}
}

/*pre: none
post: id the record is found it is printed*/
void HashTable::SearchByID(int id, string header, string border) const{
	bool wasFound = false;
	string rec = "";
	int index = CalcHashKey(id);
	if (hashTable[index].getID() == id && !hashTable[index].isDeleted()) {
		wasFound = true;
		rec += "\n\nRECORD FOUND:\n\n" + header + border +
			hashTable[index].ToString() + border + "\nLocation: Hash Table\n\n";
	}
	else {
		for (int i = index + 1; i != index && !wasFound; i = (i + 1) % tableSize) {
			if (hashTable[i].getID() == id && !hashTable[i].isDeleted()) {
				wasFound = true;
				rec += "\n\nRECORD FOUND:\n\n" + header + border +
					hashTable[i].ToString() + border + "\nLocation: Hash Table\n\n";
			}
		}
		if (!IsOverflowTableEmpty()) {
			for (int i = 0; i < overflowIndex && !wasFound; i++) {
				if (overflowTable[i].getID() == id && !overflowTable[i].isDeleted()) {
					wasFound = true;
					rec += "\n\nRECORD FOUND:\n\n" + header + border +
						overflowTable[i].ToString() + border + "\nLocation: Overflow Array\n\n";
				}
			}
		}
	}
	if (rec != "") {
		cout << rec;
	}
	else{
		cout << "\nNo active student with ID " << id << " was found\n";
	}
}

/*pre: none
post: the 2 tables are freed from memory*/
void HashTable::DeleteHashTable() {
	delete[] hashTable;
	delete[] overflowTable;

	hashTable = nullptr;
	overflowTable = nullptr;
}

/*pre: other must exist
post: others values are copied into the Hash Table*/
void HashTable::CopyTable(const HashTable& other, StudentRecord* hashP, StudentRecord* overflowP) {
	tableSize = other.tableSize;
	overflowTableSize = other.overflowTableSize;
	overflowIndex = other.overflowIndex;
	numRecInTable = other.numRecInTable;
	for (int i = 0; i < tableSize; i++) {
		hashP[i] = other.hashTable[i];
	}
	for (int i = 0; i < overflowTableSize; i++) {
		overflowP[i] = other.overflowTable[i];
	}
}

/*pre: other must exist
post: other is copied into the current object*/
HashTable::HashTable(const HashTable& other) {
	//
	try {
		hashTable = new StudentRecord[other.tableSize];
		overflowTable = new StudentRecord[other.overflowTableSize];
		CopyTable(other, hashTable, overflowTable);
	}
	catch(std::bad_alloc error){
		tableSize = 0;
		overflowTableSize = 0;
		overflowIndex = 0;
		numRecInTable = 0;
		delete[] hashTable;
		delete[] overflowTable;
		hashTable = nullptr;
		overflowTable = nullptr;
		cout << "Copy Failed, " << error.what();
	}
}

/*pre: RHS must exist
post: RHS is copied into LHS*/
HashTable& HashTable::operator=(const HashTable& RHS) {
	StudentRecord* tempP = nullptr;
	StudentRecord* overflowP = nullptr;
	try {
		tempP = new StudentRecord[RHS.tableSize];
		overflowP = new StudentRecord[RHS.overflowTableSize];
		CopyTable(RHS, tempP, overflowP);
		DeleteHashTable();
		hashTable = tempP;
		overflowTable = overflowP;
	}
	catch (std::bad_alloc error) {
		delete[] tempP;
		delete[] overflowP;
		tempP = nullptr;
		overflowP = nullptr;
		cout << "Copy Failed, original is still intact" << error.what();
	}
	return *this;
}