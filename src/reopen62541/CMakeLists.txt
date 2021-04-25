file(GLOB HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_library(reopen62541 ${HEADERS} ${SOURCES})
target_include_directories(reopen62541 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(reopen62541 open62541)
target_compile_features(reopen62541 PUBLIC cxx_std_17)