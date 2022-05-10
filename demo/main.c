#include <stdio.h>
#include <string.h>
#include "server.h"
#include  <signal.h>

//ideas
// fox_settings settings = calloc(1, sizeof(fox_settings));
// settings->port = "8080";
/**
 * settings:
 *  - port: alapbol NULL
 */
// fox_add_endpoint("/app/get/", FOX_GET, &myFunc);
// fox_start_server(settings);

typedef struct html_response{
    char* version;
    char* server_type;
    char* date;
    char* content_type;
    char* connection_type;
    char* content;
    char* full_response;
}html_response;

void  INThandler(int sig){
    char  c;
    signal(sig, SIG_IGN);
    printf("Do you really want to quit? [y/n] ");
    c = getchar();
    if (c == 'y' || c == 'Y'){
        WSACleanup();
        exit(0);
    }
    else{
        signal(SIGINT, INThandler);
    }
    getchar(); // Get new line character
}

char* fox_readFromFile(char* filename){
    FILE* fin;
    fin = fopen(filename, "r");
    if (!fin) {
        perror("File open Error!\n\n");
        exit(EXIT_FAILURE);
    }
    char* line = (char*)calloc(500,sizeof(char));
    char* input_buffer;
    input_buffer = (char*)calloc(20000,sizeof(char));

    while ((fscanf(fin, "%[^\n]", line))!= EOF)
    {
        fgetc(fin); // Reads in '\n' character and moves file
        // stream past delimiting character
        //printf("Line = %s \n", line);
        strcat(input_buffer,line);
        line = (char*)calloc(500,sizeof(char));     //emptying the buffer
    }
    fclose(fin);
    //whitespace removal
    //----------------------
    // To keep track of non-space character count
    int count = 0;
    // Traverse the given string. If current character
    // is not space, then place it at index count
    /*for (int i = 0; input_buffer[i]; i++)
        if (input_buffer[i] != ' ')
            input_buffer[count++] = input_buffer[i]; // increment count
    input_buffer[count] = '\0';*/

    //returning the final string
    //----------------------
    return input_buffer;
}

int fox_launch(){
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %ld\n", iResult);
        return 1;
    }
    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
    SOCKET ListenSocket;
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    struct sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(80);

    if (bind(ListenSocket,
             (SOCKADDR *) & service, sizeof (service)) == SOCKET_ERROR) {
        wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    //----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen(ListenSocket, 1) == SOCKET_ERROR) {
        wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    int numberOfConnections = 1;

    signal(SIGINT, INThandler);
    html_response FoxResponse;
    while(1){
        //----------------------
        // Create a SOCKET for accepting incoming requests.
        SOCKET AcceptSocket;
        wprintf(L"Waiting for client to connect...\n");

        //----------------------
        // Accept the connection.
        AcceptSocket = accept(ListenSocket, NULL, NULL);
        if (AcceptSocket == INVALID_SOCKET) {
            wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        } else{
            wprintf(L"Client connected.\n");
            char buffer[30001];
            recv(AcceptSocket,buffer,30000,0);
            char hello[20000];

            //setting up data about version
            FoxResponse.version = (char*) calloc(strlen("HTTP/1.1 200 OK GMT\n"),sizeof(char));
            memcpy(FoxResponse.version, "HTTP/1.1 200 OK GMT\n", strlen("HTTP/1.1 200 OK GMT\n") + 1);

            //setting up data about server type
            FoxResponse.server_type = (char*) calloc(strlen("Server: Apache/2.2.14 (Win32)\n"),sizeof(char));
            memcpy(FoxResponse.server_type, "Server: Apache/2.2.14 (Win32)\n", strlen("Server: Apache/2.2.14 (Win32)\n") + 1);

            //setting up data about date
            FoxResponse.date= (char*) calloc(strlen("Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n"),sizeof(char));
            memcpy(FoxResponse.date, "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n", strlen("Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n") + 1);

            //setting up data about content type
            FoxResponse.content_type = (char*) calloc(strlen("Content-Type: text/html\n"),sizeof(char));
            memcpy(FoxResponse.content_type, "Content-Type: text/html\n", strlen("Content-Type: text/html\n") + 1);

            //setting up data about connection type
            FoxResponse.connection_type = (char*) calloc(strlen("Connection: Closed\n"),sizeof(char));
            memcpy(FoxResponse.connection_type, "Connection: Closed\n", strlen("Connection: Closed\n") + 1);

            //setting up data about content
            char* contentFromInput = (char*)calloc(20000,sizeof(char));
            strcat(contentFromInput,"\n");  //IMPORTANT: it needs a separating \n
            strcat(contentFromInput,fox_readFromFile("../demo/src/index.html"));

            FoxResponse.content = (char*) calloc(strlen(contentFromInput),sizeof(char));
            memcpy(FoxResponse.content, contentFromInput, strlen(contentFromInput) + 1);

            //allocating memory for full response
            FoxResponse.full_response = (char*)calloc((
                    strlen(FoxResponse.version) + strlen(FoxResponse.server_type)+
                    strlen(FoxResponse.date) + strlen(FoxResponse.content_type) +
                    strlen(FoxResponse.connection_type) +
                    strlen(FoxResponse.content) + 1
                    ),sizeof(char));

            //creating full response string
            strcat(FoxResponse.full_response,FoxResponse.version);
            strcat(FoxResponse.full_response,FoxResponse.server_type);
            strcat(FoxResponse.full_response,FoxResponse.date);
            strcat(FoxResponse.full_response,FoxResponse.content_type);
            strcat(FoxResponse.full_response,FoxResponse.connection_type);
            strcat(FoxResponse.full_response,FoxResponse.content);
            //sprintf(hello,"HTTP/1.1 200 OK GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Hello, Fox %i!</h1></body></html>", numberOfConnections++);
            sprintf(hello,FoxResponse.full_response, numberOfConnections++);
            printf("\nSending:\n%s\n", hello);
            send(AcceptSocket, hello, (int)strlen(hello), 0 );
        }
        closesocket(AcceptSocket);
    }
}


int main() {
    fox_launch();
}
