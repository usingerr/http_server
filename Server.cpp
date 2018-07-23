#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main(int argc, char const *argv[]) {

    // create socket
    //int server_socket = socket(domain, type, protocol);
    int server_socket;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {

        perror("cannot create socket");
        return 0;

    }

    //identify/name the socket
    struct sockaddr_in SocketAddressIn;

    SocketAddressIn.sin_family = AF_INET;
    SocketAddressIn.sin_addr.s_addr = INADDR_ANY;
    SocketAddressIn.sin_port = htons(PORT);

    memset((char *)&SocketAddressIn, 0, sizeof(SocketAddressIn));

    if (bind(server_socket, (struct sockaddr *)&SocketAddressIn, sizeof(SocketAddressIn)) < 0) {

        perror("gotta get grack on that bind");
        return 0;

    }

    if (listen(server_socket, 8) < 0) {

        perror("I can't heeeaarr yoooouuuu");
        return 1;

    }

    char *message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";

    int new_socket;
    int addrlen = sizeof(SocketAddressIn);
    char ValueRead;

    while (true) {

        printf("\n++++++++++ Waiting for connection ++++++++++\n\n");

        if ((new_socket = accept(server_socket, (struct sockaddr *)&SocketAddressIn, (socklen_t*)&addrlen)) < 0) {

            perror("I can't accept this");
            return 1;

            }

        char buffer[30000] = {0};
        ValueRead = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);
        write(new_socket, message, strlen(message));
        printf("----------Hello message sent----------");
        close(new_socket);

    }

    return 0;
}

