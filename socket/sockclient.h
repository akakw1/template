#ifndef __CLIENT_H
#define __CLIENT_H

#include <winsock2.h>
#include <windows.h>
//#pragma comment(lib,"ws2_32.lib")

namespace socket_client {
	static char HOST[20] = "111.67.196.232";
	static int PORT = 10256;
	const int MSGSIZE = 1024;
	
	SOCKET sclient;
	sockaddr_in sin;
	
	void init() {
		WSADATA wsadata;
		WORD version = MAKEWORD(2, 2);
		if(WSAStartup(version, &wsadata) != 0) throw "Init Failed";
		sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(sclient == INVALID_SOCKET) throw "SOCKET Error";
		sin.sin_family = AF_INET;
		sin.sin_port = htons(PORT);
		sin.sin_addr.S_un.S_addr = inet_addr(HOST); 
	}
	void close() {
		closesocket(sclient);
		WSACleanup();
	}
	void connect() {
		if(connect(sclient, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
			throw "Connection Failed";
	}
	void sendmsg(const char* p) {
		send(sclient, p, strlen(p), 0);
	}
	void recvmsg(char* p) {
		int len;
		len = recv(sclient, p, MSGSIZE, 0);
		if(len <= 0) throw "Connection Is Broken";
		p[len] = '\0';
	}
}

#endif
