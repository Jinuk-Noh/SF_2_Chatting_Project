#include <iostream>
#include <string>
#include <sstream>
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
			if (userInfo.id == "") {
				cout << "로그인을 해주세요" << endl;
				continue;
			}
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
			cout << "1 ~ 3 입력해주세요 : ";
		}
	} while (input != "1" 
		&& input != "2"
		&& input != "3");

	return input;
}


