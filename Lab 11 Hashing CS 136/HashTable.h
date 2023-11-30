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
	void SetTable(int size); //initializes the member variables with correct values

	bool IsHashTableFull()const { return tableSize == numRecInTable; }	 //returns true if hashTable is full
	bool IsHashTableEmpty() const { return tableSize == 0; }	//returns true if hasTable is empty
	bool IsOverflowTableFull() const { return overflowTableSize == overflowIndex; }	//returns true if OverflowTable is full
	bool IsOverflowTableEmpty() const { return overflowIndex == 0; }	//returns true if overflowTable is empty
	int GetHashTableSize() const { return tableSize; }	//returns the hashTable size
	int GetOverFlowTableSize() const { return overflowTableSize; }	//returns the overflowTable size

	bool IsEmptySpot(const StudentRecord& rec) const { return rec.getID() == -1; } //returns true if no student record exists in that spot in the table

	string GetAllActive() const; //returns a formatted string of all the active student records
	string GetAllDeleted() const; //returns a formatted string of all the deleted student records
	bool Insert(const StudentRecord& rec);	//Inserts a record into the table
	void DeleteRec(int id);	//finds and marks student record as deleted based on ID
	void SearchByID(int id, string header, string border) const; //finds and prints student record
	int CalcHashKey(int id) const; //hash function

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
	void CopyTable(const HashTable& other); //copies other into current table
	void DeleteHashTable();	//deleted the tables
};


#endif