#pragma once
#ifndef __User_H__
#define __User_H__

#include <iostream>
#include <conio.h>
#include <string.h>
#include "Sck.h"
#include "temp.h"

using std::string;

struct UserInfo {
	string id;
	string name;
};

UserInfo userInfo;
int GetUserInfo() {
	char buf[MAX_SIZE] = {};
	string msg;
	if (recv(db_sock, buf, MAX_SIZE, 0) > 0) {
		msg = buf;
		std::stringstream ss(msg);

		vector<string> v = SplitComm(msg);
	
		if (v.size() > 0) {
			userInfo.id = v[0];
			userInfo.name = v[1];
		}

		if (userInfo.id == "") {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		return -1;
	}
}

int GetUser() {
	string id,pw_1;
	string pw="";
	int i = 0;
	int successs = 0;

	cout << "ID�� �Է����ּ��� : ";
	cin >> id;
	cout << "Password�� �Է����ּ��� : ";
	pw = PwCheck(pw);

	WSADATA wsa;

	int code = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (!code) {
		db_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		SOCKADDR_IN client_addr = ConnectDBSck();
		string userInfoStr = "login|" + id + "|" + pw;
		SendComm(client_addr, userInfoStr);

		successs = GetUserInfo();

		closesocket(db_sock);
	}
	else {
		return -1;
	}

	WSACleanup();

	return successs;

}

void Login() {
	cout << "------------------------------" << endl;
	int check = 0;
	string input = "";
	while (1) {
		check = GetUser();
		if (check < 0) {
			cout << "��Ʈ��ũ�� ������ ������ϴ�." << endl;
			break;
		}
		else {
			if (check) {
				cout << endl;
				cout << "ȯ���մϴ�. " << userInfo.name << "��!" << endl;
				break;
			}
			else if (check == 0) {
				//system("cls");
				cout << "�α��� ���� ID �Ǵ� Password�� Ȯ�����ּ���" << endl;
				
				cout << "��� �õ� : �ƹ�Ű �Է�/���� �޴� : 1 �Է� : ";
				cin >> input;
				system("cls");

				if (input == "1") {
					system("cls");
					break;
				}
			}
		}
	}
}
#endif // !__User_H__

