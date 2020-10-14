#ifndef HTTPCONNECT_H
#define HTTPCONNECT_H

#include <iostream>
class HttpConnect
{
public:
  HttpConnect();
  ~HttpConnect();
 static std::string  socketHttp(std::string host, std::string request);
 static std::string getData(std::string host, std::string path, std::string get_content);
 static std::string postData(std::string host, std::string path, std::string post_content);

};

#endif // HTTPCONNECT_H
