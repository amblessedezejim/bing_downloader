#include "../include/downloader.hpp"
#include <iostream>

int main(int argc, char **argv) {
  HTTPClient client;
  std::string xml = client.get(
      "https://www.bing.com/HPImageArchive.aspx?format=xml&idx=0&n=1");
  XmlParser parser;
  if (xml.empty())
    return EXIT_FAILURE;
  std::string url = parser.extractUrl(xml);

  ImageDownloader downloader("images");
  bool res = downloader.downloadImage(url);
  if (!res)
    std::cerr << "Failed to download image" << std::endl;
  return EXIT_SUCCESS;
}
