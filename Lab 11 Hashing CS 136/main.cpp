//100


#include <iostream>
#include <fstream>
#include <limits>

#include "HashTable.h"

using namespace std;

const int SIZE_1 = 10, SIZE_2 = 100, SIZE_3 = 1000;

enum TableSizes{SIZE1 = 1, SIZE2, SIZE3, QUIT_PROGRAM};

enum Menu{SEARCH_BY_ID = 1, DELETE_REC, PRINT_ALL_ACTIVE, PRINT_ALL_DELETED, PRINT_ALL_UNPROCESSED, QUIT};

//gets the size of the table from the user 
int GetTableSize();

//searches for a record based on the users input
void SearchForRecord(const HashTable& ht, string header, string border);

//deletes a record from the hash table
void DeleteRecordFromTable(HashTable& ht);

//gets records from input file and either puts them in the hash table or unprocessed records file
void ProcessRecords(HashTable& ht, string inFileName, string outFileName);

//Prints all the records from the unprocessed records file
void PrintUnprocessedRecords(string fileName, string header, string border);

//prints all deleted records in the hash table
void PrintDeletedRec(const HashTable& ht, string header, string border);

//prints all active records in the hash table
void PrintActiveRec(const HashTable& ht, string header, string border);

//clears any invalid input from the user
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
			SearchForRecord(ht, header, border);
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


/*pre: none
post: returns the size of the table*/
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

/*pre: hash table must exist
post: prints the record if found*/
void SearchForRecord(const HashTable& ht, string header, string border) {
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
	ht.SearchByID(id, header, border);
}


/*pre: hash tabe must exist
post: the record is marked as deleted*/
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


/*pre: input file and hash table must exist
post: the hash table and unprocessed records file is populated*/
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
	else {
		cout << "All Records were successfully stored.\n";
	}
	inFile.close();
	outFile.close();
}


/*pre: unprocessed file must exist
post: all records in the unprocessed records file are printed */
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

/*pre: Hash table must exist
post: all deleted records in the hash table are printed*/
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

/*pre: Hash table must exist
post: all active records in the hash table are printed*/
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

/*pre: none
post: clears cin, clears the keyboard buffer, prints an error message*/
void ClearInvalidInput(string errMsg) {
	cout << "\n" << errMsg << "\n";
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


//TEST OUTPUTS

//TEST #1 (ARRAY SIZE 10)
/*
Enter how large the table should be:
1. 10
2. 100
3. 1000

ENTER 4 TO QUIT

1
The Hash Table and Overflow Table are full.
Any unprocessed Student records are put inside Unprocessed_Records.txt


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887390880   PATRICIA       JOHNSON                 92
887420262   BARBARA        JONES                   97
900100006   JAMES          STEWART                  0
900588333   JOHN           SANCHEZ                 83
900940342   ROBERT         MORRIS                  55
887080838   ELIZABETH      BROWN                   53
888175671   MARY           SMITH                   44
885480567   LINDA          WILLIAMS                42
900341240   WILLIAM        REED                    87
900241601   MICHAEL        ROGERS                  27
886215457   JENNIFER       DAVIS                   88
900930645   DAVID          COOK                    73
884184644   MARIA          MILLER                  85
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

4


NO DELETED STUDENT RECORDS EXIST



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

5


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
900293220   RICHARD        MORGAN                 100
887166642   SUSAN          WILSON                  44
900469441   CHARLES        BELL                    46
881140540   MARGARET       MOORE                   44
900725648   JOSEPH         MURPHY                  83
888535894   DOROTHY        TAYLOR                  91
900954068   THOMAS         BAILEY                  43
888726173   LISA           ANDERSON                53
900761648   CHRISTOPHER    RIVERA                  33
884872635   NANCY          THOMAS                  69
900778313   DANIEL         COOPER                  30
888378591   KAREN          JACKSON                  7
900250439   PAUL           RICHARDSON               4
886006403   BETTY          WHITE                   35
900605756   MARK           COX                     50
883792379   HELEN          HARRIS                  92
900703588   DONALD         HOWARD                  24
888845822   SANDRA         MARTIN                   8
900677981   GEORGE         WARD                     7
889549774   DONNA          THOMPSON                77
900640029   KENNETH        TORRES                  33
885768820   CAROL          GARCIA                  75
900929048   STEVEN         PETERSON                28
888681414   RUTH           MARTINEZ                97
900790828   EDWARD         GRAY                    25
884553660   SHARON         ROBINSON                 5
900437635   BRIAN          RAMIREZ                 95
888719664   MICHELLE       CLARK                    2
900368838   RONALD         JAMES                    3
886236141   LAURA          RODRIGUEZ               84
900952007   ANTHONY        WATSON                  45
886663917   SARAH          LEWIS                   57
900677430   KEVIN          BROOKS                  33
885807287   KIMBERLY       LEE                      1
900848479   JASON          KELLY                   37
886641406   DEBORAH        WALKER                  35
900420830   MATTHEW        SANDERS                 93
886876793   JESSICA        HALL                    40
900173603   GARY           PRICE                   75
885000935   SHIRLEY        ALLEN                   29
900946253   TIMOTHY        BENNETT                 92
882372762   CYNTHIA        YOUNG                   13
900719443   JOSE           WOOD                    71
889470398   ANGELA         HERNANDEZ               24
900226500   LARRY          BARNES                   7
886884188   MELISSA        KING                    31
900294213   JEFFREY        ROSS                    85
885498967   BRENDA         WRIGHT                  20
900245134   FRANK          HENDERSON               46
886486581   AMY            LOPEZ                   64
900854289   SCOTT          COLEMAN                 38
885285124   ANNA           HILL                    95
900809077   ERIC           JENKINS                 11
882762402   REBECCA        SCOTT                    4
900302743   STEPHEN        PERRY                   16
888026287   VIRGINIA       GREEN                   81
900959057   ANDREW         POWELL                  12
881307506   KATHLEEN       ADAMS                   43
900341255   RAYMOND        LONG                    52
889455027   PAMELA         BAKER                   87
900317444   GREGORY        PATTERSON               35
883520571   MARTHA         GONZALEZ                78
900821003   JOSHUA         HUGHES                  34
884377300   DEBRA          NELSON                  50
900141033   JERRY          FLORES                  59
889913879   AMANDA         CARTER                  46
900735327   DENNIS         WASHINGTON              78
882915223   STEPHANIE      MITCHELL                38
900776683   WALTER         BUTLER                   6
885971153   CAROLYN        PEREZ                    2
900242765   PATRICK        SIMMONS                 93
882571225   CHRISTINE      ROBERTS                 35
900604666   PETER          FOSTER                  90
889657972   MARIE          TURNER                  87
900145742   HAROLD         GONZALES                31
886476276   JANET          PHILLIPS                59
900134947   DOUGLAS        BRYANT                   3
884779857   CATHERINE      CAMPBELL                48
900428467   HENRY          ALEXANDER                0
882594525   FRANCES        PARKER                  99
900130832   CARL           RUSSELL                 48
887709764   ANN            EVANS                   75
900301484   ARTHUR         GRIFFIN                 83
889636218   JOYCE          EDWARDS                 14
900473665   RYAN           DIAZ                    13
883851920   DIANE          COLLINS                 58
900875997   ROGER          HAYES                   66
900361905   ABBAS          BALDIWALA               50
-----------------------------------------------------



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

1
Enter the Students ID #(9 digits):
884184644


RECORD FOUND:

ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
884184644   MARIA          MILLER                  85
-----------------------------------------------------

Location: Overflow Array



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
884184644

Deletion Successful



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887390880   PATRICIA       JOHNSON                 92
887420262   BARBARA        JONES                   97
900100006   JAMES          STEWART                  0
900588333   JOHN           SANCHEZ                 83
900940342   ROBERT         MORRIS                  55
887080838   ELIZABETH      BROWN                   53
888175671   MARY           SMITH                   44
885480567   LINDA          WILLIAMS                42
900341240   WILLIAM        REED                    87
900241601   MICHAEL        ROGERS                  27
886215457   JENNIFER       DAVIS                   88
900930645   DAVID          COOK                    73
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

1
Enter the Students ID #(9 digits):
887390880


RECORD FOUND:

ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887390880   PATRICIA       JOHNSON                 92
-----------------------------------------------------

Location: Hash Table



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
887390880

Deletion Successful



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887420262   BARBARA        JONES                   97
900100006   JAMES          STEWART                  0
900588333   JOHN           SANCHEZ                 83
900940342   ROBERT         MORRIS                  55
887080838   ELIZABETH      BROWN                   53
888175671   MARY           SMITH                   44
885480567   LINDA          WILLIAMS                42
900341240   WILLIAM        REED                    87
900241601   MICHAEL        ROGERS                  27
886215457   JENNIFER       DAVIS                   88
900930645   DAVID          COOK                    73
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

4


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887390880   PATRICIA       JOHNSON                 92
884184644   MARIA          MILLER                  85
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

6

QUITTING...
*/

//TEST #2 (ARRAY SIZE 100)
/*
Enter how large the table should be:
1. 10
2. 100
3. 1000

ENTER 4 TO QUIT

2
All Records were successfully stored.


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887080838   ELIZABETH      BROWN                   53
887390880   PATRICIA       JOHNSON                 92
900725648   JOSEPH         MURPHY                  83
885498967   BRENDA         WRIGHT                  20
887166642   SUSAN          WILSON                  44
886486581   AMY            LOPEZ                   64
900293220   RICHARD        MORGAN                 100
885000935   SHIRLEY        ALLEN                   29
885285124   ANNA           HILL                    95
900134947   DOUGLAS        BRYANT                   3
883851920   DIANE          COLLINS                 58
900875997   ROGER          HAYES                   66
883520571   MARTHA         GONZALEZ                78
881140540   MARGARET       MOORE                   44
886006403   BETTY          WHITE                   35
900640029   KENNETH        TORRES                  33
900341255   RAYMOND        LONG                    52
900242765   PATRICK        SIMMONS                 93
900604666   PETER          FOSTER                  90
887420262   BARBARA        JONES                   97
900100006   JAMES          STEWART                  0
900250439   PAUL           RICHARDSON               4
900930645   DAVID          COOK                    73
900605756   MARK           COX                     50
900703588   DONALD         HOWARD                  24
900368838   RONALD         JAMES                    3
900940342   ROBERT         MORRIS                  55
900677430   KEVIN          BROOKS                  33
900946253   TIMOTHY        BENNETT                 92
900954068   THOMAS         BAILEY                  43
900245134   FRANK          HENDERSON               46
900854289   SCOTT          COLEMAN                 38
889455027   PAMELA         BAKER                   87
889657972   MARIE          TURNER                  87
886236141   LAURA          RODRIGUEZ               84
900173603   GARY           PRICE                   75
889549774   DONNA          THOMPSON                77
900735327   DENNIS         WASHINGTON              78
900952007   ANTHONY        WATSON                  45
900588333   JOHN           SANCHEZ                 83
889913879   AMANDA         CARTER                  46
886641406   DEBORAH        WALKER                  35
900341240   WILLIAM        REED                    87
885971153   CAROLYN        PEREZ                    2
886215457   JENNIFER       DAVIS                   88
900420830   MATTHEW        SANDERS                 93
888026287   VIRGINIA       GREEN                   81
888535894   DOROTHY        TAYLOR                  91
900317444   GREGORY        PATTERSON               35
884377300   DEBRA          NELSON                  50
900145742   HAROLD         GONZALES                31
886476276   JANET          PHILLIPS                59
888726173   LISA           ANDERSON                53
900294213   JEFFREY        ROSS                    85
884779857   CATHERINE      CAMPBELL                48
900428467   HENRY          ALEXANDER                0
882594525   FRANCES        PARKER                  99
884184644   MARIA          MILLER                  85
900130832   CARL           RUSSELL                 48
883792379   HELEN          HARRIS                  92
900677981   GEORGE         WARD                     7
900848479   JASON          KELLY                   37
900790828   EDWARD         GRAY                    25
900469441   CHARLES        BELL                    46
886663917   SARAH          LEWIS                   57
888175671   MARY           SMITH                   44
885480567   LINDA          WILLIAMS                42
900226500   LARRY          BARNES                   7
888378591   KAREN          JACKSON                  7
900437635   BRIAN          RAMIREZ                 95
900809077   ERIC           JENKINS                 11
882762402   REBECCA        SCOTT                    4
900778313   DANIEL         COOPER                  30
884872635   NANCY          THOMAS                  69
885768820   CAROL          GARCIA                  75
900761648   CHRISTOPHER    RIVERA                  33
900929048   STEVEN         PETERSON                28
886876793   JESSICA        HALL                    40
900302743   STEPHEN        PERRY                   16
881307506   KATHLEEN       ADAMS                   43
900821003   JOSHUA         HUGHES                  34
884553660   SHARON         ROBINSON                 5
900141033   JERRY          FLORES                  59
889470398   ANGELA         HERNANDEZ               24
882915223   STEPHANIE      MITCHELL                38
882571225   CHRISTINE      ROBERTS                 35
887709764   ANN            EVANS                   75
882372762   CYNTHIA        YOUNG                   13
900301484   ARTHUR         GRIFFIN                 83
888845822   SANDRA         MARTIN                   8
888681414   RUTH           MARTINEZ                97
886884188   MELISSA        KING                    31
900776683   WALTER         BUTLER                   6
900241601   MICHAEL        ROGERS                  27
888719664   MICHELLE       CLARK                    2
900959057   ANDREW         POWELL                  12
889636218   JOYCE          EDWARDS                 14
900719443   JOSE           WOOD                    71
900473665   RYAN           DIAZ                    13
885807287   KIMBERLY       LEE                      1
900361905   ABBAS          BALDIWALA               50
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

1
Enter the Students ID #(9 digits):
900361905


RECORD FOUND:

ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
900361905   ABBAS          BALDIWALA               50
-----------------------------------------------------

Location: Overflow Array



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
900361905

Deletion Successful



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

1
Enter the Students ID #(9 digits):
887080838


RECORD FOUND:

ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887080838   ELIZABETH      BROWN                   53
-----------------------------------------------------

Location: Hash Table



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
887080838

Deletion Successful



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887390880   PATRICIA       JOHNSON                 92
900725648   JOSEPH         MURPHY                  83
885498967   BRENDA         WRIGHT                  20
887166642   SUSAN          WILSON                  44
886486581   AMY            LOPEZ                   64
900293220   RICHARD        MORGAN                 100
885000935   SHIRLEY        ALLEN                   29
885285124   ANNA           HILL                    95
900134947   DOUGLAS        BRYANT                   3
883851920   DIANE          COLLINS                 58
900875997   ROGER          HAYES                   66
883520571   MARTHA         GONZALEZ                78
881140540   MARGARET       MOORE                   44
886006403   BETTY          WHITE                   35
900640029   KENNETH        TORRES                  33
900341255   RAYMOND        LONG                    52
900242765   PATRICK        SIMMONS                 93
900604666   PETER          FOSTER                  90
887420262   BARBARA        JONES                   97
900100006   JAMES          STEWART                  0
900250439   PAUL           RICHARDSON               4
900930645   DAVID          COOK                    73
900605756   MARK           COX                     50
900703588   DONALD         HOWARD                  24
900368838   RONALD         JAMES                    3
900940342   ROBERT         MORRIS                  55
900677430   KEVIN          BROOKS                  33
900946253   TIMOTHY        BENNETT                 92
900954068   THOMAS         BAILEY                  43
900245134   FRANK          HENDERSON               46
900854289   SCOTT          COLEMAN                 38
889455027   PAMELA         BAKER                   87
889657972   MARIE          TURNER                  87
886236141   LAURA          RODRIGUEZ               84
900173603   GARY           PRICE                   75
889549774   DONNA          THOMPSON                77
900735327   DENNIS         WASHINGTON              78
900952007   ANTHONY        WATSON                  45
900588333   JOHN           SANCHEZ                 83
889913879   AMANDA         CARTER                  46
886641406   DEBORAH        WALKER                  35
900341240   WILLIAM        REED                    87
885971153   CAROLYN        PEREZ                    2
886215457   JENNIFER       DAVIS                   88
900420830   MATTHEW        SANDERS                 93
888026287   VIRGINIA       GREEN                   81
888535894   DOROTHY        TAYLOR                  91
900317444   GREGORY        PATTERSON               35
884377300   DEBRA          NELSON                  50
900145742   HAROLD         GONZALES                31
886476276   JANET          PHILLIPS                59
888726173   LISA           ANDERSON                53
900294213   JEFFREY        ROSS                    85
884779857   CATHERINE      CAMPBELL                48
900428467   HENRY          ALEXANDER                0
882594525   FRANCES        PARKER                  99
884184644   MARIA          MILLER                  85
900130832   CARL           RUSSELL                 48
883792379   HELEN          HARRIS                  92
900677981   GEORGE         WARD                     7
900848479   JASON          KELLY                   37
900790828   EDWARD         GRAY                    25
900469441   CHARLES        BELL                    46
886663917   SARAH          LEWIS                   57
888175671   MARY           SMITH                   44
885480567   LINDA          WILLIAMS                42
900226500   LARRY          BARNES                   7
888378591   KAREN          JACKSON                  7
900437635   BRIAN          RAMIREZ                 95
900809077   ERIC           JENKINS                 11
882762402   REBECCA        SCOTT                    4
900778313   DANIEL         COOPER                  30
884872635   NANCY          THOMAS                  69
885768820   CAROL          GARCIA                  75
900761648   CHRISTOPHER    RIVERA                  33
900929048   STEVEN         PETERSON                28
886876793   JESSICA        HALL                    40
900302743   STEPHEN        PERRY                   16
881307506   KATHLEEN       ADAMS                   43
900821003   JOSHUA         HUGHES                  34
884553660   SHARON         ROBINSON                 5
900141033   JERRY          FLORES                  59
889470398   ANGELA         HERNANDEZ               24
882915223   STEPHANIE      MITCHELL                38
882571225   CHRISTINE      ROBERTS                 35
887709764   ANN            EVANS                   75
882372762   CYNTHIA        YOUNG                   13
900301484   ARTHUR         GRIFFIN                 83
888845822   SANDRA         MARTIN                   8
888681414   RUTH           MARTINEZ                97
886884188   MELISSA        KING                    31
900776683   WALTER         BUTLER                   6
900241601   MICHAEL        ROGERS                  27
888719664   MICHELLE       CLARK                    2
900959057   ANDREW         POWELL                  12
889636218   JOYCE          EDWARDS                 14
900719443   JOSE           WOOD                    71
900473665   RYAN           DIAZ                    13
885807287   KIMBERLY       LEE                      1
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

4


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887080838   ELIZABETH      BROWN                   53
900361905   ABBAS          BALDIWALA               50
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

5

NO UNPROCESSED STUDENT RECORDS EXIST



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

6

QUITTING...
*/

//TEST #3 (ARRAY SIZE 1000)
/*
Enter how large the table should be:
1. 10
2. 100
3. 1000

ENTER 4 TO QUIT

3
All Records were successfully stored.


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
900725648   JOSEPH         MURPHY                  83
900875997   ROGER          HAYES                   66
886486581   AMY            LOPEZ                   64
887080838   ELIZABETH      BROWN                   53
887390880   PATRICIA       JOHNSON                 92
885498967   BRENDA         WRIGHT                  20
885285124   ANNA           HILL                    95
900134947   DOUGLAS        BRYANT                   3
887166642   SUSAN          WILSON                  44
885000935   SHIRLEY        ALLEN                   29
900293220   RICHARD        MORGAN                 100
883520571   MARTHA         GONZALEZ                78
900604666   PETER          FOSTER                  90
900242765   PATRICK        SIMMONS                 93
881140540   MARGARET       MOORE                   44
900341255   RAYMOND        LONG                    52
882594525   FRANCES        PARKER                  99
886006403   BETTY          WHITE                   35
900640029   KENNETH        TORRES                  33
887709764   ANN            EVANS                   75
887420262   BARBARA        JONES                   97
900250439   PAUL           RICHARDSON               4
900703588   DONALD         HOWARD                  24
900100006   JAMES          STEWART                  0
900245134   FRANK          HENDERSON               46
900428467   HENRY          ALEXANDER                0
900930645   DAVID          COOK                    73
900605756   MARK           COX                     50
900145742   HAROLD         GONZALES                31
900368838   RONALD         JAMES                    3
900854289   SCOTT          COLEMAN                 38
889455027   PAMELA         BAKER                   87
900677430   KEVIN          BROOKS                  33
900940342   ROBERT         MORRIS                  55
900301484   ARTHUR         GRIFFIN                 83
900946253   TIMOTHY        BENNETT                 92
900954068   THOMAS         BAILEY                  43
900361905   ABBAS          BALDIWALA               50
889657972   MARIE          TURNER                  87
886476276   JANET          PHILLIPS                59
886236141   LAURA          RODRIGUEZ               84
900173603   GARY           PRICE                   75
900735327   DENNIS         WASHINGTON              78
900130832   CARL           RUSSELL                 48
889549774   DONNA          THOMPSON                77
900952007   ANTHONY        WATSON                  45
883851920   DIANE          COLLINS                 58
900588333   JOHN           SANCHEZ                 83
884779857   CATHERINE      CAMPBELL                48
889913879   AMANDA         CARTER                  46
886641406   DEBORAH        WALKER                  35
885971153   CAROLYN        PEREZ                    2
900341240   WILLIAM        REED                    87
900420830   MATTHEW        SANDERS                 93
886215457   JENNIFER       DAVIS                   88
884377300   DEBRA          NELSON                  50
888026287   VIRGINIA       GREEN                   81
888535894   DOROTHY        TAYLOR                  91
900317444   GREGORY        PATTERSON               35
888726173   LISA           ANDERSON                53
900294213   JEFFREY        ROSS                    85
884184644   MARIA          MILLER                  85
900677981   GEORGE         WARD                     7
883792379   HELEN          HARRIS                  92
900848479   JASON          KELLY                   37
900226500   LARRY          BARNES                   7
900790828   EDWARD         GRAY                    25
881307506   KATHLEEN       ADAMS                   43
886663917   SARAH          LEWIS                   57
900469441   CHARLES        BELL                    46
900302743   STEPHEN        PERRY                   16
900821003   JOSHUA         HUGHES                  34
885480567   LINDA          WILLIAMS                42
889636218   JOYCE          EDWARDS                 14
888175671   MARY           SMITH                   44
882762402   REBECCA        SCOTT                    4
888378591   KAREN          JACKSON                  7
900809077   ERIC           JENKINS                 11
900437635   BRIAN          RAMIREZ                 95
900141033   JERRY          FLORES                  59
900778313   DANIEL         COOPER                  30
900929048   STEVEN         PETERSON                28
884872635   NANCY          THOMAS                  69
885768820   CAROL          GARCIA                  75
900761648   CHRISTOPHER    RIVERA                  33
882915223   STEPHANIE      MITCHELL                38
886876793   JESSICA        HALL                    40
882571225   CHRISTINE      ROBERTS                 35
884553660   SHARON         ROBINSON                 5
889470398   ANGELA         HERNANDEZ               24
882372762   CYNTHIA        YOUNG                   13
888845822   SANDRA         MARTIN                   8
888681414   RUTH           MARTINEZ                97
886884188   MELISSA        KING                    31
900776683   WALTER         BUTLER                   6
900241601   MICHAEL        ROGERS                  27
888719664   MICHELLE       CLARK                    2
900959057   ANDREW         POWELL                  12
900719443   JOSE           WOOD                    71
900473665   RYAN           DIAZ                    13
885807287   KIMBERLY       LEE                      1
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

1
Enter the Students ID #(9 digits):
885807287


RECORD FOUND:

ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
885807287   KIMBERLY       LEE                      1
-----------------------------------------------------

Location: Hash Table



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
885807287

Deletion Successful



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

1
Enter the Students ID #(9 digits):
900473665


RECORD FOUND:

ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
900473665   RYAN           DIAZ                    13
-----------------------------------------------------

Location: Hash Table



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
900473665

Deletion Successful



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
900473665
Student with id # 900473665 already deleted



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
900725648   JOSEPH         MURPHY                  83
900875997   ROGER          HAYES                   66
886486581   AMY            LOPEZ                   64
887080838   ELIZABETH      BROWN                   53
887390880   PATRICIA       JOHNSON                 92
885498967   BRENDA         WRIGHT                  20
885285124   ANNA           HILL                    95
900134947   DOUGLAS        BRYANT                   3
887166642   SUSAN          WILSON                  44
885000935   SHIRLEY        ALLEN                   29
900293220   RICHARD        MORGAN                 100
883520571   MARTHA         GONZALEZ                78
900604666   PETER          FOSTER                  90
900242765   PATRICK        SIMMONS                 93
881140540   MARGARET       MOORE                   44
900341255   RAYMOND        LONG                    52
882594525   FRANCES        PARKER                  99
886006403   BETTY          WHITE                   35
900640029   KENNETH        TORRES                  33
887709764   ANN            EVANS                   75
887420262   BARBARA        JONES                   97
900250439   PAUL           RICHARDSON               4
900703588   DONALD         HOWARD                  24
900100006   JAMES          STEWART                  0
900245134   FRANK          HENDERSON               46
900428467   HENRY          ALEXANDER                0
900930645   DAVID          COOK                    73
900605756   MARK           COX                     50
900145742   HAROLD         GONZALES                31
900368838   RONALD         JAMES                    3
900854289   SCOTT          COLEMAN                 38
889455027   PAMELA         BAKER                   87
900677430   KEVIN          BROOKS                  33
900940342   ROBERT         MORRIS                  55
900301484   ARTHUR         GRIFFIN                 83
900946253   TIMOTHY        BENNETT                 92
900954068   THOMAS         BAILEY                  43
900361905   ABBAS          BALDIWALA               50
889657972   MARIE          TURNER                  87
886476276   JANET          PHILLIPS                59
886236141   LAURA          RODRIGUEZ               84
900173603   GARY           PRICE                   75
900735327   DENNIS         WASHINGTON              78
900130832   CARL           RUSSELL                 48
889549774   DONNA          THOMPSON                77
900952007   ANTHONY        WATSON                  45
883851920   DIANE          COLLINS                 58
900588333   JOHN           SANCHEZ                 83
884779857   CATHERINE      CAMPBELL                48
889913879   AMANDA         CARTER                  46
886641406   DEBORAH        WALKER                  35
885971153   CAROLYN        PEREZ                    2
900341240   WILLIAM        REED                    87
900420830   MATTHEW        SANDERS                 93
886215457   JENNIFER       DAVIS                   88
884377300   DEBRA          NELSON                  50
888026287   VIRGINIA       GREEN                   81
888535894   DOROTHY        TAYLOR                  91
900317444   GREGORY        PATTERSON               35
888726173   LISA           ANDERSON                53
900294213   JEFFREY        ROSS                    85
884184644   MARIA          MILLER                  85
900677981   GEORGE         WARD                     7
883792379   HELEN          HARRIS                  92
900848479   JASON          KELLY                   37
900226500   LARRY          BARNES                   7
900790828   EDWARD         GRAY                    25
881307506   KATHLEEN       ADAMS                   43
886663917   SARAH          LEWIS                   57
900469441   CHARLES        BELL                    46
900302743   STEPHEN        PERRY                   16
900821003   JOSHUA         HUGHES                  34
885480567   LINDA          WILLIAMS                42
889636218   JOYCE          EDWARDS                 14
888175671   MARY           SMITH                   44
882762402   REBECCA        SCOTT                    4
888378591   KAREN          JACKSON                  7
900809077   ERIC           JENKINS                 11
900437635   BRIAN          RAMIREZ                 95
900141033   JERRY          FLORES                  59
900778313   DANIEL         COOPER                  30
900929048   STEVEN         PETERSON                28
884872635   NANCY          THOMAS                  69
885768820   CAROL          GARCIA                  75
900761648   CHRISTOPHER    RIVERA                  33
882915223   STEPHANIE      MITCHELL                38
886876793   JESSICA        HALL                    40
882571225   CHRISTINE      ROBERTS                 35
884553660   SHARON         ROBINSON                 5
889470398   ANGELA         HERNANDEZ               24
882372762   CYNTHIA        YOUNG                   13
888845822   SANDRA         MARTIN                   8
888681414   RUTH           MARTINEZ                97
886884188   MELISSA        KING                    31
900776683   WALTER         BUTLER                   6
900241601   MICHAEL        ROGERS                  27
888719664   MICHELLE       CLARK                    2
900959057   ANDREW         POWELL                  12
900719443   JOSE           WOOD                    71
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

4


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
900473665   RYAN           DIAZ                    13
885807287   KIMBERLY       LEE                      1
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

5

NO UNPROCESSED STUDENT RECORDS EXIST



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

6

QUITTING...
*/

//TEST #4 (INVALID INPUTS)
/*
Enter how large the table should be:
1. 10
2. 100
3. 1000

ENTER 4 TO QUIT

safd

INVALID ENTRY

Enter how large the table should be:
1. 10
2. 100
3. 1000

ENTER 4 TO QUIT

5

INVALID ENTRY

Enter how large the table should be:
1. 10
2. 100
3. 1000

ENTER 4 TO QUIT

1
The Hash Table and Overflow Table are full.
Any unprocessed Student records are put inside Unprocessed_Records.txt


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

1
Enter the Students ID #(9 digits):
fasd

ID MUST BE A NUMBER

Enter the Students ID #(9 digits):
900361905

No active student with ID 900361905 was found


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
dsaf

ID MUST BE A NUMBER

Enter the ID number of the student you want to delete(9 digits):
7

INVALID ID LENGTH

Enter the ID number of the student you want to delete(9 digits):
900361905
Student with id # 900361905 was not found.



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

6

QUITTING...

*/

//TEST #5 (Deleting an already deleted record and searching for a deleted student)
/*
MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
900940342

Deletion Successful



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

1
Enter the Students ID #(9 digits):
900940342

No active student with ID 900940342 was found


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
900940342
Student with id # 900940342 already deleted



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
887390880   PATRICIA       JOHNSON                 92
887420262   BARBARA        JONES                   97
900100006   JAMES          STEWART                  0
900588333   JOHN           SANCHEZ                 83
887080838   ELIZABETH      BROWN                   53
888175671   MARY           SMITH                   44
885480567   LINDA          WILLIAMS                42
900341240   WILLIAM        REED                    87
900241601   MICHAEL        ROGERS                  27
886215457   JENNIFER       DAVIS                   88
900930645   DAVID          COOK                    73
884184644   MARIA          MILLER                  85
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

4


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
900940342   ROBERT         MORRIS                  55
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

6

QUITTING...

*/


//TEST #6 (no records in file)
/*
Enter how large the table should be:
1. 10
2. 100
3. 1000

ENTER 4 TO QUIT

1
The input file is empty. Quitting the program.
Press any key to continue . . .

*/

//TEST #7 (Deleting all records in table)

/*
Enter how large the table should be:
1. 10
2. 100
3. 1000

ENTER 4 TO QUIT

1


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
888175671   MARY           SMITH                   44
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

1
Enter the Students ID #(9 digits):
888175671


RECORD FOUND:

ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
888175671   MARY           SMITH                   44
-----------------------------------------------------

Location: Hash Table



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
888175671

Deletion Successful



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

3


NO ACTIVE STUDENT RECORDS EXIST



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

2
Enter the ID number of the student you want to delete(9 digits):
888175671
Student with id # 888175671 already deleted



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

4


ID #        FIRST NAME     LAST NAME      UNITS TAKEN
-----------------------------------------------------
888175671   MARY           SMITH                   44
-----------------------------------------------------


MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

5

NO UNPROCESSED STUDENT RECORDS EXIST



MENU:
1. SEARCH BY ID
2. DELETE RECORD
3. PRINT ALL ACTIVE
4. PRINT ALL DELETED
5. GET ALL UNPROCESSED
6. QUIT

6

QUITTING...

C:\Users\Abbas\source\repos\Lab 11 Hashing CS 136\x64\Debug\Lab 11 Hashing CS 136.exe (process 23592) exited with code 0.
Press any key to close this window . . .
*/