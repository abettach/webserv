#include "cgi.hpp"

CGI::CGI(/* args */)
{
    std::cout << "CGI Default constractro called" << std::endl;
}

void    CGI::runCGI(Request &_request, std::string &root,  std::string &cgi_path)
{
    // char const *argv[3];
    // int     fd;
    // int     pipefd[2];
    // pid_t   pid;

    // setenv("CONTENT_LENGTH",/*get the content length from the request*/, 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("SERVER_PORT", std::to_string(_request.getPort()).c_str(), 1);
    setenv("REQUEST_METHOD", _request.getMethode().c_str(), 1);
    setenv("SERVER_NAME", "webserv", 1);
    // setenv("REDIRECT_STATUS", "1", 1);
    setenv("PATH_INFO", "/Users/abettach/Desktop/webserv", 1);
    // setenv("QUERY_STRING", /*get the query string from the request*/, 1);

    // pipe(pipefd[2]);
    // pid = fork();

    // if (!pid)
    // {
    //     dup2(pipefd[0], 0);
    //     close(pipefd[1]);
    //     close(pipefd[0]);
        // argv[0] = cgi_path.c_str();
    //     argv[1] = /*request uri*/.c_str();
    //     argv[2] = NULL;
    //     dup2(fd, STDOUT_FILENO);

    //     chdir(root.c_str())
    //     if (execve(argv[0], (char const **)argv, environ) == -1)
    //     {
    //         std::cout << "execve probleme" << std::endl;
    //         exit(1);
    //     }
    // }
    // else
    // {
    //     close(pipefd[0]);
    //     write(pipefd[1], /*body of the request*/.c_str(), /*the length of the body*/);
    //     close(pipefd[1]);
    //     waitpid(pid, nullptr, 0);
    // }

}

CGI::~CGI()
{
    std::cout << "CGI Destractor called" << std::endl;
}
