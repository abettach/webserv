#ifndef CONFIGFILEPARSE_HPP
#define CONFIGFILEPARSE_HPP
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
#include <arpa/inet.h> // for sockaddr_in and inet_ntoa()
#include <iostream>
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <algorithm>

#define FILE_EXTENTION "conf"
#define INVALIDE_FILE_NAME "Error: Invalide File Name"
#define COMMENTV1 '#'
#define COMMENTV2 ';'

class ConfigFilePars
{
private:
    std::string file_name;
    std::vector<std::string> file_content;
public:
    ConfigFilePars(/* args */);
    ConfigFilePars(int ac, char **av);
    ~ConfigFilePars();

    void    Arguments_checker(int ac, char **av);
    void    get_file_content();
    void    ft_strtrim(std::string &str);
    void    remove_comments(std::string &str, char c);
    void    file_check();// check the inside of the server is valide ?

    class FILE_NAME_EXEPTION : public std::exception
    {
        const char *what() const throw();
    };

};

template <typename T>
void    _print(T &var, std::string type)
{
    if(type == "vector")
    {
        for (int i = 0; i < var.size(); i++)
            std::cout << var[i] << std::endl;
    }
}


#endif