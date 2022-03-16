#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "../../headers.hpp"
#include "../Request/Request.hpp"
#include "../Config/FileParss.hpp"
#include "../Cgi/cgi.hpp"
class location;
class serverINFO;
class Request;

class Response
{
private:
    // Request _request;
    bool isCGI;
    bool isLocation;
    CGI _cgi;
    std::string _body;
    std::string _statusLine;
    std::map<std::string, std::string> _headers;
    std::string _httpVersion;
    int         _statusCode;
    std::string _reasonPhrase;
    location _location;
    std::string _resp;
    serverINFO _server;

public:
    Response(/* args */);
    std::string     &getStatusLine();
    void    creatResponse(std::vector<serverINFO> &, Request &);
    void    creatBody(std::vector<serverINFO> &, Request &);
    void    runGetMethode(Request _request);
    void    getMethode(std::string _uri, Request _request);
    void get_body(std::string file_name);
    int    CheckForPerfectMatch(std::string, std::vector<location> _loactions);
    int     CheckForMatchOne(std::string _path, std::vector<location> _locations);
    std::string autoindex_run(std::string rooted_path, Request _request);
    std::string     GetBody();
    bool isDirectory(const std::string &s, int is_full);
    ~Response();
};


#endif