if(NOT MSVC)
	executable(bottomtrim OPTIONAL WINDOWS SOURCES bottomtrim.cpp LIBS SoWx OPTIONAL WINDOWS)

	executable(lefttrim  SOURCES lefttrim.cpp LIBS SoWx OPTIONAL WINDOWS)

	executable(righttrim  SOURCES righttrim.cpp LIBS SoWx OPTIONAL WINDOWS)

	executable(fullviewer SOURCES fullviewer.cpp LIBS SoWx)
endif()

executable(examinerviewer SOURCES examinerviewer.cpp LIBS SoWx)

executable(embeddedexaminer SOURCES embeddedexaminer.cpp LIBS SoWx)

executable(allviewer SOURCES allviewer.cpp LIBS SoWx)

if(NOT SO${GUI}_USE_OIV)
	executable(background_canvas SOURCES background_canvas.cpp LIBS SoWx OpenGL::GL)
endif()

