#include "../include/downloader.hpp"
#include "tinyxml2.h"
#include <chrono>
#include <cstdio>
#include <curl/curl.h>
#include <curl/easy.h>
#include <filesystem>
#include <iostream>
#include <regex>

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
    return "";
  }

  return response;
}

size_t HTTPClient::WriteCallBack(void *contents, size_t size, size_t nmeb,
                                 std::string *userp) {
  size_t actual_size = size * nmeb;
  userp->append(static_cast<char *>(contents), actual_size);
  return actual_size;
}

XmlParser::XmlParser() {}
std::string XmlParser::extractUrl(const std::string &url) {
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError result = doc.Parse(url.c_str());

  if (result != tinyxml2::XML_SUCCESS) {
    std::cerr << "XML Parse error" << doc.ErrorStr() << std::endl;
    return "";
  }

  tinyxml2::XMLElement *root = doc.FirstChildElement("images");
  if (!root)
    return "";

  tinyxml2::XMLElement *imageElem = root->FirstChildElement("image");
  if (!imageElem)
    return "";

  const char *rawUrl = imageElem->FirstChildElement("url")
                           ? imageElem->FirstChildElement("url")->GetText()
                       : imageElem->FirstChildElement("urlBase")
                           ? imageElem->FirstChildElement("urlBase")->GetText()
                           : nullptr;

  if (!rawUrl)
    return "";
  std::string absUrl = rawUrl;
  if (!isValidUrl(absUrl)) {
    absUrl = absoluteUrl(absUrl);
  }

  return absUrl;
}

std::string XmlParser::absoluteUrl(const std::string &url) {
  return "https://bing.com" + url;
}

bool XmlParser::isValidUrl(const std::string &url) {
  return url.rfind("http://", 0) == 0 || url.rfind("https://", 0) == 0;
}

ImageDownloader::ImageDownloader(const std::string &outputDir_) {
  this->outputDir_ = outputDir_;
  std::filesystem::create_directories(outputDir_);
}

bool ImageDownloader::downloadImage(const std::string &imageUrl,
                                    const std::string &filename) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    std::cerr << "Failed to initialize curl" << std::endl;
    return false;
  }
  std::string name = filename.empty() ? generateFilename(imageUrl)
                                      : sanitizeFilename(filename);
  std::string path = outputDir_ + "/" + name;
  FILE *file = fopen(path.c_str(), "wb");
  if (!file) {
    std::cerr << "Failed to create file: " << path << std::endl;
    return false;
  }

  curl_easy_setopt(curl, CURLOPT_URL, imageUrl.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFileCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

  CURLcode res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  fclose(file);

  if (res != CURLE_OK) {
    std::filesystem::remove(path);
    std::cerr << "failed to download resource" << std::endl;
    return false;
  }

  return true;
}

std::string ImageDownloader::generateFilename(const std::string &url) {
  auto now = std::chrono::system_clock::now().time_since_epoch();
  auto timestamp =
      std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
  return std::to_string(timestamp) + ".jpg";
}

std::string ImageDownloader::sanitizeFilename(const std::string &filename) {
  std::string clean =
      std::regex_replace(filename, std::regex(R"([\\/:*?"<>|]))"), "_");
  return clean;
}

size_t ImageDownloader::WriteFileCallback(void *contents, size_t size,
                                          size_t nmeb, void *userp) {
  FILE *file = static_cast<FILE *>(userp);
  return fwrite(contents, size, nmeb, file);
}
