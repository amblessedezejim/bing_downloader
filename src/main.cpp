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
  std::cout << url << std::endl;
  return EXIT_SUCCESS;
}
