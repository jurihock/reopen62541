file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

# source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${HEADERS} ${SOURCES})

add_library(reopen62541 ${HEADERS} ${SOURCES})

target_include_directories(reopen62541 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)

target_compile_features(reopen62541 PUBLIC cxx_std_17)

target_link_libraries(reopen62541 open62541)
