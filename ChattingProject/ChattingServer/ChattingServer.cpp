#include <iostream>
#include "DBHelper.h"
#include "ChattingHelper.h"


bool severClose = false;

int main()
{
	//DBHelper* dbHelper = DBHelper::CreateInstance();

	//dbHelper->InitTable();

	std::thread th1 = thread(Run);
	std::thread th2 = thread(RunDB);

	th2.detach();
	th1.join();
}

