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

	cout << "ID를 입력해주세요 : ";
	cin >> id;
	cout << "Password를 입력해주세요 : ";
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
			cout << "네트워크에 문제가 생겼습니다." << endl;
			break;
		}
		else {
			if (check) {
				cout << endl;
				cout << "환영합니다. " << userInfo.name << "님!" << endl;
				break;
			}
			else if (check == 0) {
				//system("cls");
				cout << "로그인 실패 ID 또는 Password를 확인해주세요" << endl;
				
				cout << "계속 시도 : 아무키 입력/메인 메뉴 : 1 입력 : ";
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

