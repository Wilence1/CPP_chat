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

char message[] = "Hello there!\n";
char buf[sizeof(message)];

int main()
{
    int sock;
    struct sockaddr_in SockAddr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        printf("soket");
        exit(1);
    }

    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(3100); 
    SockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&SockAddr, sizeof(SockAddr)) < 0)
    {
        printf("socket");
        exit(2);
    }

    send(sock, message, sizeof(message), 0);
    recv(sock, buf, sizeof(message), 0);

    printf("%s", buf);
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return 0;
}