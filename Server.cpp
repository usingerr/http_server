#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <ctype.h>
#include <fstream>

using namespace std;

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
	start += 5;

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

	//printf("\n%s 6911420", path);

	/* Return the allocated storage, or NULL in the event of an error, to the caller. */
	return(path);

}

void doTheThing(int new_socket /*char *message*/) {

	char *message = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 150\n\nbig dicks";
	char ValueRead;
	char *filePath;
	char buffer[50000] = {0};
	ValueRead = read(new_socket, buffer, 50000);
	printf("\nValueRead has been assigned\n");
	printf("%s\n", buffer);
	filePath = getPathOfGetRequest(buffer);
	printf("\nfilepath has been assigned: %s\n", filePath);
	std::string filePathString (filePath);
	
	FILE *file = fopen(filePath, "rb");
	if (file == NULL) {
		
		printf("\nwe didn't do it, reddit\n");
		FILE *fileOhfour = fopen("fourOhfour.html", "rb");
		fseek(fileOhfour, 0, SEEK_END);
		long fsizeOhfour = ftell(fileOhfour);
		fseek(fileOhfour, 0, SEEK_SET);
		char *stringOhfour = (char *)malloc(fsizeOhfour + 1);
		fread(stringOhfour, fsizeOhfour, 1, fileOhfour);
		fclose(fileOhfour);
		write(new_socket, stringOhfour, strlen(stringOhfour));

	} else {
	
		printf("\nwe did it, reddit\n");
		printf("the filepath variable works: %s", filePath);
		FILE *file = fopen(filePath, "rb");
		fseek(file, 0, SEEK_END);
		long fsize = ftell(file);
		fseek(file, 0, SEEK_SET);
		char *string = (char *)malloc(fsize + 1);
		fread(string, fsize, 1, file);
		fclose(file);
		write(new_socket, string, strlen(string));

	}
	
	printf("\npage has been written to");
        printf("\n----------Hello message sent----------\n");

        close(new_socket);
}


int main(int argc, char const *argv[]) {

    // create socket
    int server_socket;

    //test message
    //char *message = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 150\n\nHello World!";



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
		
		std::thread t1(doTheThing, new_socket /*message*/);
	
		printf("\nthe thread shit worked, yo");

		t1.join();
	    }


    }


    return 0;
}


