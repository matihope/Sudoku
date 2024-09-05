add_executable(${PROJECT_NAME} main.cpp
        entities/SudokuTileEntity.hpp
        entities/SudokuBoardEntity.hpp
        entities/SudokuScene.hpp
)
add_subdirectory(sudoku)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine SudokuLogic)
