cmake_minimum_required(VERSION 3.17)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
  project(reopen62541 LANGUAGES C CXX CSharp)
  # TODO cleanup
  # add_definitions(-DWIN32_LEAN_AND_MEAN)
  # set(CMAKE_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7")
  # set_property(TARGET ${targetName} PROPERTY COMMON_LANGUAGE_RUNTIME "netcore")
  # set_property(TARGET ${targetName} PROPERTY DOTNET_TARGET_FRAMEWORK "net10-windows")
else()
  project(reopen62541 LANGUAGES C CXX)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/open62541)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/reopen62541)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/examples/calculator)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/reopen62541-cli)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/tests/reopen62541-cli)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/examples/calculator-cli)
endif()
