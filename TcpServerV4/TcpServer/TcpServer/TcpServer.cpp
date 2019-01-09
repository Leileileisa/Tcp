// TcpServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "TcpServer.h"
#include<WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
using namespace std;
//Server::Server(int port) {
//	FD_ZERO(&server_addr, sizeof(server_addr));
//}
INT main()
{
	WSAData wSaData;
	if (WSAStartup(MAKEWORD(2, 2), &wSaData) != 0) {
		cout << "initialize fail" << WSAGetLastError()<<endl;
		return 0;
	}
	SOCKET tcpListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	U_LONG uLong = 1;
	ioctlsocket(tcpListen, FIONBIO, &uLong);
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.S_un.S_addr = ADDR_ANY;
	if (bind(tcpListen, (SOCKADDR*)&sin, sizeof(sin))) {
		cout << "bind fail" << WSAGetLastError() << endl;
		return 0;
	}
	listen(tcpListen, BACKLOG);
	FD_SET socketSet;
	FD_ZERO(&socketSet);
	FD_SET(tcpListen, &socketSet);
	TIMEVAL time = { 1,0 };
	CHAR buf[BUFFER_SIZE];
	FD_SET readSet;
	FD_ZERO(&readSet);
	FD_SET writeSet;
	FD_ZERO(&writeSet);
	while (true) {
		readSet = socketSet;
		writeSet = socketSet;
		INT nRetAll = select(0, &readSet, &writeSet, NULL, NULL);
		if (nRetAll > 0) {
			if (FD_ISSET(tcpListen, &readSet)) {
				if (socketSet.fd_count < FD_SETSIZE) {
					SOCKADDR_IN addrRemote;
					INT nAddrLen = sizeof(addrRemote);
					SOCKET sClient = accept(tcpListen, (SOCKADDR*)&addrRemote, &nAddrLen);
					if (sClient != INVALID_SOCKET) {
						FD_SET(sClient, &socketSet);
						cout << "accept success" << inet_ntoa(addrRemote.sin_addr) << endl;
					}
				}
				else {
					cout << "max df_size!" << endl;
					continue;
				}
			}
			for (int i = 0; i < socketSet.fd_count; i++) {
				if (FD_ISSET(socketSet.fd_array[i], &readSet)) {
					INT nRecv = recv(socketSet.fd_array[i], buf, BUFFER_SIZE, 0);
					if (nRecv > 0) {
						buf[nRecv] = 0;
						cout << socketSet.fd_array[i] << "," << buf << endl;
					}
				}
				if (FD_ISSET(socketSet.fd_array[i], &writeSet)) {
					CHAR buf[] = "hello";
					INT nRet = send(socketSet.fd_array[i], buf, strlen(buf) + 1, 0);
					if (nRet < 0) {
						if (GetLastError() == WSAEWOULDBLOCK) {

						}
						else {
							closesocket(socketSet.fd_array[i]);
							FD_CLR(socketSet.fd_array[i], &socketSet);
						}
					}
					else {
						cout << "send hello!" << endl;
					}
				}
			}
		}
		else if(nRetAll==0){
			cout << "time out!" << endl;
		}
		else {
			cout << "select error!" << WSAGetLastError() << endl;
			break;
		}
		Sleep(1000);
	}
	closesocket(tcpListen);
	WSACleanup();
}


