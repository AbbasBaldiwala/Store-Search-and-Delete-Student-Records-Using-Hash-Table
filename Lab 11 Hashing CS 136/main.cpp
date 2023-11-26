#include <iostream>
#include <string>

#include "HashTable.h"

using namespace std;

//int CalcHashKey(int id, int tSize);

int main() {
	StudentRecord rec(881234567, "Abbas", "Baldiwala", 15);
	HashTable ht(10);

	cout << ht.GetHashTableSize() << " " << ht.GetOverFlowTableSize() << endl;
	
	cout << ht.CalcHashKey(881234567) << endl;
	cout << "test" << endl;
	for (int i = 0; i < 4; i++) {
		ht.Insert(rec);
	}
	for (int i = 0; i < 4; i++) {
		ht.DeleteRec(881234567);
	}

	string active = ht.GetAllActive();
	string deleted = ht.GetAllDeleted();

	//FIXME 
	if (active == "") {
		cout << "no active records";
	}
	else {
		cout << active;
	}

	if (deleted == "") {
		cout << "no deleted records";
	}
	else {
		cout << deleted;
	}

	return 0;
}


//int CalcHashKey(int id, int tSize) {
//	//int key;
//	string keyS = "";
//	string idS = std::to_string(id);
//	int len = idS.length();
//	//extraction
//	//id = id % int(pow(10, 6));
//	//boundary folding
//	string left = "", right = "";
//	for (int i = len - 1; i >= 2; i--) {
//		if (i >= 5) {
//			right += idS[i];
//		}
//		else if (i < 5) {
//			left += idS[i];
//		}
//	}
//	int boundaryFolding = stoi(left) + stoi(right);
//	boundaryFolding = pow(boundaryFolding, 2);
//	string bf = to_string(boundaryFolding);
//	int startIndex = bf.length() / 2 - 1;
//	int numDigitNeeded = to_string(tSize).length() - 1;
//	cout << numDigitNeeded << endl;
//	for (int i = startIndex; i < startIndex + numDigitNeeded; i++) {
//		keyS += bf[i];
//	}
//	cout << keyS << endl;
//	return stoi(keyS);
//}