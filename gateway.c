//
// Created by bravo8234 on 11/07/2022.
//
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define PORT 9999;
int main(int argc, char* argv[])
{
    struct {u_long num;}msg;
    int port = 0;
    if(argc < 2)
    {
        perror("not enough parameters");
        exit(1);
    }
    else if(argc > 2)// didnt get port;
    {
        port = strtol(argv[2],NULL,10);
    }
    else
    {
       port = PORT;
    }
    struct hostent *hostnet = gethostbyname(argv[1]);
    int sock_send = socket(AF_INET,SOCK_DGRAM,0);//create udp socket

    struct sockaddr_in dest;//set up socket for sending
    bzero((char *)&dest, sizeof (dest));
    dest.sin_family = AF_INET;
    dest.sin_port = port +1;
    bcopy(hostnet->h_addr, (char *)&dest.sin_addr,hostnet->h_length);
    printf("port for sending: %d\n",dest.sin_port);

    struct sockaddr_in listen,recv;
    int sock_recv = socket(AF_INET,SOCK_DGRAM,0); //set up socket for listening
    bzero((char *) &listen, sizeof(listen));
    listen.sin_family = AF_INET;
    listen.sin_port = port;
    listen.sin_addr.s_addr = htonl(INADDR_ANY);//recives from every internal interface
    printf("port for listening %d\n",listen.sin_port);
    srandom(7846985);

    bind(sock_recv,(struct sockaddr *)&listen,sizeof(listen));//bind socket and address to listen
    for(;;)
    {
        float result = ((float)random())/((float)RAND_MAX);// generate random number
        int fsize = sizeof(recv);
        int cc = recvfrom(sock_recv,&msg,sizeof(msg),0,(struct sockaddr *)&recv,&fsize);//recvive messege from socket
        printf("Got data ::%ld\n",msg.num);// prints the process id gotten from the socket
        if(result > 0.5)
        {
            printf("sending %ld\n",msg.num);
            sendto(sock_send,&msg,sizeof(msg),0,(struct sockaddr*)&dest,sizeof (dest));//send data
        }

        fflush(stdout);//writes to or updates the standard output
    }

}