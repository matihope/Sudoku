#include "GUI/Button.hpp"
#include "entities/SudokuScene.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include <iostream>

class Menu: public mk::WorldEntity {
public:
	void onReady(mk::Game& game) override {
		// std::array<std::pair<std::string, sudoku::SudokuGame::Difficulty>, 2> button_things{
		// 	{ "Easy", sudoku::SudokuGame::Difficulty::EASY },
		// 	{ "Normal", sudoku::SudokuGame::Difficulty::NORMAL }
		// };

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
	std::cout << "Looped: " << sudoku::getLoopCounter() << "\n";
	game.run();
}
