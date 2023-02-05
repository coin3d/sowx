enable_testing()

include(${wxWidgets_USE_FILE})
add_definitions(-DWXINTL_NO_GETTEXT_MACRO -DSOWX_INTERNAL=1)

include_directories(
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${wxWidgets_INCLUDE_DIRS}
)

add_subdirectory(devices)
add_subdirectory(widgets)

set(TEST_NAME test_soww)
add_executable(${TEST_NAME} TestSuiteMain.cpp TestSoWx.cpp)
target_link_libraries(${TEST_NAME}  SoWx )

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})