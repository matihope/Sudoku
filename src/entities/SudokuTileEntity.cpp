#include "SudokuTileEntity.hpp"

SudokuTileEntity::SudokuTileEntity(
	sudoku::SudokuValue column, sudoku::SudokuValue row, float tile_size, sf::Color text_color
):
	  tile_size(tile_size),
	  text_color(text_color) {
	setPosition({ (column() - 0.5f) * tile_size, (row() - 0.5f) * tile_size });
}

void SudokuTileEntity::onReady(mk::Game& game) {
	auto font  = game.getDefaultFont();
	main_digit = addChild<mk::GUI::Label>(game, font);
	main_digit->setAlignment(mk::GUI::HAlignment::MIDDLE, mk::GUI::VAlignment::CENTER);
	main_digit->setText("0");
	main_digit->setTextSize(64);
	main_digit->setColor(text_color);
	main_digit->hide();

	auto grid_unit = -tile_size / 3.2f;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			auto   index = y * 3 + x;
			auto&& lbl   = note_digits[index];
			lbl          = addChild<mk::GUI::Label>(game, font);
			lbl->setText(std::string(1, '0' + (index + 1)));
			lbl->setAlignment(mk::GUI::HAlignment::MIDDLE, mk::GUI::VAlignment::CENTER);
			lbl->setPosition({
				-grid_unit * (x - 1.f),
				-grid_unit * (y - 1.f),
			});
			lbl->setColor(text_color);
		}
	}
}

void SudokuTileEntity::setMainDigit(sudoku::SudokuValue value) {
	main_digit->setText(std::string(1, '0' + value()));
	main_digit->show();
	for (auto&& note_digit: note_digits) note_digit->hide();
}

void SudokuTileEntity::setNote(sudoku::SudokuValue digit, bool show) {
	if (show)
		note_digits[digit() - 1]->show();
	else
		note_digits[digit() - 1]->hide();
}

void SudokuTileEntity::clear() {
	main_digit->hide();
	for (sudoku::SudokuValue digit: sudoku::value_range) setNote(digit, false);
}
