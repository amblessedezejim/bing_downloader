#include "../include/downloader.hpp"
#include <iostream>

HTTPClient::HTTPClient() {
  curl_ = curl_easy_init();
  if (!curl_) {
    std::cerr << "Failed to initialize curl" << std::endl;
  }
}

HTTPClient::~HTTPClient() {
  if (curl_) {
    curl_easy_cleanup(curl_);
  }
}

std::string HTTPClient::get(const std::string &url) {
  std::string response;
  if (!curl_) {
    std::cerr << "CURL not initialized" << std::endl;
    return response;
  }

  curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallBack);
  curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);

  CURLcode res = curl_easy_perform(curl_);
  if (res != CURLE_OK) {
    std::cerr << "CURL ERROR: " << curl_easy_strerror(res) << std::endl;
  }

  return response;
}

size_t HTTPClient::WriteCallBack(void *contents, size_t size, size_t nmeb,
                                 std::string *userp) {
  size_t actual_size = size * nmeb;
  userp->append(static_cast<char *>(contents));
  return actual_size;
}
