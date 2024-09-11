#include "SudokuTileEntity.hpp"
#include "GUI/Button.hpp"
#include "JsonBridge/JsonBridge.hpp"
#include "Nodes/RectShape.hpp"
#include "SudokuGame.hpp"
#include "Utils/Converters.hpp"

SudokuTileEntity::SudokuTileEntity(
	sudoku::SudokuValue column, sudoku::SudokuValue row, float tile_size
):
	  tile_size(tile_size) {
	mk::JsonBridge colors("colors.json");
	background_color      = mk::converters::colorFromStrRGB(colors["board"]["background"]);
	text_color            = mk::converters::colorFromStrRGB(colors["board"]["font"]);
	note_text_color       = mk::converters::colorFromStrRGB(colors["board"]["line"]);
	highlight_color       = mk::converters::colorFromStrRGB(colors["board"]["highlight"]);
	minor_highlight_color = mk::converters::colorFromStrRGB(colors["board"]["minor_highlight"]);
	wrong_color           = mk::converters::colorFromStrRGB(colors["board"]["wrong"]);

	setPosition({ (column() - 0.5f) * tile_size, (row() - 0.5f) * tile_size });
}

void SudokuTileEntity::onReady(mk::Game& game) {
	auto size = mk::math::Vector2f(tile_size, tile_size);

	background_tile = addChild<mk::RectShape>(game, background_color, size);
	background_tile->setOrigin(tile_size / 2., tile_size / 2.);
	background_tile->setOrigin(tile_size / 2., tile_size / 2.);

	tile_button = addChild<mk::gui::Button>(game, game.getDefaultFont(), "");
	tile_button->setBackgroundColors(sf::Color::Transparent);
	tile_button->setMinSize(size);
	tile_button->setOrigin((size / 2.).as<sf::Vector2f>());

	auto font  = game.getDefaultFont();
	main_digit = addChild<mk::gui::Label>(game, font);
	main_digit->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::CENTER);
	main_digit->setText("0");
	main_digit->setTextSize(64);
	main_digit->setColor(text_color);
	main_digit->hide();

	auto grid_unit = -tile_size / 3.2f;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			auto   index = y * 3 + x;
			auto&& lbl   = note_digits[index];
			lbl          = addChild<mk::gui::Label>(game, font);
			lbl->setText(std::string(1, '0' + (index + 1)));
			lbl->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::CENTER);
			lbl->setPosition({
				-grid_unit * (x - 1.f),
				-grid_unit * (y - 1.f),
			});
			lbl->setColor(note_text_color);
		}
	}
}

void SudokuTileEntity::setSquare(const sudoku::SudokuSquare& square) {
	main_digit->hide();
	if (square.main_digit.has_value()) {
		main_digit->setText(std::string(1, '0' + square.main_digit.value()()));
		main_digit->show();

		main_digit->setColor(text_color);
		if (!square.is_correct) main_digit->setColor(wrong_color);
	}
	for (auto&& note_digit: sudoku::value_range)
		if (square.note_digits[note_digit()])
			note_digits[note_digit() - 1]->show();
		else
			note_digits[note_digit() - 1]->hide();
}

void SudokuTileEntity::setHighlight() {
	background_tile->setColor(highlight_color);
	is_highlighted = true;
}

void SudokuTileEntity::unHighlight() {
	background_tile->setColor(background_color);
	is_highlighted = false;
}

bool SudokuTileEntity::isPressed() const { return tile_button->isPressed(); }

bool SudokuTileEntity::isHighlighted() const { return is_highlighted; }

void SudokuTileEntity::setMinorHighlight() { background_tile->setColor(minor_highlight_color); }

void SudokuTileEntity::setWrong() { main_digit->setColor(wrong_color); }
