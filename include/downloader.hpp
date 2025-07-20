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
