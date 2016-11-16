cmake_minimum_required (VERSION 2.6)
PROJECT (Packer)

# C++11 IS REQUIRED
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

########
# BOOST
########
SET (Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE (Boost)

INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIR})
LINK_DIRECTORIES (${Boost_LIBRARY_DIR})

########
# PACKER
########
INCLUDE_DIRECTORIES (./src)
ADD_EXECUTABLE (Packer ./src/bws/packer/main.cpp)
TARGET_LINK_LIBRARIES (Packer ${Boost_LIBRARY_DIR})