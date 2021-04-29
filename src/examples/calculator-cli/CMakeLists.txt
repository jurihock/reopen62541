include(CSharpUtilities)

# SERVER

file(GLOB SOURCES ${CMAKE_CURRENT_LIST_DIR}/Server.cs ${CMAKE_CURRENT_LIST_DIR}/Interface.cs ${CMAKE_CURRENT_LIST_DIR}/Implementation.cs)

add_executable(examples-calculator-cli-server ${SOURCES})
add_dependencies(examples-calculator-cli-server reopen62541-cli)

set_target_properties(examples-calculator-cli-server PROPERTIES WIN32_EXECUTABLE FALSE)
set_target_properties(examples-calculator-cli-server PROPERTIES VS_DOTNET_REFERENCES "System")

# enable unmanaged debugging
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
  set_target_properties(examples-calculator-cli-server PROPERTIES VS_GLOBAL_EnableUnmanagedDebugging "true")
endif()

# CLIENT

file(GLOB SOURCES ${CMAKE_CURRENT_LIST_DIR}/Client.cs ${CMAKE_CURRENT_LIST_DIR}/Interface.cs)

add_executable(examples-calculator-cli-client ${SOURCES})
add_dependencies(examples-calculator-cli-client reopen62541-cli)

set_target_properties(examples-calculator-cli-client PROPERTIES WIN32_EXECUTABLE FALSE)
set_target_properties(examples-calculator-cli-client PROPERTIES VS_DOTNET_REFERENCES "System")

# enable unmanaged debugging
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
  set_target_properties(examples-calculator-cli-client PROPERTIES VS_GLOBAL_EnableUnmanagedDebugging "true")
endif()
