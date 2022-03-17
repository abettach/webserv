#include "cgi.hpp"

CGI::CGI(/* args */){}

std::string    CGI::runCGI(Request _request, std::string root,  std::string cgi_path)
{
    char const *argv[3];
    int Ifd[2];
    int Ofd[2];
    pid_t   pid;
    std::string content;
    std::string file_path;
    extern char **environ;

    // setenv("CONTENT_LENGTH",/*get the content length from the request*/, 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("SERVER_PORT", std::to_string(_request.getPort()).c_str(), 1);
    setenv("REQUEST_METHOD", _request.getMethod().c_str(), 1);
    setenv("SERVER_NAME", "webserv", 1);
    setenv("REDIRECT_STATUS", "1", 1);
    setenv("PATH_INFO", "/Users/abettach/Desktop/webserv", 1);
    std::cout << "my file name = " <<( root + _request.getTarget()).c_str() << std::endl;
    setenv("SCRIPT_FILENAME", (root + _request.getTarget()).c_str(), 1);//root + _req.getTarget()
    // setenv("QUERY_STRING", /*get the query string from the request*/, 1);
    if (pipe(Ifd) || pipe(Ofd))
        perror("[CGI ERROR] PIPE");
    // for (size_t i = 0; environ[i] ; i++)
    //     std::cout << environ[i] << std::endl;
    pid = fork();
    if (!pid)
    {
        close(Ifd[0]);
        // dup2(Ofd[0], 0);
        // close(Ofd[1]);
        dup2(Ifd[1], 1);
        // close(Ofd[0]);
        close(Ifd[1]);
        argv[0] = cgi_path.c_str();
        file_path = (root + _request.getTarget()).c_str(); // SCRIPT_FILENAME in env :)
        argv[1] = file_path.c_str();
        argv[2] = NULL;            
        if (execve(argv[0], (char* const*)argv, environ) == -1)
            perror("Error: Execve Can't run");
    }
    else
    {
        close(Ifd[1]);
        // close(Ofd[0]);
        // close(Ofd[1]);
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