#include "Response.hpp"

Response::Response(/* args */) : _statusCode(200) , _headers("")
{
    // this->_status = this->_request.getStatusCode();
}

std::string     Response::getRespHeader()
{
    return this->_headers;
}

void Response::autoindex_run(std::string rooted_path)
{
	DIR *directory = opendir(rooted_path.c_str());
	struct dirent *en;
	std::string fileName;
	std::string _autoIndexPage;

    if (!access(rooted_path.c_str(), F_OK))
    {
	    _autoIndexPage = "<!DOCTYPE html>\n<html lang=\"en\">\n\
	    				  <head>\n\
	    				  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
	    				  <title>AUTO INDEX</title>\n\
	    				  </head>\n\
	    				  <body>\n\
	    				  <div style=\"margin-left: 5%; margin-top:10%;\">\n\
	    				  <hr>\n";

	    if (directory)
	    {
	    	while ((en = readdir(directory)) != nullptr)
	    	{
	    		fileName = en->d_name;
	    		if (en->d_type == DT_DIR)
	    			fileName.append("/");
	    		std::string url = this->_request.getTarget();
	    		std::string str;
	    		if (url[0] != '/')
	    			str = url + fileName;
	    		else if (url == "/")
	    			str = fileName;
	    		else
	    			str = url + '/' + fileName;
	    		_autoIndexPage.append("\t\t\t<p><a href=\"" + str + "\">" + fileName + "</a></p>\n");
	    	}
	    	closedir(directory);
	    }
	    _autoIndexPage += "\
	    				   <hr>\n\
	    				   </div>\n\
	    				   </body>\n\
	    				   </html>\n";
        this->_body = _autoIndexPage;       
    }
    else
        setErrorPage(NOT_FOUND);
}

std::string Response::getStatusCodeTranslate()
{
	if (this->_statusCode == OK)
		return "OK\r\n";
	else if (this->_statusCode == MOVED_PERMANENTLY)
		return "Moved Permenantly\r\n";
	else if (this->_statusCode == BAD_REQUEST)
		return "Bad Request\r\n";
	else if (this->_statusCode == FORBIDEN)
		return "Forbidden\r\n";
	else if (this->_statusCode == NOT_FOUND)
		return "Not Found\r\n";
	else if (this->_statusCode == METHOD_NOT_ALLOWED)
		return "Not Allowed\r\n";
	else if (this->_statusCode == INTERNAL_SERVER_ERROR)
		return "Internal Server Error\r\n";
	else if (this->_statusCode == NOT_IMPLEMENTED)
		return "Not Implemented\r\n";
	else if (this->_statusCode == REQUEST_ENTITY_TOO_LARGE)
		return "Payload Too Large\r\n";
	else if (this->_statusCode == BAD_GETEWAY)
		return "Bad Gateway\r\n";
	else if (this->_statusCode == HTTP_VERSOIN_NOT_SUPPORTED)
		return "HTTP Version Not Supported\r\n";
	return "";
}

std::string     Response::GetBody()
{
    return this->_body;
}

void    Response::get_body(std::string file_name)
{
    std::ifstream file(file_name);
    if (access(file_name.c_str(), F_OK) != 0) // Check if The file existe
        setErrorPage(NOT_FOUND);
    else
    {
	    if (file)
	    {
	        std::ostringstream ss;
	        ss << file.rdbuf();
	        this->_body = ss.str();
	    }
        else
            setErrorPage(INTERNAL_SERVER_ERROR);
	    file.close(); // close the file(filename)
    }
}

std::string Response::getDefaultErrorPage(int status)
{
    std::string errorPage = "<!DOCTYPE html>\n\
    <html>\n\
        <head>\n\
            <meta charset=\"UTF-8\" />\n\
            <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n\
            <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n\
            <title>Document</title>\n\
            <style>\n\
            body {\n\
                background-color: rgba(0, 0, 0, 0.119);\n\
            }\n\
            .container {\n\
                margin: 10%;\n\
                text-align: center;\n\
                color: rgba(250, 52, 52, 0.708);\n\
            }\n\
            h1 {\n\
                font-size: 14rem;\n\
                font-family: \"Courier New\", Courier, monospace;\n\
                font-weight: bold;\n\
                margin:-5rem 0 0 0;\n\
            }\n\
            .parag {\n\
                margin:0;\n\
                font-weight: bold;\n\
                font-size: 5rem;\n\
                font-family: \"Courier New\", Courier, monospace;\n\
            }\n\
            </style>\n\
        </head>\n\
        <body>\n\
            <div class=\"container\">\n\
            <h1>$1</h1>\n\
            <p class=\"parag\">$2</p>\n\
            </div>\n\
        </body>\n\
    </html>";
    errorPage.replace(errorPage.find("$1"), 2, std::to_string(status));
    errorPage.replace(errorPage.find("$2"), 2, getStatusCodeTranslate());
    return errorPage;
}

void    Response::setErrorPage(int _Error_code)
{
    this->_statusCode = _Error_code;
    if (!this->_server.getErrorPage()[this->_statusCode].empty())
        get_body(this->_server.getErrorPage()[this->_statusCode]);
    else
        this->_body = getDefaultErrorPage(this->_statusCode);
}

void    Response::getMethod(std::string _uri)
{
    // std::cout << "uri = " << _uri << std::endl;
    // if (this->isLocation)
    //     std::cout << "My Location:" << this->_location.getLocationPath() << std::endl;
    // else
    //     std::cout << "My Location:" << "No Location Found!!" << std::endl;
    // std::cout << "autoindex : " << this->_location.getLocationAutoIndex() << std::endl;
    // std::cout << "index :" << this->_location.getLocationIndex() << std::endl;
    // std::cout << "CGI status :" << isCGI << std::endl;
    // std::cout << "Location status:" << isLocation << std::endl;
    std::map<std::string, bool> allowedMethods = this->_location.getLocationAllowedMethods();
    if (this->isLocation)
    {
        if (this->isCGI)
        {
            this->_body = _cgi.runCGI(this->_request, this->_server.getRootDir(), this->_location.getLocationFastCgiPass()).substr(67,_cgi.runCGI(this->_request, this->_server.getRootDir(), this->_location.getLocationFastCgiPass()).size());
            std::cout << "*********************************CGI Body*******************************" << std::endl;
            std::cout << this->_body << std::endl;
            std::cout << "************************************************************************" << std::endl;
        }
        if (this->_location.getLocationAutoIndex() && this->_location.getLocationIndex().empty())
            autoindex_run(_uri);       
        else if (this->_location.getLocationAutoIndex() && !this->_location.getLocationIndex().empty())
        {
            std::string filePath = _uri + "/" + this->_location.getLocationIndex();
            std::ifstream file(filePath.c_str());
            if (file.is_open())
                get_body(_uri + "/" + this->_location.getLocationIndex());
            else
                autoindex_run(this->_server.getRootDir() + this->_request.getTarget()); 
            file.close();
        }
        else if (!this->_location.getLocationAutoIndex() && this->_location.getLocationIndex().empty() && !this->_location.getLocationPath().compare("/"))
            get_body(this->_server.getRootDir() + this->_request.getTarget() + RINDEX);
    }
    else
        get_body(_uri);
}

int    Response::CheckForPerfectMatch(std::string _path, std::vector<location> _locations)
{
    this->_location = _locations[0];
    if (_path.find(".py") != std::string::npos || _path.find(".php") != std::string::npos)
        isCGI = true;
    for (std::vector<location>::iterator it = _locations.begin(); it != _locations.end(); it++)
    {
        // std::cout << "path =|" << _path << "|, location path=|" << it->getLocationPath() << "|" << std::endl;
        if (_path == it->getLocationPath())
        {
            this->_location = *it;
            return 1;
        }
        if (_path.find(".") == std::string::npos && it->getLocationPath() == "/")
        {
            this->_location = *it;
            return 1;
        }
    }
    return 0;
}

int     Response::CheckForMatchOne(std::string _path, std::vector<location> _locations)
{
    this->_location = _locations[0];
    if (_path.find(".py") != std::string::npos || _path.find(".php") != std::string::npos)
        isCGI = true;
    _path.erase(0, _path.find(".") + 1);
    for (std::vector<location>::iterator it = _locations.begin(); it != _locations.end(); it++)
    {
        if (_path == it->getLocationPath().erase(0, it->getLocationPath().find(".") + 1))
        {
            this->_location = *it;
            return 1;
        }
    }
    return 0;
}

bool Response::isDirectory(std::string path)
{
    DIR *dir;

    if ((dir = opendir(path.c_str())))
    {
        closedir(dir);
        return true;
    }
    return false;
}

void    Response::deleteMethod(std::string _Path)
{
    std::cout << "Path =" << _Path << std::endl;
    if (isDirectory(_Path))
        setErrorPage(NOT_FOUND);
    else
    {
        if (access(_Path.c_str(), F_OK))
            setErrorPage(NOT_FOUND);
        else
        {
            if (!access(_Path.c_str(), W_OK))
            {
                if (std::remove(_Path.c_str()))
                    setErrorPage(INTERNAL_SERVER_ERROR);
            }
            else
                setErrorPage(FORBIDEN);
        }
    }
}

void    Response::creatBody()
{
    //*****************Find the right location and server information****************
    std::string RequestPath = this->_request.getTarget();
    this->isLocation = false;
    this->isCGI = false;
    if (RequestPath.empty())
        RequestPath.append("/");
    // std::cout << "RequestPath :" << RequestPath << std::endl; 
    for (size_t i = 0; i < this->_servers.size(); i++)
        if (this->_servers[i].getPort() == this->_request.getPort())
        {
            this->_server = this->_servers[i];
            break;
        }
    std::map<std::string, location> locations = this->_server.getLocations();
    std::vector<location> myLocations;
    for (std::map<std::string, location>::iterator it = locations.begin(); it != locations.end() ; it++)
        myLocations.push_back(it->second);
    // std::cout << "server path = " << RequestPath << std::endl;
    if (this->CheckForPerfectMatch(RequestPath, myLocations) || this->CheckForMatchOne(RequestPath, myLocations))
        isLocation = true;
    std::map<std::string, bool> allowedMethods = this->_location.getLocationAllowedMethods();
    if (!allowedMethods[this->_request.getMethod()])
    {
        if (this->_request.getMethod() != "GET" && this->_request.getMethod() != "POST" && this->_request.getMethod() != "DELETE")
			setErrorPage(NOT_IMPLEMENTED);
		else
			setErrorPage(METHOD_NOT_ALLOWED);
        // throw std::runtime_error("Error: Method Not allowed");
    }
    // std::cout << "Method = " << this->_request.getMethod() << std::endl;
    if (!this->_request.getMethod().compare("GET") && allowedMethods[this->_request.getMethod()])
        getMethod(this->_server.getRootDir() + this->_request.getTarget());
    // else if (!_request.getMethod().compare("POST"))
    //     PostMethod(_request.getTarget());
    else if (!_request.getMethod().compare("DELETE"))
    {
        std::cout << "im heeere" << std::endl;
        deleteMethod(this->_server.getRootDir() + this->_request.getTarget());
    } 
}

std::string Response::getRespContentType()
{
    if (this->_request.getReqValue("Content-Type").size())
        return (this->_request.getReqValue("Content-Type"));
	if (_request.getTarget().find(".") != std::string::npos &&  !this->_request.getTarget().substr(_request.getTarget().find(".")).compare(".html"))
		return "text/html; charset=UTF-8";
	else if (_request.getTarget().find(".") != std::string::npos && !this->_request.getTarget().substr(_request.getTarget().find(".")).compare(".css"))
		return "text/css";
	else if (_request.getTarget().find(".") != std::string::npos && !this->_request.getTarget().substr(_request.getTarget().find(".")).compare(".json"))
		return "application/json";
	else if (_request.getTarget().find(".") != std::string::npos && !this->_request.getTarget().substr(_request.getTarget().find(".")).compare(".xml"))
		return "application/xml";
	else if (_request.getTarget().find(".") != std::string::npos && !this->_request.getTarget().substr(_request.getTarget().find(".")).compare(".js"))
		return "aplication/javascript";
	else
		return "text/html; charset=UTF-8";
}

void    Response::creatResponse(std::vector<serverINFO> &servers, Request &request)
{
    time_t _time;
    std::string tm;
    time(&_time);
    tm = ctime(&_time);

    tm.pop_back();
    this->_request = request;
    this->_servers = servers;
    this->creatBody();
    this->_headers.append("HTTP/1.1");
    this->_headers.append(" ");
    this->_headers.append(std::to_string(this->_statusCode));
    this->_headers.append(" ");
    this->_headers.append(getStatusCodeTranslate());
    if (this->_statusCode == MOVED_PERMANENTLY)
    {
        this->_headers.append("Location: " + this->_redirectionLocation);
        this->_headers.append("\r\n");
        this->_headers.append("\r\n\r\n");
    }
    else
    {
        this->_headers.append("Server: webserv\r\n");
        this->_headers.append("Date: " + tm.append("GMT"));
        this->_headers.append("\r\n");
        this->_headers.append("Connection: " + _request.getReqValue("Connection"));
        this->_headers.append("\r\n");
        this->_headers.append("Content-Type: " + getRespContentType());
        if(this->_request.getReqValue("Transfer-Encoding").size())
        {
            this->_headers.append("\r\n");
            this->_headers.append("Transfer-Encoding: " + this->_request.getReqValue("Transfer-Encoding"));
        }
        else
        {
            this->_headers.append("\r\n");
            this->_headers.append("Content-Length: " + std::to_string(this->_body.length()));
        }
        if (this->_request.getReqValue("cookie").size())
        {
            this->_headers.append("\r\n");
            this->_headers.append("Set-cookie: " + this->_request.getReqValue("cookie"));
        }
        this->_headers.append("\r\n\r\n");
        this->_headers.append(this->_body);
    }
    std::cout << BYEL << "******************************** Response ********************************" << std::endl;
    std::cout << BRED << this->_headers << std::endl;
    std::cout << BYEL <<"********************************* Response ********************************" << BWHT <<std::endl;
}

void    Response::clear()
{
    this->_body.clear();
    this->_headers.clear();
    this->_statusCode = 200;    
}

Response::~Response()
{
}