#if defined(__GNUC__) 

#include <stdlib.h>  // exit
#include <unistd.h>  // close
#include <string.h>  // memset
#include <netdb.h>  // getaddrinfo
#include <sys/socket.h>
#include <arpa/inet.h>

#include <ipc.h>

namespace msim{ 
	int send_all(int sockfd, const char* buf){
		size_t total = 0;
		size_t len = sizeof buf;
		size_t bytesleft = len;
		int n;

		while(total < len){
			if((n = ::send(sockfd, buf + total, bytesleft, 0)) == -1)
				break;
			total += n;
			bytesleft -= n;                        
		}

		return n == -1 ? -1 : total; // actual data sent
	}

	int receive_new(int sockfd, char* buf){
		return ::recv(sockfd, buf, sizeof buf, 0);
	}
}

namespace msim{

	// ipc_client

	ipc_client::ipc_client(const char* host, const char* port) : sockfd(-1){
		struct addrinfo hints;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		int rv;
		struct addrinfo *servinfo;
		if((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0){
			// throw exception
			printf("getaddrinfo exception\n");
			exit(1);
		}

		// connect to first available result
		struct addrinfo* p;
		for(p = servinfo; p != NULL; p = p->ai_next){
			if((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1){
					continue;                                
			}

			if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
				disconnect();
				continue;                            
			}
			break;
		}

		if(p == NULL){
			freeaddrinfo(servinfo);
			// throw exception
			printf("servinfo list exception\n");
			exit(1);
		}

		freeaddrinfo(servinfo);
	}

	int ipc_client::receive(char* buf){
		int numbytes;
		if((numbytes = receive_new(sockfd, buf)) == -1){
			disconnect();
			// throw exception
			printf("recv exception\n");
		}
		return numbytes;
	}

	int ipc_client::send(const char* buf){
		return send_all(sockfd, buf);
	}

	void ipc_client::disconnect(){
		if(sockfd != -1){
			close(sockfd);
			sockfd = -1;
		}
	}

	ipc_client::~ipc_client(){
		disconnect();
	}



	// ipc_server

	ipc_server::ipc_server(const char* port) : sockfd(-1), new_fd(-1){
		struct addrinfo hints;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE; //using current IP

		int rv;
		struct addrinfo *servinfo;
		if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0){
			// throw exception
			printf("getaddrinfo exception\n");
			exit(1);
		}

		// bind to first available result
		int yes = 1;
		struct addrinfo *p;
		for (p = servinfo; p != NULL; p = p->ai_next){
			if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1){
					continue;
			}
			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1){
					freeaddrinfo(servinfo);
					// throw exception 
					printf("setsockopt exception\n");
					exit(1);
			}
			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
				close(sockfd);
				continue;
			}
			break;
		}

		if (p == NULL){
			freeaddrinfo(servinfo);
			// throw exception
			printf("servinfo list exception\n");
			exit(1);
		}

		freeaddrinfo(servinfo);
	}

	void ipc_server::wait_for_incoming(){
		if (listen(sockfd, 1) == -1){
			// throw exception 
			printf("listen exception\n");
			close(sockfd);			
			exit(1);
		}

		// wait for and accept connection
		socklen_t sin_size;
		struct sockaddr_storage their_addr;		
		sin_size = sizeof their_addr;
		if ((new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size))
			== -1){
				// throw exception
				printf("accept exception\n");
				close(sockfd);				
				exit(1);
		}

		close(sockfd);
		sockfd = -1;
	}

	ipc_server::~ipc_server(){
		if(sockfd != -1)
			close(sockfd);
		close_client();
	}

	int ipc_server::send(const char* buf){
		return send_all(new_fd, buf);
	}

	int ipc_server::receive(char* buf){
		int numbytes;
		if((numbytes = receive_new(new_fd, buf)) == -1){
			close_client();
			// throw exception
			printf("recv exception\n");
		}
		return numbytes;
	}

	void ipc_server::close_client(){
		if(new_fd != -1){
			close(new_fd);
			new_fd = -1;
		}
	}

}

#endif
