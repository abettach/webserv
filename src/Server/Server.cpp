#include "Server.hpp"
#include "../Cgi/cgi.hpp"
// Default constructor
Server::Server() : running(1),
				   _masterSockFD(0),
				   _port(0),
				   _host(""),
				   _addrLen(0),
				   _maxSockFD(0),
				   _isChunked(false),
				   _contentLength(0),
				   _portServer(0),
				   _mbs(0),
				   _isvalid(1)
{
}

Server::Server(std::vector<serverINFO> &servers) : running(1),
												   _servers(servers),
												   _masterSockFD(0),
												   _port(0),
												   _host(""),
												   _addrLen(0),
												   _maxSockFD(0),
												   _isChunked(false),
												   _contentLength(0),
												   _portServer(0),
												   _mbs(0),
												   _isvalid(1)
{
	// _servers.assign(parser.getServers().begin(), parser.getServers().end());
	this->makeSockets();
	this->waitingForConnections();
}

// Copy constructor
Server::Server(Server const &ths)
{
	*this = ths;
	return;
}

// Destructor
Server::~Server()
{
	for (std::vector<int>::iterator it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
	{
		close(*it);
	}
	_servers.clear();
	_masterSockFDs.clear();
	_ports.clear();
	_clients.clear();
	_accptMaster.clear();
}

// Assignment operator=
Server &Server::operator=(Server const &ths)
{
	if (this != &ths)
	{
		// this->_parser = ths._parser;
		this->_servers = ths._servers;
		this->_masterSockFDs = ths._masterSockFDs;
		this->_masterSockFD = ths._masterSockFD;
		this->_ports = ths._ports;
		this->_port = ths._port;
		this->_host = ths._host;
		this->_serverAddr = ths._serverAddr;
		this->_clientAddr = ths._clientAddr;
		this->_addrLen = ths._addrLen;
		this->_masterFDs = ths._masterFDs;
		this->_readFDs = ths._readFDs;
		this->_writeFDs = ths._writeFDs;
		this->_maxSockFD = ths._maxSockFD;
		this->_clients = ths._clients;
		this->_accptMaster = ths._accptMaster;
		// this->_request = ths._request;
		this->_isChunked = ths._isChunked;
		this->_contentLength = ths._contentLength;
		// this->_server = ths._server;
		this->_mbs = ths._mbs;
		this->_isvalid = ths._isvalid;
	}
	return *this;
}

void Server::makeSockets()
{
	// fd_set structures initializing
	FD_ZERO(&_masterFDs);
	FD_ZERO(&_writeFDs);
	FD_SET(STDIN_FILENO, &_masterFDs);
	// making master sockets for()
	for (std::vector<serverINFO>::iterator itServer = _servers.begin(); itServer != _servers.end(); itServer++)
	{
		_ports = itServer->getPorts();
		_host = itServer->getHost();
		std::cout << _ports[2] << std::endl;
		for (std::vector<int>::iterator itPort = _ports.begin(); itPort != _ports.end(); ++itPort)
		{
			_port = *itPort;
			try
			{

				// Socket creating
				this->createSocket();
				// Bind a name to a socket
				this->bindSocket();
				// Listen for socket connections
				this->listenSocket();
				std::cout << "New socket " + std::to_string(_masterSockFD) + " bind to " << _host << ':' << _port << std::endl;
			}
			catch (const std::exception &e)
			{
				close(_masterSockFD);
				std::cerr << e.what() << '\n';
			}
		}
	}
}

// Socket creating
void Server::createSocket()
{
	if ((_masterSockFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Unable to create a socket.");

	// set socket to non-blocking
	if (fcntl(_masterSockFD, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Unable to set the socket " + std::to_string(_masterSockFD) + " to non-blocking.");
	// set socket option of reusing address
	int opt = 1;
	if (setsockopt(_masterSockFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
		throw std::runtime_error("Unable to set socket option to the socket " + std::to_string(_masterSockFD));
}

// Socket binding
void Server::bindSocket()
{
	std::memset(&_serverAddr, 0, sizeof(_serverAddr));
	_addrLen = sizeof(_serverAddr);
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = (_host == "ANY") ? htonl(INADDR_ANY) : inet_addr(_host.c_str());
	if (bind(_masterSockFD, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1)
		throw std::runtime_error("Unable to bind the socket " + std::to_string(_masterSockFD) + " with " + _host + ":" + std::to_string(_port) + ": Already used");
}

// Listen for incoming connections from clients
void Server::listenSocket()
{
	if (listen(_masterSockFD, BACKLOG) == -1)
		throw std::runtime_error("Unable to listen for connections in the socket " + std::to_string(_masterSockFD));
	// set socket to fd_set struct
	FD_SET(_masterSockFD, &_masterFDs);
	_maxSockFD = (_masterSockFD > _maxSockFD) ? _masterSockFD : _maxSockFD;
	// Add the socket to the sockets vector
	_masterSockFDs.push_back(_masterSockFD);
}

void Server::waitingForConnections()
{
	int running = 1;
	std::cout << "\t<Server running... waiting for connections./>" << std::endl;
	while (running)
	{

		FD_ZERO(&_readFDs);
		_readFDs = _masterFDs;
		usleep(2000);
		struct timeval _tv = {1, 0};
		int activity = select(_maxSockFD + 1, &_readFDs, &_writeFDs, NULL, &_tv);
		if (activity == -1)
			throw std::runtime_error("Select failed to multiplexing Input/Output.");
		if (activity > 0)
		{
			if (FD_ISSET(0, &_readFDs))
			{ /* Check keyboard */
				std::cout << "Shutting down server gracefuly" << std::endl;
				// getchar();
				running = 0;
				for (std::vector<int>::iterator it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
				{
					close(*it);
				}
				FD_ZERO(&_masterFDs);
				FD_ZERO(&_readFDs);
				FD_ZERO(&_writeFDs);
				break;
			}
			for (int sockFD = 1; sockFD < _maxSockFD + 1; sockFD++)
			{
				if (FD_ISSET(sockFD, &_readFDs))
				{
					int newConnect = 0;
					for (std::vector<int>::iterator it = _masterSockFDs.begin(); it != _masterSockFDs.end(); it++)
					{
						if (sockFD == *it)
						{
							newConnect = 1;
							break;
						}
					}
					newConnect ? this->newConnectHandling(sockFD) : this->accptedConnectHandling(sockFD);
				}
			}
		}
	}
}

void Server::newConnectHandling(int &sockFD)
{
	std::cout << "new connection" << std::endl;

	int accptSockFD = accept(sockFD, (struct sockaddr *)&_clientAddr, &_addrLen);
	if (accptSockFD == -1)
		throw std::runtime_error("Unable to accept the connection from client by the socket " + std::to_string(accptSockFD));
	std::cout << "New connection: Master socket " << std::to_string(sockFD) << ". Accept socket " + std::to_string(accptSockFD) << ", address " << inet_ntoa(_clientAddr.sin_addr) << ":" << std::to_string(ntohs(_clientAddr.sin_port)) << std::endl;
	if (fcntl(accptSockFD, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Unable to set the socket " + std::to_string(accptSockFD) + " to non-blocking.");
	FD_SET(accptSockFD, &_masterFDs);
	FD_SET(accptSockFD, &_writeFDs);
	if (accptSockFD > _maxSockFD)
		_maxSockFD = accptSockFD;
	_clients.insert(std::pair<int, std::string>(accptSockFD, ""));
	std::map<int, int>::iterator it = _accptMaster.find(accptSockFD);
	if (it != _accptMaster.end())
		it->second = sockFD;
	else
		_accptMaster.insert(std::pair<int, int>(accptSockFD, sockFD));
}

// bool checkContent(std::string &buffer)
// {
// 	for (size_t i = 0; i < buffer.size(); i++)
// 	{
// 		if (buffer[i] != '\r' && buffer[i] != '\n')
// 			return true;
// 	}
// 	buffer.clear();
// 	return false;
// }

// bool Server::detectEndRequest(std::string &buffReq, int &accptSockFD)
// {
// 	getServerBySocket(accptSockFD, &_server, &_portServer);
// 	_mbs = _server.getClientMaxBodySize() * 1024 * 1024;
// 	if (!checkContent(buffReq))
// 		return false;
// 	else if (!(buffReq.find("\r\n\r\n") == std::string::npos))
// 	{
// 		std::string headers = buffReq.substr(0, buffReq.find("\r\n\r\n") + 4);
// 		if (headers.find("Transfer-Encoding: chunked") != std::string::npos)
// 		{
// 			_isChunked = true;
// 			if (buffReq.find("0\r\n\r\n") != std::string::npos)
// 				return true;
// 			else
// 				return false;
// 		}
// 		else if (headers.find("Content-Length") != std::string::npos)
// 		{
// 			try
// 			{
// 				size_t length = std::stoi(headers.substr(headers.find("Content-Length: ") + 16));
// 				std::string body = buffReq.substr(buffReq.find("\r\n\r\n") + 4);

// 				if (length > (size_t)_mbs)
// 					return true;
// 				else if (body.length() < length)
// 					return false;
// 			}
// 			catch (const std::exception &e)
// 			{
// 				std::cerr << e.what() << '\n';
// 			}
// 		}
// 		return true;
// 	}
// 	return false;
// }

void Server::accptedConnectHandling(int &accptSockFD)
{
	char _buffRes[BUFFER_SIZE + 1] = {0};
	bzero(_buffRes, sizeof(_buffRes));
	int valRead = recv(accptSockFD, _buffRes, BUFFER_SIZE, 0);
	// std::cout << _buffRes << std::endl;
	std::cout << "Activity in socket " << std::to_string(accptSockFD) << ", address: " << inet_ntoa(_clientAddr.sin_addr) << ':' << std::to_string(ntohs(_clientAddr.sin_port)) << std::endl;
	if (valRead > 0)
	{
		_buffRes[valRead] = '\0';
		std::map<int, std::string>::iterator it = _clients.find(accptSockFD);
		if (it != _clients.end())
			it->second += _buffRes;
		std::string req(_buffRes);
		_request.Request_start(req);
		if (FD_ISSET(accptSockFD, &_writeFDs))
		{
			this->responseHandling(accptSockFD);
		}
	}
	if (valRead == 0)
	{
		std::cout << "Client Disconnected socket: " << std::to_string(accptSockFD) << std::endl;
		close(accptSockFD);
		FD_CLR(accptSockFD, &_masterFDs);
		FD_CLR(accptSockFD, &_writeFDs);
		_clients.erase(accptSockFD);
	}
	else
		return; // Socket is connected but doesn't send request.
}

// bool checkChunkSize(std::string &size)
// {
// 	std::string cmp = "0123456789ABCDEFabcdef";
// 	for (size_t i = 0; i < size.size(); i++)
// 	{
// 		if (cmp.find(size[i]) == std::string::npos)
// 			return false;
// 	}
// 	return true;
// }

// size_t getChunkedDataSize(std::string &chunkSize)
// {
// 	chunkSize.pop_back();
// 	if (!checkChunkSize(chunkSize))
// 		throw std::runtime_error("Invalid character in chunk size");
// 	size_t size = 0;
// 	std::stringstream stream(chunkSize);
// 	stream >> std::hex >> size;
// 	return size;
// }

// std::string Server::unchunkingRequest(std::string &request)
// {
// 	std::string body = request.substr(request.find("\r\n\r\n") + 4);
// 	std::string unchunkedData = request.substr(0, request.find("\r\n\r\n") + 4);
// 	_contentLength = 0;
// 	for (size_t i = 0; i < body.size();)
// 	{
// 		std::string tmpBody = body.substr(i, std::string::npos);
// 		size_t chunkSize = 0;
// 		std::stringstream bodyStream(tmpBody);
// 		std::string line("");
// 		std::getline(bodyStream, line);
// 		i += line.length() + 1;
// 		// if (line.find("0") == std::string::npos)
// 		chunkSize = getChunkedDataSize(line);
// 		unchunkedData.append(body.c_str() + i, chunkSize);
// 		i += chunkSize + 2;
// 		_contentLength += chunkSize;
// 	}
// 	_isChunked = false;
// 	return unchunkedData;
// }

// void Server::getServerBySocket(int &accptSockFD, HttpServer *server, unsigned short *port)
// {
// 	std::map<int, int>::iterator it = _accptMaster.find(accptSockFD);
// 	struct sockaddr_in serverAddr;
// 	std::memset(&serverAddr, 0, _addrLen);
// 	if (getsockname(it->second, (struct sockaddr *)&serverAddr, &_addrLen) == -1)
// 		throw std::runtime_error("Unable to get server informations from socket " + std::to_string(it->second));
// 	for (std::vector<HttpServer>::iterator it = _servers.begin(); it != _servers.end(); it++)
// 	{
// 		if (it->getHost() == inet_ntoa(serverAddr.sin_addr))
// 		{
// 			_ports = it->getPorts();
// 			for (std::vector<unsigned short>::iterator itPort = _ports.begin(); itPort != _ports.end(); itPort++)
// 			{
// 				if (*itPort == ntohs(serverAddr.sin_port))
// 				{
// 					*server = *it;
// 					*port = *itPort;
// 					return;
// 				}
// 			}
// 		}
// 	}
// }

std::string Server::get_body(std::string file_name)
{
	std::string _body;
	std::ifstream file(file_name);
	if (file)
	{
		std::ostringstream ss;
		ss << file.rdbuf();
		_body = ss.str();
		file.close(); // close the file(filename)
	}
	return _body;
}

int Server::num_len(int n)
{
	int i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

char *Server::ft_itoa(int n)
{
	char *str;
	int numlen;
	unsigned int nb;

	numlen = num_len(n);
	nb = n;
	if (n < 0)
	{
		nb = -n;
		numlen++;
	}
	if (!(str = (char *)malloc(sizeof(char) * numlen + 1)))
		return (0);
	str[numlen] = '\0';
	str[--numlen] = nb % 10 + '0';
	while (nb /= 10)
		str[--numlen] = nb % 10 + '0';
	if (n < 0)
		*(str) = '-';
	return (str);
}

void Server::responseHandling(int &accptSockFD)
{
	// Response response(this->_request, _server, _portServer);
	std::string body;
	std::string path = _request.getTarget().erase(0, 1);
	// if (_request.getTarget().compare(0, _request.getTarget().size(), ""))
	// 	std::cout << _request.getPort() << std::endl;
	char *header = strdup("HTTP/1.1 200 OK\r\nContent-Length: ");
	// std::string cgi_path = "/Users/abettach/goinfre/.brew/bin/php-cgi";
    // std::string root = "/Users/abettach/Desktop/webserv";
    // CGI _cgi;
    // std::cout << _cgi.runCGI(_request, root, cgi_path) << std::endl;
	// body = _cgi.runCGI(_request, root, cgi_path).substr(64,_cgi.runCGI(_request, root, cgi_path).size());
	// std::cout << body << std::endl;
	Response _resp;
	_resp.creatResponse(this->_servers, this->_request);
	// if (path.size() == 0 || path == "/")
	// 	body = get_body("index.html");
	// else
	// 	body = get_body(path);
	std::string all = std::string(header) + std::string(ft_itoa(_resp.GetBody().size())) + "\r\n\r\n" + _resp.GetBody();

	if (FD_ISSET(accptSockFD, &_writeFDs))
	{
		if (send(accptSockFD, all.c_str(), all.length(), 0) != (ssize_t)all.length())
			throw std::runtime_error("Unable to send the response to client in socket " + std::to_string(accptSockFD));
		if (0) // if connection is set to close in request close
		{
			std::cout << "Disconnected socket: " << std::to_string(accptSockFD) << std::endl;
			close(accptSockFD);
			FD_CLR(accptSockFD, &_masterFDs);
			FD_CLR(accptSockFD, &_writeFDs);
		}
		// _request.clearRequest();
		// response.clearAll();
	}
}