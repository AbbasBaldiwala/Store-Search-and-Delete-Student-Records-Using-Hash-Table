#include <iostream>
#include <fstream>

#include "HashTable.h"

using namespace std;

const int SIZE_1 = 10, SIZE_2 = 100, SIZE_3 = 1000;

enum TableSizes{SIZE1 = 1, SIZE2, SIZE3, QUIT_PROGRAM};

enum Menu{SEARCH_BY_ID = 1, DELETE_REC, PRINT_ALL_ACTIVE, PRINT_ALL_DELETED, PRINT_ALL_UNPROCESSED, QUIT};

int GetTableSize();

void SearchForRecord(const HashTable& ht);

void DeleteRecordFromTable(HashTable& ht);

void ProcessRecords(HashTable& ht, string inFileName, string outFileName);

void PrintUnprocessedRecords(string fileName, string header, string border);

void PrintDeletedRec(const HashTable& ht, string header, string border);

void PrintActiveRec(const HashTable& ht, string header, string border);

/*
pre: none
post: clears cin, clears the keyboard buffer, prints an error message
*/
void ClearInvalidInput(string errMsg);

int main() {

	int userChoice;
	string inFileName = "Records.txt", outFileName = "Unprocessed_Records.txt";

	stringstream headerSS, borderSS;

	headerSS << left << setw(SETW_ID) << "ID #" << setw(SETW_NAME) << "FIRST NAME"
		<< setw(SETW_NAME) << "LAST NAME" << setw(SETW_UNITS) << "UNITS TAKEN\n";
	borderSS << setfill('-') << setw(TABLE_SIZE + 1) << "\n";

	string header = headerSS.str();
	string border = borderSS.str();

	int tableSize = GetTableSize();

	cout << left;
	HashTable ht(tableSize);

	ProcessRecords(ht, inFileName, outFileName);
	do {
		cout << "\n\nMENU: \n"
			<< SEARCH_BY_ID << ". SEARCH BY ID\n"
			<< DELETE_REC << ". DELETE RECORD\n"
			<< PRINT_ALL_ACTIVE << ". PRINT ALL ACTIVE\n"
			<< PRINT_ALL_DELETED << ". PRINT ALL DELETED\n"
			<< PRINT_ALL_UNPROCESSED << ". GET ALL UNPROCESSED\n"
			<< QUIT << ". QUIT\n\n";
		cin >> userChoice;

		switch (userChoice) {
		case SEARCH_BY_ID:
			SearchForRecord(ht);
			break;
		case DELETE_REC:
			DeleteRecordFromTable(ht);
			break;
		case PRINT_ALL_ACTIVE:
			PrintActiveRec(ht, header, border);
			break;
		case PRINT_ALL_DELETED:
			PrintDeletedRec(ht, header, border);
			break;
		case PRINT_ALL_UNPROCESSED:
			PrintUnprocessedRecords(outFileName, header, border);
			break;
		case QUIT:
			cout << "\nQUITTING..." << endl;
			break;
		default:
			ClearInvalidInput("INVALID MENU SELECTION");
		}
	} while (userChoice != QUIT);

	return 0;
}


int GetTableSize() {
	int choice, size = 0;
	bool invalidInput;
	do {
		cout << "Enter how large the table should be:\n" <<
			SIZE1 << ". " << SIZE_1 << "\n" <<
			SIZE2 << ". " << SIZE_2 << "\n" <<
			SIZE3 << ". " << SIZE_3 << "\n\n" <<
			"ENTER " << QUIT_PROGRAM << " TO QUIT\n\n";
		cin >> choice;

		switch (choice) {
		case SIZE1: 
			size = SIZE_1;
			invalidInput = false;
			break;
		case SIZE2:
			size = SIZE_2;
			invalidInput = false;
			break;
		case SIZE3:
			size = SIZE_3;
			invalidInput = false;
			break;
		case QUIT_PROGRAM:
			cout << "QUITTING...\n";
			invalidInput = false;
			break;
		default:
			invalidInput = true;
			ClearInvalidInput("INVALID ENTRY\n");
			break;
		}
	} while (invalidInput);
	return size;
}

void SearchForRecord(const HashTable& ht) {
	int id;
	bool invalidInput;
	do {
		invalidInput = false;
		cout << "Enter the Students ID #(" << ID_LEN << " digits):\n";
		cin >> id;
		if (!id) {
			ClearInvalidInput("ID MUST BE A NUMBER\n");
			invalidInput = true;
		}
		else if (to_string(id).length() != ID_LEN) {
			ClearInvalidInput("INVALID ID LENGTH\n");
			invalidInput = true;
		}
	} while (invalidInput);
	ht.SearchByID(id);
}

void DeleteRecordFromTable(HashTable& ht) {
	int id;
	bool invalidInput;
	do {
		invalidInput = false;
		cout << "Enter the ID number of the student you want to delete(" << ID_LEN << " digits):\n";
		cin >> id;
		if (!id) {
			ClearInvalidInput("ID MUST BE A NUMBER\n");
			invalidInput = true;
		}
		else if (to_string(id).length() != ID_LEN) {
			ClearInvalidInput("INVALID ID LENGTH\n");
			invalidInput = true;
		}
	} while (invalidInput);
	ht.DeleteRec(id);
}

void ProcessRecords(HashTable& ht, string inFileName, string outFileName) {
	ifstream inFile(inFileName);
	ofstream outFile(outFileName);

	if (!inFile) {
		cout << "Input file not found. Exiting the program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	if (inFile.peek() == EOF) {
		cout << "The input file is empty. Quitting the program." << endl;
		inFile.close();
		system("pause");
		exit(EXIT_FAILURE);
	}

	while (!inFile.eof()) {
		int tempId, tempUnits;
		string tempFirst, tempLast;
		inFile >> tempId >> tempFirst >> tempLast >> tempUnits;
		StudentRecord tempRec(tempId, tempFirst, tempLast, tempUnits);
		bool wasInserted = ht.Insert(tempRec);
		if (!wasInserted) {
			outFile << tempRec.ToString();
		}
	}
	if (ht.IsHashTableFull() && ht.IsOverflowTableFull()) {
		cout << "The Hash Table and Overflow Table are full.\n"
			"Any unprocessed Student records are put inside " << outFileName << "\n";
	}
	else if (ht.IsOverflowTableFull()) {
		cout << "The Overflow Table is full.\n"
			"Any unprocessed Student records are put inside " << outFileName << "\n";
	}
	inFile.close();
	outFile.close();
}

void PrintUnprocessedRecords(string fileName, string header, string border) {
	ifstream inFile(fileName);
	if (inFile.peek() == EOF) {
		cout << "\nNO UNPROCESSED STUDENT RECORDS EXIST\n\n";
		inFile.close();
	}
	string rec = "";
	string line;
	
	while (getline(inFile, line)) {
		rec += line + "\n";
	}
	
	if (rec != "") {
		cout << "\n\n" << header << border << rec << border << "\n";

	}
}

void PrintDeletedRec(const HashTable& ht, string header, string border) {
	string records = "";
	records += ht.GetAllDeleted();
	if (records != "") {
		cout << "\n\n" << header << border << records << border;
	}
	else {
		cout << "\n\nNO DELETED STUDENT RECORDS EXIST\n\n";
	}
}

void PrintActiveRec(const HashTable& ht, string header, string border) {
	string records = "";
	records += ht.GetAllActive();
	if (records != "") {
		cout << "\n\n" << header << border << records << border;
	}
	else {
		cout << "\n\nNO ACTIVE STUDENT RECORDS EXIST\n\n";
	}
}

void ClearInvalidInput(string errMsg) {
	cout << "\n" << errMsg << "\n";
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}