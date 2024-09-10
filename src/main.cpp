#include "GUI/Button.hpp"
#include "SudokuGame.hpp"
#include "entities/SudokuSceneEntity.hpp"
#include <iostream>
#include <memory>

class Menu: public mk::WorldEntity {
public:
	void onReady(mk::Game& game) override {
		std::array button_presets = {
			std::make_pair("Easy", sudoku::SudokuGame::Difficulty::EASY),
			std::make_pair("Normal", sudoku::SudokuGame::Difficulty::NORMAL),
			std::make_pair("Hard", sudoku::SudokuGame::Difficulty::HARD),
			std::make_pair("Expert", sudoku::SudokuGame::Difficulty::EXPERT),
			std::make_pair("Full board", sudoku::SudokuGame::Difficulty::NONE),
			std::make_pair("Empty board", sudoku::SudokuGame::Difficulty::EMPTY),
		};

		std::optional<mk::GUI::Button*> previous_button;
		for (auto&& [button_text, difficulty]: button_presets) {
			buttons.push_back({ nullptr, difficulty });
			auto&& button = buttons.back().first;
			button        = addChild<mk::GUI::Button>(game, game.getDefaultFont(), button_text);
			button->setAlignment(mk::GUI::HAlignment::MIDDLE, mk::GUI::VAlignment::TOP);
			button->setMinSize({ game.getViewportSize().x / 3.f, 50.f });
			button->setMinSpaceBetween({ 5, 5 });
			float button_y = game.getViewportSize().y / 3.;
			if (previous_button.has_value())
				button_y = previous_button.value()->getPosition().y
				         + previous_button.value()->getBounds().height + 10.;
			button->setPosition(game.getViewportSize().x / 2., button_y);
			previous_button = buttons.back().first;
		}
	}

	void onUpdate(mk::Game& game, float dt) override {
		for (auto&& [btn, difficulty]: buttons)
			if (btn->isPressed()) game.addScene(std::make_unique<SudokuScene>(difficulty));
	}

private:
	std::vector<std::pair<mk::GUI::Button*, sudoku::SudokuGame::Difficulty>> buttons;
};

int main() {
	mk::Game game = mk::Game("settings.json");
	game.getDefaultFont()->setSmooth(true);
	game.addScene(std::make_unique<Menu>());
	game.run();
}
