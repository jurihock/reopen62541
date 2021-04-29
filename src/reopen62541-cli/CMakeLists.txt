file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_library(reopen62541-cli SHARED ${HEADERS} ${SOURCES})
target_include_directories(reopen62541-cli PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(reopen62541-cli reopen62541 open62541)

set_target_properties(reopen62541-cli PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(reopen62541-cli PROPERTIES VS_DOTNET_REFERENCES "System")

# enable unmanaged debugging
target_link_options(reopen62541-cli PUBLIC $<$<OR:$<CONFIG:>,$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:/DEBUG /ASSEMBLYDEBUG>)
