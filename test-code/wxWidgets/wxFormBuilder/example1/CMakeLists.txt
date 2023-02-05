add_executable(example_1 example_1.cpp main.cpp MyFrame.cpp)
#message(FATAL_ERROR ${Coin_LIBRARIES})
target_link_libraries(example_1 ${wxWidgets_LIBRARIES} SoWx)