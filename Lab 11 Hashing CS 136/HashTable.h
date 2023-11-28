#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "StudentRecord.h"

const float OVERFLOW_PERCENT = 25;
const int DEFAULT_TABLE_SIZE = 10,
DISCARD_AMOUNT_BEGINNING = 2,
SPLIT_INDEX = 5,
POWER_OF = 2,
NUM_DIVISIONS = 2;

class HashTable {
public:
	HashTable() { SetTable(DEFAULT_TABLE_SIZE); }
	HashTable(int size) { SetTable(size); }
	void SetTable(int size);

	bool IsHashTableFull()const { return tableSize == numRecInTable; }
	bool IsHashTableEmpty() const { return tableSize == 0; }
	bool IsOverflowTableFull() const { return overflowTableSize == overflowIndex; }
	bool IsOverflowTableEmpty() const { return overflowIndex == 0; }
	int GetHashTableSize() const { return tableSize; }
	int GetOverFlowTableSize() const { return overflowTableSize; }

	bool IsEmptySpot(const StudentRecord& rec) const { return rec.getID() == -1; }

	string GetAllActive() const;
	string GetAllDeleted() const;
	bool Insert(const StudentRecord& rec);
	void DeleteRec(int id);
	void SearchByID(int id) const;
	int CalcHashKey(int id) const;

	HashTable& operator=(const HashTable& RHS);
	HashTable(const HashTable& other);
	~HashTable() { DeleteHashTable(); }
private:
	int tableSize = 0,
		overflowTableSize = 0,
		overflowIndex = 0,
		numRecInTable = 0;
	StudentRecord* hashTable = nullptr;
	StudentRecord* overflowTable = nullptr;
	void CopyTable(const HashTable& other);
	void DeleteHashTable();

};


#endif