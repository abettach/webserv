#include "cgi.hpp"

CGI::CGI(){}

std::string    CGI::runCGI(Request _request, std::string root,  std::string cgi_path)
{
    char const *argv[3];
    int Ifd[2];
    pid_t   pid;
    std::string content;
    std::string file_path;
    extern char **environ;

    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("SERVER_PORT", std::to_string(_request.getPort()).c_str(), 1);
    setenv("REQUEST_METHOD", _request.getMethod().c_str(), 1);
    setenv("SERVER_NAME", "webserv", 1);
    setenv("REDIRECT_STATUS", "1", 1);
    setenv("PATH_INFO", "/Users/abettach/Desktop/webserv", 1);
    setenv("SCRIPT_FILENAME", (root + _request.getTarget()).c_str(), 1);
    if (pipe(Ifd))
        perror("[CGI ERROR] PIPE");
    if (!_request.getReqValue("Cookie").empty())
        setenv("HTTP_COOKIE", _request.getReqValue("Cookie").c_str(), 1);
    if (_request.getMethod() == "GET")
    {
        setenv("QUERY_STRING", _request.getQueryString().c_str(), 1);
        setenv("CONTENT_LENGTH", "0", 1);
    }
    pid = fork();
    if (!pid)
    {
        close(Ifd[0]);
        dup2(Ifd[1], 1);
        close(Ifd[1]);
        argv[0] = cgi_path.c_str();
        file_path = (root + _request.getTarget()).c_str();
        argv[1] = file_path.c_str();
        argv[2] = NULL;            
        if (execve(argv[0], (char* const*)argv, environ) == -1)
            perror("Error: Execve Can't run");
    }
    else
    {
        close(Ifd[1]);
        int ret = 1;
        char buffer[1024];
        while (ret)
        {
            memset(buffer, 0, 1024);
            ret = read(Ifd[0], buffer, 1024);
            content += buffer;
        }
        close(Ifd[0]);
        waitpid(pid, nullptr, 0);
    }
    return (content);
}

CGI::~CGI(){}