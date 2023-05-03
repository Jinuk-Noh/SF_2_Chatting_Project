#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "User.h"
#include "Chatting.h"

using namespace std;

string GetCommNum();

int main() {
	while (1) {
		cout << "------------------------------" << endl;
		cout << "1 : 로그인" << endl;
		cout << "2 : 회원가입" << endl;
		cout << "3 : 채팅방 입장" << endl;
		cout << "------------------------------" << endl;
		cout << "입력 : ";

		int commNum = stoi(GetCommNum());

		switch (commNum)
		{
		case 1:
			Login();
			break;
		case 2:
			break;
		case 3:
			if (userInfo.id == "") cout << "로그인 하지 않았습니다." << endl;
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
			cout << "1 ~ 3 를 입력하세요 : ";
		}
	} while (input != "1" 
		&& input != "2"
		&& input != "3");

	return input;
}


