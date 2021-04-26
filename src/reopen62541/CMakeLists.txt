file(GLOB HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_library(reopen62541 ${HEADERS} ${SOURCES})
target_include_directories(reopen62541 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(reopen62541 open62541)
target_compile_features(reopen62541 PUBLIC cxx_std_17)

install(FILES ${HEADERS} ${SOURCES} DESTINATION reopen62541)
install(SCRIPT ${CMAKE_SOURCE_DIR}/scripts/amalgamate.cmake)

# DOXYGEN

find_package(Doxygen)

if (DOXYGEN_FOUND)

  set(DOXYFILE ${CMAKE_CURRENT_LIST_DIR}/Doxyfile.txt)

  add_custom_target(reopen62541-docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Generating reopen62541 docs..."
    VERBATIM)

else (DOXYGEN_FOUND)

  message("-- Doxygen needs to be installed to generate reopen62541 docs...")

endif (DOXYGEN_FOUND)
