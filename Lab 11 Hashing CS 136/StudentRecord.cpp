#include "StudentRecord.h"

/*pre: none
post: the member variables are assigned the correct values*/
void StudentRecord::setRecord(int idNum, string first, string last, int units) {
	id = idNum;
	firstName = first;
	lastName = last;
	numUnitsTaken = units;
	deleted = false;
}

/*pre: none
post: a formatted string of the record is returned*/
string StudentRecord::ToString() const {
	stringstream ss;
	ss << left << setw(SETW_ID) << id << setw(SETW_NAME) << firstName << setw(SETW_NAME) << lastName << right << setw(SETW_UNITS) << numUnitsTaken << "\n";
	return ss.str();
}