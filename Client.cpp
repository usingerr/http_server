#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char const *argv[]) {

    int client_socket;
    const int PORT = 8080;

    if (client_socket = socket(AF_INET, SOCK_STREAM, 0) < 0) {
        printf("\n Socket? More like sucket \n");
        return -1;
    }

    struct sockaddr_in ServerAddress;
    char *hellwo = "Hello from the client";

    memset(&ServerAddress, '0', sizeof(ServerAddress));

    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PORT);

    //if (inet_pton(AF_INET, "127.0.0.1", &ServerAddress.sin_addr) <= 0) {

        //printf("\n This is not the address you're looking for *hand wave* \n");
        //return -1;
    //}

    if (connect(client_socket, (struct sockaddr *)&ServerAddress, sizeof(ServerAddress)) < 0) {
        printf("\n No connection for you! \n");
        return -1;
    }

    char ValueRead;
    char buffer[1024] = {0};
    send(client_socket, hellwo, strlen(hellwo), 0);
    printf("Hello message sent \n");
    ValueRead = read(client_socket, buffer, 1024);
    printf("%s\n", buffer);
    return 0;
}
