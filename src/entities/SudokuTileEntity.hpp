#pragma once

#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "Game/Game.hpp"
#include "Nodes/RectShape.hpp"
#include "WorldEntity/WorldEntity.hpp"
#include "../sudoku/SudokuGame.hpp"

class SudokuTileEntity: public mk::WorldEntity {
public:
	SudokuTileEntity(sudoku::SudokuValue column, sudoku::SudokuValue row, float tile_size);

	void onReady(mk::Game& game) override;

	void setSquare(const sudoku::SudokuSquare& square);

	void setHighlight();
	void setMinorHighlight();
	void unHighlight();
	void setWrong();

	bool isHighlighted() const;

	bool isPressed() const;

private:
	bool      is_highlighted = false;
	sf::Color background_color{}, text_color{}, minor_highlight_color{}, highlight_color{},
		wrong_color{}, note_text_color{};
	float           tile_size{};
	mk::GUI::Label* main_digit{};
	mk::GUI::Label* note_digits[9]{};
	mk::RectShape*  background_tile;

	mk::GUI::Button* tile_button = nullptr;
};
