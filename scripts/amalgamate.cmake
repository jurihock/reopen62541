set(SRC "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}/reopen62541")
set(DST "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_PREFIX}")

set(FILES convert error strings uid argument variant client server)
set(TYPES h cpp)

foreach(TYPE ${TYPES})

  set(HEAD)
  set(BODY)

  foreach(FILE ${FILES})

    if(NOT EXISTS "${SRC}/${FILE}.${TYPE}")
      continue()
    endif()

    message("Processing ${SRC}/${FILE}.${TYPE}")

    file(READ "${SRC}/${FILE}.${TYPE}" LINES)

    string(REPLACE ";" "\;" LINES "${LINES}")
    string(REPLACE "\n" "\n;" LINES "${LINES}")

    string(LENGTH "${FILE}.${TYPE}" ORNAMENT)
    MATH(EXPR ORNAMENT "${ORNAMENT} + 8")
    string(REPEAT "/" ${ORNAMENT} ORNAMENT)

    list(APPEND BODY "${ORNAMENT}\n")
    list(APPEND BODY "/// ${FILE}.${TYPE} ///\n")
    list(APPEND BODY "${ORNAMENT}\n\n")

    set(SKIP_EMPTY_LINES 1)

    foreach(LINE ${LINES})

      string(REPLACE ";" "\;" LINE "${LINE}")

      if(SKIP_EMPTY_LINES AND "${LINE}" STREQUAL "\n")
        continue()
      endif()

      if("${LINE}" MATCHES "^namespace")
        set(SKIP_EMPTY_LINES 0)
      endif()

      if("${LINE}" MATCHES "^ua::")
        set(SKIP_EMPTY_LINES 0)
      endif()

      if("${LINE}" MATCHES "^#pragma once")
        continue()
      endif()

      if("${LINE}" MATCHES "^#include <open62541")
        continue()
      endif()

      if("${LINE}" MATCHES "^#include <reopen62541")
        continue()
      endif()

      if("${LINE}" MATCHES "^#include")
        list(APPEND HEAD "${LINE}")
      else()
        list(APPEND BODY "${LINE}")
      endif()

    endforeach()

    list(APPEND BODY "\n")

  endforeach()

  if("${TYPE}" STREQUAL "h")

    list(REMOVE_DUPLICATES HEAD)
    list(SORT HEAD)

    list(PREPEND HEAD "#include \"open62541.h\"\n\n")
    list(PREPEND HEAD "#pragma once\n\n")

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

    list(JOIN HEAD "" HEAD)
    list(JOIN BODY "" BODY)

    string(STRIP "${HEAD}" HEAD)
    string(STRIP "${BODY}" BODY)

    file(WRITE "${DST}/reopen62541.cpp" "${HEAD}\n\n${BODY}\n")

  endif()

endforeach()
