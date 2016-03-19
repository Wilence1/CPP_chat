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
    
    set_nonblock(listener);
	
    if(bind(listener, (struct sockaddr *) &SockAddr, sizeof(SockAddr)) < 0)
	{
		printf("bind");
		exit(2);	
	}
   

	listen(listener, SOMAXCONN);
    
    int KQueue = kqueue();
    
    struct kevent KEvent;
    
    bzero(&KEvent, sizeof(KEvent));
    
    EV_SET(&KEvent, MasterSocket, EVFILT_READ, EV_ADD, 0, 0, 0);
    kevent(KQueue, &KEvent, 1, NULL, 0, NULL);
    
	while(1)
	{
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
			printf("%s\n", buf);
		}
		shutdown(sock, SHUT_RDWR);
		close(sock);
	}
	return 0;
}