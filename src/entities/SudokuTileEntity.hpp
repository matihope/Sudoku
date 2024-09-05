#pragma once

#include "GUI/Label.hpp"
#include "Game/Game.hpp"
#include "WorldEntity/WorldEntity.hpp"
#include "../sudoku/SudokuGame.hpp"

class SudokuTileEntity: public mk::WorldEntity {
public:
	SudokuTileEntity(
		sudoku::SudokuValue column, sudoku::SudokuValue row, float tile_size, sf::Color text_color
	);

	void onReady(mk::Game& game) override;

	void setMainDigit(sudoku::SudokuValue value);
	void setNote(sudoku::SudokuValue digit, bool show);
	void clear();

private:
	sf::Color       text_color{};
	float           tile_size{};
	mk::GUI::Label* main_digit{};
	mk::GUI::Label* note_digits[9]{};
};
