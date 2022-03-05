#include "webserv.hpp"



int main(int argc, char *argv[])
{
	std::vector<int> servSock; /* Socket descriptors for server */
	int maxDescriptor;		   /* Maximum socket descriptor value */
	fd_set sockSet;			   /* Set of socket descriptors for select() */
	long timeout;			   /* Timeout value given on command-line */
	struct timeval selTimeout; /* Timeout for select() */
	int running = 1;		   /* 1 if server should be running; 0 otherwise */
	int noPorts;			   /* Number of port specified on command-line */
	int port;				   /* Looping variable for ports */
	unsigned short portNo;	   /* Actual port number */
	int ret;

	if (argc < 3)
		dieWithError("Usage: Webserver <Timeout (secs.)> <Port 1> ...\n");

	timeout = atol(argv[1]); /* First arg: Timeout */
	noPorts = argc - 2;

	servSock.reserve(noPorts); /* Allocate list of sockets for incoming connections */
	maxDescriptor = -1;		   /* Initialize maxDescriptorfor use by select() */
	std::string body = htmlToString(std::string("index.html"));
	// std::cout << body << std::endl;
	for (port = 0; port < noPorts; port++)
	{ /* Create list of ports and sockets to handle ports */
		portNo = atoi(argv[port + 2]);
		servSock[port] = CreateTCPServerSocket(portNo); /* Create port socket */
		maxDescriptor = servSock[port];
	}

	printf("Startingserver: Hit return to shutdown\n");
	while (running)
	{
		/* Zero socket descriptor vector and set for server sockets */
		/* This must be reset every time select() is called */
		FD_ZERO(&sockSet);
		FD_SET(STDIN_FILENO, &sockSet); /* Add keyboard to descriptor vector */
		for (port = 0; port < noPorts; port++)
			FD_SET(servSock[port], &sockSet);
		/* Timeout specification */
		/* This must be reset every time select() is called */
		selTimeout.tv_sec = timeout; /* timeout (secs.) */
		selTimeout.tv_usec = 0;		 /* 0 microseconds */
		/* Suspend program until descriptor is ready or timeout */
		ret = select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout);
		if (ret == -1)
			dieWithError("couldn't select socket");
		else if (ret == 0)
			std::cout << "No echo requests for " << timeout << "secs...Server still alive" << std::endl;
		else
		{
			if (FD_ISSET(0, &sockSet))
			{ /* Check keyboard */
				std::cout << "Shuttingdown server" << std::endl;
				// getchar();
				running = 0;
				for (port = 0; port < noPorts; port++)
					close(servSock[port]);
				FD_ZERO(&sockSet);
			}
			for (port = 0; port < noPorts; port++)
				if (FD_ISSET(servSock[port], &sockSet))
				{
					std::cout << "Request on Port: " << argv[port + 1] << std::endl;
					HandleTCPClient(AcceptTCPConnection(servSock[port]));
				}
		}
	}
	for (port = 0; port < noPorts; port++)
		close(servSock[port]);
	FD_ZERO(&sockSet);
	/* Close sockets */
	exit(0);
}
