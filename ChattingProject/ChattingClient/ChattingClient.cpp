#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

#include "User.h"
#include "Chatting.h"
#include "temp.h"

using namespace std;
string GetCommNum();
int main() {
	while (1) {
		cout << "------------------------------" << endl;
		cout << "1: 로그인" << endl;
		cout << "2: 회원 가입" << endl;
		cout << "3: 채팅 들어가기" << endl;
		cout << "------------------------------"<<endl;
		cout << "입력: ";

		int commNum = stoi(GetCommNum());

		switch (commNum)
		{
		case 1:
			Login();
			break;
		case 2:
			SignUp();
			break;
		case 3:
			if (userInfo.id == "") cout << "채팅 들어가기" << endl;
			Chatting();
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
		if (input != "1" && input != "2" && input != "3") {
			cout << "1 ~ 3 이내의 수를 입력해주십시오: ";
		}
	} while (input != "1" 
		&& input != "2"
		&& input != "3");

	return input;
}


