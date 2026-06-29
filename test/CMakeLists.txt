# test/CMakeLists.txt
# Unit tests with GoogleTest (fetched at configure time). The real implementation sources are
# compiled into the test binary so the tests link against them directly.
add_executable(
  unit_tests
  test_simple.cpp
  test_math.cpp
  ${CMAKE_SOURCE_DIR}/src/math.cpp)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
  SYSTEM)
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(unit_tests PRIVATE GTest::gtest)

include(GoogleTest)
gtest_discover_tests(unit_tests)
