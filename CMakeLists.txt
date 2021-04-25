cmake_minimum_required(VERSION 3.17)

project(reopen62541 LANGUAGES C CXX)

include(${CMAKE_CURRENT_LIST_DIR}/src/open62541/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/reopen62541/CMakeLists.txt)

include(${CMAKE_CURRENT_LIST_DIR}/src/examples/calculator/CMakeLists.txt)
