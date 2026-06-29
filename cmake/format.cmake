# cmake/format.cmake
# Convenience targets for clang-format (style lives in .clang-format):
#   cmake --build <dir> --target format        # rewrite sources in place
#   cmake --build <dir> --target format-check   # fail if anything is unformatted

file(
  GLOB_RECURSE FORMAT_SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/*.hpp"
  "${CMAKE_SOURCE_DIR}/src/*.h")

find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE AND FORMAT_SOURCES)
  add_custom_target(
    format
    COMMAND ${CLANG_FORMAT_EXE} -i --style=file ${FORMAT_SOURCES}
    COMMENT "clang-format: rewriting sources in place"
    VERBATIM)
  add_custom_target(
    format-check
    COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror --style=file ${FORMAT_SOURCES}
    COMMENT "clang-format: checking sources (no changes written)"
    VERBATIM)
else()
  message(STATUS "clang-format not found; 'format' / 'format-check' targets unavailable.")
endif()
