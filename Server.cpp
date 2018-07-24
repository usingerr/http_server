#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main(int argc, char const *argv[]) {

    // create socket
    int server_socket;

    //test message
    char *message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";


    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {

        perror("cannot create socket");
        return 0;

    }

    //identify/name the socket
    struct sockaddr_in SocketAddressIn;

    SocketAddressIn.sin_family = AF_INET;
    SocketAddressIn.sin_addr.s_addr = INADDR_ANY;
    SocketAddressIn.sin_port = htons(PORT);

    //memset(server_socket, '\0', sizeof(SocketAddressIn));

    if (bind(server_socket, (struct sockaddr *)&SocketAddressIn, sizeof(SocketAddressIn)) < 0) {

        perror("gotta get grack on that bind");
        return 0;

    }

    if (listen(server_socket, 10) < 0) {

        perror("I can't heeeaarr yoooouuuu");
        return 1;

    }

    int new_socket;
    int AddressLength = sizeof(SocketAddressIn);
    char ValueRead;

    while (true) {

        printf("\n++++++++++ Waiting for connection ++++++++++\n\n");

        if ((new_socket = accept(server_socket, (struct sockaddr *)&SocketAddressIn, (socklen_t*)&AddressLength)) < 0) {

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

