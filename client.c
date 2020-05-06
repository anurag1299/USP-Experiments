#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char const *argv[])
{
    int socketFD, portNo, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3)
    {
        fprintf(stderr, "use as follow:\n%s hostname port\n", argv[0]);
        exit(0);
    }

    portNo = atoi(argv[2]);
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0)
    {
        error("Error in opening socket");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error,No such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portNo);
    if (connect(socketFD, &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error connecting");
    }

    printf("Enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 256, stdin);
    n = write(socketFD, buffer, strlen(buffer));
    if (n < 0)
    {
        error("Error in writing to socket");
    }
    bzero(buffer, 256);
    n = read(socketFD, buffer, 255);
    if (n < 0)
    {
        error("Error in reading from socket");
    }
    printf("%s\n", buffer);

    return 0;
}
