file(READ ${CMAKE_SOURCE_DIR}/VERSION VERSION)
string(STRIP "${VERSION}" VERSION)
set(VERSION "${VERSION}.0.0.0")
string(REPLACE "." ";" VERSION ${VERSION})
list(GET VERSION 0 1 2 3 VERSION)
list(JOIN VERSION "." DOTVERSION)
list(JOIN VERSION "," COMVERSION)

set(AssemblyInfo ${CMAKE_CURRENT_LIST_DIR}/AssemblyInfo.cpp)
file(WRITE  ${AssemblyInfo} "using namespace System::Reflection;\n\n")
file(APPEND ${AssemblyInfo} "[assembly:AssemblyTitleAttribute(\"reopen62541-cli\")];\n")
file(APPEND ${AssemblyInfo} "[assembly:AssemblyDescriptionAttribute(\"https://github.com/jurihock/reopen62541\")];\n")
file(APPEND ${AssemblyInfo} "[assembly:AssemblyVersionAttribute(\"${DOTVERSION}\")];\n")
file(APPEND ${AssemblyInfo} "[assembly:AssemblyFileVersionAttribute(\"${DOTVERSION}\")];\n")

set(AssemblyInfo ${CMAKE_CURRENT_LIST_DIR}/AssemblyInfo.h)
file(WRITE  ${AssemblyInfo} "#define VERSION_PRODUCTNAME_STRING     \"reopen62541\\0\"\n")
file(APPEND ${AssemblyInfo} "#define VERSION_FILEDESCRIPTION_STRING \"https://github.com/jurihock/reopen62541\\0\"\n")
file(APPEND ${AssemblyInfo} "#define VERSION_PRODUCTVERSION         ${COMVERSION}\n")
file(APPEND ${AssemblyInfo} "#define VERSION_PRODUCTVERSION_STRING  \"${DOTVERSION}\\0\"\n")
file(APPEND ${AssemblyInfo} "#define VERSION_FILEVERSION            ${COMVERSION}\n")
file(APPEND ${AssemblyInfo} "#define VERSION_FILEVERSION_STRING     \"${DOTVERSION}\\0\"\n")
file(APPEND ${AssemblyInfo} "")

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB_RECURSE RESOURCES ${CMAKE_CURRENT_LIST_DIR}/*.rc)

add_library(reopen62541-cli SHARED ${HEADERS} ${SOURCES} ${RESOURCES})
target_include_directories(reopen62541-cli PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(reopen62541-cli reopen62541 open62541)

set_target_properties(reopen62541-cli PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(reopen62541-cli PROPERTIES VS_DOTNET_REFERENCES "System")

# enable unmanaged debugging
target_link_options(reopen62541-cli PUBLIC $<$<OR:$<CONFIG:>,$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:/DEBUG /ASSEMBLYDEBUG>)

# DOXYGEN

find_package(Doxygen)

if (DOXYGEN_FOUND)

  set(DOXYFILE ${CMAKE_CURRENT_LIST_DIR}/Doxyfile.txt)

  add_custom_target(reopen62541-cli-docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Generating reopen62541-cli docs..."
    VERBATIM)

else (DOXYGEN_FOUND)

  message("-- Doxygen needs to be installed to generate reopen62541-cli docs...")

endif (DOXYGEN_FOUND)
