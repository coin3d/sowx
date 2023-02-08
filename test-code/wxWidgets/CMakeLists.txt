add_subdirectory(simple)
add_subdirectory(wxFormBuilder)
add_subdirectory(uiaction)

executable(test_egl SOURCES test_egl.cpp LIBS ${wxWidgets_LIBRARIES}  OPTIONAL WINDOWS)
