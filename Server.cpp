#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <ctype.h>


#define PORT 8080


char * getPathOfGetRequest(const char *buf) 
{
	const char *start = buf;
	const char *end;
	char       *path=NULL;
	size_t      pathLen;

	/* Verify that there is a 'GET ' at the beginning of the string. */
	if(strncmp("GET ", start, 4))
	{
		fprintf(stderr, "Parse error: 'GET ' is missing.\n");
		goto CLEANUP;
	}

	/* Set the start pointer at the first character beyond the 'GET '. */
	start += 4;

	/* From the start position, set the end pointer to the first white-space character found in the string. */
	end=start;
	while(*end && !isspace(*end))
	++end;

	/* Calculate the path length, and allocate sufficient memory for the path plus string termination. */
	pathLen = (end - start);
	path = (char*)malloc(pathLen + 1);
	if(NULL == path)
	{
		fprintf(stderr, "malloc() failed. \n");
		goto CLEANUP;
	}

	/* Copy the path string to the path storage. */
	memcpy(path, start, pathLen);

	/* Terminate the string. */
	path[pathLen] = '\0';

	CLEANUP:

	/* Return the allocated storage, or NULL in the event of an error, to the caller. */
	return(path);

}

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
	char *filePath = getPathOfGetRequest(buffer);
        printf("\n----------Hello message sent----------\n");
	printf("%s\n",getPathOfGetRequest(buffer));
        close(new_socket);
}


int main(int argc, char const *argv[]) {

    // create socket
    int server_socket;

    //test message
    char *message = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 150\n\nHello World!";



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

            perror("I can't believe you've done this");
            return 1;

            } else {
		
		std::thread t1(doTheThing, new_socket, message);
	
		printf("\nthe thread shit worked, yo");

		t1.join();
	    }


    }


    return 0;
}


