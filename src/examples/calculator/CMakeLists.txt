set(HEADERS ${CMAKE_CURRENT_LIST_DIR}/server.h ${CMAKE_CURRENT_LIST_DIR}/interface.h ${CMAKE_CURRENT_LIST_DIR}/implementation.h)
set(SOURCES ${CMAKE_CURRENT_LIST_DIR}/server.cpp)

add_executable(examples-calculator-server ${HEADERS} ${SOURCES})
target_include_directories(examples-calculator-server PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(examples-calculator-server reopen62541)

set(HEADERS ${CMAKE_CURRENT_LIST_DIR}/client.h ${CMAKE_CURRENT_LIST_DIR}/interface.h)
set(SOURCES ${CMAKE_CURRENT_LIST_DIR}/client.cpp)

add_executable(examples-calculator-client ${HEADERS} ${SOURCES})
target_include_directories(examples-calculator-client PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(examples-calculator-client reopen62541)
