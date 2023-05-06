#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> //Winsock 헤더파일 include. WSADATA 들어있음
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

#include "User.h"
#include "Chatting.h"

#define MAX_SIZE 1024
using namespace std;
SOCKET client_sock;
int CheckIdInfo();
int main() {
	while (1) {
		cout << "------------------------------" << endl;
		cout << "1 : 로그인" << endl;
		cout << "2 : 회원가입" << endl;
		cout << "3 : 채팅 들어가기" << endl;
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
			if (userInfo.id == "") cout << "로그인 해 주십시오." << endl;
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
			cout << "1 ~ 3 이내의 수를 입력해주십시오 : ";
		}
	} while (input != "1" 
		&& input != "2"
		&& input != "3");

	return input;
}


