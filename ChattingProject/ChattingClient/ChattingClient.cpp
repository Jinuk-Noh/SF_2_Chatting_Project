#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> //Winsock 헤더파일 include. WSADATA 들어있음
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
	cout << "회원가입" << endl << "사용할 ID를 입력해주십시오: ";
	cin >> id;
	cout << endl;

}
