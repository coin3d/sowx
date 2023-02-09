
executable(thumbwheel SOURCES thumbwheel.cpp LIBS SoWx OPTIONAL WINDOWS)

executable(gridbagsizer SOURCES gridbagsizer.cpp LIBS SoWx OPTIONAL WINDOWS)

executable(buttons SOURCES buttons.cpp LIBS SoWx OPTIONAL WINDOWS)

if(NOT MSVC)
	executable(gridbagsizer2 SOURCES gridbagsizer2.cpp LIBS SoWx OPTIONAL WINDOWS)
	executable(multisizer SOURCES multisizer.cpp LIBS SoWx OPTIONAL WINDOWS)
	executable(glarea SOURCES glarea.cpp LIBS SoWx OPTIONAL WINDOWS)
endif()
