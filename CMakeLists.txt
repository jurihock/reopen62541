cmake_minimum_required(VERSION 3.17)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
  project(reopen62541 LANGUAGES C CXX CSharp)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  set(CMAKE_DOTNET_TARGET_FRAMEWORK_VERSION "v4.0")
else()
  project(reopen62541 LANGUAGES C CXX)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/src/open62541/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/reopen62541/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/src/examples/calculator/CMakeLists.txt)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
  include(${CMAKE_CURRENT_LIST_DIR}/src/reopen62541-cli/CMakeLists.txt)
  include(${CMAKE_CURRENT_LIST_DIR}/src/tests/reopen62541-cli/CMakeLists.txt)
  include(${CMAKE_CURRENT_LIST_DIR}/src/examples/calculator-cli/CMakeLists.txt)
endif()
