#pragma once
#ifndef __TEMP_H__
#define __TEMP_H__

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> //Winsock 헤더파일 include. WSADATA 들어있음.
#include <WS2tcpip.h>
#include <cstring>
#include <iostream>
#include "CCrypt.h"
#include <cstddef>


#define MAX_SIZE 1024
using std::string;
SOCKET client_sock;

int CheckIdInfo() {
	char buf[MAX_SIZE] = {};
	string msg;
	if (recv(client_sock, buf, MAX_SIZE, 0) > 0) {
		msg = buf;
		std::stringstream check(msg);
		return msg == "true" ? 1 : 0;
	}
	else {
		return -1;
	}
}
SOCKADDR_IN ServerCheck() {
	client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN client_addr = {};
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(7720);
	InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);
	return client_addr;
}
void SendMsgCon(SOCKADDR_IN client_addr, string a) {
	while (1) {
		if (!connect(client_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
			send(client_sock, a.c_str(), a.length(), 0);
			break;
		}
		cout << "Connecting..." << endl;
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
	cout << "------------------------------" << endl;
	string id, name, pw, pw_1;

	string anyKey = "";
	while (1) {
		cout << "사용할 ID를 입력해주십시오: ";
		cin >> id;
		cout << "------------------------------" << endl;
		cout << endl;
		WSADATA wsa;

		int code = WSAStartup(MAKEWORD(2, 2), &wsa);
		if (!code) {
			SOCKADDR_IN client_addr = ServerCheck();
			SendMsgCon(client_addr, ("duple|" + id));
			int check = CheckIdInfo();
			closesocket(client_sock);
			WSACleanup();
			if (check < 0) {
				cout << "서버에 오류가 발생하였습니다. 재시도 해주십시오." << endl;
			}
			else {
				if (check == 0) {
					while (1) {
						int i = 0;
						cout << "사용할 비밀번호를 입력해주십시오: ";
						pw=PwCheck(pw);
						cout << endl << "비밀번호를 확인해주십시오: ";
						pw_1=PwCheck(pw_1);
			
						if (pw!=pw_1) {
							cout << endl << "입력한 비밀번호가 서로 다릅니다! \n다시 입력해주십시오." << endl << endl;
							continue;
						}
						else {
							cout << endl << "사용할 닉네임을 입력해 주십시오: ";
							cin >> name;
							string uploadSignUp = "upload|" + id + "|" + pw + "|" + name;
							int code = WSAStartup(MAKEWORD(2, 2), &wsa);
							if (!code) {
								SOCKADDR_IN client_addr= ServerCheck();
								SendMsgCon(client_addr, uploadSignUp);
								closesocket(client_sock);
								WSACleanup();
								break;
							}
						}break;
					}
				}
				else {
					cout << "이미 존재하는 아이디입니다. 다른 아이디를 입력해주십시오."<<endl;
					continue;
				}
			}
		}
		cout << "회원가입 완료!"<<endl;
		cout << "계속하려면 아무키를 입력하십시오.";
		cin >> anyKey;
		system("cls");
		break;
	}
}
#endif // !__TEMP_H__

