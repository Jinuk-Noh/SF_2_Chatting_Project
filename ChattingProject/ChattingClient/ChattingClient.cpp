#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "User.h"

using namespace std;

string GetCommNum();

int main() {
	while (1) {
		cout << "------------------------------" << endl;
		cout << "1 : �α���" << endl;
		cout << "2 : ȸ������" << endl;
		cout << "------------------------------" << endl;
		cout << "�Է� : ";

		int commNum = stoi(GetCommNum());

		switch (commNum)
		{
		case 1:
			Login();
			break;
		case2:
			break;
		default:
			break;
		}
	}
}

string GetCommNum() {
	string input = "";
	do {
		cin >> input;
		if (input != "1" && input != "2") {
			cout << "1 �Ǵ� 2�� �Է��ϼ��� : ";
		}
	} while (input != "1" && input != "2");

	return input;
}


