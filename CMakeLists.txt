cmake_minimum_required(VERSION 3.0)

project(google-translate-cli LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenSSL REQUIRED)
find_package(Boost 1.66.0 REQUIRED COMPONENTS system thread locale)

set(HOST_DEFAULT translate.google.com CACHE STRING "Default host")
set(LANGUAGE_DEFAULT_FROM auto CACHE STRING "Default language from")
set(LANGUAGE_DEFAULT_TO zh-cn CACHE STRING "Default language to")

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

include_directories(${PROJECT_BINARY_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

add_executable(translate ${PROJECT_SOURCE_DIR}/main.cpp)

target_link_libraries(translate ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
