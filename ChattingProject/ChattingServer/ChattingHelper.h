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
#define MAX_CLIENT 12

using std::cout;
using std::cin;
using std::endl;
using std::string;

struct SOCKET_INFO {
	SOCKET sck;
	string content;

	bool operator ==(SOCKET_INFO input) {
		return sck == input.sck && content == input.content;
	}
};

std::vector<string> SplitComm(string comm);


#pragma region ChattingSocket
void server_init();

void send_msg(const char* msg);
void send_msg(const char* msg, string nickName);
void send_chat_log(const char* msg, SOCKET sck);

void recv_msg(int idx, int thIdx, SOCKET_INFO sck);
void add_client(int thIdx);
void del_client(SOCKET_INFO sck, int thIdx);

void CheckThread();

string MakeNickName(std::vector<string> v);

std::vector<SOCKET_INFO> sck_list;
SOCKET_INFO server_sock;
int client_cnt = 0;

std::map<int, std::thread> dicTh;
std::vector<int> deletedThreadIdx;

string MakeNickName(std::vector<string> v) {
	return v[1] + "(" + v[0] + ")";
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

//전체 메시지
void send_msg(const char* msg) {
	for (int i = 0; i < client_cnt; i++) {
		send(sck_list[i].sck, msg, MAX_SIZE, 0);
	}
}

//수신자 제외 다른 사용자에게 채팅 전달
void send_msg(const char* msg, string nickName) {
	for (int i = 0; i < client_cnt; i++) {
		if (sck_list[i].content == nickName) continue;
		
		send(sck_list[i].sck, msg, MAX_SIZE, 0);
	}
}

//이전 채팅을 Client로 Send
void send_chat_log(const char* msg, SOCKET sck) {
	send(sck, msg, MAX_SIZE, 0);
}

void recv_msg(int idx, int thIdx, SOCKET_INFO sck) {
	char buf[MAX_SIZE] = {};
	string msg = "";
	std::vector<string> v = SplitComm(sck.content);
	string nickName = MakeNickName(v);
	while (1) {
		ZeroMemory(&buf, MAX_SIZE);
		if (recv(sck.sck, buf, MAX_SIZE, 0) > 0) {
			std::vector<string> msgV = SplitComm(string(buf));
			
			msg = nickName + " : " + msgV[1];
			InsertChatLog(DBHelper::CreateInstance(), v[0].c_str(), msgV[1].c_str(), msgV[0].c_str());
			cout << msg << endl;
			send_msg(msg.c_str(), sck.content);
		}
		else {
			msg = "[공지] " + nickName + " 님이 퇴장했습니다.";
			cout << msg << endl;
			send_msg(msg.c_str());
			del_client(sck, thIdx);

			return;
		}
	}
}

void add_client(int thIdx) {
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
	std::vector<string> chatLogV = GetChattingLog(DBHelper::CreateInstance());
	std::thread th(recv_msg, client_cnt, thIdx, new_client);

	for (int i = 0; i < chatLogV.size(); i++) {
		send_chat_log(chatLogV.at(i).c_str(), new_client.sck);
	}

	client_cnt++;
	cout << "[공지 현재 접속자 수 : " << client_cnt << "명" << endl;
	send_msg(msg.c_str());

	th.join();
}

void del_client(SOCKET_INFO sck, int thIdx) {
	//closesocket(sck_list[idx].sck);
	closesocket(sck.sck);
	client_cnt--;
	sck_list.erase(remove(sck_list.begin(), sck_list.end(), sck), sck_list.end());

	deletedThreadIdx.push_back(thIdx);
}

void CheckThread() {
	while (1) {
		std::vector<int> v = deletedThreadIdx;
		if (v.size() > 0 ) {
			for (auto i : v) {
				if (dicTh[i].joinable()) {
					dicTh[i].join();
					dicTh[i] = std::thread(add_client, i);
				}
			}

			//for (int i = 0; i < v.size(); i++) {
			//	deletedThreadIdx.erase(deletedThreadIdx.begin());
			//}
		}
	}
}

void Run() {
	WSADATA wsa;

	int code = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (!code) {
		server_init();
		
		for (int i = 0; i < MAX_CLIENT; i++) {
			dicTh.insert(make_pair(i, std::thread(add_client, i)));
		}

		std::thread th = std::thread(CheckThread);
		th.detach();

		while (1) {
			string text, msg = "";
			std::getline(cin, text);
			const char* buf = text.c_str();

			msg =  server_sock.content + " : " + buf;
			send_msg(msg.c_str());
		}

		if (dicTh.size() > 0) {
			for (auto i = dicTh.begin(); i != dicTh.end(); i++) {
				if (i->second.joinable()) {
					i->second.join();
				}
			}
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