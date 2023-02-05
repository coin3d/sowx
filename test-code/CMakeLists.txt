function(executable executable_name)
  set(multiValueArgs SOURCES LIBS)
  cmake_parse_arguments(executable "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  add_executable(${executable_name} ${executable_SOURCES})
  target_link_libraries(${executable_name} ${executable_LIBS})
endfunction(executable)


    if(MSVC)
   #     set_target_properties(${executable_name}  PROPERTIES    LINK_FLAGS /SUBSYSTEM:WINDOWS)
    endif()


dump_variable(
Coin_LIBRARY
WXWIDGETS_LIBRARIES
)

include(${wxWidgets_USE_FILE})
add_definitions(-DWXINTL_NO_GETTEXT_MACRO)

include_directories(
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${wxWidgets_INCLUDE_DIRS}
)

add_subdirectory(assertdbg)
add_subdirectory(components)
add_subdirectory(Mentor)
add_subdirectory(viewers)
add_subdirectory(widgets)
add_subdirectory(wxWidgets)