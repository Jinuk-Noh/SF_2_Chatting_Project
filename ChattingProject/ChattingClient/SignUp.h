#pragma once
#ifndef __SIGNUP_H__
#define __SIGNUP_H__
#include <iostream>
#include <cstring>
#include <cstddef>

#include "Sck.h"
#include "CCrypt.h"


#define MAX_SIZE 1024
using std::string;

int CheckIdInfo() {
	char buf[MAX_SIZE] = {};
	string msg;
	if (recv(db_sock, buf, MAX_SIZE, 0) > 0) {
		msg = buf;
		std::stringstream check(msg);
		return msg == "true" ? 1 : 0;
	}
	else {
		return -1;
	}
}

string PwCheck(string pw) {
	int cnt = 0;
	pw = "";
	while (1) {
		if (_kbhit()){
			int c = _getch();
			/*a += _getch();*/
			//엔터
			if (c == 13) {
				pw = PwEncrypt(pw);
				return pw;
			}
			//스페이스바
			else if (c == 32) {
				continue;
			}
			//백스페이스
			else if (c == 8) {
				if (cnt == 0) {
					continue;
				}
				else {
					//* 별지우기
					cout << "\b \b";
					pw.pop_back();
					if (cnt != 0) {
						cnt--;
					}
					continue;
				}
			}
			else {
				//10자리 이상 입력 제한
				if (cnt > 9) continue;
				pw += c;
				cnt++;
				cout << "*";
			}
		}
	}
}

void SignUp() {
	string id, name, pw, pw_1;
	int input;

	string anyKey = "";
	cout << endl;
	system("cls");
	while (1) {
		cout << "사용할 ID를 입력해주십시오: ";
		cin >> id;
		WSADATA wsa;

		int code = WSAStartup(MAKEWORD(2, 2), &wsa);
		if (!code) {
			SOCKADDR_IN client_addr = ConnectDBSck(); //ServerCheck();
			SendComm(client_addr, ("duple|" + id));
			int check = CheckIdInfo();
			closesocket(db_sock);
			WSACleanup();

			if (check < 0) {
				cout << "서버에 오류가 발생하였습니다. 재시도 해주십시오." << endl;
			}
			else {
				if (check == 0) {
					cout << "해당 아이디로 회원가입 하시겠습니까?" << endl <<endl;
					cout << "--------------------------------------------------" << endl;
					cout << endl << "예: 1 입력\n아니오: 아무키 입력"<<endl << endl;
					cout << "--------------------------------------------------" << endl;
					cin >> input;
					if (input != 1) {
						system("cls");
						continue;
					}

					system("cls");
					while (1) {
						int i = 0;
						cout << "입력한 ID: " << id << endl;
						cout << "사용할 비밀번호를 입력해주십시오: ";
						pw=PwCheck(pw);
						cout << endl << "비밀번호를 확인해주십시오: ";
						pw_1=PwCheck(pw_1);
			
						if (pw != pw_1) {
							system("cls");
							cout << endl << "입력한 비밀번호가 서로 다릅니다! \n다시 입력해주십시오." << endl << endl;
						}
						else {

							cout << endl << "사용할 닉네임을 입력해 주십시오: ";
							cin >> name;
							string uploadSignUp = "upload|" + id + "|" + pw + "|" + name;
							int code = WSAStartup(MAKEWORD(2, 2), &wsa);
							if (!code) {
								SOCKADDR_IN client_addr= ConnectDBSck();
								SendComm(client_addr, uploadSignUp);
								closesocket(db_sock);
								WSACleanup();
								break;
							}
						}
					}
				}
				else {
					cout << endl;
					cout << "이미 존재하는 아이디입니다." << endl;
					cout<<"다른 아이디를 입력해주십시오."<<endl;
					cout << endl;
					continue;
				}
			}
		}
		cout << "회원가입 완료!"<<endl<<endl;
		cout << "메인 메뉴로 돌아가려면 아무키를 입력하십시오."<<endl;
		cin >> anyKey;
		system("cls");
		break;
	}
}
#endif // !__SIGNUP_H___

