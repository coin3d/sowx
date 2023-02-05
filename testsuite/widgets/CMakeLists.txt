
set(TEST_NAME test_soww_widgets)
add_executable(${TEST_NAME} ../TestSuiteMain.cpp TestSoWxGLArea.cpp)
target_link_libraries(${TEST_NAME}  SoWx )

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})