#pragma once
#ifndef __CHATTING_HELPER_H__
#define __CHATTING_HELPER_H__

#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

#define MAX_SIZE 1024
#define MAX_CLIENT 12

using namespace std;

struct SOCKET_INFO {
	SOCKET sck;
	string user;
};
#pragma region ChattingSocket
std::vector<SOCKET_INFO> sck_list;
SOCKET_INFO server_sock;
int client_cnt = 0;

void server_init() {
	server_sock.sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN server_addr = {};
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(7718);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(server_sock.sck, (sockaddr*)&server_addr, sizeof(server_addr));
	listen(server_sock.sck, SOMAXCONN);

	server_sock.user = "server";

	cout << "Server On" << endl;
}

void add_client() {
	SOCKADDR_IN addr = {};
	int addrSize = sizeof(addr);
	char buf[MAX_SIZE] = {};

	ZeroMemory(&addr, addrSize);

	SOCKET_INFO new_client = {};

	new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrSize);
	recv(new_client.sck, buf, MAX_SIZE, 0);
	new_client.user = string(buf);

	string msg = "[공지] " + new_client.user + "님이 입장했습니다.";
	cout << msg << endl;
	sck_list.push_back(new_client);

	std::thread th(recv_msg, client_cnt);

	client_cnt++;
	cout << "[공지 현재 접속자 수 : " << client_cnt << "명" << endl;
	send_msg(msg.c_str());

	th.join();
}

void send_msg(const char* msg) {
	for (int i = 0; i < client_cnt; i++) {
		send(sck_list[i].sck, msg, MAX_SIZE, 0);
	}
}

void recv_msg(int idx) {
	char buf[MAX_SIZE] = {};
	string msg = "";

	while (1) {
		ZeroMemory(&buf, MAX_SIZE);
		if (recv(sck_list[idx].sck, buf, MAX_SIZE, 0) > 0) {
			msg = sck_list[idx].user + " : " + buf;
			cout << msg << endl;
			send_msg(msg.c_str());
		}
		else {
			msg = "[공지] " + sck_list[idx].user + " 님이 퇴장했습니다.";
			cout << msg << endl;
			send_msg(msg.c_str());
			del_client(idx);

			return;
		}
	}
}

void del_client(int idx) {
	closesocket(sck_list[idx].sck);
	client_cnt--;
}

void Run() {
	WSADATA wsa;

	int code = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (!code) {
		server_init();
		std::thread th1[MAX_CLIENT];
		for (int i = 0; i < MAX_CLIENT; i++) {
			th1[i] = std::thread(add_client);
		}
	
	while (1) {
		if (client_cnt < MAX_CLIENT) {
			thread(add_client);
		}

		string text, msg = "";

		std::getline(cin, text);
		const char* buf = text.c_str();
		msg = server_sock.user + " : " + buf;
		send_msg(msg.c_str());
	}

	for (int i = 0; i < MAX_CLIENT; i++) {
		th1[i].join();
	}
		closesocket(server_sock.sck);
	}
	else {
	cout << "프로그램 종료. (Error code : " << code << ")";
	}

	WSACleanup();
}
#pragma endregion

#pragma region DBSocket
SOCKET_INFO dbServer_sock;
void db_server_init() {
	dbServer_sock.sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN server_addr = {};
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(7720);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(dbServer_sock.sck, (sockaddr*)&server_addr, sizeof(server_addr));
	listen(dbServer_sock.sck, SOMAXCONN);

	dbServer_sock.user = "dbServer";

	cout << "DB Server On" << endl;
}

void RunDB() {
	db_server_init();

	while (1) {
		SOCKADDR_IN addr = {};
		int addrSize = sizeof(addr);
		char buf[MAX_SIZE] = {};

		ZeroMemory(&addr, addrSize);

		SOCKET_INFO new_client = {};

		new_client.sck = accept(dbServer_sock.sck, (sockaddr*)&addr, &addrSize);

		recv(new_client.sck, buf, MAX_SIZE, 0);
		new_client.user = string(buf);
		string msg = "";
		send(new_client.sck, msg.c_str(), MAX_SIZE, 0);

		closesocket(new_client.sck);
	}
}
#pragma endregion


#endif // !__CHATTING_HELPER_H__
