#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <vector>
#include <string>
#include <map>
#include <arpa/inet.h> 
#include <iostream>
#include <fstream>
#include <sstream>

#define LENGTH 30000
#define MAX_PEND 3
int AcceptTCPConnection(int sock);
int CreateTCPServerSocket(int portNo);
void HandleTCPClient(int clntSocket);
void dieWithError(const char *err);
std::string htmlToString(std::string file_name);


struct Request
{
	enum e_type
	{
		GET,
		POST,
		DELETE,
	};
	e_type type;

};



struct Server
{
	unsigned short port;
	std::string root;
	std::vector<std::string> indexes;
};

struct webserv
{
	std::vector<Server> servers;

};


#endif