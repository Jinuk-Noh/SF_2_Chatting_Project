#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> //Winsock ������� include. WSADATA �������
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#define MAX_SIZE 1024
using namespace std;
SOCKET client_sock;
int CheckIdInfo();
int main() {
	string id, pw, pw_1, name;
	cout << "ȸ������" << endl << "����� ID�� �Է����ֽʽÿ�: ";
	cin >> id;
	cout << endl;

}
