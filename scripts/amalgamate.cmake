# scans SRC directory for .h and .cpp files,
# extracts include directives and source code,
# writes concatenated results to DST directory

file(READ "${CMAKE_CURRENT_LIST_DIR}/../VERSION" VERSION)
string(STRIP "${VERSION}" VERSION)

set(META)
list(APPEND META "// SOURCE  https://github.com/jurihock/reopen62541")
list(APPEND META "// VERSION ${VERSION}")
list(JOIN META "\n" META)

set(SRC "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/reopen62541")
set(DST "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}")

set(FILES exception convert strings uid argument variant client server)
set(TYPES h cpp)

foreach(TYPE ${TYPES})

  set(HEAD)
  set(BODY)

  foreach(FILE ${FILES})

    if(NOT EXISTS "${SRC}/${FILE}.${TYPE}")
      continue()
    endif()

    message("-- Amalgamating: ${SRC}/${FILE}.${TYPE}")

    file(READ "${SRC}/${FILE}.${TYPE}" LINES)

    string(STRIP "${LINES}" LINES)
    string(REPLACE ";" "\;" LINES "${LINES}")
    string(REPLACE "\n" "\n;" LINES "${LINES}")

    set(LABEL "/// reopen62541/${FILE}.${TYPE} ///")
    string(LENGTH "${LABEL}" N)
    string(REPEAT "/" ${N} ORNAMENT)

    list(APPEND BODY "\n\n")
    list(APPEND BODY "${ORNAMENT}\n")
    list(APPEND BODY "${LABEL}\n")
    list(APPEND BODY "${ORNAMENT}\n")
    list(APPEND BODY "\n")

    set(SKIP_EMPTY_LINES 1)

    foreach(LINE ${LINES})

      string(REPLACE ";" "\;" LINE "${LINE}")

      if(SKIP_EMPTY_LINES AND "${LINE}" STREQUAL "\n")
        continue()
      endif()

      if("${LINE}" MATCHES "^(namespace ua|ua::)")
        set(SKIP_EMPTY_LINES 0)
      endif()

      if("${LINE}" MATCHES "^#pragma once")
        continue()
      endif()

      if("${LINE}" MATCHES "^#include <(open|reopen)62541")
        continue()
      endif()

      if("${LINE}" MATCHES "^#include")
        list(APPEND HEAD "${LINE}")
      else()
        list(APPEND BODY "${LINE}")
      endif()

    endforeach()

  endforeach()

  if("${TYPE}" STREQUAL "h")

    list(REMOVE_DUPLICATES HEAD)
    list(SORT HEAD)

    list(PREPEND HEAD "#include \"open62541.h\"\n\n")
    list(PREPEND HEAD "#pragma once\n\n")
    list(PREPEND HEAD "${META}\n\n")

    list(JOIN HEAD "" HEAD)
    list(JOIN BODY "" BODY)

    string(STRIP "${HEAD}" HEAD)
    string(STRIP "${BODY}" BODY)

    file(WRITE "${DST}/reopen62541.h" "${HEAD}\n\n${BODY}\n")

  endif()

  if("${TYPE}" STREQUAL "cpp")

    list(REMOVE_DUPLICATES HEAD)
    list(SORT HEAD)

    list(PREPEND HEAD "#include \"reopen62541.h\"\n\n")
    list(PREPEND HEAD "${META}\n\n")

    list(JOIN HEAD "" HEAD)
    list(JOIN BODY "" BODY)

    string(STRIP "${HEAD}" HEAD)
    string(STRIP "${BODY}" BODY)

    file(WRITE "${DST}/reopen62541.cpp" "${HEAD}\n\n${BODY}\n")

  endif()

endforeach()
