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
		cout << "1 : 로그인" << endl;
		cout << "2 : 회원가입" << endl;
		cout << "------------------------------" << endl;
		cout << "입력 : ";

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
			cout << "1 또는 2를 입력하세요 : ";
		}
	} while (input != "1" && input != "2");

	return input;
}


