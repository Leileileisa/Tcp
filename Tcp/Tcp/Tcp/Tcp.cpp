// Tcp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <WinSock2.h>
#include <thread>
#pragma comment( lib, "ws2_32.lib" )
int RunServer(int port) {
	WORD versionRequested;
	WSADATA saData;
	int err;
	versionRequested = MAKEWORD(1, 1);
	err = WSAStartup(versionRequested, &saData);

	if (err != 0) {
		return 0;
	}
	if (LOBYTE(saData.wVersion) != 1 || HIBYTE(saData.wVersion) != 1) {
		WSACleanup();
		return 0;
	}
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	listen(sockSrv, 5);
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	while (1) {
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		char sendBuf[50];
		sprintf_s(sendBuf, "Welcome %s to %d!", inet_ntoa(addrClient.sin_addr),port);
		send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);
		char recvBuf[50];
		recv(sockConn, recvBuf, 50, 0);
		printf("%s\n", recvBuf);

		closesocket(sockConn);
		Sleep(2000);
	}
	WSACleanup();
	return 0;
}
int main()
{
	for (int i = 4000; i < 4005; i++) {

	}
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
