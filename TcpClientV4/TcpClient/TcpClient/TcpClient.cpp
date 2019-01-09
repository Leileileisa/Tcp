// TcpClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<WINSOCK2.H>
#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <string>
using namespace std;
constexpr auto BUF_SIZE = 64;
#pragma comment(lib,"WS2_32.lib")
void recv(PVOID pt) {
	SOCKET sHost = *((SOCKET*)pt);
	while (true) {
		char buf[BUF_SIZE];
		memset(buf, 0, BUF_SIZE);
		int retval = recv(sHost, buf, sizeof(buf), 0);
		if (retval == SOCKET_ERROR) {
			int err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK) {
				Sleep(1000);
				cout << "waiting reply!" << endl;
				continue;
			}
			else if (err == WSAETIMEDOUT || err == WSAENETDOWN || err == WSAECONNRESET) {
				cout << "recv failed" << endl;
				closesocket(sHost);
				WSACleanup();
				return;
			}
		}
		Sleep(1000);
		cout << buf << endl;
	}
}
int main()
{
	WSADATA wSaData;
	SOCKET sHost;
	SOCKADDR_IN servAddr;
	int retVal;
	char buf[BUF_SIZE];
	if (WSAStartup(MAKEWORD(2, 2), &wSaData) != 0) {
		cout << "WSAStartup fail" << endl;
		return -1;
	}
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(4000);
	int sServerAddlen = sizeof(servAddr);
	int iMode = 1;
	retVal = ioctlsocket(sHost, FIONBIO, (u_long FAR*)&iMode);
	if (retVal == SOCKET_ERROR) {
		cout << "ioctlsocket fail" << endl;
		WSACleanup();
		return 0;
	}
	while (true) {
		retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
		if (retVal == SOCKET_ERROR) {
			INT err = WSAGetLastError();
			if (err == WSAEWOULDBLOCK || err == WSAEINVAL) {
				Sleep(1);
				cout << "check connect" << endl;
				continue;
			}
			else if (err == WSAEISCONN) {
				break;
			}
			else {
				cout<<"connection failed";
				closesocket(sHost);
				WSACleanup();
				return 0;
			}
		}
	}
	u_long threadId = _beginthread(recv, 0, &sHost);
	while (true) {
		cout << "input a string to send" << endl;
		string str;
		cin >> str;
		ZeroMemory(buf, BUF_SIZE);
		strcpy(buf, str.c_str());
		if (strcmp(buf, "quit") == 0) {
			cout << "quit" << endl;
			break;
		}
		while (true){
			retVal = send(sHost, buf, strlen(buf), 0);
			if (SOCKET_ERROR == retVal) {
				INT err = WSAGetLastError();
				if (err == WSAEWOULDBLOCK) {
					Sleep(5);
					continue;
				}
				else {
					cout << "send fail" << endl;
					closesocket(sHost);
					WSACleanup();
					return 0;
				}
			}
			break;
		}
	}
	return 0;
}


