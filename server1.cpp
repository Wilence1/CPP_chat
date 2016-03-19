#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>


#include <errno.h>
#include <string.h>
int set_nonblock(int fd)
{
	int flags;
	#if defined(O_NONBLOCK)
	if (-1 == (flags = fcntl(fd, F_GETFL, 0))) flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
	#endif
}
int main()
{
	int sock, listener;
	struct sockaddr_in SockAddr;
	char buf[1024];
	int bytes_read;

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
	{
		printf("socket");
		exit(1);
	}
	//set_nonblock(listener);
	memset(&SockAddr, 0, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(3100);
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listener, (struct sockaddr *) &SockAddr, sizeof(SockAddr)) < 0) 
	{
		printf("bind");
		exit(2);	
	}
	listen(listener, SOMAXCONN);

	int kq;
	struct kevent evSet;
	kq = kqueue();
	memset(&evSet, sizeof(evSet));
	EV_SET(&evSet, listener, EVFILT_READ, EV_ADD, 0, 0, 0);
	if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
    	err(1, "kevent");

    struct kevent evList[32];
    int neww;
	while(1)
	{
		neww = kevent(kq, NULL, 0, evList, 32, NULL);
		if(neww < 1)
		{
			printf("neww");
			exit(4);
		}
		/*for(int i = 0; i < neww; i++)
		{

		}*/
		struct sockaddr_in Addr;
		socklen_t socklen;
		

		sock = accept(listener, (struct sockaddr *) &Addr , &socklen);
		if(sock < 0)
		{
			printf("accept");
			exit(3);
		}
		while(1)
		{
			bytes_read = recv(sock, buf, 1024, 0);
			if(bytes_read <= 0) break;
			//send(sock, buf, bytes_read, 0);
			printf("%s", buf);
		}
		shutdown(sock, SHUT_RDWR);
		close(sock);
	}
	return 0;
}