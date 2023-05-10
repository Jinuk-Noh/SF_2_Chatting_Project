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
		if (userInfo.id == "")  cout << "1: �α���" << endl;
		cout << "2: ȸ�� ����" << endl;
		cout << "3: ä�� ����" << endl<<endl;
		if (userInfo.id != "")
		{
			cout << "4: �α׾ƿ�" << endl;
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
				cout << "�α����� ���ּ���" << endl;
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
			cout << "���ڸ� �Է����ּ��� �Է����ּ��� : ";
		}
	} while ((input != "1"
		&& input != "2"
		&& input != "3"
		&& input != "4")
		|| (input == "4" && userInfo.id == "")
		|| (input == "1" && userInfo.id != ""));

	return input;
}


