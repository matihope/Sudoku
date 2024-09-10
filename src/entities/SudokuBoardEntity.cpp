#include "SudokuBoardEntity.hpp"
#include "SudokuGame.hpp"
#include "Utils/Converters.hpp"
#include <iostream>

SudokuBoardEntity::SudokuBoardEntity() {
	mk::JsonBridge colors("colors.json");

	line_color = mk::converters::colorFromStrRGB(colors["board"]["line"]);
}

void SudokuBoardEntity::onReady(mk::Game& game) {
	auto [view_width, view_height] = game.getViewportSize();
	square3x3_size                 = (float) std::min(view_width, view_height) / BOARD_SIZE;

	spawnTiles(game, square3x3_size);
	spawnLines(game, square3x3_size);
}

void SudokuBoardEntity::spawnTiles(mk::Game& game, float tile_size) {
	for (sudoku::SudokuValue col: sudoku::value_range) {
		for (sudoku::SudokuValue row: sudoku::value_range) {
			label_tiles[col()][row()]
				= addChild<SudokuTileEntity>(game, col(), row(), square3x3_size);
		}
	}
}

void SudokuBoardEntity::spawnLines(mk::Game& game, float tile_size) {
	lines    = addChild<mk::SpriteBatch>(game);
	uint num = 8;
	lines->setSize(num * 2);
	float line_length = tile_size * BOARD_SIZE;
	for (int i = 0; i < num * 2; i++) {
		auto&& sprite         = lines->getSprite(i);
		auto   pos            = tile_size * (i % num + 1);
		float  line_thickness = 2.f;
		if ((i % num) % 3 == 2) line_thickness *= 2.f;
		if (i < 8) {
			// Vertical
			sprite.setPosition({ pos - line_thickness / 2, 0. });
			sprite.setSize({ line_thickness, line_length });
		} else {
			// Horizontal
			sprite.setPosition({ 0., pos - line_thickness / 2 });
			sprite.setSize({ line_length, line_thickness });
		}
		sprite.setColor(line_color);
	}
}

void SudokuBoardEntity::load(const sudoku::SudokuBoard& board_data) {
	for (auto&& column: sudoku::value_range)
		for (auto&& row: sudoku::value_range)
			label_tiles[column()][row()]->setSquare(board_data(column, row));
}

mk::RectF SudokuBoardEntity::getBounds() const {
	auto [x, y] = getPosition();
	return { x, y, square3x3_size * 9.f, square3x3_size * 9.f };
}

void SudokuBoardEntity::onUpdate(mk::Game& game, float dt) {
	bool                found_pressed = false;
	sudoku::SudokuValue press_column{ 1 }, press_row{ 1 };
	for (sudoku::SudokuValue col: sudoku::value_range) {
		for (sudoku::SudokuValue row: sudoku::value_range) {
			auto&& tile = label_tiles[col()][row()];
			if (tile->isPressed()) {
				found_pressed = true;
				press_column  = col;
				press_row     = row;
			}
		}
	}
	if (found_pressed) {
		// Disable highlight
		for (sudoku::SudokuValue col: sudoku::value_range)
			for (sudoku::SudokuValue row: sudoku::value_range)
				label_tiles[col()][row()]->unHighlight();

		// Set minor highlights
		// Rows
		for (sudoku::SudokuValue col: sudoku::value_range)
			label_tiles[col()][press_row()]->setMinorHighlight();
		// Columns
		for (sudoku::SudokuValue row: sudoku::value_range)
			label_tiles[press_column()][row()]->setMinorHighlight();
		// Neighboring square
		auto sq3x3_rows = sudoku::SudokuGame::getColOrRowRangeForValue(press_row);
		auto sq3x3_cols = sudoku::SudokuGame::getColOrRowRangeForValue(press_column);
		for (sudoku::SudokuValue col: sq3x3_cols)
			for (sudoku::SudokuValue row: sq3x3_rows)
				label_tiles[col()][row()]->setMinorHighlight();
		label_tiles[press_column()][press_row()]->setHighlight();
	}
}

std::optional<std::pair<sudoku::SudokuValue, sudoku::SudokuValue>>
	SudokuBoardEntity::getSelectedTile() const {
	for (sudoku::SudokuValue col: sudoku::value_range)
		for (sudoku::SudokuValue row: sudoku::value_range)
			if (label_tiles[col()][row()]->isHighlighted()) return { { col, row } };
	return {};
}
