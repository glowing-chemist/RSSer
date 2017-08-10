cmake_minimum_required(VERSION 3.7)
project(RSSer)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_FLAGS "-I/home/ollie/programs/c++/cpp-netlib -ltinyxml -std=c++1z")

link_directories(/home/ollie/programs/c++/cpp-netlib)

set(SOURCE_FILES main.cpp)
add_executable(RSSer ${SOURCE_FILES})

target_link_libraries(RSSer boost_system pthread boost_thread cppnetlib-client-connections cppnetlib-server-parsers cppnetlib-uri)
