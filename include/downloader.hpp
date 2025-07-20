#pragma once
#include <curl/curl.h>
#include <string>

class HTTPClient {
public:
  HTTPClient();
  ~HTTPClient();

  std::string get(const std::string &url);
  ;

private:
  CURL *curl_;
  static size_t WriteCallBack(void *contents, size_t size, size_t nmeb,
                              std::string *userp);
};

class XmlParser {
public:
  XmlParser();

  std::string extractUrl(const std::string &xmlContent);

private:
  std::string absoluteUrl(const std::string &relativeUrl);
  bool isValidUrl(const std::string &url);
};
