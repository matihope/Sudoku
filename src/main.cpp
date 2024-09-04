#include "Game/Game.hpp"
#include "GUI/Button.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "SpriteBatch/SpriteBatch.hpp"
#include "Math/Vector2.hpp"
#include "sudoku/SudokuGame.hpp"
#include <iostream>

class SudokuTileEntity: public mk::WorldEntity {
public:
	SudokuTileEntity(
		sudoku::SudokuValue column, sudoku::SudokuValue row, float tile_size, sf::Color text_color
	):
		  tile_size(tile_size),
		  text_color(text_color) {
		setPosition({ (column() - 0.5f) * tile_size, (row() - 0.5f) * tile_size });
	}

	void onReady(mk::Game& game) override {
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

	void setMainDigit(sudoku::SudokuValue value) {
		main_digit->setText(std::string(1, '0' + value()));
		main_digit->show();
		for (auto&& note_digit: note_digits) note_digit->hide();
	}

	void setNote(sudoku::SudokuValue value, bool show) {
		if (show)
			note_digits[value() - 1]->show();
		else
			note_digits[value() - 1]->hide();
	}

	void clear() {
		main_digit->hide();
		for (int i = 1; i <= 9; i++) setNote(i, false);
	}

private:
	sf::Color       text_color;
	float           tile_size;
	mk::GUI::Label* main_digit{};
	mk::GUI::Label* note_digits[9]{};
};

class SudokuBoardEntity: public mk::WorldEntity {
	static constexpr int BOARD_SIZE   = 9;
	static constexpr int SQUARE_COUNT = 3;

public:
	void onReady(mk::Game& game) override {
		tiles = addChild<mk::SpriteBatch>(game);
		tiles->setSize(81);

		mk::JsonBridge colors("colors.json");

		using sf::Color;
		using std::min;
		using std::stoul;
		using std::string;
		auto even_color = Color(stoul(string(colors["board"]["even"]), nullptr, 16) * 256 + 0xFF);
		auto odd_color  = Color(stoul(string(colors["board"]["odd"]), nullptr, 16) * 256 + 0xFF);
		auto line_color = Color(stoul(string(colors["board"]["line"]), nullptr, 16) * 256 + 0xFF);

		auto [view_width, view_height] = game.getViewportSize();
		float sprite_size              = (float) min(view_width, view_height) / BOARD_SIZE;

		for (int x = 0; x < BOARD_SIZE; x++) {
			for (int y = 0; y < BOARD_SIZE; y++) {
				auto&& sprite = tiles->getSprite(y * BOARD_SIZE + x);
				sprite.setPosition((mk::Math::Vector2f(x, y) * sprite_size).as<sf::Vector2f>());
				sprite.setSize({ sprite_size, sprite_size });
				if ((x + y) % 2)
					sprite.setColor(odd_color);
				else
					sprite.setColor(even_color);
			}
		}

		lines = addChild<mk::SpriteBatch>(game);
		lines->setSize(4);
		float line_thickness = 4.f;
		float line_length    = sprite_size * BOARD_SIZE;
		for (int i = 0; i < 4; i++) {
			auto&& sprite = lines->getSprite(i);
			auto   pos    = sprite_size * SQUARE_COUNT;
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

		for (auto col: sudoku::value_range) {
			for (auto row: sudoku::value_range) {
				auto&& text_color = (row() + col()) % 2 ? even_color : odd_color;
				label_tiles[col()][row()]
					= addChild<SudokuTileEntity>(game, col(), row(), sprite_size, text_color);
				label_tiles[col()][row()]->setMainDigit(row());
			}
		}
	}

	void load(const sudoku::SudokuBoard& board_data) {
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

	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const override {}

private:
	mk::SpriteBatch*                                                          tiles = nullptr;
	mk::SpriteBatch*                                                          lines = nullptr;
	std::array<std::array<SudokuTileEntity*, BOARD_SIZE + 1>, BOARD_SIZE + 1> label_tiles{};
};

class SudokuScene: public mk::WorldEntity {
public:
	explicit SudokuScene(sudoku::SudokuGame::Difficulty difficulty = sudoku::SudokuGame::Difficulty::EASY):
		  sudoku(difficulty) {}

	void onReady(mk::Game& game) override {
		board = addChild<SudokuBoardEntity>(game);
		board->load(sudoku.getBoard());
	}

	void handleEvent(mk::Game& game, const sf::Event& event) override {
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
			game.replaceTopScene(std::make_unique<SudokuScene>());
	}

private:
	SudokuBoardEntity* board = nullptr;
	sudoku::SudokuGame sudoku;
};

class Menu: public mk::WorldEntity {
public:
	void onReady(mk::Game& game) override {
		auto font = &mk::ResourceManager::get().getFont("Born2bSportyV2.ttf");
		button    = addChild<mk::GUI::Button>(game, font, "Play");
		button->setAlignment(mk::GUI::HAlignment::MIDDLE, mk::GUI::VAlignment::CENTER);
		button->setMinSize({ game.getViewportSize().x / 3.f, -1 });
		button->setMinSpaceBetween({ 5, 5 });
		button->setPosition(game.getViewportSize().x / 2, game.getViewportSize().y / 2);
	}

	void onUpdate(mk::Game& game, float dt) override {
		if (button->isPressed()) game.addScene(std::make_unique<SudokuScene>());
	}

private:
	mk::GUI::Button* button = nullptr;
};

int main() {
	mk::Game game = mk::Game("settings.json");
	game.getDefaultFont()->setSmooth(false);
	game.addScene(std::make_unique<SudokuScene>());
	game.run();
}
