#ifndef STUDENT_RECORD_H
#define STUDENT_RECORD_H

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

const int ID_LEN = 9,
SETW_ID = ID_LEN + 3,
SETW_NAME = 15,
SETW_UNITS = 11,
TABLE_SIZE = SETW_ID + SETW_NAME + SETW_NAME + SETW_UNITS;

class StudentRecord {
public:
	StudentRecord() { setRecord(-1, "", "", -1); }
	StudentRecord(int idNum, string first, string last, int units){ setRecord(idNum, first, last, units); } 
	void setRecord(int, string, string, int);	//initializes members with correct values

	int getID() const { return id; }	//returns the id #
	string getFirst() const { return firstName; } //returns the first name
	string getLast() const { return lastName; } //returns the last name
	int getUnits() const { return numUnitsTaken; } //returns how many units the student took
	bool isDeleted() const { return deleted; }	//returns true if the record has been marked as deleted

	void setDeleteStatus(bool isDeleted) { deleted = isDeleted; } //sets member variable deleted as true or false
	string ToString() const; //returns a formatted string of the record

private:
	int id = -1;
	string firstName = "";
	string lastName = "";
	int numUnitsTaken = -1;
	bool deleted = false;
};

#endif

