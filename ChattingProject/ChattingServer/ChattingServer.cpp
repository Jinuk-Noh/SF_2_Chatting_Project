#include <iostream>
#include "DBHelper.h"

int main()
{
	DBHelper* dbHelper = DBHelper::CreateInstance();

	cout << (dbHelper == nullptr) <<endl;
	dbHelper->InitTable();
}
