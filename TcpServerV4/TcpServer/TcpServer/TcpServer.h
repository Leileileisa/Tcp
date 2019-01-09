#pragma once
#include<WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
using namespace std;
typedef socklen_t SOCKLEN_T;
typedef u_long U_LONG;
constexpr auto BUFFER_SIZE=4096;
constexpr USHORT PORT = 4000;
constexpr auto BUF_SIZE = 4096;
constexpr auto BACKLOG = 5;
struct PACKET_HEAD {
	int lenghth;
};
class Server {
private:
	SOCKADDR_IN server_addr;
	SOCKLEN_T server_addr_len;
	INT listen_fd;
	INT max_fd;
	FD_SET master_set;
	FD_SET working_set;
	TIMEVAL timeout;
public:
	//Server(INT port);
	//~Server();
	Server(const Server &) = delete;
	Server& operator=(const Server&) = delete;
};