cmake_minimum_required(VERSION 3.10)
project(BingDownloader VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)

pkg_check_modules(TINYXML2 tinyxml2)

add_executable(bing_downloader
    src/main.cpp
    src/downloader.cpp
)

target_include_directories(bing_downloader PRIVATE include)

target_link_libraries(bing_downloader PRIVATE CURL::libcurl)

if(TINYXML2_FOUND)
    target_link_libraries(bing_downloader PRIVATE ${TINYXML2_LIBRARIES})
    target_include_directories(bing_downloader PRIVATE ${TINYXML2_INCLUDE_DIRS})
else()
    if(EXISTS "${CMAKE_SOURCE_DIR}/third_party/tinyxml2")
        add_library(tinyxml2 STATIC
            third_party/tinyxml2/tinyxml2.cpp
        )
        target_include_directories(tinyxml2 PUBLIC third_party/tinyxml2)
        target_link_libraries(bing_downloader PRIVATE tinyxml2)
    else()
        message(FATAL_ERROR "TinyXML2 not found. Install it or place source in third_party/tinyxml2/")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(bing_downloader PRIVATE stdc++fs)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(bing_downloader PRIVATE -Wall -Wextra -O2)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(bing_downloader PRIVATE /W4 /O2)
endif()

file(MAKE_DIRECTORY "$ENV{HOME}/Downloads/Bing Wallpapers")
