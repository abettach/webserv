#include "Response.hpp"

Response::Response(/* args */)
{
}

std::string Response::autoindex_run(std::string rooted_path, Request _request)
{
	DIR *directory = opendir(rooted_path.c_str());
	struct dirent *en;
	std::string fileName;
	std::string _autoIndexPage;
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
			std::string url = _request.getTarget();
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
	return _autoIndexPage;
}

// std::string Response::getStatusCodeSTR()
// {
// 	if (this->_status == ST_OK)
// 		return "OK\r\n";
// 	else if (this->_status == ST_MOVED_PERM)
// 		return "Moved Permenantly\r\n";
// 	else if (this->_status == ST_BAD_REQUEST)
// 		return "Bad Request\r\n";
// 	else if (this->_status == ST_FORBIDDEN)
// 		return "Forbidden\r\n";
// 	else if (this->_status == ST_NOT_FOUND)
// 		return "Not Found\r\n";
// 	else if (this->_status == ST_METHOD_NOT_ALLOWED)
// 		return "Not Allowed\r\n";
// 	else if (this->_status == ST_SERVER_ERROR)
// 		return "Internal Server Error\r\n";
// 	else if (this->_status == ST_NOT_IMPLEMENTED)
// 		return "Not Implemented\r\n";
// 	else if (this->_status == ST_PAYLOAD_LARGE)
// 		return "Payload Too Large\r\n";
// 	else if (this->_status == ST_BAD_GATEWAY)
// 		return "Bad Gateway\r\n";
// 	else if (this->_status == ST_NOT_SUPPORTED)
// 		return "HTTP Version Not Supported\r\n";
// 	return "";
// }

std::string     Response::GetBody()
{
    return this->_body;
}

void    Response::get_body(std::string file_name)
{
	std::ifstream file(file_name);
	if (file)
	{
		std::ostringstream ss;
		ss << file.rdbuf();
		this->_body = ss.str();
		file.close(); // close the file(filename)
	}
}

// bool Response::isDirectory(const std::string &s, int is_full)
// {
// 	std::string dir = s;
// 	DIR *d;
// 	if (!is_full)
// 		dir = getPulicDirectory().append(s);
// 	if ((d = opendir(dir.c_str())) == NULL) {
// 		return false;
//     }
// 	closedir(d);
// 	return true;
// }

void    Response::getMethode(std::string _uri, Request _request)
{
    std::cout << "uri = " << _uri << std::endl;
    if (this->isLocation)
        std::cout << "My Location:" << this->_location.getLocationPath() << std::endl;
    else
        std::cout << "My Location:" << "No Location Found!!" << std::endl;
    std::cout << "autoindex : " << this->_location.getLocationAutoIndex() << std::endl;
    std::cout << "index :" << this->_location.getLocationIndex() << std::endl;
    std::cout << "CGI status :" << isCGI << std::endl;
    std::cout << "Location status:" << isLocation << std::endl;
    if (this->isLocation)
    {
        if (this->isCGI)
        {
            std::map<std::string, bool> allowedMethods = this->_location.getLocationAllowedMethods();
            if (!allowedMethods["GET"])
                throw std::runtime_error("Error: GET Not Allowed");
            this->_body = _cgi.runCGI(_request, this->_server.getRootDir(), this->_location.getLocationFastCgiPass()).substr(67,_cgi.runCGI(_request, this->_server.getRootDir(), this->_location.getLocationFastCgiPass()).size());
            std::cout << "*********************************CGI Body*******************************" << std::endl;
            std::cout << this->_body << std::endl;
            std::cout << "************************************************************************" << std::endl;
        }
        if (this->_location.getLocationAutoIndex() && this->_location.getLocationIndex().empty())
            this->_body = autoindex_run(_uri, _request);       
        else if (this->_location.getLocationAutoIndex() && !this->_location.getLocationIndex().empty())
        {
            std::string filePath = _uri + this->_location.getLocationIndex();
            std::ifstream file(filePath.c_str());
            if (!file.is_open())
                this->_body = autoindex_run(this->_server.getRootDir() + _request.getTarget(), _request); 
            else
                get_body(this->_server.getRootDir() + "/" + this->_location.getLocationIndex());
        }
        else if (!this->_location.getLocationAutoIndex() && this->_location.getLocationIndex().empty() && !this->_location.getLocationPath().compare("/"))
        {
            std::string filePath = this->_server.getRootDir() + _request.getTarget() + "/" + "index.html";
            std::ifstream file(filePath.c_str());
            if (!file.is_open())
            {
                get_body(this->_server.getErrorPage()[404]);
            }
            else
            get_body(this->_server.getRootDir() + _request.getTarget() + "/" + "index.html");
        }
    }
    else
        get_body(_uri);
}

int    Response::CheckForPerfectMatch(std::string _path, std::vector<location> _locations)
{
    if (_path.find(".py") != std::string::npos || _path.find(".php") != std::string::npos)
        isCGI = true;
    for (std::vector<location>::iterator it = _locations.begin(); it != _locations.end(); it++)
    {
        std::cout << "path =|" << _path << "|, location path=|" << it->getLocationPath() << "|" << std::endl;
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
void    Response::creatBody(std::vector<serverINFO> &_servers, Request &_request)
{
    //*****************Find the right location and server information****************
    std::string RequestPath = _request.getTarget();
    this->isLocation = false;
    this->isCGI = false;
    if (RequestPath.empty())
        RequestPath.append("/");
    std::cout << "RequestPath :" << RequestPath << std::endl; 
    for (size_t i = 0; i < _servers.size(); i++)
        if (_servers[i].getPort() == _request.getPort())
        {
            this->_server = _servers[i];
            break;
        }
    std::map<std::string, location> locations = this->_server.getLocations();
    std::vector<location> myLocations;
    for (std::map<std::string, location>::iterator it = locations.begin(); it != locations.end() ; it++)
        myLocations.push_back(it->second);
    std::cout << "server path = " << RequestPath << std::endl;
    if (this->CheckForPerfectMatch(RequestPath, myLocations) ||
        this->CheckForMatchOne(RequestPath, myLocations))
        isLocation = true;
    // std::map<std::string, bool> allowedMethods = this->_location.getLocationAllowedMethods();
    // if (!allowedMethods[_request.getMethode()])
    // {
    //     if (_request.getMethod() != "GET" && _request.getMethod() != "POST" && _request.getMethod() != "DELETE")
	// 		_status = ST_NOT_IMPLEMENTED;
	// 	else
	// 		_status = ST_METHOD_NOT_ALLOWED;
    //     throw std::runtime_error("Error: Methode Not allowed");
    // }
    //**********************************************************************************
    std::cout << "methode = " << _request.getMethode() << std::endl;
    if (!_request.getMethode().compare("GET"))
        getMethode(this->_server.getRootDir() + _request.getTarget(), _request);
    // else if (!_request.getMethode().compare("POST"))
    //     PostMethode(_request.getTarget());
    // else if (!_request.getTarget().compare("DELETE"))
    //     DeleteMethode();
}

void    Response::creatResponse(std::vector<serverINFO> &_servers, Request &_request)
{

    creatBody(_servers, _request);
  

    // this->_resp = "HTTP/1.1 ";
    // this->_resp.append(std::to_string(_statusCode));
    // this->_resp.append(" ");
    // this->_resp.append(getStatusCode());
}

Response::~Response()
{
}