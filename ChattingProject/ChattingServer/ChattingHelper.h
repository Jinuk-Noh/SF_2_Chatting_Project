#pragma once
#ifndef __CHATTING_HELPER_H__
#define __CHATTING_HELPER_H__

#pragma comment(lib, "ws2_32.lib")

#include "DBHelper.h"
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <sstream>

#define MAX_SIZE 1024
#define MAX_CLIENT 2

using std::cout;
using std::cin;
using std::endl;
using std::string;

struct SOCKET_INFO {
	SOCKET sck;
	string content;
};
std::vector<string> SplitComm(string comm);
#pragma region ChattingSocket
std::vector<SOCKET_INFO> sck_list;
SOCKET_INFO server_sock;
int client_cnt = 0;

string MakeNickName(std::vector<string> v) {

	return v[1] + "(" + v[0] + ")";
}

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

	server_sock.content = "Server";

	cout << "Server On" << endl;
}

void send_msg(const char* msg) {
	for (int i = 0; i < client_cnt; i++) {
		send(sck_list[i].sck, msg, MAX_SIZE, 0);
	}
}

void send_msg(const char* msg, string nickName) {
	for (int i = 0; i < client_cnt; i++) {
		if (sck_list[i].content == nickName) continue;
		
		send(sck_list[i].sck, msg, MAX_SIZE, 0);
	}
}

void send_chat_log(const char* msg, string nickName) {
	for (int i = 0; i < sck_list.size(); i++) {
		if (sck_list[i].content == nickName) {
			send(sck_list[i].sck, msg, MAX_SIZE, 0);
			break;
		}
	}
}

void recv_msg(int idx) {
	char buf[MAX_SIZE] = {};
	string msg = "";
	std::vector<string> v = SplitComm(sck_list[idx].content);
	string nickName = MakeNickName(v);

	while (1) {
		ZeroMemory(&buf, MAX_SIZE);
		if (recv(sck_list[idx].sck, buf, MAX_SIZE, 0) > 0) {
			std::vector<string> msgV = SplitComm(string(buf));
			
			msg = nickName + " : " + msgV[1];
			InsertChatLog(DBHelper::CreateInstance(), v[0].c_str(), msgV[1].c_str(), msgV[0].c_str());
			cout << msg << endl;
			send_msg(msg.c_str(), sck_list[idx].content);
		}
		else {
			msg = "[공지] " + nickName + " 님이 퇴장했습니다.";
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

	new_client.content = string(buf); // socket에 name(id) 형식으로 저장
	std::vector<string> v = SplitComm(new_client.content);
	string nickName = MakeNickName(v);

	string msg = "[공지] " + nickName + "님이 입장했습니다.";
	cout << msg << endl;
	sck_list.push_back(new_client);
	cout << "여기1" << endl;
	std::vector<string> chatLogV = GetChattingLog(DBHelper::CreateInstance());
	cout << "여기2" << endl;
	std::thread th(recv_msg, client_cnt);

	for (int i = 0; i < chatLogV.size(); i++) {
		send_chat_log(chatLogV.at(i).c_str(), new_client.content);
	}

	client_cnt++;
	cout << "[공지 현재 접속자 수 : " << client_cnt << "명" << endl;
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

			msg =  server_sock.content + " : " + buf;
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

	dbServer_sock.content = "dbServer";

	cout << "DB Server On" << endl;
}

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
			//회원가입
			new_client.content = string(buf);

			string comm = new_client.content;
			std::vector<string> v;

			string msg;

			if (comm.find("duple") == 0) {

				v = SplitComm(comm);

				msg = UserDupleCheck(DBHelper::CreateInstance(), v[1].c_str());

				send(new_client.sck, msg.c_str(), MAX_SIZE, 0);

			}
			else if (comm.find("login") == 0) {
				v = SplitComm(comm);
				msg = GetUserInfo(DBHelper::CreateInstance(), v[1].c_str(), v[2].c_str());

				send(new_client.sck, msg.c_str(), MAX_SIZE, 0);
			}
			else if (comm.find("upload") == 0) {
				v=SplitComm(comm);
				UploadSignUp(DBHelper::CreateInstance(), v[1].c_str(), v[2].c_str(), v[3].c_str());
			}
			//

			

			closesocket(new_client.sck);
		}
	}
	else {
		cout << "DB 소켓 종료. (Error code : " << code << ")";
	}

	WSACleanup();
}

std::vector<string> SplitComm(string comm) {
	std::vector<string> v;

	std::stringstream ss(comm);
	while (getline(ss, comm, '|')) {
		v.push_back(comm);
	}
	return v;
}
#pragma endregion
#endif // !__CHATTING_HELPER_H__