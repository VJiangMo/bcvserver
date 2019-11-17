# CMake version check
cmake_minimum_required(VERSION 2.8)

# project info
project(bcvServer CXX)

# print
message("Project src dir:${PROJECT_SOURCE_DIR}")
message("Project bin dir:${PROJECT_BINARY_DIR}")

#v1.5.0 °æ±¾Éý¼¶µ½v1.5.0 2016/7/21
set(VERSION "v1.5.0")	
message(${VERSION})

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O2 -Wall")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++0x -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -std=c++0x -Wall")
message("Project CMAKE_CXX_COMPILER:${CMAKE_CXX_COMPILER}")

# set output bin directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# set output lib directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# make install
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/../bin)
install(FILES bin/${target} DESTINATION .)

aux_source_directory(. DIR_SRCS)

add_executable(bcvServer-${VERSION} ${DIR_SRCS})

SET (CMAKE_BUILD_TYPE Debug)

target_link_libraries(bcvServer-${VERSION}  mysqlcppconn pthread)




