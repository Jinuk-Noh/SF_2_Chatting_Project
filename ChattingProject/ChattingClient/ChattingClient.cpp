#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include "User.h"
#include "Chatting.h"
#include "temp.h"


using namespace std;
string GetCommNum();
int main() {
	system("mode con cols=50 lines=30 | title Chatting");
	while (1) {
		
		//cout.setf(ios::right); (setw(10)
		cout <<"------------------------------" << endl;
		cout << "1: �α���" << endl;
		cout << "2: ȸ�� ����" << endl;
		cout << "3: ä�� ����" << endl;
		cout << "------------------------------"<<endl;
		cout << "�Է�: ";


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
				cout << "�α����� ���ּ���" << endl;
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
			cout << "1 ~ 3 �Է����ּ��� : ";
		}
	} while (input != "1" 
		&& input != "2"
		&& input != "3");

	return input;
}


