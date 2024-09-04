add_executable(${PROJECT_NAME} main.cpp
        sudoku/SudokuGame.cpp
        sudoku/SudokuGame.hpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)
