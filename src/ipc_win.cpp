#if defined(_WIN32) 

#include <ipc.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

namespace msim{
	inline int receive_new(SOCKET sock, char* buf){ 
		return ::recv(sock, buf, sizeof buf, 0); // blocks until there is new data to be received
	}

	int send_all(SOCKET sock, const char* buf){
		size_t total = 0;
		size_t len = sizeof buf;
		size_t bytesleft = len;
		int iResult;

		while(total < len){
			iResult = ::send(sock, buf + total, bytesleft, 0); // blocks if outbound traffic is clogged
			if(iResult == SOCKET_ERROR){
				printf("send failed: %d\n", WSAGetLastError());
				break;
			}
			total += iResult;
			bytesleft -= iResult;
		}

		return iResult == SOCKET_ERROR ? iResult : total; // actual data sent
	}
}

namespace msim{

	// client

	ipc_client::ipc_client(const char* host, const char* port) : 
		ConnectSocket(INVALID_SOCKET) {
			WSADATA wsaData;
			int iResult;

			// start-up winsock
			iResult	= WSAStartup(MAKEWORD(2,2), &wsaData);
			if(iResult != 0){
				printf("WSAStartup failed: %d\n", iResult);
				exit(1);
				// throw exception
			}

			struct addrinfo *result = NULL, hints;
			ZeroMemory( &hints, sizeof(hints) );
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM; 
			hints.ai_protocol = IPPROTO_TCP;

			// getaddrinfo
			iResult = getaddrinfo(host, port, &hints, &result);
			if(iResult != 0){
				printf("getaddrinfo failed: %d\n", iResult);
				WSACleanup();
				exit(1);
				// throw exception
			}

			// use first available result
			struct addrinfo *ptr = NULL;
			for(ptr = result; ptr != NULL; ptr = ptr->ai_next){
				// socket
				ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if(ConnectSocket == INVALID_SOCKET){
					printf("Error at socket(): %ld\n", WSAGetLastError());
					continue;
				}
				// connect
				iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
				if(iResult == SOCKET_ERROR){
					closesocket(ConnectSocket);
					ConnectSocket = INVALID_SOCKET;
					continue;
				}
				break;
			}

			freeaddrinfo(result);

			if(ConnectSocket == INVALID_SOCKET){
				printf("Unable to connect to server\n");
				WSACleanup();
				exit(1);
				// throw exception
			}
	}

	int ipc_client::receive(char* buf){
		int numbytes;
		numbytes = receive_new(ConnectSocket, buf);
		if(numbytes == SOCKET_ERROR){
			disconnect();			
			// throw exception
			printf("recv exception\n");
		}
		return numbytes;
	}

	int ipc_client::send(const char* buf){
		int iResult = send_all(ConnectSocket, buf);
		if(iResult == SOCKET_ERROR){
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
		}
		return iResult;
	}

	void ipc_client::disconnect(){
		if(ConnectSocket != INVALID_SOCKET){
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}
	}

	ipc_client::~ipc_client(){
		disconnect();
		WSACleanup();
	}


	// server

	ipc_server::ipc_server(const char* port) : ListenSocket(INVALID_SOCKET),
		ClientSocket(INVALID_SOCKET) {
			WSADATA wsaData;
			int iResult;

			// start-up winsock
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0){
				printf("WSAStartup failed: %d\n", iResult);
				exit(1);
				// throw exception
			}

			struct addrinfo *result = NULL, hints;
			ZeroMemory(&hints, sizeof hints);
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE; // use current IP address

			// resolve local address and port to be used by the server
			iResult = getaddrinfo(NULL, port, &hints, &result);
			if (iResult != 0){
				printf("getaddrinfo failed: %d\n", iResult);
				WSACleanup();
				exit(1);
				// throw exception
			}

			// use first available result
			struct addrinfo *ptr = NULL;
			for(ptr = result; ptr != NULL; ptr = ptr->ai_next){
				// listen
				ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
				if (ListenSocket == INVALID_SOCKET){
					printf("error at socket(): %ld\n", WSAGetLastError());
					continue;
				}
				// bind
				iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
				if (iResult == SOCKET_ERROR){
					printf("bind failed with error: %d\n", WSAGetLastError());
					closesocket(ListenSocket);
					continue;
				}
				break;
			}

			freeaddrinfo(result);

			if(ListenSocket == INVALID_SOCKET){
				printf("Unable to setup server\n");
				WSACleanup();
				exit(1);
				// throw exception
			}
	}

	void ipc_server::wait_for_incoming(){
		// listen
		if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR){
			printf("listen failed with error %ld\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			exit(1);
			// throw exception
		}

		// wait for and accept connection
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET){
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			exit(1);
		}

		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
	}

	ipc_server::~ipc_server(){
		if(ListenSocket != INVALID_SOCKET)
			closesocket(ListenSocket);
		close_client();
	}

	int ipc_server::send(const char* buf){
		int iResult = send_all(ClientSocket, buf);
		if(iResult == SOCKET_ERROR){
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
		}
		return iResult;
	}

	int ipc_server::receive(char* buf){
		int numbytes;
		numbytes = receive_new(ClientSocket, buf);
		if(numbytes == SOCKET_ERROR){
			close_client();			
			// throw exception
			printf("recv exception\n");
		}
		return numbytes;
	}

	void ipc_server::close_client(){
		if(ClientSocket != INVALID_SOCKET){
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
		}
	}

}

#endif
