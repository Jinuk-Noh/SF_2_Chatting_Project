#pragma once
#ifndef  __CHATTING_H__
#define __CHATTING_H__

#include <iostream>
#include "Sck.h"
#include "User.h"
#include <thread>
#include <ctime>


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
            //std::stringstream ss(msg);  // ���ڿ��� ��Ʈ��ȭ
            //string userId;
            //ss >> userId; // ��Ʈ���� ����, ���ڿ��� ���� �и��� ������ �Ҵ�
            //if (userId != userInfo.id) cout << buf << endl; // ���� ���� �� �ƴ� ��쿡�� ����ϵ���.
            cout << buf << endl;
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
        int tryCnt = 0;
        while (1) {
            if (!connect(chat_sock, (SOCKADDR*)&client_addr, sizeof(client_addr))) {
                cout << "Server Connect" << endl;
                string msg = (userInfo.id + "|" + userInfo.name);
                send(chat_sock, msg.c_str(), msg.length(), 0);
                system("cls");
                break;
            }
            cout << "Connecting..." << endl;
            if (tryCnt > 5) {
                cout << "������ ������ �߻��Ͽ����ϴ�. ��õ� ���ֽʽÿ�." << endl;
                return 0;
            }

            tryCnt++;
        }

        std::thread th(chat_recv);
        
        cin.ignore();
        while (1) {
            string text;
           
            std::getline(cin, text);

            time_t now;
            struct tm tt;
            char nowDate[256];
            time(&now);
            localtime_s(&tt, &now);
            strftime(nowDate, sizeof(nowDate), "%Y-%m-%d %H:%M:%S", &tt);

            string strNowDate = nowDate;

            string msg = strNowDate+"|"+text;
            const char* buffer = msg.c_str(); // string���� char* Ÿ������ ��ȯ
            send(chat_sock, buffer, strlen(buffer), 0);
        }

        th.join();
        closesocket(chat_sock);
    }

    WSACleanup();
    return 0;
}


#endif // ! __CHATTING_H__
