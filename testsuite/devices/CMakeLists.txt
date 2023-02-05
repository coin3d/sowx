
set(TEST_NAME test_soww_devices)
add_executable(${TEST_NAME} ../TestSuiteMain.cpp TestSoWxKeyboardP.cpp)
target_link_libraries(${TEST_NAME}  SoWx )

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})