cmake_minimum_required(VERSION 3.16)

project(pkg)

file(GLOB SRC CONFIGURE_DEPENDS "*.hpp" "*.cpp")

set(CMAKE_BUILD_TYPE Debug CURL_LIBRARY "-lcurl")

find_package(CURL REQUIRED)

add_executable(${PROJECT_NAME} ${SRC})

include_directories(${CURL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
