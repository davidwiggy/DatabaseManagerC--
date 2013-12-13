/******************************************************************************************************************
*FILE : DatabaseManager.cpp                                                                                       *
*PROG : Timothy Wiggins                                                                                           *
*PURP : To input a pair of provided files. Then to provide a menu to: either List the file, Search the file,      *
*PURP : Add a record to the file, or delete a record from the file.  When list is select all the information from *
*PURP : the file is printed to the screen. If search the file is selected the user inputs a userID then the       *
*PURP : program searchs for the id, if found it's printed, if not found its states that the user was not found.   *   
*PURP : If add is selected the user is prompted to enter all the information. All the information enter is        *
*PURP : validated then wrote to the file. If deleted is selected the file is only removed from the index.         *
*******************************************************************************************************************/
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
using namespace std;

//Declaring Structure
struct data
{
	int pos;
	int id;
};

struct questions
{
	char custID[45];
	char custName[45];
	char state[45];
	char discount[45];
	char balance[45];
	char date[60];
};

struct date
{
	int day;
	int month;
	int year;
};

struct tempStruct
{
	int custId;
	char custName[28];
	char state[3];
	char disCode;
	double balDue;
	struct date dueDate;
};

//Function Prototypes
int    loadDatabase(struct data *pointer, int numRecords);
int    getInt(questions *anArr);
int    binarySearch(struct data *pointer, int searchNum, int sizeOfArray);
void   getStr(char *anArr, char *passedInQuestion);
double getDouble(char *anArr, questions *pointer);
char   getChar(char *anArr, questions *pointer);
bool   is_int(char *anString);
bool   is_float(char *anArr);
bool   idValidator(char *anArr);
bool   stateValidator(char *anArr);
bool   dateValidator(char *anArr, struct tempStruct *dataPointer);
void   setDate(struct tempStruct *recPoint, struct questions *pointer);
void   setName(char *anArr, questions *pointer);
void   setState(char *anArr, questions *pointer);
void   listTable(data *pointer, int sizeOfArray);
void   searchfile(data *pointer, questions *questPointer, int sizeOfRecords);
void   sortStruct(data *pointer);
int    deleteRecord(data *pointer, questions *questPointer, int sizeOfRecords);
int    addRecord(data *pointer, questions *questPointer, int sizeOfRecords);

int main()
{
	int  id = NULL, sizeOfRecords = NULL, menu = 1;
	double balance = NULL;
	const int NUMOFSTRUCTS = 12;
	char buffer[40];
	struct data records[NUMOFSTRUCTS];
	struct questions quest;
	struct questions *qPointer;
	qPointer = &quest;

	strcpy_s(quest.custID, 45, "Please enter a three digit customer id.   ");
	strcpy_s(quest.custName, 45, "Please enter a customer name.    ");
	strcpy_s(quest.state, 45, "Please enter a state abbreviation.    ");
	strcpy_s(quest.discount, 45, "Please enter a discount code.    ");
	strcpy_s(quest.balance, 45, "Please enter a balance due.    ");
	strcpy_s(quest.date, 60, "Please enter a date in this format XX/XX/XXXX.      ");

	printf("Hello and welcome to the Database program.\n");
	sizeOfRecords = loadDatabase(records, NUMOFSTRUCTS);

	while(menu != 0)
	{
		printf("\nEnter a 1 to PRINT, 2 to SEARCH, 3 to ADD, 4 to DELETE, or 0 to EXIT.  ");
		gets(buffer);
		menu = atoi(buffer);

		if(menu == 1)
			listTable(records, sizeOfRecords);
		else if(menu == 2)
			searchfile(records, qPointer, sizeOfRecords);
		else if(menu == 3)
			sizeOfRecords = addRecord(records, qPointer, sizeOfRecords);
		else if(menu == 4)
			sizeOfRecords = deleteRecord(records, qPointer, sizeOfRecords);
	}

	gets(buffer);
	return 0;
}

//***********************Function definitions***********************************
/******************************************************************************
*Purpose   : To load a database                                               *
*Passed In : A pointer to the struct and to the file                          *
*Returned  : Nothing, however a struct is loaded                              *
*******************************************************************************/
int loadDatabase(struct data *pointer, int numRecords)
{
	int buf[2];
	int ct = NULL, temp = NULL;
	FILE *fp;

	if ((fp = fopen("ASSIGNV1.IND", "rb")) == NULL) 
	{
		printf("Couldn't open file for reading\n");
		exit(1);
	}

	fread(buf, sizeof(int), 2, fp);
	
	while(!feof(fp))
	{
		pointer[ct].id = buf[0];
		pointer[ct].pos = buf[1];

		fread(buf, sizeof(int), 2, fp);
		ct++;
	}
	
	temp = ct;
	if(numRecords >= temp)
	{
		while(numRecords > temp)
		{
			pointer[temp].id  = 9999;
			pointer[temp].pos = 9999;
			temp++;
		}
	}

	fclose(fp);
	return ct;
}

/******************************************************************************
*Purpose   : To call functions to test whether the entered value is valid     *
*Purpose   : and then convert to integer                                      *
*Passed In : A array of questions                                             *
*Returned  : A valid customer id as an integer                                *
*******************************************************************************/
int getInt(questions *anArr)
{
	char buffer[80];
	bool validInt, validId;
	int id = NULL;
	validInt = validId = false;
	
	while(validInt == false)
	{
		getStr(buffer, anArr->custID);
		validInt = is_int(buffer);
		if(validInt == true)
			validId = idValidator(buffer);
		
		if(validId == true)
			id = atoi(buffer);
		else
			validInt = false;
		
		if(validInt == false)
			printf("Sorry you must enter a valid three digit customer id.\n");
	}
	
	return id;
}

/******************************************************************************
*Purpose   : To get a string                                                  *
*Passed In : A array of questions and filing status                           *
*Returned  : Nothing                                                          *
*******************************************************************************/
void getStr(char *anArr, char *passedInQuestion)
{
	printf("%s", passedInQuestion);
	gets(anArr);
}

/******************************************************************************
*Purpose   : To get a double                                                  *
*Passed In : A array of questions                                             *
*Returned  : A double                                                         *
*******************************************************************************/
double getDouble(char *anArr, questions *pointer)
{
	double value = NULL;
	bool valid = false;

	while(valid == false)
	{
		getStr(anArr, pointer->balance);
		valid = is_float(anArr);
		if(valid == false)
			printf("Sorry you must enter a valid balance due.\n");
		else
			value = atof(anArr);
	}

	return value;
}

/******************************************************************************
*Purpose   : To get a character                                               *
*Passed In : A array of questions and an array                                *
*Returned  : A character                                                      *
*******************************************************************************/
char getChar(char *anArr, questions *pointer)
{
	char code = NULL;
	bool valid = false;

	while(valid == false)
	{
		getStr(anArr, pointer->discount);
		code = toupper(anArr[0]);

		if(code == 'A' || code == 'B' || code == 'C' || code == ' ')
			valid = true;
		else
			printf("Please enter a valid discount EX: 'A', 'B', 'C', or one blank space.\n");
	}

	return code;
}

/******************************************************************************
*Purpose   : To get a valid state abbreviation                                *
*Passed In : A array of questions, and an array                               *
*Returned  : Nothing                                                          *
*******************************************************************************/
void setState(char *anArr, questions *pointer)
{
	bool valid = false;

	while(valid == false)
	{
		getStr(anArr, pointer->state);
		valid = stateValidator(anArr);
		if(valid == false)
			printf("Sorry you must enter a valid state abbreviation.\n");
	}
}

/******************************************************************************
*Purpose   : To set a string with valid information not containing all blanks *
*Passed In : An array, and an array of questions                              *
*Returned  : Nothing                                                          *
*******************************************************************************/
void setName(char *anArr, questions *pointer)
{
	bool valid = false;
	
	while(valid == false)
	{
		int blankCt = NULL;
		
		getStr(anArr, pointer->custName);

		for(int x = 0; x < strlen(anArr); x++)
		{
			if(anArr[x] == ' ')
				blankCt++;
		}

		if(strlen(anArr) == blankCt)
			printf("Sorry you must enter a valid customer name.\n");
		else
			valid = true;
	}
}

/******************************************************************************
*Purpose   : To set the due date                                              *
*Passed In : Data struct, questions struct, and the arraySize                 *
*Returned  : Nothing                                                          *
*******************************************************************************/
void setDate(struct tempStruct *recPoint, struct questions *pointer)
{
	bool valid = false;
	char buffer[30];

	while(valid == false)
	{
		getStr(buffer, pointer->date);
		valid = dateValidator(buffer, recPoint);
		if(valid == false)
			printf("Sorry you must enter a valid date in this format: XX/XX/XXXX\n");
	}
}


/******************************************************************************
*Purpose   : To make sure a enter value is a valid digit                      *
*Passed In : A string containing the customer id                              *
*Returned  : A boolean value                                                  *
*******************************************************************************/
bool is_int(char *anString)
{
	bool  b = true;
	int x;

	for(x = 0; x < strlen(anString); x++)
	{
		if(!isdigit(anString[x]))
			b = false;
	}

	return b;
}

/******************************************************************************
*Purpose   : To make sure a enter value is a valid decimal                    *
*Passed In : A string containing the balance due                              *
*Returned  : A boolean value                                                  *
*******************************************************************************/
bool is_float(char *anArr)
{
	bool valid = true;

	int x = NULL, dec = NULL;

	if(strlen(anArr) == 0) 
		valid = false;

	for(x = 0; x < strlen(anArr); x++)
		if(anArr[x] == '.')
			dec++;
		else if(!isdigit(anArr[x]))
			valid = false;


	if(dec > 1)
		valid = false;
	
	return valid;
}

/******************************************************************************
*Purpose   : To make sure that an enter id is within a valid range            *
*Passed In : An Customer ID                                                   *
*Returned  : A boolean value                                                  *
*******************************************************************************/
bool idValidator(char *anArr)
{
	bool idOkay = false;
	int  tempId = NULL;

	if(strlen(anArr) != 3)
	{
		idOkay =false;
		return idOkay;
	}
	
	tempId = atoi(anArr);

	if(tempId >= 000 && tempId <= 998)
		idOkay = true;

	return idOkay;
}

/******************************************************************************
*Purpose   : To load an array of state abbreviations from a file and the do a *
*Purpose   : binary search to find the enter state.                           *
*Passed In : A state abbreviation                                             *
*Returned  : A boolean value                                                  *
*******************************************************************************/
bool stateValidator(char *anArr)
{
	static char stateAbbr[50][3];
	bool found = false;
	char buf[40];
	FILE *fp;
	int first = NULL, last = 50, mid = NULL, x = NULL, result = NULL;

	if(strlen(stateAbbr[0]) == 0)
	{
		
		if ((fp = fopen("STATES.DAT", "r")) == NULL) 
		{
			printf("Couldn't open file for reading\n");
			exit(1);
		}
		
		fgets(buf, 40, fp);
		while(!feof(fp))
		{
			fgets(stateAbbr[x], 4, fp);
			stateAbbr[x][strlen(stateAbbr[x])-1] = '\0';
			x++;
			fgets(buf, 40, fp);
		}
		fclose(fp);
	}

	for(int y = 0; y < strlen(anArr); y++)
		anArr[y] = toupper(anArr[y]);

	while (first <=last && found == false)
	{
		mid = (first + last) / 2;
		result = strcmp(stateAbbr[mid], anArr);

		if (result == 0)
			found = true;
		else if (result < 0)
			first = mid + 1;
		else
			last = mid - 1;
	}

	return found;
}

/******************************************************************************
*Purpose   : To validate that an enter date is proper                         *
*Passed In : A potential date, the data struct, and the size of records       *
*Returned  : A boolean value, it also has a side effect of loading the date   * 
*Returned  : into the data struct                                             *
*******************************************************************************/
bool dateValidator(char *anArr, struct tempStruct *dataPointer)
{
	bool validInt = false, leapYear = false, validYear = false;
	char buffer[15], temp[15];
	int x = NULL, ct = NULL, month = NULL, day = NULL, year = NULL;
	int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if(strlen(anArr) != 10)
		return validInt;

	if(anArr[2] != '/' || anArr[5] != '/')
		return validInt;

	while(x < 9)
	{
		if(ct == 2)
			ct++;
		else if(ct == 5)
			ct++;
		buffer[x] = anArr[ct];
		x++;
		ct++;
	}

	validInt = is_int(buffer);
	if(validInt == false)
		return validInt;

	for(int y = 0; y < 8; y++)
	{
		if(y < 2)
			temp[y] = buffer[y];
		else if(y == 2)
		{
			month = atoi(temp);
			temp[y - 2] = buffer[y];
		}
		else if(y < 4)
			temp[y - 2] = buffer[y];
		else if(y == 4)
		{
			day = atoi(temp);
			temp[y - 4] = buffer[y];
		}
		else
			temp[y - 4] = buffer[y];
	}

	year = atoi(temp);

	if((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
		leapYear = true;
	else 
		leapYear = false;

	if(leapYear == true)
		monthDays[1] = 29;

	if(year < 1900 || year > 2050)
		return validYear;
	if(month < 1 || month > 12)
		return validYear;
	if(day < 1 || day > monthDays[month - 1])
		return validYear;

	validYear = true;

	dataPointer->dueDate.day   = day;
	dataPointer->dueDate.month = month;
	dataPointer->dueDate.year  = year;

	return validYear;
}

/******************************************************************************
*Purpose   : To print the data struct                                         *
*Passed In : A pointer to the struct and the size of records                  *
*Returned  : Nothing                                                          *
*******************************************************************************/
void listTable(data *pointer, int sizeOfArray)
{
	FILE *fp;
	int x = 0;

	struct tempStruct temp;

	system("cls");

	printf("\nCust Id   Customer Name       State   DisCode   Bal Due   Due Date\n");
	if ((fp = fopen("ASSIGNV1.RND", "rb")) == NULL) 
	{
		printf("Couldn't open file for reading\n");
		exit(1);
	}

	fseek(fp, sizeof(tempStruct) * pointer[x].pos, SEEK_SET);
	fread(&temp, sizeof(temp), 1, fp);

	for(int x = 0; x < sizeOfArray; x++)
	{
		fseek(fp, sizeof(tempStruct) * pointer[x].pos, SEEK_SET);
		fread(&temp, sizeof(temp), 1, fp);

		printf("%-10i%-20s%-10s%-7c$%-10.2f%i/%i/%i\n", temp.custId, temp.custName, temp.state,
			temp.disCode, temp.balDue, temp.dueDate.month, temp.dueDate.day, temp.dueDate.year);

	}
	fclose(fp);
}

/****************************************************************************************
*Purpose   : To search for a customer and print the info of the customer                *
*Passed In : A pointer to the struct, pointer to quest struct, and the size of records  *
*Returned  : Nothing                                                                    *
*****************************************************************************************/
void searchfile(data *pointer, questions *questPointer, int sizeOfRecords)
{

	int id = NULL, indexNum = NULL;
	FILE *fp;
	struct tempStruct temp;

	if ((fp = fopen("ASSIGNV1.RND", "rb")) == NULL) 
	{
		printf("Couldn't open file for reading\n");
		exit(1);
	}

	id = getInt(questPointer);
	indexNum = binarySearch(pointer, id, sizeOfRecords);

	if(indexNum == -1)
		printf("RECORD NOT IN DATABASE.\n\n");
	else
	{
		fseek(fp, sizeof(tempStruct) * pointer[indexNum].pos, SEEK_SET);
		fread(&temp, sizeof(temp), 1, fp);

		printf("\nCust Id   Customer Name       State   DisCode   Bal Due   Due Date\n");
		printf("%-10i%-20s%-10s%-7c$%-10.2f%i/%i/%i\n\n", temp.custId, temp.custName, temp.state,
			temp.disCode, temp.balDue, temp.dueDate.month, temp.dueDate.day, temp.dueDate.year);
	}

	fclose(fp);
}

/******************************************************************************
*Purpose   : To search the database for a custId                              *
*Passed In : A pointer to the struct, id to search for, and size of records   *
*Returned  : Nothing                                                          *
*******************************************************************************/
int binarySearch(struct data *pointer, int searchNum, int sizeOfArray)
{
	int first = 0;
	int last  = sizeOfArray;
	int found = 0;
	int mid   = 0;
	int indexNum = -1;

	while (first <=last && found == 0)
	{
		mid = (first + last) / 2;

		if (pointer[mid].id == searchNum)
		{
			found = 1;
			indexNum = mid;
		}
		else if (pointer[mid].id < searchNum)
		{
			first = mid + 1;
		}
		else
		{
			last = mid - 1;
		}
	}
	return indexNum;
}

/**********************************************************************************
*Purpose   : To add a record                                                      *
*Passed In : A pointer to the struct, pointer to questions, and size of records   *
*Returned  : Size of records                                                      *
**********************************************************************************/
int addRecord(data *pointer, questions *questPointer, int sizeOfRecords)
{

	int id = NULL, indexNum = NULL;
	FILE *fp;
	struct tempStruct temp;
	struct tempStruct *p1;
	p1 = &temp;
	char buffer[40];

	if ((fp = fopen("ASSIGNV1.RND", "r+b")) == NULL) 
	{
		printf("Couldn't open file for reading\n");
		exit(1);
	}

	id = getInt(questPointer);
	indexNum = binarySearch(pointer, id, sizeOfRecords);

	if(indexNum == -1)
	{
		temp.custId = id;
		pointer[sizeOfRecords].id = id;
		pointer[sizeOfRecords].pos = sizeOfRecords;

		setName(buffer, questPointer); 
		strcpy_s(temp.custName, 28, buffer);

		setState(buffer, questPointer);
		strcpy_s(temp.state, 3, buffer);

		temp.disCode = getChar(buffer, questPointer);

		temp.balDue = getDouble(buffer, questPointer);

		setDate(p1, questPointer);

		sizeOfRecords++;
		sortStruct(pointer);
	}
	else
	{
		printf("Sorry record already exists in database.\n\n");
		return sizeOfRecords;
	}

	fseek(fp, 0, SEEK_END);
	fwrite(&temp, sizeof(tempStruct), 1, fp);

	fclose(fp);
	return sizeOfRecords;
}

/******************************************************************************
*Purpose   : To sort the data struct                                          *
*Passed In : A pointer to the struct, and size of records                     *
*Returned  : A sorted struct                                                  *
*******************************************************************************/
void sortStruct(data *pointer)
{
	bool swap = true;
	data tempSwap;

	while(swap)
	{
		swap = false;

		for(int x = 0; x < 11; x++)
		{
			if(pointer[x].id > pointer[x + 1].id)
			{
				swap = true;
				tempSwap = pointer[x];
				pointer[x] = pointer[x + 1];
				pointer[x + 1] = tempSwap;
			}
		}
	}
}

/******************************************************************************
*Purpose   : To delete a record in the index                                  *
*Passed In : A pointer to the struct, and size of records                     *
*Returned  : Nothing                                                          *
*******************************************************************************/
int deleteRecord(data *pointer, questions *questPointer, int sizeOfRecords)
{
	int id = NULL;
	int indexNum = NULL;

	printf("You have selected to delete a record.\n");
	id = getInt(questPointer);

	indexNum = binarySearch(pointer, id, sizeOfRecords);

	if(indexNum == -1)
		printf("Sorry the record does not exist in the database.\n");
	else
	{
		pointer[indexNum].id  = 99999;
		pointer[indexNum].pos = 99999;
		sortStruct(pointer);
		sizeOfRecords--;
	}

	return sizeOfRecords;
}