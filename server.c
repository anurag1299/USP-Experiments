#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int socketFD, newSocketFD, portNo, clientLen, n;
    char buffer[256];
    struct sockaddr_in server_addr, client_addr;

    if (argc < 2)
    {
        fprintf(stderr, "Error, no port provided\n");
        exit(1);
    }

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0)
    {
        error("Error opening socket");
    }
    bzero((char *)&server_addr, sizeof(server_addr));
    portNo = atoi(argv[1]);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNo);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(socketFD, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        error("Error in binding");
    }

    listen(socketFD, 5);

    clientLen = sizeof(client_addr);
    newSocketFD = accept(socketFD, (struct sockaddr *)&client_addr, &clientLen);
    if (newSocketFD < 0)
    {
        error("Error in accepting");
    }

    bzero(buffer, 256);
    n = read(newSocketFD, buffer, 256);
    if (n < 0)
    {
        error("Error in reading from socket");
    }
    printf("Message: %s\n", buffer);

    n = write(newSocketFD, "Message received", 16);
    if (n < 0)
    {
        error("Error in writing to socket");
    }

    return 0;
}
