#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> //Winsock ������� include. WSADATA �������.��
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
	cout << "ȯ���մϴ�~" << endl;
	cout << "------------------------------" << endl;
	cout << "1 : �α���" << endl;
	cout << "2 : ȸ������" << endl;
	cout << "1 �Ǵ� 2�� �Է����ּ��� : ";
	
	string input = "";
	do {
		cin >> input;
		if (input != "1" && input != "2") {
			cout << "1 �Ǵ� 2�� �Է��ϼ��� : ";
		}
	} while (input != "1" && input != "2");

	cout << "------------------------------" << endl;

	if (input == "1") {
		while (1) {
			if (GetUser()) {
				cout << "ȯ���մϴ�. " << userInfo.name << "��!" << endl;
				break;
			}
			else {
				cout << "�α��� ���� ID �Ǵ� Password�� Ȯ�����ּ���" << endl;
			}
		}
	}
	else {
		cout << "ȸ������" << endl;
	}
}


bool GetUser() {
	string id;
	string pw;
	bool successs = false;

	cout << "ID�� �Է����ּ��� : " << endl;
	cin >> id;
	cout << "Password�� �Է����ּ��� : " << endl;
	cin >> pw;

	WSADATA wsa;

	// Winsock�� �ʱ�ȭ�ϴ� �Լ�. MAKEWORD(2, 2)�� Winsock�� 2.2 ������ ����ϰڴٴ� �ǹ�.
	// ���࿡ �����ϸ� 0��, �����ϸ� �� �̿��� ���� ��ȯ.
	// 0�� ��ȯ�ߴٴ� ���� Winsock�� ����� �غ� �Ǿ��ٴ� �ǹ�.
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