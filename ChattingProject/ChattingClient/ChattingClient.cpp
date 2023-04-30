#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> //Winsock 헤더파일 include. WSADATA 들어있음.ㄴ
#include <WS2tcpip.h>

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#define MAX_SIZE 1024

using namespace std;

SOCKET db_sock;
struct UserInfo {
	string id;
	string name;
};

UserInfo userInfo;
bool GetUser();
bool GetUserInfo();
int main() {
	cout << "환영합니다~" << endl;
	cout << "------------------------------" << endl;
	cout << "1 : 로그인" << endl;
	cout << "2 : 회원가입" << endl;
	cout << "1 또는 2를 입력해주세요 : ";
	
	string input = "";
	do {
		cin >> input;
		if (input != "1" && input != "2") {
			cout << "1 또는 2를 입력하세요 : ";
		}
	} while (input != "1" && input != "2");

	cout << "------------------------------" << endl;

	if (input == "1") {
		while (1) {
			if (GetUser()) {
				cout << "환영합니다. " << userInfo.name << "님!" << endl;
				break;
			}
			else {
				cout << "로그인 실패 ID 또는 Password를 확인해주세요" << endl;
			}
		}
	}
	else {
		cout << "회원가입" << endl;
	}
}


bool GetUser() {
	string id;
	string pw;
	bool successs = false;

	cout << "ID를 입력해주세요 : " << endl;
	cin >> id;
	cout << "Password를 입력해주세요 : " << endl;
	cin >> pw;

	WSADATA wsa;

	// Winsock를 초기화하는 함수. MAKEWORD(2, 2)는 Winsock의 2.2 버전을 사용하겠다는 의미.
	// 실행에 성공하면 0을, 실패하면 그 이외의 값을 반환.
	// 0을 반환했다는 것은 Winsock을 사용할 준비가 되었다는 의미.
	int code = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (!code) {
		db_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 

		SOCKADDR_IN client_addr = {};
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(7720);
		InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);
		
		string userInfoStr = "login|" + id + "|" + pw;

		while (1) {
			if (!connect(db_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
				cout << "Server Connect" << endl;
				send(db_sock, userInfoStr.c_str(), userInfoStr.length(), 0);
				break;
			}
			cout << "Connecting..." << endl;
		}

		successs = GetUserInfo();

		closesocket(db_sock);
	}
	else {

	}

	WSACleanup();

	return successs;

}

bool GetUserInfo() {
	char buf[MAX_SIZE] = {};
	string msg;
	if (recv(db_sock, buf, MAX_SIZE, 0) > 0) {
		msg = buf;
		std::stringstream ss(msg);

		vector<string> v;
		while (getline(ss, msg, '|')) {
			v.push_back(msg);
		}

		if (v.size() > 0) {
			userInfo.id = v[0];
			userInfo.name = v[1];
		}
	}

	if (userInfo.id == "") {
		return false;
	}
	else {
		return true;
	}
}