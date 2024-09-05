add_library(SudokuLogic SudokuGame.hpp SudokuGame.cpp)
target_link_libraries(SudokuLogic PRIVATE Engine)
target_include_directories(SudokuLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
