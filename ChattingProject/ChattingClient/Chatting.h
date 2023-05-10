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
            //std::stringstream ss(msg);  // 문자열을 스트림화
            //string userId;
            //ss >> userId; // 스트림을 통해, 문자열을 공백 분리해 변수에 할당
            //if (userId != userInfo.id) cout << buf << endl; // 내가 보낸 게 아닐 경우에만 출력하도록.
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
                cout << "서버에 오류가 발생하였습니다. 재시도 해주십시오." << endl;
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
            const char* buffer = msg.c_str(); // string형을 char* 타입으로 변환
            send(chat_sock, buffer, strlen(buffer), 0);
        }

        th.join();
        closesocket(chat_sock);
    }

    WSACleanup();
    return 0;
}


#endif // ! __CHATTING_H__
