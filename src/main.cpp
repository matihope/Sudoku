#include "GUI/Button.hpp"
#include "SudokuGame.hpp"
#include "entities/SudokuSceneEntity.hpp"
#include <iostream>
#include <memory>

class Menu: public mk::WorldEntity {
public:
	void onReady(mk::Game& game) override {
		std::array button_presets = {
			sudoku::SudokuGame::Difficulty::EASY, sudoku::SudokuGame::Difficulty::NORMAL,
			sudoku::SudokuGame::Difficulty::HARD, sudoku::SudokuGame::Difficulty::EXPERT,
			sudoku::SudokuGame::Difficulty::NONE, sudoku::SudokuGame::Difficulty::EMPTY,
		};

		std::optional<mk::gui::Button*> previous_button;
		for (auto difficulty: button_presets) {
			auto button_text = sudoku::difficultyToString(difficulty);
			buttons.push_back({ nullptr, difficulty });
			auto&& button = buttons.back().first;
			button        = addChild<mk::gui::Button>(game, game.getDefaultFont(), button_text);
			button->setAlignment(mk::gui::HAlignment::MIDDLE, mk::gui::VAlignment::TOP);
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
	std::vector<std::pair<mk::gui::Button*, sudoku::SudokuGame::Difficulty>> buttons;
};

int main() {
	mk::Game game = mk::Game("settings.json");
	game.getDefaultFont()->setSmooth(true);
	game.addScene(std::make_unique<Menu>());
	game.run();
}
