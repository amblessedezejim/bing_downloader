#include "../include/downloader.hpp"
#include <iostream>

int main(int argc, char **argv) {
  int count = 1;
  std::string output = "./images";
  if (argc > 1) {
    count = std::stoi(argv[1]);
  }
  if (argc > 2) {
    output = argv[2];
  }
  BingDownloader downloader(output);
  if (!downloader.downloadImages(count)) {
    std::cerr << "Download failed." << std::endl;
    return 1;
  }

  std::cout << "Images downloaded to: " << output << std::endl;
  return 0;
}
