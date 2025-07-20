#include "../include/downloader.hpp"
#include <iostream>

int main(int argc, char **argv) {
  HTTPClient client;
  std::string xml = client.get(
      "https://www.bing.com/HPImageArchive.aspx?format=xml&idx=0&n=1");
  std::cout << xml << std::endl;
  return EXIT_SUCCESS;
}
