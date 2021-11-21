#include "headers.hpp"
#include "parssingConfigFile/../inc/ConfigFilePars.hpp"

static int		num_len(int n)
{
	int	i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}

char			*ft_itoa(int n)
{
	char			*str;
	int				numlen;
	unsigned int	nb;

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

std::string get_body(std::string file_name)
{
    std::ifstream file(file_name.c_str());
    std::stringstream string;
    string << file.rdbuf();
    std::string string_two = string.str();
    file.close(); // close the file(filename)
    return string_two;

}

int     main(int ac, char **av)
{
    //creat a socket
    int server_fd;
    if (!(server_fd = socket(AF_INET, SOCK_STREAM, 0))) //STREAM ==> TCP
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    //binding the socket |int bind(int socket, const struct sockaddr *address, socklen_t address_len);|
    struct sockaddr_in address;
    std::string str(av[1], strlen(av[1]));
    const int PORT = std::stoi(str);
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    memset(address.sin_zero, '\0', sizeof (address.sin_zero));

    if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0)
    { 
        perror("bind field");
        return 0; 
    }
    //int listen(int socket, int backlog); accept the connections
    //int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
    int new_socket;
    int addrlen = sizeof(address);
    long valread;
    if(listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    char *header = strdup("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ");
    std::string body = get_body("index.html");
    std::string all = std::string(header)  + std::string(ft_itoa(strlen(body.c_str())))+ "\n\n"  + body;
    std::cout << all << std::endl;
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[1000] = {0};
        valread = read( new_socket , buffer, 30000);
        // std::cout << buffer << std::endl;
        write(new_socket , all.c_str() , strlen(all.c_str()));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }

    return 0;
}//