#pragma once
#ifndef __TEMP_H__
#define __TEMP_H__

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> //Winsock ������� include. WSADATA �������.
#include <WS2tcpip.h>
#include <iostream>


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
void PwCheck(string a) {
	int i = 0;
	a = "";
	while (1) {
		if (_kbhit())
		{
			a += _getch();
			if (a[i] == 13) {
				break;
			}
			i++;
			cout << "*";
		}
	}
}

void SignUp() {
	cout << "------------------------------" << endl;
	string id, name, pw, pw_1;

	string anyKey = "";
	while (1) {
		cout << "����� ID�� �Է����ֽʽÿ�: ";
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
				cout << "������ ������ �߻��Ͽ����ϴ�. ��õ� ���ֽʽÿ�." << endl;
			}
			else {
				if (check == 0) {
					while (1) {
						int i = 0;
						cout << "����� ��й�ȣ�� �Է����ֽʽÿ�: ";
						PwCheck(pw);
						cout << endl << "��й�ȣ�� Ȯ�����ֽʽÿ�: ";
						PwCheck(pw_1);
			
						if (pw!=pw_1) {
							cout << endl << "�Է��� ��й�ȣ�� ���� �ٸ��ϴ�! \n�ٽ� �Է����ֽʽÿ�." << endl << endl;
							continue;
						}
						else {
							cout << endl << "����� �г����� �Է��� �ֽʽÿ�: ";
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
					cout << "�̹� �����ϴ� ���̵��Դϴ�. �ٸ� ���̵� �Է����ֽʽÿ�."<<endl;
					continue;
				}
			}
		}
		cout << "ȸ������ �Ϸ�!"<<endl;
		cout << "����Ϸ��� �ƹ�Ű�� �Է��Ͻʽÿ�.";
		cin >> anyKey;
		system("cls");
		break;
	}
}
#endif // !__TEMP_H__