// TcpClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::flush;
const char DEFAULT_PORT[] = "4000";
const int SEND_BUF_SIZE = 256;
int main()
{
	WSADATA wSaData;
	int i_result = 0;
	SOCKET sock_client = INVALID_SOCKET;
	addrinfo* result = nullptr, hints;
	i_result = WSAStartup(MAKEWORD(2,2),&wSaData);
	if (i_result != 0) {
		cerr << "WSAStartUp() function failed" << i_result << endl;
		system("pause");
		return 1;
	}
	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	i_result = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (i_result != 0) {
		cerr << "getaddrinfo() function failed with error:" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 1;
	}
	sock_client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock_client == INVALID_SOCKET) {
		cerr << "socket()function failed with error:" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 1;
	}
	i_result = connect(sock_client, result->ai_addr, result->ai_addrlen);
	if (i_result == SOCKET_ERROR) {
		cerr << "connect() function failed with error:"<<WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 1;
	}
	cout << "connect server successfully" << endl;
	freeaddrinfo(result);
	char send_buf[SEND_BUF_SIZE];
	int recv_result = 0;
	do {
		cout << "enter the message that you want to send:" << flush;
		cin.getline(send_buf, SEND_BUF_SIZE);
		i_result = send(sock_client, send_buf, static_cast<int>(strlen(send_buf)), 0);
		if (i_result == SOCKET_ERROR) {
			cerr << "send() function failed with error:" << WSAGetLastError() << endl;
			closesocket(sock_client);
			WSACleanup();
			system("pause");
			return 1;
		}
		cout << "Bytes send:" << i_result << endl;
		recv_result = recv(sock_client, send_buf, SEND_BUF_SIZE, 0);
		if (recv_result > 0) {
			cout << "feedback from server" << send_buf << endl;
		}
		else if (recv_result==0) {
			cout << "connection closed" << endl;
		}
		else {
			cerr << "recv() function failed with error:" << WSAGetLastError() << endl;
			closesocket(sock_client);
			WSACleanup();
			system("pause");
			return 1;
		}
	} while (recv_result>0);
	i_result = shutdown(sock_client, SD_SEND);
	if (i_result == SOCKET_ERROR) {
		cerr << "shutdown() function failed with error:" << WSAGetLastError << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 1;
	}
	closesocket(sock_client);
	WSACleanup();
	cout << "socket closed" << endl;
	system("pause");
	return 0;

}

