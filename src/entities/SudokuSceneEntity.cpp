#include "SudokuSceneEntity.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/Timer.hpp"
#include "Math/Vector2.hpp"
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
	spawnButtonsAndLabels(game);
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
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) undo();
}

void SudokuScene::spawnButtonsAndLabels(mk::Game& game) {
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

	// Difficulty label
	auto diff_and_mistakes_center = top_left - mk::math::Vector2f(0.f, 40.f + padding);
	auto diff_diff                = 10.0;
	auto diff_label               = addChild<mk::gui::Label>(
        game, game.getDefaultFont(), "Difficulty: " + sudoku::difficultyToString(difficulty)
    );
	diff_label->setPosition(
		(diff_and_mistakes_center - mk::math::Vector2f(0.f, diff_diff)).as<sf::Vector2f>()
	);
	diff_label->setAlignment(mk::gui::HAlignment::LEFT, mk::gui::VAlignment::BOTTOM);
	diff_label->setTextSize(30);

	// Timer label
	timer = addChild<mk::gui::Timer>(
		game, std::move(mk::gui::Timer::Increasing(game.getDefaultFont()))
	);
	timer->setPosition(diff_and_mistakes_center.as<sf::Vector2f>());
	timer->setAlignment(mk::gui::HAlignment::RIGHT, mk::gui::VAlignment::CENTER);
	timer->move(menu_button_width, 0.0);
	timer->enableHours(true);

	// Mistake label
	mistake_label = addChild<mk::gui::Label>(game, game.getDefaultFont(), "Mistakes: 0");
	mistake_label->setAlignment(mk::gui::HAlignment::LEFT, mk::gui::VAlignment::TOP);
	mistake_label->setPosition(
		(diff_and_mistakes_center + mk::math::Vector2f(0.f, diff_diff)).as<sf::Vector2f>()
	);
}

void SudokuScene::onUpdate(mk::Game& game, float dt) {
	if (menu_button->isPressed()) game.popScene();

	for (sudoku::SudokuValue digit: sudoku::value_range) {
		auto&& digit_button = number_buttons[digit()];
		if (digit_button->isPressed()) handlePutDigit(digit);
	}

	if (undo_button->isPressed()) undo();

	if (note_button->isPressed()) {
		taking_notes = !taking_notes;
		note_button->setText(std::string("Notes: ") + (taking_notes ? "On" : "Off"));
	}


	timer->setStop(sudoku.isOver());
}

void SudokuScene::handlePutDigit(sudoku::SudokuValue digit) {
	auto tile = board->getSelectedTile();
	if (tile.has_value()) {
		auto [col, row] = *tile;
		if (taking_notes)
			sudoku.toggleNote(col, row, digit);
		else if (!sudoku.tryPlay(col, row, digit)) {
			mistake_counter++;
			mistake_label->setText(std::string("Mistakes: ") + std::to_string(mistake_counter));
		}


		board->load(sudoku.getBoard());
	}
}

void SudokuScene::undo() {
	sudoku.undo();
	board->load(sudoku.getBoard());
}
