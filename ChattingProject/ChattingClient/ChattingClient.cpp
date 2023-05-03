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
		cout << "1 : �α���" << endl;
		cout << "2 : ȸ������" << endl;
		cout << "3 : ä�ù� ����" << endl;
		cout << "------------------------------" << endl;
		cout << "�Է� : ";

		int commNum = stoi(GetCommNum());

		switch (commNum)
		{
		case 1:
			Login();
			break;
		case 2:
			break;
		case 3:
			if (userInfo.id == "") cout << "�α��� ���� �ʾҽ��ϴ�." << endl;
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
			cout << "1 ~ 3 �� �Է��ϼ��� : ";
		}
	} while (input != "1" 
		&& input != "2"
		&& input != "3");

	return input;
}


