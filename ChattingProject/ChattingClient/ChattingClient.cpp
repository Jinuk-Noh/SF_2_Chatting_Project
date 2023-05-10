#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include "User.h"
#include "Chatting.h"
#include "SignUp.h"


using namespace std;
string GetCommNum();
int main() {
	system("mode con cols=50 lines=30 | title Chatting");
	while (1) {
		
		//cout.setf(ios::right); (setw(10)
		cout <<"--------------------------------------------------" << endl;
		if (userInfo.id == "")  cout << "1: 로그인" << endl;
		cout << "2: 회원 가입" << endl;
		cout << "3: 채팅 들어가기" << endl<<endl;
		if (userInfo.id != "")
		{
			cout << "4: 로그아웃" << endl;
		}
		cout << "--------------------------------------------------"<<endl;


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
		case 4:
			userInfo.id = "";
			userInfo.name = "";
			system("cls");
		default:
			break;
		}
	}
}

string GetCommNum() {
	string input = "";
	do {
		cin >> input;
		if ((input != "1"
			&& input != "2"
			&& input != "3"
			&& input != "4")
			|| (input == "4" && userInfo.id == "")
			|| (input == "1" && userInfo.id != "")) {
			cout << "숫자를 입력해주세요 입력해주세요 : ";
		}
	} while ((input != "1"
		&& input != "2"
		&& input != "3"
		&& input != "4")
		|| (input == "4" && userInfo.id == "")
		|| (input == "1" && userInfo.id != ""));

	return input;
}


