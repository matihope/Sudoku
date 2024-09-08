#include "SudokuBoardEntity.hpp"
#include "Utils/Converters.hpp"

SudokuBoardEntity::SudokuBoardEntity() {
	mk::JsonBridge colors("colors.json");

	even_color = mk::Converters::colorFromStrRGB(colors["board"]["even"]);
	odd_color  = mk::Converters::colorFromStrRGB(colors["board"]["odd"]);
	line_color = mk::Converters::colorFromStrRGB(colors["board"]["line"]);
}

void SudokuBoardEntity::onReady(mk::Game& game) {
	tiles = addChild<mk::SpriteBatch>(game);
	tiles->setSize(81);

	auto [view_width, view_height] = game.getViewportSize();
	square3x3_size                 = (float) std::min(view_width, view_height) / BOARD_SIZE;

	spawnTiles(game, square3x3_size);
	spawnLines(game, square3x3_size);

	for (sudoku::SudokuValue col: sudoku::value_range) {
		for (sudoku::SudokuValue row: sudoku::value_range) {
			auto&& text_color = (row() + col()) % 2 ? even_color : odd_color;
			label_tiles[col()][row()]
				= addChild<SudokuTileEntity>(game, col(), row(), square3x3_size, text_color);
			label_tiles[col()][row()]->setMainDigit(row());
		}
	}
}

void SudokuBoardEntity::spawnTiles(mk::Game& game, float tile_size) {
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			auto&& sprite = tiles->getSprite(y * BOARD_SIZE + x);
			sprite.setPosition((mk::math::Vector2f(x, y) * tile_size).as<sf::Vector2f>());
			sprite.setSize({ tile_size, tile_size });
			if ((x + y) % 2)
				sprite.setColor(odd_color);
			else
				sprite.setColor(even_color);
		}
	}
}

void SudokuBoardEntity::spawnLines(mk::Game& game, float tile_size) {
	lines = addChild<mk::SpriteBatch>(game);
	lines->setSize(4);
	float line_thickness = 4.f;
	float line_length    = tile_size * BOARD_SIZE;
	for (int i = 0; i < 4; i++) {
		auto&& sprite = lines->getSprite(i);
		auto   pos    = tile_size * SQUARE_COUNT;
		if (i % 2) pos = pos * 2;
		if (i < 2) {
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
	for (auto&& column: sudoku::value_range) {
		for (auto&& row: sudoku::value_range) {
			auto&& board_tile = board_data(column, row);
			auto&& my_tile    = label_tiles[column()][row()];
			my_tile->clear();
			if (board_tile.main_digit.has_value())
				my_tile->setMainDigit(board_tile.main_digit.value());
			for (auto&& note_digit: sudoku::value_range)
				my_tile->setNote(note_digit, board_tile.note_digits[note_digit()]);
		}
	}
}

mk::RectF SudokuBoardEntity::getBounds() const {
	auto [x, y] = getPosition();
	return { x, y, square3x3_size * 9.f, square3x3_size * 9.f };
}
