cmake_minimum_required(VERSION 3.27)
project(Networking)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.84.0 REQUIRED)
file(GLOB SOURCES src/* src/*/* src/*/*/*)
add_library(${PROJECT_NAME} ${SOURCES})

if(MINGW)
    target_link_libraries(${PROJECT_NAME} ws2_32)
    target_link_libraries(${PROJECT_NAME} wsock32)
endif()

target_include_directories(${PROJECT_NAME}
                    PUBLIC
                        $<INSTALL_INTERFACE:include>
                        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                        ${Boost_INCLUDE_DIRS}
                    PRIVATE
)

target_link_libraries(${PROJECT_NAME}
                    PUBLIC
                        ${Boost_LIBRARIES}
)