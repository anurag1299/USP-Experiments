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

void talkToClient(int, struct sockaddr_in);

int main(int argc, char const *argv[])
{
    int socketFD, newSocketFD, portNo, clientLen, n, pid;
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
    while (1)
    {
        newSocketFD = accept(socketFD, (struct sockaddr *)&client_addr, &clientLen);
        if (newSocketFD < 0)
        {
            error("Error in accepting");
        }
        pid = fork();
        if (pid < 0)
        {
            error("Error in fork");
        }
        if (pid == 0)
        {
            close(socketFD);
            talkToClient(newSocketFD, client_addr);
            exit(0);
        }
        else
        {
            close(newSocketFD);
        }
    }
    return 0;
}
void talkToClient(int newSocket, struct sockaddr_in client_addr)
{
    int n;
    char buffer[256];
    bzero(buffer, 256);
    n = read(newSocket, buffer, 256);
    if (n < 0)
    {
        error("Error in reading from socket");
    }
    printf("Message from %d: %s\n", client_addr.sin_addr.s_addr, buffer);

    n = write(newSocket, "Message received", 16);
    if (n < 0)
    {
        error("Error in writing to socket");
    }
}
