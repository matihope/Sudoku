#include "SudokuSceneEntity.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "SudokuGame.hpp"
#include "Utils/Converters.hpp"
#include <cstdint>

SudokuScene::SudokuScene(sudoku::SudokuGame::Difficulty difficulty): sudoku(difficulty) {}

void SudokuScene::onReady(mk::Game& game) {
	board = addChild<SudokuBoardEntity>(game);
	board->load(sudoku.getBoard());
	spawnButtons(game);
}

void SudokuScene::handleEvent(mk::Game& game, const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
		game.replaceTopScene(std::make_unique<SudokuScene>());
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		sudoku.fill();
		board->load(sudoku.getBoard());
	}
}

void SudokuScene::spawnButtons(mk::Game& game) {
	float padding = 20.f;

	float menu_button_width = (game.getViewportSize().x - board->getBounds().width) - 2.f * padding;
	menu_button             = addChild<mk::GUI::Button>(game, game.getDefaultFont(), "Menu");
	menu_button->setAlignment(mk::GUI::HAlignment::MIDDLE, mk::GUI::VAlignment::BOTTOM);
	menu_button->setMinSize({ menu_button_width, 50.f });
	menu_button->setMinSpaceBetween({ 5, 5 });
	menu_button->setPosition({ (game.getViewportSize().x + board->getBounds().width) / 2.f,
	                           game.getViewportSize().y - 20.f });

	float num_button_width = (menu_button_width - padding * 2) / 3.f;

	mk::math::Vector2f top_left = { board->getBounds().width + padding, padding };

	mk::JsonBridge colors("colors.json");
	// auto bg_color = mk::Converters::colorFromStrRGB(const std::string &rgb)


	for (uint8_t y = 0; y < 3; y++) {
		for (uint8_t x = 0; x < 3; x++) {
			sudoku::SudokuValue value = y * 3 + x + 1;

			number_buttons[value()] = addChild<mk::GUI::Button>(
				game, game.getDefaultFont(), std::string(1, '0' + value())
			);
			auto&& btn = number_buttons[value()];
			btn->setAlignment(mk::GUI::HAlignment::LEFT, mk::GUI::VAlignment::TOP);
			btn->setMinSize({ num_button_width, num_button_width });
			btn->setPosition((top_left
			                  + mk::math::Vector2f(
								  x * (num_button_width + padding), y * (num_button_width + padding)
							  ))
			                     .as<sf::Vector2f>());

		}
	}
}
