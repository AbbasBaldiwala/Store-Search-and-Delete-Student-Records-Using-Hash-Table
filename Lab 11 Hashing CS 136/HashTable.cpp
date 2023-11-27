#include "HashTable.h"
#include <string>
#include <cmath>


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
		cerr << "Creation of Hash Table failed, " << error.what();
	}
}

int HashTable::CalcHashKey(int id) {
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

void HashTable::DeleteHashTable() {
	delete[] hashTable;
	delete[] overflowTable;

	hashTable = nullptr;
	overflowTable = nullptr;
}

string HashTable::GetAllActive() {
	string table = "";
	for (int i = 0; i < tableSize; i++) {
		if (!IsEmptySpot(hashTable[i]) && !hashTable[i].isDeleted()) {
			table += hashTable[i].ToString();
		}
	}
	for (int i = 0; i < overflowTableSize; i++) {
		if (!IsEmptySpot(overflowTable[i]) && !hashTable[i].isDeleted()) {
			table += overflowTable[i].ToString();
		}
	}
	return table;
}

string HashTable::GetAllDeleted() {
	string table = "";
	if (!IsHashTableEmpty()) {
		for (int i = 0; i < tableSize; i++) {
			if (!IsEmptySpot(hashTable[i]) && hashTable[i].isDeleted()) {
				table += hashTable[i].ToString();
			}
		}
	}
	if (!IsOverflowTableEmpty()) {
		for (int i = 0; i < overflowTableSize; i++) {
			if (!IsEmptySpot(overflowTable[i]) && hashTable[i].isDeleted()) {
				table += overflowTable[i].ToString();
			}
		}
	}
	return table;
}

bool HashTable::Insert(const StudentRecord& rec) {
	int index = CalcHashKey(rec.getID());
	int wasInserted = false;
	if (!IsHashTableFull()) {
		if (IsEmptySpot(hashTable[index])) {
			hashTable[index] = rec;
			numRecInTable++;
			wasInserted = true;
			//cout << "insertion Successful\n";
		}
		else if (!IsOverflowTableFull()) {
			//cout << "\ninserting in overflow\n";
			overflowTable[overflowIndex] = rec;
			wasInserted = true;
			overflowIndex++;
		}
		//else {
		//	for (int i = index + 1; !wasInserted; i++ % tableSize) {
		//		if (IsEmptySpot(hashTable[i])) {
		//			hashTable[i] = rec;
		//			wasInserted = true;
		//			numRecInTable++;
		//			cout << "insertion successful\n";
		//		}
		//		else {
		//			cout << "hasTable[" << i << "] has a student record in it" << endl;
		//		}
		//	}
		//}
	}
	else if (!IsOverflowTableFull()) {
		overflowTable[overflowIndex] = rec;
		overflowIndex++;
	}
	else {
		wasInserted = false;
	}
	return wasInserted;
}

void HashTable::DeleteRec(int id) {
	int index = CalcHashKey(id);
	bool wasDeleted = false;
	if (hashTable[index].getID() == id) {
		hashTable[index].setDeleteStatus(true);
		wasDeleted = true;
		cout << "Deletion Successful\n\n";
	}
	else {
		for (int i = 0; i < overflowTableSize; i++) {
			if (overflowTable[i].getID() == id) {
				overflowTable[i].setDeleteStatus(true);
				wasDeleted = true;
				cout << "Deletion Successful\n\n";
			}
		}
	}
	if (!wasDeleted) {
		cout << "Student with id # " << id << " was not found.\n\n";
	}
}