#include "StudentRecord.h"

void StudentRecord::setRecord(int idNum, string first, string last, int units) {
	id = idNum;
	firstName = first;
	lastName = last;
	numUnitsTaken = units;
	deleted = false;
}

string StudentRecord::ToString() {
	stringstream ss;
	ss << left << setw(SETW_ID) << id << setw(SETW_NAME) << firstName << setw(SETW_NAME) << lastName << right << setw(SETW_UNITS) << numUnitsTaken << "\n";
	return ss.str();
}