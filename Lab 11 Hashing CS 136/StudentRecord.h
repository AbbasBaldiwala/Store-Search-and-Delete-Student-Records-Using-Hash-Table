#ifndef STUDENT_RECORD_H
#define STUDENT_RECORD_H

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

const int ID_LEN = 9,
SETW_ID = ID_LEN + 3,
SETW_NAME = 15,
SETW_UNITS = 7;

class StudentRecord {
public:
	StudentRecord() { setRecord(-1, "", "", -1); }
	StudentRecord(int idNum, string first, string last, int units){ setRecord(idNum, first, last, units); }
	void setRecord(int, string, string, int);

	int getID() const { return id; }
	string getFirst() const { return firstName; }
	string getLast() const { return lastName; }
	int getUnits() const { return numUnitsTaken; }
	bool isDeleted() const { return deleted; }

	void setDeleteStatus(bool isDeleted) { deleted = isDeleted; }
	string ToString();

private:
	int id = -1;
	string firstName = "";
	string lastName = "";
	int numUnitsTaken = -1;
	bool deleted = false;
};

#endif

