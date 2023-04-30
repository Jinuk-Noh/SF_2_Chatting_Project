#pragma once
#ifndef __CHATTING_HELPER_H__
#define __CHATTING_HELPER_H__

#pragma comment(lib, "ws2_32.lib")

#include "DBHelper.h"
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <sstream>

#define MAX_SIZE 1024
#define MAX_CLIENT 1

using std::cout;
using std::cin;
using std::endl;
using std::string;

struct SOCKET_INFO {
	SOCKET sck;
	string content;
};
#pragma region ChattingSocket
std::vector<SOCKET_INFO> sck_list;
SOCKET_INFO server_sock;
int client_cnt = 0;

void del_client(int idx) {
	closesocket(sck_list[idx].sck);
	client_cnt--;
}

void server_init() {
	server_sock.sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN server_addr = {};
	server_addr.sin_family = PF_INET;
	server_addr.sin_port = htons(7718);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(server_sock.sck, (sockaddr*)&server_addr, sizeof(server_addr));
	listen(server_sock.sck, SOMAXCONN);

	server_sock.content = "server";

	cout << "Server On" << endl;
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
			msg = sck_list[idx].content + " : " + buf;
			cout << msg << endl;
			send_msg(msg.c_str());
		}
		else {
			msg = "[����] " + sck_list[idx].content + " ���� �����߽��ϴ�.";
			cout << msg << endl;
			send_msg(msg.c_str());
			del_client(idx);

			return;
		}
	}
}

void add_client() {
	SOCKADDR_IN addr = {};
	int addrSize = sizeof(addr);
	char buf[MAX_SIZE] = {};

	ZeroMemory(&addr, addrSize);

	SOCKET_INFO new_client = {};

	new_client.sck = accept(server_sock.sck, (sockaddr*)&addr, &addrSize);
	recv(new_client.sck, buf, MAX_SIZE, 0);
	new_client.content = string(buf);

	string msg = "[����] " + new_client.content + "���� �����߽��ϴ�.";
	cout << msg << endl;
	sck_list.push_back(new_client);

	std::thread th(recv_msg, client_cnt);

	client_cnt++;
	cout << "[���� ���� ������ �� : " << client_cnt << "��" << endl;
	send_msg(msg.c_str());

	th.join();
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
			string text, msg = "";

			std::getline(cin, text);
			const char* buf = text.c_str();
			msg = server_sock.content + " : " + buf;
			send_msg(msg.c_str());
		}

		for (int i = 0; i < MAX_CLIENT; i++) {
			th1[i].join();
		}

		closesocket(server_sock.sck);
		
	}
	else {
		cout << "���α׷� ����. (Error code : " << code << ")";
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

	dbServer_sock.content = "dbServer";

	//cout << "DB Server On" << endl; //Ȯ�� ��
}

string GetUser(string comm, std::vector<string>* v);

void RunDB() {

	WSADATA wsa;

	int code = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (!code) {
		db_server_init();

		while (1) {
			SOCKADDR_IN addr = {};
			int addrSize = sizeof(addr);
			char buf[MAX_SIZE] = {};

			ZeroMemory(&addr, addrSize);

			SOCKET_INFO new_client = {};

			new_client.sck = accept(dbServer_sock.sck, (sockaddr*)&addr, &addrSize);

			recv(new_client.sck, buf, MAX_SIZE, 0);

			new_client.content = string(buf);

			//�α���
			string comm = buf;
			
			std::vector<string> v ;

			string msg;

			if (comm.find("login") == 0) {
				msg = GetUser(comm, &v);
			}

			//string msg = msg = id + "|" + pw;;
			//�α��� ��
			send(new_client.sck, msg.c_str(), MAX_SIZE, 0);

			closesocket(new_client.sck);
		}
	}
	else {
		cout << "DB ���� ����. (Error code : " << code << ")";
	}

	//cout << "���� ��� ����"<< endl;
	WSACleanup();
}

string GetUser(string comm, std::vector<string>* v) {
	std::stringstream ss(comm);
	while (getline(ss, comm, '|')) {
		v->push_back(comm);
		//cout << v[v.size() -1] << endl;  // Ȯ�ο�
	}

	return GetUserInfo(DBHelper::CreateInstance(), v->at(1).c_str(), v->at(2).c_str());
}

#pragma endregion
#endif // !__CHATTING_HELPER_H__
