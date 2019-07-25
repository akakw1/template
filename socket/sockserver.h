#ifndef __SERVER_H
#define __SERVER_H

#include <winsock2.h>
#include <windows.h>
//#pragma comment(lib,"ws2_32.lib")

namespace socket_server {
	const int ListenSize = 5;
	static int PORT = 80;
	const int MSGSIZE = 1024;
	SOCKET slisten;
	sockaddr_in sin;
	
	void init() {
		WSADATA wsadata;
		WORD version = MAKEWORD(2, 2);
		if(WSAStartup(version, &wsadata) != 0) throw "Init Failed";
		slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(slisten == INVALID_SOCKET) throw "SOCKET Error";
		sin.sin_family=AF_INET;
		sin.sin_port=htons(PORT);
		sin.sin_addr.S_un.S_addr=INADDR_ANY; 
	}
	void bind() {
		if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) throw "Bind Error";
		if(listen(slisten, ListenSize) == SOCKET_ERROR) throw "Listen Error";
	}
	void close() {
		closesocket(slisten);
		WSACleanup();
	}
	void accept(SOCKET &sClient, sockaddr_in &remoteAddr) {
		int nAddrlen = sizeof(remoteAddr);
		sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
		if(sClient == INVALID_SOCKET) throw "Accept Error";
	}
	void sendmsg(SOCKET target, const char *p) {
		send(target, p, strlen(p), 0);
	}
	void recvmsg(SOCKET target, char *p) {
		int len;
		len = recv(target, p, MSGSIZE, 0);
		if(len <= 0) throw "Connection Is Broken";
		p[len] = '\0';
	}
}

#endif
