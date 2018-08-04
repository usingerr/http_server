#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080

void doTheThing(int new_socket, char *message) {

	char ValueRead;
	printf("\nValueRead instantiated");
	int socket = new_socket;
	printf("\nnew socket made");
	char buffer[30000] = {0};
	printf("\nbuffer made");
	ValueRead = read(new_socket, buffer, 30000);
	printf("\nValueRead has been read\n");
	printf("%s\n", buffer);
	//if statements for message, based on the path
	write(new_socket, message, strlen(message));
	printf("\npage has been written to");
	printf("\n----------Hello message sent----------");
	close(new_socket);
	printf("\nsocket closed\n");


}

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

    if (bind(server_socket, (struct sockaddr *)&SocketAddressIn, sizeof(SocketAddressIn)) < 0) {

        perror("gotta get grack on that bind");
        return 0;

    }

    if (listen(server_socket, 10) < 0) {

        perror("I can't heeeaarr yoooouuuu");
        return 1;

    }

    int client_socket;
    int AddressLength = sizeof(SocketAddressIn);
    char ValueRead;

    while (true) {
	
	printf("\n++++++++++ Waiting for connection ++++++++++\n\n");

	if ((client_socket = accept(server_socket, (struct sockaddr *)&SocketAddressIn, (socklen_t*)&AddressLength)) < 0) {

		perror("I can't believe you've done this");
		return 1;

		} else {

			std::thread t1(doTheThing, client_socket, message);

			printf("\nthe thread shit worked, yo");

			t1.join();	

    	}
    
    }
	return 0;
}

