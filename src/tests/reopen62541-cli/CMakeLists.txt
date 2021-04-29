include(CSharpUtilities)

file(GLOB SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cs)

add_library(tests-reopen62541-cli SHARED ${SOURCES})
add_dependencies(tests-reopen62541-cli reopen62541-cli)

set_target_properties(tests-reopen62541-cli PROPERTIES VS_GLOBAL_PROJECT_TYPES "{3AC096D0-A1C2-E12C-1390-A8335801FDAB};{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}")
set_target_properties(tests-reopen62541-cli PROPERTIES VS_DOTNET_REFERENCES "System;Microsoft.VisualStudio.QualityTools.UnitTestFramework")

# enable unmanaged debugging
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
  set_target_properties(tests-reopen62541-cli PROPERTIES VS_GLOBAL_EnableUnmanagedDebugging "true")
endif()
