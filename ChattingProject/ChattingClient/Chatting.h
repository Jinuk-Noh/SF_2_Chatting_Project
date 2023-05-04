#pragma once
#ifndef  __CHATTING_H__
#define __CHATTING_H__

#include <iostream>
#include "Sck.h"
#include "User.h"

#define MAX_SIZE 1024

using std::cout;
using std::cin;
using std::endl;
using std::string;

int chat_recv() {
    char buf[MAX_SIZE] = { };
    string msg;

    while (1) {
        ZeroMemory(&buf, MAX_SIZE);
        if (recv(chat_sock, buf, MAX_SIZE, 0) > 0) {
            msg = buf;
            std::stringstream ss(msg);  // ���ڿ��� ��Ʈ��ȭ
            string user;
            ss >> user; // ��Ʈ���� ����, ���ڿ��� ���� �и��� ������ �Ҵ�
            if (user != userInfo.name) cout << buf << endl; // ���� ���� �� �ƴ� ��쿡�� ����ϵ���.
        }
        else {
            cout << "Server Off" << endl;
            return -1;
        }
    }
}

int Chatting() {
    WSADATA wsa;

    int code = WSAStartup(MAKEWORD(2, 2), &wsa);

    if (!code) {
        chat_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // 

        SOCKADDR_IN client_addr = ConnectChattSck();

        while (1) {
            if (!connect(chat_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
                cout << "Server Connect" << endl;
                send(chat_sock, userInfo.name.c_str(), userInfo.name.length(), 0);
                break;
            }
            cout << "Connecting..." << endl;
        }

        std::thread th(chat_recv);

        while (1) {
            string text;
            std::getline(cin, text);
            const char* buffer = text.c_str(); // string���� char* Ÿ������ ��ȯ
            send(chat_sock, buffer, strlen(buffer), 0);
        }

        th.join();
        closesocket(chat_sock);
    }

    WSACleanup();
    return 0;
}


#endif // ! __CHATTING_H__
