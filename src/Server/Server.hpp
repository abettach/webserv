#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <algorithm>
#include <vector>
#include <map>
#include "../Config/FileParss.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
class serverINFO;
class Request;
#define BUFFER_SIZE 1024
#define BACKLOG 2048

class Server
{
private:
	std::vector<serverINFO> _servers;
	Request _request;
	// Making sockets
	int _masterSockFD;
	std::vector<int> _masterSockFDs;
	std::vector<int> _ports;
	int _port;
	std::string _host;

	// Socket infos
	struct sockaddr_in _serverAddr;
	struct sockaddr_in _clientAddr;
	socklen_t _addrLen;

	// fd_set structures select()
	fd_set _masterFDs;
	fd_set _readFDs;
	fd_set _writeFDs;

	// Max of fds
	int _maxSockFD;

	// Clients sockets data request will hold by second element
	std::map<int, std::string> _clients;

	// first is accept socket ; second is master socket
	std::map<int, int> _accptMaster;

	// HttpServer _server;
	int _isvalid;

public:
	Server();
	Server(std::vector<serverINFO> &);
	Server(Server const &);
	~Server();
	Server &operator=(const Server &);
	std::string get_body(std::string file_name);
	char *ft_itoa(int n);
	static int num_len(int n);
	void makeSockets();
	void createSocket();
	void bindSocket();
	void listenSocket();
	void waitingForConnections();
	void newConnectHandling(int &);
	void accptedConnectHandling(int &);
	void responseHandling(int &);
};

#endif
