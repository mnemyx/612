/**
    C++ client example using sockets
    http://www.binarytides.com/code-a-simple-socket-client-class-in-c/
    
    Modified - GBG 9/2013 - CpSc6120
**/


#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent

/**
    TCP Client class
**/

class tcp_client {
	private:
		int sock;
		std::string address;
		int port;
		struct sockaddr_in server;
		 
	public:
		tcp_client();
		bool conn(std::string port, int address);
		bool send_data(std::string data);
		std::string receive(int size=512);
};

#endif
