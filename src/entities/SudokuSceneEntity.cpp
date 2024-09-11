#include "SudokuSceneEntity.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SudokuGame.hpp"
#include "Utils/Converters.hpp"
#include <cstdint>

SudokuScene::SudokuScene(sudoku::SudokuGame::Difficulty difficulty):
	  difficulty(difficulty),
	  sudoku(difficulty) {}

void SudokuScene::onReady(mk::Game& game) {
	board = addChild<SudokuBoardEntity>(game);
	board->load(sudoku.getBoard());
	spawnButtons(game);
}

void SudokuScene::handleEvent(mk::Game& game, const sf::Event& event) {
	constexpr static std::array num_keys = {
		sf::Keyboard::Num0, sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3,
		sf::Keyboard::Num4, sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7,
		sf::Keyboard::Num8, sf::Keyboard::Num9,
	};
	if (event.type == sf::Event::KeyPressed)
		for (sudoku::SudokuValue digit: sudoku::value_range)
			if (event.key.code == num_keys[digit()]) handlePutDigit(digit);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		sudoku.solve();
		board->load(sudoku.getBoard());
	}
}

void SudokuScene::spawnButtons(mk::Game& game) {
	float padding = 20.f;

	// Button colors
	mk::JsonBridge colors("colors.json");
	auto           normal_color = mk::converters::colorFromStrRGB(colors["buttons"]["normal"]);
	auto           hover_color  = mk::converters::colorFromStrRGB(colors["buttons"]["hover"]);
	auto           press_color  = mk::converters::colorFromStrRGB(colors["buttons"]["press"]);
	sf::Color      font_color   = mk::converters::colorFromStrRGB(colors["board"]["background"]);

	// Menu button
	float menu_button_width = (game.getViewportSize().x - board->getBounds().width) - 2.f * padding;
	menu_button             = addChild<mk::gui::Button>(game, game.getDefaultFont(), "Menu");
	menu_button->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::BOTTOM);
	menu_button->setMinSize({ menu_button_width, 50.f });
	menu_button->setMinSpaceBetween({ 5, 5 });
	menu_button->setPosition({ (game.getViewportSize().x + board->getBounds().width) / 2.f,
	                           game.getViewportSize().y - 20.f });
	menu_button->setBackgroundColors(normal_color, hover_color, press_color);
	menu_button->setFontColors(font_color);


	// Num buttons
	mk::math::Vector2f top_left = { board->getBounds().width + padding, padding * 2 + 80.f };
	float              num_button_width = (menu_button_width - padding * 2) / 3.f;
	for (uint8_t y = 0; y < 3; y++) {
		for (uint8_t x = 0; x < 3; x++) {
			sudoku::SudokuValue value = y * 3 + x + 1;

			number_buttons[value()] = addChild<mk::gui::Button>(
				game, game.getDefaultFont(), std::string(1, '0' + value())
			);
			auto&& btn = number_buttons[value()];
			btn->setAlignment(mk::gui::HAlignment::LEFT, mk::gui::VAlignment::TOP);
			btn->setMinSize({ num_button_width, num_button_width });
			btn->setPosition((top_left
			                  + mk::math::Vector2f(
								  x * (num_button_width + padding), y * (num_button_width + padding)
							  ))
			                     .as<sf::Vector2f>());
			btn->setBackgroundColors(normal_color, hover_color, press_color);
			btn->setFontColors(font_color);
			btn->setTextSize(72);
		}
	}

	// Undo button
	undo_button = addChild<mk::gui::Button>(game, game.getDefaultFont(), "Undo");
	undo_button->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::BOTTOM);
	undo_button->setMinSize({ menu_button_width, 50.f });
	undo_button->setMinSpaceBetween({ 5, 5 });
	undo_button->setPosition({ menu_button->getPosition().x, menu_button->getPosition().y - 75.f });
	undo_button->setBackgroundColors(normal_color, hover_color, press_color);
	undo_button->setFontColors(font_color);

	// Note button
	note_button = addChild<mk::gui::Button>(game, game.getDefaultFont(), "Notes: Off");
	note_button->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::BOTTOM);
	note_button->setMinSize({ menu_button_width, 50.f });
	note_button->setMinSpaceBetween({ 5, 5 });
	note_button->setPosition({ undo_button->getPosition().x, undo_button->getPosition().y - 55.f });
	note_button->setBackgroundColors(normal_color, hover_color, press_color);
	note_button->setFontColors(font_color);

	// Difficulty and time
	auto diff_label = addChild<mk::gui::Label>(
		game, game.getDefaultFont(), "Difficulty: " + sudoku::difficultyToString(difficulty)
	);
	diff_label->setPosition((top_left - mk::math::Vector2f(0.f, 40.f + padding)).as<sf::Vector2f>()
	);
	diff_label->setAlignment(mk::gui::HAlignment::LEFT, mk::gui::VAlignment::CENTER);
	diff_label->setTextSize(30);
}

void SudokuScene::onUpdate(mk::Game& game, float dt) {
	if (menu_button->isPressed()) game.popScene();

	for (sudoku::SudokuValue digit: sudoku::value_range) {
		auto&& digit_button = number_buttons[digit()];
		if (digit_button->isPressed()) handlePutDigit(digit);
	}

	if (undo_button->isPressed()) {
		sudoku.undo();
		board->load(sudoku.getBoard());
	}
	if (note_button->isPressed()) {
		taking_notes = !taking_notes;
		note_button->setText(std::string("Notes: ") + (taking_notes ? "On" : "Off"));
	}
}

void SudokuScene::handlePutDigit(sudoku::SudokuValue digit) {
	auto tile = board->getSelectedTile();
	if (tile.has_value()) {
		auto [col, row] = *tile;
		if (taking_notes)
			sudoku.toggleNote(col, row, digit);
		else
			sudoku.tryPlay(col, row, digit);
		board->load(sudoku.getBoard());
	}
}
