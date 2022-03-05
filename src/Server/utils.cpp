#include "webserv.hpp"

void HandleTCPClient(int clntSocket)
{
	// std::string body = std::string("HTTP/1.1 200 OK\r\n") + std::string("Content-Type: image/gif\r\n") + std::string("Content-Length: [") + std::to_string(imageSize) + std::string("]\r\n") + std::string("\r\n") + ;

	// std::string header = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 5\n\r\n\r\nhello";
	char echoBuffer[LENGTH]; /* Buffer for echo string */
	int recvMsgSize;		 /* Size of received message */
	// std::cout << body << std::endl;
	int valread = read(clntSocket, echoBuffer, 30000);
	std::cout << echoBuffer << std::endl;
	close(clntSocket); /* Close client socket */
					   // }
}

void dieWithError(const char *err)
{
	perror(err);
	exit(EXIT_FAILURE);
}

std::string htmlToString(std::string file_name)
{

	std::ifstream file(file_name.c_str()); // c_string because in c++98 iftream dont take a string file name but take cont char *

	std::stringstream string;
	string << file.rdbuf();
	std::string result = string.str();
	file.close(); // close the file(filename)
	return result;
}

int CreateTCPServerSocket(int portNo)
{
	int server_fd;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		dieWithError("couldn't create socket");
	int opt = 1;
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(portNo);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		dieWithError("bind failed");

	if (listen(server_fd, MAX_PEND) < 0)
		dieWithError("In listen");

	return server_fd;
}

int AcceptTCPConnection(int sock)
{

	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int new_socket;
	if ((new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		dieWithError("In listen");
	return new_socket;
}
