// TcpServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include"server.h"
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using std::cerr;
using std::cout;
using std::endl;

constexpr auto SERVER_PORT = 4000;
constexpr auto MSG_BUF_SIZE = 1024;
Server::Server() {
	cout << "Initializing Server.." << endl;
	winsock_ver = MAKEWORD(2, 2);
	addr_len = sizeof(SOCKADDR_IN);
	addr_svr.sin_family = AF_INET;
	addr_svr.sin_port = htons(SERVER_PORT);
	addr_svr.sin_addr.S_un.S_addr = ADDR_ANY;
	memset(buf_ip,0,IP_BUF_SIZE);
	ret_val = WSAStartup(winsock_ver, &wsa_data);
	if (ret_val != 0) {

	}
	cout << "WSA started up successfully" << endl;
	sock_svr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_svr == INVALID_SOCKET) {

	}
	cout << "Server socket created successfully" << endl;
	ret_val = bind(sock_svr, (SOCKADDR*)&addr_svr, addr_len);
	if (ret_val != 0) {

	}
	cout << "Server socket bound successfully" << endl;
	ret_val = listen(sock_svr,SOMAXCONN);
	if (ret_val == SOCKET_ERROR) {

	}
	cout << "Server socket started to listen.." << endl;
	cout << "Server started successfully" << endl;
}
Server::~Server() {
	closesocket(sock_svr);
	closesocket(sock_clt);
	WSACleanup();
	cout << "Socket closed" << endl;
}
DWORD WINAPI CreateClientThread(LPVOID lpParameter);
void Server::WaitForClient() {
	while (true) {
		sock_clt = accept(sock_svr,(SOCKADDR*)&addr_clt,&addr_len);
		if (sock_clt == INVALID_SOCKET) {
			
		}
		inet_ntop(addr_clt.sin_family, &addr_clt, buf_ip, IP_BUF_SIZE);
		cout << "A new client connected..Ip address::" << buf_ip << ",port number:" << ntohs(addr_clt.sin_port)<<endl;
		h_thread = CreateThread(nullptr, 0, CreateClientThread, (LPVOID)sock_clt, 0, nullptr);
		if (h_thread == NULL) {

		}
		CloseHandle(h_thread);
	}
}
DWORD WINAPI CreateClientThread(LPVOID lpParameter) {
	SOCKET sock_clt = (SOCKET)lpParameter;
	char buf_msg[MSG_BUF_SIZE];
	int ret_val=0;
	int snd_result=0;
	do {
		memset(buf_msg, 0, MSG_BUF_SIZE);
		ret_val = recv(sock_clt, buf_msg, MSG_BUF_SIZE, 0);
		if (ret_val > 0) {
			if (strcmp(buf_msg, "exit") == 0) {
				cout << "client requests to close the connection" << endl;
				break;
			}
			cout << "Message received:" << buf_msg << endl;
			snd_result = send(sock_clt, buf_msg, MSG_BUF_SIZE, 0);
			if (snd_result == SOCKET_ERROR) {

			}

		}
		else if(ret_val==0){
			cout << "connection closed" << endl;
		}
		else {

		}
	} while (ret_val>0);
	ret_val = shutdown(sock_clt, SD_SEND);
	if (ret_val == SOCKET_ERROR) {

	}
	return 0;
}
int main()
{
	Server svr;
	svr.WaitForClient();
	system("pause");
	return 0;
}

