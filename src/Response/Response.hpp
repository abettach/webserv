#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "../utils/headers.hpp"
#include "../Request/Request.hpp"
#include "../Config/FileParss.hpp"
#include "../Cgi/cgi.hpp"
#include "../Request/../utils/StatusCode.hpp"
# include <unistd.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>

#define RINDEX "/index.html"
class location;
class serverINFO;
class Request;
class Response
{
private:
    std::map<int , std::string> _errors;
    std::vector<serverINFO> _servers;
    Request _request;
    serverINFO _server;
    location _location;
    CGI _cgi;
    std::string _body;
    std::string _statusLine;
    std::string _headers;
    std::string _resp;
    int         _statusCode;
    bool isCGI;
    bool isLocation;

public:
    Response(/* args */);
    std::string     &getStatusLine();
    void    creatResponse(std::vector<serverINFO> &, Request &);
    void    creatBody();
    void    runGetMethod(Request _request);
    void    getMethod(std::string _uri);
    void get_body(std::string file_name);
    int    CheckForPerfectMatch(std::string, std::vector<location> _loactions);
    int     CheckForMatchOne(std::string _path, std::vector<location> _locations);
    void autoindex_run(std::string rooted_path);
    std::string     GetBody();
    bool isDirectory(const std::string &s, int is_full);
    void    setErrorPage(int _Error_code);
    std::string getDefaultErrorPage(int status);
    std::string getStatusCodeTranslate();
    std::string getRespContentType();
    std::string     getRespHeader();
    void    clear();
    bool    isDirectory(std::string path);
    void    deleteMethod(std::string _Path);
    void    parseCgiResp(std::string &);
    void    PostMethod();
    std::string getUploadPath();
    location getRedirection(std::string locName);
    std::string getDefaultPage();
    ~Response();
};


#endif