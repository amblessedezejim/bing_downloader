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

class ImageDownloader {
public:
  ImageDownloader(const std::string &outputDir_);
  bool downloadImage(const std::string &imageUrl,
                     const std::string &filename = "");

private:
  std::string outputDir_;
  std::string generateFilename(const std::string &url);
  std::string sanitizeFilename(const std::string &filename);
  static size_t WriteFileCallback(void *contents, size_t size, size_t nmeb,
                                  void *userp);
};

class BingDownloader {
public:
  BingDownloader(const std::string &outputDir = "./images");
  bool downloadTodaysImage();
  bool downloadImages(int count = 1, int startIndex = 0);

private:
  std::string outputDir_;
  HTTPClient httpClient_;
  XmlParser xmlParser_;
  ImageDownloader imageDownloader_;

  std::string buildApiUrl(int index, int count);
  void log(const std::string &message);
};
