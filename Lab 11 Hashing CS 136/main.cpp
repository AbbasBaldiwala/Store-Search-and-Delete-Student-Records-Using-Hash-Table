#include <iostream>
#include <string>
#include <fstream>

#include "HashTable.h"

using namespace std;

void ProcessRecords(HashTable& ht, string inFileName, string outFileName);

void PrintUnprocessedRecords(string fileName);
//int CalcHashKey(int id, int tSize);

int main() {
	//StudentRecord rec(881234567, "Abbas", "Baldiwala", 15);
	HashTable ht(100);
	string inFileName = "Records.txt", outFileName = "Unprocessed_Records.txt";

	//cout << ht.GetHashTableSize() << " " << ht.GetOverFlowTableSize() << endl;
	//
	//cout << ht.CalcHashKey(881234567) << endl;
	//cout << "test" << endl;
	//for (int i = 0; i < 4; i++) {
	//	ht.Insert(rec);
	//}
	//for (int i = 0; i < 4; i++) {
	//	ht.DeleteRec(881234567);
	//}

	//string active = ht.GetAllActive();
	//string deleted = ht.GetAllDeleted();

	////FIXME 
	//if (active == "") {
	//	cout << "no active records";
	//}
	//else {
	//	cout << active;
	//}

	//if (deleted == "") {
	//	cout << "no deleted records";
	//}
	//else {
	//	cout << deleted;
	//}
	ProcessRecords(ht, inFileName, outFileName);
	string active = ht.GetAllActive();
	cout << active << endl;
	PrintUnprocessedRecords(outFileName);

	return 0;
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

	inFile.close();
	outFile.close();
}

void PrintUnprocessedRecords(string fileName) {
	ifstream inFile(fileName);

	//if (!inFile) {
	//	cout << "Input file not found. Exiting the program." << endl;
	//	system("pause");
	//	exit(EXIT_FAILURE);
	//}
	if (inFile.peek() == EOF) {
		cout << "The input file is empty. Quitting the program." << endl;
		inFile.close();
		system("pause");
		exit(EXIT_FAILURE);
	}
	string line;
	while (getline(inFile, line)) {
		cout << line << "\n";
	}
}