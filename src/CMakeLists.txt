add_executable(${PROJECT_NAME} main.cpp
        entities/SudokuTileEntity.hpp
        entities/SudokuTileEntity.cpp
        entities/SudokuBoardEntity.hpp
        entities/SudokuBoardEntity.cpp
        entities/SudokuSceneEntity.hpp
        entities/SudokuSceneEntity.cpp
)
add_subdirectory(sudoku)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine SudokuLogic)
