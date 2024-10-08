configure_file(config.h.cmake.in config.h)

# Processing files and headers common to all SoGui libraries
# This is pulled from the subrepo SoGui.
set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Inventor/${Gui}/common)
add_subdirectory(${COMMON_DIR})

if(HAVE_NEW_MATERIALS)
  configure_file(${COMMON_DIR}/SoAnyMaterialList.cpp.in ${CMAKE_BINARY_DIR}/src/Inventor/${Gui}/SoAnyMaterialList.cpp)
  configure_file(${COMMON_DIR}/SoAnyMaterialList.h.in ${CMAKE_BINARY_DIR}/src/Inventor/${Gui}/SoAnyMaterialList.h)
  configure_file(${COMMON_DIR}/SoGuiMaterialList.h.in ${CMAKE_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}MaterialList.h)
  configure_file(${COMMON_DIR}/SoAnyMaterialListBuiltins.cpp.in ${CMAKE_BINARY_DIR}/src/Inventor/${Gui}/SoAnyMaterialListBuiltins.cpp)
endif()

if(SO${GUI}_USE_OIV)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
  set(thirdparty_SRCS thirdparty/Inventor/C/threads/mutex.cpp)
endif()

file(GLOB_RECURSE COMMON_HDRS ${CMAKE_CURRENT_BINARY_DIR}/*.h)
file(GLOB_RECURSE COMMON_SRCS ${CMAKE_CURRENT_BINARY_DIR}/*.cpp)

set(HDRS
  Inventor/${Gui}/So${Gui}Clipboard.h               # added
  Inventor/${Gui}/So${Gui}ComponentP.h
  Inventor/${Gui}/So${Gui}DirectionalLightEditor.h  # added
  Inventor/${Gui}/So${Gui}GLWidgetP.h
  #Inventor/${Gui}/So${Gui}ImageReader.h            # missing
  Inventor/${Gui}/So${Gui}Internal.h
  # Inventor/${Gui}/So${Gui}LightSliderSet.h          # added
  # Inventor/${Gui}/So${Gui}MaterialSliderSet.h       # added
  # Inventor/${Gui}/So${Gui}SignalThread.h           # missing
  # Inventor/${Gui}/So${Gui}SliderSetBase.h           # added
  # Inventor/${Gui}/So${Gui}SliderSet.h               # added
  # Inventor/${Gui}/So${Gui}TransformSliderSet.h      # added
  Inventor/${Gui}/So${Gui}DirectionalLightEditor.h  # added
  Inventor/${Gui}/So${Gui}GraphEditor.h             # added
  Inventor/${Gui}/devices/6DOFEvents.h
  Inventor/${Gui}/devices/So${Gui}DeviceP.h
  Inventor/${Gui}/devices/So${Gui}SpacenavDevice.h
  Inventor/${Gui}/viewers/So${Gui}ExaminerViewerP.h
  Inventor/${Gui}/viewers/So${Gui}FullViewerP.h
  Inventor/${Gui}/viewers/So${Gui}PlaneViewerP.h
  Inventor/${Gui}/widgets/So${Gui}GLArea.h
  Inventor/${Gui}/widgets/${Gui}NativePopupMenu.h
  # Inventor/${Gui}/widgets/So${Gui}Slider.h          # added
  Inventor/${Gui}/widgets/So${Gui}ThumbWheel.h
  Inventor/${Gui}/widgets/WheelEvents.h     # added
)

if(HAVE_NEW_MATERIALS)
  list(HDRS APPEND
          ${CMAKE_BINARY_DIR}/src/Inventor/${Gui}/SoAnyMaterialList.h # added
          ${CMAKE_BINARY_DIR}/src/Inventor/${Gui}/So${Gui}MaterialList.h # added
          )
endif()

set(SRCS
  Inventor/${Gui}/So${Gui}.cpp
  Inventor/${Gui}/So${Gui}P.cpp #added
  Inventor/${Gui}/So${Gui}Clipboard.cpp #added
  Inventor/${Gui}/So${Gui}Component.cpp
  Inventor/${Gui}/So${Gui}ComponentP.cpp #added
  Inventor/${Gui}/So${Gui}DirectionalLightEditor.cpp #added
  Inventor/${Gui}/So${Gui}GLWidget.cpp
  Inventor/${Gui}/So${Gui}GLWidgetP.cpp #added
  Inventor/${Gui}/So${Gui}Internal.cpp #added
  # Inventor/${Gui}/So${Gui}LightSliderSet.cpp #added
  Inventor/${Gui}/So${Gui}MaterialList.cpp           # FIXME why not?!
  # Inventor/${Gui}/So${Gui}MaterialSliderSet.cpp #added
  # Inventor/${Gui}/So${Gui}SliderSetBase.cpp #added
  # Inventor/${Gui}/So${Gui}SliderSet.cpp #added
  # Inventor/${Gui}/So${Gui}TransformSliderSet.cpp #added
  # Inventor/${Gui}/devices/6DOFEvents.cpp             # missing
  Inventor/${Gui}/So${Gui}DirectionalLightEditor.cpp
  Inventor/${Gui}/So${Gui}GraphEditor.cpp             # added
  Inventor/${Gui}/devices/So${Gui}Device.cpp
  Inventor/${Gui}/devices/So${Gui}InputFocus.cpp
  Inventor/${Gui}/devices/So${Gui}Keyboard.cpp
  Inventor/${Gui}/devices/So${Gui}KeyboardP.cpp # added
  Inventor/${Gui}/devices/So${Gui}Mouse.cpp
  Inventor/${Gui}/devices/So${Gui}SpacenavDevice.cpp
  Inventor/${Gui}/viewers/ExaminerViewer.cpp
  Inventor/${Gui}/viewers/So${Gui}ExaminerViewerP.cpp # added
  Inventor/${Gui}/viewers/FullViewer.cpp
  Inventor/${Gui}/viewers/So${Gui}FullViewerP.cpp  # added
  Inventor/${Gui}/viewers/PlaneViewer.cpp
  # Inventor/${Gui}/viewers/WalkViewer.cpp             # FIXME why not?!
  Inventor/${Gui}/widgets/So${Gui}GLArea.cpp
  # Inventor/${Gui}/widgets/So${Gui}Slider.cpp #added
  Inventor/${Gui}/widgets/So${Gui}ThumbWheel.cpp
  Inventor/${Gui}/widgets/${Gui}NativePopupMenu.cpp
  Inventor/${Gui}/widgets/WheelEvents.cpp  # added
  ${thirdparty_SRCS} # For providing Coin specific code
)

if(HAVE_NEW_MATERIALS)
  list(SRCS APPEND
          ${CMAKE_BINARY_DIR}/src/Inventor/${Gui}/SoAnyMaterialList.cpp # added
          ${CMAKE_BINARY_DIR}/src/Inventor/${Gui}/SoAnyMaterialListBuiltins.cpp # added
          )
endif()


file(GLOB_RECURSE COMMON_RESOURCES ${CMAKE_BINARY_DIR}/data/materials/*.h)

if(HAVE_JOYSTICK_LINUX)
  list(APPEND HDRS Inventor/${Gui}/devices/So${Gui}LinuxJoystick.h)
  list(APPEND SRCS Inventor/${Gui}/devices/So${Gui}LinuxJoystick.cpp)
endif()

# Create the library target
if(SO${GUI}_BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${HDRS} ${COMMON_HDRS} ${SRCS} ${COMMON_SRCS} ${COMMON_RESOURCES})
else()
  add_library(${PROJECT_NAME} STATIC ${HDRS} ${COMMON_HDRS} ${SRCS} ${COMMON_SRCS} ${COMMON_RESOURCES})
endif()

if(WIN32 AND NOT MINGW)
  set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${SO${GUI}_SO_VERSION})
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES VERSION "${SO${GUI}_SO_VERSION}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" SOVERSION ${SO${GUI}_SO_VERSION})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_CONFIG_H SO${GUI}_INTERNAL SO${GUI}_DEBUG=$<CONFIG:Debug>)

if(WIN32)
  if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
  endif()
  if(SO${GUI}_BUILD_SHARED_LIBS)
    configure_file(${PROJECT_NAME}.rc.cmake.in ${PROJECT_NAME}.rc)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc")
    target_compile_definitions(${PROJECT_NAME} PRIVATE SO${GUI}_MAKE_DLL INTERFACE SO${GUI}_DLL)
  else()
    target_compile_definitions(${PROJECT_NAME} INTERFACE SO${GUI}_NOT_DLL)
    if(MSVC)
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7")                       # Override default /Zi to embed
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Z7")     # debugging info in the .lib.
    endif()
  endif()
elseif(APPLE)
  if(SO${GUI}_BUILD_MAC_FRAMEWORK)
    # Set the framework target properties just to make sure the relevant links inside the framework 
    # are created.  Because the default framework name may differ from the project name we change the
    # output name property.
    set_target_properties(${PROJECT_NAME} PROPERTIES
      FRAMEWORK TRUE
      FRAMEWORK_VERSION ${SO${GUI}_MAC_FRAMEWORK_VERSION}
      #PUBLIC_HEADER "${SO${GUI}_MAC_FRAMEWORK_HEADER}"
      #PRIVATE_HEADER "${SO${GUI}_PRIVATE_HEADERS}"
      RESOURCE "${CMAKE_BINARY_DIR}/version.plist"
      MACOSX_FRAMEWORK_IDENTIFIER org.coin3d.SoWx.framework
      MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PROJECT_VERSION}
      MACOSX_FRAMEWORK_BUNDLE_VERSION ${PROJECT_VERSION}
      MACOSX_FRAMEWORK_INFO_PLIST "${CMAKE_BINARY_DIR}/Info.plist"
      OUTPUT_NAME ${SO${GUI}_MAC_FRAMEWORK_NAME}
    )
    # adapt install directory to allow distribution of dylibs/frameworks in users framework/application bundle
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "@rpath")
    if(NOT CMAKE_SKIP_BUILD_RPATH)
      if(CMAKE_VERSION VERSION_LESS 3.9)
        set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
      else()
        set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_NAME_DIR TRUE)
      endif()
    endif()
  endif()
endif()

include(${wxWidgets_USE_FILE})

add_definitions(-DWXINTL_NO_GETTEXT_MACRO)
target_include_directories(${PROJECT_NAME}
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  PUBLIC
    ${wxWidgets_INCLUDE_DIRS}
)
if(NOT SO${GUI}_BUILD_MAC_FRAMEWORK)
  target_include_directories(${PROJECT_NAME} INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endif()

if(NOT SO${GUI}_USE_OIV)
	target_link_libraries(${PROJECT_NAME} Coin::Coin ${wxWidgets_LIBRARIES} OpenGL::GL)
else()
	target_link_libraries(${PROJECT_NAME} ${OIV_LIBRARY} ${wxWidgets_LIBRARIES} OpenGL::GL)
endif()


# Add a target to generate API documentation with Doxygen
if(SO${GUI}_BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

  if(NOT "${Coin_DOC_DIR}" STREQUAL "")
    get_filename_component(_coin_versioned_dir ${Coin_DOC_DIR} NAME)
    set(DOXYGEN_TAGFILES "${Coin_DOC_DIR}/html/Coin.tag=../../${_coin_versioned_dir}/html")
  endif()

# ############################################################################
# Setup documentation options
# ############################################################################
  set(GENERATE_HTMLHELP NO)
  set(DOXYGEN_GENERATE_MAN NO)
  set(GENERATE_QHP NO)
  set(GENERATE_TREEVIEW YES)
  set(HHC_PROGRAM)
  if(WIN32)
    if(SO${GUI}_BUILD_DOC_CHM)
      find_program(HHC_PROGRAM NAMES hhc.exe PATHS "C:/Program Files/HTML Help Workshop" "C:/Program Files (x86)/HTML Help Workshop" DOC "HTML Help Compiler program")
      if(NOT HHC_PROGRAM)
        message(FATAL_ERROR "Missing program HTML Help Compiler")
      else()
        set(GENERATE_HTMLHELP YES)
        set(GENERATE_TREEVIEW NO)
        mark_as_advanced(HHC_PROGRAM)
      endif()
    endif()
  else()
    if(SO${GUI}_BUILD_DOC_MAN)
      set(DOXYGEN_GENERATE_MAN YES)
    endif()
  endif()
  if(SO${GUI}_BUILD_DOC_QTHELP)
    find_program(QHG_LOCATION NAMES qhelpgenerator qhelpgenerator-qt5 DOC "Qt qhelpgenerator")
    if(NOT QHG_LOCATION)
      message(FATAL_ERROR "Missing program Qt qhelpgenerator")
    else()
      set(GENERATE_QHP YES)
      mark_as_advanced(QHG_LOCATION)
    endif()
  endif()

  set(DOXYGEN_INTERNAL_DOCS NO)
  set(DOXYGEN_EXTRACT_PRIVATE NO)
  set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
  set(DOXYGEN_EXCLUDE)

  set(DOXYFILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  set(DOXYGEN_OUTPUT "${CMAKE_BINARY_DIR}/html/index.html")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Inventor/${Gui}/common/sogui.doxygen.cmake.in" ${DOXYFILE} @ONLY)

# ############################################################################
# Setup documentation targets
# ############################################################################
  add_custom_command(
    OUTPUT ${DOXYGEN_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E echo_append "Generating API documentation with Doxygen "
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    COMMAND ${CMAKE_COMMAND} -E echo "done."
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS ${DOXYFILE}
  )
  add_custom_target(documentation ALL DEPENDS ${DOXYGEN_OUTPUT})

# ############################################################################
# Install built documentation files
# ############################################################################
  if(NOT SO${GUI}_BUILD_MAC_FRAMEWORK)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/html" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation REGEX ".*\\.(chm|qch)" EXCLUDE)
    if(GENERATE_HTMLHELP)
      install(FILES "${CMAKE_BINARY_DIR}/html/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.chm" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation)
    endif()
    if(GENERATE_QHP)
      install(FILES "${CMAKE_BINARY_DIR}/html/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.qch" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation)
    endif()
    if(DOXYGEN_GENERATE_MAN)
      install(DIRECTORY "${CMAKE_BINARY_DIR}/man/man3" DESTINATION ${CMAKE_INSTALL_MANDIR} COMPONENT documentation)
      message(STATUS "CMAKE_INSTALL_MANDIR ${CMAKE_INSTALL_MANDIR}")
    endif()
  endif()
endif()

# Add a target to generate new modern API documentation with Doxygen
if(SO${GUI}_BUILD_AWESOME_DOCUMENTATION)
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

  if(NOT "${Coin_DOC_DIR}" STREQUAL "")
    get_filename_component(_coin_versioned_dir ${Coin_DOC_DIR} NAME)
    set(DOXYGEN_TAGFILES "${Coin_DOC_DIR}/html/Coin.tag=../../${_coin_versioned_dir}/html")
  endif()

# ############################################################################
# Setup documentation options
# ############################################################################
  set(GENERATE_HTMLHELP NO)
  set(DOXYGEN_GENERATE_MAN NO)
  set(GENERATE_QHP NO)
  set(GENERATE_TREEVIEW YES)
  set(DOXYGEN_INTERNAL_DOCS NO)
  set(DOXYGEN_EXTRACT_PRIVATE NO)
  set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
  set(DOXYGEN_EXCLUDE)
  set(GITHUB_LINK "https://github.com/coin3d/so${gui}")

  set(DOXYFILE_AWESOME "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_awesome")
  set(HTML_HEADER_AWESOME "${CMAKE_BINARY_DIR}/header_awesome.html")
  set(DOXYGEN_OUTPUT_AWESOME "${CMAKE_BINARY_DIR}/html_awesome/index.html")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Inventor/${Gui}/common/sogui.doxygen.awesome.cmake.in" ${DOXYFILE_AWESOME} @ONLY)
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Inventor/${Gui}/common/docs/doxygen-awesome/doxygen-custom/header.html.cmake.in" ${HTML_HEADER_AWESOME} @ONLY)

# ############################################################################
# Setup documentation targets
# ############################################################################
  add_custom_command(
    OUTPUT ${DOXYGEN_OUTPUT_AWESOME}
    COMMAND ${CMAKE_COMMAND} -E echo_append "Generating modern API documentation with Doxygen "
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_AWESOME}
    COMMAND ${CMAKE_COMMAND} -E echo "done."
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS ${DOXYFILE_AWESOME}
  )
  add_custom_target(documentation_awesome ALL DEPENDS ${DOXYGEN_OUTPUT_AWESOME})

# ############################################################################
# Install built documentation files
# ############################################################################
  install(DIRECTORY "${CMAKE_BINARY_DIR}/html_awesome" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation REGEX ".*\\.(chm|qch)" EXCLUDE)
endif()

# ############################################################################
# Installation headers
# ############################################################################

set(INST_HDRS
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/So${Gui}.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/So${Gui}Basic.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/So${Gui}ColorEditor.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/So${Gui}Component.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/So${Gui}Cursor.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/So${Gui}GLWidget.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/So${Gui}MaterialEditor.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/So${Gui}Object.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/So${Gui}RenderArea.h"
  Inventor/${Gui}/So${Gui}Clipboard.h
  Inventor/${Gui}/So${Gui}DirectionalLightEditor.h
  # Inventor/${Gui}/So${Gui}LightSliderSet.h
  # Inventor/${Gui}/So${Gui}MaterialSliderSet.h
  # Inventor/${Gui}/So${Gui}PrintDialog.h
  Inventor/${Gui}/So${Gui}Resource.h
  Inventor/${Gui}/So${Gui}SliderSet.h
  # Inventor/${Gui}/So${Gui}SliderSetBase.h
  # Inventor/${Gui}/So${Gui}TransformSliderSet.h
)

set(INST_DEVICES_HDRS
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/devices/So${Gui}Device.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/devices/So${Gui}InputFocus.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/devices/So${Gui}Keyboard.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/devices/So${Gui}Mouse.h"
)


if(HAVE_JOYSTICK_LINUX)
  list(APPEND INST_DEVICES_HDRS "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/devices/So${Gui}LinuxJoystick.h")
endif()

set(INST_EDITORS_HDRS
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/editors/So${Gui}ColorEditor.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/editors/So${Gui}MaterialEditor.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/devices/So${Gui}SpacenavDevice.h"
)

set(INST_NODES_HDRS
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/nodes/SoGuiColorEditor.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/nodes/SoGuiMaterialEditor.h"
)

set(INST_VIEWERS_HDRS
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/viewers/So${Gui}ConstrainedViewer.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/viewers/So${Gui}ExaminerViewer.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/viewers/So${Gui}FlyViewer.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/viewers/So${Gui}FullViewer.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/viewers/So${Gui}PlaneViewer.h"
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/viewers/So${Gui}Viewer.h"
  #"${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/viewers/So${Gui}WalkViewer.h"
)

set(INST_WIDGETS_HDRS
  "${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}/widgets/So${Gui}PopupMenu.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Inventor/${Gui}/widgets/So${Gui}ThumbWheel.h"
)

# Setting up a framework means adding all the files and links under a framework
# directory. A single install command then moves the entire contents of the
# framework directory to the SO${GUI}_MAC_FRAMEWORK_PREFIX directory (/Library/Frameworks
# by default)
# As the PUBLIC_HEADER and RESOURCE options of install target do not support
# directory structure creation when building a framework we set the MACOSX_PACKAGE_LOCATION
# property on the source files and add them to the target. This does however not work
# for the generated documentation files.
# All non standard links must be created manually.
if(SO${GUI}_BUILD_MAC_FRAMEWORK)

# determine which subdirectory this file (header, resource) should be installed into.
function(set_macosx_properties _removable_prefixes _install_prefix _source_files)
  foreach(_file ${_source_files})
    get_filename_component(_loc "${_file}" DIRECTORY)
    foreach(_prefix ${_removable_prefixes})
      string(REPLACE "${_prefix}" "" _loc "${_loc}")
    endforeach()
    set_source_files_properties(${_file} PROPERTIES MACOSX_PACKAGE_LOCATION ${_install_prefix}${_loc})
  endforeach()
endfunction()

# ############################################################################
# Add framework headers
# ############################################################################
  set_macosx_properties("${CMAKE_CURRENT_SOURCE_DIR}/Inventor/${Gui};${CMAKE_CURRENT_BINARY_DIR}/Inventor/${Gui}"  "Headers" "${INST_HDRS};${INST_DEVICES_HDRS};${INST_EDITORS_HDRS};${INST_NODES_HDRS};${INST_VIEWERS_HDRS};${INST_WIDGETS_HDRS}")

# ############################################################################
# Add framework data files
# ############################################################################
  set_macosx_properties("${CMAKE_SOURCE_DIR}/data;${CMAKE_BINARY_DIR}/data" "Resources" "${COMMON_RESOURCES}")

# ############################################################################
# Add framework documentation files
# ############################################################################

  if(SO${GUI}_BUILD_DOCUMENTATION)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
      COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/Documentation
      COMMAND ${CMAKE_COMMAND} -E create_symlink ./Versions/Current/Documentation $<TARGET_FILE_DIR:${PROJECT_NAME}>/../../Documentation
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/html $<TARGET_FILE_DIR:${PROJECT_NAME}>/Documentation/html
      COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE_DIR:${PROJECT_NAME}>/Documentation/html/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.qch)
    if(GENERATE_QHP)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/html/${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.qch $<TARGET_FILE_DIR:${PROJECT_NAME}>/Documentation)
    endif()
    if(DOXYGEN_GENERATE_MAN)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/man/man3 $<TARGET_FILE_DIR:${PROJECT_NAME}>/Documentation/man)
    endif()
  endif()

# ############################################################################
# Add framework links
# ############################################################################

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/Libraries
    COMMAND ${CMAKE_COMMAND} -E create_symlink ./Versions/Current/Libraries $<TARGET_FILE_DIR:${PROJECT_NAME}>/../../Libraries
    COMMAND ${CMAKE_COMMAND} -E create_symlink ../${SO${GUI}_MAC_FRAMEWORK_NAME} $<TARGET_FILE_DIR:${PROJECT_NAME}>/Libraries/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${SO${GUI}_SO_VERSION}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E create_symlink ../${SO${GUI}_MAC_FRAMEWORK_NAME} $<TARGET_FILE_DIR:${PROJECT_NAME}>/Libraries/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${SO${GUI}_SO_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E create_symlink ../${SO${GUI}_MAC_FRAMEWORK_NAME} $<TARGET_FILE_DIR:${PROJECT_NAME}>/Libraries/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources/include
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources/include/Inventor
    COMMAND ${CMAKE_COMMAND} -E create_symlink ../../../Headers $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources/include/Inventor/${Gui}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )

# ############################################################################
# Install framework target
# ############################################################################

  install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
    FRAMEWORK DESTINATION . COMPONENT development
  )

else()

# ############################################################################
# Install headers
# ############################################################################

  install(FILES ${INST_HDRS}         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}         COMPONENT development)
  install(FILES ${INST_DEVICES_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/devices COMPONENT development)
  install(FILES ${INST_EDITORS_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/editors COMPONENT development)
  install(FILES ${INST_NODES_HDRS}   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/nodes   COMPONENT development)
  install(FILES ${INST_VIEWERS_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/viewers COMPONENT development)
  install(FILES ${INST_WIDGETS_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Inventor/${Gui}/widgets COMPONENT development)

# ############################################################################
# Install data files
# ############################################################################

  install(DIRECTORY ${CMAKE_BINARY_DIR}/data/materials/ DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/materials)

# ############################################################################
# Install target
# ############################################################################

  if(NOT CMAKE_VERSION VERSION_LESS 3.12)
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_COMPONENT development
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
      PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT development
      RESOURCE DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME} COMPONENT development
    )
    if(SO${GUI}_BUILD_SHARED_LIBS)
      if(MSVC)
        install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
      endif()
    endif()
  else()
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_SKIP
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
      PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT development
      RESOURCE DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME} COMPONENT development
    )
    if(SO${GUI}_BUILD_SHARED_LIBS)
      install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development NAMELINK_ONLY)
      if(MSVC)
        install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
      endif()
    endif()
  endif()
endif()

# ############################################################################
# Install pkgconfig file and CMake config package
# ############################################################################

if(SO${GUI}_BUILD_MAC_FRAMEWORK)
  set(SO${GUI}_CONFIG_PACKAGE_LOCATION ${SO${GUI}_MAC_FRAMEWORK_NAME}.framework/Resources/CMake)
  string(TOLOWER ${SO${GUI}_MAC_FRAMEWORK_NAME} SO${GUI}_CONFIG_PACKAGE_BASENAME)
else()
  set(SO${GUI}_CONFIG_PACKAGE_LOCATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION})
  string(TOLOWER ${PROJECT_NAME} SO${GUI}_CONFIG_PACKAGE_BASENAME)
  if(UNIX OR MINGW)
    configure_file("${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.pc.cmake.in" ${PROJECT_NAME}.pc @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
  endif()
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_SOURCE_DIR}/${PROJECT_NAME_LOWER}-config.cmake.in"
  ${SO${GUI}_CONFIG_PACKAGE_BASENAME}-config.cmake
  INSTALL_DESTINATION ${SO${GUI}_CONFIG_PACKAGE_LOCATION}
  PATH_VARS CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${SO${GUI}_CONFIG_PACKAGE_BASENAME}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion
)

# To make the component usable not only from the install directory but also from the build directory
export(
  TARGETS ${PROJECT_NAME}
  FILE ${SO${GUI}_CONFIG_PACKAGE_BASENAME}-export.cmake
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${SO${GUI}_CONFIG_PACKAGE_BASENAME}-config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${SO${GUI}_CONFIG_PACKAGE_BASENAME}-config-version.cmake"
  DESTINATION ${SO${GUI}_CONFIG_PACKAGE_LOCATION}
  COMPONENT development
)

install(EXPORT ${PROJECT_NAME}
  DESTINATION ${SO${GUI}_CONFIG_PACKAGE_LOCATION}
  NAMESPACE ${PROJECT_NAME}::
  FILE ${SO${GUI}_CONFIG_PACKAGE_BASENAME}-export.cmake
  COMPONENT development
)
