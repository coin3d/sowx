add_subdirectory(simple)
add_subdirectory(wxFormBuilder)
add_subdirectory(uiaction)

add_executable(test_egl  test_egl.cpp )
target_link_libraries(test_egl ${wxWidgets_LIBRARIES})
