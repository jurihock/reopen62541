add_executable(examples-calculator-server ${CMAKE_CURRENT_LIST_DIR}/server.cpp)
target_link_libraries(examples-calculator-server reopen62541)

add_executable(examples-calculator-client ${CMAKE_CURRENT_LIST_DIR}/client.cpp)
target_link_libraries(examples-calculator-client reopen62541)
