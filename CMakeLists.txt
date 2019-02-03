cmake_minimum_required(VERSION 3.0)

project(google-translate-cli LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OPENSSL_USE_STATIC_LIBS TRUE)
set(OPENSSL_MSVC_STATIC_RT TRUE)
find_package(OpenSSL REQUIRED)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
set(OPENSSL_LIBRARIES ${OPENSSL_LIBRARIES} ws2_32 crypt32)
endif(CMAKE_SYSTEM_NAME MATCHES "Windows")

set(Boost_USE_STATIC_LIBS TRUE)
set(Boost_USE_STATIC_RUNTIME TRUE)
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

target_link_libraries(translate ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CMAKE_DL_LIBS})
