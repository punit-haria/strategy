#ifndef GUARD_IPC
#define GUARD_IPC

#if defined(_WIN32) 
#include <WinSock2.h>
#endif

#include <stdio.h>

namespace msim{

	class ipc_server{ // transmitter
	public:
		ipc_server(){}
		ipc_server(const char* port);
		void wait_for_incoming();
		int send(const char*);
		int receive(char*);
		void close_client();
		~ipc_server();
	private:
#if defined(__GNUC__) 
		int sockfd, new_fd;
#elif defined(_WIN32) 
		SOCKET ListenSocket, ClientSocket;
#endif
		ipc_server(const ipc_server&);
		ipc_server& operator=(const ipc_server&);
	};

	inline int operator<<(ipc_server& server, const char* buf){
		return server.send(buf);
	}

	inline int operator>>(ipc_server& server, char* buf){
		return server.receive(buf);
	}

	class ipc_client{ // receiver
	public:
		ipc_client(){}
		ipc_client(const char* host, const char* port);
		int send(const char*);
		int receive(char*);
		void disconnect();
		~ipc_client();
	private:
#if defined(__GNUC__) 
		int sockfd;	
#elif defined(_WIN32) 
		SOCKET ConnectSocket;
#endif
		ipc_client(const ipc_client&);
		ipc_client& operator=(const ipc_client&);
	};
	
	inline int operator<<(ipc_client& client, const char* buf){
		return client.send(buf);
	}

	inline int operator>>(ipc_client& client, char* buf){
		return client.receive(buf);
	}
}

#endif // !GUARD_IPC
