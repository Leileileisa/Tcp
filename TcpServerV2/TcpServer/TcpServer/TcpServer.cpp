// TcpServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using std::cerr;
using std::cout;
using std::endl;
const char DEFAULT_PORT[] = "4000";
const int RECV_BUF_SIZE = 256;
const size_t IP_BUF_SIZE = 65;
int main()
{
	WSADATA wSaData;
	int i_result = 0;
	SOCKET sock_server = INVALID_SOCKET;
	SOCKET sock_client = INVALID_SOCKET;
	addrinfo* result = nullptr;
	addrinfo hints;
	i_result = WSAStartup(MAKEWORD(2,2),&wSaData);
	if (i_result != 0) {
		cerr << "getaddrinfo() function failed with error" << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	SecureZeroMemory(&hints, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	i_result = getaddrinfo(nullptr,DEFAULT_PORT,&hints,&result);
	if (i_result != 0) {
		cerr << "getaddrinfo() function failed with error" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		return 1;
	}
	sock_server = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock_server == INVALID_SOCKET) {
		cerr << "socket() function failed with error:" << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		system("pause");
		return 1;
	}
	i_result = bind(sock_server, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (i_result == SOCKET_ERROR) {
		cerr << "bind()function failed with error:" << WSAGetLastError << endl;
		freeaddrinfo(result);
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 1;
	}
	freeaddrinfo(result);
	cout << "server started successfully" << endl;
	cout << "start listening" << endl;
	i_result = listen(sock_server, SOMAXCONN);
	if (i_result == SOCKET_ERROR) {
		cerr << "listen() function failed with error:" << WSAGetLastError() << endl;
		closesocket(sock_server);
		system("pause");
		return 1;
	}
	SOCKADDR_IN addr_client;
	int len_addr = sizeof(SOCKADDR_IN);
	char ip_buf[IP_BUF_SIZE];
	SecureZeroMemory(ip_buf,IP_BUF_SIZE);
	sock_client = accept(sock_server,(SOCKADDR*)&addr_client,&len_addr);
	if (sock_client == INVALID_SOCKET) {
		cerr << "accept() function failed with error:" << WSAGetLastError() << endl;
		closesocket(sock_server);
		WSACleanup();
		system("pause");
		return 1;
	}
	cout << "client connected.." << endl;
	inet_ntop(AF_INET, &addr_client, ip_buf, IP_BUF_SIZE);
	cout << "client ip address:" << ip_buf << endl;
	char recv_buf[RECV_BUF_SIZE];
	int send_result = 0;
	do {
		SecureZeroMemory(recv_buf,RECV_BUF_SIZE);
		i_result = recv(sock_client, recv_buf, RECV_BUF_SIZE, 0);
		if (i_result > 0) {
			if (strcmp(recv_buf, "exit") == 0) {
				cout << "client requests to close the connection" << endl;
				break;
			}
			cout << "Bytes received:" << i_result << endl;
			cout << "Message received:" << recv_buf << endl;
			send_result = send(sock_client, recv_buf, i_result, 0);
			if (send_result == SOCKET_ERROR) {
				cerr << "Send() function failed with error:" << WSAGetLastError() << endl;
				closesocket(sock_client);
				WSACleanup();
				system("pause");
				return 1;
			}
		}
		else if (i_result == 0) {
			cout << "connection closed" << endl;
		}
		else {
			cerr << "recv() function failed with error:" << WSAGetLastError() << endl;
			closesocket(sock_client);
			WSACleanup();
			system("pause");
			return 1;
		}
	} while (i_result>0);
	i_result = shutdown(sock_client, SD_SEND);
	if (i_result == SOCKET_ERROR) {
		cerr << "shutdown() function failed with error:" << WSAGetLastError() << endl;
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 1;
	}
	i_result = closesocket(sock_server);
	WSACleanup();
	cout << "socket closed" << endl;
	system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
