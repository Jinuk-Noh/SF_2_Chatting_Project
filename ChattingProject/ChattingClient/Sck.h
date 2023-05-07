#pragma once
#ifndef __Sck_H__
#define __SckDB_H__

#define MAX_SIZE 1024
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h> //Winsock 헤더파일 include. WSADATA 들어있음.ㄴ
#include <WS2tcpip.h>
#include <iostream>

using namespace std;

SOCKET db_sock;
SOCKET chat_sock;
SOCKADDR_IN ConnectDBSck() {
	db_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN client_addr = {};
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(7720);
	InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

	return client_addr;
}

SOCKADDR_IN ConnectChattSck() {
	SOCKADDR_IN client_addr = {};
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(7718);
	InetPton(AF_INET, TEXT("127.0.0.1"), &client_addr.sin_addr);

	return client_addr;
}

void SendComm(SOCKADDR_IN sock_addr, string comm) {
	while (1) {
		if (!connect(db_sock, (SOCKADDR*)&sock_addr, sizeof(sock_addr))) {
			//cout << "Server Connect" << endl;
			send(db_sock, comm.c_str(), comm.length(), 0);
			break;
		}
		//cout << "Test - Connecting..." << endl;
	}
}

vector<string> SplitComm(string comm) {
	vector<string> v;

	std::stringstream ss(comm);
	while (getline(ss, comm, '|')) {
		v.push_back(comm);
	}
	
	return v;
}


#endif // !__SckDB_H__

